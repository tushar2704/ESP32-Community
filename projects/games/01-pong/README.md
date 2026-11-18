# Project 01: Retro Pong for SSD1306 OLED

Relive classic 1970s arcade gaming with a 60 FPS implementation of Pong running on an ESP32 and a 0.96" SSD1306 OLED display.

---

## Overview

This project renders 2D arcade physics, dynamic paddle angle deflection, an automated AI opponent, and score tracking on an I2C OLED display (128x64 resolution).

| Feature | Specification |
|---|---|
| **Difficulty** | Intermediate |
| **BOM Cost** | ~\$6–\$8 (ESP32 Dev Board + 0.96" I2C OLED + 2 Tactile Buttons) |
| **Display** | 0.96" / 1.3" SSD1306 I2C OLED (128x64 pixels) |
| **Frame Rate** | ~60 FPS |
| **Controls** | Dual pushbuttons (Up / Down) with internal pull-ups |
| **Audio** | Piezo buzzer sound effects on bounce and goal |
| **Supported Chips** | ESP32 Classic, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6 |

---

## Hardware Pinout Reference

| Component Pin | ESP32 Classic GPIO | Notes |
|---|---|---|
| **OLED SDA** | GPIO 21 | I2C Data line |
| **OLED SCL** | GPIO 22 | I2C Clock line |
| **OLED VCC** | 3.3V / 5V | Powers the display module |
| **OLED GND** | GND | Ground rail |
| **Button UP** | GPIO 18 | Connect other side to GND (`INPUT_PULLUP`) |
| **Button DOWN** | GPIO 19 | Connect other side to GND (`INPUT_PULLUP`) |
| **Piezo Buzzer (+)** | GPIO 23 | Connect negative pin to GND |

---

## Gameplay Mechanics

- **Player:** Controls the left paddle using the UP and DOWN pushbuttons.
- **AI Opponent:** Tracks the vertical position of the ball in real time.
- **Angle Deflection:** Hitting the ball near the edge of your paddle introduces a sharp vertical trajectory, making it harder for the AI to return.
- **Speed Volleys:** The ball accelerates by 5% with each successful paddle return!

---

## Building and Flashing

### Using PlatformIO CLI
```bash
pio run -d projects/games/01-pong -e esp32dev -t upload
pio device monitor -d projects/games/01-pong
```

### Using Arduino IDE
1. Open `projects/games/01-pong/pong.ino`.
2. Install **Adafruit SSD1306** and **Adafruit GFX** via the Arduino Library Manager.
3. Select your ESP32 board and click **Upload**.

---

## Community & Credits

- Maintained by [ESP32 Community](https://github.com/tushar2704/ESP32-Community)
- Video guides & electronics builds: [@IamNotIntoTech](https://www.youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://www.youtube.com/@Makerrdesigns)
