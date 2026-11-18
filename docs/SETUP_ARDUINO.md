# 🎨 Arduino IDE Setup for ESP32

> **The easiest way to get started with ESP32 development — perfect for beginners and rapid prototyping.**

[![Arduino](https://img.shields.io/badge/Arduino-IDE%202.x-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/en/software)
[![ESP32](https://img.shields.io/badge/ESP32-Supported-E7352C?style=for-the-badge&logo=espressif&logoColor=white)](https://www.espressif.com/)

---

## 📑 Table of Contents

- [Prerequisites](#-prerequisites)
- [Step 1 — Install Arduino IDE 2.x](#-step-1--install-arduino-ide-2x)
- [Step 2 — Add ESP32 Board Manager URL](#-step-2--add-esp32-board-manager-url)
- [Step 3 — Install ESP32 Board Package](#-step-3--install-esp32-board-package)
- [Step 4 — Select Your Board](#-step-4--select-your-board)
- [Step 5 — First Upload (Blink!)](#-step-5--first-upload-blink)
- [Common Settings & Configuration](#%EF%B8%8F-common-settings--configuration)
- [Board Selection Reference](#-board-selection-reference)
- [Troubleshooting](#-troubleshooting)
- [Additional Resources](#-additional-resources)

---

## 📋 Prerequisites

| Requirement | Details |
|---|---|
| **Operating System** | Windows 10+, macOS 10.15+, or Linux (64-bit) |
| **USB Driver** | CP2102 or CH340 (depending on your board) |
| **ESP32 Board** | Any ESP32 variant (Classic, S2, S3, C3, C6, H2) |
| **USB Cable** | USB-A to Micro-USB or USB-C (data cable, not charge-only) |

> [!WARNING]
> Make sure you have a **data-capable** USB cable. Many cables are charge-only and will not allow communication with the board. If your board doesn't appear in the port list, try a different cable first.

---

## 📥 Step 1 — Install Arduino IDE 2.x

1. Visit the official download page: **[https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)**
2. Download **Arduino IDE 2.x** for your operating system
3. Run the installer and follow the on-screen instructions

| OS | Installer | Notes |
|---|---|---|
| **Windows** | `.exe` installer | Run as administrator if prompted |
| **macOS** | `.dmg` file | Drag to Applications folder |
| **Linux** | `.AppImage` or `.zip` | Make executable: `chmod +x arduino-ide_*.AppImage` |

> [!TIP]
> Arduino IDE 2.x has significant improvements over 1.x, including an auto-complete code editor, integrated serial plotter, and a built-in debugger. Always use 2.x for ESP32 development.

---

## 🔗 Step 2 — Add ESP32 Board Manager URL

The ESP32 board package is not included by default. You need to add Espressif's board manager URL.

1. Open Arduino IDE
2. Go to **File → Preferences** (or **Arduino IDE → Settings** on macOS)
3. Find **"Additional boards manager URLs"**
4. Paste the following URL:

```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

5. If you already have other URLs, click the icon to the right and add it on a **new line**
6. Click **OK**

> [!NOTE]
> This URL provides board definitions for **all ESP32 variants** — ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6, and ESP32-H2. You only need this single URL.

---

## 📦 Step 3 — Install ESP32 Board Package

1. Open **Tools → Board → Boards Manager** (or click the board icon in the left sidebar)
2. Search for **`esp32`**
3. Find **"esp32 by Espressif Systems"**
4. Click **Install** (choose the latest stable version)
5. Wait for the installation to complete (this may take several minutes)

```
📂 What gets installed:
├── ESP32 toolchain (xtensa-esp32-elf, riscv32-esp-elf)
├── ESP32 Arduino core libraries
├── Board definitions for all ESP32 variants
├── esptool.py (upload utility)
└── Partition tables & bootloader binaries
```

> [!IMPORTANT]
> The package is approximately **1.5–2 GB** in size. Ensure you have sufficient disk space and a stable internet connection during installation.

---

## 🎯 Step 4 — Select Your Board

1. Connect your ESP32 board via USB
2. Go to **Tools → Board → esp32**
3. Select your specific board from the list
4. Go to **Tools → Port** and select the COM/serial port for your board

### Identifying Your Port

| OS | Port Format | Example |
|---|---|---|
| **Windows** | `COMx` | `COM3`, `COM4` |
| **macOS** | `/dev/cu.usbserial-*` or `/dev/cu.wchusbserial-*` | `/dev/cu.usbserial-0001` |
| **Linux** | `/dev/ttyUSBx` or `/dev/ttyACMx` | `/dev/ttyUSB0` |

> [!TIP]
> If you don't see your port, install the appropriate USB driver:
> - **CP2102** (Silicon Labs): [https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
> - **CH340/CH9102** (WCH): [https://www.wch-ic.com/downloads/CH341SER_ZIP.html](https://www.wch-ic.com/downloads/CH341SER_ZIP.html)

---

## 🚀 Step 5 — First Upload (Blink!)

Create your first sketch to verify everything works:

```cpp
// ESP32 Blink — Verify your setup is working!
// Built-in LED pin varies by board (usually GPIO 2)

#define LED_PIN 2

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("ESP32 Blink Test — Setup Complete!");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED ON");
  delay(1000);

  digitalWrite(LED_PIN, LOW);
  Serial.println("LED OFF");
  delay(1000);
}
```

### Upload Steps

1. Click the **✔ Verify** button (compiles the sketch)
2. Click the **→ Upload** button
3. **Watch the output panel** — when you see `Connecting...`, some boards require you to hold the **BOOT** button until upload starts
4. Wait for `Done uploading`
5. Open **Tools → Serial Monitor** (set baud to `115200`)

> [!WARNING]
> **ESP32-CAM and some older boards** require you to hold the **BOOT/GPIO0** button during upload. Release it once you see `Writing at 0x00010000...` in the console. If upload fails with `A fatal error occurred: Failed to connect`, try holding BOOT and pressing RESET, then release BOOT.

---

## ⚙️ Common Settings & Configuration

Navigate to **Tools** menu to configure these important settings:

### Partition Scheme

| Scheme | App Space | SPIFFS/LittleFS | OTA | Best For |
|---|---|---|---|---|
| **Default 4MB with spiffs** | 1.2 MB | 1.5 MB | ✅ | General use |
| **Huge APP (3MB No OTA)** | 3 MB | 1 MB | ❌ | Large sketches |
| **Minimal SPIFFS** | 1.9 MB | 190 KB | ✅ | Code-heavy projects |
| **No OTA (2MB APP)** | 2 MB | 2 MB | ❌ | Max storage |
| **16MB Flash (16MB)** | 4.5 MB | 6.9 MB | ✅ | 16MB flash boards |

### Upload Speed

| Speed | Reliability | Use Case |
|---|---|---|
| **921600** | ⚠️ May fail on long cables | Fast uploads, short USB cable |
| **460800** | ✅ Recommended | Default — good balance |
| **230400** | ✅ Very stable | If 460800 fails |
| **115200** | ✅ Most reliable | Troubleshooting upload issues |

### Flash Mode

| Mode | Speed | Compatibility |
|---|---|---|
| **QIO** | Fastest | Most boards (default) |
| **DIO** | Fast | Some WROVER modules |
| **QOUT** | Medium | Older modules |
| **DOUT** | Slowest | Maximum compatibility |

### Other Important Settings

| Setting | Recommended Value | Notes |
|---|---|---|
| **Flash Frequency** | 80 MHz | 40 MHz for stability issues |
| **Core Debug Level** | None (production) / Verbose (debugging) | Controls `ESP_LOGx` output |
| **PSRAM** | Enabled (if your board has it) | Required for ESP32-CAM, WROVER |
| **USB CDC On Boot** | Enabled (S2/S3/C3/C6 with native USB) | For serial output via USB |
| **Arduino Runs On** | Core 1 | Default; Core 0 for WiFi-heavy apps |

---

## 📊 Board Selection Reference

Quick reference for the most common boards in the Arduino IDE board selector:

| Physical Board | Select in Arduino IDE |
|---|---|
| ESP32 DevKitC v4 | `ESP32 Dev Module` |
| ESP32-WROOM-32 generic | `ESP32 Dev Module` |
| ESP32-WROVER | `ESP32 Wrover Module` |
| NodeMCU-32S | `NodeMCU-32S` |
| ESP32-CAM (AI-Thinker) | `AI Thinker ESP32-CAM` |
| ESP32-S2 DevKitM | `ESP32S2 Dev Module` |
| ESP32-S3 DevKitC | `ESP32S3 Dev Module` |
| XIAO ESP32-S3 | `XIAO_ESP32S3` |
| ESP32-C3 DevKitM | `ESP32C3 Dev Module` |
| ESP32-C3 SuperMini | `ESP32C3 Dev Module` |
| ESP32-C6 DevKitC | `ESP32C6 Dev Module` |
| ESP32-H2 DevKitM | `ESP32H2 Dev Module` |
| Cheap Yellow Display (CYD) | `ESP32 Dev Module` |
| LilyGo T-Display-S3 | `LilyGo T-Display-S3` |

---

## 🔧 Troubleshooting

| Problem | Solution |
|---|---|
| **Board not found in port list** | Install CP2102/CH340 USB driver; try a different cable |
| **Upload fails: "Failed to connect"** | Hold BOOT button during upload; check cable |
| **Sketch too large** | Change partition scheme to "Huge APP (3MB No OTA)" |
| **Brownout detector triggered** | Use a better USB cable or powered USB hub |
| **WiFi won't connect** | Check Core Debug Level = Verbose for diagnostics |
| **Serial Monitor shows garbage** | Match baud rate to `Serial.begin()` value |
| **Compilation error after update** | Clear cache: **File → Preferences → show sketch folder**, delete `build` |
| **PSRAM not detected** | Enable PSRAM in Tools menu; check board has PSRAM |

> [!TIP]
> You can enable **verbose output** during upload in **File → Preferences** by checking "Show verbose output during: upload". This provides detailed logs that help diagnose upload issues.

---

## 📚 Additional Resources

| Resource | Link |
|---|---|
| ESP32 Arduino Core Docs | [https://docs.espressif.com/projects/arduino-esp32/](https://docs.espressif.com/projects/arduino-esp32/) |
| Arduino Reference | [https://www.arduino.cc/reference/en/](https://www.arduino.cc/reference/en/) |
| ESP32 Pinout Reference | [../boards/esp32-classic/README.md](../boards/esp32-classic/README.md) |
| PlatformIO Setup (Alternative) | [SETUP_PLATFORMIO.md](SETUP_PLATFORMIO.md) |
| ESP-IDF Setup (Advanced) | [SETUP_ESPIDF.md](SETUP_ESPIDF.md) |
| MicroPython Setup | [SETUP_MICROPYTHON.md](SETUP_MICROPYTHON.md) |

---

<p align="center">Made with ❤️ by <a href="https://github.com/tushar2704">Tushar Aggarwal</a> | 📺 <a href="https://youtube.com/@IamNotIntoTech">@IamNotIntoTech</a> + <a href="https://youtube.com/@Makerrdesigns">@Makerrdesigns</a></p>
