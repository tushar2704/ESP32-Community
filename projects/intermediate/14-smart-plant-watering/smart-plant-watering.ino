/**
 * ESP32-Community: Automated Smart Plant Watering System
 * Features capacitive soil moisture sensing, OLED status display,
 * 5V pump relay driver with safety limits, and glassmorphism web dashboard.
 *
 * Compatible with ESP32-WROOM-32, ESP32-S3, ESP32-C3.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>

// --- Pin Definitions ---
#if defined(CONFIG_IDF_TARGET_ESP32C3)
  #define PIN_SOIL_ADC   0
  #define PIN_PUMP_RELAY 3
  #define I2C_SDA        4
  #define I2C_SCL        5
  #define PIN_LED        8
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
  #define PIN_SOIL_ADC   4
  #define PIN_PUMP_RELAY 10
  #define I2C_SDA        8
  #define I2C_SCL        9
  #define PIN_LED        2
#else
  #define PIN_SOIL_ADC   34  // ADC1 channel
  #define PIN_PUMP_RELAY 26
  #define I2C_SDA        21
  #define I2C_SCL        22
  #define PIN_LED        2
#endif

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
WebServer server(80);

// Calibration for Capacitive Soil Moisture Sensor v1.2
const int AIR_VALUE = 3200;    // Dry air sensor value
const int WATER_VALUE = 1400;  // Submerged in water sensor value

// Configuration & States
const char* default_ssid = "YOUR_WIFI_SSID";
const char* default_password = "YOUR_WIFI_PASSWORD";

int moisturePercent = 50;
int rawADC = 2300;
int triggerThreshold = 35;     // Auto-water if moisture drops below this %
bool autoWateringEnabled = true;
bool pumpActive = false;
unsigned long pumpStartTime = 0;
const unsigned long PUMP_DURATION_MS = 3500; // 3.5s pump burst
unsigned long lastWateredTime = 0;
int wateringCyclesCount = 0;
unsigned long lastSensorReadTime = 0;

void triggerPump() {
  if (pumpActive) return;
  digitalWrite(PIN_PUMP_RELAY, HIGH);
  digitalWrite(PIN_LED, HIGH);
  pumpActive = true;
  pumpStartTime = millis();
  wateringCyclesCount++;
  lastWateredTime = millis();
  Serial.println("💧 Water pump engaged!");
}

void updateOLED() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.printf("Smart Flora: %s", pumpActive ? "PUMPING!" : "ONLINE");
  display.drawFastHLine(0, 9, 128, SSD1306_WHITE);

  display.setCursor(0, 14);
  display.printf("Moisture: %d%%", moisturePercent);

  // Moisture Bar
  display.drawRect(0, 26, 128, 12, SSD1306_WHITE);
  int barWidth = map(constrain(moisturePercent, 0, 100), 0, 100, 0, 124);
  display.fillRect(2, 28, barWidth, 8, SSD1306_WHITE);

  display.setCursor(0, 42);
  const char* statusStr = (moisturePercent < triggerThreshold) ? "Status: THIRSTY ⚠️" :
                          (moisturePercent > 70) ? "Status: SATURATED" : "Status: OPTIMAL ✅";
  display.print(statusStr);

  display.setCursor(0, 54);
  if (lastWateredTime == 0) {
    display.print("Watered: Never");
  } else {
    unsigned long secAgo = (millis() - lastWateredTime) / 1000;
    display.printf("Watered: %lum ago", secAgo / 60);
  }

  display.display();
}

// --- Web Dashboard UI ---
const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Smart Plant Care</title>
  <style>
    :root {
      --bg: #090d16;
      --card: #131c2e;
      --border: #1e293b;
      --accent: #10b981;
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
      max-width: 440px;
      width: 100%;
      text-align: center;
      box-shadow: 0 20px 25px -5px rgba(0, 0, 0, 0.4);
    }
    h1 { font-size: 1.6rem; margin-top: 0; }
    .gauge {
      font-size: 3.5rem;
      font-weight: 800;
      color: var(--accent);
      margin: 10px 0;
    }
    .status-text { font-size: 1.1rem; font-weight: 600; margin-bottom: 20px; }
    .grid {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 12px;
      margin-bottom: 24px;
      text-align: left;
    }
    .box {
      background: #0b1322;
      padding: 12px;
      border-radius: 10px;
      border: 1px solid var(--border);
    }
    .lbl { font-size: 0.75rem; color: var(--text-muted); text-transform: uppercase; }
    .val { font-size: 1.1rem; font-weight: 700; margin-top: 4px; }
    .btn {
      width: 100%;
      padding: 14px;
      font-size: 1.1rem;
      font-weight: 700;
      border: none;
      border-radius: 8px;
      background: var(--accent);
      color: #090d16;
      cursor: pointer;
      margin-bottom: 10px;
    }
    .btn:active { opacity: 0.8; }
  </style>
</head>
<body>
  <div class="card">
    <h1>🌱 Smart Flora Sentinel</h1>
    <div class="gauge" id="moistVal">50%</div>
    <div class="status-text" id="statusDesc">Checking...</div>

    <div class="grid">
      <div class="box">
        <div class="lbl">Pump State</div>
        <div class="val" id="pumpVal">IDLE</div>
      </div>
      <div class="box">
        <div class="lbl">Auto Watering</div>
        <div class="val" id="autoVal">ENABLED</div>
      </div>
      <div class="box">
        <div class="lbl">Cycles Run</div>
        <div class="val" id="cyclesVal">0</div>
      </div>
      <div class="box">
        <div class="lbl">Raw Sensor</div>
        <div class="val" id="rawVal">2300</div>
      </div>
    </div>

    <button class="btn" onclick="waterNow()">💧 WATER PLANT NOW</button>
  </div>
  <script>
    function refresh() {
      fetch('/api/status')
        .then(r => r.json())
        .then(d => {
          document.getElementById('moistVal').innerText = d.moisturePercent + '%';
          document.getElementById('statusDesc').innerText = d.soilStatus;
          document.getElementById('pumpVal').innerText = d.pumpActive ? 'PUMPING 💧' : 'IDLE';
          document.getElementById('autoVal').innerText = d.autoWatering ? 'ENABLED ✅' : 'DISABLED ❌';
          document.getElementById('cyclesVal').innerText = d.cycles;
          document.getElementById('rawVal').innerText = d.rawADC;
        });
    }
    function waterNow() {
      fetch('/api/water', { method: 'POST' }).then(() => refresh());
    }
    setInterval(refresh, 1500);
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
  doc["moisturePercent"] = moisturePercent;
  doc["rawADC"] = rawADC;
  doc["soilStatus"] = (moisturePercent < triggerThreshold) ? "THIRSTY ⚠️" :
                      (moisturePercent > 70) ? "SATURATED" : "OPTIMAL ✅";
  doc["pumpActive"] = pumpActive;
  doc["autoWatering"] = autoWateringEnabled;
  doc["cycles"] = wateringCyclesCount;
  doc["uptime"] = millis();

  String out;
  serializeJson(doc, out);
  server.send(200, "application/json", out);
}

void handleApiWater() {
  triggerPump();
  server.send(200, "application/json", "{\"success\":true,\"message\":\"Pump triggered.\"}");
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n[ESP32 Smart Plant Watering System]");

  pinMode(PIN_PUMP_RELAY, OUTPUT);
  digitalWrite(PIN_PUMP_RELAY, LOW);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  Wire.begin(I2C_SDA, I2C_SCL);
  if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 24);
    display.println("Smart Plant Care");
    display.display();
  }

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
    Serial.println("\nWiFi timeout; creating fallback AP: ESP32-SmartFlora");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP32-SmartFlora", "flora1234");
    Serial.printf("AP IP: %s\n", WiFi.softAPIP().toString().c_str());
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/status", HTTP_GET, handleApiStatus);
  server.on("/api/water", HTTP_POST, handleApiWater);
  server.begin();
  Serial.println("Web server running.");
}

void loop() {
  server.handleClient();

  // Read Soil Sensor every 1 second
  if (millis() - lastSensorReadTime >= 1000) {
    lastSensorReadTime = millis();
    rawADC = analogRead(PIN_SOIL_ADC);
    moisturePercent = constrain(map(rawADC, AIR_VALUE, WATER_VALUE, 0, 100), 0, 100);

    // Auto-water if enabled and dry
    if (autoWateringEnabled && !pumpActive && (moisturePercent < triggerThreshold)) {
      // Cooldown: at least 60 seconds between automated waterings
      if (millis() - lastWateredTime >= 60000UL) {
        Serial.printf("Soil dry (%d%% < %d%% threshold). Initiating watering cycle.\n",
                      moisturePercent, triggerThreshold);
        triggerPump();
      }
    }

    updateOLED();
  }

  // Handle Pump auto-shutoff safety timer
  if (pumpActive && (millis() - pumpStartTime >= PUMP_DURATION_MS)) {
    digitalWrite(PIN_PUMP_RELAY, LOW);
    digitalWrite(PIN_LED, LOW);
    pumpActive = false;
    Serial.println("Watering burst finished.");
  }

  delay(20);
}
