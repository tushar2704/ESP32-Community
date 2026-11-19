/**
 * ESP32-Community: Smart Addressable WS2812B / NeoPixel LED Strip Web Controller
 * Features interactive web color picker, speed/brightness controls, and 8 animated lighting effects.
 *
 * Compatible with ESP32-WROOM-32, ESP32-S3, ESP32-C3.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>

// --- Pin & Hardware Definitions ---
#if defined(CONFIG_IDF_TARGET_ESP32C3)
  #define LED_PIN    4
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
  #define LED_PIN    18
#else
  #define LED_PIN    18
#endif

#define NUM_LEDS   30

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
WebServer server(80);

// --- State Variables ---
bool powerOn = true;
uint8_t currentRed = 255;
uint8_t currentGreen = 100;
uint8_t currentBlue = 50;
uint8_t brightness = 180;
int currentEffect = 1; // 0=Solid, 1=Rainbow, 2=Color Wipe, 3=Theater Chase, 4=Breathing, 5=Fire, 6=Meteor, 7=Strobe
int effectSpeed = 30;  // Delay in ms between frames

unsigned long lastFrameTime = 0;
uint16_t frameCounter = 0;

// Configuration
const char* default_ssid = "YOUR_WIFI_SSID";
const char* default_password = "YOUR_WIFI_PASSWORD";

// --- Effect Implementations ---
uint32_t wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void applySolidColor(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}

void updateAnimation() {
  if (!powerOn) {
    strip.clear();
    strip.show();
    return;
  }

  if (millis() - lastFrameTime < (unsigned long)effectSpeed) {
    return;
  }
  lastFrameTime = millis();

  switch (currentEffect) {
    case 0: // Solid Color
      applySolidColor(currentRed, currentGreen, currentBlue);
      break;

    case 1: // Rainbow Cycle
      for (uint16_t i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, wheel(((i * 256 / NUM_LEDS) + frameCounter) & 255));
      }
      strip.show();
      frameCounter++;
      break;

    case 2: // Color Wipe
      strip.setPixelColor(frameCounter % NUM_LEDS, strip.Color(currentRed, currentGreen, currentBlue));
      strip.show();
      frameCounter++;
      break;

    case 3: // Theater Chase
      strip.clear();
      for (int i = 0; i < NUM_LEDS; i += 3) {
        int idx = (i + (frameCounter % 3)) % NUM_LEDS;
        strip.setPixelColor(idx, strip.Color(currentRed, currentGreen, currentBlue));
      }
      strip.show();
      frameCounter++;
      break;

    case 4: { // Breathing / Pulse
      float val = (exp(sin(frameCounter * 0.05)) - 0.36787944) * 108.0;
      uint8_t factor = constrain((int)val, 10, 255);
      uint8_t r = (currentRed * factor) / 255;
      uint8_t g = (currentGreen * factor) / 255;
      uint8_t b = (currentBlue * factor) / 255;
      applySolidColor(r, g, b);
      frameCounter++;
      break;
    }

    case 5: { // Fire Flicker
      for (int i = 0; i < NUM_LEDS; i++) {
        int flicker = random(0, 55);
        int r = constrain(currentRed - flicker, 0, 255);
        int g = constrain(currentGreen - flicker, 0, 255);
        int b = constrain(currentBlue - flicker, 0, 255);
        strip.setPixelColor(i, strip.Color(r, g, b));
      }
      strip.show();
      break;
    }

    case 6: { // Meteor Rain
      // Fade all LEDs
      for (int i = 0; i < NUM_LEDS; i++) {
        uint32_t c = strip.getPixelColor(i);
        uint8_t r = (c >> 16) & 0xff;
        uint8_t g = (c >> 8) & 0xff;
        uint8_t b = c & 0xff;
        r = (r <= 30) ? 0 : r - 30;
        g = (g <= 30) ? 0 : g - 30;
        b = (b <= 30) ? 0 : b - 30;
        strip.setPixelColor(i, strip.Color(r, g, b));
      }
      int head = frameCounter % (NUM_LEDS + 10);
      if (head < NUM_LEDS) {
        strip.setPixelColor(head, strip.Color(currentRed, currentGreen, currentBlue));
      }
      strip.show();
      frameCounter++;
      break;
    }

    case 7: { // Strobe
      if ((frameCounter % 2) == 0) {
        applySolidColor(currentRed, currentGreen, currentBlue);
      } else {
        strip.clear();
        strip.show();
      }
      frameCounter++;
      break;
    }

    default:
      applySolidColor(currentRed, currentGreen, currentBlue);
      break;
  }
}

// --- Web UI ---
const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 LED Strip Controller</title>
  <style>
    :root {
      --bg: #0d1117;
      --card: #161b22;
      --border: #30363d;
      --accent: #58a6ff;
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
      padding: 20px;
    }
    .card {
      background: var(--card);
      border: 1px solid var(--border);
      border-radius: 16px;
      padding: 24px;
      max-width: 440px;
      width: 100%;
      box-shadow: 0 10px 30px rgba(0,0,0,0.5);
    }
    h1 { margin-top: 0; font-size: 1.5rem; text-align: center; color: #fff; }
    .control-group { margin-bottom: 20px; }
    label { display: block; font-weight: 600; margin-bottom: 8px; font-size: 0.95rem; }
    input[type="range"] {
      width: 100%;
      -webkit-appearance: none;
      background: #21262d;
      height: 8px;
      border-radius: 4px;
      outline: none;
    }
    input[type="range"]::-webkit-slider-thumb {
      -webkit-appearance: none;
      width: 20px;
      height: 20px;
      border-radius: 50%;
      background: var(--accent);
      cursor: pointer;
    }
    input[type="color"] {
      width: 100%;
      height: 50px;
      border: 1px solid var(--border);
      border-radius: 8px;
      cursor: pointer;
      background: transparent;
    }
    select, .btn-power {
      width: 100%;
      padding: 12px;
      background: #21262d;
      color: #fff;
      border: 1px solid var(--border);
      border-radius: 8px;
      font-size: 1rem;
      cursor: pointer;
    }
    .btn-power {
      font-weight: 700;
      margin-bottom: 20px;
      background: #238636;
      border: none;
    }
    .btn-off { background: #da3633; }
  </style>
</head>
<body>
  <div class="card">
    <h1>🌈 NeoPixel Controller</h1>
    <button id="pwrBtn" class="btn-power" onclick="togglePower()">POWER ON</button>
    <div class="control-group">
      <label>Choose Color</label>
      <input type="color" id="picker" value="#ff6432" onchange="sendColor()">
    </div>
    <div class="control-group">
      <label>Lighting Effect</label>
      <select id="effect" onchange="sendEffect()">
        <option value="0">Solid Color</option>
        <option value="1" selected>Rainbow Cycle</option>
        <option value="2">Color Wipe</option>
        <option value="3">Theater Chase</option>
        <option value="4">Breathing Pulse</option>
        <option value="5">Fire Flicker</option>
        <option value="6">Meteor Rain</option>
        <option value="7">Strobe</option>
      </select>
    </div>
    <div class="control-group">
      <label>Brightness: <span id="valBright">180</span></label>
      <input type="range" id="bright" min="0" max="255" value="180" oninput="sendBrightness()">
    </div>
    <div class="control-group">
      <label>Speed (Delay ms): <span id="valSpeed">30</span></label>
      <input type="range" id="speed" min="5" max="150" value="30" oninput="sendSpeed()">
    </div>
  </div>
  <script>
    let isPower = true;
    function sendPayload(payload) {
      fetch('/api/led', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify(payload)
      });
    }
    function togglePower() {
      isPower = !isPower;
      const b = document.getElementById('pwrBtn');
      b.innerText = isPower ? 'POWER ON' : 'POWER OFF';
      b.className = isPower ? 'btn-power' : 'btn-power btn-off';
      sendPayload({ power: isPower });
    }
    function sendColor() {
      const hex = document.getElementById('picker').value;
      const r = parseInt(hex.substr(1,2), 16);
      const g = parseInt(hex.substr(3,2), 16);
      const b = parseInt(hex.substr(5,2), 16);
      sendPayload({ r: r, g: g, b: b });
    }
    function sendEffect() {
      sendPayload({ effect: parseInt(document.getElementById('effect').value) });
    }
    function sendBrightness() {
      const v = parseInt(document.getElementById('bright').value);
      document.getElementById('valBright').innerText = v;
      sendPayload({ brightness: v });
    }
    function sendSpeed() {
      const v = parseInt(document.getElementById('speed').value);
      document.getElementById('valSpeed').innerText = v;
      sendPayload({ speed: v });
    }
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", HTML_PAGE);
}

void handleApiStatus() {
  StaticJsonDocument<256> doc;
  doc["power"] = powerOn;
  doc["r"] = currentRed;
  doc["g"] = currentGreen;
  doc["b"] = currentBlue;
  doc["brightness"] = brightness;
  doc["effect"] = currentEffect;
  doc["speed"] = effectSpeed;
  doc["numLeds"] = NUM_LEDS;

  String res;
  serializeJson(doc, res);
  server.send(200, "application/json", res);
}

void handleApiLed() {
  if (server.hasArg("plain")) {
    StaticJsonDocument<256> doc;
    DeserializationError err = deserializeJson(doc, server.arg("plain"));
    if (!err) {
      if (doc.containsKey("power")) powerOn = doc["power"];
      if (doc.containsKey("r")) currentRed = doc["r"];
      if (doc.containsKey("g")) currentGreen = doc["g"];
      if (doc.containsKey("b")) currentBlue = doc["b"];
      if (doc.containsKey("brightness")) {
        brightness = doc["brightness"];
        strip.setBrightness(brightness);
      }
      if (doc.containsKey("effect")) currentEffect = doc["effect"];
      if (doc.containsKey("speed")) effectSpeed = doc["speed"];

      StaticJsonDocument<128> reply;
      reply["success"] = true;
      String out;
      serializeJson(reply, out);
      server.send(200, "application/json", out);
      return;
    }
  }
  server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n[ESP32 Smart LED Strip Controller]");

  strip.begin();
  strip.setBrightness(brightness);
  strip.show();

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
    Serial.println("\nWiFi connection failed; creating fallback AP: ESP32-NeoPixel");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP32-NeoPixel", "light1234");
    Serial.printf("AP IP: %s\n", WiFi.softAPIP().toString().c_str());
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/status", HTTP_GET, handleApiStatus);
  server.on("/api/led", HTTP_POST, handleApiLed);
  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  server.handleClient();
  updateAnimation();
}
