# 🔧 PlatformIO Setup for ESP32

> **The professional-grade development environment for ESP32 — powerful build system, library management, and multi-board support in VS Code.**

[![PlatformIO](https://img.shields.io/badge/PlatformIO-IDE-FF7F00?style=for-the-badge&logo=platformio&logoColor=white)](https://platformio.org/)
[![VS Code](https://img.shields.io/badge/VS%20Code-Editor-007ACC?style=for-the-badge&logo=visual-studio-code&logoColor=white)](https://code.visualstudio.com/)
[![ESP32](https://img.shields.io/badge/ESP32-Supported-E7352C?style=for-the-badge&logo=espressif&logoColor=white)](https://www.espressif.com/)

---

## 📑 Table of Contents

- [Why PlatformIO?](#-why-platformio)
- [Prerequisites](#-prerequisites)
- [Step 1 — Install VS Code](#-step-1--install-vs-code)
- [Step 2 — Install PlatformIO Extension](#-step-2--install-platformio-extension)
- [Step 3 — Create a New Project](#-step-3--create-a-new-project)
- [Step 4 — Understanding platformio.ini](#-step-4--understanding-platformioini)
- [Step 5 — Building & Uploading](#-step-5--building--uploading)
- [Step 6 — Serial Monitor](#-step-6--serial-monitor)
- [Step 7 — Library Manager](#-step-7--library-manager)
- [platformio.ini for Every ESP32 Variant](#-platformioini-for-every-esp32-variant)
- [Project Structure](#-project-structure)
- [Useful PlatformIO Commands](#-useful-platformio-commands)
- [Troubleshooting](#-troubleshooting)
- [Additional Resources](#-additional-resources)

---

## 🌟 Why PlatformIO?

| Feature | Arduino IDE | PlatformIO |
|---|---|---|
| **Code Editor** | Basic | Full VS Code (IntelliSense, refactoring) |
| **Build System** | Single board | Multi-environment (build for 5 boards at once) |
| **Library Management** | Manual install | Declarative in `platformio.ini` |
| **Debugging** | ❌ Limited | ✅ Full JTAG/OpenOCD debugging |
| **Unit Testing** | ❌ | ✅ Built-in test framework |
| **CI/CD** | ❌ | ✅ GitHub Actions integration |
| **Multi-file Projects** | Awkward | Native C/C++ project structure |

---

## 📋 Prerequisites

| Requirement | Details |
|---|---|
| **Operating System** | Windows 10+, macOS 10.15+, or Linux (64-bit) |
| **Python** | 3.6+ (usually bundled with PlatformIO) |
| **USB Driver** | CP2102 or CH340 (same as Arduino) |
| **Disk Space** | ~1.5 GB for PlatformIO Core + ESP32 platform |

---

## 💻 Step 1 — Install VS Code

1. Download from **[https://code.visualstudio.com/](https://code.visualstudio.com/)**
2. Install for your operating system
3. Launch VS Code

> [!TIP]
> If you already have VS Code installed, make sure it's updated to the latest version for best PlatformIO compatibility.

---

## 🧩 Step 2 — Install PlatformIO Extension

1. Open VS Code
2. Click the **Extensions** icon in the left sidebar (or press `Ctrl+Shift+X` / `Cmd+Shift+X`)
3. Search for **`PlatformIO IDE`**
4. Click **Install** on the extension by **PlatformIO**
5. Wait for installation to complete — a **PlatformIO Home** tab will open automatically
6. **Restart VS Code** when prompted

> [!IMPORTANT]
> PlatformIO will download its core components on first launch. This requires an internet connection and may take a few minutes. Watch the status bar at the bottom for progress.

---

## 🆕 Step 3 — Create a New Project

### Via GUI

1. Click the **PlatformIO icon** (alien head 👽) in the left sidebar
2. Click **"Create New Project"** (or go to PlatformIO Home → New Project)
3. Fill in:
   - **Name**: `my-esp32-project`
   - **Board**: Search and select your board (e.g., `Espressif ESP32 Dev Module`)
   - **Framework**: `Arduino` (or `ESP-IDF` for native development)
4. Click **Finish**

### Via CLI

```bash
# Install PlatformIO Core CLI (if not already installed)
pip install platformio

# Create a new project
mkdir my-esp32-project && cd my-esp32-project
pio project init --board esp32dev --project-option "framework=arduino"
```

---

## 📄 Step 4 — Understanding platformio.ini

The `platformio.ini` file is the heart of your project. It defines boards, frameworks, libraries, and build options.

### Basic Configuration

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
upload_speed = 921600
```

### Advanced Configuration

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino

; --- Serial ---
monitor_speed = 115200
monitor_filters = esp32_exception_decoder
upload_speed = 921600

; --- Build Flags ---
build_flags =
    -DCORE_DEBUG_LEVEL=3
    -DBOARD_HAS_PSRAM
    -mfix-esp32-psram-cache-issue

; --- Partition Table ---
board_build.partitions = default.csv

; --- Libraries (auto-installed) ---
lib_deps =
    bblanchon/ArduinoJson@^7.0.0
    knolleary/PubSubClient@^2.8
    adafruit/Adafruit NeoPixel@^1.12.0

; --- Upload Protocol ---
upload_protocol = esptool
; upload_port = COM3           ; Uncomment to fix port
; upload_port = /dev/ttyUSB0   ; Linux
```

> [!NOTE]
> Libraries listed in `lib_deps` are **automatically downloaded** on the first build. No manual installation required! You can specify exact versions with `@` syntax.

---

## 🏗️ Step 5 — Building & Uploading

### Using the Toolbar (Bottom Status Bar)

| Icon | Action | Shortcut |
|---|---|---|
| ✔️ | Build | `Ctrl+Alt+B` |
| → | Upload | `Ctrl+Alt+U` |
| 🧹 | Clean | — |
| 🔌 | Serial Monitor | `Ctrl+Alt+S` |

### Using the Command Palette

Press `Ctrl+Shift+P` (or `Cmd+Shift+P` on macOS) and type:
- `PlatformIO: Build`
- `PlatformIO: Upload`
- `PlatformIO: Clean`

### Using the CLI

```bash
# Build
pio run

# Upload
pio run --target upload

# Build + Upload
pio run -t upload

# Build a specific environment
pio run -e esp32s3

# Clean build files
pio run --target clean
```

> [!WARNING]
> If upload fails, try reducing `upload_speed` to `460800` or `115200` in `platformio.ini`. Also check that no other application (like Arduino IDE or a serial terminal) is occupying the port.

---

## 📡 Step 6 — Serial Monitor

### Start the Monitor

```bash
# CLI
pio device monitor

# With specific baud rate
pio device monitor --baud 115200

# With exception decoder (ESP32)
pio device monitor --filter esp32_exception_decoder
```

### Useful Monitor Filters

Add to `platformio.ini`:

```ini
monitor_speed = 115200
monitor_filters =
    esp32_exception_decoder    ; Decode stack traces
    colorize                   ; Color-coded output
    time                       ; Add timestamps
    send_on_enter              ; Send input on Enter key
```

> [!TIP]
> The `esp32_exception_decoder` filter is invaluable — it automatically translates crash stack traces into readable function names and line numbers. Always enable it during development.

---

## 📚 Step 7 — Library Manager

### Search for Libraries

```bash
# Search by name
pio pkg search "DHT sensor"

# Search with filters
pio pkg search "mqtt" --platform espressif32
```

### Install Libraries

```bash
# Install via CLI
pio pkg install --library "bblanchon/ArduinoJson"
pio pkg install --library "knolleary/PubSubClient@^2.8"
```

### Or add directly to `platformio.ini`:

```ini
lib_deps =
    ; Using library name
    bblanchon/ArduinoJson

    ; Using specific version
    knolleary/PubSubClient@^2.8

    ; Using GitHub URL
    https://github.com/me-no-dev/ESPAsyncWebServer.git

    ; Using local path
    ; symlink:///path/to/my/library
```

### Commonly Used ESP32 Libraries

| Library | `lib_deps` Entry | Use Case |
|---|---|---|
| **ArduinoJson** | `bblanchon/ArduinoJson` | JSON parsing/serialization |
| **PubSubClient** | `knolleary/PubSubClient` | MQTT client |
| **AsyncWebServer** | `me-no-dev/ESPAsyncWebServer` | Async HTTP server |
| **AsyncTCP** | `me-no-dev/AsyncTCP` | Async TCP (dependency) |
| **WiFiManager** | `tzapu/WiFiManager` | WiFi provisioning portal |
| **Adafruit NeoPixel** | `adafruit/Adafruit NeoPixel` | WS2812/SK6812 LEDs |
| **FastLED** | `fastled/FastLED` | Advanced LED control |
| **TFT_eSPI** | `bodmer/TFT_eSPI` | TFT display driver |
| **LittleFS** | Built-in | File system |

---

## 🎛️ platformio.ini for Every ESP32 Variant

### Multi-Environment Configuration

You can target multiple boards from a single project:

```ini
; ============================================
; Shared settings across all environments
; ============================================
[platformio]
default_envs = esp32dev

[common]
monitor_speed = 115200
lib_deps =
    bblanchon/ArduinoJson@^7.0.0

; ============================================
; ESP32 Classic (Xtensa LX6, Dual-Core)
; ============================================
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = ${common.monitor_speed}
lib_deps = ${common.lib_deps}

; ============================================
; ESP32-S2 (Xtensa LX7, Single-Core, USB OTG)
; ============================================
[env:esp32s2]
platform = espressif32
board = esp32-s2-saola-1
framework = arduino
monitor_speed = ${common.monitor_speed}
lib_deps = ${common.lib_deps}

; ============================================
; ESP32-S3 (Xtensa LX7, Dual-Core, USB + AI)
; ============================================
[env:esp32s3]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
monitor_speed = ${common.monitor_speed}
build_flags = -DBOARD_HAS_PSRAM
lib_deps = ${common.lib_deps}

; ============================================
; ESP32-C3 (RISC-V, Single-Core, BLE 5.0)
; ============================================
[env:esp32c3]
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino
monitor_speed = ${common.monitor_speed}
lib_deps = ${common.lib_deps}

; ============================================
; ESP32-C6 (RISC-V, WiFi 6, Thread/Zigbee)
; ============================================
[env:esp32c6]
platform = espressif32
board = esp32-c6-devkitc-1
framework = arduino
monitor_speed = ${common.monitor_speed}
lib_deps = ${common.lib_deps}

; ============================================
; ESP32-H2 (RISC-V, BLE + Thread, NO WiFi)
; ============================================
[env:esp32h2]
platform = espressif32
board = esp32-h2-devkitm-1
framework = arduino
monitor_speed = ${common.monitor_speed}
lib_deps = ${common.lib_deps}

; ============================================
; ESP32-CAM (AI-Thinker, OV2640)
; ============================================
[env:esp32cam]
platform = espressif32
board = esp32cam
framework = arduino
monitor_speed = ${common.monitor_speed}
build_flags = -DBOARD_HAS_PSRAM
lib_deps = ${common.lib_deps}
```

---

## 📂 Project Structure

```
my-esp32-project/
├── .pio/                   # Build artifacts (auto-generated, gitignored)
│   └── build/
│       └── esp32dev/
├── .vscode/                # VS Code settings (auto-generated)
│   ├── c_cpp_properties.json
│   └── extensions.json
├── include/                # Header files (.h)
│   └── config.h
├── lib/                    # Private/local libraries
│   └── MyLibrary/
│       ├── MyLibrary.h
│       └── MyLibrary.cpp
├── src/                    # Source files
│   └── main.cpp            # ← Your main code goes here
├── test/                   # Unit tests
│   └── test_main.cpp
├── data/                   # SPIFFS/LittleFS files (uploaded separately)
│   ├── index.html
│   └── config.json
└── platformio.ini          # Project configuration
```

> [!NOTE]
> In PlatformIO, your main file is `src/main.cpp` (not `.ino`). You must include `#include <Arduino.h>` at the top of the file — this is automatic in Arduino IDE but explicit in PlatformIO.

### Minimal `src/main.cpp`

```cpp
#include <Arduino.h>

#define LED_PIN 2

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    Serial.println("PlatformIO + ESP32 — Ready!");
}

void loop() {
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
}
```

---

## ⌨️ Useful PlatformIO Commands

```bash
# Project Management
pio project init                    # Initialize new project in current dir
pio project init --board esp32dev   # Init with specific board
pio project config                  # Show current project config

# Build & Upload
pio run                             # Build default environment
pio run -e esp32s3                  # Build specific environment
pio run -t upload                   # Build and upload
pio run -t upload -e esp32cam       # Upload to specific environment
pio run -t clean                    # Clean build files

# File System (SPIFFS/LittleFS)
pio run -t uploadfs                 # Upload data/ folder to file system
pio run -t buildfs                  # Build file system image only

# Monitoring
pio device monitor                  # Open serial monitor
pio device list                     # List connected devices

# Library Management
pio pkg install -l "ArduinoJson"    # Install library
pio pkg update                      # Update all packages
pio pkg list                        # List installed packages

# Advanced
pio check                           # Static code analysis
pio test                            # Run unit tests
pio debug                           # Start debugger
```

---

## 🔧 Troubleshooting

| Problem | Solution |
|---|---|
| **IntelliSense errors (red squiggles)** | Build once (`Ctrl+Alt+B`) to generate include paths |
| **Upload fails: port busy** | Close Serial Monitor before uploading; check no other app uses the port |
| **Build error: "Arduino.h not found"** | Ensure `framework = arduino` is in `platformio.ini` |
| **Wrong board selected** | Verify `board = ` matches your hardware. Run `pio boards espressif32` to list all options |
| **Library conflict** | Use `lib_deps` version pinning: `library@^2.0.0` |
| **SPIFFS/LittleFS upload fails** | Use `pio run -t uploadfs`; ensure files are in `data/` directory |
| **PlatformIO Home won't open** | Run `pio home` from terminal; reinstall extension if persistent |
| **Build is slow** | Add `build_cache_dir = .pio/build_cache` to `[platformio]` section |

> [!TIP]
> Run `pio boards espressif32` to see the full list of **500+** supported ESP32 boards with their specifications. Use the board ID in your `platformio.ini`.

---

## 📚 Additional Resources

| Resource | Link |
|---|---|
| PlatformIO Documentation | [https://docs.platformio.org/](https://docs.platformio.org/) |
| Espressif32 Platform | [https://registry.platformio.org/platforms/platformio/espressif32](https://registry.platformio.org/platforms/platformio/espressif32) |
| PlatformIO Library Registry | [https://registry.platformio.org/](https://registry.platformio.org/) |
| Arduino IDE Setup (Alternative) | [SETUP_ARDUINO.md](SETUP_ARDUINO.md) |
| ESP-IDF Setup (Advanced) | [SETUP_ESPIDF.md](SETUP_ESPIDF.md) |
| MicroPython Setup | [SETUP_MICROPYTHON.md](SETUP_MICROPYTHON.md) |

---

<p align="center">Made with ❤️ by <a href="https://github.com/tushar2704">Tushar Aggarwal</a> | 📺 <a href="https://youtube.com/@IamNotIntoTech">@IamNotIntoTech</a> + <a href="https://youtube.com/@Makerrdesigns">@Makerrdesigns</a></p>
