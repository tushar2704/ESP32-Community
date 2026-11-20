# 🎙️ Digital ESP-NOW Walkie-Talkie (I2S Microphone & DAC)

> **Real-time digital push-to-talk (PTT) walkie-talkie using direct ESP-NOW raw 2.4 GHz radio frames. Streams 11,025 Hz 16-bit PCM audio directly between two ESP32 devices with sub-15ms latency without any Wi-Fi router or cellular connectivity.**

---

## 📋 Features

- ⚡ **Ultra-Low Latency Audio**: Sub-15ms voice streaming over ESP-NOW raw 802.11 vendor action frames.
- 📡 **Zero Infrastructure Required**: Works outdoors, in basements, or in remote areas with no Wi-Fi router or internet access.
- 🎙️ **Digital I2S Microphone**: Direct digital sampling via INMP441 MEMS microphone with zero analog hum.
- 🔊 **High-Fidelity Class-D DAC**: Crystal-clear speaker playback via MAX98357A I2S amplifier.
- 🔘 **Push-To-Talk (PTT)**: Half-duplex operation with tactile switch and transmission status LED.

---

## 🛠️ Bill of Materials (BOM)

| Component | Quantity | Description |
| :--- | :---: | :--- |
| **ESP32 DevKit** | 2 | Classic ESP32 or ESP32-S3 boards |
| **INMP441 Microphone** | 2 | Omnidirectional digital I2S MEMS mic |
| **MAX98357A DAC** | 2 | 3.2W Mono I2S Class-D amplifier module |
| **4Ω / 8Ω Speaker** | 2 | 2W–3W small speaker driver |
| **Tactile Pushbutton** | 2 | Push-To-Talk (PTT) switch |
| **Jumper Wires** | — | Standard breadboard wires |

---

## 🔌 Wiring Pinout

### Microphone (INMP441) → ESP32 (I2S Port 0)
| INMP441 Pin | ESP32 Pin | Function |
| :---: | :---: | :--- |
| **SCK** | **GPIO 14** | I2S Bit Clock (BCLK) |
| **WS** | **GPIO 15** | I2S Word Select (LRCLK) |
| **SD** | **GPIO 32** | I2S Serial Data Out |
| **L/R** | **GND** | Left Channel Select |
| **VDD** | **3.3V** | Power Supply |
| **GND** | **GND** | Ground reference |

### Amplifier DAC (MAX98357A) → ESP32 (I2S Port 1)
| MAX98357A Pin | ESP32 Pin | Function |
| :---: | :---: | :--- |
| **BCLK** | **GPIO 26** | I2S Bit Clock |
| **LRC** | **GPIO 27** | I2S Left/Right Word Clock |
| **DIN** | **GPIO 22** | I2S Serial Data In |
| **GAIN** | **GND** | 12 dB Gain Setting |
| **VIN** | **5V / 3.3V** | Power Supply |
| **GND** | **GND** | Ground reference |

### Push-To-Talk (PTT) & LED
| Component | ESP32 Pin | Description |
| :---: | :---: | :--- |
| **PTT Switch** | **GPIO 13** | Tied to GND on press (internal pull-up) |
| **Status LED** | **GPIO 2** | HIGH = Transmitting, LOW = Receiving |

---

## 🚀 Quick Start

### 1. Flash Both Boards
Flash the same firmware to both ESP32 boards:
```bash
cd projects/audio/06-walkie-talkie
pio run --target upload
```

### 2. Operation
1. Power up both ESP32 units with 5V USB power bank or 3.7V LiPo battery.
2. Hold the PTT button on GPIO 13 on Unit A and speak into the INMP441 microphone.
3. Unit B immediately plays back your voice with sub-15ms real-time fidelity!

---

<div align="center">

**Part of the [ESP32 Community Hub](https://github.com/tushar2704/ESP32-Community)**  
<sub>Maintained by Tushar Aggarwal ([@tushar2704](https://github.com/tushar2704)) • Videos on [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://youtube.com/@Makerrdesigns)</sub>

</div>
