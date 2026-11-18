# 🐍 MicroPython Setup for ESP32

> **Write Python on microcontrollers — rapid prototyping, interactive REPL, and beginner-friendly development for ESP32.**

[![MicroPython](https://img.shields.io/badge/MicroPython-v1.23+-2B2728?style=for-the-badge&logo=micropython&logoColor=white)](https://micropython.org/)
[![Python](https://img.shields.io/badge/Python-3.x-3776AB?style=for-the-badge&logo=python&logoColor=white)](https://python.org/)
[![ESP32](https://img.shields.io/badge/ESP32-Supported-E7352C?style=for-the-badge&logo=espressif&logoColor=white)](https://www.espressif.com/)

---

## 📑 Table of Contents

- [What is MicroPython?](#-what-is-micropython)
- [Prerequisites](#-prerequisites)
- [Step 1 — Download Firmware](#-step-1--download-firmware)
- [Step 2 — Install esptool.py](#-step-2--install-esptoolpy)
- [Step 3 — Flash the Firmware](#-step-3--flash-the-firmware)
- [Step 4 — Connect with Thonny](#-step-4--connect-with-thonny)
- [Step 5 — Connect with mpremote](#-step-5--connect-with-mpremote)
- [Step 6 — Your First Script](#-step-6--your-first-script)
- [Step 7 — File Management](#-step-7--file-management)
- [Step 8 — Installing Packages with mip](#-step-8--installing-packages-with-mip)
- [MicroPython vs Arduino vs ESP-IDF](#-micropython-vs-arduino-vs-esp-idf)
- [Essential MicroPython Modules](#-essential-micropython-modules)
- [Troubleshooting](#-troubleshooting)
- [Additional Resources](#-additional-resources)

---

## 🔍 What is MicroPython?

MicroPython is a **lean implementation of Python 3** optimized for microcontrollers. It runs on bare metal and provides a Python REPL (interactive prompt) directly on the ESP32.

| Feature | Description |
|---|---|
| **Interactive REPL** | Type Python code and see results instantly |
| **No compilation** | Write → save → run (no build step) |
| **File system** | Store `.py` files directly on the ESP32 |
| **Hardware access** | GPIO, I2C, SPI, PWM, ADC via Python |
| **WiFi & Bluetooth** | Full networking with Python sockets |
| **Package manager** | Install third-party libraries with `mip` |

> [!NOTE]
> MicroPython trades raw performance for development speed. It's about **5–20x slower** than compiled C, but you can prototype in minutes instead of hours. For performance-critical sections, you can use inline assembly or native code emitters.

---

## 📋 Prerequisites

| Requirement | Details |
|---|---|
| **Python 3.8+** | On your computer (for esptool & mpremote) |
| **pip** | Python package manager |
| **USB Driver** | CP2102 or CH340 (same as Arduino) |
| **ESP32 Board** | Any variant with 4MB+ flash |
| **USB Cable** | Data-capable cable |

---

## 📥 Step 1 — Download Firmware

Download the correct MicroPython firmware for your ESP32 variant:

| ESP32 Variant | Firmware Download Page |
|---|---|
| **ESP32** (Classic) | [micropython.org/download/ESP32_GENERIC](https://micropython.org/download/ESP32_GENERIC/) |
| **ESP32-S2** | [micropython.org/download/ESP32_GENERIC_S2](https://micropython.org/download/ESP32_GENERIC_S2/) |
| **ESP32-S3** | [micropython.org/download/ESP32_GENERIC_S3](https://micropython.org/download/ESP32_GENERIC_S3/) |
| **ESP32-C3** | [micropython.org/download/ESP32_GENERIC_C3](https://micropython.org/download/ESP32_GENERIC_C3/) |
| **ESP32-C6** | [micropython.org/download/ESP32_GENERIC_C6](https://micropython.org/download/ESP32_GENERIC_C6/) |

1. Visit the download page for your chip
2. Download the latest **stable** `.bin` file (avoid "preview" versions for production)
3. Note where you saved the file

> [!TIP]
> Choose the firmware variant that matches your board's flash size. Most boards have 4MB; some ESP32-S3 boards have 8MB or 16MB. If your board has PSRAM (SPIRAM), look for firmware builds with SPIRAM support.

---

## 🔧 Step 2 — Install esptool.py

`esptool.py` is Espressif's official tool for flashing firmware:

```bash
# Install esptool
pip install esptool

# Verify installation
esptool.py version
```

> [!IMPORTANT]
> On Linux, you may need to add your user to the `dialout` group for serial port access:
> ```bash
> sudo usermod -aG dialout $USER
> # Log out and back in for the change to take effect
> ```

---

## ⚡ Step 3 — Flash the Firmware

### Identify Your Port

```bash
# macOS
ls /dev/cu.usb*

# Linux
ls /dev/ttyUSB* /dev/ttyACM*

# Windows — check Device Manager for COMx
```

### Erase & Flash

```bash
# Step 1: Erase the entire flash (recommended for clean install)
esptool.py --chip esp32 --port /dev/ttyUSB0 erase_flash

# Step 2: Flash MicroPython firmware
esptool.py --chip esp32 --port /dev/ttyUSB0 \
    --baud 460800 write_flash -z 0x1000 \
    ESP32_GENERIC-20241025-v1.23.0.bin
```

### Flash Addresses by Chip

| Chip | Flash Address | Example Command |
|---|---|---|
| **ESP32** | `0x1000` | `write_flash -z 0x1000 firmware.bin` |
| **ESP32-S2** | `0x1000` | `write_flash -z 0x1000 firmware.bin` |
| **ESP32-S3** | `0x0` | `write_flash -z 0x0 firmware.bin` |
| **ESP32-C3** | `0x0` | `write_flash -z 0x0 firmware.bin` |
| **ESP32-C6** | `0x0` | `write_flash -z 0x0 firmware.bin` |

> [!WARNING]
> The flash address differs between chip variants! Using the wrong address will result in a non-bootable device. ESP32 Classic and S2 use `0x1000`; S3, C3, and C6 use `0x0`. Always check the firmware download page for the correct address.

### Verify Flash

After flashing, connect to the REPL to verify:

```bash
# Connect via serial (screen)
screen /dev/ttyUSB0 115200

# Or use mpremote
mpremote connect /dev/ttyUSB0
```

Press **Enter** — you should see the MicroPython `>>>` prompt:

```
MicroPython v1.23.0 on 2024-10-25; Generic ESP32 module with ESP32
Type "help()" for more information.
>>>
```

---

## 🖥️ Step 4 — Connect with Thonny

Thonny is the **easiest way** to develop with MicroPython — it's a beginner-friendly Python IDE with built-in MicroPython support.

### Install Thonny

| OS | Installation |
|---|---|
| **Windows / macOS** | Download from [https://thonny.org/](https://thonny.org/) |
| **Linux** | `pip install thonny` or `sudo apt install thonny` |

### Configure for MicroPython

1. Open Thonny
2. Go to **Tools → Options → Interpreter**
3. Select **"MicroPython (ESP32)"** from the dropdown
4. Select your **Port** (e.g., `/dev/ttyUSB0` or `COM3`)
5. Click **OK**

### Using Thonny

| Feature | How |
|---|---|
| **REPL** | Bottom pane — type Python directly on the ESP32 |
| **Run script** | Write code in the top pane → click ▶ Run |
| **Save to ESP32** | File → Save as → Select "MicroPython device" |
| **File manager** | View → Files (shows both PC and ESP32 files) |
| **Stop** | Click 🛑 Stop/Restart to interrupt running code |

> [!TIP]
> Thonny can also **flash MicroPython firmware** directly! Go to **Tools → Options → Interpreter → Install or update MicroPython (esptool)** — select your port, choose the firmware file, and click Install.

---

## 📟 Step 5 — Connect with mpremote

`mpremote` is the official **command-line tool** for MicroPython, ideal for scripting and advanced workflows.

### Install

```bash
pip install mpremote
```

### Basic Usage

```bash
# Connect to REPL (interactive mode)
mpremote connect /dev/ttyUSB0

# Run a script on the device (without saving it)
mpremote run my_script.py

# Execute a one-liner
mpremote exec "import machine; print(machine.freq())"

# Copy file to the device
mpremote cp main.py :main.py

# Copy file from the device
mpremote cp :main.py ./main_backup.py

# List files on the device
mpremote ls

# Remove a file
mpremote rm :old_script.py

# Reset the device
mpremote reset

# Mount local directory on device (for development)
mpremote mount .
```

> [!NOTE]
> `mpremote mount .` is a powerful development feature — it mounts your **local directory** on the device so you can edit files on your computer and they're immediately available on the ESP32 without copying. Perfect for rapid iteration!

---

## 🚀 Step 6 — Your First Script

### LED Blink (via REPL)

Type this directly in the MicroPython REPL:

```python
from machine import Pin
from time import sleep

led = Pin(2, Pin.OUT)  # GPIO 2 — built-in LED on most boards

while True:
    led.on()
    print("LED ON")
    sleep(1)
    led.off()
    print("LED OFF")
    sleep(1)
```

Press `Ctrl+C` to stop.

### WiFi Connection

```python
import network
import time

def connect_wifi(ssid, password):
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)

    if not wlan.isconnected():
        print(f"Connecting to {ssid}...")
        wlan.connect(ssid, password)

        timeout = 10
        while not wlan.isconnected() and timeout > 0:
            time.sleep(1)
            timeout -= 1
            print(".", end="")

    if wlan.isconnected():
        print(f"\n✅ Connected! IP: {wlan.ifconfig()[0]}")
    else:
        print("\n❌ Connection failed!")

    return wlan

# Usage
wlan = connect_wifi("YOUR_SSID", "YOUR_PASSWORD")
```

### Boot & Main Scripts

MicroPython automatically runs two special files:

| File | When | Purpose |
|---|---|---|
| **`boot.py`** | On every boot (first) | Hardware init, WiFi connect |
| **`main.py`** | After `boot.py` | Your main application |

```python
# boot.py — runs on every startup
import network
import time

wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect("YOUR_SSID", "YOUR_PASSWORD")

while not wlan.isconnected():
    time.sleep(0.5)

print(f"WiFi connected: {wlan.ifconfig()[0]}")
```

```python
# main.py — your application
from machine import Pin, PWM
from time import sleep

led = Pin(2, Pin.OUT)
print("Application started!")

while True:
    led.value(not led.value())
    sleep(0.5)
```

---

## 📁 Step 7 — File Management

### Using mpremote

```bash
# List all files on the device
mpremote ls
mpremote ls /lib                    # List files in /lib directory

# Copy files to device
mpremote cp main.py :main.py
mpremote cp boot.py :boot.py
mpremote cp lib/my_module.py :/lib/my_module.py

# Copy files from device
mpremote cp :main.py ./backup_main.py

# Copy entire directories
mpremote cp -r ./lib/ :/lib/

# Delete files
mpremote rm :old_file.py

# Create directories
mpremote mkdir /lib
mpremote mkdir /data

# Cat file contents
mpremote cat :main.py
```

### Using the REPL

```python
import os

# List files in root
os.listdir("/")

# List files in /lib
os.listdir("/lib")

# File stats
os.stat("main.py")

# Create directory
os.mkdir("data")

# Remove file
os.remove("old_file.py")

# Remove empty directory
os.rmdir("empty_dir")

# Check free space
import esp
print(f"Flash size: {esp.flash_size()} bytes")

fs_stat = os.statvfs("/")
free = fs_stat[0] * fs_stat[3]
total = fs_stat[0] * fs_stat[2]
print(f"File system: {free // 1024}KB free / {total // 1024}KB total")
```

### File System Layout

```
/ (root of internal flash)
├── boot.py          # Auto-runs on boot (first)
├── main.py          # Auto-runs after boot.py
├── lib/             # Custom modules and libraries
│   ├── my_sensor.py
│   └── wifi_manager.py
└── data/            # Data files, configs, logs
    └── config.json
```

---

## 📦 Step 8 — Installing Packages with mip

`mip` is MicroPython's **built-in package manager** (replaces the older `upip`).

### Install from MicroPython REPL

```python
import mip

# Install a package (requires WiFi connection)
mip.install("aiohttp")
mip.install("umqtt.simple")
mip.install("ssd1306")               # OLED display driver
mip.install("neopixel")              # WS2812 LED driver
mip.install("dht")                   # DHT temperature sensor

# Install from a specific URL
mip.install("github:peterhinch/micropython-async/v3/primitives")

# Install to a specific target directory
mip.install("umqtt.simple", target="/lib")
```

### Install from CLI with mpremote

```bash
# Install packages via mpremote (no WiFi needed on device!)
mpremote mip install aiohttp
mpremote mip install umqtt.simple
mpremote mip install ssd1306
mpremote mip install github:peterhinch/micropython-async/v3/primitives
```

> [!TIP]
> Using `mpremote mip install` downloads the package on your computer and transfers it to the device. This is more reliable than installing directly from the REPL, especially for large packages or unreliable WiFi.

### Popular MicroPython Packages

| Package | Install Command | Use Case |
|---|---|---|
| **umqtt.simple** | `mip.install("umqtt.simple")` | MQTT client |
| **aiohttp** | `mip.install("aiohttp")` | Async HTTP requests |
| **ssd1306** | `mip.install("ssd1306")` | OLED displays (I2C/SPI) |
| **dht** | Built-in | DHT11/22 temperature sensor |
| **neopixel** | Built-in | WS2812/SK6812 LEDs |
| **urequests** | `mip.install("urequests")` | HTTP requests (simple) |
| **uasyncio** | Built-in | Async/await framework |
| **micropython-async** | `mip.install("github:peterhinch/micropython-async/v3/primitives")` | Advanced async utilities |
| **microdot** | `mip.install("github:miguelgrinberg/microdot/src/microdot")` | Lightweight web framework |

---

## ⚖️ MicroPython vs Arduino vs ESP-IDF

| Criteria | MicroPython | Arduino | ESP-IDF |
|---|---|---|---|
| **Language** | Python | C++ | C |
| **Learning curve** | 🟢 Easy | 🟡 Medium | 🔴 Steep |
| **Performance** | ⚠️ Slower (interpreted) | ✅ Fast | ✅ Fastest |
| **Prototyping speed** | 🚀 Fastest | 🟡 Medium | 🐢 Slowest |
| **Memory usage** | Higher overhead | Medium | Most efficient |
| **Debugging** | REPL + print | Serial + limited | Full JTAG |
| **Libraries** | Growing ecosystem | Massive | Espressif components |
| **Production ready** | Prototyping | Yes | Yes (enterprise) |
| **Best for** | Learning, prototyping | Projects, hobbyists | Products, firmware |

---

## 🧰 Essential MicroPython Modules

### Built-in Modules

```python
# Hardware Access
from machine import Pin, ADC, PWM, I2C, SPI, UART, Timer, RTC
import esp, esp32

# Networking
import network       # WiFi, Ethernet
import socket        # TCP/UDP sockets
import ssl           # TLS/SSL

# System
import os            # File operations
import sys           # System info
import gc            # Garbage collector
import time          # Time functions
import json          # JSON encode/decode
import struct        # Binary data packing
import hashlib       # Hashing (SHA256, etc.)

# Async
import asyncio       # Async/await (uasyncio)
```

### Quick Hardware Examples

```python
# ADC (Analog Read)
from machine import ADC, Pin
adc = ADC(Pin(34))
adc.atten(ADC.ATTN_11DB)    # 0-3.3V range
value = adc.read()            # 0-4095

# PWM
from machine import PWM, Pin
pwm = PWM(Pin(2), freq=1000, duty=512)  # 50% duty cycle

# I2C
from machine import I2C, Pin
i2c = I2C(0, scl=Pin(22), sda=Pin(21), freq=400000)
devices = i2c.scan()          # Returns list of addresses
print(f"I2C devices: {[hex(d) for d in devices]}")

# Deep Sleep
import machine
machine.deepsleep(60000)      # Sleep for 60 seconds (ms)

# Temperature (internal sensor)
import esp32
print(f"CPU Temp: {esp32.raw_temperature()}°F")
```

---

## 🔧 Troubleshooting

| Problem | Solution |
|---|---|
| **No `>>>` prompt after flash** | Press Enter or Reset button; verify baud = 115200 |
| **`ImportError: no module named X`** | Install with `mip.install("X")` or check spelling |
| **Out of memory (`MemoryError`)** | Use `gc.collect()` often; split code into modules |
| **WiFi won't connect** | Verify credentials; check signal strength; call `wlan.active(True)` |
| **REPL is frozen** | Press `Ctrl+C` to interrupt; `Ctrl+D` for soft reboot |
| **File system full** | Run `os.listdir("/")` and remove unneeded files |
| **`OSError: [Errno 2] ENOENT`** | File/directory doesn't exist — check path and use `os.listdir()` |
| **Firmware won't flash** | Erase flash first: `esptool.py erase_flash`; hold BOOT button |
| **Screen/serial garbled text** | Wrong baud rate — use 115200 for MicroPython REPL |
| **Board reboots in loop** | Error in `boot.py` or `main.py`. Connect and press `Ctrl+C` fast, then fix the script |

> [!WARNING]
> If `boot.py` or `main.py` has an error that causes an infinite reboot loop, you may need to **erase the flash** and re-flash MicroPython to recover. To prevent this, always test code in the REPL before saving to `main.py`.

> [!TIP]
> Use **safe mode** to bypass `boot.py` and `main.py`: connect to serial, press `Ctrl+C` rapidly during boot to interrupt auto-run. If that fails, re-flash the firmware with `esptool.py`.

---

## 📚 Additional Resources

| Resource | Link |
|---|---|
| MicroPython Documentation | [https://docs.micropython.org/](https://docs.micropython.org/) |
| MicroPython ESP32 Quick Ref | [https://docs.micropython.org/en/latest/esp32/quickref.html](https://docs.micropython.org/en/latest/esp32/quickref.html) |
| MicroPython Firmware Downloads | [https://micropython.org/download/](https://micropython.org/download/) |
| MicroPython Forum | [https://forum.micropython.org/](https://forum.micropython.org/) |
| Awesome MicroPython | [https://awesome-micropython.com/](https://awesome-micropython.com/) |
| Arduino IDE Setup (Alternative) | [SETUP_ARDUINO.md](SETUP_ARDUINO.md) |
| PlatformIO Setup (Alternative) | [SETUP_PLATFORMIO.md](SETUP_PLATFORMIO.md) |
| ESP-IDF Setup (Advanced) | [SETUP_ESPIDF.md](SETUP_ESPIDF.md) |

---

<p align="center">Made with ❤️ by <a href="https://github.com/tushar2704">Tushar Aggarwal</a> | 📺 <a href="https://youtube.com/@IamNotIntoTech">@IamNotIntoTech</a> + <a href="https://youtube.com/@Makerrdesigns">@Makerrdesigns</a></p>
