# 🟢 Project 01: Hello ESP32 — The Universal Hardware Diagnostic

> Complete test suite and diagnostic baseline for any ESP32 board: chip verification, CPU speed, flash size, PSRAM detection, MAC address, internal temperature, and live 2.4 GHz Wi-Fi spectrum scan.

---

## 📋 Overview
- **Category**: Beginner (⭐ 1/5)
- **Supported Boards**: Classic ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6, ESP32-H2
- **External Components Required**: **None!** (Runs 100% on internal peripherals and onboard LED)
- **Primary Goals**:
  1. Verify the board is functioning properly and not bootlooping.
  2. Measure CPU clock frequency and active core count.
  3. Detect external PSRAM and flash memory sizing.
  4. Blink onboard status LED.
  5. Scan surrounding 2.4 GHz Wi-Fi channels and print RSSI signal bars.

---

## 🔧 Requirements

### Hardware
- Any ESP32 development board (e.g. ESP32-WROOM-32 DevKitC, ESP32-S3-DevKitC-1, etc.)
- Micro-USB or USB-C data cable (must be a data cable, not charge-only!)

### Software & Libraries
- Arduino IDE 2.x or PlatformIO
- Standard `WiFi.h` and `esp_system.h` (Built into the official ESP32 Arduino core)

---

## 📺 Video Tutorial & 3D Builds
- 🎬 **Watch the Coding Walkthrough**: [@IamNotIntoTech on YouTube](https://youtube.com/@IamNotIntoTech)
- 🖨️ **3D Printable Desktop Enclosure**: [@Makerrdesigns on YouTube](https://youtube.com/@Makerrdesigns)

---

## 🚀 Installation & Flashing

### Option A: PlatformIO (Recommended)
1. Open this project directory in VS Code with the PlatformIO extension.
2. Select your environment in `platformio.ini` (default: `esp32dev`).
3. Click **Build (✓)** and then **Upload (→)**.
4. Open the Serial Monitor at **115200 baud**.

### Option B: Arduino IDE
1. Open `hello-esp32.ino` in Arduino IDE.
2. Select **Tools > Board > ESP32 Arduino > ESP32 Dev Module** (or your specific board).
3. Select your serial port under **Tools > Port**.
4. Set Upload Speed to `921600` and Flash Frequency to `80MHz`.
5. Click **Upload**.

---

## 📄 Expected Serial Output

```text
==================================================
  ⚡ ESP32 Community — Project 01: Hello ESP32
  https://github.com/tushar2704/ESP32-Community
==================================================
🔍 Hardware Diagnostics:
  - Chip Model:       ESP32-D0WD-V3 (Rev 3)
  - CPU Cores:        2 core(s) @ 240 MHz
  - Flash Memory:     4 MB (DIO, 40MHz)
  - Embedded PSRAM:   Not Present ❌
  - Free Heap:        298,420 bytes
  - MAC Address:      7C:DF:A1:03:42:10
--------------------------------------------------
📡 Scanning 2.4 GHz Wi-Fi Spectrum...
  [ 1] MyHomeFiber            | RSSI: -52 dBm [||||] | Ch:  1 | Secured 🔒
  [ 2] Studio_Guest           | RSSI: -68 dBm [|||.] | Ch:  6 | Secured 🔒
  [ 3] IoT_Devices            | RSSI: -74 dBm [||..] | Ch: 11 | Secured 🔒
--------------------------------------------------
💡 Diagnostic heartbeat running... LED toggling every 1s.
```

---

## 📜 License
Licensed under GNU AGPLv3. See [LICENSE](../../../LICENSE) for details.
