# 🛠️ ESP32 External Hardware & Add-on Modules Guide

> **A practical guide to connecting sensors, displays, communication transceivers, and actuators to your ESP32 boards.**

---

## 📋 Recommended Hardware Directory

| Category | Component / Module | Interface | Default Safe Pins (Classic ESP32) | Recommended Arduino Library |
| :--- | :--- | :--- | :--- | :--- |
| **Displays** | 0.96" SSD1306 OLED (128x64) | I2C | SDA: GPIO 21, SCL: GPIO 22 | `Adafruit_SSD1306` / `U8g2` |
| **Displays** | 2.8" ILI9341 SPI TFT LCD | SPI | MOSI: 23, MISO: 19, SCK: 18, CS: 5, DC: 2 | `TFT_eSPI` / `LovyanGFX` |
| **Displays** | 2.13" E-Paper Display | SPI | MOSI: 23, SCK: 18, CS: 5, DC: 17, RST: 16 | `GxEPD2` |
| **Sensors** | BME280 (Temp/Hum/Pressure) | I2C | SDA: GPIO 21, SCL: GPIO 22 | `Adafruit_BME280` |
| **Sensors** | DHT22 / AM2302 (Temp/Hum) | 1-Wire | DATA: GPIO 4 (10k pull-up) | `DHT sensor library` |
| **Sensors** | MPU6050 (6-Axis IMU Gyro/Accel)| I2C | SDA: GPIO 21, SCL: GPIO 22 | `Adafruit_MPU6050` |
| **Sensors** | VL53L0X (ToF Laser Distance) | I2C | SDA: GPIO 21, SCL: GPIO 22 | `Adafruit_VL53L0X` |
| **Sensors** | LD2410 (mmWave Human Presence) | UART | RX: GPIO 16, TX: GPIO 17 | `ld2410` |
| **Sensors** | SCD40 (Photoacoustic CO2/PPM) | I2C | SDA: GPIO 21, SCL: GPIO 22 | `SensirionI2CScd4x` |
| **Audio** | MAX98357A (I2S Mono 3W Amp) | I2S | BCLK: 26, LRC: 25, DIN: 22 | `ESP32-audioI2S` |
| **Audio** | INMP441 (I2S Digital Mic) | I2S | SCK: 14, WS: 15, SD: 32 | `arduino-audio-tools` |
| **Audio** | PCM5102A (I2S High-Res DAC) | I2S | BCK: 26, LRCK: 25, DATA: 22 | `ESP32-audioI2S` |
| **Wireless** | SX1276 / SX1278 (LoRa 868/915) | SPI | SCK: 18, MISO: 19, MOSI: 23, NSS: 5, DIO0: 26 | `RadioLib` |
| **Wireless** | NEO-6M / NEO-M8N (GPS) | UART | RX: GPIO 16 (to TX), TX: GPIO 17 (to RX) | `TinyGPSPlus` |
| **Wireless** | PN532 (NFC / RFID Reader) | I2C | SDA: GPIO 21, SCL: GPIO 22 | `Adafruit_PN532` |
| **Industrial**| MAX485 / RS485 Transceiver | UART | RX: 16, TX: 17, DE/RE: GPIO 4 | `ModbusMaster` |
| **Industrial**| SN65HVD230 (CAN Bus Transceiver)| CAN | TX: GPIO 5, RX: GPIO 4 | Built-in ESP32 TWAI driver |
| **Actuators** | SG90 / MG996R Servos | PWM | Signal: GPIO 13, 14, 27 | `ESP32Servo` |
| **Actuators** | WS2812B / NeoPixel LED Strip | 1-Wire | Data: GPIO 16, 17, or 27 | `FastLED` / `Adafruit_NeoPixel` |
| **Actuators** | 5V Relay Module | Digital | IN: GPIO 26 or 27 (Active LOW/HIGH) | Direct `digitalWrite()` |

---

## ⚡ Quick Wiring Snippets

### 1. I2C Bus Connection
Connect all standard I2C sensors in parallel:
```text
ESP32 3V3  ────────► Sensor VCC (Check if 3.3V safe!)
ESP32 GND  ────────► Sensor GND
ESP32 21   ────────► Sensor SDA
ESP32 22   ────────► Sensor SCL
```

### 2. High-Quality I2S Audio Output (MAX98357A)
```text
ESP32 5V (VIN) ────► Amp VIN (for max 3W power)
ESP32 GND      ────► Amp GND
ESP32 GPIO 26  ────► Amp BCLK (Bit Clock)
ESP32 GPIO 25  ────► Amp LRC (Word Select / Left-Right Clock)
ESP32 GPIO 22  ────► Amp DIN (Data Input)
```

### 3. LoRa SX1276 Transceiver Wiring
```text
ESP32 3V3      ────► SX1276 VCC (NEVER 5V!)
ESP32 GND      ────► SX1276 GND
ESP32 GPIO 18  ────► SX1276 SCK
ESP32 GPIO 19  ────► SX1276 MISO
ESP32 GPIO 23  ────► SX1276 MOSI
ESP32 GPIO 5   ────► SX1276 NSS / CS
ESP32 GPIO 26  ────► SX1276 DIO0
ESP32 GPIO 14  ────► SX1276 RST
```

---

<div align="center">

**Part of the [ESP32 Community Hub](https://github.com/tushar2704/ESP32-Community)**  
<sub>Maintained by Tushar Aggarwal ([@tushar2704](https://github.com/tushar2704))</sub>

</div>
