# Project 10: Serial JSON Command API

Turn your ESP32 into a clean JSON-RPC microservice controlled over USB Serial or hardware UART.

---

## Overview

Interfacing microcontrollers with host applications (such as Python scripts, Node.js servers, Electron apps, or ROS nodes) is often done with fragile raw string parsing. This project establishes a robust, extensible JSON API running on the ESP32.

| Feature | Specification |
|---|---|
| **Difficulty** | Beginner |
| **BOM Cost** | \$0 (ESP32 Development Board + Micro-USB / USB-C cable) |
| **Baud Rate** | 115200 bps (newline `\n` delimited) |
| **Parser Library** | ArduinoJson v6.x |
| **Supported Chips** | ESP32 Classic, S2, S3, C3, C6, H2 |

---

## Command Reference

Every command must be sent as a single line ending in a newline (`\n`):

### 1. Heartbeat Ping
**Request:**
```json
{"cmd":"ping"}
```
**Response:**
```json
{"status":"ok","reply":"pong","uptime_ms":4210}
```

### 2. Live Telemetry Status
**Request:**
```json
{"cmd":"status"}
```
**Response:**
```json
{"status":"ok","led":false,"uptime_sec":128,"heap_free_bytes":284120,"heap_min_bytes":279800}
```

### 3. Control LED
**Request:**
```json
{"cmd":"set_led","state":true}
```
**Response:**
```json
{"status":"ok","led":true}
```

### 4. Silicon & System Info
**Request:**
```json
{"cmd":"system_info"}
```
**Response:**
```json
{"status":"ok","chip_model":"ESP32-D0WDQ6","chip_revision":1,"chip_cores":2,"cpu_freq_mhz":240,"flash_size_bytes":4194304,"sdk_version":"v4.4.7"}
```

---

## Python Client Example

A quick script to test the interface from a PC or Raspberry Pi:

```python
import serial
import json
import time

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
time.sleep(2)  # Wait for ESP32 boot

# Send Ping
ser.write(b'{"cmd":"ping"}\n')
response = ser.readline().decode('utf-8')
print("Response:", json.loads(response))

# Turn on LED
ser.write(b'{"cmd":"set_led","state":true}\n')
print("LED Response:", json.loads(ser.readline().decode('utf-8')))
```

---

## Building and Flashing

### Using PlatformIO CLI
```bash
pio run -d projects/beginner/10-serial-json-api -e esp32dev -t upload
pio device monitor -d projects/beginner/10-serial-json-api
```

### Using Arduino IDE
1. Open `projects/beginner/10-serial-json-api/serial-json-api.ino`.
2. Install the **ArduinoJson** library via **Library Manager**.
3. Upload to your ESP32 board and open the Serial Monitor at 115200 baud with "Newline" line ending enabled.

---

## Community & Credits

- Maintained by [ESP32 Community](https://github.com/tushar2704/ESP32-Community)
- Video guides & electronics builds: [@IamNotIntoTech](https://www.youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://www.youtube.com/@Makerrdesigns)
