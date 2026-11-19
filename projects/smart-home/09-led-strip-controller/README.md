# 🌈 Addressable NeoPixel / WS2812B LED Strip Web Controller

[![PlatformIO CI](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml/badge.svg)](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml)
[![License: AGPL-3.0](https://img.shields.io/badge/License-AGPL--3.0-blue.svg)](LICENSE)
[![Hardware: ESP32](https://img.shields.io/badge/Hardware-ESP32%20%7C%20S3%20%7C%20C3-red.svg)](hardware/)

A complete web-controlled addressable RGB LED strip driver for WS2812B, WS2811, and SK6812 NeoPixels powered by the ESP32. Featuring a sleek modern web control dashboard, interactive color picker, brightness and animation speed sliders, 8 animated lighting modes, and REST API support.

---

## 📸 Features

- **Responsive Web UI**: Control your lights from any smartphone, tablet, or PC without needing an app.
- **8 Dynamic Animation Modes**:
  1. Solid Color
  2. Rainbow Cycle
  3. Color Wipe
  4. Theater Chase
  5. Breathing / Pulse
  6. Fire Flicker
  7. Meteor Rain
  8. Strobe
- **Granular Controls**: Power ON/OFF, full 24-bit 16.7-million color picker, 0-255 brightness dimming, and millisecond animation speed slider.
- **REST API**: Easily integrate into Home Assistant, Stream Deck, Apple Shortcuts, or curl scripts.
- **Automatic Fallback AP**: Automatically launches `ESP32-NeoPixel` WiFi hotspot if home router is unavailable.

---

## 🛠️ Hardware Requirements

| Component | Quantity | Notes |
|:---|:---:|:---|
| **ESP32 Dev Module** | 1 | ESP32-WROOM-32, ESP32-S3, or ESP32-C3 |
| **WS2812B / SK6812 LED Strip** | 1 | 30 or 60 LEDs/meter (5V) |
| **Logic Level Shifter or 330Ω Resistor**| 1 | Recommended on data line for 3.3V to 5V transition |
| **1000µF 16V Capacitor** | 1 | Placed across +5V and GND power rails for surge protection |
| **5V 2A–5A Power Supply** | 1 | Sized to 60mA per LED at full white |

---

## 🔌 Pin Connections

| Signal | ESP32-WROOM-32 | ESP32-S3 | ESP32-C3 | LED Strip Pin |
|:---|:---:|:---:|:---:|:---|
| **DIN (Data In)** | `GPIO 18` | `GPIO 18` | `GPIO 4` | **DI / DATA** |
| **GND** | `GND` | `GND` | `GND` | **GND** (Common) |
| **+5V** | External 5V PSU | External 5V PSU | External 5V PSU | **+5V / VCC** |

> [!WARNING]
> Do NOT power long LED strips directly from the ESP32 board's 3.3V or 5V VIN pin. Connect the LED strip directly to a dedicated 5V power supply and tie all grounds together.

---

## 🚀 Quick Start

1. Open this directory in PlatformIO:
   ```bash
   cd projects/smart-home/09-led-strip-controller
   ```
2. Update WiFi credentials in `led-strip-controller.ino`:
   ```cpp
   const char* default_ssid = "YOUR_WIFI_SSID";
   const char* default_password = "YOUR_WIFI_PASSWORD";
   ```
3. Compile and upload:
   ```bash
   pio run -t upload -e esp32dev
   ```
4. Access the web dashboard by navigating to the ESP32 IP address in your browser.

---

## 🌐 REST API Reference

### 1. Retrieve Current State
- **Method**: `GET /api/status`
- **Response**:
```json
{
  "power": true,
  "r": 255,
  "g": 100,
  "b": 50,
  "brightness": 180,
  "effect": 1,
  "speed": 30,
  "numLeds": 30
}
```

### 2. Update LED Settings
- **Method**: `POST /api/led`
- **Body**:
```json
{
  "power": true,
  "r": 0,
  "g": 150,
  "b": 255,
  "brightness": 220,
  "effect": 1,
  "speed": 20
}
```

---

## 📺 Community & Credits

- Maintained by [Tushar Aggarwal](https://github.com/tushar2704)
- Tutorials & Video Guides: [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://youtube.com/@Makerrdesigns)
- Licensed under [AGPL-3.0](LICENSE).
