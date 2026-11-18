# Project 06: Capacitive Touch Buttons

Transform simple jumper wires, copper tape, or metal pads into touch-sensitive capacitive switches without adding extra external components or mechanical switches.

---

## Overview

The ESP32 family (Classic, S2, and S3) features built-in capacitive touch-sensing peripherals. These internal circuits detect changes in capacitance caused by the electrical characteristics of human touch.

| Feature | Specification |
|---|---|
| **Difficulty** | Beginner |
| **BOM Cost** | ~\$4 (ESP32 Board + 2 Jumper Wires) |
| **Build Time** | 5 minutes |
| **Supported Chips** | ESP32 Classic, ESP32-S2, ESP32-S3 |

---

## Hardware Pinout Reference

| Function | ESP32 Classic Pin | ESP32-S2 Pin | ESP32-S3 Pin | Notes |
|---|---|---|---|---|
| **Touch Pad 1 (Toggle)** | GPIO 4 (T0) | GPIO 1 (T1) | GPIO 1 (T1) | Plug jumper wire or copper foil here |
| **Touch Pad 2 (Flash)** | GPIO 14 (T6) | GPIO 2 (T2) | GPIO 2 (T2) | Plug second jumper wire here |
| **Status LED** | GPIO 2 | GPIO 15 | GPIO 38 / RGB | Onboard LED or external LED with 330Ω resistor |

> **Note on RISC-V Chips (C3, C6, H2):** These silicon dies do not include capacitive touch sensing peripherals. Use physical pushbuttons or external capacitive sensor ICs (such as TTP223 or MPR121) when using these chips.

---

## Wiring Diagram

```
+------------------------------------------+
|            ESP32 Classic                 |
|                                          |
|  [GPIO 4]  o-----[ Jumper Wire / Copper Pad 1 ]
|                                          |
|  [GPIO 14] o-----[ Jumper Wire / Copper Pad 2 ]
|                                          |
|  [GPIO 2]  o-----[ Internal LED ]        |
+------------------------------------------+
```

---

## How It Works

1. `touchRead(pin)` charges and discharges the pin's internal capacitor and counts the charge cycles over a fixed period.
2. When a finger approaches or touches the pin, your body's capacitance adds to the circuit, taking longer to charge and causing the raw count to **drop**.
3. Typical readings:
   - **Open air / untouched:** `70` to `90`
   - **Touched by finger:** `15` to `35`
4. The sketch triggers an event when the value falls below `TOUCH_THRESHOLD` (default: `30`).

---

## Building and Flashing

### Using PlatformIO CLI
```bash
pio run -d projects/beginner/06-touch-buttons -e esp32dev -t upload
pio device monitor -d projects/beginner/06-touch-buttons
```

### Using Arduino IDE
1. Open `projects/beginner/06-touch-buttons/touch-buttons.ino`.
2. Under **Tools > Board**, select **ESP32 Dev Module**.
3. Click **Upload** and open Serial Monitor at **115200 baud**.

---

## Community & Credits

- Maintained by [ESP32 Community](https://github.com/tushar2704/ESP32-Community)
- Video guides & electronics builds: [@IamNotIntoTech](https://www.youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://www.youtube.com/@Makerrdesigns)
