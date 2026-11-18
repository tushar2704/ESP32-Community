# Project 01: Wi-Fi Deauthentication & Disassociation Detector

Monitor the 2.4GHz spectrum for malicious Wi-Fi deauth floods using the ESP32's native 802.11 promiscuous packet sniffer.

---

## Overview

Wi-Fi deauthentication attacks exploit an architectural weakness in unencrypted 802.11 management frames (pre-WPA3 Protected Management Frames). Attackers spoof the MAC address of an Access Point to forcibly disconnect connected devices (phones, IoT sensors, cameras) from their network.

This defensive monitor passively listens to 2.4GHz channels, extracts raw 802.11 management frames, and triggers immediate visual and serial alarms when deauth or disassociation frames are detected.

| Feature | Specification |
|---|---|
| **Difficulty** | Intermediate |
| **BOM Cost** | \$4 (ESP32 Dev Board) |
| **Monitored Frames** | Type 0x00 Subtype 0x0C (Deauth) & Subtype 0x0A (Disassoc) |
| **Channels** | Automatic channel hopping across 1–13 (300ms dwell) |
| **Telemetry** | Attacker MAC, Target MAC, BSSID, RSSI, and Channel |
| **Supported Chips** | ESP32 Classic, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6 |

---

## Technical Details

The ESP32 radio is configured into promiscuous mode (`esp_wifi_set_promiscuous(true)`), filtering exclusively for 802.11 Management frames (`WIFI_PROMIS_FILTER_MASK_MGMT`).

When a frame is intercepted:
1. Byte 0 is checked: `0xC0` indicates Deauthentication, `0xA0` indicates Disassociation.
2. Bytes 4–9 contain the Destination/Target MAC address.
3. Bytes 10–15 contain the Transmitter/Attacker MAC address.
4. Bytes 16–21 contain the Access Point BSSID.
5. The received signal strength (`rssi`) is logged to assist in proximity triangulation.

---

## Building and Flashing

### Using PlatformIO CLI
```bash
pio run -d projects/security/01-wifi-deauth-detector -e esp32dev -t upload
pio device monitor -d projects/security/01-wifi-deauth-detector
```

### Using Arduino IDE
1. Open `projects/security/01-wifi-deauth-detector/wifi-deauth-detector.ino`.
2. Select **ESP32 Dev Module** under **Tools > Board**.
3. Upload and open Serial Monitor at **115200 baud**.

---

## Community & Credits

- Maintained by [ESP32 Community](https://github.com/tushar2704/ESP32-Community)
- Video guides & electronics builds: [@IamNotIntoTech](https://www.youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://www.youtube.com/@Makerrdesigns)
