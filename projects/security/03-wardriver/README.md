# Project 03: Wi-Fi Wardriver & Geospatial Scanner

Conduct continuous 2.4GHz wireless signal surveys and export data in standard WiGLE v1.4 format for RF coverage mapping and wardriving databases.

---

## Overview

Wardriving is the act of searching for and mapping Wi-Fi wireless networks. This project turns the ESP32 into an ultra-portable RF survey tool that sweeps channels 1 to 13, extracts both visible and hidden SSIDs, and formats the telemetry into standard CSV lines compatible with [WiGLE.net](https://wigle.net).

| Feature | Specification |
|---|---|
| **Difficulty** | Intermediate |
| **BOM Cost** | \$4 (ESP32) or ~\$12 (with optional NEO-6M GPS module) |
| **Data Format** | WiGLE CSV 1.4 (`WigleWifi-1.4`) |
| **Captured Parameters** | MAC (BSSID), SSID, Auth Mode, Channel, RSSI, Geo-coordinates |
| **Supported Chips** | ESP32 Classic, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6 |

---

## WiGLE Output Format

The output follows the standard WiGLE header structure:
```csv
WigleWifi-1.4,appRelease=1.0,model=ESP32,release=1.0,device=ESP32-Wardriver,display=none,board=ESP32,brand=ESP32-Community
MAC,SSID,AuthMode,FirstSeen,Channel,RSSI,CurrentLatitude,CurrentLongitude,AltitudeMeters,AccuracyMeters,Type
24:0A:C4:12:34:56,HomeNetwork,[WPA2-PSK-CCMP][ESS],2026-01-01 00:01:23,6,-54,0.000000,0.000000,0.0,10.0,WIFI
```

---

## Adding Real GPS Tracking (Optional)

To attach a **NEO-6M GPS module**:
1. Connect NEO-6M **TX** to ESP32 **GPIO 16 (RX2)**.
2. Connect NEO-6M **RX** to ESP32 **GPIO 17 (TX2)**.
3. Power the module from the ESP32 3.3V or 5V pin.
4. Read NMEA sentences using the `TinyGPSPlus` library.

---

## Building and Flashing

### Using PlatformIO CLI
```bash
pio run -d projects/security/03-wardriver -e esp32dev -t upload
pio device monitor -d projects/security/03-wardriver
```

### Using Arduino IDE
1. Open `projects/security/03-wardriver/wardriver.ino`.
2. Select **ESP32 Dev Module** under **Tools > Board**.
3. Upload and open Serial Monitor at **115200 baud**.

---

## Community & Credits

- Maintained by [ESP32 Community](https://github.com/tushar2704/ESP32-Community)
- Video guides & electronics builds: [@IamNotIntoTech](https://www.youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://www.youtube.com/@Makerrdesigns)
