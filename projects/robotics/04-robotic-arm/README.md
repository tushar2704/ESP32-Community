# Project 04: 4-DOF Robotic Arm Controller

Control a 4-degree-of-freedom robotic arm (SG90 or MG996R servos) from any web browser with smooth motion easing and pose memory.

---

## Overview

Driving multiple servos directly from an ESP32 often produces violent jerking movements that can strip plastic gear teeth and draw massive current spikes. This project uses `ESP32Servo` coupled with a smooth software interpolation loop, providing butter-smooth robotic motion controlled via a smartphone web dashboard.

| Feature | Specification |
|---|---|
| **Difficulty** | Intermediate |
| **BOM Cost** | ~\$20–\$35 (Acrylic/3D-printed arm kit + 4x SG90/MG90S servos + ESP32) |
| **Degrees of Freedom** | 4 (Base Waist, Shoulder, Elbow, Gripper Claw) |
| **Control UI** | HTML5 Touch Sliders + 3 Stored Pose Presets |
| **Interpolation** | Software easing loop running at 66 Hz |
| **Supported Chips** | ESP32 Classic, ESP32-S3 |

---

## Pinout Reference

| Joint | Servo Type | ESP32 GPIO | Signal Wire Color | Angle Limits |
|---|---|---|---|---|
| **Base** | SG90 / MG90S | GPIO 13 | Orange / White | 0&deg; to 180&deg; |
| **Shoulder** | MG90S / MG996R | GPIO 12 | Orange / White | 15&deg; to 165&deg; |
| **Elbow** | SG90 / MG90S | GPIO 14 | Orange / White | 10&deg; to 170&deg; |
| **Gripper (Claw)** | SG90 | GPIO 27 | Orange / White | 10&deg; (Closed) to 120&deg; (Open) |

> **POWER CAUTION:** Do NOT power 4 servos directly from the ESP32 3.3V or 5V pin! Stalling servos can draw 2A+ and brown out or destroy the ESP32. Always use a dedicated **5V 3A external power supply** for the servo power rails, and connect the external supply's GND to the ESP32 GND!

---

## Operating Instructions

1. Power the external 5V servo supply and the ESP32.
2. Connect to the robot's Wi-Fi network:
   - **SSID:** `ESP32-Robotic-Arm`
   - **Password:** `12345678`
3. In any browser, open:
   - `http://192.168.4.1`
4. Drag individual joint sliders or click pose preset buttons (**Home**, **Pick Up**, **Drop Off**).

---

## Building and Flashing

### Using PlatformIO CLI
```bash
pio run -d projects/robotics/04-robotic-arm -e esp32dev -t upload
pio device monitor -d projects/robotics/04-robotic-arm
```

### Using Arduino IDE
1. Open `projects/robotics/04-robotic-arm/robotic-arm.ino`.
2. Install the **ESP32Servo** library via **Library Manager**.
3. Select **ESP32 Dev Module** under **Tools > Board**.
4. Upload and open Serial Monitor at **115200 baud**.

---

## Community & Credits

- Maintained by [ESP32 Community](https://github.com/tushar2704/ESP32-Community)
- Video guides & electronics builds: [@IamNotIntoTech](https://www.youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://www.youtube.com/@Makerrdesigns)
