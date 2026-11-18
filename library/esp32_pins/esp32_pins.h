/**
 * @file esp32_pins.h
 * @brief Universal GPIO Pin Abstraction for popular ESP32 development boards
 * @author Tushar Aggarwal (@tushar2704)
 * @license AGPL-3.0
 * @url https://github.com/tushar2704/ESP32-Community
 *
 * Include this single header to configure default SPI, I2C, UART, and onboard
 * peripherals for your chosen board variant.
 */

#ifndef ESP32_PINS_H
#define ESP32_PINS_H

#include <Arduino.h>

// ══════════════════════════════════════════════════════════════════════════
// 1. ESP32 DevKitC v4 / NodeMCU-32S (Classic ESP32 WROOM/WROVER)
// ══════════════════════════════════════════════════════════════════════════
#if defined(BOARD_ESP32_DEVKITC) || defined(ARDUINO_ESP32_DEV) || (!defined(BOARD_ESP32_CAM) && !defined(BOARD_ESP32_S3_DEVKIT) && !defined(BOARD_ESP32_C3_DEVKIT) && !defined(BOARD_ESP32_C6_DEVKIT))

    #define BOARD_NAME          "ESP32-DevKitC v4"
    #define PIN_LED             2       // Built-in Blue LED
    #define PIN_BUTTON_BOOT     0       // Onboard BOOT key

    // I2C Defaults
    #define PIN_I2C_SDA         21
    #define PIN_I2C_SCL         22

    // VSPI Defaults (Primary SPI)
    #define PIN_SPI_MOSI        23
    #define PIN_SPI_MISO        19
    #define PIN_SPI_SCK         18
    #define PIN_SPI_CS          5

    // UART2 (Serial2)
    #define PIN_UART2_RX        16
    #define PIN_UART2_TX        17

    // DAC & ADC
    #define PIN_DAC1            25
    #define PIN_DAC2            26
    #define PIN_ADC_DEFAULT     34

// ══════════════════════════════════════════════════════════════════════════
// 2. AI-Thinker ESP32-CAM Module
// ══════════════════════════════════════════════════════════════════════════
#elif defined(BOARD_ESP32_CAM)

    #define BOARD_NAME          "AI-Thinker ESP32-CAM"
    #define PIN_FLASH_LED       4       // High-power onboard flashlight
    #define PIN_STATUS_LED      33      // Tiny red status LED (Active LOW)

    // Camera OV2640 Pinout
    #define PWDN_GPIO_NUM       32
    #define RESET_GPIO_NUM      -1
    #define XCLK_GPIO_NUM       0
    #define SIOD_GPIO_NUM       26
    #define SIOC_GPIO_NUM       27
    #define Y9_GPIO_NUM         35
    #define Y8_GPIO_NUM         34
    #define Y7_GPIO_NUM         39
    #define Y6_GPIO_NUM         36
    #define Y5_GPIO_NUM         21
    #define Y4_GPIO_NUM         19
    #define Y3_GPIO_NUM         18
    #define Y2_GPIO_NUM         5
    #define VSYNC_GPIO_NUM      25
    #define HREF_GPIO_NUM       23
    #define PCLK_GPIO_NUM       22

    // MicroSD Card Pins (1-bit / 4-bit mode)
    #define PIN_SD_CMD          15
    #define PIN_SD_CLK          14
    #define PIN_SD_D0           2
    #define PIN_SD_D1           4
    #define PIN_SD_D2           12
    #define PIN_SD_D3           13

// ══════════════════════════════════════════════════════════════════════════
// 3. ESP32-S3-DevKitC-1
// ══════════════════════════════════════════════════════════════════════════
#elif defined(BOARD_ESP32_S3_DEVKIT) || defined(CONFIG_IDF_TARGET_ESP32S3)

    #define BOARD_NAME          "ESP32-S3-DevKitC-1"
    #define PIN_RGB_LED         48      // WS2812 Addressable RGB LED
    #define PIN_BUTTON_BOOT     0

    // I2C Defaults
    #define PIN_I2C_SDA         8
    #define PIN_I2C_SCL         9

    // FSPI Defaults
    #define PIN_SPI_MOSI        11
    #define PIN_SPI_MISO        13
    #define PIN_SPI_SCK         12
    #define PIN_SPI_CS          10

    // Native USB Pins
    #define PIN_USB_DM          19
    #define PIN_USB_DP          20

// ══════════════════════════════════════════════════════════════════════════
// 4. ESP32-C3-DevKitM-1 / SuperMini (RISC-V)
// ══════════════════════════════════════════════════════════════════════════
#elif defined(BOARD_ESP32_C3_DEVKIT) || defined(CONFIG_IDF_TARGET_ESP32C3)

    #define BOARD_NAME          "ESP32-C3-DevKitM-1"
    #define PIN_LED             8       // Onboard user LED / RGB
    #define PIN_BUTTON_BOOT     9       // BOOT key

    // I2C Defaults
    #define PIN_I2C_SDA         8
    #define PIN_I2C_SCL         9

    // SPI Defaults
    #define PIN_SPI_MOSI        6
    #define PIN_SPI_MISO        5
    #define PIN_SPI_SCK         4
    #define PIN_SPI_CS          7

    // USB Serial/JTAG
    #define PIN_USB_DM          18
    #define PIN_USB_DP          19

// ══════════════════════════════════════════════════════════════════════════
// 5. ESP32-C6-DevKitC-1 (Wi-Fi 6 + Thread/Zigbee)
// ══════════════════════════════════════════════════════════════════════════
#elif defined(BOARD_ESP32_C6_DEVKIT) || defined(CONFIG_IDF_TARGET_ESP32C6)

    #define BOARD_NAME          "ESP32-C6-DevKitC-1"
    #define PIN_RGB_LED         8       // WS2812 Addressable LED
    #define PIN_BUTTON_BOOT     9

    // I2C Defaults
    #define PIN_I2C_SDA         6
    #define PIN_I2C_SCL         7

    // SPI Defaults
    #define PIN_SPI_MOSI        21
    #define PIN_SPI_MISO        20
    #define PIN_SPI_SCK         19
    #define PIN_SPI_CS          18

#endif

#endif // ESP32_PINS_H
