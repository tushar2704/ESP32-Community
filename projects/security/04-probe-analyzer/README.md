# 📡 802.11 Wi-Fi Probe Request Analyzer & Privacy Monitor

> **Promiscuous mode 802.11 packet sniffer for ESP32. Captures mobile Wi-Fi probe requests, audits persistent vs randomized MAC addresses, extracts targeted SSIDs, and hosts a real-time web privacy dashboard.**

---

## 📋 Features

- 🔍 **Promiscuous Packet Sniffing**: Inspects raw 802.11 Management Probe Request frames (`Subtype 0x04`) across 2.4 GHz channels 1 through 13.
- 🛡️ **Privacy & MAC Address Auditing**: Identifies whether transmitting smartphones are using randomized MACs (locally administered bit) or persistent hardware OUIs.
- 📶 **SSID Discovery**: Extracts targeted network names that nearby devices are actively probing for (e.g., home or office networks).
- 🌐 **Embedded Web Dashboard**: Self-hosted SoftAP interface showing live captured probe streams, signal RSSI proximity, and statistics.
- ⚡ **Zero External Hardware Needed**: Runs standalone on any standard ESP32 development board.

---

> [!WARNING]
> **Educational and Authorized Testing Only**  
> This project is designed strictly for educational research, IoT security auditing, and personal wireless privacy awareness. Never monitor networks without proper authorization.

---

## 🛠️ Bill of Materials (BOM)

| Component | Quantity | Description |
| :--- | :---: | :--- |
| **ESP32 DevKit** | 1 | Classic ESP32, ESP32-S3, or ESP32-C3 |
| **Micro-USB / USB-C Cable** | 1 | Power & Serial data |

---

## 🚀 Quick Start

### 1. PlatformIO
```bash
cd projects/security/04-probe-analyzer
pio run --target upload
pio device monitor -b 115200
```

### 2. View Web Dashboard
1. Connect your phone or laptop to the Wi-Fi AP: `ESP32-Probe-Analyzer` (Password: `12345678`).
2. Navigate to `http://192.168.4.1`.
3. Watch probe requests stream in real time as nearby devices search for networks!

---

<div align="center">

**Part of the [ESP32 Community Hub](https://github.com/tushar2704/ESP32-Community)**  
<sub>Maintained by Tushar Aggarwal ([@tushar2704](https://github.com/tushar2704)) • Videos on [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://youtube.com/@Makerrdesigns)</sub>

</div>
