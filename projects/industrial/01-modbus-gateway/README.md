# ⚡ Industrial Modbus RTU Gateway (RS485 to Web & MQTT)

> **Industrial-grade RS485 Modbus RTU master for ESP32. Polls PLCs, industrial energy meters, and climate sensors with hardware CRC-16 calculation, real-time web telemetry dashboard, and MQTT output.**

---

## 📋 Features

- 🏭 **Industrial RS485 Interface**: High-reliability half-duplex UART driver with automatic direction control (`DE`/`RE` pins).
- 🔍 **Hardware CRC-16 Calculation**: Strict error detection conforming to standard Modbus RTU specifications.
- 🌐 **Responsive Web Dashboard**: Real-time browser gauges for temperature, humidity, 3-phase AC voltage, current, and active wattage.
- 📡 **MQTT Telemetry Integration**: Publishes JSON sensor payloads to external brokers (Home Assistant, Node-RED, ThingsBoard).
- 🔄 **Broad SoC Compatibility**: Pre-configured for classic ESP32, ESP32-S3, and ESP32-C3.

---

## 🛠️ Bill of Materials (BOM)

| Component | Quantity | Description |
| :--- | :---: | :--- |
| **ESP32 DevKit** | 1 | Classic ESP32, ESP32-S3, or ESP32-C3 |
| **MAX485 Module** | 1 | 5V/3.3V TTL-to-RS485 transceiver board |
| **Termination Resistor**| 1 | 120Ω resistor across A and B terminals |
| **Modbus Slave Device** | 1 | RS485 Energy meter, PLC, or BME280 Modbus sensor |
| **Jumper Wires** | 6 | Male-to-Female / Male-to-Male |

---

## 🔌 Wiring Pinout

| ESP32 Pin | MAX485 Pin | Function |
| :---: | :---: | :--- |
| **GPIO 16** | **RO** (Receiver Out) | Hardware Serial2 RX |
| **GPIO 17** | **DI** (Driver In) | Hardware Serial2 TX |
| **GPIO 4** | **DE & RE** (Tied) | Direction Enable (HIGH = TX, LOW = RX) |
| **5V / 3.3V** | **VCC** | Power Supply |
| **GND** | **GND** | Ground reference |
| — | **A / B** | Differential RS485 twisted pair with 120Ω terminator |

---

## 🚀 Quick Start

### 1. PlatformIO
```bash
cd projects/industrial/01-modbus-gateway
pio run --target upload
pio device monitor -b 115200
```

### 2. Connect to Web Dashboard
1. Connect to Wi-Fi network `ESP32-Modbus-Gateway` (Password: `12345678`).
2. Open your browser and navigate to `http://192.168.4.1` (or local station IP).
3. Observe live Modbus register telemetry and health statistics.

---

<div align="center">

**Part of the [ESP32 Community Hub](https://github.com/tushar2704/ESP32-Community)**  
<sub>Maintained by Tushar Aggarwal ([@tushar2704](https://github.com/tushar2704)) • Videos on [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://youtube.com/@Makerrdesigns)</sub>

</div>
