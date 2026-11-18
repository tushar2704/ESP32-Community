/**
 * @file mqtt-dashboard.ino
 * @brief Project INT-02: MQTT IoT Telemetry & Home Assistant Bridge
 * @author Tushar Aggarwal (@tushar2704)
 * @url https://github.com/tushar2704/ESP32-Community
 * @license AGPL-3.0
 */

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* WIFI_SSID     = "YOUR_WIFI_SSID";
const char* WIFI_PASS     = "YOUR_WIFI_PASSWORD";
const char* MQTT_BROKER   = "192.168.1.100"; // Your MQTT broker IP
const int   MQTT_PORT     = 1883;
const char* MQTT_USER     = "";              // Optional
const char* MQTT_PASSWORD = "";              // Optional

const char* TOPIC_STATE   = "tele/esp32_node/state";
const char* TOPIC_SENSOR  = "tele/esp32_node/sensor";
const char* TOPIC_COMMAND = "cmnd/esp32_node/power";
const char* TOPIC_STATUS  = "stat/esp32_node/power";

WiFiClient espClient;
PubSubClient client(espClient);

bool relayState = false;
unsigned long lastMsg = 0;

void callback(char* topic, byte* payload, unsigned int length) {
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';

    Serial.printf("📥 [MQTT RX] Topic: %s | Payload: %s\n", topic, message);

    if (strcmp(topic, TOPIC_COMMAND) == 0) {
        if (strcasecmp(message, "ON") == 0) {
            relayState = true;
        } else if (strcasecmp(message, "OFF") == 0) {
            relayState = false;
        } else if (strcasecmp(message, "TOGGLE") == 0) {
            relayState = !relayState;
        }
        client.publish(TOPIC_STATUS, relayState ? "ON" : "OFF", true);
        Serial.printf("💡 Relay state updated: %s\n", relayState ? "ON" : "OFF");
    }
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Connecting to MQTT broker...");
        String clientId = "ESP32-" + String(ESP.getEfuseMac(), HEX);

        // Connect with Last Will and Testament (LWT)
        if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD, TOPIC_STATE, 1, true, "offline")) {
            Serial.println(" Connected! ✅");
            client.publish(TOPIC_STATE, "online", true);
            client.subscribe(TOPIC_COMMAND);
            client.publish(TOPIC_STATUS, relayState ? "ON" : "OFF", true);
        } else {
            Serial.printf(" Failed, rc=%d. Retrying in 5 seconds...\n", client.state());
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("\n========================================================");
    Serial.println("  ⚡ ESP32 Community — Project INT-02: MQTT Telemetry");
    Serial.println("========================================================");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(300);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi Connected!");

    client.setServer(MQTT_BROKER, MQTT_PORT);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    unsigned long now = millis();
    if (now - lastMsg > 10000) { // Publish every 10 seconds
        lastMsg = now;

        StaticJsonDocument<256> doc;
        doc["uptime_sec"] = now / 1000;
        doc["free_heap"]  = ESP.getFreeHeap();
        doc["wifi_rssi"]  = WiFi.RSSI();
        doc["temp_c"]     = 23.5f + (float)(rand() % 40) / 10.0f;
        doc["humidity"]   = 50 + (rand() % 15);
        doc["relay"]      = relayState ? "ON" : "OFF";

        char buffer[256];
        serializeJson(doc, buffer);
        client.publish(TOPIC_SENSOR, buffer);
        Serial.printf("📤 [MQTT TX] %s\n", buffer);
    }
}
