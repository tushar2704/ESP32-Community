# Project 02: Retro Snake with NVS High-Score

Classic Nokia-style Snake running on an ESP32 and SSD1306 OLED, complete with persistent high-score saving to ESP32 Flash Non-Volatile Storage (NVS).

---

## Overview

This project implements the timeless arcade game Snake using a 2-button rotational steering mechanic (Left / Right). When players beat the high score, the new score is written directly to the ESP32's internal NVS partition using `Preferences.h`, preserving the top score across reboots and power cycles.

| Feature | Specification |
|---|---|
| **Difficulty** | Intermediate |
| **BOM Cost** | ~\$6–\$8 (ESP32 Dev Board + 0.96" I2C OLED + 2 Tactile Buttons) |
| **Display** | 0.96" / 1.3" SSD1306 I2C OLED (128x64 pixels) |
| **Persistence** | ESP32 Flash NVS (`Preferences`) |
| **Steering** | 2-button relative steering (Left Turn / Right Turn) |
| **Speed Scaling** | Game speed dynamically accelerates as score increases |
| **Supported Chips** | ESP32 Classic, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6 |

---

## Hardware Pinout Reference

| Component Pin | ESP32 Classic GPIO | Notes |
|---|---|---|
| **OLED SDA** | GPIO 21 | I2C Data line |
| **OLED SCL** | GPIO 22 | I2C Clock line |
| **OLED VCC** | 3.3V / 5V | Powers the display module |
| **OLED GND** | GND | Ground rail |
| **Button LEFT** | GPIO 18 | Connect other side to GND (`INPUT_PULLUP`) |
| **Button RIGHT** | GPIO 19 | Connect other side to GND (`INPUT_PULLUP`) |
| **Piezo Buzzer (+)** | GPIO 23 | Connect negative pin to GND |

---

## How NVS Persistence Works

Unlike traditional microcontrollers with limited EEPROM cycles, the ESP32 provides a structured key-value storage system called **Non-Volatile Storage (NVS)** stored in flash memory:

```cpp
#include <Preferences.h>
Preferences prefs;

// Read high score at boot
prefs.begin("snake-game", false);
highScore = prefs.getInt("high_score", 0);

// Save updated score upon game over
if (currentScore > highScore) {
  highScore = currentScore;
  prefs.putInt("high_score", highScore);
}
```

---

## Building and Flashing

### Using PlatformIO CLI
```bash
pio run -d projects/games/02-snake -e esp32dev -t upload
pio device monitor -d projects/games/02-snake
```

### Using Arduino IDE
1. Open `projects/games/02-snake/snake.ino`.
2. Install **Adafruit SSD1306** and **Adafruit GFX** via the Arduino Library Manager.
3. Select your ESP32 board and click **Upload**.

---

## Community & Credits

- Maintained by [ESP32 Community](https://github.com/tushar2704/ESP32-Community)
- Video guides & electronics builds: [@IamNotIntoTech](https://www.youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://www.youtube.com/@Makerrdesigns)
