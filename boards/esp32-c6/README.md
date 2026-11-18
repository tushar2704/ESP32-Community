# ESP32-C6 — Board Reference Card

> The smart home chip — WiFi 6 + BLE 5.0 + Thread/Zigbee/Matter on a single RISC-V die. The future of IoT connectivity.

## 📋 Chip Specifications

| Feature | Details |
|---|---|
| **Architecture** | RISC-V (32-bit) |
| **Cores** | 1 high-performance + 1 low-power |
| **Clock Speed** | 160 MHz (HP core) / 20 MHz (LP core) |
| **WiFi** | 802.11ax — WiFi 6 (2.4 GHz) |
| **Bluetooth** | BLE 5.0 |
| **Thread/Zigbee** | ✅ IEEE 802.15.4 |
| **Matter** | ✅ Native support via Thread/WiFi |
| **USB** | ❌ No native USB (USB-Serial-JTAG built-in) |
| **GPIO** | 30 |
| **ADC** | 7 channels (12-bit) |
| **DAC** | ❌ None |
| **Touch** | ❌ None |
| **I2C** | 2× |
| **SPI** | 1× (general purpose) |
| **I2S** | 1× |
| **UART** | 3× |
| **Flash** | 4–8 MB (external) |
| **PSRAM** | ❌ Not available |
| **Security** | ✅ Secure Boot V2, Flash Encryption, Digital Signature |
| **Operating Temp** | -40°C to 85°C |

## 📦 Module Variants

| Module | Flash | PSRAM | Antenna | Notes |
|---|---|---|---|---|
| **ESP32-C6-WROOM-1** | 4–8 MB | ❌ | PCB | Standard module |
| **ESP32-C6-WROOM-1U** | 4–8 MB | ❌ | U.FL | External antenna |
| **ESP32-C6-MINI-1** | 4 MB | ❌ | PCB | Compact module |

## 🛒 Recommended Development Boards

| Board | USB | Features | Link |
|---|---|---|---|
| **ESP32-C6-DevKitC-1** | USB-Serial-JTAG + UART | Reference board, N8 | [Espressif](https://www.espressif.com/) |
| **ESP32-C6-DevKitM-1** | USB-Serial-JTAG | MINI-1 module based | [Espressif](https://www.espressif.com/) |
| **XIAO ESP32-C6** | USB-Serial-JTAG | Ultra-compact, Seeed Studio | [Seeed Studio](https://www.seeedstudio.com/) |
| **Adafruit QT Py ESP32-C6** | USB-Serial-JTAG | STEMMA QT | [Adafruit](https://www.adafruit.com/) |

## 📌 ESP32-C6-DevKitC-1 Pinout

```
                    ┌──────────┐
              3V3 ──┤ 1     30 ├── GND
         GPIO  0 ──┤ 2     29 ├── GPIO 23
         GPIO  1 ──┤ 3     28 ├── GPIO 22
         GPIO  2 ──┤ 4     27 ├── GPIO 21
         GPIO  3 ──┤ 5     26 ├── GPIO 20
         GPIO  4 ──┤ 6     25 ├── GPIO 19
         GPIO  5 ──┤ 7     24 ├── GPIO 18
         GPIO  6 ──┤ 8     23 ├── GPIO 17 (TX)
         GPIO  7 ──┤ 9     22 ├── GPIO 16 (RX)
         GPIO  8 ──┤10     21 ├── GPIO 15
         GPIO  9 ──┤11     20 ├── GPIO 14
         GPIO 10 ──┤12     19 ├── GPIO 13 (D-)
         GPIO 11 ──┤13     18 ├── GPIO 12 (D+)
              5V ──┤14     17 ├── RST
              GND ──┤15     16 ├── GND
                    └──────────┘
                      USB-C
```

> [!WARNING]
> **Strapping Pins:**
> - **GPIO 8** — Boot mode (HIGH = SPI boot, LOW = download mode)
> - **GPIO 9** — Boot button on dev boards (keep HIGH at boot)
> - **GPIO 15** — Controls boot log UART output

> [!WARNING]
> **Reserved GPIOs:**
> - **GPIO 12/13** — USB D+/D- (used by USB-Serial-JTAG)
> - **GPIO 24–30** — Connected to SPI flash. **Do NOT use!**

## 🏠 Thread/Zigbee/Matter — Why C6 Matters

The ESP32-C6 is Espressif's first chip with **IEEE 802.15.4** radio, enabling:

- **Thread** — Mesh networking protocol for smart home (backed by Google, Apple, Amazon)
- **Zigbee** — Established smart home protocol (Philips Hue, IKEA)
- **Matter** — Unified smart home standard running over Thread or WiFi
- **Border Router** — ESP32-C6 can serve as a Thread Border Router bridging Thread mesh to WiFi

## 📚 Recommended Libraries

| Library | Purpose | Install |
|---|---|---|
| **WiFi.h** | WiFi 6 connectivity | Built-in |
| **BLE** | BLE 5.0 | Built-in |
| **esp_openthread** | Thread protocol stack | ESP-IDF component |
| **esp_zigbee** | Zigbee 3.0 stack | ESP-IDF component |
| **esp_matter** | Matter protocol | ESP-IDF component |
| **ESPAsyncWebServer** | Async HTTP server | PlatformIO/manual |

## ⚡ Quick Start — LED Blink

```cpp
// ESP32-C6-DevKitC-1 — Onboard RGB LED (WS2812 on GPIO 8)
// Board: ESP32-C6 Dev Module
#include <Adafruit_NeoPixel.h>

#define RGB_PIN 8
#define NUM_LEDS 1

Adafruit_NeoPixel pixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  pixel.begin();
  pixel.setBrightness(20);
  Serial.println("ESP32-C6 — WiFi 6 + Thread ready!");
}

void loop() {
  pixel.setPixelColor(0, pixel.Color(0, 128, 255));  // Cyan
  pixel.show();
  delay(500);
  pixel.setPixelColor(0, pixel.Color(0, 0, 0));
  pixel.show();
  delay(500);
}
```

> [!TIP]
> For Matter smart home devices, use **ESP-IDF** (not Arduino) with the `esp-matter` component. Matter support in Arduino is still experimental. The ESP32-C6 is the ideal chip for Matter-over-Thread endpoints.

## 🔗 Official Resources

| Resource | Link |
|---|---|
| **Datasheet** | [ESP32-C6 Datasheet (PDF)](https://www.espressif.com/sites/default/files/documentation/esp32-c6_datasheet_en.pdf) |
| **Technical Reference** | [ESP32-C6 TRM](https://www.espressif.com/sites/default/files/documentation/esp32-c6_technical_reference_manual_en.pdf) |
| **ESP-IDF Guide** | [ESP-IDF for ESP32-C6](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/) |
| **Thread & Matter Docs** | [Espressif Thread Solution](https://docs.espressif.com/projects/esp-thread-br/en/latest/) |

---

<p align="center">Part of <a href="https://github.com/tushar2704/ESP32-Community">ESP32 Community</a> by <a href="https://github.com/tushar2704">Tushar Aggarwal</a></p>
