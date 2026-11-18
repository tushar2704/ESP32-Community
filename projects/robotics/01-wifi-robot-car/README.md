# Project 01: Wi-Fi Remote Controlled Robot Car

Build an autonomous or smartphone-controlled dual-motor robotic chassis powered by an ESP32, L298N/TB6612FNG motor driver, and a zero-latency responsive web D-Pad controller.

---

## Overview

The ESP32 broadcasts its own independent Wi-Fi Access Point (`ESP32-Robot-Car`). Any smartphone, tablet, or laptop connects to this network and loads an interactive web touch-pad right in the browser—no third-party apps or internet connection required.

| Feature | Specification |
|---|---|
| **Difficulty** | Intermediate |
| **BOM Cost** | ~\$15–\$25 (Chassis kit + Motor driver + Battery + ESP32) |
| **Control Interface** | Mobile Web App (Touch D-Pad + Speed Slider) |
| **Motor Drive** | 2-WD or 4-WD DC Gearmotors (Yellow TT motors) |
| **Driver Compatibility** | L298N, TB6612FNG, L9110S |
| **Supported Chips** | ESP32 Classic, ESP32-S3 |

---

## Bill of Materials (BOM)

| Item | Qty | Description |
|---|---|---|
| **ESP32 Dev Board** | 1 | 30-pin or 38-pin DevKit |
| **2WD or 4WD Robot Chassis** | 1 | Acrylic or 3D printed base + TT DC motors |
| **Motor Driver** | 1 | L298N Dual H-Bridge module or TB6612FNG |
| **Power Supply** | 1 | 2x 18650 Li-ion cells in series (7.4V) or 3x (11.1V) |
| **Buck Converter (Optional)** | 1 | LM2596 step-down to 5V (or use L298N onboard 5V regulator) |
| **Jumper Wires** | 10 | Male-to-Female & Male-to-Male |

---

## Wiring Diagram

### Motor Driver (L298N) to ESP32:

| L298N Pin | ESP32 Pin | Function |
|---|---|---|
| **ENA** | GPIO 14 | Left Motor Speed (PWM) |
| **IN1** | GPIO 27 | Left Motor Forward |
| **IN2** | GPIO 26 | Left Motor Reverse |
| **IN3** | GPIO 25 | Right Motor Forward |
| **IN4** | GPIO 33 | Right Motor Reverse |
| **ENB** | GPIO 12 | Right Motor Speed (PWM) |
| **GND** | GND | Common Ground (Critical!) |
| **+12V** | Battery (+) | 7.4V - 12V from battery pack |
| **+5V** | ESP32 VIN | Powers ESP32 from L298N regulator |

> **IMPORTANT:** Ensure the battery ground, L298N ground, and ESP32 GND are all tied together!

---

## Operating Instructions

1. Power on the robot car.
2. On your smartphone, scan for Wi-Fi networks and connect to:
   - **SSID:** `ESP32-Robot-Car`
   - **Password:** `12345678`
3. Open your mobile browser (Chrome/Safari) and go to:
   - **URL:** `http://192.168.4.1`
4. Use the on-screen arrows to drive the car. Releasing an arrow automatically halts the motors for safety!

---

## Building and Flashing

### Using PlatformIO CLI
```bash
pio run -d projects/robotics/01-wifi-robot-car -e esp32dev -t upload
pio device monitor -d projects/robotics/01-wifi-robot-car
```

### Using Arduino IDE
1. Open `projects/robotics/01-wifi-robot-car/wifi-robot-car.ino`.
2. Select **ESP32 Dev Module** under **Tools > Board**.
3. Upload and open Serial Monitor at **115200 baud**.

---

## Community & Credits

- Maintained by [ESP32 Community](https://github.com/tushar2704/ESP32-Community)
- Video guides & electronics builds: [@IamNotIntoTech](https://www.youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://www.youtube.com/@Makerrdesigns)
