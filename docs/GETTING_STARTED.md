# 🚀 Getting Started with ESP32

> Your zero-to-hero guide — from unboxing to your first blinking LED!

[![ESP32 Community](https://img.shields.io/badge/ESP32-Community-blue?style=for-the-badge&logo=espressif)](https://github.com/tushar2704/ESP32-Community)

---

## 📑 Table of Contents

- [Choosing Your First Board](#-choosing-your-first-board)
- [What You'll Need](#-what-youll-need)
- [Installing Arduino IDE 2.x](#-installing-arduino-ide-2x)
- [Installing PlatformIO](#-installing-platformio)
- [Your First Project — LED Blink](#-your-first-project--led-blink)
- [Using the Web Flasher](#-using-the-web-flasher)
- [Next Steps](#-next-steps)

---

## 🛒 Choosing Your First Board

Not sure which ESP32 to pick? Here's a quick comparison of beginner-friendly boards:

| Board | Chip | WiFi | Bluetooth | USB | Price (Approx.) | Best For |
|-------|------|------|-----------|-----|-----------------|----------|
| **ESP32 DevKitC V4** | ESP32 (Classic) | 2.4 GHz | BT 4.2 + Classic | Micro-USB (CP2102) | ~$4-6 | General learning |
| **ESP32-S3 DevKitC** | ESP32-S3 | 2.4 GHz | BLE 5.0 | USB-C (native) | ~$7-10 | AI/ML, USB projects |
| **ESP32-C3 SuperMini** | ESP32-C3 | 2.4 GHz | BLE 5.0 | USB-C (native) | ~$2-3 | Compact, low-cost IoT |
| **ESP32-C6 DevKit** | ESP32-C6 | 2.4 GHz + WiFi 6 | BLE 5.3 | USB-C (native) | ~$8-10 | Matter/Thread/Zigbee |
| **XIAO ESP32-S3** | ESP32-S3 | 2.4 GHz | BLE 5.0 | USB-C (native) | ~$7 | Tiny wearables |
| **ESP32-CAM** | ESP32 (Classic) | 2.4 GHz | BT 4.2 | None (needs FTDI) | ~$4-6 | Camera projects |

> [!TIP]
> **Our recommendation for absolute beginners:** The **ESP32 DevKitC V4** or **ESP32-S3 DevKitC**. They have the most tutorials, widest library support, and are easy to find online.

> [!WARNING]
> Avoid the ESP32-CAM as your first board — it has no built-in USB-to-serial, requires an FTDI adapter, and has limited GPIO pins.

---

## 🧰 What You'll Need

| Item | Purpose | Required? |
|------|---------|-----------|
| ESP32 development board | The brain! | ✅ Yes |
| USB cable (Micro-USB or USB-C) | Power + programming | ✅ Yes |
| Computer (Win/Mac/Linux) | Development environment | ✅ Yes |
| Breadboard | Prototyping | 🟡 Recommended |
| Jumper wires | Connecting components | 🟡 Recommended |
| LED + 220Ω resistor | First project | 🟡 Recommended |

> [!NOTE]
> Most ESP32 boards have a **built-in LED** (usually on GPIO 2), so you can do the blink project without any external components!

---

## 💻 Installing Arduino IDE 2.x

**Step 1:** Download Arduino IDE 2.x from [arduino.cc/en/software](https://www.arduino.cc/en/software)

**Step 2:** Install and launch the IDE

**Step 3:** Add the ESP32 board manager URL:
- Go to **File → Preferences**
- In "Additional boards manager URLs", add:
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

**Step 4:** Install the ESP32 board package:
- Go to **Tools → Board → Boards Manager**
- Search for **"esp32"**
- Install **"esp32 by Espressif Systems"** (latest version)

**Step 5:** Select your board:
- Go to **Tools → Board → esp32**
- Choose your specific board (e.g., "ESP32 Dev Module", "ESP32S3 Dev Module")

**Step 6:** Select the correct port:
- Go to **Tools → Port**
- Select the COM/serial port your ESP32 is connected to

> [!TIP]
> Can't find the port? You may need to install USB drivers:
> - **CP2102/CP2104**: [Silicon Labs drivers](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
> - **CH340/CH341**: [WCH drivers](http://www.wch-ic.com/downloads/CH341SER_ZIP.html)

📖 **Full guide:** [SETUP_ARDUINO.md](SETUP_ARDUINO.md)

---

## ⚡ Installing PlatformIO

**Step 1:** Install [Visual Studio Code](https://code.visualstudio.com/)

**Step 2:** Open VS Code → Extensions → Search "PlatformIO IDE" → Install

**Step 3:** Create a new project:
- Click the PlatformIO icon (alien head) in the sidebar
- Click **"New Project"**
- Select your board (e.g., "Espressif ESP32 Dev Module")
- Choose framework: **Arduino** or **ESP-IDF**

**Step 4:** Write code in `src/main.cpp` and click **Upload** (→ arrow)

> [!TIP]
> PlatformIO automatically detects your board, installs the right toolchain, and manages libraries. It's the recommended IDE for serious development!

📖 **Full guide:** [SETUP_PLATFORMIO.md](SETUP_PLATFORMIO.md)

---

## 💡 Your First Project — LED Blink

This classic project makes an LED blink on and off. Most ESP32 boards have a built-in LED on **GPIO 2**.

### Arduino IDE Code

```cpp
// ESP32 LED Blink — Your First Project!
// Built-in LED is typically on GPIO 2

#define LED_PIN 2

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Hello from ESP32! 🚀");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);   // LED ON
  Serial.println("LED is ON");
  delay(1000);                   // Wait 1 second

  digitalWrite(LED_PIN, LOW);    // LED OFF
  Serial.println("LED is OFF");
  delay(1000);                   // Wait 1 second
}
```

### Upload Steps

1. Connect your ESP32 via USB
2. Select the correct **Board** and **Port** in Tools menu
3. Click the **Upload** button (→ arrow)
4. Open **Serial Monitor** (magnifying glass icon) at **115200 baud**
5. Watch the LED blink and see messages in the monitor! 🎉

> [!WARNING]
> **Upload failing?** Hold the **BOOT** button on your ESP32 while the IDE says "Connecting..." — release it once upload begins. Some boards need this for the first upload.

### MicroPython Version

```python
from machine import Pin
from time import sleep

led = Pin(2, Pin.OUT)

while True:
    led.on()
    print("LED is ON")
    sleep(1)
    led.off()
    print("LED is OFF")
    sleep(1)
```

---

## 🌐 Using the Web Flasher

Don't want to install anything? Use the **ESP Web Flasher** to program your board directly from Chrome/Edge!

1. Go to [**web.esphome.io**](https://web.esphome.io/) or [**espressif.github.io/esptool-js**](https://espressif.github.io/esptool-js/)
2. Connect your ESP32 via USB
3. Click **"Connect"** and select the serial port
4. Choose a firmware file (.bin) or a pre-built project
5. Click **"Install"** / **"Program"**

> [!NOTE]
> Web Flasher uses **WebSerial API** and only works in Chromium-based browsers (Chrome, Edge, Brave). Safari and Firefox are not supported.

**Popular Web Flasher projects:**
- **ESPHome** — Smart home firmware: [web.esphome.io](https://web.esphome.io/)
- **WLED** — Addressable LED controller: [install.wled.me](https://install.wled.me/)
- **Tasmota** — Smart device firmware: [tasmota.github.io/install](https://tasmota.github.io/install/)

---

## 🎯 Next Steps

Now that you've got your LED blinking, here's your learning path:

```
LED Blink ✅
    │
    ├── 📡 Connect to WiFi → WIRELESS_PROTOCOLS.md
    │
    ├── 📟 Add a Display → ADDONS.md
    │
    ├── 🌡️ Read Sensors → ADDONS.md
    │
    ├── 🔋 Go Battery-Powered → POWER_MANAGEMENT.md
    │
    ├── 📌 Understand GPIO Pins → PINOUT_REFERENCE.md
    │
    └── 🏗️ Build a Full Project → projects/ folder
```

| Resource | What You'll Learn |
|----------|-------------------|
| [CHIP_COMPARISON.md](CHIP_COMPARISON.md) | Deep dive into all ESP32 chip variants |
| [VARIANTS.md](VARIANTS.md) | Identify your specific board |
| [PINOUT_REFERENCE.md](PINOUT_REFERENCE.md) | Which pins to use (and avoid!) |
| [WIRELESS_PROTOCOLS.md](WIRELESS_PROTOCOLS.md) | WiFi, BLE, ESP-NOW, Thread, Matter |
| [ADDONS.md](ADDONS.md) | Sensors, displays, motors, and more |
| [TROUBLESHOOTING.md](TROUBLESHOOTING.md) | Fix common problems |
| [FAQ.md](FAQ.md) | Buying guide & common questions |
| [SHOWCASE.md](SHOWCASE.md) | Community project gallery |

> [!TIP]
> Join the community! Submit your projects, ask questions, and help others. Check out our [CONTRIBUTING.md](../CONTRIBUTING.md) guide.

---

<p align="center">Made with ❤️ by <a href="https://github.com/tushar2704">Tushar Aggarwal</a> | 📺 <a href="https://youtube.com/@IamNotIntoTech">@IamNotIntoTech</a> + <a href="https://youtube.com/@Makerrdesigns">@Makerrdesigns</a></p>
