/*
 * ESP32-Community | Beginner Project 10: Serial JSON Command API
 * 
 * Provides an industrial-style JSON RPC interface over USB-Serial.
 * Send structured JSON requests and receive instant JSON responses.
 * Perfect for Python host scripts, Node-RED, Electron apps, or web dashboards.
 * 
 * Supported Commands:
 * - {"cmd":"ping"}
 * - {"cmd":"status"}
 * - {"cmd":"set_led", "state": true|false}
 * - {"cmd":"system_info"}
 * - {"cmd":"restart"}
 * 
 * Part of ESP32-Community Hub: https://github.com/tushar2704/ESP32-Community
 * Video Tutorials & Community: @IamNotIntoTech | @Makerrdesigns
 */

#include <Arduino.h>
#include <ArduinoJson.h>
#include <esp_system.h>

#ifndef LED_BUILTIN
  #define LED_BUILTIN 2
#endif

bool ledState = false;

void processCommand(const String &jsonString) {
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, jsonString);

  StaticJsonDocument<512> response;

  if (error) {
    response["status"] = "error";
    response["message"] = "Invalid JSON payload";
    response["code"] = 400;
    serializeJson(response, Serial);
    Serial.println();
    return;
  }

  const char *cmd = doc["cmd"] | "";

  if (strcmp(cmd, "ping") == 0) {
    response["status"] = "ok";
    response["reply"] = "pong";
    response["uptime_ms"] = millis();
  } 
  else if (strcmp(cmd, "status") == 0) {
    response["status"] = "ok";
    response["led"] = ledState;
    response["uptime_sec"] = millis() / 1000;
    response["heap_free_bytes"] = ESP.getFreeHeap();
    response["heap_min_bytes"] = ESP.getMinFreeHeap();
  }
  else if (strcmp(cmd, "set_led") == 0) {
    if (doc.containsKey("state")) {
      ledState = doc["state"].as<bool>();
      digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);
      response["status"] = "ok";
      response["led"] = ledState;
    } else {
      response["status"] = "error";
      response["message"] = "Missing 'state' boolean field";
      response["code"] = 422;
    }
  }
  else if (strcmp(cmd, "system_info") == 0) {
    response["status"] = "ok";
    response["chip_model"] = ESP.getChipModel();
    response["chip_revision"] = ESP.getChipRevision();
    response["chip_cores"] = ESP.getChipCores();
    response["cpu_freq_mhz"] = ESP.getCpuFreqMHz();
    response["flash_size_bytes"] = ESP.getFlashChipSize();
    response["sdk_version"] = ESP.getSdkVersion();
  }
  else if (strcmp(cmd, "restart") == 0) {
    response["status"] = "ok";
    response["message"] = "ESP32 will restart in 1 second";
    serializeJson(response, Serial);
    Serial.println();
    delay(1000);
    ESP.restart();
    return;
  }
  else {
    response["status"] = "error";
    response["message"] = "Unknown command. Available: ping, status, set_led, system_info, restart";
    response["code"] = 404;
  }

  // Print serialized JSON reply followed by newline delimiter
  serializeJson(response, Serial);
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Buffer input lines
  Serial.setTimeout(100);

  delay(500);
  Serial.println("{\"ready\":true,\"system\":\"ESP32-Community-Serial-API\",\"baud\":115200}");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() > 0) {
      processCommand(input);
    }
  }
}
