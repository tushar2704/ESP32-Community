# 🏠 ESPHome & Home Assistant Native Smart Node

[![PlatformIO CI](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml/badge.svg)](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml)
[![License: AGPL-3.0](https://img.shields.io/badge/License-AGPL--3.0-blue.svg)](LICENSE)
[![Hardware: ESP32](https://img.shields.io/badge/Hardware-ESP32%20%7C%20S3%20%7C%20C3-red.svg)](hardware/)

The complete guide and dual-implementation template for integrating the ESP32 into **Home Assistant**. Demonstrates both the declarative **ESPHome YAML** path (Native API with zero-lag RPC & OTA) and the pure **Arduino C++ MQTT Auto-Discovery** path for seamless plug-and-play home automation.

---

## 📸 Two Implementation Paths

### Path A: Declarative ESPHome YAML (`esphome-basics.yaml`)
- Native encrypted Home Assistant API (`noise-psk`).
- Automatic entity discovery directly in the Home Assistant Integrations dashboard.
- Built-in captive portal fallback and wireless Over-The-Air (OTA) updates.

### Path B: Arduino C++ MQTT Auto-Discovery (`esphome-basics.ino`)
- Pure Arduino framework using `PubSubClient` and `ArduinoJson`.
- Automatically broadcasts discovery payloads on `homeassistant/switch/<device_id>/config`.
- Home Assistant creates interactive switch cards and state listeners automatically with zero YAML editing.

---

## 🛠️ Hardware Requirements

| Component | Quantity | Notes |
|:---|:---:|:---|
| **ESP32 Dev Module** | 1 | ESP32-WROOM-32, ESP32-S3, or ESP32-C3 |
| **5V Relay Module** | 1 | Controls lights, appliances, or solenoids |
| **Status LED** | 1 | Built-in or external indicator |

---

## 🔌 Pin Connections

| Signal | ESP32-WROOM-32 | ESP32-S3 | ESP32-C3 | Connection |
|:---|:---:|:---:|:---:|:---|
| **Relay Signal (IN)** | `GPIO 26` | `GPIO 10` | `GPIO 4` | Active-HIGH relay trigger |
| **Status LED** | `GPIO 2` | `GPIO 2` | `GPIO 8` | Onboard / External LED |

---

## 🚀 Quick Start

### Running via ESPHome Dashboard
```bash
pip install esphome
esphome run esphome-basics.yaml
```

### Running via PlatformIO
1. Open this directory:
   ```bash
   cd projects/smart-home/01-esphome-basics
   ```
2. Configure your WiFi and Home Assistant MQTT broker IP in `esphome-basics.ino`.
3. Compile and flash:
   ```bash
   pio run -t upload -e esp32dev
   ```

---

## 📺 Community & Credits

- Maintained by [Tushar Aggarwal](https://github.com/tushar2704)
- Tutorials & Video Guides: [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://youtube.com/@Makerrdesigns)
- Licensed under [AGPL-3.0](LICENSE).
