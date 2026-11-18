# ESP32-H2 — Board Reference Card

> Ultra-low-power Thread/Zigbee mesh node — BLE 5.0 + IEEE 802.15.4 with **no WiFi**. Built for battery-powered smart home endpoints.

## 📋 Chip Specifications

| Feature | Details |
|---|---|
| **Architecture** | RISC-V (32-bit) |
| **Cores** | 1 (single-core) |
| **Clock Speed** | 96 MHz |
| **WiFi** | ❌ None |
| **Bluetooth** | BLE 5.0 (LE Coded PHY, 2M PHY) |
| **Thread/Zigbee** | ✅ IEEE 802.15.4 |
| **Matter** | ✅ Via Thread |
| **USB** | ❌ No native USB (USB-Serial-JTAG built-in) |
| **GPIO** | 26 |
| **ADC** | 5 channels (12-bit) |
| **DAC** | ❌ None |
| **Touch** | ❌ None |
| **I2C** | 2× |
| **SPI** | 2× (1 used for flash) |
| **I2S** | 1× |
| **UART** | 2× |
| **Flash** | 4 MB (external) |
| **PSRAM** | ❌ Not available |
| **Security** | ✅ Secure Boot V2, Flash Encryption, Digital Signature, HMAC |
| **Operating Temp** | -40°C to 85°C |

## 📦 Module Variants

| Module | Flash | PSRAM | Antenna | Notes |
|---|---|---|---|---|
| **ESP32-H2-MINI-1** | 4 MB | ❌ | PCB | Standard module |
| **ESP32-H2-MINI-1U** | 4 MB | ❌ | U.FL | External antenna |

## 🛒 Recommended Development Boards

| Board | USB | Features | Link |
|---|---|---|---|
| **ESP32-H2-DevKitM-1** | USB-Serial-JTAG | Reference design | [Espressif](https://www.espressif.com/) |

## 📌 ESP32-H2-DevKitM-1 Pinout

```
                    ┌──────────┐
              3V3 ──┤ 1     26 ├── GND
         GPIO  0 ──┤ 2     25 ├── GPIO 14
         GPIO  1 ──┤ 3     24 ├── GPIO 13
         GPIO  2 ──┤ 4     23 ├── GPIO 12
         GPIO  3 ──┤ 5     22 ├── GPIO 11
         GPIO  4 ──┤ 6     21 ├── GPIO 10
         GPIO  5 ──┤ 7     20 ├── GPIO  9
         GPIO 25 ──┤ 8     19 ├── GPIO  8 (RGB LED)
         GPIO 26 ──┤ 9     18 ├── GPIO 27 (D-)
         GPIO 22 ──┤10     17 ├── GPIO 26 (D+)
         GPIO 23 ──┤11     16 ├── GPIO 24 (TX)
         GPIO 24 ──┤12     15 ├── GPIO 23 (RX)
              5V ──┤13     14 ├── RST
                    └──────────┘
                      USB-C
```

> [!WARNING]
> **Strapping Pins:**
> - **GPIO 8** — Boot mode selection (HIGH = normal, LOW = download)
> - **GPIO 9** — Boot button on dev board (keep HIGH for SPI boot)
> - **GPIO 25** — Boot log UART output control

> [!WARNING]
> **Reserved GPIOs:**
> - **GPIO 15–21** — Connected to SPI flash. **Do NOT use!**
> - **GPIO 26/27** — USB D+/D- (used by USB-Serial-JTAG)

## 🔋 Ultra-Low-Power Design

The ESP32-H2 is optimized for **battery-powered endpoints**:

| Power Mode | Current | Use Case |
|---|---|---|
| **Active (TX)** | ~63 mA | Thread/Zigbee transmission |
| **Active (RX)** | ~52 mA | Receiving packets |
| **Modem Sleep** | ~3.2 mA | CPU active, radio off |
| **Light Sleep** | ~130 µA | Periodic wake-up |
| **Deep Sleep** | ~7 µA | RTC timer wake only |

> [!TIP]
> Pair with a CR2032 coin cell or small LiPo battery for wireless sensors that last months or years. Use Thread Sleepy End Devices (SED) for maximum battery life.

## 📚 Recommended Libraries

| Library | Purpose | Install |
|---|---|---|
| **BLE** | BLE 5.0 | Built-in |
| **esp_openthread** | Thread protocol stack | ESP-IDF component |
| **esp_zigbee** | Zigbee 3.0 stack | ESP-IDF component |
| **esp_matter** | Matter endpoint | ESP-IDF component |
| **Wire.h** | I2C sensors | Built-in |

## ⚡ Quick Start — LED Blink

```cpp
// ESP32-H2-DevKitM-1 — Onboard RGB LED (WS2812 on GPIO 8)
// Board: ESP32-H2 Dev Module
#include <Adafruit_NeoPixel.h>

#define RGB_PIN 8
#define NUM_LEDS 1

Adafruit_NeoPixel pixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  pixel.begin();
  pixel.setBrightness(20);
  Serial.println("ESP32-H2 — Thread/Zigbee Mesh Node!");
}

void loop() {
  pixel.setPixelColor(0, pixel.Color(255, 165, 0));  // Orange
  pixel.show();
  delay(1000);
  pixel.setPixelColor(0, pixel.Color(0, 0, 0));
  pixel.show();
  delay(1000);
}
```

## 🏠 Use Cases

- **Thread temperature/humidity sensor** → Battery-powered, mesh-connected
- **Zigbee door/window sensor** → Works with Zigbee2MQTT, Home Assistant
- **Matter light switch** → Thread endpoint, works with Apple Home / Google Home
- **BLE beacon** → Proximity detection, asset tracking

## 🔗 Official Resources

| Resource | Link |
|---|---|
| **Datasheet** | [ESP32-H2 Datasheet (PDF)](https://www.espressif.com/sites/default/files/documentation/esp32-h2_datasheet_en.pdf) |
| **Technical Reference** | [ESP32-H2 TRM](https://www.espressif.com/sites/default/files/documentation/esp32-h2_technical_reference_manual_en.pdf) |
| **Arduino Docs** | [Espressif Arduino Documentation](https://docs.espressif.com/projects/arduino-esp32/) |
| **ESP-IDF Guide** | [ESP-IDF for ESP32-H2](https://docs.espressif.com/projects/esp-idf/en/latest/esp32h2/) |

---

<p align="center">Part of <a href="https://github.com/tushar2704/ESP32-Community">ESP32 Community</a> by <a href="https://github.com/tushar2704">Tushar Aggarwal</a></p>
