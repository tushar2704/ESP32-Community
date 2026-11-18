/**
 * @file web-server-hello.ino
 * @brief Project 04: Embedded Web Server & IoT Control Panel
 * @author Tushar Aggarwal (@tushar2704)
 * @url https://github.com/tushar2704/ESP32-Community
 * @license AGPL-3.0
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// Enter your Wi-Fi credentials (or leave blank to launch Soft-AP mode)
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";

// LED Pin definition
#if defined(CONFIG_IDF_TARGET_ESP32S3)
    #define LED_PIN 48
#elif defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C6)
    #define LED_PIN 8
#else
    #define LED_PIN 2
#endif

WebServer server(80);
bool led_state = false;

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Control Hub</title>
  <style>
    body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; background: #0f172a; color: #f8fafc; text-align: center; margin: 0; padding: 20px; }
    .card { background: #1e293b; border-radius: 12px; padding: 24px; max-width: 400px; margin: 20px auto; box-shadow: 0 10px 15px -3px rgba(0,0,0,0.5); }
    h1 { color: #38bdf8; font-size: 24px; margin-bottom: 8px; }
    p { color: #94a3b8; font-size: 14px; }
    .btn { display: inline-block; background: #3b82f6; color: white; padding: 14px 28px; border-radius: 8px; font-weight: bold; text-decoration: none; font-size: 16px; transition: 0.2s; margin-top: 15px; }
    .btn:hover { background: #2563eb; }
    .badge { display: inline-block; padding: 4px 10px; border-radius: 20px; font-size: 12px; font-weight: bold; }
    .on { background: #22c55e; color: black; }
    .off { background: #ef4444; color: white; }
    .stats { text-align: left; margin-top: 20px; font-size: 13px; color: #cbd5e1; }
  </style>
</head>
<body>
  <div class="card">
    <h1>⚡ ESP32 Web Hub</h1>
    <p>Community Project 04 &bull; Local Control</p>
    <div style="margin: 20px 0;">
      LED Status: <span class="badge %LED_CLASS%">%LED_STATUS%</span>
    </div>
    <a href="/toggle" class="btn">Toggle LED</a>
    <div class="stats">
      <hr style="border: 0; border-top: 1px solid #334155; margin: 15px 0;">
      <div>⏱️ Uptime: <strong>%UPTIME%</strong></div>
      <div>💾 Free RAM: <strong>%RAM% KB</strong></div>
      <div>📶 Signal: <strong>%RSSI% dBm</strong></div>
    </div>
  </div>
</body>
</html>
)rawliteral";

String getUptimeStr() {
    uint32_t s = millis() / 1000;
    char buf[32];
    snprintf(buf, sizeof(buf), "%02uh %02um %02us", s / 3600, (s % 3600) / 60, s % 60);
    return String(buf);
}

void handleRoot() {
    String html = INDEX_HTML;
    html.replace("%LED_STATUS%", led_state ? "ON" : "OFF");
    html.replace("%LED_CLASS%", led_state ? "on" : "off");
    html.replace("%UPTIME%", getUptimeStr());
    html.replace("%RAM%", String(ESP.getFreeHeap() / 1024));
    html.replace("%RSSI%", String(WiFi.RSSI()));
    server.send(200, "text/html", html);
}

void handleToggle() {
    led_state = !led_state;
    digitalWrite(LED_PIN, led_state ? HIGH : LOW);
    server.sendHeader("Location", "/");
    server.send(303);
}

void handleStatusJson() {
    String json = "{";
    json += "\"led\":" + String(led_state ? "true" : "false") + ",";
    json += "\"heap\":" + String(ESP.getFreeHeap()) + ",";
    json += "\"uptime_sec\":" + String(millis() / 1000) + ",";
    json += "\"rssi\":" + String(WiFi.RSSI());
    json += "}";
    server.send(200, "application/json", json);
}

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.begin(115200);
    delay(500);

    Serial.println("\n========================================================");
    Serial.println("  ⚡ ESP32 Community — Project 04: Embedded Web Server");
    Serial.println("========================================================");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    Serial.print("Connecting to Wi-Fi");
    uint32_t start_ms = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - start_ms) < 8000) {
        delay(250);
        Serial.print(".");
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("Connected! IP Address: http://%s\n", WiFi.localIP().toString().c_str());
    } else {
        Serial.println("⚠️ Could not connect to Wi-Fi. Launching Soft-AP mode...");
        WiFi.mode(WIFI_AP);
        WiFi.softAP("ESP32-Web-Hub", "12345678");
        Serial.printf("Access Point Live! Connect to 'ESP32-Web-Hub' and visit http://%s\n", 
                      WiFi.softAPIP().toString().c_str());
    }

    server.on("/", handleRoot);
    server.on("/toggle", handleToggle);
    server.on("/api/status", handleStatusJson);
    server.begin();
    Serial.println("🚀 HTTP Server started on port 80!");
}

void loop() {
    server.handleClient();
}
