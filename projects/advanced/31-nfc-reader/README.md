# 🛡️ RFID / NFC Smart Access Control System

[![PlatformIO CI](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml/badge.svg)](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml)
[![License: AGPL-3.0](https://img.shields.io/badge/License-AGPL--3.0-blue.svg)](LICENSE)
[![Hardware: ESP32](https://img.shields.io/badge/Hardware-ESP32%20%7C%20S3%20%7C%20C3-red.svg)](hardware/)

An enterprise-ready physical access control system built on the ESP32 and MFRC522 SPI RFID/NFC reader. Reads 13.56 MHz MIFARE Classic 1K and NTAG smartcards, persists authorized card credentials in non-volatile flash memory (NVS Preferences), triggers 12V magnetic door lock strikes, sounds audible feedback, and provides an embedded web administration dashboard.

---

## 📸 Key Features

- **13.56 MHz RFID/NFC Detection**: Compatible with ISO14443A cards, fobs, transit passes, and NFC smartphone tags.
- **NVS Persistent Storage**: Authorized cards remain stored across power outages and reboots inside ESP32 non-volatile memory.
- **Audible & Visual Cues**: 2 sharp confirmation beeps + status LED on access grant; long error tone on unauthorized swipe.
- **Electric Door Strike Control**: 3-second relay pulse securely operates 12V NC/NO electric door strikes and electromagnetic mag-locks.
- **Web Administration Panel**: Remote door unlock button, live access event counter, and last scanned card UID readout.

---

## 🛠️ Hardware Requirements

| Component | Quantity | Notes |
|:---|:---:|:---|
| **ESP32 Dev Module** | 1 | ESP32-WROOM-32, ESP32-S3, or ESP32-C3 |
| **MFRC522 RFID Module** | 1 | 3.3V SPI module with RFID card & key fob |
| **5V / 12V Relay Module** | 1 | Connects to 12V DC electric strike or maglock |
| **Piezo Buzzer** | 1 | Audio feedback for grant/deny |
| **Status LED** | 1 | Visual indicator |

---

## 🔌 Pin Connections

| Signal | ESP32-WROOM-32 | ESP32-S3 | ESP32-C3 | RC522 Module Pin |
|:---|:---:|:---:|:---:|:---|
| **SDA / SS** | `GPIO 5` | `GPIO 10` | `GPIO 7` | **SDA / NSS** |
| **SCK** | `GPIO 18` | `GPIO 12` | `GPIO 4` | **SCK** |
| **MOSI** | `GPIO 23` | `GPIO 11` | `GPIO 6` | **MOSI** |
| **MISO** | `GPIO 19` | `GPIO 13` | `GPIO 5` | **MISO** |
| **RST** | `GPIO 22` | `GPIO 9` | `GPIO 3` | **RST** |
| **Door Relay** | `GPIO 26` | `GPIO 4` | `GPIO 2` | Active-HIGH Relay Signal |
| **Piezo Buzzer** | `GPIO 25` | `GPIO 5` | `GPIO 1` | Active Buzzer |
| **3.3V & GND** | `3.3V` / `GND` | `3.3V` / `GND` | `3.3V` / `GND` | Power (Do NOT use 5V on RC522) |

---

## 🚀 Quick Start

1. Open this directory in PlatformIO:
   ```bash
   cd projects/advanced/31-nfc-reader
   ```
2. Set your WiFi credentials in `nfc-reader.ino`:
   ```cpp
   const char* default_ssid = "YOUR_WIFI_SSID";
   const char* default_password = "YOUR_WIFI_PASSWORD";
   ```
3. Compile and flash your board:
   ```bash
   pio run -t upload -e esp32dev
   ```
4. Swipe your RFID tag against the RC522 antenna. View the Serial Monitor or Web Dashboard to register the scanned UID as an authorized card.

---

## 📺 Community & Credits

- Maintained by [Tushar Aggarwal](https://github.com/tushar2704)
- Tutorials & Video Guides: [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://youtube.com/@Makerrdesigns)
- Licensed under [AGPL-3.0](LICENSE).
