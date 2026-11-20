# 🧠 Simon Says Hardware Memory Game

> **Interactive 4-button hardware memory game with progressive sequence generation, musical tone synthesis via LEDC, flash high-score persistence, and browser-based Wokwi simulation.**

---

## 📋 Features

- 🎮 **Classic Arcade Gameplay**: Memorize and repeat an ever-lengthening sequence of light and sound cues.
- 🎵 **Tone Synthesis**: 4 distinct musical frequencies (C4 261Hz, E4 329Hz, G4 392Hz, C5 523Hz) synthesized over piezo buzzer.
- ⚡ **Dynamic Speed Scaling**: Sequence playback speeds up automatically as levels advance.
- 💾 **Flash NVS Leaderboard**: Saves all-time record score directly to onboard flash memory via Preferences API.
- 🌐 **Interactive Wokwi Simulation**: Play instantly in your browser without hardware using the included `diagram.json` and `wokwi.toml`.

---

## 🛠️ Bill of Materials (BOM)

| Component | Quantity | Description |
| :--- | :---: | :--- |
| **ESP32 DevKit** | 1 | Classic ESP32, ESP32-S3, or ESP32-C3 |
| **Pushbuttons** | 4 | Red, Green, Blue, Yellow tactile switches |
| **5mm LEDs** | 4 | Red, Green, Blue, Yellow LEDs |
| **Resistors** | 4 | 220Ω current-limiting resistors for LEDs |
| **Piezo Buzzer** | 1 | Passive buzzer |
| **Breadboard & Wires**| 1 | Standard jumper wires |

---

## 🔌 Wiring Pinout

| Color | Pushbutton GPIO | LED GPIO | Audio Tone |
| :---: | :---: | :---: | :---: |
| 🔴 **Red** | **GPIO 18** | **GPIO 16** | 261 Hz (C4) |
| 🟢 **Green** | **GPIO 19** | **GPIO 17** | 329 Hz (E4) |
| 🔵 **Blue** | **GPIO 23** | **GPIO 21** | 392 Hz (G4) |
| 🟡 **Yellow** | **GPIO 5** | **GPIO 22** | 523 Hz (C5) |
| 🔊 **Buzzer** | — | **GPIO 25** | Variable |

*Note: All pushbuttons connect from their designated GPIO pin to GND (using ESP32 internal pull-ups).*

---

## 🚀 Quick Start

### 1. PlatformIO
```bash
cd projects/games/09-simon-says
pio run --target upload
pio device monitor -b 115200
```

### 2. Browser Simulation (Wokwi)
Open `diagram.json` in [Wokwi](https://wokwi.com) to test the gameplay and audio feedback directly in Chrome or Edge!

---

<div align="center">

**Part of the [ESP32 Community Hub](https://github.com/tushar2704/ESP32-Community)**  
<sub>Maintained by Tushar Aggarwal ([@tushar2704](https://github.com/tushar2704)) • Videos on [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://youtube.com/@Makerrdesigns)</sub>

</div>
