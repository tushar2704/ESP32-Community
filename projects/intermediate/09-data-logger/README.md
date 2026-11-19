# 💾 MicroSD Telemetry Data Logger & Web File Server

[![PlatformIO CI](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml/badge.svg)](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml)
[![License: AGPL-3.0](https://img.shields.io/badge/License-AGPL--3.0-blue.svg)](LICENSE)
[![Hardware: ESP32](https://img.shields.io/badge/Hardware-ESP32%20%7C%20S3%20%7C%20C3-red.svg)](hardware/)

A high-speed FAT32 MicroSD card data logger built on the ESP32. Records environmental, hardware, and network telemetry at up to 100 Hz in standard CSV format, and hosts a web dashboard with real-time record statistics and 1-click browser CSV download.

---

## 📸 Key Features

- **FAT32 MicroSD Card Storage**: Supports standard SD, SDHC, and SDXC micro-cards up to 64GB.
- **Structured CSV Logging**: Writes timestamped rows (`timestamp_ms`, `temperature_c`, `free_heap_bytes`, `wifi_rssi_dbm`) ready for immediate analysis in Excel, Python/Pandas, or Grafana.
- **Embedded Web File Server**:
  - `GET /` — Real-time telemetry record count & SD card capacity dashboard.
  - `GET /download` — Downloads the active `telemetry.csv` file directly to your smartphone or PC.
  - `POST /clear` — Rotates or wipes the active log file without removing the physical card.
- **LED Activity Heartbeat**: Status LED blinks briefly on every physical block write.

---

## 🛠️ Hardware Requirements

| Component | Quantity | Notes |
|:---|:---:|:---|
| **ESP32 Dev Module** | 1 | ESP32-WROOM-32, ESP32-S3, or ESP32-C3 |
| **MicroSD SPI Breakout Module** | 1 | Standard 5V/3.3V SPI interface |
| **MicroSD Card** | 1 | Formatted as FAT32 |

---

## 🔌 Pin Connections

| Signal | ESP32-WROOM-32 | ESP32-S3 | ESP32-C3 | MicroSD Module Pin |
|:---|:---:|:---:|:---:|:---|
| **SPI CS** | `GPIO 5` | `GPIO 10` | `GPIO 7` | **CS** |
| **SPI MOSI** | `GPIO 23` | `GPIO 11` | `GPIO 6` | **MOSI / DI** |
| **SPI MISO** | `GPIO 19` | `GPIO 13` | `GPIO 5` | **MISO / DO** |
| **SPI SCK** | `GPIO 18` | `GPIO 12` | `GPIO 4` | **CLK / SCK** |
| **Status LED** | `GPIO 2` | `GPIO 2` | `GPIO 8` | Onboard / External LED |
| **VCC & GND** | `3.3V` / `GND` | `3.3V` / `GND` | `3.3V` / `GND` | Power Rails |

---

## 🚀 Quick Start

1. Open this directory in PlatformIO:
   ```bash
   cd projects/intermediate/09-data-logger
   ```
2. Insert a FAT32-formatted MicroSD card into the module.
3. Set your WiFi credentials in `data-logger.ino`:
   ```cpp
   const char* default_ssid = "YOUR_WIFI_SSID";
   const char* default_password = "YOUR_WIFI_PASSWORD";
   ```
4. Compile and flash your board:
   ```bash
   pio run -t upload -e esp32dev
   ```
5. Navigate to the ESP32 IP address and click **📥 DOWNLOAD TELEMETRY CSV**.

---

## 📺 Community & Credits

- Maintained by [Tushar Aggarwal](https://github.com/tushar2704)
- Tutorials & Video Guides: [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://youtube.com/@Makerrdesigns)
- Licensed under [AGPL-3.0](LICENSE).
