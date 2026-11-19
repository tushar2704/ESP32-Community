/**
 * ESP32-Community: Smart Garage Door Opener & Monitor
 * Web UI, Magnetic Reed Sensor, Relay Pulse Trigger, Auto-Close Safety Alert
 *
 * Fully compatible with ESP32-WROOM-32, ESP32-S3, ESP32-C3.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// --- Pin Definitions ---
#if defined(CONFIG_IDF_TARGET_ESP32C3)
  #define PIN_RELAY       4
  #define PIN_REED_SWITCH 5
  #define PIN_BUZZER      6
  #define PIN_LED         8
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
  #define PIN_RELAY       10
  #define PIN_REED_SWITCH 11
  #define PIN_BUZZER      12
  #define PIN_LED         2
#else
  #define PIN_RELAY       26
  #define PIN_REED_SWITCH 27
  #define PIN_BUZZER      25
  #define PIN_LED         2
#endif

// --- Configuration ---
const char* default_ssid = "YOUR_WIFI_SSID";
const char* default_password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

// --- State Variables ---
bool doorIsOpen = false;
unsigned long doorLastChangeTime = 0;
bool relayActive = false;
unsigned long relayTriggerTime = 0;
const unsigned long RELAY_PULSE_MS = 600;

// Auto-Close Safety Feature
bool autoCloseEnabled = false;
unsigned long autoCloseDelayMs = 15 * 60 * 1000UL; // 15 minutes default
bool warningBeepActive = false;
unsigned long warningStartTime = 0;

void triggerRelayPulse() {
  digitalWrite(PIN_RELAY, HIGH);
  digitalWrite(PIN_LED, HIGH);
  relayActive = true;
  relayTriggerTime = millis();
}

void beepBuzzer(int count, int durationMs) {
  for (int i = 0; i < count; i++) {
    digitalWrite(PIN_BUZZER, HIGH);
    delay(durationMs);
    digitalWrite(PIN_BUZZER, LOW);
    if (i < count - 1) delay(durationMs);
  }
}

// --- HTML / UI Web Interface ---
const char HTML_INDEX[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Smart Garage Door</title>
  <style>
    :root {
      --bg: #0d1117;
      --card: #161b22;
      --border: #30363d;
      --accent: #58a6ff;
      --green: #2ea043;
      --red: #da3633;
      --text: #c9d1d9;
    }
    body {
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
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
      box-shadow: 0 10px 30px rgba(0,0,0,0.5);
    }
    h1 { margin-top: 0; font-size: 1.6rem; color: #fff; }
    .status-badge {
      display: inline-block;
      padding: 8px 20px;
      border-radius: 20px;
      font-size: 1.2rem;
      font-weight: 700;
      margin: 16px 0;
      text-transform: uppercase;
    }
    .badge-closed { background: rgba(46,160,67,0.2); color: var(--green); border: 1px solid var(--green); }
    .badge-open { background: rgba(218,54,51,0.2); color: var(--red); border: 1px solid var(--red); }
    .door-visual {
      width: 180px;
      height: 140px;
      margin: 16px auto;
      border: 4px solid var(--border);
      border-radius: 8px;
      position: relative;
      overflow: hidden;
      background: #21262d;
    }
    .door-shutter {
      position: absolute;
      top: 0;
      left: 0;
      right: 0;
      background: repeating-linear-gradient(180deg, #38414a 0px, #38414a 18px, #1c2128 19px, #1c2128 20px);
      transition: height 1s ease-in-out;
    }
    .door-closed { height: 100%; }
    .door-open { height: 18%; }
    .btn {
      background: var(--accent);
      color: #0d1117;
      border: none;
      border-radius: 10px;
      padding: 16px 32px;
      font-size: 1.15rem;
      font-weight: 700;
      cursor: pointer;
      width: 100%;
      box-sizing: border-box;
      margin-top: 14px;
      transition: opacity 0.2s;
    }
    .btn:active { opacity: 0.8; }
    .meta { margin-top: 20px; font-size: 0.85rem; color: #8b949e; }
  </style>
</head>
<body>
  <div class="card">
    <h1>Smart Garage Door</h1>
    <div id="badge" class="status-badge badge-closed">Checking...</div>
    <div class="door-visual">
      <div id="shutter" class="door-shutter door-closed"></div>
    </div>
    <p id="duration" style="color: #8b949e; font-size: 0.95rem;">Last changed: -</p>
    <button class="btn" onclick="triggerDoor()">TRIGGER DOOR</button>
    <div class="meta">
      <p id="rssi">WiFi Signal: -</p>
      <p id="uptime">Uptime: -</p>
    </div>
  </div>
  <script>
    function updateState() {
      fetch('/api/status')
        .then(r => r.json())
        .then(data => {
          const badge = document.getElementById('badge');
          const shutter = document.getElementById('shutter');
          if (data.doorOpen) {
            badge.innerText = 'DOOR OPEN';
            badge.className = 'status-badge badge-open';
            shutter.className = 'door-shutter door-open';
          } else {
            badge.innerText = 'DOOR CLOSED';
            badge.className = 'status-badge badge-closed';
            shutter.className = 'door-shutter door-closed';
          }
          const elapsedSec = Math.floor(data.elapsedSinceChange / 1000);
          const mins = Math.floor(elapsedSec / 60);
          const secs = elapsedSec % 60;
          document.getElementById('duration').innerText = `State held: ${mins}m ${secs}s`;
          document.getElementById('rssi').innerText = `WiFi Signal: ${data.wifiRSSI} dBm`;
          const upMin = Math.floor(data.uptime / 60000);
          document.getElementById('uptime').innerText = `ESP32 Uptime: ${upMin} minutes`;
        })
        .catch(err => console.error(err));
    }
    function triggerDoor() {
      fetch('/api/trigger', { method: 'POST' })
        .then(r => r.json())
        .then(data => {
          alert(data.message || 'Door pulsed!');
          setTimeout(updateState, 1500);
        });
    }
    setInterval(updateState, 3000);
    updateState();
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", HTML_INDEX);
}

void handleApiStatus() {
  StaticJsonDocument<256> doc;
  doc["doorOpen"] = doorIsOpen;
  doc["doorState"] = doorIsOpen ? "OPEN" : "CLOSED";
  doc["elapsedSinceChange"] = (millis() >= doorLastChangeTime) ? (millis() - doorLastChangeTime) : 0;
  doc["autoCloseEnabled"] = autoCloseEnabled;
  doc["autoCloseRemainingMs"] = (autoCloseEnabled && doorIsOpen) ? 
    ((millis() - doorLastChangeTime < autoCloseDelayMs) ? (autoCloseDelayMs - (millis() - doorLastChangeTime)) : 0) : 0;
  doc["wifiRSSI"] = WiFi.RSSI();
  doc["uptime"] = millis();

  String output;
  serializeJson(doc, output);
  server.send(200, "application/json", output);
}

void handleApiTrigger() {
  triggerRelayPulse();
  StaticJsonDocument<128> doc;
  doc["success"] = true;
  doc["message"] = "Relay pulse triggered for garage button.";
  String output;
  serializeJson(doc, output);
  server.send(200, "application/json", output);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n[ESP32 Smart Garage Door]");

  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, LOW);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  // Reed switch: LOW when magnet connects (closed), HIGH when separated (open)
  pinMode(PIN_REED_SWITCH, INPUT_PULLUP);
  doorIsOpen = (digitalRead(PIN_REED_SWITCH) == HIGH);
  doorLastChangeTime = millis();

  // Connect to WiFi
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
    Serial.println("\nWiFi connection failed; starting fallback AP: ESP32-GarageDoor");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP32-GarageDoor", "garage1234");
    Serial.printf("AP IP: %s\n", WiFi.softAPIP().toString().c_str());
  }

  // Web Server Routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/status", HTTP_GET, handleApiStatus);
  server.on("/api/trigger", HTTP_POST, handleApiTrigger);
  server.begin();
  Serial.println("HTTP Server started on port 80.");

  // Audible startup confirmation
  beepBuzzer(2, 80);
}

void loop() {
  server.handleClient();

  // Read reed sensor (LOW = Magnet in contact = CLOSED, HIGH = OPEN)
  bool currentOpen = (digitalRead(PIN_REED_SWITCH) == HIGH);
  if (currentOpen != doorIsOpen) {
    doorIsOpen = currentOpen;
    doorLastChangeTime = millis();
    Serial.printf("Door state changed: %s\n", doorIsOpen ? "OPEN" : "CLOSED");
    if (doorIsOpen) {
      beepBuzzer(1, 150); // Warning chirp on open
    } else {
      beepBuzzer(2, 100); // Dual confirmation beep on close
    }
  }

  // Handle Relay pulse deactivation
  if (relayActive && (millis() - relayTriggerTime >= RELAY_PULSE_MS)) {
    digitalWrite(PIN_RELAY, LOW);
    digitalWrite(PIN_LED, LOW);
    relayActive = false;
  }

  // Auto-close safety check
  if (autoCloseEnabled && doorIsOpen) {
    unsigned long openDuration = millis() - doorLastChangeTime;
    if (openDuration >= autoCloseDelayMs) {
      Serial.println("Auto-close threshold exceeded! Triggering pulse.");
      beepBuzzer(5, 200); // 5 warning beeps
      triggerRelayPulse();
      doorLastChangeTime = millis(); // Reset timer
    }
  }

  delay(10);
}
