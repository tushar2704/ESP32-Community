/**
 * ESP32-Community: Industrial RS485 Modbus RTU to Web & MQTT Gateway
 * 
 * Hardware:
 *   - ESP32 DevKit (or ESP32-S3 / C3)
 *   - MAX485 TTL-to-RS485 Transceiver Module
 *   - 120Ω terminating resistor across RS485 A & B bus lines
 *
 * Pin Connections:
 *   - MAX485 DI (Driver In / TX) -> ESP32 GPIO 17 (TX2)
 *   - MAX485 RO (Receiver Out / RX) -> ESP32 GPIO 16 (RX2)
 *   - MAX485 DE & RE (Direction Enable) -> ESP32 GPIO 4
 *   - MAX485 VCC -> 5V (or 3.3V depending on module variant)
 *   - MAX485 GND -> ESP32 GND
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// --- Configuration ---
const char* WIFI_SSID     = "Your_WiFi_SSID";
const char* WIFI_PASSWORD = "Your_WiFi_Password";

const char* MQTT_BROKER   = "192.168.1.100";
const int   MQTT_PORT     = 1883;
const char* MQTT_TOPIC    = "industrial/modbus/telemetry";

#define PIN_RS485_RX   16
#define PIN_RS485_TX   17
#define PIN_RS485_DIR  4    // HIGH = Transmit, LOW = Receive
#define PIN_STATUS_LED 2

// Modbus Target Slave configuration
#define MODBUS_SLAVE_ID 0x01
#define MODBUS_BAUD     9600

// Modbus Function Codes
#define MODBUS_FC_READ_COILS            0x01
#define MODBUS_FC_READ_HOLDING_REGS     0x03
#define MODBUS_FC_READ_INPUT_REGS       0x04
#define MODBUS_FC_WRITE_SINGLE_COIL     0x05
#define MODBUS_FC_WRITE_SINGLE_REG      0x06

WebServer server(80);
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// --- Telemetry State ---
struct ModbusData {
  float temperature = 0.0f;
  float humidity    = 0.0f;
  float voltage     = 0.0f;
  float current     = 0.0f;
  float power       = 0.0f;
  bool  relay1State = false;
  uint32_t pollCount = 0;
  uint32_t errorCount = 0;
  uint32_t lastPollTime = 0;
} mbData;

// --- Helper: Standard Modbus CRC-16 Calculation ---
uint16_t calculateCRC(const uint8_t *buffer, size_t length) {
  uint16_t crc = 0xFFFF;
  for (size_t i = 0; i < length; i++) {
    crc ^= (uint16_t)buffer[i];
    for (int j = 0; j < 8; j++) {
      if (crc & 0x0001) {
        crc = (crc >> 1) ^ 0xA001;
      } else {
        crc = crc >> 1;
      }
    }
  }
  return crc;
}

// Set RS485 direction
void setRS485Tx(bool enableTx) {
  digitalWrite(PIN_RS485_DIR, enableTx ? HIGH : LOW);
  delayMicroseconds(50);
}

// Send raw Modbus RTU frame with CRC
void sendModbusRequest(uint8_t slave, uint8_t func, uint16_t regAddr, uint16_t regCount) {
  uint8_t frame[8];
  frame[0] = slave;
  frame[1] = func;
  frame[2] = (regAddr >> 8) & 0xFF;
  frame[3] = regAddr & 0xFF;
  frame[4] = (regCount >> 8) & 0xFF;
  frame[5] = regCount & 0xFF;

  uint16_t crc = calculateCRC(frame, 6);
  frame[6] = crc & 0xFF;          // CRC Low
  frame[7] = (crc >> 8) & 0xFF;   // CRC High

  // Clear input buffer
  while (Serial2.available()) Serial2.read();

  // Transmit
  setRS485Tx(true);
  Serial2.write(frame, sizeof(frame));
  Serial2.flush();
  setRS485Tx(false);
}

// Read Modbus RTU response with timeout
bool readModbusResponse(uint8_t expectedSlave, uint8_t expectedFunc, uint8_t *payload, size_t &payloadLen, uint32_t timeoutMs = 300) {
  uint8_t rxBuffer[64];
  size_t rxLen = 0;
  uint32_t start = millis();

  while ((millis() - start) < timeoutMs) {
    if (Serial2.available()) {
      if (rxLen < sizeof(rxBuffer)) {
        rxBuffer[rxLen++] = Serial2.read();
      }
    }
  }

  if (rxLen < 5) {
    return false; // Incomplete frame
  }

  // Validate CRC
  uint16_t receivedCRC = rxBuffer[rxLen - 2] | (rxBuffer[rxLen - 1] << 8);
  uint16_t calculatedCRC = calculateCRC(rxBuffer, rxLen - 2);
  if (receivedCRC != calculatedCRC) {
    Serial.println("[Modbus] CRC Checksum Mismatch!");
    return false;
  }

  // Validate Slave & Function
  if (rxBuffer[0] != expectedSlave || rxBuffer[1] != expectedFunc) {
    return false;
  }

  // Extract byte count and data
  uint8_t byteCount = rxBuffer[2];
  if (rxLen < (size_t)(3 + byteCount + 2)) {
    return false;
  }

  payloadLen = byteCount;
  for (size_t i = 0; i < byteCount; i++) {
    payload[i] = rxBuffer[3 + i];
  }

  return true;
}

// Poll sensors via Modbus RTU
void pollModbusSlave() {
  mbData.pollCount++;
  mbData.lastPollTime = millis();

  // Request 4 Input Registers starting at 0x0000
  sendModbusRequest(MODBUS_SLAVE_ID, MODBUS_FC_READ_INPUT_REGS, 0x0000, 4);

  uint8_t payload[32];
  size_t payloadLen = 0;
  if (readModbusResponse(MODBUS_SLAVE_ID, MODBUS_FC_READ_INPUT_REGS, payload, payloadLen, 250)) {
    if (payloadLen >= 8) {
      // Decode registers (big-endian)
      int16_t rawTemp = (payload[0] << 8) | payload[1];
      int16_t rawHumi = (payload[2] << 8) | payload[3];
      int16_t rawVolt = (payload[4] << 8) | payload[5];
      int16_t rawCurr = (payload[6] << 8) | payload[7];

      mbData.temperature = rawTemp / 10.0f;
      mbData.humidity    = rawHumi / 10.0f;
      mbData.voltage     = rawVolt / 10.0f;
      mbData.current     = rawCurr / 100.0f;
      mbData.power       = mbData.voltage * mbData.current;

      digitalWrite(PIN_STATUS_LED, !digitalRead(PIN_STATUS_LED));
      Serial.printf("[Modbus] T=%.1f°C | H=%.1f%% | V=%.1fV | I=%.2fA | P=%.1fW\n",
                    mbData.temperature, mbData.humidity, mbData.voltage, mbData.current, mbData.power);
    }
  } else {
    mbData.errorCount++;
    Serial.println("[Modbus] Polling timeout or CRC error.");
  }
}

// --- Web Server Handlers ---
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Modbus RTU Gateway</title>
  <style>
    body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif; background: #0f172a; color: #f8fafc; margin: 0; padding: 20px; }
    .container { max-width: 800px; margin: 0 auto; }
    h1 { color: #38bdf8; display: flex; align-items: center; gap: 10px; margin-bottom: 5px; }
    .subtitle { color: #94a3b8; font-size: 0.95rem; margin-bottom: 25px; }
    .grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 16px; margin-bottom: 25px; }
    .card { background: #1e293b; border: 1px solid #334155; border-radius: 12px; padding: 20px; text-align: center; }
    .card h3 { margin: 0 0 10px 0; font-size: 0.85rem; text-transform: uppercase; letter-spacing: 0.05em; color: #94a3b8; }
    .card .val { font-size: 2rem; font-weight: 700; color: #38bdf8; }
    .card .unit { font-size: 1rem; color: #64748b; font-weight: 400; }
    .stats { background: #1e293b; border-radius: 12px; border: 1px solid #334155; padding: 16px 20px; font-family: monospace; font-size: 0.85rem; color: #cbd5e1; }
    .btn { background: #0284c7; color: white; border: none; padding: 10px 20px; font-size: 0.95rem; border-radius: 8px; cursor: pointer; transition: background 0.2s; }
    .btn:hover { background: #0369a1; }
  </style>
</head>
<body>
  <div class="container">
    <h1>⚡ Industrial Modbus Gateway</h1>
    <div class="subtitle">ESP32 RS485 Master • Polling Slave ID #1</div>
    <div class="grid">
      <div class="card"><h3>Temperature</h3><div class="val" id="temp">--</div><div class="unit">°C</div></div>
      <div class="card"><h3>Humidity</h3><div class="val" id="humi">--</div><div class="unit">% RH</div></div>
      <div class="card"><h3>AC Voltage</h3><div class="val" id="volt">--</div><div class="unit">Volts</div></div>
      <div class="card"><h3>AC Current</h3><div class="val" id="curr">--</div><div class="unit">Amperes</div></div>
      <div class="card"><h3>Active Power</h3><div class="val" id="pow">--</div><div class="unit">Watts</div></div>
    </div>
    <div class="stats" id="stats">Poll Count: 0 | Errors: 0</div>
  </div>
  <script>
    function update() {
      fetch('/api/data')
        .then(r => r.json())
        .then(d => {
          document.getElementById('temp').innerText = d.temperature.toFixed(1);
          document.getElementById('humi').innerText = d.humidity.toFixed(1);
          document.getElementById('volt').innerText = d.voltage.toFixed(1);
          document.getElementById('curr').innerText = d.current.toFixed(2);
          document.getElementById('pow').innerText  = d.power.toFixed(1);
          document.getElementById('stats').innerText = `Polls: ${d.pollCount} | Errors: ${d.errorCount} | Uptime: ${Math.floor(d.uptime)}s`;
        });
    }
    setInterval(update, 2000);
    update();
  </script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html", html);
}

void handleApiData() {
  StaticJsonDocument<256> doc;
  doc["temperature"] = mbData.temperature;
  doc["humidity"]    = mbData.humidity;
  doc["voltage"]     = mbData.voltage;
  doc["current"]     = mbData.current;
  doc["power"]       = mbData.power;
  doc["pollCount"]   = mbData.pollCount;
  doc["errorCount"]  = mbData.errorCount;
  doc["uptime"]      = millis() / 1000.0f;

  String res;
  serializeJson(doc, res);
  server.send(200, "application/json", res);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- [ESP32-Community] Industrial Modbus Gateway ---");

  pinMode(PIN_RS485_DIR, OUTPUT);
  pinMode(PIN_STATUS_LED, OUTPUT);
  setRS485Tx(false);

  // Initialize Serial2 for RS485 communication
  Serial2.begin(MODBUS_BAUD, SERIAL_8N1, PIN_RS485_RX, PIN_RS485_TX);
  Serial.printf("[Modbus] Serial2 initialized on RX=%d, TX=%d, DIR=%d at %d baud.\n",
                PIN_RS485_RX, PIN_RS485_TX, PIN_RS485_DIR, MODBUS_BAUD);

  // Connect to Wi-Fi or launch SoftAP
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("[WiFi] Connecting to network");
  uint32_t t0 = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - t0) < 6000) {
    delay(300);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\n[WiFi] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    WiFi.softAP("ESP32-Modbus-Gateway", "12345678");
    Serial.printf("\n[WiFi] AP Mode Active! IP: %s\n", WiFi.softAPIP().toString().c_str());
  }

  // Setup Web Server
  server.on("/", handleRoot);
  server.on("/api/data", handleApiData);
  server.begin();
  Serial.println("[HTTP] Web server started.");
}

void loop() {
  server.handleClient();

  // Poll Modbus slave every 2 seconds
  static uint32_t lastPoll = 0;
  if (millis() - lastPoll >= 2000) {
    lastPoll = millis();
    pollModbusSlave();
  }
}
