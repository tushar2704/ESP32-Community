# ESP32-S2 — Board Reference Card

> WiFi-only single-core with native USB OTG — perfect for USB HID devices, keyboards, and CDC gadgets.

## 📋 Chip Specifications

| Feature | Details |
|---|---|
| **Architecture** | Xtensa LX7 |
| **Cores** | 1 (single-core) |
| **Clock Speed** | 240 MHz |
| **WiFi** | 802.11 b/g/n (2.4 GHz) |
| **Bluetooth** | ❌ None |
| **Thread/Zigbee** | ❌ No |
| **USB** | ✅ Native USB OTG (1.1) |
| **GPIO** | 43 |
| **ADC** | 20 channels (13-bit) |
| **DAC** | 2 channels (8-bit) |
| **Touch** | 14 capacitive touch pins |
| **I2C** | 2× |
| **SPI** | 4× (1 used for flash) |
| **I2S** | 1× |
| **UART** | 2× |
| **Flash** | 2–4 MB (external) |
| **PSRAM** | Up to 2 MB (WROVER) |
| **Operating Temp** | -40°C to 85°C |

## 📦 Module Variants

| Module | Flash | PSRAM | Antenna | Notes |
|---|---|---|---|---|
| **ESP32-S2-WROOM** | 4 MB | ❌ | PCB | Standard module |
| **ESP32-S2-WROVER** | 4 MB | 2 MB | PCB | With PSRAM |
| **ESP32-S2-MINI-1** | 4 MB | ❌ | PCB | Compact form factor |
| **ESP32-S2-SOLO** | 4 MB | ❌ | PCB | Single-core variant |

## 🛒 Recommended Development Boards

| Board | USB | Features | Link |
|---|---|---|---|
| **ESP32-S2-DevKitM-1** | Native USB | Reference board | [Espressif](https://www.espressif.com/) |
| **ESP32-S2-Saola-1** | Native USB + UART | Dual USB ports | [Espressif](https://www.espressif.com/) |
| **XIAO ESP32-S2** | Native USB | Ultra-compact, Seeed Studio | [Seeed Studio](https://www.seeedstudio.com/) |
| **Adafruit Feather ESP32-S2** | Native USB | Feather ecosystem, LiPo | [Adafruit](https://www.adafruit.com/) |

## 📌 ESP32-S2-DevKitM-1 Pinout

```
                    ┌──────────┐
              3V3 ──┤ 1     44 ├── GND
         GPIO  0 ──┤ 2     43 ├── GPIO 46
         GPIO  1 ──┤ 3     42 ├── GPIO 45
         GPIO  2 ──┤ 4     41 ├── GPIO 44 (RX)
         GPIO  3 ──┤ 5     40 ├── GPIO 43 (TX)
         GPIO  4 ──┤ 6     39 ├── GPIO 42
         GPIO  5 ──┤ 7     38 ├── GPIO 41
         GPIO  6 ──┤ 8     37 ├── GPIO 40
         GPIO  7 ──┤ 9     36 ├── GPIO 39
         GPIO  8 ──┤10     35 ├── GPIO 38
         GPIO  9 ──┤11     34 ├── GPIO 37
         GPIO 10 ──┤12     33 ├── GPIO 36
         GPIO 11 ──┤13     32 ├── GPIO 35
         GPIO 12 ──┤14     31 ├── GPIO 34
         GPIO 13 ──┤15     30 ├── GPIO 33
         GPIO 14 ──┤16     29 ├── GPIO 26
              GND ──┤17     28 ├── GPIO 21
         GPIO 15 ──┤18     27 ├── GPIO 20 (D-)
         GPIO 16 ──┤19     26 ├── GPIO 19 (D+)
         GPIO 17 ──┤20     25 ├── GPIO 18
         GPIO 18 ──┤21     24 ├── 5V
              RST ──┤22     23 ├── GND
                    └──────────┘
                      USB-C
```

> [!WARNING]
> **Strapping Pins:**
> - **GPIO 0** — Boot mode selection (HIGH = normal, LOW = download)
> - **GPIO 45** — VDD_SPI voltage select (keep LOW for 3.3V flash)
> - **GPIO 46** — Boot mode / ROM log (input-only, no pull-up)

> [!WARNING]
> **Reserved GPIOs:**
> - **GPIO 19/20** — USB D+/D- (used by native USB)
> - **GPIO 26** — Connected to PSRAM on WROVER modules
> - **GPIO 22–25** — Not available (used for internal flash)

## 📚 Recommended Libraries

| Library | Purpose | Install |
|---|---|---|
| **WiFi.h** | WiFi connectivity | Built-in |
| **USB.h** | USB OTG device support | Built-in |
| **USBHIDKeyboard.h** | USB HID keyboard | Built-in |
| **USBHIDMouse.h** | USB HID mouse | Built-in |
| **USBCDC.h** | USB serial (CDC) | Built-in |
| **TinyUSB** | Advanced USB gadgets | Library Manager |

## ⚡ Quick Start — LED Blink

```cpp
// ESP32-S2 — LED Blink via Native USB Serial
// Board: ESP32-S2 Dev Module

#define LED_PIN 15  // Varies by board; check your dev board docs

void setup() {
  Serial.begin(115200);  // USB CDC serial
  pinMode(LED_PIN, OUTPUT);
  Serial.println("ESP32-S2 — LED Blink!");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
}
```

> [!TIP]
> The ESP32-S2's native USB makes it ideal for building custom USB keyboards, mice, game controllers, and MIDI devices without any external USB-to-serial chip.

## 🔗 Official Resources

| Resource | Link |
|---|---|
| **Datasheet** | [ESP32-S2 Datasheet (PDF)](https://www.espressif.com/sites/default/files/documentation/esp32-s2_datasheet_en.pdf) |
| **Technical Reference** | [ESP32-S2 TRM](https://www.espressif.com/sites/default/files/documentation/esp32-s2_technical_reference_manual_en.pdf) |
| **Arduino Core** | [espressif/arduino-esp32](https://github.com/espressif/arduino-esp32) |
| **ESP-IDF Guide** | [ESP-IDF for ESP32-S2](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/) |

---

<p align="center">Part of <a href="https://github.com/tushar2704/ESP32-Community">ESP32 Community</a> by <a href="https://github.com/tushar2704">Tushar Aggarwal</a></p>
