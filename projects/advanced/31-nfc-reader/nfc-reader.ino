/**
 * ESP32-Community: RFID / NFC Smart Access Control System
 * MFRC522 SPI reader with NVS persistent card authorization,
 * door strike relay control, audible buzzers, and web management dashboard.
 *
 * Compatible with ESP32-WROOM-32, ESP32-S3, ESP32-C3.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Preferences.h>
#include <ArduinoJson.h>

// --- Pin Definitions ---
#if defined(CONFIG_IDF_TARGET_ESP32C3)
  #define RC522_SS    7
  #define RC522_RST   3
  #define RC522_SCK   4
  #define RC522_MOSI  6
  #define RC522_MISO  5
  #define PIN_RELAY   2
  #define PIN_BUZZER  1
  #define PIN_LED     8
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
  #define RC522_SS    10
  #define RC522_RST   9
  #define RC522_SCK   12
  #define RC522_MOSI  11
  #define RC522_MISO  13
  #define PIN_RELAY   4
  #define PIN_BUZZER  5
  #define PIN_LED     2
#else
  #define RC522_SS    5
  #define RC522_RST   22
  #define RC522_SCK   18
  #define RC522_MOSI  23
  #define RC522_MISO  19
  #define PIN_RELAY   26
  #define PIN_BUZZER  25
  #define PIN_LED     2
#endif

MFRC522 mfrc522(RC522_SS, RC522_RST);
Preferences prefs;
WebServer server(80);

const char* default_ssid = "YOUR_WIFI_SSID";
const char* default_password = "YOUR_WIFI_PASSWORD";

// Access State
bool relayUnlocked = false;
unsigned long unlockTime = 0;
const unsigned long UNLOCK_DURATION_MS = 3000;
String lastScannedUID = "None";
bool lastAccessGranted = false;
int accessGrantedCount = 0;
int accessDeniedCount = 0;

// Hardcoded Master Card UID (change to your master card)
const String MASTER_UID = "A1B2C3D4";

void beep(int count, int durationMs, int pauseMs = 50) {
  for (int i = 0; i < count; i++) {
    digitalWrite(PIN_BUZZER, HIGH);
    delay(durationMs);
    digitalWrite(PIN_BUZZER, LOW);
    if (i < count - 1) delay(pauseMs);
  }
}

void grantAccess() {
  relayUnlocked = true;
  unlockTime = millis();
  lastAccessGranted = true;
  accessGrantedCount++;
  digitalWrite(PIN_RELAY, HIGH);
  digitalWrite(PIN_LED, HIGH);
  beep(2, 70, 60);
  Serial.println("🔓 Access GRANTED! Relay pulsed.");
}

void denyAccess() {
  lastAccessGranted = false;
  accessDeniedCount++;
  digitalWrite(PIN_LED, HIGH);
  digitalWrite(PIN_BUZZER, HIGH);
  delay(400);
  digitalWrite(PIN_BUZZER, LOW);
  digitalWrite(PIN_LED, LOW);
  Serial.println("⛔ Access DENIED!");
}

String getUIDString(MFRC522::Uid *uid) {
  String res = "";
  for (byte i = 0; i < uid->size; i++) {
    if (uid->uidByte[i] < 0x10) res += "0";
    res += String(uid->uidByte[i], HEX);
  }
  res.toUpperCase();
  return res;
}

// --- Web UI ---
const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 RFID Access Controller</title>
  <style>
    :root {
      --bg: #090d16;
      --card: #131c2e;
      --border: #1e293b;
      --accent: #38bdf8;
      --green: #22c55e;
      --red: #ef4444;
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
    h1 { font-size: 1.5rem; margin-top: 0; }
    .status-badge {
      padding: 12px;
      border-radius: 10px;
      font-size: 1.15rem;
      font-weight: 700;
      margin: 16px 0;
    }
    .badge-locked { background: rgba(51,65,85,0.4); color: #cbd5e1; border: 1px solid var(--border); }
    .badge-unlocked { background: rgba(34,197,94,0.2); color: var(--green); border: 1px solid var(--green); }
    .grid {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 12px;
      margin: 20px 0;
      text-align: left;
    }
    .box {
      background: #0b1322;
      padding: 12px;
      border-radius: 10px;
      border: 1px solid var(--border);
    }
    .lbl { font-size: 0.75rem; color: var(--text-muted); text-transform: uppercase; }
    .val { font-size: 1.15rem; font-weight: 700; margin-top: 4px; }
    .btn {
      width: 100%;
      padding: 14px;
      font-size: 1.1rem;
      font-weight: 700;
      border: none;
      border-radius: 8px;
      cursor: pointer;
      background: var(--accent);
      color: #090d16;
    }
  </style>
</head>
<body>
  <div class="card">
    <h1>🛡️ RFID Access Controller</h1>
    <div id="badge" class="status-badge badge-locked">DOOR LOCKED 🔒</div>

    <div class="grid">
      <div class="box">
        <div class="lbl">Last Scanned UID</div>
        <div class="val" id="lastUid">None</div>
      </div>
      <div class="box">
        <div class="lbl">Last Event</div>
        <div class="val" id="lastEvent">-</div>
      </div>
      <div class="box">
        <div class="lbl">Granted Total</div>
        <div class="val" id="grantCount">0</div>
      </div>
      <div class="box">
        <div class="lbl">Denied Total</div>
        <div class="val" id="denyCount">0</div>
      </div>
    </div>

    <button class="btn" onclick="manualUnlock()">⚡ UNLOCK DOOR (3s)</button>
  </div>
  <script>
    function refresh() {
      fetch('/api/status')
        .then(r => r.json())
        .then(d => {
          const b = document.getElementById('badge');
          if (d.relayUnlocked) {
            b.innerText = 'DOOR UNLOCKED 🔓';
            b.className = 'status-badge badge-unlocked';
          } else {
            b.innerText = 'DOOR LOCKED 🔒';
            b.className = 'status-badge badge-locked';
          }
          document.getElementById('lastUid').innerText = d.lastUID;
          document.getElementById('lastEvent').innerText = d.lastGranted ? 'GRANTED ✅' : 'DENIED ⛔';
          document.getElementById('grantCount').innerText = d.granted;
          document.getElementById('denyCount').innerText = d.denied;
        });
    }
    function manualUnlock() {
      fetch('/api/unlock', { method: 'POST' }).then(() => refresh());
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
  doc["relayUnlocked"] = relayUnlocked;
  doc["lastUID"] = lastScannedUID;
  doc["lastGranted"] = lastAccessGranted;
  doc["granted"] = accessGrantedCount;
  doc["denied"] = accessDeniedCount;
  doc["uptime"] = millis();

  String out;
  serializeJson(doc, out);
  server.send(200, "application/json", out);
}

void handleApiUnlock() {
  grantAccess();
  server.send(200, "application/json", "{\"success\":true}");
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n[ESP32 RFID Access Controller]");

  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, LOW);

  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  // Initialize SPI & MFRC522
  SPI.begin(RC522_SCK, RC522_MISO, RC522_MOSI, RC522_SS);
  mfrc522.PCD_Init();
  delay(50);
  mfrc522.PCD_DumpVersionToSerial();

  // NVS Preferences for authorized tags
  prefs.begin("access", false);

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
    Serial.println("\nWiFi timeout; launching fallback AP: ESP32-RFIDAccess");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP32-RFIDAccess", "rfid1234");
    Serial.printf("AP IP: %s\n", WiFi.softAPIP().toString().c_str());
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/status", HTTP_GET, handleApiStatus);
  server.on("/api/unlock", HTTP_POST, handleApiUnlock);
  server.begin();
  Serial.println("Access control server ready.");
}

void loop() {
  server.handleClient();

  // Handle Relay timer
  if (relayUnlocked && (millis() - unlockTime >= UNLOCK_DURATION_MS)) {
    digitalWrite(PIN_RELAY, LOW);
    digitalWrite(PIN_LED, LOW);
    relayUnlocked = false;
    Serial.println("Door re-locked.");
  }

  // Check for RFID tag present
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    lastScannedUID = getUIDString(&(mfrc522.uid));
    Serial.printf("Card scanned: %s\n", lastScannedUID.c_str());

    // Check if card is Master or Authorized in NVS
    bool authorized = (lastScannedUID == MASTER_UID) || prefs.getBool(lastScannedUID.c_str(), false);

    if (authorized) {
      grantAccess();
    } else {
      denyAccess();
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(1000); // Debounce tag read
  }

  delay(20);
}
