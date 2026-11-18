/*
 * ESP32-Community | Beginner Project 15: ArduinoOTA Wireless Firmware Updates
 * 
 * Update your ESP32 firmware over Wi-Fi without plugging in a USB cable!
 * Uses ArduinoOTA and mDNS ("esp32-ota.local").
 * 
 * Part of ESP32-Community Hub: https://github.com/tushar2704/ESP32-Community
 * Video Tutorials & Community: @IamNotIntoTech | @Makerrdesigns
 */

#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// Enter your Wi-Fi credentials here
const char *SSID = "YOUR_WIFI_SSID";
const char *PASSWORD = "YOUR_WIFI_PASSWORD";

// Optional OTA password for secure wireless flashing
const char *OTA_PASSWORD = "admin";

#ifndef LED_BUILTIN
  #define LED_BUILTIN 2
#endif

void setupOTA() {
  // Hostname for mDNS: will advertise as esp32-ota.local
  ArduinoOTA.setHostname("esp32-ota");

  // Optional authentication password
  ArduinoOTA.setPassword(OTA_PASSWORD);

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    Serial.println("\n[OTA] Start updating " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\n[OTA] Update Complete! Rebooting...");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("[OTA] Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("[OTA] Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();
  Serial.println("[OTA] Service Ready! You can now flash wirelessly via Arduino IDE or PlatformIO.");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("\n==================================================");
  Serial.println("     ESP32 Community: ArduinoOTA Wireless Demo    ");
  Serial.println("==================================================");

  // Connect to Wi-Fi
  Serial.printf("Connecting to %s ", SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 20) {
    delay(500);
    Serial.print(".");
    timeout++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WiFi] Connected successfully!");
    Serial.print("[WiFi] IP Address: ");
    Serial.println(WiFi.localIP());
    setupOTA();
  } else {
    Serial.println("\n[WiFi] Connection timed out. OTA will not start.");
  }
}

void loop() {
  // Handle incoming OTA requests
  ArduinoOTA.handle();

  // Heartbeat blink to verify code execution
  static unsigned long lastBlink = 0;
  static bool ledState = false;
  if (millis() - lastBlink > 1000) {
    lastBlink = millis();
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);
  }
}
