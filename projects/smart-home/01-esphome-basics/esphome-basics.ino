/**
 * ESP32-Community: Home Assistant MQTT Auto-Discovery Smart Node
 * Automatically publishes MQTT Discovery payloads conforming to Home Assistant standards,
 * enabling zero-config integration for relays and onboard telemetry.
 *
 * Compatible with ESP32-WROOM-32, ESP32-S3, ESP32-C3.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// --- Pin Definitions ---
#if defined(CONFIG_IDF_TARGET_ESP32C3)
  #define PIN_RELAY 4
  #define PIN_LED   8
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
  #define PIN_RELAY 10
  #define PIN_LED   2
#else
  #define PIN_RELAY 26
  #define PIN_LED   2
#endif

// --- Network & MQTT Settings ---
const char* default_ssid = "YOUR_WIFI_SSID";
const char* default_password = "YOUR_WIFI_PASSWORD";
const char* mqtt_server = "homeassistant.local"; // or Home Assistant IP
const int   mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

String deviceId;
String stateTopic;
String commandTopic;
String discoveryTopic;

bool relayState = false;
unsigned long lastTelemetry = 0;

void publishDiscovery() {
  StaticJsonDocument<512> doc;
  doc["name"] = "Smart Relay";
  doc["unique_id"] = deviceId + "_relay";
  doc["cmd_t"] = commandTopic;
  doc["stat_t"] = stateTopic;
  doc["pl_on"] = "ON";
  doc["pl_off"] = "OFF";

  JsonObject dev = doc.createNestedObject("device");
  JsonArray ids = dev.createNestedArray("identifiers");
  ids.add(deviceId);
  dev["name"] = "ESP32 Community Node";
  dev["model"] = "ESP32-SmartNode";
  dev["manufacturer"] = "ESP32-Community";

  String payload;
  serializeJson(doc, payload);
  client.publish(discoveryTopic.c_str(), payload.c_str(), true);
  Serial.println("📢 Home Assistant MQTT Auto-Discovery payload broadcasted!");
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) message += (char)payload[i];
  Serial.printf("Incoming MQTT [%s]: %s\n", topic, message.c_str());

  if (message == "ON") {
    relayState = true;
    digitalWrite(PIN_RELAY, HIGH);
    digitalWrite(PIN_LED, HIGH);
    client.publish(stateTopic.c_str(), "ON", true);
  } else if (message == "OFF") {
    relayState = false;
    digitalWrite(PIN_RELAY, LOW);
    digitalWrite(PIN_LED, LOW);
    client.publish(stateTopic.c_str(), "OFF", true);
  }
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to Home Assistant MQTT Broker...");
    if (client.connect(deviceId.c_str())) {
      Serial.println(" connected! ✅");
      client.subscribe(commandTopic.c_str());
      publishDiscovery();
      client.publish(stateTopic.c_str(), relayState ? "ON" : "OFF", true);
    } else {
      Serial.printf(" failed, rc=%d. Retrying in 5 seconds...\n", client.state());
      delay(5000);
      break;
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n[ESP32 Home Assistant Auto-Discovery Node]");

  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, LOW);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  uint8_t mac[6];
  WiFi.macAddress(mac);
  char idBuf[32];
  snprintf(idBuf, sizeof(idBuf), "esp32_%02X%02X%02X", mac[3], mac[4], mac[5]);
  deviceId = String(idBuf);

  stateTopic = "homeassistant/switch/" + deviceId + "/state";
  commandTopic = "homeassistant/switch/" + deviceId + "/set";
  discoveryTopic = "homeassistant/switch/" + deviceId + "/config";

  WiFi.mode(WIFI_STA);
  WiFi.begin(default_ssid, default_password);
  Serial.printf("Connecting to WiFi: %s", default_ssid);

  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 8000) {
    delay(250);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\nConnected! IP: %s\n", WiFi.localIP().toString().c_str());
  }

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      reconnectMQTT();
    }
    client.loop();
  }

  delay(10);
}
