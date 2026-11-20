# 🕹️ 60 FPS Retro OLED Tetris

> **Full 60 FPS Tetris arcade engine running on ESP32 with an SSD1306 0.96" OLED display. Features 7 standard tetromino shapes, rotation with wall kicks, drop acceleration, piezo audio, and persistent flash high-score tracking.**

---

## 📋 Features

- 🎮 **Authentic Tetris Mechanics**: Standard 7 tetrominoes (`I`, `J`, `L`, `O`, `S`, `T`, `Z`) with 4-state rotations and boundary checks.
- 📺 **OLED Optimized**: Scaled 10×20 block grid with sidebar displaying live score, persistent high score, and line counter.
- ⚡ **60 FPS Rendering**: Smooth, flicker-free graphics buffer updates.
- 🔊 **8-Bit Audio Sound Effects**: Piezo buzzer tones for piece movements, rotations, line clears, and game-over fanfares.
- 💾 **NVS Persistent High Score**: High score is saved directly to non-volatile flash storage using ESP32 Preferences API.
- 🌐 **Interactive Browser Simulation**: Includes `diagram.json` and `wokwi.toml` for 1-click browser simulation in Wokwi.

---

## 🛠️ Bill of Materials (BOM)

| Component | Quantity | Description |
| :--- | :---: | :--- |
| **ESP32 DevKit** | 1 | Classic ESP32, ESP32-S3, or ESP32-C3 |
| **0.96" SSD1306 OLED** | 1 | I2C monochrome 128×64 display |
| **Tactile Pushbuttons** | 4 | Left, Right, Rotate, Drop |
| **Passive Piezo Buzzer** | 1 | 5V / 3.3V buzzer |
| **Breadboard & Wires** | 1 | Standard jumper wires |

---

## 🔌 Wiring Pinout

| ESP32 Pin | Component Pin | Description |
| :---: | :---: | :--- |
| **GPIO 21** | OLED **SDA** | I2C Data line |
| **GPIO 22** | OLED **SCL** | I2C Clock line |
| **GPIO 18** | Button **Left** | Move Left (Tied to GND on press) |
| **GPIO 19** | Button **Right** | Move Right (Tied to GND on press) |
| **GPIO 23** | Button **Rotate** | Rotate Tetromino (Tied to GND on press) |
| **GPIO 5**  | Button **Drop** | Hard Drop (Tied to GND on press) |
| **GPIO 25** | Buzzer **(+)** | Audio tone generation |
| **GND** | All Button GND / OLED GND / Buzzer (-) | Common Ground |
| **3.3V** | OLED **VCC** | Power Supply |

---

## 🚀 Quick Start

### 1. PlatformIO
```bash
cd projects/games/03-tetris
pio run --target upload
```

### 2. Browser Simulation (Wokwi)
Open `diagram.json` in [Wokwi](https://wokwi.com) to play Tetris directly in your browser with keyboard and mouse button controls!

---

<div align="center">

**Part of the [ESP32 Community Hub](https://github.com/tushar2704/ESP32-Community)**  
<sub>Maintained by Tushar Aggarwal ([@tushar2704](https://github.com/tushar2704)) • Videos on [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://youtube.com/@Makerrdesigns)</sub>

</div>
