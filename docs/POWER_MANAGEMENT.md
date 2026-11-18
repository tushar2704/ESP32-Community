# 🔋 ESP32 Power Management — Battery & Sleep Guide

> **Everything you need to know about keeping your ESP32 running for days, weeks, or months on battery — sleep modes, power budgets, battery selection, and solar charging.**

---

## 📑 Table of Contents

- [Current Consumption by Chip](#-current-consumption-by-chip)
- [Sleep Modes Overview](#-sleep-modes-overview)
- [Deep Sleep](#-deep-sleep)
  - [Timer Wakeup](#timer-wakeup)
  - [Touch Wakeup](#touch-wakeup)
  - [External Wakeup (ext0)](#external-wakeup-ext0)
  - [External Wakeup (ext1)](#external-wakeup-ext1)
  - [ULP Coprocessor Wakeup](#ulp-coprocessor-wakeup)
- [Light Sleep](#-light-sleep)
- [Modem Sleep & WiFi Power Save](#-modem-sleep--wifi-power-save)
- [Battery Selection Guide](#-battery-selection-guide)
- [Solar Charging](#-solar-charging)
- [Power Budget Calculator](#-power-budget-calculator)
- [Practical Power-Saving Tips](#-practical-power-saving-tips)
- [Resources](#-resources)

---

## ⚡ Current Consumption by Chip

Understanding your chip's power draw in each mode is essential for calculating battery life.

| Mode | ESP32 | ESP32-S2 | ESP32-S3 | ESP32-C3 | ESP32-C6 | ESP32-H2 |
|---|:---:|:---:|:---:|:---:|:---:|:---:|
| Active (WiFi TX) | ~240 mA | ~310 mA | ~355 mA | ~350 mA | ~300 mA | — |
| Active (BLE TX) | ~130 mA | — | ~100 mA | ~100 mA | ~100 mA | ~80 mA |
| Active (CPU only) | ~30-68 mA | ~22-36 mA | ~25-50 mA | ~15-25 mA | ~18-30 mA | ~15-25 mA |
| Modem Sleep | ~20 mA | ~12 mA | ~25 mA | ~15 mA | ~18 mA | ~15 mA |
| Light Sleep | ~0.8 mA | ~0.75 mA | ~0.24 mA | ~0.13 mA | ~0.03 mA | ~0.03 mA |
| Deep Sleep | ~10 µA | ~20 µA | ~7 µA | ~5 µA | ~7 µA | ~8 µA |
| Hibernation | ~5 µA | ~20 µA | ~7 µA | ~5 µA | ~7 µA | ~8 µA |

> [!NOTE]
> Values are typical and vary with clock speed, peripherals in use, antenna design, and board-level regulators. Dev boards draw more than bare modules due to onboard LEDs, USB-UART bridges, and LDOs.

> [!TIP]
> The **ESP32-C6** has exceptional light sleep performance (~30 µA), making it one of the best choices for battery-powered WiFi/Thread devices.

---

## 💤 Sleep Modes Overview

| Mode | CPU | RAM | RTC | WiFi/BT | Wakeup Time | Use Case |
|---|:---:|:---:|:---:|:---:|:---:|---|
| **Active** | ✅ | ✅ | ✅ | ✅ | — | Processing, transmitting |
| **Modem Sleep** | ✅ | ✅ | ✅ | Pulsed | — | Connected but idle |
| **Light Sleep** | Paused | ✅ | ✅ | ❌ | ~1 ms | Frequent short wakeups |
| **Deep Sleep** | ❌ | ❌ | ✅ | ❌ | ~10 ms | Periodic sensor readings |
| **Hibernation** | ❌ | ❌ | Minimal | ❌ | ~10 ms | Extreme low power |

---

## 🌙 Deep Sleep

In deep sleep, the main CPU cores, most RAM, and all digital peripherals are powered off. Only the **RTC controller**, **RTC memory** (8 KB), and optionally the **ULP coprocessor** remain active. The chip draws only **5–20 µA**.

When the chip wakes, it goes through a full boot — `setup()` runs again from scratch. Store data in **RTC memory** to persist it across sleep cycles.

```cpp
// Store data across deep sleep cycles
RTC_DATA_ATTR int bootCount = 0;
```

### Timer Wakeup

Wake up after a fixed time period — the most common deep sleep pattern for periodic sensor readings.

```cpp
#define uS_TO_S_FACTOR 1000000ULL
#define TIME_TO_SLEEP  300  // 5 minutes

RTC_DATA_ATTR int bootCount = 0;

void setup() {
  Serial.begin(115200);
  bootCount++;
  Serial.printf("Boot #%d\n", bootCount);

  // === Do your work here ===
  // Read sensors, send data via WiFi/ESP-NOW, etc.

  // Configure timer wakeup
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Going to sleep for 5 minutes...");
  Serial.flush();
  esp_deep_sleep_start();
}

void loop() {
  // Never reached in deep sleep pattern
}
```

### Touch Wakeup

Wake when a capacitive touch pin is activated (ESP32 and ESP32-S2/S3 only).

```cpp
#define TOUCH_PIN T0  // GPIO4 on ESP32
#define THRESHOLD 40

void setup() {
  Serial.begin(115200);

  // Check wakeup reason
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TOUCHPAD) {
    Serial.println("Woke up from touch!");
  }

  touchAttachInterrupt(TOUCH_PIN, {}, THRESHOLD);
  esp_sleep_enable_touchpad_wakeup();
  esp_deep_sleep_start();
}

void loop() {}
```

> [!WARNING]
> Touch wakeup is **not available** on ESP32-C3, ESP32-C6, or ESP32-H2 (they lack capacitive touch hardware). Use ext0/ext1 GPIO wakeup instead.

### External Wakeup (ext0)

Wake when **one specific RTC GPIO** goes HIGH or LOW — ideal for a single button or PIR sensor.

```cpp
#define WAKEUP_PIN GPIO_NUM_33  // Must be an RTC GPIO

void setup() {
  Serial.begin(115200);
  Serial.println("Going to sleep... press the button to wake!");

  esp_sleep_enable_ext0_wakeup(WAKEUP_PIN, LOW); // Wake on LOW
  esp_deep_sleep_start();
}

void loop() {}
```

### External Wakeup (ext1)

Wake when **any pin in a bitmask** changes — allows multiple wakeup sources.

```cpp
#define BUTTON_PIN_BITMASK ((1ULL << GPIO_NUM_32) | (1ULL << GPIO_NUM_33))

void setup() {
  Serial.begin(115200);

  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);
  Serial.println("Sleeping... any button wakes me.");
  esp_deep_sleep_start();
}

void loop() {}
```

> [!IMPORTANT]
> `ext0` and `ext1` wakeup pins **must be RTC GPIOs**. On the original ESP32, these are: GPIO 0, 2, 4, 12-15, 25-27, 32-39. Check your chip's datasheet for the RTC GPIO mapping.

### ULP Coprocessor Wakeup

The Ultra-Low-Power (ULP) coprocessor can run a small program while the main CPUs sleep — it can read ADC values, check GPIOs, and wake the main chip only when thresholds are exceeded.

```c
// ULP programming requires ESP-IDF and the ULP toolchain
// Example: Read ADC, wake main CPU if value > threshold
// See: examples/system/ulp/ in ESP-IDF

#include "esp32/ulp.h"
#include "ulp_main.h"  // Generated from ULP assembly

void setup() {
  // Load ULP program
  ulp_load_binary(0, ulp_main_bin_start,
    (ulp_main_bin_end - ulp_main_bin_start) / sizeof(uint32_t));

  // Set wakeup period (measure every 100ms)
  ulp_set_wakeup_period(0, 100000);

  esp_sleep_enable_ulp_wakeup();
  ulp_run(&ulp_entry - RTC_SLOW_MEM);
  esp_deep_sleep_start();
}
```

> [!NOTE]
> The original ESP32 has a **FSM-based ULP**. The ESP32-S2 and ESP32-S3 also include a **RISC-V ULP** that can be programmed in C. The C3/C6/H2 do not have a ULP coprocessor.

---

## 😴 Light Sleep

Light sleep keeps RAM contents intact and wakes **much faster** (~1 ms vs ~10 ms for deep sleep). The CPU is paused but not powered off.

```cpp
void setup() {
  Serial.begin(115200);

  // Wake on timer
  esp_sleep_enable_timer_wakeup(2000000); // 2 seconds

  Serial.println("Entering light sleep...");
  Serial.flush();
  esp_light_sleep_start();  // Blocks here, resumes after wakeup

  Serial.println("Woke up from light sleep!"); // Continues here
}

void loop() {
  // Can use light sleep in loop too
  esp_sleep_enable_timer_wakeup(1000000);
  esp_light_sleep_start();
  // Do periodic work here
}
```

Light sleep supports all the same wakeup sources as deep sleep (timer, GPIO, touch, UART).

> [!TIP]
> Use light sleep when you need **fast response times** and need to preserve RAM state. Use deep sleep when you can afford a full reboot and need **absolute minimum power**.

---

## 📡 Modem Sleep & WiFi Power Save

When connected to WiFi but not actively transmitting, modem sleep turns off the radio between DTIM beacon intervals.

```cpp
#include <WiFi.h>

void setup() {
  WiFi.begin("SSID", "password");
  while (WiFi.status() != WL_CONNECTED) delay(500);

  // Enable WiFi power save (modem sleep)
  esp_wifi_set_ps(WIFI_PS_MIN_MODEM);  // Wake at every DTIM
  // or
  esp_wifi_set_ps(WIFI_PS_MAX_MODEM);  // Wake at listen interval
}
```

| Mode | Current | Latency | Description |
|---|---|---|---|
| `WIFI_PS_NONE` | ~120 mA | Lowest | Radio always on |
| `WIFI_PS_MIN_MODEM` | ~20 mA | Medium | Wake at every DTIM |
| `WIFI_PS_MAX_MODEM` | ~15 mA | Higher | Wake less frequently |

---

## 🔌 Battery Selection Guide

| Battery Type | Voltage | Capacity | Pros | Cons | Best For |
|---|:---:|---|---|---|---|
| **LiPo 3.7V** | 3.0–4.2V | 500–10,000 mAh | High energy density, rechargeable | Needs protection circuit, swelling risk | Most ESP32 projects |
| **LiFePO4 3.2V** | 2.5–3.6V | 500–3,000 mAh | Very safe, 2000+ cycles | Lower voltage, needs 3.3V LDO bypass | Long-lifetime outdoor sensors |
| **18650 Li-Ion** | 3.0–4.2V | 2,500–3,500 mAh | Cheap, widely available, rugged | Bulky, needs holder | Medium to large projects |
| **2×AA Alkaline** | 2.0–3.0V | ~2,500 mAh | Very cheap, no charger needed | Not rechargeable, voltage drops | Disposable deployments |
| **2×AA NiMH** | 2.0–2.8V | 1,900–2,800 mAh | Rechargeable, cheap | Low voltage, may need boost converter | Budget rechargeable |
| **CR123A** | 3.0V | ~1,500 mAh | Compact, high voltage | Expensive, not rechargeable | Compact sensors |
| **LiPo Coin (LIR2032)** | 3.0–4.2V | 40–70 mAh | Tiny | Very low capacity | ESP32-H2/C6 deep sleep only |

> [!WARNING]
> The ESP32's operating voltage range is **2.3V–3.6V** (varies by chip). Most dev boards include a 3.3V LDO, so you can feed them 5V USB or ~4.2V from a LiPo. **Never feed more than 3.6V directly to the VDD pin** without a regulator.

> [!TIP]
> For LiPo-powered projects, use boards with built-in charging like **Adafruit Feather ESP32**, **TTGO T-Display**, or **FireBeetle ESP32** — they include TP4056-based charging and battery connectors.

---

## ☀️ Solar Charging

Solar panels paired with a charging IC let your ESP32 run indefinitely outdoors.

### Recommended Charging ICs

| IC | Input Voltage | Charge Current | Features |
|---|:---:|:---:|---|
| **TP4056** | 4.5–5.5V | Up to 1A | Cheapest, linear, micro-USB modules available |
| **CN3791** | 4.5–6V | Up to 4A | MPPT-like, designed for solar panels |
| **BQ25185** | 4.4–13.8V | Up to 1.1A | True MPPT, I²C telemetry |
| **SPV1050** | 0.15–5.5V | — | Ultra-low-power energy harvester, tiny panels |

### Typical Solar Setup

```
┌──────────┐    ┌──────────┐    ┌────────┐    ┌────────┐
│  Solar   │───►│ TP4056   │───►│  LiPo  │───►│ ESP32  │
│  Panel   │    │ Charger  │    │  Cell   │    │  Board │
│  6V/2W   │    │ Module   │    │ 3.7V   │    │        │
└──────────┘    └──────────┘    └────────┘    └────────┘
```

**Panel sizing rule of thumb:**
- Panel watts ≥ **3× average consumption** (to account for cloudy days, angle, and losses)
- For a sensor that sleeps most of the time (~0.5 mA average): a small **6V / 1W** panel + 1000 mAh LiPo works well
- For always-on WiFi (~120 mA average): you need a **6V / 5W** panel + 3000+ mAh battery

> [!CAUTION]
> Do **not** connect a solar panel directly to the ESP32 without a regulator/charger IC. Solar panel open-circuit voltage can exceed the chip's maximum rating and cause permanent damage.

---

## 🧮 Power Budget Calculator

Use this formula to estimate your battery life:

```
Battery Life (hours) = Battery Capacity (mAh) / Average Current (mA)
```

**Example — Weather station with 5-minute intervals:**

| Phase | Duration | Current | Energy (mAh) |
|---|:---:|:---:|:---:|
| Deep sleep (5 min) | 299.5s | 0.01 mA | 0.00083 mAh |
| Boot + WiFi connect | 3s | 160 mA | 0.133 mAh |
| Read sensors | 0.1s | 30 mA | 0.00083 mAh |
| HTTP POST | 1.5s | 240 mA | 0.100 mAh |
| **Total per cycle** | **304.1s** | — | **0.235 mAh** |

Average current = 0.235 mAh / (304.1s / 3600) = **2.78 mA**

With a **3000 mAh LiPo**:
```
3000 / 2.78 = ~1,079 hours ≈ 45 days
```

> [!TIP]
> The biggest power drain in most projects is **WiFi connection time**. Pre-store the WiFi channel and BSSID in RTC memory to skip the scan phase and reduce connection time from ~3s to ~0.5s. Using ESP-NOW instead of WiFi can cut transmission power and time by 10×.

---

## 💡 Practical Power-Saving Tips

### Hardware Tips

1. **Remove the power LED** — Many dev boards have always-on LEDs drawing 2–10 mA. Desolder them or cut the trace.
2. **Bypass the LDO** — If powering from a LiFePO4 (3.2V) or regulated 3.3V supply, bypass the onboard LDO to eliminate its quiescent current (~50–100 µA on cheap boards).
3. **Use bare modules** — Ditch dev boards for production. An ESP32-WROOM module alone draws far less than a full DevKitC.
4. **Add a load switch** — Use a MOSFET or load switch IC (e.g., TPS22918) to cut power to sensors, displays, and SD cards when sleeping.
5. **Choose efficient regulators** — Replace cheap AMS1117 LDOs (5 mA quiescent) with HT7833 (< 4 µA) or ME6211 (40 µA).

### Software Tips

6. **Reduce CPU clock** — `setCpuFrequencyMhz(80)` halves power vs 240 MHz with minimal performance impact for I/O-bound tasks.
7. **Disable unused peripherals** — Turn off ADC, DAC, Bluetooth, touch controllers you don't need:
   ```cpp
   esp_bt_controller_disable();
   adc_power_off();
   ```
8. **Fast WiFi reconnect** — Cache channel + BSSID in RTC memory:
   ```cpp
   RTC_DATA_ATTR int32_t savedChannel = 0;
   RTC_DATA_ATTR uint8_t savedBSSID[6] = {};
   // On reconnect: WiFi.begin(ssid, pass, savedChannel, savedBSSID, true);
   ```
9. **Use static IP** — Skip DHCP to save ~1 second per connection:
   ```cpp
   WiFi.config(staticIP, gateway, subnet, dns);
   ```
10. **Batch sensor readings** — Read sensors multiple times in ULP, store in RTC memory, wake main CPU only when the buffer is full.

> [!IMPORTANT]
> Always measure real current draw with a **multimeter** or dedicated tool like the **Nordic PPK2** or **INA219 breakout**. Datasheet values are theoretical; real boards behave differently.

---

## 📚 Resources

| Resource | Link |
|---|---|
| ESP-IDF Sleep Modes Docs | [docs.espressif.com](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/sleep_modes.html) |
| ESP32 Power Management | [docs.espressif.com](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/power_management.html) |
| ESP32 ULP Coprocessor | [docs.espressif.com](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/ulp.html) |
| TP4056 Datasheet | [NanJing Top Power](https://dlnmh9ip6v2uc.cloudfront.net/datasheets/Prototyping/TP4056.pdf) |
| Battery University | [batteryuniversity.com](https://batteryuniversity.com/) |

---

<p align="center">Made with ❤️ by <a href="https://github.com/tushar2704">Tushar Aggarwal</a> | 📺 <a href="https://youtube.com/@IamNotIntoTech">@IamNotIntoTech</a> + <a href="https://youtube.com/@Makerrdesigns">@Makerrdesigns</a></p>
