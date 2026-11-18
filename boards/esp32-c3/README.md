# ESP32-C3 — Board Reference Card

> RISC-V meets IoT — the low-cost, low-power ESP8266 killer with WiFi + BLE 5.0, secure boot, and flash encryption.

## 📋 Chip Specifications

| Feature | Details |
|---|---|
| **Architecture** | RISC-V (32-bit) |
| **Cores** | 1 (single-core) |
| **Clock Speed** | 160 MHz |
| **WiFi** | 802.11 b/g/n (2.4 GHz) |
| **Bluetooth** | BLE 5.0 |
| **Thread/Zigbee** | ❌ No |
| **USB** | ❌ No native USB (USB-Serial-JTAG built-in) |
| **GPIO** | 22 |
| **ADC** | 6 channels (12-bit) |
| **DAC** | ❌ None |
| **Touch** | ❌ None |
| **I2C** | 1× |
| **SPI** | 3× (1 used for flash) |
| **I2S** | 1× |
| **UART** | 2× |
| **Flash** | 4 MB (external) |
| **PSRAM** | ❌ Not available |
| **Security** | ✅ Secure Boot V2, Flash Encryption, Digital Signature |
| **Operating Temp** | -40°C to 85°C |

## 📦 Module Variants

| Module | Flash | PSRAM | Antenna | Notes |
|---|---|---|---|---|
| **ESP32-C3-MINI-1** | 4 MB | ❌ | PCB | Compact, most popular |
| **ESP32-C3-WROOM-02** | 4 MB | ❌ | PCB | Standard module |
| **ESP32-C3-WROOM-02U** | 4 MB | ❌ | U.FL | External antenna |
| **ESP8685** | 2–4 MB | ❌ | PCB | Cost-optimized variant |

## 🛒 Recommended Development Boards

| Board | USB | Features | Link |
|---|---|---|---|
| **ESP32-C3-DevKitM-1** | USB-Serial-JTAG | Reference design | [Espressif](https://www.espressif.com/) |
| **XIAO ESP32-C3** | USB-Serial-JTAG | Ultra-compact, Seeed Studio | [Seeed Studio](https://www.seeedstudio.com/) |
| **ESP32-C3 SuperMini** | USB-Serial-JTAG | Tiny, breadboard-friendly, cheap | [Search](https://www.aliexpress.com/) |
| **Adafruit QT Py ESP32-C3** | USB-Serial-JTAG | STEMMA QT, Feather ecosystem | [Adafruit](https://www.adafruit.com/) |

## 📌 ESP32-C3-DevKitM-1 Pinout

```
                    ┌──────────┐
              3V3 ──┤ 1     18 ├── GND
         GPIO  0 ──┤ 2     17 ├── GPIO 10
         GPIO  1 ──┤ 3     16 ├── GPIO  9
         GPIO  2 ──┤ 4     15 ├── GPIO  8 (RGB LED)
         GPIO  3 ──┤ 5     14 ├── GPIO 19 (D-)
         GPIO  4 ──┤ 6     13 ├── GPIO 18 (D+)
         GPIO  5 ──┤ 7     12 ├── GPIO 21 (RX)
         GPIO  6 ──┤ 8     11 ├── GPIO 20 (TX)
         GPIO  7 ──┤ 9     10 ├── 5V
                    └──────────┘
                      USB-C
```

> [!WARNING]
> **Strapping Pins:**
> - **GPIO 2** — Boot mode (HIGH = normal boot, LOW = download mode)
> - **GPIO 8** — Boot mode / ROM log output (keep HIGH at boot)
> - **GPIO 9** — Boot button on most dev boards (keep HIGH for SPI boot)

> [!WARNING]
> **Reserved GPIOs:**
> - **GPIO 11** — Connected to flash VDD on most modules. **Do NOT use!**
> - **GPIO 12–17** — Connected to SPI flash. **Do NOT use!**
> - **GPIO 18/19** — USB D+/D- (used by USB-Serial-JTAG)

## 📚 Recommended Libraries

| Library | Purpose | Install |
|---|---|---|
| **WiFi.h** | WiFi connectivity | Built-in |
| **BLE** | BLE 5.0 | Built-in |
| **ESPAsyncWebServer** | Async HTTP server | PlatformIO/manual |
| **ArduinoJson** | JSON parsing | Library Manager |
| **PubSubClient** | MQTT client | Library Manager |
| **Preferences** | NVS key-value storage | Built-in |

## ⚡ Quick Start — RGB LED Blink

```cpp
// ESP32-C3-DevKitM-1 — Onboard RGB LED (WS2812 on GPIO 8)
// Board: ESP32-C3 Dev Module
#include <Adafruit_NeoPixel.h>

#define RGB_PIN 8
#define NUM_LEDS 1

Adafruit_NeoPixel pixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  pixel.begin();
  pixel.setBrightness(20);
  Serial.println("ESP32-C3 — RGB LED Blink!");
}

void loop() {
  pixel.setPixelColor(0, pixel.Color(0, 255, 0));  // Green
  pixel.show();
  delay(500);
  pixel.setPixelColor(0, pixel.Color(0, 0, 0));    // Off
  pixel.show();
  delay(500);
}
```

> [!TIP]
> The ESP32-C3 is the best drop-in replacement for ESP8266-based projects. It's pin-compatible with many ESP-12 footprints and adds BLE 5.0, secure boot, and a modern RISC-V core — all at a similar price point.

## 🔗 Official Resources

| Resource | Link |
|---|---|
| **Datasheet** | [ESP32-C3 Datasheet (PDF)](https://www.espressif.com/sites/default/files/documentation/esp32-c3_datasheet_en.pdf) |
| **Technical Reference** | [ESP32-C3 TRM](https://www.espressif.com/sites/default/files/documentation/esp32-c3_technical_reference_manual_en.pdf) |
| **Arduino Docs** | [Espressif Arduino Documentation](https://docs.espressif.com/projects/arduino-esp32/) |
| **ESP-IDF Guide** | [ESP-IDF for ESP32-C3](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/) |

---

<p align="center">Part of <a href="https://github.com/tushar2704/ESP32-Community">ESP32 Community</a> by <a href="https://github.com/tushar2704">Tushar Aggarwal</a></p>
