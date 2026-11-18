# Project 08: Debounced Button Input & Multi-Click Detector

Master digital inputs, mechanical contact debouncing, and multi-action gestures (Single Click, Double Click, and Long Press) on any ESP32 board.

---

## Overview

Mechanical switches generate tens of milliseconds of voltage spikes and bounces when opened or closed. This project demonstrates how to eliminate false triggers with a non-blocking software debounce routine, while decoding multiple distinct user inputs using just one tactile button.

| Feature | Specification |
|---|---|
| **Difficulty** | Beginner |
| **BOM Cost** | \$0 (uses onboard BOOT button) or ~\$1 (external pushbutton) |
| **Actions** | Single Click, Double Click, Long Press (>1000ms) |
| **Supported Chips** | ESP32 Classic, S2, S3, C3, C6, H2 |

---

## Hardware Setup

By default, the sketch binds to **GPIO 0** (the onboard `BOOT` button on almost every ESP32 development board). You do not need any external parts to run this demo immediately!

### Using an External Pushbutton:
Connect one terminal of your momentary button to an ESP32 GPIO (e.g. GPIO 4) and the other terminal directly to **GND**.

```
ESP32 Pin (GPIO 0 or 4) o------+
                               |
                              [ / ] Momentary Tactile Switch
                               |
ESP32 GND               o------+
```

*(The code enables `pinMode(BUTTON_PIN, INPUT_PULLUP)` using the ESP32's internal 45kΩ pull-up resistor)*

---

## Control Modes

| Gesture | Action |
|---|---|
| **Single Click** | Toggles LED between SOLID ON and OFF |
| **Double Click** | Activates Strobe / Fast Blink mode (100ms) |
| **Long Press (>1 sec)** | Activates Beacon / Slow Pulse mode (500ms) |

---

## Building and Flashing

### Using PlatformIO CLI
```bash
pio run -d projects/beginner/08-button-led -e esp32dev -t upload
pio device monitor -d projects/beginner/08-button-led
```

### Using Arduino IDE
1. Open `projects/beginner/08-button-led/button-led.ino`.
2. Select your ESP32 board under **Tools > Board**.
3. Upload and open Serial Monitor at **115200 baud**.

---

## Community & Credits

- Maintained by [ESP32 Community](https://github.com/tushar2704/ESP32-Community)
- Video guides & electronics builds: [@IamNotIntoTech](https://www.youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://www.youtube.com/@Makerrdesigns)
