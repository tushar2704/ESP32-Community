# ⌨️ ESP32-S3 Native USB HID Macro Pad & Media Volume Controller

[![PlatformIO CI](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml/badge.svg)](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml)
[![License: AGPL-3.0](https://img.shields.io/badge/License-AGPL--3.0-blue.svg)](LICENSE)
[![Hardware: ESP32-S3](https://img.shields.io/badge/Hardware-ESP32--S3-red.svg)](hardware/)

A hardware USB Human Interface Device (HID) built on the ESP32-S3 using its native high-speed USB Full Speed OTG peripheral. Features a smooth rotary encoder volume knob with push-to-mute, dedicated media keys (Play/Pause, Next Track), and programmable multi-key developer shortcut macros (VSCode Palette, Discord Mute).

---

## 📸 Key Features

- **True Driverless USB HID**: Recognized immediately as a native USB Composite Keyboard + Consumer Control device on Windows, macOS, and Linux without any drivers.
- **Precision Rotary Volume Knob**: Quadrature rotary encoder provides smooth volume stepping (Up/Down) with momentary push-to-mute.
- **Mechanical Macro Keys**: 4 programmable mechanical hotkey buttons with hardware interrupt debouncing:
  - **Key 1**: Media Play / Pause (`CONSUMER_CONTROL_PLAY_PAUSE`)
  - **Key 2**: Next Track (`CONSUMER_CONTROL_SCAN_NEXT`)
  - **Key 3**: IDE Power Macro: VSCode Command Palette (`Ctrl+Shift+P` / `Cmd+Shift+P`)
  - **Key 4**: Discord / Slack Microphone Mute (`Ctrl+Shift+M`)
- **Visual Feedback**: Real-time status LED pulses on every keypress and knob rotation.

---

## 🛠️ Hardware Requirements

| Component | Quantity | Notes |
|:---|:---:|:---|
| **ESP32-S3 Dev Board** | 1 | Must have native USB port (USB D+ on GPIO 20, USB D- on GPIO 19) |
| **Rotary Encoder Module** | 1 | EC11 or standard rotary encoder with integrated push switch |
| **Mechanical Switches** | 4 | Cherry MX, Gateron, or tactile buttons |
| **Custom Enclosure** | 1 | 3D printable desktop desk-knob enclosure |

---

## 🔌 Pin Connections

| Signal | ESP32-S3 Pin | Component Connection |
|:---|:---:|:---|
| **Encoder CLK** | `GPIO 4` | Rotary Encoder Pin A |
| **Encoder DT** | `GPIO 5` | Rotary Encoder Pin B |
| **Encoder SW** | `GPIO 6` | Rotary Encoder Push Button (Mute) |
| **Macro Key 1** | `GPIO 7` | Cherry Switch 1 (Play / Pause) |
| **Macro Key 2** | `GPIO 8` | Cherry Switch 2 (Next Track) |
| **Macro Key 3** | `GPIO 9` | Cherry Switch 3 (Dev Palette) |
| **Macro Key 4** | `GPIO 10` | Cherry Switch 4 (Discord Mute) |
| **Activity LED** | `GPIO 2` | On-board / External LED |
| **GND** | `GND` | Common Ground for all switches |

---

## 🚀 Quick Start

1. Open this directory in PlatformIO:
   ```bash
   cd projects/advanced/08-usb-hid
   ```
2. Build and upload firmware using the ESP32-S3 native USB or UART port:
   ```bash
   pio run -t upload -e esp32-s3-devkitc-1
   ```
3. Connect the ESP32-S3 via its **USB** (OTG) port to your computer.
4. Turn the encoder to adjust system volume, press down to mute, and click keys to trigger macros!

---

## 📺 Community & Credits

- Maintained by [Tushar Aggarwal](https://github.com/tushar2704)
- Tutorials & Video Guides: [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://youtube.com/@Makerrdesigns)
- Licensed under [AGPL-3.0](LICENSE).
