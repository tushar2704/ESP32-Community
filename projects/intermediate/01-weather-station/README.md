# 🟡 Project INT-01: Wi-Fi Weather Station & Forecast Dashboard

> Real-time weather station querying the free Open-Meteo REST API (zero API keys needed!). Displays temperature (°C/°F), relative humidity %, barometric pressure, wind speed, weather condition codes, and current atomic time via NTP.

---

## 📋 Overview
- **Category**: Intermediate (⭐⭐⭐ 3/5)
- **Supported Boards**: Classic ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6
- **External Components Required**: Optional I2C SSD1306 OLED (128x64) or SPI TFT display (Serial output works without any display!)
- **Primary Goals**:
  1. Securely connect to local Wi-Fi and fetch JSON weather payloads via HTTPS.
  2. Parse nested JSON fields using `ArduinoJson` v6/v7 with zero memory fragmentation.
  3. Map WMO weather interpretation codes to human-readable weather descriptions (Clear, Overcast, Thunderstorm, Rain, Snow).

---

## 🔧 Hardware & Wiring (Optional OLED)

```text
ESP32 3V3  ────────► SSD1306 VCC
ESP32 GND  ────────► SSD1306 GND
ESP32 21   ────────► SSD1306 SDA
ESP32 22   ────────► SSD1306 SCL
```

---

## 📺 Video Tutorial & Enclosures
- 🎬 **Watch the Tutorial**: [@IamNotIntoTech on YouTube](https://youtube.com/@IamNotIntoTech)
- 🖨️ **3D Desktop Weather Bezel**: [@Makerrdesigns on YouTube](https://youtube.com/@Makerrdesigns)

---

## 📜 License
Licensed under GNU AGPLv3. See [LICENSE](../../../LICENSE) for details.
