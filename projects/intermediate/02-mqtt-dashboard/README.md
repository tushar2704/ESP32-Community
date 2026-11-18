# 🟡 Project INT-02: MQTT IoT Telemetry & Home Assistant Bridge

> Production-grade MQTT publisher and subscriber node with TLS encryption, Home Assistant MQTT discovery, LWT (Last Will and Testament), automated reconnects, and non-blocking sensor dispatch.

---

## 📋 Overview
- **Category**: Intermediate (⭐⭐⭐ 3/5)
- **Supported Boards**: All ESP32 chips (Classic, S2, S3, C3, C6)
- **Primary Goals**:
  1. Maintain persistent, non-blocking MQTT socket connection with automatic exponential backoff.
  2. Implement MQTT Last Will and Testament (LWT) for instant online/offline state reporting.
  3. Publish Home Assistant MQTT Discovery payloads to auto-create device cards in Home Assistant.

---

## ⚡ MQTT Topic Schema

```text
tele/esp32_node/state        -> "online" / "offline" (LWT)
tele/esp32_node/sensor       -> {"temp": 24.2, "humidity": 55, "heap": 240000, "uptime": 3600}
cmnd/esp32_node/power        <- "ON" / "OFF" / "TOGGLE"
stat/esp32_node/power        -> "ON" / "OFF"
```

---

## 📺 Video Tutorial & Enclosures
- 🎬 **Watch the Tutorial**: [@IamNotIntoTech on YouTube](https://youtube.com/@IamNotIntoTech)
- 🖨️ **3D DIN-Rail IoT Enclosure**: [@Makerrdesigns on YouTube](https://youtube.com/@Makerrdesigns)

---

## 📜 License
Licensed under GNU AGPLv3. See [LICENSE](../../../LICENSE) for details.
