# 🔌 Universal ESP32 GPIO Pinout & Hardware Reference

> **The ultimate safety guide for ESP32 GPIO pins:** Which pins are safe to use, which are strapping pins that can prevent booting, ADC channel quirks, and bus defaults across all major chip families.

---

## 🚦 Pin Safety Traffic Light Matrix (Classic ESP32)

```text
 🟢 SAFE TO USE ANYTIME     🟡 USE WITH CAUTION (STRAPPING)     🔴 DO NOT USE / RESTRICTED
 ─────────────────────     ───────────────────────────────     ──────────────────────────
 GPIO 4                    GPIO 0  (Bootloader state)          GPIO 6  (Integrated Flash)
 GPIO 13                   GPIO 2  (Must be LOW to flash)      GPIO 7  (Integrated Flash)
 GPIO 14                   GPIO 5  (Outputs PWM on boot)       GPIO 8  (Integrated Flash)
 GPIO 16 (UART2 RX)        GPIO 12 (Must be LOW for 3.3V)      GPIO 9  (Integrated Flash)
 GPIO 17 (UART2 TX)        GPIO 15 (Silent boot if LOW)        GPIO 10 (Integrated Flash)
 GPIO 18 (VSPI SCK)        GPIO 34 (Input only, no pullup)     GPIO 11 (Integrated Flash)
 GPIO 19 (VSPI MISO)       GPIO 35 (Input only, no pullup)
 GPIO 21 (I2C SDA)         GPIO 36 / VP (Input only)
 GPIO 22 (I2C SCL)         GPIO 39 / VN (Input only)
 GPIO 23 (VSPI MOSI)
 GPIO 25 (DAC1)
 GPIO 26 (DAC2)
 GPIO 27
 GPIO 32
 GPIO 33
```

---

## 📌 Standard 38-Pin DevKitC ASCII Pinout

```text
                           ┌──────────────┐
                    3V3  ──┤  1        38 ├──  GND
                     EN  ──┤  2        37 ├──  GPIO 23 (VSPI MOSI)
      (ADC1_CH0) GPIO 36 ──┤  3        36 ├──  GPIO 22 (I2C SCL)
      (ADC1_CH3) GPIO 39 ──┤  4        35 ├──  GPIO 1  (U0 TXD)
      (ADC1_CH6) GPIO 34 ──┤  5        34 ├──  GPIO 3  (U0 RXD)
      (ADC1_CH7) GPIO 35 ──┤  6        33 ├──  GPIO 21 (I2C SDA)
      (ADC1_CH4) GPIO 32 ──┤  7        32 ├──  GND
      (ADC1_CH5) GPIO 33 ──┤  8        31 ├──  GPIO 19 (VSPI MISO)
      (DAC1)     GPIO 25 ──┤  9        30 ├──  GPIO 18 (VSPI SCK)
      (DAC2)     GPIO 26 ──┤ 10        29 ├──  GPIO 5  (VSPI SS)
                 GPIO 27 ──┤ 11        28 ├──  GPIO 17 (U2 TXD)
      (HSPI SCK) GPIO 14 ──┤ 12        27 ├──  GPIO 16 (U2 RXD)
     (HSPI MISO) GPIO 12 ──┤ 13        26 ├──  GPIO 4  (ADC2_CH0)
                    GND  ──┤ 14        25 ├──  GPIO 0  (BOOT PIN)
     (HSPI MOSI) GPIO 13 ──┤ 15        24 ├──  GPIO 2  (ONBOARD LED)
       (HSPI SS) GPIO 15 ──┤ 16        23 ├──  GPIO 15 (HSPI SS)
                 GPIO 2  ──┤ 17        22 ├──  GPIO 8  (FLASH CLK) ⚠️
                 GPIO 0  ──┤ 18        21 ├──  GPIO 7  (FLASH SDO) ⚠️
                    VIN  ──┤ 19        20 ├──  GPIO 6  (FLASH SDI) ⚠️
                           └──────────────┘
```

---

## ⚠️ The Golden Rules of ESP32 GPIOs

### 1. Flash Memory Pins (GPIO 6 to 11)
Never connect sensors, buttons, or wires to GPIO 6, 7, 8, 9, 10, or 11. These connect the internal microcontroller die directly to the SPI NOR Flash memory chip. Modifying their voltage crashes the ESP32 instantly.

### 2. Strapping Pins (Boot Configuration)
These pins dictate whether the ESP32 boots into your code or enters firmware download mode:
- **GPIO 0**: Must be HIGH for normal SPI boot; pulled LOW to enter ROM serial flasher.
- **GPIO 2**: Must be floating or LOW during flashing; connected to onboard blue LED on most boards.
- **GPIO 12**: Selects flash voltage (HIGH = 1.8V, LOW = 3.3V). If pulled HIGH externally, the flash will not get enough power and the board enters a continuous bootloop.
- **GPIO 15**: Controls boot message debug output over UART0.

### 3. Input-Only Pins (GPI 34–39)
GPIO 34, 35, 36 (VP), and 39 (VN) are **input only**:
- They **do not have internal pull-up or pull-down resistors**.
- They cannot be used as digital outputs (`pinMode(34, OUTPUT)` does nothing).
- Excellent for analog sensors, battery monitors, or buttons with external resistors.

### 4. ADC2 Wi-Fi Conflict
- **ADC1 (GPIO 32, 33, 34, 35, 36, 39)**: Works 100% of the time, even with Wi-Fi transmitting.
- **ADC2 (GPIO 0, 2, 4, 12, 13, 14, 15, 25, 26, 27)**: Shares the analog controller with the Wi-Fi baseband driver. When `WiFi.begin()` is called, `analogRead()` on ADC2 pins returns invalid or corrupted data. Always map analog sensors to ADC1!

---

## 🗺️ ESP32-S3 Pin Highlights
The ESP32-S3 has 45 programmable GPIOs:
- **Native USB D- / D+**: Fixed on **GPIO 19 (D-)** and **GPIO 20 (D+)**.
- **Default I2C**: SDA = GPIO 8, SCL = GPIO 9.
- **Default SPI (FSPI)**: MOSI = GPIO 11, MISO = GPIO 13, SCK = GPIO 12, CS = GPIO 10.
- **RGB LED**: Commonly integrated on **GPIO 48** (WS2812B addressable LED).

## 🗺️ ESP32-C3 Pin Highlights
The ESP32-C3 has 22 GPIOs (RISC-V):
- **Default I2C**: SDA = GPIO 8, SCL = GPIO 9.
- **Default SPI**: MOSI = GPIO 6, MISO = GPIO 5, SCK = GPIO 4, CS = GPIO 7.
- **Built-in USB-Serial**: Fixed on **GPIO 18 (D-)** and **GPIO 19 (D+)**.
- **Boot Pin**: **GPIO 9** (Pulled LOW for bootloader).

---

<div align="center">

**Part of the [ESP32 Community Hub](https://github.com/tushar2704/ESP32-Community)**  
<sub>Maintained by Tushar Aggarwal ([@tushar2704](https://github.com/tushar2704))</sub>

</div>
