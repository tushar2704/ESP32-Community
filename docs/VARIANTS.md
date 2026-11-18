# 🏷️ ESP32 Board Identification & Variant Guide

> **How to identify exactly which ESP32 board, module, and silicon revision you have sitting on your workbench.**

---

## 🔍 How to Identify Your Board in 4 Steps

### Step 1: Read the Metal Shield Laser Engraving
Every official Espressif module features a laser-etched RF shielding can:
- **`ESP32-WROOM-32` / `ESP32-WROOM-32D` / `ESP32-WROOM-32U`**: Classic ESP32, 4MB/8MB/16MB flash, no PSRAM.
- **`ESP32-WROVER` / `ESP32-WROVER-B` / `ESP32-WROVER-E`**: Classic ESP32 with 4MB or 8MB external SPI PSRAM.
- **`ESP32-S3-WROOM-1` / `ESP32-S3-WROOM-2`**: ESP32-S3 chip, Quad or Octal Flash/PSRAM.
- **`ESP32-C3-WROOM-02`**: Single-core RISC-V C3 module.
- **`ESP32-C6-WROOM-1`**: Wi-Fi 6 + Thread/Zigbee C6 module.

### Step 2: Check Pin Count & Layout
- **30-Pin Narrow Board**: Typical NodeMCU-32S layout (0.9" breadboard pitch).
- **38-Pin Wide Board**: Standard Espressif DevKitC layout (takes up full mini breadboard).
- **Dual Type-C Connectors**: Usually **ESP32-S3-DevKitC-1** (one USB-to-UART bridge port, one native USB OTG port).

### Step 3: Check USB-to-Serial Chip
Inspect the rectangular IC near the USB port:
- **CP2102 / CP2104 (Silicon Labs)**: Small square chip. Very reliable, needs Silicon Labs VCP driver.
- **CH340G / CH340C / CH340N (WCH)**: Very common on budget AliExpress/Amazon boards. Needs WCH CH34x driver.
- **FT232RL (FTDI)**: Rare on modern boards.
- **None (Direct USB connection)**: Board relies on the ESP32-S2 or ESP32-S3 native USB peripheral.

### Step 4: Run Software Auto-Detection
Upload this simple sketch or run `esptool.py flash_id` to query the silicon directly:

```bash
esptool.py flash_id
```

Output will identify:
```text
Detecting chip type... ESP32-S3 (QFN56)
Chip is ESP32-S3 (revision v0.2)
Features: WiFi, BLE, Embedded PSRAM 8MB (Octal)
Crystal is 40MHz
MAC: 7c:df:a1:03:42:10
Manufacturer: c8
Device: 4018
Detected flash size: 16MB
```

---

## 📋 Popular Development Board Profiles

### 1. ESP32 DevKitC v4 (Classic)
- **Module**: ESP32-WROOM-32D / 32E
- **Pins**: 38 pins
- **Buttons**: `EN` (Reset) + `BOOT` (GPIO 0)
- **LEDs**: Red Power LED + Blue user LED on GPIO 2
- **Best For**: General learning, breadboard prototyping

### 2. ESP32-CAM (AI-Thinker)
- **Module**: ESP32-S with OV2640 camera connector
- **Pins**: 16 pins (no onboard USB port unless mounted on ESP32-CAM-MB base)
- **Features**: MicroSD card slot, high-intensity flash LED on GPIO 4
- **Caveats**: GPIO 0 must be shorted to GND while pressing Reset to enter flash mode

### 3. ESP32-S3-DevKitC-1
- **Module**: ESP32-S3-WROOM-1 or WROOM-2
- **Ports**: Dual USB-C (COM port marked `UART`, native port marked `USB`)
- **Buttons**: `RST` + `BOOT`
- **Features**: Addressable RGB LED (WS2812) on GPIO 48
- **Best For**: TinyML, USB HID emulation, LVGL touch displays

### 4. Seeed Studio XIAO ESP32 Series
- **Variants**: XIAO ESP32-C3, XIAO ESP32-S3, XIAO ESP32-S3 Sense (with camera/mic)
- **Form Factor**: Ultra-compact thumb-sized (21 x 17.5 mm), single-sided components
- **Features**: Integrated battery charging circuit (pads on bottom)
- **Best For**: Wearables, micro drones, miniature sensor pods

### 5. Cheap Yellow Display (CYD / Sunton)
- **Variants**: ESP32-2432S028R (2.8" Resistive), ESP32-2432S028C (2.8" Capacitive), 3.5", 4.0", 5.0", 7.0"
- **Features**: Integrated TFT display, touch controller, RGB LED, LDR light sensor, speaker connector, SD slot
- **Best For**: Smart home dashboards, touch terminals, portable retro games

---

<div align="center">

**Part of the [ESP32 Community Hub](https://github.com/tushar2704/ESP32-Community)**  
<sub>Maintained by Tushar Aggarwal ([@tushar2704](https://github.com/tushar2704))</sub>

</div>
