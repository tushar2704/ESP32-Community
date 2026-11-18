/**
 * @file wifi-scanner.ino
 * @brief Project 02: 2.4 GHz Spectrum Scanner & Network Auditor
 * @author Tushar Aggarwal (@tushar2704)
 * @url https://github.com/tushar2704/ESP32-Community
 * @license AGPL-3.0
 */

#include <Arduino.h>
#include <WiFi.h>

const char* getEncryptionType(wifi_auth_mode_t authType) {
    switch (authType) {
        case WIFI_AUTH_OPEN:            return "Open [None]";
        case WIFI_AUTH_WEP:             return "WEP [Insecure]";
        case WIFI_AUTH_WPA_PSK:         return "WPA-PSK";
        case WIFI_AUTH_WPA2_PSK:        return "WPA2-PSK";
        case WIFI_AUTH_WPA_WPA2_PSK:    return "WPA/WPA2-PSK";
        case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-Enterprise";
        case WIFI_AUTH_WPA3_PSK:        return "WPA3-SAE [Modern]";
        case WIFI_AUTH_WPA2_WPA3_PSK:   return "WPA2/WPA3 Mixed";
        default:                        return "Unknown";
    }
}

const char* getSignalBar(int rssi) {
    if (rssi >= -50) return "████ (Excellent)";
    if (rssi >= -65) return "███░ (Good)";
    if (rssi >= -75) return "██░░ (Fair)";
    if (rssi >= -85) return "█░░░ (Weak)";
    return "░░░░ (Very Poor)";
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n========================================================");
    Serial.println("  ⚡ ESP32 Community — Project 02: 2.4 GHz Wi-Fi Scanner");
    Serial.println("========================================================");

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
}

void loop() {
    Serial.println("\n📡 Initiating wireless channel sweep...");
    uint32_t scan_start = millis();
    int n = WiFi.scanNetworks(false, true); // show hidden SSIDs = true
    uint32_t duration = millis() - scan_start;

    Serial.printf("✨ Discovered %d access point(s) in %u ms:\n\n", n, duration);
    Serial.println("  #  | SSID                     | BSSID             | Ch | RSSI (dBm) | Signal Quality    | Encryption");
    Serial.println("-----+--------------------------+-------------------+----+------------+-------------------+------------------");

    for (int i = 0; i < n; ++i) {
        String ssid = WiFi.SSID(i);
        if (ssid.length() == 0) ssid = "<Hidden SSID>";

        Serial.printf(" %2d  | %-24s | %17s | %2d | %7d dBm | %-17s | %s\n",
                      i + 1,
                      ssid.substring(0, 24).c_str(),
                      WiFi.BSSIDstr(i).c_str(),
                      WiFi.channel(i),
                      WiFi.RSSI(i),
                      getSignalBar(WiFi.RSSI(i)),
                      getEncryptionType(WiFi.encryptionType(i)));
        delay(10);
    }
    Serial.println("-------------------------------------------------------------------------------------------------------");

    WiFi.scanDelete(); // Free memory
    Serial.println("⏳ Pausing 10 seconds before next scan sweep...\n");
    delay(10000);
}
