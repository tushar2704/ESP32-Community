/**
 * ESP32-Community: Smart Water Leak Detector & Automatic Shutoff Controller
 * Monitors dual-probe conductivity/rain sensors, triggers audible siren,
 * energizes cutoff valve relay, and provides responsive glassmorphism web dashboard.
 *
 * Compatible with ESP32-WROOM-32, ESP32-S3, ESP32-C3.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// --- Pin Definitions ---
#if defined(CONFIG_IDF_TARGET_ESP32C3)
  #define PIN_SENSOR_ANALOG  2   // ADC1 channel
  #define PIN_SENSOR_DIGITAL 3   // Digital comparator threshold
  #define PIN_RELAY_VALVE    4   // Solenoid shutoff valve
  #define PIN_BUZZER         5   // Alarm buzzer
  #define PIN_LED            8   // Status LED
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
  #define PIN_SENSOR_ANALOG  4   // ADC1 channel
  #define PIN_SENSOR_DIGITAL 5
  #define PIN_RELAY_VALVE    10
  #define PIN_BUZZER         11
  #define PIN_LED            2
#else
  #define PIN_SENSOR_ANALOG  34  // ADC1 channel
  #define PIN_SENSOR_DIGITAL 35
  #define PIN_RELAY_VALVE    26
  #define PIN_BUZZER         25
  #define PIN_LED            2
#endif

// Configuration
const char* default_ssid = "YOUR_WIFI_SSID";
const char* default_password = "YOUR_WIFI_PASSWORD";
const int LEAK_ANALOG_THRESHOLD = 1800; // Lower reading = higher conductivity/water

WebServer server(80);

// State Variables
bool leakDetected = false;
bool alarmMuted = false;
bool valveClosed = false;
int rawAnalogValue = 4095;
unsigned long leakStartTime = 0;
unsigned long lastSirenToggle = 0;
bool sirenState = false;
int leakEventCount = 0;

// Web Dashboard
const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Smart Water Leak Detector</title>
  <style>
    :root {
      --bg: #090d16;
      --card: #131c2e;
      --border: #1e293b;
      --safe: #22c55e;
      --danger: #ef4444;
      --text: #f8fafc;
      --text-muted: #94a3b8;
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
      max-width: 460px;
      width: 100%;
      text-align: center;
      box-shadow: 0 20px 25px -5px rgba(0, 0, 0, 0.4);
    }
    h1 { font-size: 1.6rem; margin-top: 0; }
    .status-banner {
      padding: 16px;
      border-radius: 12px;
      font-size: 1.3rem;
      font-weight: 800;
      margin: 20px 0;
      text-transform: uppercase;
      letter-spacing: 1px;
    }
    .status-safe {
      background: rgba(34, 197, 94, 0.15);
      color: var(--safe);
      border: 1px solid var(--safe);
    }
    .status-danger {
      background: rgba(239, 68, 68, 0.2);
      color: var(--danger);
      border: 1px solid var(--danger);
      animation: pulse 1s infinite alternate;
    }
    @keyframes pulse {
      from { opacity: 0.7; }
      to { opacity: 1; transform: scale(1.02); }
    }
    .metrics {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 12px;
      margin-bottom: 24px;
      text-align: left;
    }
    .metric-box {
      background: #0b1322;
      padding: 14px;
      border-radius: 10px;
      border: 1px solid var(--border);
    }
    .metric-label { font-size: 0.75rem; color: var(--text-muted); text-transform: uppercase; }
    .metric-val { font-size: 1.25rem; font-weight: 700; margin-top: 4px; }
    .actions { display: flex; gap: 10px; }
    .btn {
      flex: 1;
      padding: 12px;
      font-size: 0.95rem;
      font-weight: 700;
      border: none;
      border-radius: 8px;
      cursor: pointer;
      transition: opacity 0.2s;
    }
    .btn:active { opacity: 0.8; }
    .btn-mute { background: #3b82f6; color: #fff; }
    .btn-reset { background: #334155; color: #f8fafc; }
  </style>
</head>
<body>
  <div class="card">
    <h1>💧 Water Leak Sentinel</h1>
    <div id="banner" class="status-banner status-safe">ALL DRY & SECURE</div>

    <div class="metrics">
      <div class="metric-box">
        <div class="metric-label">Sensor Level</div>
        <div class="metric-val" id="sensorVal">4095</div>
      </div>
      <div class="metric-box">
        <div class="metric-label">Water Valve</div>
        <div class="metric-val" id="valveStatus">OPEN ✅</div>
      </div>
      <div class="metric-box">
        <div class="metric-label">Alarm State</div>
        <div class="metric-val" id="alarmStatus">SILENT</div>
      </div>
      <div class="metric-box">
        <div class="metric-label">Incidents</div>
        <div class="metric-val" id="incidents">0</div>
      </div>
    </div>

    <div class="actions">
      <button class="btn btn-mute" onclick="muteAlarm()">Toggle Mute</button>
      <button class="btn btn-reset" onclick="resetValve()">Reset Valve</button>
    </div>
  </div>
  <script>
    function refresh() {
      fetch('/api/status')
        .then(r => r.json())
        .then(d => {
          const b = document.getElementById('banner');
          if (d.leakDetected) {
            b.innerText = '🚨 WATER LEAK DETECTED!';
            b.className = 'status-banner status-danger';
          } else {
            b.innerText = 'ALL DRY & SECURE';
            b.className = 'status-banner status-safe';
          }
          document.getElementById('sensorVal').innerText = d.rawAnalog;
          document.getElementById('valveStatus').innerText = d.valveClosed ? 'SHUT OFF 🔒' : 'OPEN ✅';
          document.getElementById('alarmStatus').innerText = d.alarmMuted ? 'MUTED 🔇' : (d.leakDetected ? 'SOUNDING 🔊' : 'STANDBY');
          document.getElementById('incidents').innerText = d.incidentCount;
        });
    }
    function muteAlarm() {
      fetch('/api/mute', { method: 'POST' }).then(() => refresh());
    }
    function resetValve() {
      fetch('/api/reset', { method: 'POST' }).then(() => refresh());
    }
    setInterval(refresh, 1000);
    refresh();
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", HTML_PAGE);
}

void handleApiStatus() {
  StaticJsonDocument<256> doc;
  doc["leakDetected"] = leakDetected;
  doc["rawAnalog"] = rawAnalogValue;
  doc["valveClosed"] = valveClosed;
  doc["alarmMuted"] = alarmMuted;
  doc["incidentCount"] = leakEventCount;
  doc["uptime"] = millis();

  String out;
  serializeJson(doc, out);
  server.send(200, "application/json", out);
}

void handleApiMute() {
  alarmMuted = !alarmMuted;
  if (alarmMuted) {
    digitalWrite(PIN_BUZZER, LOW);
  }
  server.send(200, "application/json", "{\"success\":true}");
}

void handleApiReset() {
  leakDetected = false;
  valveClosed = false;
  alarmMuted = false;
  digitalWrite(PIN_RELAY_VALVE, LOW); // De-energize shutoff valve
  digitalWrite(PIN_BUZZER, LOW);
  digitalWrite(PIN_LED, LOW);
  server.send(200, "application/json", "{\"success\":true,\"message\":\"Valve and system reset.\"}");
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n[ESP32 Smart Water Leak Sentinel]");

  pinMode(PIN_RELAY_VALVE, OUTPUT);
  digitalWrite(PIN_RELAY_VALVE, LOW);

  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_SENSOR_DIGITAL, INPUT);

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
    Serial.println("\nWiFi timeout; starting fallback AP: ESP32-LeakSentinel");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP32-LeakSentinel", "water1234");
    Serial.printf("AP IP: %s\n", WiFi.softAPIP().toString().c_str());
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/status", HTTP_GET, handleApiStatus);
  server.on("/api/mute", HTTP_POST, handleApiMute);
  server.on("/api/reset", HTTP_POST, handleApiReset);
  server.begin();
  Serial.println("Web server active.");
}

void loop() {
  server.handleClient();

  // Read analog moisture sensor (0 = Direct short/water, 4095 = Completely dry)
  rawAnalogValue = analogRead(PIN_SENSOR_ANALOG);
  bool digitalActive = (digitalRead(PIN_SENSOR_DIGITAL) == LOW);

  bool currentLeak = (rawAnalogValue < LEAK_ANALOG_THRESHOLD) || digitalActive;

  if (currentLeak && !leakDetected) {
    leakDetected = true;
    valveClosed = true;
    leakStartTime = millis();
    leakEventCount++;
    digitalWrite(PIN_RELAY_VALVE, HIGH); // Shut off main water line!
    Serial.printf("🚨 ALARM: Water leak detected! Raw Analog: %d. Solenoid Valve Closed!\n", rawAnalogValue);
  }

  // Handle active audible & visual alarm siren
  if (leakDetected) {
    if (!alarmMuted) {
      if (millis() - lastSirenToggle >= 250) {
        lastSirenToggle = millis();
        sirenState = !sirenState;
        digitalWrite(PIN_BUZZER, sirenState ? HIGH : LOW);
        digitalWrite(PIN_LED, sirenState ? HIGH : LOW);
      }
    } else {
      digitalWrite(PIN_BUZZER, LOW);
      digitalWrite(PIN_LED, HIGH); // Solid LED indicates muted active leak
    }
  } else {
    digitalWrite(PIN_LED, LOW);
    digitalWrite(PIN_BUZZER, LOW);
  }

  delay(20);
}
