# 📦 ESP32_Easy — Universal ESP32 Helper Library

> **The easiest way to initialize, inspect, and connect any ESP32 microcontroller in under 3 lines of code.**

`ESP32_Easy` is designed for beginners and rapid prototypers. It automatically detects your silicon model (ESP32 Classic, S2, S3, C3, C6, H2), queries available PSRAM/Flash memory, formats Wi-Fi connection diagnostics with visual ASCII signal bars, and provides one-line deep sleep and filesystem helpers.

---

## ⚡ 3-Line Quick Start

```cpp
#include <ESP32_Easy.h>

void setup() {
    ESP32_Easy.begin();                          // Detects chip, starts 115200 baud serial, prints diagnostics
    ESP32_Easy.connectWiFi("MyWiFi", "Secret");   // Connects with retry & prints IP/RSSI bars
    ESP32_Easy.scanNetworks();                   // Visual 2.4 GHz spectrum scan
}

void loop() {
    Serial.printf("⏱️ Uptime: %s | Free Heap: %u bytes\n", 
                  ESP32_Easy.getUptime().c_str(), 
                  ESP32_Easy.getFreeHeap());
    delay(5000);
}
```

---

## 📖 API Reference

| Method | Return Type | Description |
| :--- | :--- | :--- |
| `begin()` | `void` | Starts Serial at 115200 baud and prints banner with hardware specs. |
| `printChipInfo()` | `void` | Prints model, revision, cores, frequency, flash, PSRAM, and MAC address. |
| `getChipModel()` | `String` | Returns friendly string (e.g., `"ESP32-S3"` or `"ESP32 (Classic)"`). |
| `getCoreCount()` | `uint8_t` | Returns number of CPU cores (1 or 2). |
| `getFlashSizeMB()` | `uint32_t` | Returns flash memory capacity in Megabytes (4, 8, 16, 32). |
| `hasPSRAM()` | `bool` | Returns `true` if external SPI/Octal PSRAM is detected and ready. |
| `getPSRAMSizeMB()` | `uint32_t` | Returns total PSRAM capacity in Megabytes. |
| `getFreePSRAM()` | `uint32_t` | Returns free PSRAM in bytes. |
| `getFreeHeap()` | `uint32_t` | Returns free internal SRAM heap in bytes. |
| `getTemperature()` | `float` | Returns internal silicon temperature in °C (NaN if unsupported). |
| `getUptime()` | `String` | Formats uptime as `"1d 4h 12m 30s"`. |
| `connectWiFi(ssid, pass, timeout)` | `bool` | Connects to Wi-Fi station with status logging and timeout. |
| `startAP(ssid, pass)` | `bool` | Launches a soft Access Point. |
| `scanNetworks()` | `int` | Scans and prints nearby Wi-Fi SSIDs with signal bars (`[||||]`). |
| `deepSleep(seconds)` | `void` | Configures timer wakeup and enters ultra-low power deep sleep. |
| `setLED(pin, state)` | `void` | Sets pin mode to OUTPUT and writes digital level. |
| `initSPIFFS(formatOnFail)` | `bool` | Mounts SPIFFS filesystem. |
| `initLittleFS(formatOnFail)` | `bool` | Mounts LittleFS filesystem. |

---

<div align="center">

**Part of the [ESP32 Community Hub](https://github.com/tushar2704/ESP32-Community)**  
<sub>Author: Tushar Aggarwal ([@tushar2704](https://github.com/tushar2704)) • License: AGPLv3</sub>

</div>
