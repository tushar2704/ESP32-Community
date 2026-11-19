/**
 * ESP32-Community: Smart AC Power & Energy Monitor
 * Non-invasive current sensing using SCT-013 CT clamp with true RMS sampling,
 * live Web dashboard gauge, cumulative kWh accumulator, and REST API.
 *
 * Compatible with ESP32-WROOM-32, ESP32-S3, ESP32-C3.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// --- Pin Definitions ---
#if defined(CONFIG_IDF_TARGET_ESP32C3)
  #define PIN_ADC_CURRENT 2
  #define PIN_STATUS_LED  8
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
  #define PIN_ADC_CURRENT 4
  #define PIN_STATUS_LED  2
#else
  #define PIN_ADC_CURRENT 34 // ADC1 pin (compatible with WiFi)
  #define PIN_STATUS_LED  2
#endif

// --- Energy Monitor Calibration Constants ---
const float MAINS_VOLTAGE = 230.0;    // Mains RMS voltage (230V EU/UK/Asia or 120V US)
const float CT_CALIBRATION = 30.0;    // Calibration factor for SCT-013-030 (or SCT-013-000 with burden)
const float COST_PER_KWH = 0.15;      // Electricity tariff rate ($ / kWh)

// WiFi Credentials
const char* default_ssid = "YOUR_WIFI_SSID";
const char* default_password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

// Measurement Variables
float currentRMS = 0.0;
float activePower = 0.0;
float cumulativeKWh = 0.0;
float totalCost = 0.0;
unsigned long lastMeasureTime = 0;
unsigned long sampleIntervalMs = 1000;

// Calculate True RMS Current over AC cycles
float calculateRMSCurrent(int numSamples = 1500) {
  uint32_t adcSum = 0;
  for (int i = 0; i < 200; i++) {
    adcSum += analogRead(PIN_ADC_CURRENT);
    delayMicroseconds(50);
  }
  float midPoint = adcSum / 200.0f; // Dynamic DC bias offset

  double sumSq = 0;
  for (int i = 0; i < numSamples; i++) {
    int raw = analogRead(PIN_ADC_CURRENT);
    float centered = (raw - midPoint) * (3.3f / 4095.0f);
    float currentInst = centered * CT_CALIBRATION;
    sumSq += (currentInst * currentInst);
    delayMicroseconds(80); // ~12 kHz sample rate
  }

  float rms = sqrt(sumSq / numSamples);
  if (rms < 0.08f) rms = 0.0f; // Noise gate filter for idle state
  return rms;
}

// --- Web Dashboard UI ---
const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Smart Energy Monitor</title>
  <style>
    :root {
      --bg: #0d1117;
      --card: #161b22;
      --border: #30363d;
      --accent: #58a6ff;
      --green: #2ea043;
      --yellow: #f1e05a;
      --text: #c9d1d9;
    }
    body {
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
      background: var(--bg);
      color: var(--text);
      display: flex;
      flex-direction: column;
      align-items: center;
      margin: 0;
      padding: 24px;
    }
    .card {
      background: var(--card);
      border: 1px solid var(--border);
      border-radius: 16px;
      padding: 28px;
      max-width: 480px;
      width: 100%;
      text-align: center;
      box-shadow: 0 10px 30px rgba(0,0,0,0.5);
    }
    h1 { margin-top: 0; font-size: 1.6rem; color: #fff; }
    .metric-main {
      font-size: 3.2rem;
      font-weight: 800;
      color: var(--accent);
      margin: 12px 0 4px 0;
    }
    .metric-unit { font-size: 1.2rem; color: #8b949e; }
    .grid {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 16px;
      margin-top: 24px;
      text-align: left;
    }
    .stat-box {
      background: #21262d;
      padding: 16px;
      border-radius: 12px;
      border: 1px solid var(--border);
    }
    .stat-label { font-size: 0.8rem; color: #8b949e; text-transform: uppercase; }
    .stat-val { font-size: 1.4rem; font-weight: 700; color: #fff; margin-top: 6px; }
    .btn-reset {
      margin-top: 24px;
      background: #21262d;
      color: #8b949e;
      border: 1px solid var(--border);
      border-radius: 8px;
      padding: 10px 20px;
      font-weight: 600;
      cursor: pointer;
      transition: background 0.2s;
    }
    .btn-reset:hover { background: #30363d; color: #fff; }
  </style>
</head>
<body>
  <div class="card">
    <h1>⚡ Live Power Meter</h1>
    <div class="metric-main" id="power">0.0</div>
    <div class="metric-unit">WATTS (W)</div>

    <div class="grid">
      <div class="stat-box">
        <div class="stat-label">RMS Current</div>
        <div class="stat-val" id="current">0.00 A</div>
      </div>
      <div class="stat-box">
        <div class="stat-label">Grid Voltage</div>
        <div class="stat-val" id="voltage">230 V</div>
      </div>
      <div class="stat-box">
        <div class="stat-label">Energy Consumed</div>
        <div class="stat-val" id="kwh">0.000 kWh</div>
      </div>
      <div class="stat-box">
        <div class="stat-label">Estimated Cost</div>
        <div class="stat-val" id="cost">$ 0.00</div>
      </div>
    </div>

    <button class="btn-reset" onclick="resetAccumulator()">Reset Energy Counter</button>
  </div>
  <script>
    function updateMetrics() {
      fetch('/api/power')
        .then(r => r.json())
        .then(d => {
          document.getElementById('power').innerText = d.powerWatts.toFixed(1);
          document.getElementById('current').innerText = d.currentRMS.toFixed(2) + ' A';
          document.getElementById('voltage').innerText = d.mainsVoltage.toFixed(0) + ' V';
          document.getElementById('kwh').innerText = d.energyKWh.toFixed(4) + ' kWh';
          document.getElementById('cost').innerText = '$ ' + d.estimatedCost.toFixed(3);
        })
        .catch(err => console.error(err));
    }
    function resetAccumulator() {
      if (confirm('Reset cumulative kWh energy counter?')) {
        fetch('/api/reset', { method: 'POST' }).then(() => updateMetrics());
      }
    }
    setInterval(updateMetrics, 1200);
    updateMetrics();
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", HTML_PAGE);
}

void handleApiPower() {
  StaticJsonDocument<256> doc;
  doc["currentRMS"] = currentRMS;
  doc["powerWatts"] = activePower;
  doc["mainsVoltage"] = MAINS_VOLTAGE;
  doc["energyKWh"] = cumulativeKWh;
  doc["estimatedCost"] = totalCost;
  doc["uptime"] = millis();

  String out;
  serializeJson(doc, out);
  server.send(200, "application/json", out);
}

void handleApiReset() {
  cumulativeKWh = 0.0;
  totalCost = 0.0;
  server.send(200, "application/json", "{\"success\":true,\"message\":\"Energy counter reset.\"}");
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n[ESP32 Smart AC Energy Monitor]");

  pinMode(PIN_STATUS_LED, OUTPUT);
  digitalWrite(PIN_STATUS_LED, LOW);

  analogReadResolution(12);

  WiFi.mode(WIFI_STA);
  WiFi.begin(default_ssid, default_password);
  Serial.printf("Connecting to %s...", default_ssid);
  
  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 8000) {
    delay(250);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\nConnected! IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\nWiFi connection failed; creating fallback AP: ESP32-EnergyMonitor");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP32-EnergyMonitor", "power1234");
    Serial.printf("AP IP: %s\n", WiFi.softAPIP().toString().c_str());
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/power", HTTP_GET, handleApiPower);
  server.on("/api/reset", HTTP_POST, handleApiReset);
  server.begin();
  Serial.println("Web server online.");

  lastMeasureTime = millis();
}

void loop() {
  server.handleClient();

  unsigned long now = millis();
  if (now - lastMeasureTime >= sampleIntervalMs) {
    unsigned long deltaMs = now - lastMeasureTime;
    lastMeasureTime = now;

    digitalWrite(PIN_STATUS_LED, HIGH);
    currentRMS = calculateRMSCurrent(1000);
    activePower = currentRMS * MAINS_VOLTAGE;

    // Accumulate kWh: (Watts * hours) / 1000
    float deltaHours = (float)deltaMs / (1000.0f * 3600.0f);
    cumulativeKWh += (activePower * deltaHours) / 1000.0f;
    totalCost = cumulativeKWh * COST_PER_KWH;

    digitalWrite(PIN_STATUS_LED, LOW);

    Serial.printf("Current: %.2f A | Power: %.1f W | Total: %.4f kWh ($%.3f)\n",
                  currentRMS, activePower, cumulativeKWh, totalCost);
  }

  delay(10);
}
