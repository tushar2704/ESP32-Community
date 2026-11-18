# 🛠️ ESP-IDF Setup for ESP32

> **The official Espressif IoT Development Framework — maximum control, full access to every hardware feature, and production-grade firmware development.**

[![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v5.x-E7352C?style=for-the-badge&logo=espressif&logoColor=white)](https://docs.espressif.com/projects/esp-idf/)
[![FreeRTOS](https://img.shields.io/badge/FreeRTOS-Included-00C853?style=for-the-badge)](https://www.freertos.org/)
[![C](https://img.shields.io/badge/C-Language-A8B9CC?style=for-the-badge&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))

---

## 📑 Table of Contents

- [What is ESP-IDF?](#-what-is-esp-idf)
- [Prerequisites](#-prerequisites)
- [Step 1 — Install ESP-IDF v5.x](#-step-1--install-esp-idf-v5x)
- [Step 2 — Configure Your Environment](#-step-2--configure-your-environment)
- [Step 3 — Create a Project with idf.py](#-step-3--create-a-project-with-idfpy)
- [Step 4 — Project Structure](#-step-4--project-structure)
- [Step 5 — menuconfig](#-step-5--menuconfig)
- [Step 6 — Build & Flash](#-step-6--build--flash)
- [Step 7 — Monitor](#-step-7--monitor)
- [VS Code Integration](#-vs-code-integration)
- [Key ESP-IDF Concepts](#-key-esp-idf-concepts)
- [Common idf.py Commands](#-common-idfpy-commands)
- [Troubleshooting](#-troubleshooting)
- [Additional Resources](#-additional-resources)

---

## 🔍 What is ESP-IDF?

ESP-IDF (Espressif IoT Development Framework) is the **official development framework** from Espressif for all ESP32 variants. Unlike Arduino, it gives you:

| Feature | Description |
|---|---|
| **Full hardware access** | Every peripheral, register, and feature exposed |
| **FreeRTOS built-in** | Real-time OS with tasks, queues, semaphores |
| **Component system** | Modular architecture with reusable components |
| **menuconfig** | Kernel-style configuration for 1000+ options |
| **Production tools** | Secure boot, flash encryption, OTA, manufacturing |
| **Latest features** | New chip support lands here first |

> [!NOTE]
> Arduino-ESP32 is actually built **on top of** ESP-IDF. When you use the Arduino framework, you're using a simplified wrapper around ESP-IDF. Going native gives you everything underneath.

---

## 📋 Prerequisites

### System Requirements

| OS | Requirements |
|---|---|
| **Windows 10/11** | No WSL needed — native installer available |
| **macOS** | Xcode Command Line Tools |
| **Ubuntu/Debian** | Build essentials + Python 3 + Git |
| **Fedora/RHEL** | Development tools + Python 3 + Git |

### Install System Dependencies

**macOS:**
```bash
xcode-select --install
brew install cmake ninja dfu-util python3
```

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install -y git wget flex bison gperf python3 python3-pip \
    python3-venv cmake ninja-build ccache libffi-dev libssl-dev \
    dfu-util libusb-1.0-0
```

**Fedora:**
```bash
sudo dnf install git wget flex bison gperf python3 python3-pip \
    cmake ninja-build ccache dfu-util libusb
```

> [!IMPORTANT]
> ESP-IDF requires **Python 3.8+**, **Git**, **CMake 3.16+**, and **Ninja**. Verify with:
> ```bash
> python3 --version && git --version && cmake --version && ninja --version
> ```

---

## 📥 Step 1 — Install ESP-IDF v5.x

### Option A — Using the Install Script (Recommended for macOS/Linux)

```bash
# Clone ESP-IDF
mkdir -p ~/esp
cd ~/esp
git clone -b v5.3.2 --recursive https://github.com/espressif/esp-idf.git

# Run the install script (installs all toolchains)
cd esp-idf
./install.sh all
```

> [!TIP]
> Use `./install.sh esp32` to install tools for only ESP32 Classic, or `./install.sh esp32,esp32s3,esp32c3` for specific targets. Using `all` installs toolchains for every chip variant.

### Option B — Windows Installer

1. Download the **ESP-IDF Tools Installer** from: [https://dl.espressif.com/dl/esp-idf/](https://dl.espressif.com/dl/esp-idf/)
2. Run the installer — it handles Git, Python, toolchains, and drivers
3. Choose ESP-IDF version **5.3.x** (latest stable)
4. Use the **ESP-IDF PowerShell** or **ESP-IDF Command Prompt** shortcuts created by the installer

### Option C — Using espup (Rust-based Installer)

```bash
# Alternative lightweight installer
cargo install espup
espup install
```

---

## ⚙️ Step 2 — Configure Your Environment

After installation, you must **source the environment** in every new terminal session:

**macOS / Linux:**
```bash
# Source the environment (required in every new terminal)
. $HOME/esp/esp-idf/export.sh
```

**Windows (PowerShell):**
```powershell
# Use the ESP-IDF PowerShell shortcut, or run:
C:\Espressif\frameworks\esp-idf-v5.3.2\export.ps1
```

### Make It Permanent (Optional)

Add to your shell profile for automatic setup:

```bash
# Add to ~/.bashrc, ~/.zshrc, or ~/.profile
alias get_idf='. $HOME/esp/esp-idf/export.sh'
```

Then type `get_idf` whenever you need ESP-IDF tools.

> [!WARNING]
> Do NOT add `export.sh` directly to your shell profile's auto-run. It modifies `PATH` and Python virtualenvs which can interfere with other development tools. Use an alias instead.

### Verify Installation

```bash
idf.py --version
# Expected output: ESP-IDF v5.3.2
```

---

## 🆕 Step 3 — Create a Project with idf.py

### From a Template

```bash
# Create a new project from the hello_world template
idf.py create-project my_esp32_project
cd my_esp32_project

# Or copy an example
cp -r $IDF_PATH/examples/get-started/hello_world ~/esp/my_project
cd ~/esp/my_project
```

### Set Target Chip

```bash
# Set target before building (default is esp32)
idf.py set-target esp32       # ESP32 Classic
idf.py set-target esp32s2     # ESP32-S2
idf.py set-target esp32s3     # ESP32-S3
idf.py set-target esp32c3     # ESP32-C3
idf.py set-target esp32c6     # ESP32-C6
idf.py set-target esp32h2     # ESP32-H2
```

> [!IMPORTANT]
> `set-target` **clears the build directory** and reconfigures the project. Run it once when starting a project, not on every build. If you switch targets, it will delete your previous build and `sdkconfig`.

---

## 📂 Step 4 — Project Structure

```
my_esp32_project/
├── CMakeLists.txt           # Top-level CMake file
├── sdkconfig                # Generated by menuconfig (do not edit manually)
├── sdkconfig.defaults       # Default config overrides (commit this)
├── partitions.csv           # Custom partition table (optional)
├── main/
│   ├── CMakeLists.txt       # Component CMake file
│   └── main.c               # ← Your application entry point
├── components/              # Custom/third-party components
│   └── my_component/
│       ├── CMakeLists.txt
│       ├── include/
│       │   └── my_component.h
│       └── my_component.c
├── build/                   # Build output (auto-generated, gitignored)
└── managed_components/      # Components from IDF Component Registry
```

### Minimal `main/main.c`

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "main";

#define LED_GPIO GPIO_NUM_2

void app_main(void)
{
    ESP_LOGI(TAG, "ESP-IDF Blink Example — Starting!");

    // Configure GPIO
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_set_level(LED_GPIO, 1);
        ESP_LOGI(TAG, "LED ON");
        vTaskDelay(pdMS_TO_TICKS(1000));

        gpio_set_level(LED_GPIO, 0);
        ESP_LOGI(TAG, "LED OFF");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

### Minimal `main/CMakeLists.txt`

```cmake
idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
)
```

### Top-level `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(my_esp32_project)
```

---

## ⚙️ Step 5 — menuconfig

ESP-IDF includes a powerful configuration system (Kconfig) with 1000+ options:

```bash
idf.py menuconfig
```

This opens a **terminal-based configuration UI** where you can configure:

| Category | Key Options |
|---|---|
| **Serial flasher config** | Flash size, speed, mode |
| **Partition Table** | Predefined or custom CSV |
| **Component config → ESP32** | Clock frequency, PSRAM, brownout |
| **Component config → Wi-Fi** | TX power, protocol, scan method |
| **Component config → Bluetooth** | BLE/Classic/both, memory mode |
| **Component config → FreeRTOS** | Tick rate, stack sizes, watchdog |
| **Component config → ESP-TLS** | Certificate bundle, TLS version |
| **Component config → Log** | Default log level (None → Verbose) |
| **Security features** | Secure boot, flash encryption |

### Navigation

| Key | Action |
|---|---|
| `↑ ↓` | Navigate items |
| `Enter` | Enter submenu / toggle |
| `Space` | Toggle boolean option |
| `?` | Help for current item |
| `S` | Save |
| `Q` | Quit |

> [!TIP]
> Use `sdkconfig.defaults` to persist your custom settings. This file is applied before menuconfig and should be committed to version control. The full `sdkconfig` file should be **gitignored** as it's auto-generated.
>
> ```bash
> # Create sdkconfig.defaults from current config
> diff sdkconfig sdkconfig.defaults
> ```

---

## 🔨 Step 6 — Build & Flash

### Build

```bash
# Full build
idf.py build

# Build with verbose output
idf.py build -v
```

The first build takes several minutes. Subsequent builds are incremental and much faster.

### Flash

```bash
# Flash to default port
idf.py flash

# Flash to specific port
idf.py -p /dev/ttyUSB0 flash          # Linux
idf.py -p /dev/cu.usbserial-0001 flash # macOS
idf.py -p COM3 flash                    # Windows

# Flash with specific baud rate
idf.py -p /dev/ttyUSB0 -b 460800 flash

# Build + Flash in one step
idf.py -p /dev/ttyUSB0 flash monitor
```

> [!WARNING]
> If flashing fails with `A fatal error occurred: Could not connect`, hold the **BOOT** button on your board while running the flash command. Release it once you see `Connecting...` succeed.

---

## 📺 Step 7 — Monitor

```bash
# Start serial monitor
idf.py monitor

# With specific port
idf.py -p /dev/ttyUSB0 monitor

# Build + Flash + Monitor (the holy trinity)
idf.py -p /dev/ttyUSB0 flash monitor
```

### Monitor Keyboard Shortcuts

| Shortcut | Action |
|---|---|
| `Ctrl+]` | Exit monitor |
| `Ctrl+T Ctrl+H` | Show help |
| `Ctrl+T Ctrl+R` | Reset target (via RTS) |
| `Ctrl+T Ctrl+F` | Toggle output filtering |
| `Ctrl+T Ctrl+Y` | Toggle timestamp display |
| `Ctrl+T Ctrl+P` | Toggle pause |

> [!NOTE]
> The IDF Monitor automatically **decodes panic backtraces** using addr2line, showing you the exact file and line number where a crash occurred. This works out of the box with no extra configuration.

---

## 💻 VS Code Integration

### ESP-IDF Extension for VS Code

1. Install the **"Espressif IDF"** extension from the VS Code Marketplace
2. Open Command Palette → `ESP-IDF: Configure ESP-IDF Extension`
3. Select your ESP-IDF installation path
4. Choose your Python environment

### VS Code Commands (Command Palette)

| Command | Description |
|---|---|
| `ESP-IDF: Set Espressif Device Target` | Set target chip |
| `ESP-IDF: SDK Configuration Editor` | GUI menuconfig |
| `ESP-IDF: Build your Project` | Build |
| `ESP-IDF: Flash your Project` | Flash |
| `ESP-IDF: Monitor your Device` | Serial monitor |
| `ESP-IDF: Build, Flash and Monitor` | All-in-one |
| `ESP-IDF: Show Examples Projects` | Browse IDF examples |
| `ESP-IDF: Create Project from Extension Template` | New project wizard |

> [!TIP]
> The VS Code extension includes a **GUI menuconfig editor** that is much more user-friendly than the terminal version. Use `ESP-IDF: SDK Configuration Editor` to access it.

### Recommended `.vscode/settings.json`

```json
{
    "idf.espIdfPath": "${env:HOME}/esp/esp-idf",
    "idf.toolsPath": "${env:HOME}/.espressif",
    "idf.port": "/dev/ttyUSB0",
    "idf.flashBaudRate": "460800",
    "idf.monitorBaudRate": "115200",
    "idf.openOcdConfigs": [
        "interface/ftdi/esp32_devkitj_v1.cfg",
        "target/esp32.cfg"
    ]
}
```

---

## 🧠 Key ESP-IDF Concepts

### ESP Logging System

```c
#include "esp_log.h"

static const char *TAG = "my_module";

ESP_LOGE(TAG, "Error message");      // Red — errors
ESP_LOGW(TAG, "Warning message");    // Yellow — warnings
ESP_LOGI(TAG, "Info message");       // Green — informational
ESP_LOGD(TAG, "Debug message");      // No color — debug
ESP_LOGV(TAG, "Verbose message");    // No color — verbose
```

### FreeRTOS Tasks

```c
void my_task(void *pvParameters) {
    while (1) {
        ESP_LOGI(TAG, "Task running on core %d", xPortGetCoreID());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// In app_main():
xTaskCreatePinnedToCore(my_task, "my_task", 4096, NULL, 5, NULL, 1);
```

### Non-Volatile Storage (NVS)

```c
#include "nvs_flash.h"

// Initialize NVS (required before WiFi)
esp_err_t ret = nvs_flash_init();
if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
}
ESP_ERROR_CHECK(ret);
```

---

## ⌨️ Common idf.py Commands

```bash
# Project Setup
idf.py create-project <name>       # Create new project
idf.py set-target <chip>           # Set chip target
idf.py menuconfig                  # Open configuration menu
idf.py reconfigure                 # Re-run CMake configuration

# Build & Flash
idf.py build                       # Build project
idf.py flash                       # Flash to device
idf.py monitor                     # Open serial monitor
idf.py flash monitor               # Flash then monitor
idf.py -p <PORT> flash monitor     # Specify port

# Maintenance
idf.py clean                       # Clean build (keep config)
idf.py fullclean                   # Full clean (delete build dir)
idf.py size                        # Show binary size report
idf.py size-components             # Size per component
idf.py size-files                  # Size per file

# Advanced
idf.py erase-flash                 # Erase entire flash
idf.py erase-otadata               # Reset OTA boot partition
idf.py uf2                         # Generate UF2 file (S2/S3)
idf.py add-dependency <component>  # Add IDF component registry dep
```

---

## 🔧 Troubleshooting

| Problem | Solution |
|---|---|
| **`idf.py: command not found`** | Run `. $HOME/esp/esp-idf/export.sh` |
| **CMake version too old** | Upgrade CMake: `pip install cmake --upgrade` |
| **Python errors during install** | Use `python3` explicitly; ensure 3.8+ |
| **Build fails: missing component** | Run `idf.py reconfigure` or `idf.py fullclean && idf.py build` |
| **Flash fails: permission denied** | Linux: `sudo usermod -aG dialout $USER` then re-login |
| **menuconfig won't open** | Install curses: `pip install windows-curses` (Windows) |
| **Stack overflow crash** | Increase stack in menuconfig or `xTaskCreate` size parameter |
| **WiFi won't start** | Ensure `nvs_flash_init()` is called before WiFi init |

---

## 📚 Additional Resources

| Resource | Link |
|---|---|
| ESP-IDF Programming Guide | [https://docs.espressif.com/projects/esp-idf/](https://docs.espressif.com/projects/esp-idf/) |
| ESP-IDF GitHub | [https://github.com/espressif/esp-idf](https://github.com/espressif/esp-idf) |
| IDF Component Registry | [https://components.espressif.com/](https://components.espressif.com/) |
| ESP-IDF Examples | [https://github.com/espressif/esp-idf/tree/master/examples](https://github.com/espressif/esp-idf/tree/master/examples) |
| Arduino IDE Setup (Beginner) | [SETUP_ARDUINO.md](SETUP_ARDUINO.md) |
| PlatformIO Setup (Alternative) | [SETUP_PLATFORMIO.md](SETUP_PLATFORMIO.md) |
| MicroPython Setup | [SETUP_MICROPYTHON.md](SETUP_MICROPYTHON.md) |

---

<p align="center">Made with ❤️ by <a href="https://github.com/tushar2704">Tushar Aggarwal</a> | 📺 <a href="https://youtube.com/@IamNotIntoTech">@IamNotIntoTech</a> + <a href="https://youtube.com/@Makerrdesigns">@Makerrdesigns</a></p>
