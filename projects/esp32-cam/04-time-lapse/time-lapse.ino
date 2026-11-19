/**
 * ESP32-Community: Automated ESP32-CAM Time-Lapse Photography System
 * Captures high-resolution JPEG stills at configurable intervals, stores them
 * sequentially to FAT32 MicroSD card via SD_MMC (1-bit mode), and hosts an in-browser preview gallery.
 *
 * Compatible with AI-Thinker ESP32-CAM.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"
#include <ArduinoJson.h>

// --- AI-Thinker Camera Pin Mapping ---
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define FLASH_LED_PIN      4

const char* default_ssid = "YOUR_WIFI_SSID";
const char* default_password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

bool sdReady = false;
unsigned long captureIntervalMs = 10000; // 10 seconds between frames
unsigned long lastCaptureTime = 0;
int frameCount = 0;
bool flashEnabled = false;

bool initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_SVGA; // 800x600 for sharp time-lapse
    config.jpeg_quality = 12;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 14;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  return (err == ESP_OK);
}

void captureTimeLapseFrame() {
  if (!sdReady) return;

  if (flashEnabled) {
    digitalWrite(FLASH_LED_PIN, HIGH);
    delay(40);
  }

  camera_fb_t *fb = esp_camera_fb_get();

  if (flashEnabled) {
    digitalWrite(FLASH_LED_PIN, LOW);
  }

  if (!fb) {
    Serial.println("Camera capture failed!");
    return;
  }

  frameCount++;
  char path[32];
  snprintf(path, sizeof(path), "/frame_%04d.jpg", frameCount);

  fs::FS &fs = SD_MMC;
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.printf("Failed to create file: %s\n", path);
  } else {
    file.write(fb->buf, fb->len);
    file.close();
    Serial.printf("📸 Saved time-lapse: %s (%u bytes)\n", path, (unsigned int)fb->len);
  }

  esp_camera_fb_return(fb);
}

// --- Web UI ---
const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32-CAM Time-Lapse Studio</title>
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
      padding: 24px;
      max-width: 520px;
      width: 100%;
      text-align: center;
      box-shadow: 0 20px 25px -5px rgba(0, 0, 0, 0.4);
    }
    h1 { font-size: 1.5rem; margin-top: 0; }
    .preview-box {
      width: 100%;
      height: 280px;
      background: #000;
      border-radius: 12px;
      overflow: hidden;
      margin: 16px 0;
      display: flex;
      align-items: center;
      justify-content: center;
      border: 1px solid var(--border);
    }
    .preview-box img { max-width: 100%; max-height: 100%; object-fit: contain; }
    .grid {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 10px;
      margin-bottom: 20px;
      text-align: left;
    }
    .box {
      background: #0b1322;
      padding: 12px;
      border-radius: 8px;
      border: 1px solid var(--border);
    }
    .lbl { font-size: 0.75rem; color: var(--text-muted); text-transform: uppercase; }
    .val { font-size: 1.15rem; font-weight: 700; margin-top: 4px; }
    .btn {
      width: 100%;
      padding: 12px;
      font-size: 1rem;
      font-weight: 700;
      border: none;
      border-radius: 8px;
      background: var(--accent);
      color: #090d16;
      cursor: pointer;
    }
  </style>
</head>
<body>
  <div class="card">
    <h1>📷 Time-Lapse Studio</h1>
    <div class="preview-box">
      <img id="liveImg" src="/capture" alt="Latest Frame">
    </div>

    <div class="grid">
      <div class="box">
        <div class="lbl">Frames Saved</div>
        <div class="val" id="frames">0</div>
      </div>
      <div class="box">
        <div class="lbl">SD Storage</div>
        <div class="val" id="sdState">Checking...</div>
      </div>
      <div class="box">
        <div class="lbl">Interval</div>
        <div class="val" id="interval">10s</div>
      </div>
      <div class="box">
        <div class="lbl">Resolution</div>
        <div class="val">800x600 SVGA</div>
      </div>
    </div>

    <button class="btn" onclick="triggerManual()">📸 SNAP INSTANT FRAME</button>
  </div>
  <script>
    function updateMetrics() {
      fetch('/api/status')
        .then(r => r.json())
        .then(d => {
          document.getElementById('frames').innerText = d.framesCaptured;
          document.getElementById('sdState').innerText = d.sdMounted ? 'MOUNTED ✅' : 'NO CARD ❌';
          document.getElementById('interval').innerText = (d.intervalMs / 1000) + 's';
        });
    }
    function triggerManual() {
      fetch('/api/snap', { method: 'POST' }).then(() => {
        setTimeout(() => {
          document.getElementById('liveImg').src = '/capture?t=' + Date.now();
          updateMetrics();
        }, 500);
      });
    }
    setInterval(updateMetrics, 2500);
    updateMetrics();
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", HTML_PAGE);
}

void handleCapture() {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    server.send(500, "text/plain", "Camera capture error");
    return;
  }
  server.sendHeader("Content-Type", "image/jpeg");
  server.sendHeader("Content-Length", String(fb->len));
  server.send_P(200, "image/jpeg", (const char *)fb->buf, fb->len);
  esp_camera_fb_return(fb);
}

void handleApiStatus() {
  StaticJsonDocument<256> doc;
  doc["framesCaptured"] = frameCount;
  doc["sdMounted"] = sdReady;
  doc["intervalMs"] = captureIntervalMs;
  doc["uptime"] = millis();

  String out;
  serializeJson(doc, out);
  server.send(200, "application/json", out);
}

void handleApiSnap() {
  captureTimeLapseFrame();
  server.send(200, "application/json", "{\"success\":true}");
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n[ESP32-CAM Automated Time-Lapse Studio]");

  pinMode(FLASH_LED_PIN, OUTPUT);
  digitalWrite(FLASH_LED_PIN, LOW);

  // Initialize Camera
  if (initCamera()) {
    Serial.println("✅ OV2640 Camera initialized successfully.");
  } else {
    Serial.println("❌ Camera init failed! Check camera ribbon cable seating.");
  }

  // Initialize MicroSD card in 1-bit SD_MMC mode
  if (SD_MMC.begin("/sdcard", true)) { // 1-bit mode frees GPIO4/12/13
    sdReady = true;
    Serial.printf("✅ SD_MMC mounted! Card size: %llu MB\n", SD_MMC.cardSize() / (1024 * 1024));
  } else {
    sdReady = false;
    Serial.println("⚠️ SD_MMC mount failed. Check if card is FAT32 formatted.");
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
    Serial.println("\nWiFi timeout; launching fallback AP: ESP32CAM-TimeLapse");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP32CAM-TimeLapse", "cam12345");
    Serial.printf("AP IP: %s\n", WiFi.softAPIP().toString().c_str());
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/capture", HTTP_GET, handleCapture);
  server.on("/api/status", HTTP_GET, handleApiStatus);
  server.on("/api/snap", HTTP_POST, handleApiSnap);
  server.begin();
  Serial.println("Web server online.");

  lastCaptureTime = millis();
}

void loop() {
  server.handleClient();

  if (millis() - lastCaptureTime >= captureIntervalMs) {
    lastCaptureTime = millis();
    captureTimeLapseFrame();
  }

  delay(10);
}
