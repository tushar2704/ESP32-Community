# 💧 Smart Water Leak Detector & Automatic Shutoff Sentinel

[![PlatformIO CI](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml/badge.svg)](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml)
[![License: AGPL-3.0](https://img.shields.io/badge/License-AGPL--3.0-blue.svg)](LICENSE)
[![Hardware: ESP32](https://img.shields.io/badge/Hardware-ESP32%20%7C%20S3%20%7C%20C3-red.svg)](hardware/)

An autonomous smart flood prevention and water leak mitigation system powered by the ESP32. Monitors dual-probe conductivity sensors, sounds an emergency audio siren, instantly triggers a 12V/24V motorized water shutoff valve relay, and hosts a real-time web control dashboard.

---

## 📸 Key Features

- **Instant Flood Mitigation**: Energizes a relay to close the main water supply valve within milliseconds of leak detection.
- **Dual-Mode Sensing**: Uses ADC1 analog conductivity level reading alongside digital comparator threshold inputs.
- **Audible & Visual Siren**: Flashing warning LED and pulsing piezo siren with remote mute capability.
- **Glassmorphism Web UI**: Live leak detection status, analog moisture level gauge, incident event counter, and remote valve reset.
- **REST API**: Automated endpoints (`/api/status`, `/api/mute`, `/api/reset`) for Home Assistant, Node-RED, or curl scripts.

---

## 🛠️ Hardware Requirements

| Component | Quantity | Notes |
|:---|:---:|:---|
| **ESP32 Dev Module** | 1 | ESP32-WROOM-32, ESP32-S3, or ESP32-C3 |
| **Water Leak Sensor Probe** | 1–4 | PCB rain/conductive trace probe or gold-plated floor contact |
| **5V / 12V Relay Module** | 1 | Controls motorized ball valve / solenoid water valve |
| **Active Piezo Buzzer** | 1 | 3.3V or 5V audible siren |
| **Status LED** | 1 | Visual warning indicator |

---

## 🔌 Pin Connections

| Signal | ESP32-WROOM-32 | ESP32-S3 | ESP32-C3 | Connection |
|:---|:---:|:---:|:---:|:---|
| **Sensor Analog (AO)** | `GPIO 34` | `GPIO 4` | `GPIO 2` | Sensor Probe Analog Out |
| **Sensor Digital (DO)** | `GPIO 35` | `GPIO 5` | `GPIO 3` | Sensor Comparator Out |
| **Solenoid Relay** | `GPIO 26` | `GPIO 10` | `GPIO 4` | Active-HIGH relay trigger |
| **Audible Siren** | `GPIO 25` | `GPIO 11` | `GPIO 5` | Active piezo buzzer |
| **Status LED** | `GPIO 2` | `GPIO 2` | `GPIO 8` | Status LED |

---

## 🚀 Quick Start

1. Open this directory in PlatformIO:
   ```bash
   cd projects/smart-home/13-water-leak-detector
   ```
2. Set your WiFi credentials in `water-leak-detector.ino`:
   ```cpp
   const char* default_ssid = "YOUR_WIFI_SSID";
   const char* default_password = "YOUR_WIFI_PASSWORD";
   ```
3. Compile and flash your board:
   ```bash
   pio run -t upload -e esp32dev
   ```
4. Access the web dashboard by navigating to the ESP32 IP address or connecting to `ESP32-LeakSentinel` AP.

---

## 📺 Community & Credits

- Maintained by [Tushar Aggarwal](https://github.com/tushar2704)
- Tutorials & Video Guides: [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://youtube.com/@Makerrdesigns)
- Licensed under [AGPL-3.0](LICENSE).
