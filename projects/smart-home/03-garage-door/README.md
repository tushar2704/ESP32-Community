# 🚪 Smart Garage Door Opener & Monitor

[![PlatformIO CI](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml/badge.svg)](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml)
[![License: AGPL-3.0](https://img.shields.io/badge/License-AGPL--3.0-blue.svg)](LICENSE)
[![Hardware: ESP32](https://img.shields.io/badge/Hardware-ESP32%20%7C%20S3%20%7C%20C3-red.svg)](hardware/)

An enterprise-grade smart garage door monitor and opener powered by the ESP32. Featuring a responsive glassmorphism web dashboard, real-time magnetic reed door sensor state tracking, isolated relay pulse triggering, safety auto-close timer with audible buzzer alerts, and REST API integration for home automation.

---

## 📸 Overview & Features

- **Responsive Web Dashboard**: Animated SVG/CSS roll-up garage door graphic showing real-time Open/Closed status.
- **Relay Pulse Triggering**: Simulates momentary garage pushbutton wall switch (600ms pulse) without affecting manual remotes.
- **Magnetic Reed Sensor**: Detects exact door physical position with hardware debounce and state duration tracking.
- **Audible Alerts**: Piezo buzzer warning chirps on state change and 5-beep safety warning before auto-close triggers.
- **Auto-Close Safety**: Configurable automated timer closes the door if accidentally left open for more than 15 minutes.
- **WiFi Fallback AP**: Automatically launches `ESP32-GarageDoor` access point if primary WiFi is unavailable.
- **REST API**: Full JSON API for Home Assistant, Node-RED, or curl automation.

---

## 🛠️ Hardware Requirements

| Component | Quantity | Notes |
|:---|:---:|:---|
| **ESP32 Dev Module** | 1 | ESP32-WROOM-32, ESP32-S3, or ESP32-C3 |
| **5V / 3.3V Relay Module** | 1 | Optocoupled relay connected across garage opener push button |
| **Magnetic Reed Switch** | 1 | Mounted on garage door frame (Normally Closed or Normally Open) |
| **Piezo Buzzer** | 1 | 5V or 3.3V active buzzer for audio feedback |
| **Status LED** | 1 | Built-in or external 330Ω LED |
| **Jumper Wires & Power Supply** | 1 | 5V 2A microUSB / Type-C power adapter |

---

## 🔌 Pin Connections

| Signal | ESP32-WROOM-32 | ESP32-S3 | ESP32-C3 | Description |
|:---|:---:|:---:|:---:|:---|
| **Relay Signal (IN)** | `GPIO 26` | `GPIO 10` | `GPIO 4` | Active-HIGH relay trigger pulse |
| **Reed Switch** | `GPIO 27` | `GPIO 11` | `GPIO 5` | Input with internal pull-up (to GND) |
| **Piezo Buzzer** | `GPIO 25` | `GPIO 12` | `GPIO 6` | Audible status and alarm beeper |
| **Status LED** | `GPIO 2` | `GPIO 2` | `GPIO 8` | On-board / external status indicator |

---

## 🚀 Quick Start

1. Open this directory in PlatformIO:
   ```bash
   cd projects/smart-home/03-garage-door
   ```
2. Configure your WiFi credentials in `garage-door.ino`:
   ```cpp
   const char* default_ssid = "YOUR_WIFI_SSID";
   const char* default_password = "YOUR_WIFI_PASSWORD";
   ```
3. Compile and flash your board:
   ```bash
   pio run -t upload -e esp32dev
   ```
4. Open your browser and navigate to the IP address printed in the Serial Monitor (or `http://192.168.4.1` on AP mode).

---

## 🌐 REST API Reference

### 1. Check Status
- **Method**: `GET /api/status`
- **Response**:
```json
{
  "doorOpen": false,
  "doorState": "CLOSED",
  "elapsedSinceChange": 142000,
  "autoCloseEnabled": false,
  "autoCloseRemainingMs": 0,
  "wifiRSSI": -56,
  "uptime": 320040
}
```

### 2. Trigger Garage Door
- **Method**: `POST /api/trigger`
- **Response**:
```json
{
  "success": true,
  "message": "Relay pulse triggered for garage button."
}
```

---

## 📺 Community & Credits

- Maintained by [Tushar Aggarwal](https://github.com/tushar2704)
- Tutorials & Video Guides: [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://youtube.com/@Makerrdesigns)
- Licensed under [AGPL-3.0](LICENSE).
