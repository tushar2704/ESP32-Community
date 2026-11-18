/**
 * @file ESP32_Easy.cpp
 * @brief Implementation of the universal ESP32 helper library
 * @author Tushar Aggarwal (@tushar2704)
 * @license AGPL-3.0
 * @url https://github.com/tushar2704/ESP32-Community
 */

#include "ESP32_Easy.h"
#include <SPIFFS.h>
#include <LittleFS.h>

#if __has_include(<driver/temperature_sensor.h>)
#include <driver/temperature_sensor.h>
#define HAS_TEMP_SENSOR 1
#endif

ESP32_Easy_Class ESP32_Easy;

void ESP32_Easy_Class::begin() {
    if (_initialized) return;

    Serial.begin(115200);
    delay(500);

    Serial.println("\n==================================================");
    Serial.println("  ⚡ ESP32 Community — ESP32_Easy Library v1.0.0");
    Serial.println("  https://github.com/tushar2704/ESP32-Community");
    Serial.println("==================================================");

    printChipInfo();
    _initialized = true;
}

String ESP32_Easy_Class::getChipModel() {
    if (_chipModel.length() > 0) return _chipModel;

    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    switch (chip_info.model) {
        case CHIP_ESP32:   _chipModel = "ESP32 (Classic)"; break;
        case CHIP_ESP32S2: _chipModel = "ESP32-S2"; break;
        case CHIP_ESP32S3: _chipModel = "ESP32-S3"; break;
        case CHIP_ESP32C3: _chipModel = "ESP32-C3"; break;
#ifdef CHIP_ESP32C6
        case CHIP_ESP32C6: _chipModel = "ESP32-C6"; break;
#endif
#ifdef CHIP_ESP32H2
        case CHIP_ESP32H2: _chipModel = "ESP32-H2"; break;
#endif
#ifdef CHIP_ESP32P4
        case CHIP_ESP32P4: _chipModel = "ESP32-P4"; break;
#endif
        default:           _chipModel = "ESP32 (Unknown/Generic)"; break;
    }
    return _chipModel;
}

uint8_t ESP32_Easy_Class::getCoreCount() {
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    return chip_info.cores;
}

uint32_t ESP32_Easy_Class::getFlashSizeMB() {
    uint32_t flash_size = 0;
    esp_flash_get_size(NULL, &flash_size);
    return flash_size / (1024 * 1024);
}

bool ESP32_Easy_Class::hasPSRAM() {
    return psramFound();
}

uint32_t ESP32_Easy_Class::getPSRAMSizeMB() {
    if (!psramFound()) return 0;
    return ESP.getPsramSize() / (1024 * 1024);
}

uint32_t ESP32_Easy_Class::getFreePSRAM() {
    if (!psramFound()) return 0;
    return ESP.getFreePsram();
}

uint32_t ESP32_Easy_Class::getFreeHeap() {
    return ESP.getFreeHeap();
}

float ESP32_Easy_Class::getTemperature() {
#if defined(HAS_TEMP_SENSOR)
    static temperature_sensor_handle_t temp_sensor = NULL;
    if (temp_sensor == NULL) {
        temperature_sensor_config_t temp_sensor_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(10, 50);
        if (temperature_sensor_install(&temp_sensor_config, &temp_sensor) != ESP_OK) {
            return NAN;
        }
        temperature_sensor_enable(temp_sensor);
    }
    float tsens_out = 0.0;
    if (temperature_sensor_get_celsius(temp_sensor, &tsens_out) == ESP_OK) {
        return tsens_out;
    }
    return NAN;
#else
    return NAN;
#endif
}

String ESP32_Easy_Class::getUptime() {
    uint32_t total_sec = millis() / 1000;
    uint32_t days = total_sec / 86400;
    uint32_t hours = (total_sec % 86400) / 3600;
    uint32_t mins = (total_sec % 3600) / 60;
    uint32_t secs = total_sec % 60;

    char buf[32];
    if (days > 0) {
        snprintf(buf, sizeof(buf), "%ud %uh %um %us", days, hours, mins, secs);
    } else if (hours > 0) {
        snprintf(buf, sizeof(buf), "%uh %um %us", hours, mins, secs);
    } else {
        snprintf(buf, sizeof(buf), "%um %us", mins, secs);
    }
    return String(buf);
}

void ESP32_Easy_Class::deepSleep(uint32_t seconds) {
    Serial.printf("💤 Entering deep sleep for %u seconds...\n", seconds);
    Serial.flush();
    esp_sleep_enable_timer_wakeup((uint64_t)seconds * 1000000ULL);
    esp_deep_sleep_start();
}

void ESP32_Easy_Class::printChipInfo() {
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);

    Serial.println(" Hardware Diagnostics:");
    Serial.printf("  - Chip Model:       %s (Rev %d)\n", getChipModel().c_str(), chip_info.revision);
    Serial.printf("  - CPU Cores:        %d core(s)\n", chip_info.cores);
    Serial.printf("  - CPU Frequency:    %d MHz\n", getCpuFrequencyMhz());
    Serial.printf("  - Flash Memory:     %u MB\n", getFlashSizeMB());
    Serial.printf("  - PSRAM:            %s (%u MB total, %u bytes free)\n", 
                  hasPSRAM() ? "Detected ✅" : "Not Present ❌", getPSRAMSizeMB(), getFreePSRAM());
    Serial.printf("  - Free Heap:        %u bytes\n", getFreeHeap());
    Serial.printf("  - MAC Address:      %02X:%02X:%02X:%02X:%02X:%02X\n", 
                  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    
    float temp = getTemperature();
    if (!isnan(temp)) {
        Serial.printf("  - Chip Temp:        %.1f °C\n", temp);
    }
    Serial.println("--------------------------------------------------");
}

bool ESP32_Easy_Class::connectWiFi(const char* ssid, const char* password, uint32_t timeout_ms) {
    Serial.printf("🌐 Connecting to Wi-Fi SSID: '%s' ...\n", ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    uint32_t start_time = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - start_time) < timeout_ms) {
        delay(250);
        Serial.print(".");
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println(" Connected successfully!");
        Serial.printf("  - IP Address:       %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("  - Signal (RSSI):    %d dBm %s\n", WiFi.RSSI(), _rssiToBars(WiFi.RSSI()).c_str());
        Serial.printf("  - Gateway:          %s\n", WiFi.gatewayIP().toString().c_str());
        Serial.printf("  - DNS:              %s\n", WiFi.dnsIP().toString().c_str());
        return true;
    } else {
        Serial.println("❌ Failed to connect to Wi-Fi (Timeout).");
        return false;
    }
}

bool ESP32_Easy_Class::startAP(const char* ssid, const char* password) {
    Serial.printf("📡 Starting Wi-Fi AP: '%s' ...\n", ssid);
    WiFi.mode(WIFI_AP);
    bool success = (strlen(password) >= 8) ? WiFi.softAP(ssid, password) : WiFi.softAP(ssid);
    if (success) {
        Serial.printf(" Access Point live at IP: %s\n", WiFi.softAPIP().toString().c_str());
    } else {
        Serial.println("❌ Failed to start Access Point.");
    }
    return success;
}

int ESP32_Easy_Class::scanNetworks() {
    Serial.println("🔍 Scanning 2.4 GHz Wi-Fi spectrum...");
    int n = WiFi.scanNetworks();
    Serial.printf(" Found %d network(s):\n", n);
    for (int i = 0; i < n; ++i) {
        Serial.printf("  [%2d] %-24s | RSSI: %3d dBm %-5s | Ch: %2d | %s\n",
                      i + 1,
                      WiFi.SSID(i).substring(0, 24).c_str(),
                      WiFi.RSSI(i),
                      _rssiToBars(WiFi.RSSI(i)).c_str(),
                      WiFi.channel(i),
                      (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open 🔓" : "Secured 🔒");
        delay(10);
    }
    Serial.println("--------------------------------------------------");
    return n;
}

void ESP32_Easy_Class::setLED(uint8_t pin, bool state) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, state ? HIGH : LOW);
}

uint16_t ESP32_Easy_Class::readADC(uint8_t pin) {
    return analogRead(pin);
}

bool ESP32_Easy_Class::initSPIFFS(bool formatOnFail) {
    if (!SPIFFS.begin(formatOnFail)) {
        Serial.println("❌ SPIFFS Mount Failed.");
        return false;
    }
    Serial.printf("📂 SPIFFS Mounted: %u / %u bytes used\n", SPIFFS.usedBytes(), SPIFFS.totalBytes());
    return true;
}

bool ESP32_Easy_Class::initLittleFS(bool formatOnFail) {
    if (!LittleFS.begin(formatOnFail)) {
        Serial.println("❌ LittleFS Mount Failed.");
        return false;
    }
    Serial.printf("📂 LittleFS Mounted: %u / %u bytes used\n", LittleFS.usedBytes(), LittleFS.totalBytes());
    return true;
}

String ESP32_Easy_Class::_rssiToBars(int rssi) {
    if (rssi >= -55) return "[||||]";
    if (rssi >= -65) return "[|||.]";
    if (rssi >= -75) return "[||..]";
    if (rssi >= -85) return "[|...]";
    return "[....]";
}
