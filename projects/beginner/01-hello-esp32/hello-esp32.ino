/**
 * @file hello-esp32.ino
 * @brief Project 01: Universal Hardware Diagnostic & Baseline Test
 * @author Tushar Aggarwal (@tushar2704)
 * @url https://github.com/tushar2704/ESP32-Community
 * @license AGPL-3.0
 */

#include <Arduino.h>
#include <WiFi.h>
#include <esp_system.h>
#include <esp_chip_info.h>
#include <esp_flash.h>
#include <esp_mac.h>

// Onboard LED pin definition (GPIO 2 on classic ESP32, GPIO 8 on C3/C6, GPIO 48 on S3)
#if defined(CONFIG_IDF_TARGET_ESP32S3)
    #define LED_PIN 48
#elif defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C6)
    #define LED_PIN 8
#else
    #define LED_PIN 2
#endif

// Helper to convert RSSI to visual bars
const char* rssiToBars(int rssi) {
    if (rssi >= -55) return "[||||]";
    if (rssi >= -65) return "[|||.]";
    if (rssi >= -75) return "[||..]";
    if (rssi >= -85) return "[|...]";
    return "[....]";
}

void printSystemDiagnostics() {
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);

    uint32_t flash_size = 0;
    esp_flash_get_size(NULL, &flash_size);

    Serial.println("\n==================================================");
    Serial.println("  ⚡ ESP32 Community — Project 01: Hello ESP32");
    Serial.println("  https://github.com/tushar2704/ESP32-Community");
    Serial.println("==================================================");
    Serial.println("🔍 Hardware Diagnostics:");
    Serial.printf("  - Chip Model:       %s (Rev %d)\n", ESP.getChipModel(), chip_info.revision);
    Serial.printf("  - CPU Cores:        %d core(s) @ %d MHz\n", chip_info.cores, getCpuFrequencyMhz());
    Serial.printf("  - Flash Memory:     %u MB\n", flash_size / (1024 * 1024));
    Serial.printf("  - PSRAM:            %s\n", psramFound() ? "Detected & Active ✅" : "Not Present ❌");
    if (psramFound()) {
        Serial.printf("  - PSRAM Capacity:   %u MB (%u bytes free)\n", 
                      ESP.getPsramSize() / (1024 * 1024), ESP.getFreePsram());
    }
    Serial.printf("  - Free SRAM Heap:   %u bytes\n", ESP.getFreeHeap());
    Serial.printf("  - MAC Address:      %02X:%02X:%02X:%02X:%02X:%02X\n", 
                  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    Serial.println("--------------------------------------------------");
}

void scanWiFiNetworks() {
    Serial.println("📡 Scanning 2.4 GHz Wi-Fi Spectrum...");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    int num_networks = WiFi.scanNetworks();
    if (num_networks == 0) {
        Serial.println("  No networks discovered.");
    } else {
        Serial.printf("  Discovered %d network(s):\n", num_networks);
        for (int i = 0; i < num_networks; ++i) {
            Serial.printf("  [%2d] %-24s | RSSI: %3d dBm %-5s | Ch: %2d | %s\n",
                          i + 1,
                          WiFi.SSID(i).substring(0, 24).c_str(),
                          WiFi.RSSI(i),
                          rssiToBars(WiFi.RSSI(i)),
                          WiFi.channel(i),
                          (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open 🔓" : "Secured 🔒");
            delay(10);
        }
    }
    Serial.println("--------------------------------------------------");
}

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.begin(115200);
    delay(1000);

    printSystemDiagnostics();
    scanWiFiNetworks();

    Serial.printf("💡 Diagnostic heartbeat running on GPIO %d (Toggling every 1s)...\n\n", LED_PIN);
}

void loop() {
    static bool led_state = false;
    led_state = !led_state;
    digitalWrite(LED_PIN, led_state ? HIGH : LOW);

    uint32_t uptime_sec = millis() / 1000;
    Serial.printf("[⏱️ Uptime: %02uh %02um %02us] Heartbeat tick | Free Heap: %u bytes\n", 
                  uptime_sec / 3600, (uptime_sec % 3600) / 60, uptime_sec % 60, ESP.getFreeHeap());

    delay(1000);
}
