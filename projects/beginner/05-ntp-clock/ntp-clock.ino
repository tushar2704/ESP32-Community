/**
 * @file ntp-clock.ino
 * @brief Project 05: NTP Internet Clock & Precision Timekeeper
 * @author Tushar Aggarwal (@tushar2704)
 * @url https://github.com/tushar2704/ESP32-Community
 * @license AGPL-3.0
 */

#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";

// NTP Server and POSIX Timezone String
// Examples:
// US Eastern: "EST5EDT,M3.2.0,M11.1.0"
// US Pacific: "PST8PDT,M3.2.0,M11.1.0"
// UK / London: "GMT0BST,M3.5.0/1,M10.5.0"
// UTC: "UTC0"
const char* NTP_SERVER = "pool.ntp.org";
const char* TIMEZONE = "UTC0"; // Adjust to your timezone

void printLocalTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("❌ Failed to obtain time from NTP server.");
        return;
    }

    char date_str[64];
    char time_str[64];
    strftime(date_str, sizeof(date_str), "%A, %B %d, %Y", &timeinfo);
    strftime(time_str, sizeof(time_str), "%H:%M:%S", &timeinfo);

    Serial.printf("⏰ Atomic Clock: %s | 📅 %s\n", time_str, date_str);
}

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("\n========================================================");
    Serial.println("  ⚡ ESP32 Community — Project 05: NTP Atomic Clock");
    Serial.println("========================================================");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(300);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi!");

    // Configure hardware SNTP with timezone
    configTzTime(TIMEZONE, NTP_SERVER);
    Serial.println("🛰️ Synchronizing with NTP atomic servers...");
}

void loop() {
    printLocalTime();
    delay(1000);
}
