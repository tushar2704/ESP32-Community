# Project 02: BLE Threat & Tracker Scanner

Scan the 2.4GHz BLE spectrum in real time to locate Bluetooth devices, smart watches, beacons, and Apple AirTag / FindMy tracking devices.

---

## Overview

Bluetooth Low Energy (BLE) advertisements are constantly broadcast by smartphones, wireless earbuds, smart watches, and item trackers. This project utilizes the ESP32's dual-mode Bluetooth controller in active scanning mode to capture advertisement packets, decode manufacturer identities (such as Apple Company ID `0x004C`), and flag proximity trackers.

| Feature | Specification |
|---|---|
| **Difficulty** | Intermediate |
| **BOM Cost** | \$4 (ESP32 Dev Board) |
| **Scan Mode** | Active Scanning (100ms interval, 99ms window) |
| **Detection Capabilities** | Apple AirTags / FindMy, iBeacons, Microsoft devices, general BLE peripherals |
| **Supported Chips** | ESP32 Classic, ESP32-S3, ESP32-C3, ESP32-C6 |

---

## AirTag & FindMy Identification

Apple devices broadcast manufacturer specific data under Bluetooth SIG Assigned Company Identifier `0x004C`.
- Byte 0 & 1: `0x4C, 0x00` (Company ID in Little-Endian)
- Byte 2: Type indicator. A value of `0x12` specifies Apple Offline Finding (AirTags, AirPods in case, and third-party FindMy accessories).

When an advertisement matching this signature is received, the scanner highlights it with an `[!AIRTAG!]` tag and logs the signal strength (`RSSI`). Signal strengths closer to 0 dBm (e.g. `-45 dBm`) indicate the tracker is within a few feet.

---

## Building and Flashing

### Using PlatformIO CLI
```bash
pio run -d projects/security/02-ble-scanner -e esp32dev -t upload
pio device monitor -d projects/security/02-ble-scanner
```

### Using Arduino IDE
1. Open `projects/security/02-ble-scanner/ble-scanner.ino`.
2. Select **ESP32 Dev Module** under **Tools > Board**.
3. Upload and open Serial Monitor at **115200 baud**.

---

## Community & Credits

- Maintained by [ESP32 Community](https://github.com/tushar2704/ESP32-Community)
- Video guides & electronics builds: [@IamNotIntoTech](https://www.youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://www.youtube.com/@Makerrdesigns)
