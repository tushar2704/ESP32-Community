/*
 * ESP32-Community | Robotics Project 04: 4-DOF Robotic Arm Controller
 * 
 * Controls a 4-degree-of-freedom robotic arm (Base, Shoulder, Elbow, Gripper)
 * via ESP32 PWM servo signals, featuring smooth speed easing and an
 * interactive mobile-friendly web dashboard with preset poses.
 * 
 * Part of ESP32-Community Hub: https://github.com/tushar2704/ESP32-Community
 * Video Tutorials & Community: @IamNotIntoTech | @Makerrdesigns
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Wi-Fi Access Point Credentials
const char *AP_SSID = "ESP32-Robotic-Arm";
const char *AP_PASS = "12345678";

// Servo Pin Definitions
const int PIN_BASE     = 13;
const int PIN_SHOULDER = 12;
const int PIN_ELBOW    = 14;
const int PIN_GRIPPER  = 27;

// Servo Objects
Servo servoBase;
Servo servoShoulder;
Servo servoElbow;
Servo servoGripper;

// Current angles & Target angles for smooth interpolation
struct Joint {
  int current;
  int target;
  int minLimit;
  int maxLimit;
};

Joint jointBase     = {90, 90, 0, 180};
Joint jointShoulder = {90, 90, 15, 165};
Joint jointElbow    = {90, 90, 10, 170};
Joint jointGripper  = {50, 50, 10, 120};

WebServer server(80);

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
  <title>ESP32 4-DOF Robotic Arm</title>
  <style>
    * { box-sizing: border-box; }
    body {
      background: #0d1117;
      color: #e6edf3;
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
      margin: 0;
      padding: 20px;
      text-align: center;
    }
    h1 { color: #58a6ff; font-size: 1.5rem; margin-bottom: 5px; }
    p.sub { color: #8b949e; font-size: 0.85rem; margin-top: 0; }
    .card {
      background: #161b22;
      border: 1px solid #30363d;
      border-radius: 10px;
      max-width: 400px;
      margin: 15px auto;
      padding: 15px 20px;
      text-align: left;
    }
    .joint-row { margin-bottom: 15px; }
    .joint-header { display: flex; justify-content: space-between; font-weight: bold; margin-bottom: 5px; }
    .joint-badge { color: #58a6ff; font-family: monospace; }
    input[type=range] { width: 100%; height: 8px; border-radius: 4px; accent-color: #58a6ff; }
    .presets { display: flex; gap: 10px; justify-content: center; margin-top: 20px; }
    .btn {
      background: #21262d;
      border: 1px solid #30363d;
      color: #e6edf3;
      padding: 10px 15px;
      border-radius: 6px;
      font-weight: bold;
      cursor: pointer;
    }
    .btn:active { background: #58a6ff; color: #0d1117; }
  </style>
</head>
<body>
  <h1>4-DOF Robotic Arm</h1>
  <p class="sub">ESP32-Community | @IamNotIntoTech</p>

  <div class="card">
    <div class="joint-row">
      <div class="joint-header"><span>Base (Waist)</span><span class="joint-badge" id="valBase">90&deg;</span></div>
      <input type="range" id="sliderBase" min="0" max="180" value="90" oninput="update('base', this.value)">
    </div>
    <div class="joint-row">
      <div class="joint-header"><span>Shoulder</span><span class="joint-badge" id="valShoulder">90&deg;</span></div>
      <input type="range" id="sliderShoulder" min="15" max="165" value="90" oninput="update('shoulder', this.value)">
    </div>
    <div class="joint-row">
      <div class="joint-header"><span>Elbow</span><span class="joint-badge" id="valElbow">90&deg;</span></div>
      <input type="range" id="sliderElbow" min="10" max="170" value="90" oninput="update('elbow', this.value)">
    </div>
    <div class="joint-row">
      <div class="joint-header"><span>Gripper (Claw)</span><span class="joint-badge" id="valGripper">50&deg;</span></div>
      <input type="range" id="sliderGripper" min="10" max="120" value="50" oninput="update('gripper', this.value)">
    </div>
  </div>

  <div class="presets">
    <button class="btn" onclick="preset(90, 90, 90, 50)">Home</button>
    <button class="btn" onclick="preset(90, 130, 40, 110)">Pick Up</button>
    <button class="btn" onclick="preset(160, 100, 70, 20)">Drop Off</button>
  </div>

  <script>
    function update(joint, angle) {
      document.getElementById('val' + joint.charAt(0).toUpperCase() + joint.slice(1)).innerHTML = angle + '&deg;';
      fetch('/set?joint=' + joint + '&angle=' + angle).catch(console.error);
    }
    function preset(b, s, e, g) {
      document.getElementById('sliderBase').value = b; update('base', b);
      document.getElementById('sliderShoulder').value = s; update('shoulder', s);
      document.getElementById('sliderElbow').value = e; update('elbow', e);
      document.getElementById('sliderGripper').value = g; update('gripper', g);
    }
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", INDEX_HTML);
}

void handleSet() {
  if (server.hasArg("joint") && server.hasArg("angle")) {
    String joint = server.arg("joint");
    int angle = server.arg("angle").toInt();

    if (joint == "base") {
      jointBase.target = constrain(angle, jointBase.minLimit, jointBase.maxLimit);
    } else if (joint == "shoulder") {
      jointShoulder.target = constrain(angle, jointShoulder.minLimit, jointShoulder.maxLimit);
    } else if (joint == "elbow") {
      jointElbow.target = constrain(angle, jointElbow.minLimit, jointElbow.maxLimit);
    } else if (joint == "gripper") {
      jointGripper.target = constrain(angle, jointGripper.minLimit, jointGripper.maxLimit);
    }
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing args");
  }
}

// Smoothly interpolate current angle toward target angle
void updateJoint(Joint &j, Servo &s) {
  if (j.current < j.target) {
    j.current++;
    s.write(j.current);
  } else if (j.current > j.target) {
    j.current--;
    s.write(j.current);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Allocate hardware PWM timers for servos
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // Standard 50Hz RC servo timing: 500us to 2400us pulse widths
  servoBase.setPeriodHertz(50);
  servoShoulder.setPeriodHertz(50);
  servoElbow.setPeriodHertz(50);
  servoGripper.setPeriodHertz(50);

  servoBase.attach(PIN_BASE, 500, 2400);
  servoShoulder.attach(PIN_SHOULDER, 500, 2400);
  servoElbow.attach(PIN_ELBOW, 500, 2400);
  servoGripper.attach(PIN_GRIPPER, 500, 2400);

  // Set initial home positions
  servoBase.write(jointBase.current);
  servoShoulder.write(jointShoulder.current);
  servoElbow.write(jointElbow.current);
  servoGripper.write(jointGripper.current);

  Serial.println("==================================================");
  Serial.println("  ESP32 Community: 4-DOF Web Robotic Arm Control  ");
  Serial.println("==================================================");

  // Start Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.printf("[AP] Connect to SSID: %s (Pass: %s)\n", AP_SSID, AP_PASS);
  Serial.print("[AP] Open browser to: http://");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
  Serial.println("[HTTP] Servo Web Controller ready!");
}

void loop() {
  server.handleClient();

  // Smooth motion interpolation tick every 15ms
  static unsigned long lastTick = 0;
  if (millis() - lastTick >= 15) {
    lastTick = millis();
    updateJoint(jointBase, servoBase);
    updateJoint(jointShoulder, servoShoulder);
    updateJoint(jointElbow, servoElbow);
    updateJoint(jointGripper, servoGripper);
  }
}
