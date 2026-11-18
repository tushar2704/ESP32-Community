# ESP32 (Classic) — Board Reference Card

> The original ESP32 — the chip that started the revolution. Dual-core powerhouse with WiFi + Bluetooth combo.

## 📋 Chip Specifications

| Feature | Details |
|---|---|
| **Architecture** | Xtensa LX6 |
| **Cores** | 2 (dual-core) |
| **Clock Speed** | 240 MHz |
| **WiFi** | 802.11 b/g/n (2.4 GHz) |
| **Bluetooth** | Classic BT 4.2 + BLE |
| **Thread/Zigbee** | ❌ No |
| **USB** | ❌ No native USB |
| **GPIO** | 34 |
| **ADC** | 18 channels (12-bit) |
| **DAC** | 2 channels (8-bit) |
| **Touch** | 10 capacitive touch pins |
| **I2C** | 2× |
| **SPI** | 3× (1 used for flash) |
| **I2S** | 2× |
| **UART** | 3× |
| **Flash** | 4–16 MB (external) |
| **PSRAM** | Up to 8 MB (WROVER modules) |
| **Operating Temp** | -40°C to 85°C |

## 📦 Module Variants

| Module | Flash | PSRAM | Antenna | Notes |
|---|---|---|---|---|
| **ESP32-WROOM-32** | 4 MB | ❌ | PCB | Original module |
| **ESP32-WROOM-32D** | 4–16 MB | ❌ | PCB | Improved revision |
| **ESP32-WROOM-32E** | 4–16 MB | ❌ | PCB | Latest revision, recommended |
| **ESP32-WROOM-32U** | 4–16 MB | ❌ | U.FL | External antenna connector |
| **ESP32-WROVER** | 4 MB | 8 MB | PCB | PSRAM for camera/display apps |
| **ESP32-WROVER-B** | 4–16 MB | 8 MB | PCB | Improved WROVER |
| **ESP32-WROVER-E** | 4–16 MB | 8 MB | PCB | Latest WROVER revision |

## 🛒 Recommended Development Boards

| Board | USB Chip | Features | Link |
|---|---|---|---|
| **ESP32-DevKitC V4** | CP2102 | Reference design, breadboard-friendly | [Espressif](https://www.espressif.com/) |
| **NodeMCU-32S** | CH340 | Popular, affordable | [Search](https://www.aliexpress.com/) |
| **Wemos D1 Mini ESP32** | CH9102 | Compact form factor | [Search](https://www.aliexpress.com/) |
| **DOIT ESP32 DevKit V1** | CP2102 | 30-pin, widely available | [Search](https://www.amazon.com/) |

## 📌 DevKitC V4 Pinout

```
                    ┌──────────┐
              3V3 ──┤ 1     38 ├── GND
            RESET ──┤ 2     37 ├── GPIO 23 (MOSI)
         GPIO 36 ──┤ 3     36 ├── GPIO 22 (SCL)
         GPIO 39 ──┤ 4     35 ├── GPIO  1 (TX0)
         GPIO 34 ──┤ 5     34 ├── GPIO  3 (RX0)
         GPIO 35 ──┤ 6     33 ├── GPIO 21 (SDA)
         GPIO 32 ──┤ 7     32 ├── GND
         GPIO 33 ──┤ 8     31 ├── GPIO 19 (MISO)
         GPIO 25 ──┤ 9     30 ├── GPIO 18 (SCK)
         GPIO 26 ──┤10     29 ├── GPIO  5 (SS)
         GPIO 27 ──┤11     28 ├── GPIO 17 (TX2)
         GPIO 14 ──┤12     27 ├── GPIO 16 (RX2)
         GPIO 12 ──┤13     26 ├── GPIO  4
              GND ──┤14     25 ├── GPIO  0 (BOOT)
         GPIO 13 ──┤15     24 ├── GPIO  2 (LED)
         GPIO  9 ──┤16     23 ├── GPIO 15
         GPIO 10 ──┤17     22 ├── GPIO  8
         GPIO 11 ──┤18     21 ├── GPIO  7
              5V ──┤19     20 ├── GPIO  6
                    └──────────┘
                      USB-C/Micro
```

> [!WARNING]
> **Strapping Pins — Do NOT connect at boot:**
> - **GPIO 0** — Must be HIGH for normal boot (LOW = download mode)
> - **GPIO 2** — Must be LOW or floating for flash boot
> - **GPIO 5** — Controls SDIO timing (keep floating)
> - **GPIO 12** — Sets flash voltage (keep LOW for 3.3V flash)
> - **GPIO 15** — Controls boot log output (keep HIGH for serial log)

> [!WARNING]
> **Reserved GPIOs — Cannot be used freely:**
> - **GPIO 6–11** — Connected to onboard SPI flash. **NEVER use these!**
> - **GPIO 34–39** — Input-only pins (no internal pull-up/pull-down)
> - **GPIO 1, 3** — Default UART0 TX/RX (used for programming/serial)

## 📚 Recommended Libraries

| Library | Purpose | Install |
|---|---|---|
| **WiFi.h** | WiFi connectivity | Built-in |
| **BluetoothSerial.h** | Classic Bluetooth SPP | Built-in |
| **BLE** | BLE peripheral/central | Built-in |
| **Wire.h** | I2C communication | Built-in |
| **ESPAsyncWebServer** | Async web server | PlatformIO/manual |
| **FastLED** | Addressable LEDs | Library Manager |
| **TFT_eSPI** | TFT display driver | Library Manager |

## ⚡ Quick Start — LED Blink

```cpp
// ESP32 Classic — Built-in LED Blink
// Board: ESP32 Dev Module

#define LED_PIN 2  // Onboard LED on most DevKitC boards

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("ESP32 Classic — LED Blink!");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
}
```

## 🔗 Official Resources

| Resource | Link |
|---|---|
| **Datasheet** | [ESP32 Datasheet (PDF)](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf) |
| **Technical Reference** | [ESP32 TRM](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf) |
| **Arduino Docs** | [Espressif Arduino Documentation](https://docs.espressif.com/projects/arduino-esp32/) |
| **ESP-IDF Guide** | [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/) |

---

<p align="center">Part of <a href="https://github.com/tushar2704/ESP32-Community">ESP32 Community</a> by <a href="https://github.com/tushar2704">Tushar Aggarwal</a></p>
