# Project 02: High-Speed PID Line Follower Robot

An autonomous racing robot powered by an ESP32 and a 5-sensor infrared array with real-time Proportional-Integral-Derivative (PID) steering.

---

## Overview

Unlike basic line followers that oscillate abruptly by turning one wheel off and on (Bang-Bang control), this robot calculates a smooth analogue position error across 5 optical sensors and applies differential PWM power to both motors using a PID control loop running at 200 Hz.

| Feature | Specification |
|---|---|
| **Difficulty** | Intermediate |
| **BOM Cost** | ~\$15–\$25 (Chassis + 5x TCRT5000 + Motor Driver + ESP32) |
| **Control Algorithm** | PID closed-loop feedback at 200 Hz |
| **Sensor Array** | 5x TCRT5000 IR reflective optical sensors |
| **Supported Chips** | ESP32 Classic, ESP32-S3 |

---

## Hardware Pinout Reference

### IR Sensor Array to ESP32:
| Sensor | Role | ESP32 Pin |
|---|---|---|
| **Sensor 1** | Far Left | GPIO 34 |
| **Sensor 2** | Left | GPIO 35 |
| **Sensor 3** | Center | GPIO 32 |
| **Sensor 4** | Right | GPIO 33 |
| **Sensor 5** | Far Right | GPIO 25 |

### Motor Driver (L298N / TB6612FNG) to ESP32:
| Driver Pin | ESP32 Pin | Function |
|---|---|---|
| **ENA** | GPIO 14 | Left Motor Speed (PWM) |
| **IN1** | GPIO 27 | Left Motor Forward |
| **IN2** | GPIO 26 | Left Motor Reverse |
| **IN3** | GPIO 13 | Right Motor Forward |
| **IN4** | GPIO 15 | Right Motor Reverse |
| **ENB** | GPIO 12 | Right Motor Speed (PWM) |

---

## Tuning the PID Loop

PID parameters are configured at the top of `line-follower.ino`:
```cpp
float Kp = 25.0;  // Proportional gain: adjusts how strongly the car turns toward the line
float Ki = 0.0;   // Integral gain: corrects steady-state drift (usually kept at 0 for line followers)
float Kd = 15.0;  // Derivative gain: dampens oscillations and prevents overshoot
```

### Tuning Procedure:
1. Set `Ki = 0` and `Kd = 0`. Increase `Kp` until the robot tracks gentle turns but starts to oscillate on straights.
2. Increase `Kd` to dampen oscillations until the robot tracks smoothly through sharp bends.
3. If the robot drifts off on banked curves, introduce small `Ki` (e.g. `0.05`).

---

## Building and Flashing

### Using PlatformIO CLI
```bash
pio run -d projects/robotics/02-line-follower -e esp32dev -t upload
pio device monitor -d projects/robotics/02-line-follower
```

### Using Arduino IDE
1. Open `projects/robotics/02-line-follower/line-follower.ino`.
2. Select **ESP32 Dev Module** under **Tools > Board**.
3. Upload and open Serial Monitor at **115200 baud**.

---

## Community & Credits

- Maintained by [ESP32 Community](https://github.com/tushar2704/ESP32-Community)
- Video guides & electronics builds: [@IamNotIntoTech](https://www.youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://www.youtube.com/@Makerrdesigns)
