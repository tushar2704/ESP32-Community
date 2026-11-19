/**
 * ESP32-Community: High-Speed MicroSD Data Logger & Web File Server
 * Logs multi-channel environmental & system telemetry to FAT32 SD card in CSV format,
 * with web-based file browser, live record monitor, and 1-click CSV download.
 *
 * Compatible with ESP32-WROOM-32, ESP32-S3, ESP32-C3.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <SD.h>
#include <ArduinoJson.h>

// --- Pin Definitions ---
#if defined(CONFIG_IDF_TARGET_ESP32C3)
  #define SD_CS   7
  #define SD_MOSI 6
  #define SD_MISO 5
  #define SD_SCK  4
  #define PIN_LED 8
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
  #define SD_CS   10
  #define SD_MOSI 11
  #define SD_MISO 13
  #define SD_SCK  12
  #define PIN_LED 2
#else
  #define SD_CS   5
  #define SD_MOSI 23
  #define SD_MISO 19
  #define SD_SCK  18
  #define PIN_LED 2
#endif

const char* default_ssid = "YOUR_WIFI_SSID";
const char* default_password = "YOUR_WIFI_PASSWORD";
const char* LOG_FILENAME = "/telemetry.csv";

WebServer server(80);

bool sdCardPresent = false;
uint64_t totalCardBytes = 0;
uint64_t usedCardBytes = 0;
unsigned long recordsLogged = 0;
unsigned long lastLogTime = 0;
const unsigned long LOG_INTERVAL_MS = 2000;

void appendTelemetryRecord() {
  if (!sdCardPresent) return;

  File file = SD.open(LOG_FILENAME, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open log file for appending!");
    return;
  }

  // Telemetry row: Timestamp(ms), Temperature(C), FreeHeap(Bytes), RSSI(dBm)
  unsigned long nowMs = millis();
  float tempC = temperatureRead();
  uint32_t heap = ESP.getFreeHeap();
  int rssi = WiFi.RSSI();

  file.printf("%lu,%.2f,%u,%d\n", nowMs, tempC, (unsigned int)heap, rssi);
  file.close();

  recordsLogged++;
  digitalWrite(PIN_LED, HIGH);
  delayMicroseconds(2500);
  digitalWrite(PIN_LED, LOW);
}

// --- Web Interface ---
const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 MicroSD Data Logger</title>
  <style>
    :root {
      --bg: #090d16;
      --card: #131c2e;
      --border: #1e293b;
      --accent: #38bdf8;
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
      max-width: 480px;
      width: 100%;
      text-align: center;
      box-shadow: 0 20px 25px -5px rgba(0, 0, 0, 0.4);
    }
    h1 { font-size: 1.6rem; margin-top: 0; }
    .metric {
      font-size: 3rem;
      font-weight: 800;
      color: var(--accent);
      margin: 10px 0;
    }
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
    .actions { display: flex; flex-direction: column; gap: 10px; }
    .btn {
      padding: 14px;
      font-size: 1rem;
      font-weight: 700;
      border: none;
      border-radius: 8px;
      cursor: pointer;
      text-decoration: none;
      display: inline-block;
      text-align: center;
    }
    .btn-dl { background: var(--accent); color: #090d16; }
    .btn-clear { background: #334155; color: #f8fafc; }
  </style>
</head>
<body>
  <div class="card">
    <h1>💾 MicroSD Data Logger</h1>
    <div class="metric" id="records">0</div>
    <div style="color:var(--text-muted); font-size:0.9rem; margin-bottom:16px;">RECORDS WRITTEN TO CSV</div>

    <div class="grid">
      <div class="box">
        <div class="lbl">SD Status</div>
        <div class="val" id="sdStatus">Detecting...</div>
      </div>
      <div class="box">
        <div class="lbl">Card Size</div>
        <div class="val" id="cardSize">- MB</div>
      </div>
      <div class="box">
        <div class="lbl">Log File</div>
        <div class="val">telemetry.csv</div>
      </div>
      <div class="box">
        <div class="lbl">Sample Rate</div>
        <div class="val">Every 2.0s</div>
      </div>
    </div>

    <div class="actions">
      <a href="/download" class="btn btn-dl">📥 DOWNLOAD TELEMETRY CSV</a>
      <button class="btn btn-clear" onclick="clearLogs()">🗑️ Clear Log File</button>
    </div>
  </div>
  <script>
    function refresh() {
      fetch('/api/status')
        .then(r => r.json())
        .then(d => {
          document.getElementById('records').innerText = d.recordsLogged;
          document.getElementById('sdStatus').innerText = d.sdReady ? 'MOUNTED ✅' : 'NO CARD ❌';
          document.getElementById('cardSize').innerText = d.cardSizeMB + ' MB';
        });
    }
    function clearLogs() {
      if (confirm('Erase all records in telemetry.csv?')) {
        fetch('/api/clear', { method: 'POST' }).then(() => refresh());
      }
    }
    setInterval(refresh, 2000);
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
  doc["sdReady"] = sdCardPresent;
  doc["cardSizeMB"] = sdCardPresent ? (uint32_t)(totalCardBytes / (1024 * 1024)) : 0;
  doc["recordsLogged"] = recordsLogged;
  doc["uptime"] = millis();

  String out;
  serializeJson(doc, out);
  server.send(200, "application/json", out);
}

void handleDownload() {
  if (!sdCardPresent || !SD.exists(LOG_FILENAME)) {
    server.send(404, "text/plain", "Log file not found or SD card not mounted.");
    return;
  }

  File file = SD.open(LOG_FILENAME, FILE_READ);
  if (!file) {
    server.send(500, "text/plain", "Failed to open file for reading.");
    return;
  }

  server.sendHeader("Content-Disposition", "attachment; filename=\"telemetry.csv\"");
  server.streamFile(file, "text/csv");
  file.close();
}

void handleClear() {
  if (sdCardPresent) {
    SD.remove(LOG_FILENAME);
    File file = SD.open(LOG_FILENAME, FILE_WRITE);
    if (file) {
      file.println("timestamp_ms,temperature_c,free_heap_bytes,wifi_rssi_dbm");
      file.close();
      recordsLogged = 0;
    }
  }
  server.send(200, "application/json", "{\"success\":true}");
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n[ESP32 MicroSD Data Logger]");

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  // Initialize SPI & SD Card
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS)) {
    Serial.println("❌ SD Card initialization failed! Check wiring & card format (FAT32).");
    sdCardPresent = false;
  } else {
    sdCardPresent = true;
    totalCardBytes = SD.cardSize();
    usedCardBytes = SD.usedBytes();
    Serial.printf("✅ SD Card mounted successfully! Size: %llu MB\n", totalCardBytes / (1024 * 1024));

    // Initialize CSV header if file does not exist
    if (!SD.exists(LOG_FILENAME)) {
      File file = SD.open(LOG_FILENAME, FILE_WRITE);
      if (file) {
        file.println("timestamp_ms,temperature_c,free_heap_bytes,wifi_rssi_dbm");
        file.close();
        Serial.println("Created fresh telemetry.csv with headers.");
      }
    }
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
    Serial.println("\nWiFi timeout; launching fallback AP: ESP32-DataLogger");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP32-DataLogger", "logger1234");
    Serial.printf("AP IP: %s\n", WiFi.softAPIP().toString().c_str());
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/status", HTTP_GET, handleApiStatus);
  server.on("/download", HTTP_GET, handleDownload);
  server.on("/api/clear", HTTP_POST, handleClear);
  server.begin();
  Serial.println("Web server running.");
}

void loop() {
  server.handleClient();

  // Log telemetry every 2 seconds
  if (millis() - lastLogTime >= LOG_INTERVAL_MS) {
    lastLogTime = millis();
    appendTelemetryRecord();
  }

  delay(10);
}
