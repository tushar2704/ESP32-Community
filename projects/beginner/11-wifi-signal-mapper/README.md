# 📡 WiFi Signal Mapper & Channel Crowding Analyzer

[![PlatformIO CI](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml/badge.svg)](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml)
[![License: AGPL-3.0](https://img.shields.io/badge/License-AGPL--3.0-blue.svg)](LICENSE)
[![Hardware: ESP32](https://img.shields.io/badge/Hardware-ESP32%20%7C%20S3%20%7C%20C3-red.svg)](hardware/)

A pocket-sized 2.4 GHz WiFi spectral analyzer and RSSI field heatmapper powered by the ESP32 and an I2C SSD1306 OLED display. Features live AP scanning, channel crowding histograms, automatic least-congested channel recommendation (1, 6, 11), and an acoustic "Geiger-counter" signal locator.

---

## 📸 Key Features

- **Multi-View OLED Display**: Automatically cycles through 3 diagnostic pages every 4 seconds:
  1. **Network Scanner List**: Displays detected SSIDs, channels, and signal strengths in dBm.
  2. **Channel Congestion Histogram**: Visualizes AP density across all 13 channels with optimal non-overlapping channel recommendation.
  3. **Acoustic Geiger Tracker**: Tracks physical distance to the strongest or target WiFi router by beeping/clicking faster as RSSI improves.
- **2.4 GHz Spectrum Optimization**: Helps pinpoint channel interference from neighboring routers and microwave ovens.
- **Portable & Battery Friendly**: Operates directly from a LiPo or USB power bank.

---

## 🛠️ Hardware Requirements

| Component | Quantity | Notes |
|:---|:---:|:---|
| **ESP32 Dev Module** | 1 | ESP32-WROOM-32, ESP32-S3, or ESP32-C3 |
| **0.96" SSD1306 OLED Display** | 1 | 128x64 resolution, I2C interface (0x3C) |
| **Active Piezo Buzzer** | 1 | 3.3V or 5V buzzer for Geiger acoustic clicks |
| **Breadboard & Jumper Wires** | 1 | Standard prototyping setup |

---

## 🔌 Pin Connections

| Signal | ESP32-WROOM-32 | ESP32-S3 | ESP32-C3 | OLED / Buzzer Pin |
|:---|:---:|:---:|:---:|:---|
| **I2C SDA** | `GPIO 21` | `GPIO 8` | `GPIO 4` | **SDA** |
| **I2C SCL** | `GPIO 22` | `GPIO 9` | `GPIO 5` | **SCL** |
| **Piezo Buzzer (+)** | `GPIO 25` | `GPIO 12` | `GPIO 6` | **Positive / SIG** |
| **GND** | `GND` | `GND` | `GND` | **GND** (Common) |
| **VCC** | `3.3V` | `3.3V` | `3.3V` | **VCC** |

---

## 🚀 Quick Start

1. Open this directory in PlatformIO:
   ```bash
   cd projects/beginner/11-wifi-signal-mapper
   ```
2. Build and upload firmware:
   ```bash
   pio run -t upload -e esp32dev
   ```
3. Open the Serial Monitor (115200 baud) or watch the OLED screen as the device scans and graphs nearby networks.

---

## 📺 Community & Credits

- Maintained by [Tushar Aggarwal](https://github.com/tushar2704)
- Tutorials & Video Guides: [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://youtube.com/@Makerrdesigns)
- Licensed under [AGPL-3.0](LICENSE).
