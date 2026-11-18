# ESP32-S3 — Board Reference Card

> The AI/ML powerhouse — dual-core Xtensa LX7 with vector instructions, native USB, WiFi + BLE 5.0, and massive PSRAM support.

## 📋 Chip Specifications

| Feature | Details |
|---|---|
| **Architecture** | Xtensa LX7 |
| **Cores** | 2 (dual-core) |
| **Clock Speed** | 240 MHz |
| **WiFi** | 802.11 b/g/n (2.4 GHz) |
| **Bluetooth** | BLE 5.0 |
| **Thread/Zigbee** | ❌ No |
| **USB** | ✅ Native USB OTG (1.1) |
| **AI/ML** | ✅ Vector instructions for acceleration |
| **GPIO** | 45 |
| **ADC** | 20 channels (12-bit) |
| **DAC** | ❌ None (removed from S3) |
| **Touch** | 14 capacitive touch pins |
| **I2C** | 2× |
| **SPI** | 4× (1 used for flash) |
| **I2S** | 2× |
| **UART** | 3× |
| **Flash** | 4–16 MB (external) |
| **PSRAM** | Up to 16 MB (Octal SPI) |
| **Camera** | DVP 8/16-bit (OV2640, OV5640) |
| **LCD** | 8/16-bit parallel, SPI, I2C |
| **Operating Temp** | -40°C to 85°C |

## 📦 Module Variants

| Module | Flash | PSRAM | Antenna | Notes |
|---|---|---|---|---|
| **ESP32-S3-WROOM-1** | 4–16 MB | ❌ | PCB | Standard, no PSRAM |
| **ESP32-S3-WROOM-1 (N16R8)** | 16 MB | 8 MB | PCB | Popular choice for projects |
| **ESP32-S3-WROOM-2** | 16–32 MB | 8–16 MB | PCB | Octal flash + PSRAM |
| **ESP32-S3-MINI-1** | 4–8 MB | ❌ | PCB | Compact module |

## 🛒 Recommended Development Boards

| Board | USB | Features | Link |
|---|---|---|---|
| **ESP32-S3-DevKitC-1** | Native USB + UART | Reference design, N16R8 | [Espressif](https://www.espressif.com/) |
| **XIAO ESP32-S3** | Native USB | Ultra-compact, camera connector | [Seeed Studio](https://www.seeedstudio.com/) |
| **XIAO ESP32-S3 Sense** | Native USB | Built-in OV2640 + mic + SD | [Seeed Studio](https://www.seeedstudio.com/) |
| **LilyGo T-Display-S3** | Native USB | 1.9" TFT built-in | [LilyGo](https://www.lilygo.cc/) |
| **Cheap Yellow Display 4.0"** | UART | 4" ILI9488 touch display | [Search](https://www.aliexpress.com/) |
| **Cheap Yellow Display 5.0"** | UART | 5" parallel touch display | [Search](https://www.aliexpress.com/) |
| **Cheap Yellow Display 7.0"** | UART | 7" 800×480 touch display | [Search](https://www.aliexpress.com/) |

## 📌 ESP32-S3-DevKitC-1 Pinout

```
                    ┌──────────┐
              3V3 ──┤ 1     44 ├── GND
         GPIO  0 ──┤ 2     43 ├── GPIO 48 (RGB LED)
         GPIO  1 ──┤ 3     42 ├── GPIO 47
         GPIO  2 ──┤ 4     41 ├── GPIO 21
         GPIO  3 ──┤ 5     40 ├── GPIO 20 (D-)
         GPIO  4 ──┤ 6     39 ├── GPIO 19 (D+)
         GPIO  5 ──┤ 7     38 ├── GPIO 18
         GPIO  6 ──┤ 8     37 ├── GPIO 17
         GPIO  7 ──┤ 9     36 ├── GPIO 16
         GPIO 15 ──┤10     35 ├── GPIO 15
         GPIO 16 ──┤11     34 ├── GPIO 14
         GPIO 17 ──┤12     33 ├── GPIO 13
         GPIO 18 ──┤13     32 ├── GPIO 12
         GPIO  8 ──┤14     31 ├── GPIO 11
         GPIO  9 ──┤15     30 ├── GPIO 10
         GPIO 10 ──┤16     29 ├── GPIO  9
         GPIO 11 ──┤17     28 ├── GPIO 46
         GPIO 12 ──┤18     27 ├── GPIO 45
         GPIO 13 ──┤19     26 ├── GPIO 44 (RX)
         GPIO 14 ──┤20     25 ├── GPIO 43 (TX)
              5V ──┤21     24 ├── RST
              GND ──┤22     23 ├── GND
                    └──────────┘
                 USB-C (native)  USB-C (UART)
```

> [!WARNING]
> **Strapping Pins:**
> - **GPIO 0** — Boot mode (HIGH = normal, LOW = download mode)
> - **GPIO 3** — JTAG signal source selection
> - **GPIO 45** — VDD_SPI voltage select (keep LOW for 3.3V)
> - **GPIO 46** — Boot mode / ROM log (input-only, no pull-up)

> [!WARNING]
> **Reserved GPIOs:**
> - **GPIO 19/20** — USB D+/D- (native USB)
> - **GPIO 26–32** — Connected to Octal SPI flash/PSRAM on N8R8/N16R8 modules. **Do NOT use!**
> - **GPIO 33–37** — Also Octal SPI on WROOM-2 modules

## 📚 Recommended Libraries

| Library | Purpose | Install |
|---|---|---|
| **WiFi.h** | WiFi connectivity | Built-in |
| **BLE** | BLE 5.0 peripheral/central | Built-in |
| **USB.h** | Native USB OTG | Built-in |
| **ESP-DL** | AI/ML inference (TensorFlow Lite) | ESP-IDF component |
| **esp32-camera** | Camera driver (OV2640/OV5640) | Library Manager |
| **TFT_eSPI** | Display driver | Library Manager |
| **LVGL** | Graphics UI framework | Library Manager |
| **LovyanGFX** | Fast display driver | Library Manager |

## ⚡ Quick Start — RGB LED Blink (Neopixel)

```cpp
// ESP32-S3-DevKitC-1 — Onboard RGB LED (WS2812 on GPIO 48)
// Board: ESP32-S3 Dev Module
#include <Adafruit_NeoPixel.h>

#define RGB_PIN 48
#define NUM_LEDS 1

Adafruit_NeoPixel pixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  pixel.begin();
  pixel.setBrightness(30);
  Serial.println("ESP32-S3 — RGB LED Blink!");
}

void loop() {
  pixel.setPixelColor(0, pixel.Color(255, 0, 0));  // Red
  pixel.show();
  delay(500);
  pixel.setPixelColor(0, pixel.Color(0, 0, 255));  // Blue
  pixel.show();
  delay(500);
}
```

> [!TIP]
> The ESP32-S3 is the best choice for camera projects, TinyML inference, and display-heavy applications thanks to its vector instructions, large PSRAM support, and dual USB ports.

## 🔗 Official Resources

| Resource | Link |
|---|---|
| **Datasheet** | [ESP32-S3 Datasheet (PDF)](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf) |
| **Technical Reference** | [ESP32-S3 TRM](https://www.espressif.com/sites/default/files/documentation/esp32-s3_technical_reference_manual_en.pdf) |
| **Arduino Docs** | [Espressif Arduino Documentation](https://docs.espressif.com/projects/arduino-esp32/) |
| **ESP-IDF Guide** | [ESP-IDF for ESP32-S3](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/) |

---

<p align="center">Part of <a href="https://github.com/tushar2704/ESP32-Community">ESP32 Community</a> by <a href="https://github.com/tushar2704">Tushar Aggarwal</a></p>
