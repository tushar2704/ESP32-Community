# 📷 Project CAM-01: ESP32-CAM Security Surveillance Camera & Video Streamer

> High-performance MJPEG live streaming video server with motion detection snapshot triggers, flashlight toggle, resolution selector (UXGA to QVGA), and RTSP/HTTP endpoints.

---

## 📋 Overview
- **Category**: ESP32-CAM (⭐⭐ 2/5)
- **Supported Boards**: AI-Thinker ESP32-CAM, ESP32-CAM-MB, Freenove ESP32-WROVER CAM, Xiao ESP32-S3 Sense
- **Camera Sensor**: Omnivision OV2640 (2 Megapixel) or OV5640 (5 Megapixel)
- **Primary Goals**:
  1. Stream fluid 20–30 FPS video over local Wi-Fi.
  2. Provide web-based controls for resolution, quality, brightness, and contrast.
  3. Include an on-demand flashlight toggle (GPIO 4 high-power LED).

---

## ⚡ Hardware Connections

For AI-Thinker ESP32-CAM without onboard USB:
```text
ESP32-CAM 5V   ────────► FTDI USB-to-TTL 5V (Must provide >= 2A power!)
ESP32-CAM GND  ────────► FTDI GND
ESP32-CAM U0R  ────────► FTDI TX
ESP32-CAM U0T  ────────► FTDI RX
ESP32-CAM IO0  ────────► GND (Connect ONLY when flashing, disconnect to run!)
```

---

## 🚀 Installation & Flashing
1. In Arduino IDE:
   - Board: **AI Thinker ESP32-CAM**
   - Partition Scheme: **Huge APP (3MB No OTA / 1MB SPIFFS)**
   - PSRAM: **Enabled**
2. If using PlatformIO, use the included `platformio.ini` environment `[env:esp32cam]`.
3. Open Serial monitor at **115200 baud** to see the stream URL (e.g. `http://192.168.1.120`).

---

## 📺 Video Tutorial & Enclosures
- 🎬 **Watch the Tutorial**: [@IamNotIntoTech on YouTube](https://youtube.com/@IamNotIntoTech)
- 🖨️ **3D Printable Security Wall Mount**: [@Makerrdesigns on YouTube](https://youtube.com/@Makerrdesigns)

---

## 📜 License
Licensed under GNU AGPLv3. See [LICENSE](../../../LICENSE) for details.
