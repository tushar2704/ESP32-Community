# 🟢 Project 03: RGB LED Controller & PWM Color Engine

> Hardware LED control using the ESP32 LEDC PWM peripheral engine. Supports breathing pulses, rainbow fades, and serial hex color mixing (`#FF5500`).

---

## 📋 Overview
- **Category**: Beginner (⭐ 1/5)
- **Supported Boards**: Classic ESP32, ESP32-S3, ESP32-C3, ESP32-C6
- **External Components Required**: 1x Common Cathode RGB LED (or onboard RGB LED) + 3x 220Ω resistors
- **Primary Goals**:
  1. Learn how to use the ESP32 hardware PWM timer (LEDC) for smooth 8-bit color fading.
  2. Implement an automated rainbow hue cycle.
  3. Parse interactive Serial commands in real-time (e.g. `R255 G0 B128`).

---

## ⚡ Wiring Diagram

```text
ESP32 GPIO 25 ───[ 220Ω ]───► Red Anode
ESP32 GPIO 26 ───[ 220Ω ]───► Green Anode
ESP32 GPIO 27 ───[ 220Ω ]───► Blue Anode
ESP32 GND      ──────────────► Common Cathode
```

---

## 📺 Video Tutorial & Enclosures
- 🎬 **Watch the Tutorial**: [@IamNotIntoTech on YouTube](https://youtube.com/@IamNotIntoTech)
- 🖨️ **3D Printable Diffuser Lens**: [@Makerrdesigns on YouTube](https://youtube.com/@Makerrdesigns)

---

## 📜 License
Licensed under GNU AGPLv3. See [LICENSE](../../../LICENSE) for details.
