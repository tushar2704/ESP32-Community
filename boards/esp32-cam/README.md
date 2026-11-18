# ESP32-CAM — Board Reference Card

> The iconic camera module — ESP32-S + OV2640 + microSD + flash LED on a single board. The go-to for wireless camera projects.

## 📋 Module Specifications

| Feature | Details |
|---|---|
| **Base Chip** | ESP32-S (Xtensa LX6 dual-core) |
| **Clock Speed** | 240 MHz |
| **WiFi** | 802.11 b/g/n (2.4 GHz) |
| **Bluetooth** | Classic BT 4.2 + BLE |
| **Camera** | OV2640 (2MP, 1600×1200) |
| **Camera Interface** | DVP 8-bit |
| **microSD** | ✅ 1-bit / 4-bit SD mode |
| **Flash LED** | ✅ Super-bright white LED (GPIO 4) |
| **PSRAM** | 4 MB (SPI, required for camera) |
| **Flash** | 4 MB |
| **GPIO** | ~10 usable (many occupied by camera/SD) |
| **Antenna** | PCB or U.FL (solder pad selection) |
| **USB-to-Serial** | ❌ Not included on module |
| **Manufacturer** | AI-Thinker |

## 📦 Module & Board Variants

| Variant | Features | Notes |
|---|---|---|
| **ESP32-CAM (AI-Thinker)** | Base module, no USB | Needs external FTDI/CH340 |
| **ESP32-CAM-MB** | ESP32-CAM + USB base board | Plug-and-play with micro USB |
| **ESP32-S3-CAM** | ESP32-S3 based, USB-C | Newer, recommended for new projects |

## 📌 ESP32-CAM Pinout

```
                    ┌─────────────┐
                    │  ┌───────┐  │
                    │  │OV2640 │  │
                    │  │Camera │  │
                    │  └───────┘  │
                    │   [Flash]   │
                    │             │
         GPIO  3 ──┤ U0R    GND ├── GND
         GPIO  1 ──┤ U0T    VCC ├── 5V
         GPIO 12 ──┤ HS2   GPIO│── GPIO 16
         GPIO 13 ──┤ HS1   IO0 ├── GPIO  0 (BOOT)
         GPIO 15 ──┤ CMD   GND ├── GND
         GPIO 14 ──┤ CLK   3V3 ├── 3V3
         GPIO  2 ──┤ SD2   IO4 ├── GPIO  4 (Flash LED)
         GPIO  4 ──┤ SD3   IO2 ├── GPIO  2
                    └─────────────┘
```

**Camera Pin Mapping (DO NOT CHANGE):**

| Signal | GPIO | Signal | GPIO |
|---|---|---|---|
| **D0** | 5 | **D4** | 35 |
| **D1** | 18 | **D5** | 34 |
| **D2** | 19 | **D6** | 39 |
| **D3** | 21 | **D7** | 36 |
| **XCLK** | 0 | **PCLK** | 22 |
| **HREF** | 23 | **VSYNC** | 25 |
| **SIOD (SDA)** | 26 | **SIOC (SCL)** | 27 |
| **PWDN** | 32 | **RESET** | -1 |

> [!WARNING]
> **No Onboard USB-to-Serial!**
> The base ESP32-CAM module has **no USB port**. To program it:
> 1. Use an external FTDI adapter or CH340 programmer
> 2. Connect: FTDI TX → U0R, FTDI RX → U0T, FTDI GND → GND, FTDI 5V → 5V
> 3. **Connect GPIO 0 to GND** to enter flash/download mode
> 4. Press RESET, upload, then disconnect GPIO 0 and reset again
>
> Or use the **ESP32-CAM-MB** base board for USB plug-and-play.

> [!WARNING]
> **GPIO 4 — Flash LED Conflict:**
> GPIO 4 controls the bright flash LED **AND** is shared with the microSD card (SD_DATA1 / HS2_DATA1 in 4-bit mode). When using 4-bit SD mode, the flash LED will flicker during SD operations. Use **1-bit SD mode** to free GPIO 4 for the flash LED.

> [!WARNING]
> **Limited Free GPIOs:**
> Most GPIOs are used by the camera and SD card. Available GPIOs for general use:
> - **GPIO 2** — Onboard LED (active HIGH) + SD card data
> - **GPIO 4** — Flash LED + SD card data (see above)
> - **GPIO 12** — Strapping pin (keep LOW at boot!)
> - **GPIO 13** — Free to use
> - **GPIO 14** — Free to use (SD CLK in SD mode)
> - **GPIO 15** — Free to use (SD CMD in SD mode)
> - **GPIO 33** — Onboard red LED (active LOW)

> [!WARNING]
> **Antenna Selection:**
> The ESP32-CAM has a solder pad to select between PCB antenna and U.FL connector. **Default is PCB antenna.** To use an external antenna, move the 0-ohm resistor to the U.FL position. Incorrect selection = no WiFi!

## 📸 Camera Configuration

```cpp
camera_config_t config;
config.ledc_channel = LEDC_CHANNEL_0;
config.ledc_timer   = LEDC_TIMER_0;
config.pin_d0       = 5;
config.pin_d1       = 18;
config.pin_d2       = 19;
config.pin_d3       = 21;
config.pin_d4       = 35;
config.pin_d5       = 34;
config.pin_d6       = 39;
config.pin_d7       = 36;
config.pin_xclk     = 0;
config.pin_pclk     = 22;
config.pin_vsync    = 25;
config.pin_href     = 23;
config.pin_sccb_sda = 26;
config.pin_sccb_scl = 27;
config.pin_pwdn     = 32;
config.pin_reset    = -1;
config.xclk_freq_hz = 20000000;
config.pixel_format = PIXFORMAT_JPEG;
config.frame_size   = FRAMESIZE_VGA;    // 640x480
config.jpeg_quality = 10;               // 0-63, lower = better
config.fb_count     = 2;                // Double buffer (PSRAM required)
```

> [!IMPORTANT]
> **PSRAM is required** for camera resolutions above SVGA (800×600). The ESP32-CAM includes 4 MB PSRAM. Enable it in Arduino: `Tools → PSRAM → Enabled`.

## 📚 Recommended Libraries

| Library | Purpose | Install |
|---|---|---|
| **esp_camera** | Camera driver | Built-in (ESP32 board package) |
| **WiFi.h** | WiFi connectivity | Built-in |
| **SD_MMC.h** | microSD card (MMC mode) | Built-in |
| **ESPAsyncWebServer** | Async video streaming server | PlatformIO/manual |
| **ESP32-CAM-Video-Recorder** | Record video to SD | GitHub |
| **EEPROM.h** | Store settings | Built-in |

## ⚡ Quick Start — Camera Web Server

```cpp
// ESP32-CAM — WiFi Camera Stream
// Board: AI Thinker ESP32-CAM
#include "esp_camera.h"
#include <WiFi.h>

const char* ssid     = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Camera pin definitions (AI-Thinker ESP32-CAM)
#define PWDN_GPIO  32
#define RESET_GPIO -1
#define XCLK_GPIO   0
#define SIOD_GPIO  26
#define SIOC_GPIO  27
#define Y9_GPIO    36
#define Y8_GPIO    39
#define Y7_GPIO    34
#define Y6_GPIO    35
#define Y5_GPIO    21
#define Y4_GPIO    19
#define Y3_GPIO    18
#define Y2_GPIO     5
#define VSYNC_GPIO 25
#define HREF_GPIO  23
#define PCLK_GPIO  22

void startCameraServer();  // Defined in CameraWebServer example

void setup() {
  Serial.begin(115200);

  camera_config_t config;
  config.pin_pwdn  = PWDN_GPIO;
  config.pin_reset = RESET_GPIO;
  config.pin_xclk  = XCLK_GPIO;
  config.pin_sccb_sda = SIOD_GPIO;
  config.pin_sccb_scl = SIOC_GPIO;
  config.pin_d7 = Y9_GPIO;
  config.pin_d6 = Y8_GPIO;
  config.pin_d5 = Y7_GPIO;
  config.pin_d4 = Y6_GPIO;
  config.pin_d3 = Y5_GPIO;
  config.pin_d2 = Y4_GPIO;
  config.pin_d1 = Y3_GPIO;
  config.pin_d0 = Y2_GPIO;
  config.pin_vsync = VSYNC_GPIO;
  config.pin_href  = HREF_GPIO;
  config.pin_pclk  = PCLK_GPIO;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size   = FRAMESIZE_VGA;
  config.jpeg_quality = 12;
  config.fb_count     = 2;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed: 0x%x\n", err);
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }

  startCameraServer();
  Serial.printf("Camera ready! http://%s\n", WiFi.localIP().toString().c_str());
}

void loop() { delay(10000); }
```

> [!TIP]
> Use the built-in **CameraWebServer** example from Arduino (`File → Examples → ESP32 → Camera → CameraWebServer`). Select "AI Thinker ESP32-CAM" as the board model and enable PSRAM.

## 🔗 Official Resources

| Resource | Link |
|---|---|
| **ESP32 Datasheet** | [ESP32 Datasheet (PDF)](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf) |
| **OV2640 Datasheet** | [OV2640 (PDF)](https://www.uctronics.com/download/cam_module/OV2640DS.pdf) |
| **Community Camera Project** | [Security Camera](../../projects/esp32-cam/01-security-camera/) |

---

<p align="center">Part of <a href="https://github.com/tushar2704/ESP32-Community">ESP32 Community</a> by <a href="https://github.com/tushar2704">Tushar Aggarwal</a></p>
