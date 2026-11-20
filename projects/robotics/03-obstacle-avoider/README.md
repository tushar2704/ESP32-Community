# 🤖 Autonomous Obstacle Avoidance Rover

> **Autonomous navigation robot chassis powered by ESP32. Utilizes an HC-SR04 ultrasonic sensor mounted on a panning SG90 micro-servo radar to map surroundings and intelligently steer clear of obstacles.**

---

## 📋 Features

- 🦇 **Ultrasonic Distance Mapping**: Sub-centimeter distance measurement via high-speed microsecond pulse timing.
- 🎯 **180° Servo Radar Sweep**: Automatically pans left and right upon encountering obstacles to evaluate optimal escape trajectories.
- 🏎️ **Dual H-Bridge Motor Control**: Bi-directional differential steering with smooth start/stop states.
- 🛡️ **Intelligent Collision Avoidance**: Automatically executes reverse back-offs and 180-degree pivot spins when cornered in dead ends.
- 🌐 **Clean PlatformIO & Arduino Structure**: Universal support for classic ESP32, ESP32-S3, and ESP32-C3 boards.

---

## 🛠️ Bill of Materials (BOM)

| Component | Quantity | Description |
| :--- | :---: | :--- |
| **ESP32 DevKit** | 1 | Microcontroller board |
| **HC-SR04 Sensor** | 1 | Ultrasonic distance sensor |
| **SG90 Servo** | 1 | 9g micro servo motor |
| **L298N / L9110S** | 1 | Dual DC motor driver module |
| **2WD / 4WD Chassis**| 1 | Acrylic robot car chassis with 2x TT gearmotors & wheels |
| **18650 Li-ion Batteries** | 2 | 7.4V battery pack |
| **LM2596 DC-DC Buck** | 1 | Steps 7.4V down to 5.0V for ESP32 and servo |

---

## 🔌 Wiring Pinout

| Component | ESP32 Pin | Function |
| :---: | :---: | :--- |
| **HC-SR04 TRIG** | **GPIO 5** | Trigger pulse output |
| **HC-SR04 ECHO** | **GPIO 18** | Echo pulse input (use voltage divider to 3.3V) |
| **SG90 Servo** | **GPIO 19** | PWM Servo Control |
| **Motor L (IN1)** | **GPIO 26** | Left Motor Forward |
| **Motor L (IN2)** | **GPIO 27** | Left Motor Reverse |
| **Motor R (IN3)** | **GPIO 32** | Right Motor Forward |
| **Motor R (IN4)** | **GPIO 33** | Right Motor Reverse |
| **GND** | **GND** | Common Ground (ESP32, Motor Driver, Buck Converter) |
| **5V Output** | **VIN / 5V** | Regulated 5V from LM2596 buck converter |

---

## 🚀 Quick Start

### 1. PlatformIO
```bash
cd projects/robotics/03-obstacle-avoider
pio run --target upload
pio device monitor -b 115200
```

### 2. Tuning Navigation Sensitivity
In `obstacle-avoider.ino`, you can adjust:
- `DISTANCE_THRESHOLD_CM`: Set minimum clearance distance (default: `25` cm).
- Turn durations (`turnLeft()`, `turnRight()` delays) to match your chassis motor gearing and wheel grip.

---

<div align="center">

**Part of the [ESP32 Community Hub](https://github.com/tushar2704/ESP32-Community)**  
<sub>Maintained by Tushar Aggarwal ([@tushar2704](https://github.com/tushar2704)) • Videos on [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://youtube.com/@Makerrdesigns)</sub>

</div>
