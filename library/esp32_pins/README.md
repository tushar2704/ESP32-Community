# 🔌 esp32_pins — Universal Board Pinout Abstraction

> **Write hardware-agnostic firmware that runs seamlessly across different ESP32 boards and chip generations.**

---

## 🚀 How to Use

Select your board variant before including `esp32_pins.h` (or set it in your `platformio.ini` build flags):

```cpp
// Option A: In code
#define BOARD_ESP32_S3_DEVKIT
#include <esp32_pins.h>

void setup() {
    Serial.begin(115200);
    Serial.printf("Running on: %s\n", BOARD_NAME);
    
    // Automatic pin mapping
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    SPI.begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_CS);
}
```

```ini
; Option B: In platformio.ini
[env:esp32cam]
platform = espressif32
board = esp32cam
build_flags = -D BOARD_ESP32_CAM
```

---

## 📋 Supported Boards & Default Pin Macros

| Board | Build Flag | LED Pin | I2C (SDA / SCL) | SPI (MOSI / MISO / SCK / CS) |
| :--- | :--- | :--- | :--- | :--- |
| **ESP32 DevKitC v4** | `-D BOARD_ESP32_DEVKITC` | `2` | `21` / `22` | `23` / `19` / `18` / `5` |
| **AI-Thinker ESP32-CAM** | `-D BOARD_ESP32_CAM` | `4` (Flash) / `33` | `26` / `27` | MicroSD SDMMC Bus |
| **ESP32-S3-DevKitC-1** | `-D BOARD_ESP32_S3_DEVKIT` | `48` (RGB) | `8` / `9` | `11` / `13` / `12` / `10` |
| **ESP32-C3-DevKitM-1** | `-D BOARD_ESP32_C3_DEVKIT` | `8` | `8` / `9` | `6` / `5` / `4` / `7` |
| **ESP32-C6-DevKitC-1** | `-D BOARD_ESP32_C6_DEVKIT` | `8` (RGB) | `6` / `7` | `21` / `20` / `19` / `18` |

---

<div align="center">

**Part of the [ESP32 Community Hub](https://github.com/tushar2704/ESP32-Community)**  
<sub>Author: Tushar Aggarwal ([@tushar2704](https://github.com/tushar2704)) • License: AGPLv3</sub>

</div>
