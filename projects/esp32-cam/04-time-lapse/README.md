# 📷 Automated ESP32-CAM Time-Lapse Photography Studio

[![PlatformIO CI](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml/badge.svg)](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml)
[![License: AGPL-3.0](https://img.shields.io/badge/License-AGPL--3.0-blue.svg)](LICENSE)
[![Hardware: ESP32-CAM](https://img.shields.io/badge/Hardware-ESP32--CAM-red.svg)](hardware/)

An autonomous, low-power time-lapse photography recording station built on the AI-Thinker ESP32-CAM (OV2640 2MP). Automatically captures SVGA/UXGA JPEG frames at configurable intervals, writes sequentially to high-speed FAT32 MicroSD storage (`/frame_0001.jpg`), and serves an in-browser live gallery preview.

---

## 📸 Key Features

- **Automated Interval Recording**: Captures high-clarity SVGA frames at programmable intervals (default: 10s, expandable to hours or days).
- **Direct SD_MMC Storage**: Uses 1-bit high-speed SD card bus to maximize throughput while preserving GPIO 4.
- **In-Browser Gallery & Preview**: Connects to your local WiFi network (or launches `ESP32CAM-TimeLapse` fallback AP) allowing you to inspect the latest captured frame or trigger instant manual photos.
- **Battery Optimization Ready**: Easily combined with deep-sleep timer wakeup for off-grid construction, botanical growth, or 3D print recording.

---

## 🛠️ Hardware Requirements

| Component | Quantity | Notes |
|:---|:---:|:---|
| **AI-Thinker ESP32-CAM Module** | 1 | Includes OV2640 camera sensor & PSRAM |
| **MicroSD Card** | 1 | 8GB–32GB formatted as FAT32 |
| **ESP32-CAM-MB USB Base / FTDI Programmer** | 1 | 5V 2A power & flashing |

---

## 🚀 Quick Start

1. Open this directory in PlatformIO:
   ```bash
   cd projects/esp32-cam/04-time-lapse
   ```
2. Insert a FAT32 MicroSD card into the ESP32-CAM slot.
3. Set your WiFi credentials in `time-lapse.ino`:
   ```cpp
   const char* default_ssid = "YOUR_WIFI_SSID";
   const char* default_password = "YOUR_WIFI_PASSWORD";
   ```
4. Compile and flash your board:
   ```bash
   pio run -t upload -e esp32cam
   ```
5. Open your web browser to the ESP32-CAM IP to preview captured time-lapse frames in real time!

---

## 📺 Community & Credits

- Maintained by [Tushar Aggarwal](https://github.com/tushar2704)
- Tutorials & Video Guides: [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://youtube.com/@Makerrdesigns)
- Licensed under [AGPL-3.0](LICENSE).
