/**
 * @file ESP32_Easy.h
 * @brief Universal ESP32 helper library for beginners
 * @author Tushar Aggarwal (@tushar2704)
 * @license AGPL-3.0
 * @url https://github.com/tushar2704/ESP32-Community
 *
 * 3-line setup for any ESP32 variant. Auto-detect chip, WiFi, diagnostics.
 * Supports: ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6, ESP32-H2
 */

#ifndef ESP32_EASY_H
#define ESP32_EASY_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_system.h>
#include <esp_chip_info.h>
#include <esp_flash.h>
#include <esp_mac.h>

// Filesystem support (conditionally included in .cpp)
#include <FS.h>

/**
 * @class ESP32_Easy_Class
 * @brief Singleton class providing simple access to common ESP32 functionality.
 *
 * Usage:
 *   ESP32_Easy.begin();
 *   ESP32_Easy.printChipInfo();
 *   ESP32_Easy.connectWiFi("SSID", "PASS");
 */
class ESP32_Easy_Class {
public:
    // ─── Lifecycle ───────────────────────────────────────────────────

    /**
     * @brief Initialize the library: auto-detect chip, start Serial at 115200,
     *        and print a welcome banner with chip info.
     */
    void begin();

    // ─── Chip Information ────────────────────────────────────────────

    /**
     * @brief Print detailed chip information to Serial.
     *        Includes model, revision, cores, flash, PSRAM, MAC address.
     */
    void printChipInfo();

    /**
     * @brief Get the chip model name as a human-readable string.
     * @return String e.g. "ESP32", "ESP32-S3", "ESP32-C3"
     */
    String getChipModel();

    /**
     * @brief Get the number of CPU cores.
     * @return uint8_t Number of cores (1 or 2)
     */
    uint8_t getCoreCount();

    /**
     * @brief Get the flash memory size in megabytes.
     * @return uint32_t Flash size in MB
     */
    uint32_t getFlashSizeMB();

    /**
     * @brief Check if PSRAM (SPI RAM) is available.
     * @return true if PSRAM is detected and initialized
     */
    bool hasPSRAM();

    /**
     * @brief Get the total PSRAM size in megabytes.
     * @return uint32_t PSRAM size in MB (0 if not available)
     */
    uint32_t getPSRAMSizeMB();

    /**
     * @brief Get the free PSRAM in bytes.
     * @return uint32_t Free PSRAM bytes (0 if not available)
     */
    uint32_t getFreePSRAM();

    /**
     * @brief Get the free heap memory in bytes.
     * @return uint32_t Free heap bytes
     */
    uint32_t getFreeHeap();

    /**
     * @brief Get the internal chip temperature in °C.
     * @return float Temperature in Celsius (NaN if sensor not available)
     * @note Only available on chips with an internal temperature sensor.
     */
    float getTemperature();

    // ─── Uptime & Power ──────────────────────────────────────────────

    /**
     * @brief Get the system uptime as a human-readable string.
     * @return String formatted as "Xd Xh Xm Xs"
     */
    String getUptime();

    /**
     * @brief Enter deep sleep with a timer wakeup.
     * @param seconds Number of seconds to sleep before waking up.
     */
    void deepSleep(uint32_t seconds);

    // ─── WiFi ────────────────────────────────────────────────────────

    /**
     * @brief Connect to a WiFi network with retry logic.
     * @param ssid     Network SSID
     * @param password Network password
     * @param timeout_ms Maximum time to wait for connection (default 10000ms)
     * @return true if connected successfully
     */
    bool connectWiFi(const char* ssid, const char* password, uint32_t timeout_ms = 10000);

    /**
     * @brief Start a WiFi Access Point.
     * @param ssid     AP network name
     * @param password AP password (min 8 chars, or "" for open)
     * @return true if AP started successfully
     */
    bool startAP(const char* ssid, const char* password = "");

    /**
     * @brief Scan for nearby WiFi networks and print results to Serial.
     *        Shows SSID, RSSI with signal bars, channel, and encryption.
     * @return int Number of networks found
     */
    int scanNetworks();

    // ─── GPIO Helpers ────────────────────────────────────────────────

    /**
     * @brief Simple LED control — sets a pin HIGH or LOW.
     * @param pin   GPIO pin number
     * @param state true = HIGH (on), false = LOW (off)
     */
    void setLED(uint8_t pin, bool state);

    /**
     * @brief Read an ADC value with basic calibration.
     * @param pin GPIO pin with ADC capability
     * @return uint16_t ADC reading (0–4095 for 12-bit)
     */
    uint16_t readADC(uint8_t pin);

    // ─── Filesystem ──────────────────────────────────────────────────

    /**
     * @brief Initialize the SPIFFS filesystem.
     * @param formatOnFail If true, format SPIFFS if mount fails (default true)
     * @return true if SPIFFS mounted successfully
     */
    bool initSPIFFS(bool formatOnFail = true);

    /**
     * @brief Initialize the LittleFS filesystem.
     * @param formatOnFail If true, format LittleFS if mount fails (default true)
     * @return true if LittleFS mounted successfully
     */
    bool initLittleFS(bool formatOnFail = true);

private:
    bool _initialized = false;
    String _chipModel = "";

    /**
     * @brief Internal helper to generate signal strength bars from RSSI.
     * @param rssi WiFi signal strength in dBm
     * @return String visual bar representation
     */
    String _rssiToBars(int rssi);
};

// ─── Global Singleton Instance ───────────────────────────────────────
extern ESP32_Easy_Class ESP32_Easy;

#endif // ESP32_EASY_H
