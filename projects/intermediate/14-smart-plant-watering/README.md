# 🌱 Smart Flora: Automated Plant Watering System

[![PlatformIO CI](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml/badge.svg)](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml)
[![License: AGPL-3.0](https://img.shields.io/badge/License-AGPL--3.0-blue.svg)](LICENSE)
[![Hardware: ESP32](https://img.shields.io/badge/Hardware-ESP32%20%7C%20S3%20%7C%20C3-red.svg)](hardware/)

An intelligent botanical life-support system powered by the ESP32. Measures soil moisture with a corrosion-free capacitive sensor, displays live health diagnostics on an I2C OLED screen, automatically activates a 5V submersible water pump when soil becomes dry, and hosts an interactive web dashboard for remote plant care.

---

## 📸 Key Features

- **Capacitive Soil Sensing**: Uses high-frequency capacitive sensing that prevents electrochemical oxidation and corrosion.
- **Automated Irrigation**: Triggers a 3.5-second watering burst when soil moisture drops below the configurable threshold (35%).
- **Anti-Flood Safeguard**: Enforces a 60-second cooldown between watering cycles to allow water dispersion and prevent over-saturation.
- **OLED Dashboard**: Real-time display showing moisture percentage, status indicator (THIRSTY / OPTIMAL / SATURATED), and time since last watering.
- **Responsive Web Dashboard**: Manual "Water Plant Now" trigger button, cycle counters, and live REST API.

---

## 🛠️ Hardware Requirements

| Component | Quantity | Notes |
|:---|:---:|:---|
| **ESP32 Dev Module** | 1 | ESP32-WROOM-32, ESP32-S3, or ESP32-C3 |
| **Capacitive Soil Moisture Sensor v1.2** | 1 | Analog output (corrosion resistant) |
| **5V Submersible Mini Water Pump** | 1 | Powered via external 5V rail or relay |
| **5V Relay / MOSFET Module** | 1 | Isolates pump inductive load |
| **0.96" SSD1306 I2C OLED** | 1 | 128x64 display (0x3C) |
| **Silicone Tubing** | 1 | 6mm inner diameter |

---

## 🔌 Pin Connections

| Signal | ESP32-WROOM-32 | ESP32-S3 | ESP32-C3 | Connection |
|:---|:---:|:---:|:---:|:---|
| **Soil Sensor (AOUT)** | `GPIO 34` | `GPIO 4` | `GPIO 0` | Capacitive Probe Analog Out |
| **Pump Relay Signal** | `GPIO 26` | `GPIO 10` | `GPIO 3` | Active-HIGH relay trigger |
| **I2C SDA** | `GPIO 21` | `GPIO 8` | `GPIO 4` | OLED SDA |
| **I2C SCL** | `GPIO 22` | `GPIO 9` | `GPIO 5` | OLED SCL |
| **Status LED** | `GPIO 2` | `GPIO 2` | `GPIO 8` | Onboard / External LED |

---

## 🚀 Quick Start

1. Open this directory in PlatformIO:
   ```bash
   cd projects/intermediate/14-smart-plant-watering
   ```
2. Set your WiFi credentials in `smart-plant-watering.ino`:
   ```cpp
   const char* default_ssid = "YOUR_WIFI_SSID";
   const char* default_password = "YOUR_WIFI_PASSWORD";
   ```
3. Compile and flash your board:
   ```bash
   pio run -t upload -e esp32dev
   ```
4. Access the web dashboard by navigating to the ESP32 IP address or connecting to `ESP32-SmartFlora` AP.

---

## 📺 Community & Credits

- Maintained by [Tushar Aggarwal](https://github.com/tushar2704)
- Tutorials & Video Guides: [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://youtube.com/@Makerrdesigns)
- Licensed under [AGPL-3.0](LICENSE).
