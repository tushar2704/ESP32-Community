/*
 * ESP32-Community | Robotics Project 01: Wi-Fi Remote Controlled Robot Car
 * 
 * Creates a standalone Wi-Fi Access Point hosting a responsive mobile web controller.
 * Drives 2 or 4 DC gear motors via an L298N, TB6612FNG, or L9110S motor driver
 * with variable speed PWM control and instantaneous directional steering.
 * 
 * Part of ESP32-Community Hub: https://github.com/tushar2704/ESP32-Community
 * Video Tutorials & Community: @IamNotIntoTech | @Makerrdesigns
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi Access Point Credentials
const char *AP_SSID = "ESP32-Robot-Car";
const char *AP_PASS = "12345678";

// Motor Driver Pins (L298N / TB6612FNG)
// Left Motor
const int ENA_PIN = 14;   // Speed Enable Left
const int IN1_PIN = 27;   // Forward Left
const int IN2_PIN = 26;   // Reverse Left

// Right Motor
const int ENB_PIN = 12;   // Speed Enable Right
const int IN3_PIN = 25;   // Forward Right
const int IN4_PIN = 33;   // Reverse Right

// Headlights (optional LED)
const int HEADLIGHT_PIN = 2;

// PWM Configuration
const int PWM_FREQ = 1000;
const int PWM_RES = 8;        // 8-bit resolution (0 - 255)
const int PWM_CH_A = 0;
const int PWM_CH_B = 1;

int motorSpeed = 200;         // Default speed (0 - 255)
bool headlightState = false;

WebServer server(80);

void setMotorPWM(int leftSpeed, int rightSpeed) {
#if defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3)
  ledcWrite(ENA_PIN, constrain(leftSpeed, 0, 255));
  ledcWrite(ENB_PIN, constrain(rightSpeed, 0, 255));
#else
  ledcWrite(PWM_CH_A, constrain(leftSpeed, 0, 255));
  ledcWrite(PWM_CH_B, constrain(rightSpeed, 0, 255));
#endif
}

void stopCar() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
  setMotorPWM(0, 0);
}

void moveForward() {
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  setMotorPWM(motorSpeed, motorSpeed);
}

void moveBackward() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
  setMotorPWM(motorSpeed, motorSpeed);
}

void turnLeft() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  setMotorPWM(motorSpeed, motorSpeed);
}

void turnRight() {
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
  setMotorPWM(motorSpeed, motorSpeed);
}

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
  <title>ESP32 Robot Controller</title>
  <style>
    * { box-sizing: border-box; touch-action: manipulation; }
    body {
      background: #0d1117;
      color: #e6edf3;
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
      text-align: center;
      margin: 0;
      padding: 20px;
    }
    h1 { color: #58a6ff; font-size: 1.6rem; margin-bottom: 5px; }
    p.sub { color: #8b949e; font-size: 0.9rem; margin-top: 0; }
    .grid {
      display: grid;
      grid-template-columns: repeat(3, 80px);
      grid-gap: 15px;
      justify-content: center;
      margin: 25px auto;
    }
    .btn {
      background: #21262d;
      border: 2px solid #30363d;
      color: #58a6ff;
      font-size: 1.8rem;
      border-radius: 12px;
      height: 80px;
      display: flex;
      align-items: center;
      justify-content: center;
      user-select: none;
      -webkit-user-select: none;
    }
    .btn:active { background: #58a6ff; color: #0d1117; }
    .btn-stop { background: #da3633; color: white; border-color: #f85149; }
    .btn-stop:active { background: #b62324; }
    .controls { max-width: 320px; margin: 0 auto; }
    .slider-box {
      background: #161b22;
      border: 1px solid #30363d;
      border-radius: 8px;
      padding: 15px;
      margin-top: 15px;
    }
    input[type=range] { width: 100%; height: 10px; border-radius: 5px; }
    .btn-headlight {
      margin-top: 15px;
      width: 100%;
      padding: 12px;
      background: #238636;
      border: none;
      color: white;
      border-radius: 8px;
      font-weight: bold;
      font-size: 1rem;
    }
  </style>
</head>
<body>
  <h1>ESP32 Robot Car</h1>
  <p class="sub">@IamNotIntoTech | @Makerrdesigns</p>

  <div class="grid">
    <div></div>
    <div class="btn" onpointerdown="send('/cmd?val=F')" onpointerup="send('/cmd?val=S')">&#9650;</div>
    <div></div>
    <div class="btn" onpointerdown="send('/cmd?val=L')" onpointerup="send('/cmd?val=S')">&#9664;</div>
    <div class="btn btn-stop" onclick="send('/cmd?val=S')">&#9632;</div>
    <div class="btn" onpointerdown="send('/cmd?val=R')" onpointerup="send('/cmd?val=S')">&#9654;</div>
    <div></div>
    <div class="btn" onpointerdown="send('/cmd?val=B')" onpointerup="send('/cmd?val=S')">&#9660;</div>
    <div></div>
  </div>

  <div class="controls">
    <div class="slider-box">
      <label>Speed: <span id="speedVal">200</span></label><br>
      <input type="range" min="100" max="255" value="200" onchange="setSpeed(this.value)">
    </div>
    <button class="btn-headlight" onclick="toggleLight()">Toggle Headlights</button>
  </div>

  <script>
    function send(path) {
      fetch(path).catch(err => console.log(err));
    }
    function setSpeed(v) {
      document.getElementById('speedVal').innerText = v;
      fetch('/speed?val=' + v);
    }
    function toggleLight() {
      fetch('/light');
    }
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", INDEX_HTML);
}

void handleCommand() {
  if (server.hasArg("val")) {
    char cmd = server.arg("val")[0];
    switch (cmd) {
      case 'F': moveForward(); break;
      case 'B': moveBackward(); break;
      case 'L': turnLeft(); break;
      case 'R': turnRight(); break;
      case 'S':
      default:
        stopCar();
        break;
    }
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing val");
  }
}

void handleSpeed() {
  if (server.hasArg("val")) {
    motorSpeed = server.arg("val").toInt();
    motorSpeed = constrain(motorSpeed, 0, 255);
    Serial.printf("[CAR] Speed updated to: %d\n", motorSpeed);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing val");
  }
}

void handleLight() {
  headlightState = !headlightState;
  digitalWrite(HEADLIGHT_PIN, headlightState ? HIGH : LOW);
  server.send(200, "text/plain", headlightState ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Configure Motor Direction Pins
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  pinMode(HEADLIGHT_PIN, OUTPUT);
  digitalWrite(HEADLIGHT_PIN, LOW);

  // Configure PWM for Speed Control
#if defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3)
  ledcAttach(ENA_PIN, PWM_FREQ, PWM_RES);
  ledcAttach(ENB_PIN, PWM_FREQ, PWM_RES);
#else
  ledcSetup(PWM_CH_A, PWM_FREQ, PWM_RES);
  ledcAttachPin(ENA_PIN, PWM_CH_A);
  ledcSetup(PWM_CH_B, PWM_FREQ, PWM_RES);
  ledcAttachPin(ENB_PIN, PWM_CH_B);
#endif

  stopCar();

  Serial.println("==================================================");
  Serial.println("    ESP32 Community: Wi-Fi Robot Car Controller   ");
  Serial.println("==================================================");

  // Start Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("[AP] Access Point started! SSID: ");
  Serial.println(AP_SSID);
  Serial.print("[AP] Connect to Wi-Fi and browse to: http://");
  Serial.println(myIP);

  // Configure Web Server Routes
  server.on("/", handleRoot);
  server.on("/cmd", handleCommand);
  server.on("/speed", handleSpeed);
  server.on("/light", handleLight);
  server.begin();
  Serial.println("[HTTP] Web Controller Server running!");
}

void loop() {
  server.handleClient();
}
