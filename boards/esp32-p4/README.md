# ESP32-P4 — Board Reference Card

> High-performance HMI processor — dual-core RISC-V at 400 MHz with MIPI-DSI, MIPI-CSI, and hardware video acceleration. **No wireless — requires a companion chip.**

## 📋 Chip Specifications

| Feature | Details |
|---|---|
| **Architecture** | RISC-V (32-bit) |
| **Cores** | 2 (dual-core HP) + 1 LP core |
| **Clock Speed** | 400 MHz (HP) / 40 MHz (LP) |
| **WiFi** | ❌ None — use companion (C6, H2, etc.) |
| **Bluetooth** | ❌ None |
| **Thread/Zigbee** | ❌ None |
| **USB** | ✅ USB 2.0 OTG (High-Speed 480 Mbps) |
| **Display** | ✅ MIPI-DSI (2 lanes, 1080p) |
| **Camera** | ✅ MIPI-CSI (2 lanes) + DVP |
| **GPU/HW Accel** | ✅ 2D graphics, JPEG codec, H.264 encoder |
| **GPIO** | 55 |
| **ADC** | 8 channels (12-bit) |
| **I2C** | 2× |
| **SPI** | 3× |
| **I2S** | 3× |
| **UART** | 5× |
| **SDIO** | 2× (SD card host) |
| **Ethernet** | ✅ EMAC (RMII) |
| **Flash** | External (Octal SPI/PSRAM) |
| **PSRAM** | Up to 32 MB (Octal) |
| **Operating Temp** | -40°C to 85°C |

## 📦 Module Variants

| Module | Flash | PSRAM | Notes |
|---|---|---|---|
| **ESP32-P4** | External | Up to 32 MB | Chip only (no integrated module yet) |

> [!NOTE]
> As of 2024/2025, ESP32-P4 is relatively new. Espressif provides evaluation boards; integrated modules may follow. Check Espressif's product page for the latest module availability.

## 🛒 Recommended Development Boards

| Board | Features | Link |
|---|---|---|
| **ESP32-P4-Function-EV-Board** | MIPI-DSI display + MIPI-CSI camera + USB + Ethernet | [Espressif](https://www.espressif.com/) |

## 📌 Key Interfaces Diagram

```
┌─────────────────────────────────────────────┐
│                 ESP32-P4                     │
│            (RISC-V Dual-Core 400MHz)         │
│                                              │
│  ┌──────────┐  ┌──────────┐  ┌───────────┐  │
│  │ MIPI-DSI │  │ MIPI-CSI │  │  USB 2.0  │  │
│  │ Display  │  │  Camera  │  │  OTG HS   │  │
│  └────┬─────┘  └────┬─────┘  └─────┬─────┘  │
│       │              │              │         │
│  ┌────┴─────┐  ┌────┴─────┐  ┌─────┴─────┐  │
│  │ 1080p    │  │ OV5640/  │  │ 480 Mbps  │  │
│  │ LCD/OLED │  │ IMX219   │  │ Host/Dev  │  │
│  └──────────┘  └──────────┘  └───────────┘  │
│                                              │
│  ┌──────────┐  ┌──────────┐  ┌───────────┐  │
│  │  JPEG/   │  │   SPI    │  │ Companion │  │
│  │  H.264   │  │  SDIO    │  │  ESP32-C6 │  │
│  │  Codec   │  │  Ethernet│  │  (WiFi+BT)│  │
│  └──────────┘  └──────────┘  └───────────┘  │
└─────────────────────────────────────────────┘
```

> [!WARNING]
> **No Wireless Radio!**
> The ESP32-P4 has **no WiFi, Bluetooth, or 802.15.4** radio. For wireless connectivity, pair it with a companion chip:
> - **ESP32-C6** — WiFi 6 + BLE 5.0 + Thread (recommended)
> - **ESP32-H2** — BLE 5.0 + Thread/Zigbee
> - **ESP32-C3** — WiFi + BLE 5.0
>
> Connect the companion chip via SPI or SDIO.

> [!WARNING]
> **Strapping & Reserved Pins:**
> Consult the ESP32-P4 datasheet for strapping pin details — the pin assignments differ significantly from other ESP32 variants due to the high pin count and specialized interfaces.

## 🖥️ What Makes P4 Special

| Feature | Capability |
|---|---|
| **MIPI-DSI** | Drive high-res displays up to 1080p |
| **MIPI-CSI** | Direct camera input (no DVP bottleneck) |
| **H.264 Encoder** | Hardware video encoding for streaming |
| **JPEG Codec** | Hardware JPEG encode/decode |
| **2D Graphics** | Hardware pixel blending, rotation, scaling |
| **USB 2.0 HS** | 480 Mbps — real USB High-Speed |
| **Ethernet** | Built-in EMAC for wired networking |
| **400 MHz** | Fastest ESP32 chip for compute-heavy tasks |

## 📚 Recommended Libraries

| Library | Purpose | Install |
|---|---|---|
| **LVGL** | Graphics UI framework | ESP-IDF component |
| **esp_lcd** | MIPI-DSI / parallel display driver | ESP-IDF component |
| **esp_cam** | MIPI-CSI camera driver | ESP-IDF component |
| **esp_codec** | H.264/JPEG hardware codec | ESP-IDF component |
| **TinyUSB** | USB 2.0 HS device/host | ESP-IDF component |

> [!IMPORTANT]
> The ESP32-P4 is best supported through **ESP-IDF** (v5.3+). Arduino support is limited or experimental for P4-specific peripherals like MIPI-DSI and hardware codecs.

## ⚡ Quick Start — LED Blink (ESP-IDF)

```c
// ESP32-P4 — GPIO LED Blink (ESP-IDF)
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_GPIO 2  // Check your dev board schematic

void app_main(void) {
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    printf("ESP32-P4 — 400MHz HMI Processor Ready!\n");

    while (1) {
        gpio_set_level(LED_GPIO, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(LED_GPIO, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
```

## 🎯 Use Cases

- **Smart displays** — Home automation panels, kiosks, HMI terminals
- **AI edge devices** — Camera + ML inference at 400 MHz
- **Video streaming** — H.264 hardware encode from MIPI-CSI camera
- **Industrial HMI** — Ethernet + display + USB for factory dashboards
- **Digital signage** — 1080p display output with hardware graphics

## 🔗 Official Resources

| Resource | Link |
|---|---|
| **Datasheet** | [ESP32-P4 Datasheet (PDF)](https://www.espressif.com/sites/default/files/documentation/esp32-p4_datasheet_en.pdf) |
| **Technical Reference** | [ESP32-P4 TRM](https://www.espressif.com/sites/default/files/documentation/esp32-p4_technical_reference_manual_en.pdf) |
| **ESP-IDF Guide** | [ESP-IDF for ESP32-P4](https://docs.espressif.com/projects/esp-idf/en/latest/esp32p4/) |

---

<p align="center">Part of <a href="https://github.com/tushar2704/ESP32-Community">ESP32 Community</a> by <a href="https://github.com/tushar2704">Tushar Aggarwal</a></p>
