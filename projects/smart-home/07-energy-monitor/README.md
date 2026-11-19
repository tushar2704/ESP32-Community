# ⚡ Smart AC Power & Energy Monitor

[![PlatformIO CI](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml/badge.svg)](https://github.com/tushar2704/ESP32-Community/actions/workflows/build.yml)
[![License: AGPL-3.0](https://img.shields.io/badge/License-AGPL--3.0-blue.svg)](LICENSE)
[![Hardware: ESP32](https://img.shields.io/badge/Hardware-ESP32%20%7C%20S3%20%7C%20C3-red.svg)](hardware/)

A non-invasive AC current and energy consumption monitor powered by the ESP32 and an SCT-013 current transformer clamp. Measures true RMS current, active power consumption in Watts, cumulative electrical energy usage in kilowatt-hours (kWh), and real-time cost calculation, with an embedded responsive web dashboard and REST API.

---

## 📸 Key Features

- **Non-Invasive Sensing**: Clips around a single live/hot AC wire using an SCT-013 current transformer (no dangerous electrical rewiring or stripping needed).
- **True RMS Sampling**: Samples the AC waveform at ~12 kHz to calculate accurate RMS current for linear and non-linear inductive loads (refrigerators, PC power supplies, motors).
- **Live Glassmorphism Web Dashboard**: Real-time Watts display, RMS Amperage, cumulative kWh tracker, and estimated billing cost.
- **REST API Endpoints**: Easily poll `/api/power` from Home Assistant, Grafana, or Node-RED.
- **WiFi Fallback AP**: Launches `ESP32-EnergyMonitor` hotspot if local router is unreachable.

---

## 🛠️ Hardware Requirements

| Component | Quantity | Notes |
|:---|:---:|:---|
| **ESP32 Dev Module** | 1 | ESP32-WROOM-32, ESP32-S3, or ESP32-C3 |
| **SCT-013-030 (or SCT-013-000)** | 1 | 30A/1V or 100A/50mA split-core current transformer clamp |
| **Resistors (10kΩ)** | 2 | DC bias voltage divider ($3.3V / 2 = 1.65V$) |
| **Capacitor (10µF)** | 1 | Decoupling / smoothing capacitor for DC bias midpoint |
| **Burden Resistor (optional)** | 1 | Required only if using current-output version (e.g. 18Ω–33Ω for SCT-013-000) |
| **Status LED** | 1 | Built-in or external indicator |

---

## 🔌 Circuit Schematic & Pin Connections

Connect the CT sensor bias circuit to ADC1:

```text
3.3V ───[ 10kΩ ]───┬───[ SCT-013 CT Clamp ]─── ESP32 ADC (GPIO 34)
                   │             │
                 [10µF]       [18Ω burden (if 100A clamp)]
                   │             │
GND  ───[ 10kΩ ]───┴─────────────┴──────────── GND
```

| Signal | ESP32-WROOM-32 | ESP32-S3 | ESP32-C3 | Description |
|:---|:---:|:---:|:---:|:---|
| **CT Analog In** | `GPIO 34` | `GPIO 4` | `GPIO 2` | ADC1 channel with low noise |
| **Status LED** | `GPIO 2` | `GPIO 2` | `GPIO 8` | Blinks during sampling cycles |

---

## 🚀 Quick Start

1. Open this directory in PlatformIO:
   ```bash
   cd projects/smart-home/07-energy-monitor
   ```
2. Configure your WiFi credentials and local mains voltage (e.g. 120V US or 230V EU):
   ```cpp
   const float MAINS_VOLTAGE = 230.0;
   const float COST_PER_KWH = 0.15;
   ```
3. Compile and flash:
   ```bash
   pio run -t upload -e esp32dev
   ```
4. Clip the SCT-013 clamp around **only one wire** (the Live wire) of the appliance cord (clipping around both Live + Neutral cancels the magnetic field and reads 0A).
5. Open your browser and access the live power meter at the assigned IP address.

---

## 🌐 REST API Reference

### 1. Read Power & Energy
- **Method**: `GET /api/power`
- **Response**:
```json
{
  "currentRMS": 3.42,
  "powerWatts": 786.6,
  "mainsVoltage": 230.0,
  "energyKWh": 0.4821,
  "estimatedCost": 0.072,
  "uptime": 240050
}
```

### 2. Reset Energy Accumulator
- **Method**: `POST /api/reset`

---

## 📺 Community & Credits

- Maintained by [Tushar Aggarwal](https://github.com/tushar2704)
- Tutorials & Video Guides: [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://youtube.com/@Makerrdesigns)
- Licensed under [AGPL-3.0](LICENSE).
