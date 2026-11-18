/**
 * @file internet-radio.ino
 * @brief Project INT-11: Wi-Fi Internet Radio & Web Streamer
 * @author Tushar Aggarwal (@tushar2704)
 * @url https://github.com/tushar2704/ESP32-Community
 * @license AGPL-3.0
 */

#include <Arduino.h>
#include <WiFi.h>
#include "Audio.h"

const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";

// Digital I2S DAC Pins (MAX98357A / PCM5102A)
#if defined(CONFIG_IDF_TARGET_ESP32S3)
    #define I2S_BCLK 4
    #define I2S_LRC  5
    #define I2S_DOUT 6
#else
    #define I2S_BCLK 26
    #define I2S_LRC  25
    #define I2S_DOUT 22
#endif

Audio audio;

// Radio Station Presets
const char* STATIONS[] = {
    "http://stream.radioparadise.com/mp3-128",            // Radio Paradise (Main Mix)
    "http://stream.radioparadise.com/mellow-128",          // Radio Paradise (Mellow)
    "http://us2.internet-radio.com:8050/",                // Classic Rock Florida
    "http://icecast.omroep.nl/3fm-bb-mp3"                 // NPO 3FM
};
const int NUM_STATIONS = sizeof(STATIONS) / sizeof(STATIONS[0]);
int currentStation = 0;

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("\n========================================================");
    Serial.println("  ⚡ ESP32 Community — Project INT-11: Wi-Fi Internet Radio");
    Serial.println("========================================================");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(300);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi!");

    // Configure I2S DAC Audio output
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(12); // Volume level 0 to 21

    Serial.printf("📻 Connecting to Stream: %s\n", STATIONS[currentStation]);
    audio.connecttohost(STATIONS[currentStation]);
}

void loop() {
    audio.loop();

    // Check for serial console commands to change station or volume
    if (Serial.available()) {
        char cmd = Serial.read();
        if (cmd == 'n') {
            currentStation = (currentStation + 1) % NUM_STATIONS;
            Serial.printf("📻 Switching to Station [%d]: %s\n", currentStation, STATIONS[currentStation]);
            audio.connecttohost(STATIONS[currentStation]);
        } else if (cmd == '+') {
            audio.setVolume(audio.getVolume() + 1);
            Serial.printf("🔊 Volume: %d\n", audio.getVolume());
        } else if (cmd == '-') {
            audio.setVolume(audio.getVolume() - 1);
            Serial.printf("🔉 Volume: %d\n", audio.getVolume());
        }
    }
}

// Audio Engine Status Callbacks
void audio_info(const char *info) {
    Serial.printf("ℹ️ [Audio Info] %s\n", info);
}

void audio_showstation(const char *info) {
    Serial.printf("📡 [Station] %s\n", info);
}

void audio_showstreamtitle(const char *info) {
    Serial.printf("🎵 [Now Playing] %s\n", info);
}
