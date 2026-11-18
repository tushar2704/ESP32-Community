# 🔧 ESP32 Troubleshooting Guide

> **Solutions to the most common ESP32 problems — upload failures, WiFi issues, brownout detectors, Guru Meditation crashes, and more. Organized by symptom so you can find fixes fast.**

---

## 📑 Table of Contents

- [Upload / Flashing Problems](#-upload--flashing-problems)
  - [Upload Fails — "Failed to connect"](#upload-fails--failed-to-connect)
  - [COM Port Not Detected](#com-port-not-detected)
  - [Wrong Board Selected](#wrong-board-selected)
- [Display Problems](#-display-problems)
  - [White / Blank Screen](#white--blank-screen)
  - [Display Flickering or Garbled](#display-flickering-or-garbled)
- [WiFi Problems](#-wifi-problems)
  - [WiFi Won't Connect](#wifi-wont-connect)
  - [WiFi Disconnects Randomly](#wifi-disconnects-randomly)
  - [WiFi Scan Finds No Networks](#wifi-scan-finds-no-networks)
- [BLE Problems](#-ble-problems)
  - [BLE Device Not Discoverable](#ble-device-not-discoverable)
  - [BLE Connection Drops](#ble-connection-drops)
- [Power & Boot Problems](#-power--boot-problems)
  - [Brownout Detector Triggered](#brownout-detector-triggered)
  - [Boot Loop / Constant Resets](#boot-loop--constant-resets)
- [Crash & Panic Errors](#-crash--panic-errors)
  - [Guru Meditation Error](#guru-meditation-error)
  - [Stack Overflow (Stack Canary)](#stack-overflow-stack-canary)
  - [Watchdog Timer (WDT) Reset](#watchdog-timer-wdt-reset)
  - [LoadProhibited / StoreProhibited](#loadprohibited--storeprohibited)
- [Flash & Security Problems](#-flash--security-problems)
  - [Flash Encryption Locked](#flash-encryption-locked)
  - [Secure Boot Bricked](#secure-boot-bricked)
- [Chip-Specific Issues](#-chip-specific-issues)
  - [ESP32 (Classic)](#esp32-classic)
  - [ESP32-S2](#esp32-s2)
  - [ESP32-S3](#esp32-s3)
  - [ESP32-C3](#esp32-c3)
  - [ESP32-C6](#esp32-c6)
  - [ESP32-CAM](#esp32-cam)
- [Useful Diagnostic Commands](#-useful-diagnostic-commands)
- [Resources](#-resources)

---

## 📤 Upload / Flashing Problems

### Upload Fails — "Failed to connect"

**Symptoms:**
```
A fatal error occurred: Failed to connect to ESP32: No serial data received.
```
or
```
A fatal error occurred: Failed to connect to ESP32: Timed out waiting for packet header
```

**Solutions (try in order):**

| # | Fix | Details |
|---|---|---|
| 1 | **Hold BOOT button** | Hold the **BOOT** (GPIO 0) button while clicking Upload. Release after "Connecting..." appears. This is the #1 fix. |
| 2 | **Press BOOT + EN sequence** | Hold BOOT → press EN (reset) → release EN → release BOOT. This manually enters download mode. |
| 3 | **Use correct USB port** | Some boards have two USB ports — use the one labeled **UART** or **COM**, not the one labeled **USB** or **OTG**. |
| 4 | **Reduce upload speed** | Lower to `115200` in Arduino IDE (Tools → Upload Speed) or in `platformio.ini`: `upload_speed = 115200` |
| 5 | **Disconnect GPIO 0, 2, 12** | These are strapping pins. Any external connections can interfere with boot mode. Remove wires during upload. |
| 6 | **Install USB drivers** | See [COM Port Not Detected](#com-port-not-detected) below. |
| 7 | **Try another USB cable** | Many cables are charge-only (no data). Use a known data-capable cable. |
| 8 | **Add a 10 µF cap on EN** | Some cheap boards have no auto-reset circuit. A capacitor between EN and GND (via DTR) fixes it. |

> [!TIP]
> **ESP32-S3 and ESP32-C3** boards with native USB: if you've flashed bad code that crashes the USB stack, hold BOOT, plug in USB, then release BOOT to enter ROM download mode.

### COM Port Not Detected

Your computer doesn't see the ESP32 at all — no COM port in Device Manager / `/dev/tty*`.

| USB-UART Chip | Driver Download |
|---|---|
| **CP2102 / CP2104** (Silicon Labs) | [silabs.com/developers/usb-to-uart-bridge-vcp-drivers](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers) |
| **CH340 / CH9102** (WCH) | [wch-ic.com/downloads/CH341SER_EXE.html](https://www.wch-ic.com/downloads/CH341SER_EXE.html) |
| **FTDI FT232** | [ftdichip.com/drivers/vcp-drivers](https://ftdichip.com/drivers/vcp-drivers/) |
| **Native USB** (S2/S3/C3/C6) | No driver needed — uses built-in USB-CDC |

> [!WARNING]
> On **macOS Sonoma+**, the CH340 driver may require allowing a kernel extension in System Preferences → Privacy & Security. Restart after installing.

### Wrong Board Selected

**Symptom:** Upload succeeds but board doesn't work correctly, or upload fails with size errors.

Always match the **exact board** in your IDE:
- Arduino IDE: Tools → Board → `ESP32 Dev Module`, `ESP32-S3 Dev Module`, etc.
- PlatformIO: `board = esp32dev`, `board = esp32-s3-devkitc-1`, etc.

> [!IMPORTANT]
> Using the wrong board definition can set incorrect flash size, partition scheme, PSRAM, or clock settings — causing crashes or missing functionality.

---

## 🖥️ Display Problems

### White / Blank Screen

**The #1 cause is using the wrong display driver.**

| Display | Correct Driver | Wrong Driver (common mistake) |
|---|---|---|
| ILI9341 (320×240) | `TFT_eSPI` with `ILI9341` | `ST7789` driver |
| ST7789 (240×240/135×240) | `TFT_eSPI` with `ST7789_DRIVER` | `ILI9341` driver |
| ST7735 (128×160) | `TFT_eSPI` with `ST7735_DRIVER` | Any other driver |
| SSD1306 (128×64 OLED) | `Adafruit_SSD1306` or `U8g2` | Wrong I²C address |
| ILI9488 (480×320) | `TFT_eSPI` with `ILI9488_DRIVER` | `ILI9341` driver |

**Checklist:**

1. ✅ Correct driver selected in `User_Setup.h` (for TFT_eSPI)
2. ✅ Correct **pin assignments** (MOSI, SCLK, CS, DC, RST, BL)
3. ✅ **Backlight pin** is set HIGH (many displays need `digitalWrite(BL_PIN, HIGH)`)
4. ✅ Correct **SPI frequency** (try 20 MHz if 40 MHz doesn't work)
5. ✅ Correct **color order** (RGB vs BGR — if colors are wrong but display works)

> [!TIP]
> For the **Cheap Yellow Display (CYD / ESP32-2432S028R)**, use the dedicated `User_Setup` from the CYD community repository — the pin mapping is non-standard.

### Display Flickering or Garbled

- **Lower SPI speed**: Try `#define SPI_FREQUENCY 20000000` instead of 40 MHz
- **Check wiring**: Loose jumper wires cause intermittent artifacts
- **Add decoupling capacitors**: 100 nF close to the display's VCC pin
- **Use proper ground**: Shared GND between ESP32 and display is essential

---

## 📶 WiFi Problems

### WiFi Won't Connect

```cpp
// Diagnostic sketch — upload this first
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Scanning...");
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) {
    Serial.printf("%d: %s (%d dBm) %s\n", i, WiFi.SSID(i).c_str(),
      WiFi.RSSI(i), WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "Open" : "Encrypted");
  }

  WiFi.begin("YourSSID", "YourPassword");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(1000);
    Serial.printf("Status: %d\n", WiFi.status());
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("Connected! IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("FAILED to connect.");
  }
}

void loop() {}
```

**Common fixes:**

| # | Issue | Solution |
|---|---|---|
| 1 | **5 GHz network** | ESP32 only supports **2.4 GHz**. Make sure your router has a 2.4 GHz band enabled. |
| 2 | **Wrong password** | Passwords are case-sensitive. Double-check for trailing spaces. |
| 3 | **Special characters in SSID** | Avoid emojis or unicode in your network name. |
| 4 | **MAC filtering** | Add the ESP32's MAC to your router's allow list: `Serial.println(WiFi.macAddress());` |
| 5 | **Router too far** | RSSI below **-80 dBm** is unreliable. Move closer or add an external antenna. |
| 6 | **Too many devices** | Some cheap routers can't handle 20+ WiFi clients. |
| 7 | **WPA3-only network** | Older ESP32 firmware doesn't support WPA3. Update arduino-esp32 or switch router to WPA2. |

### WiFi Disconnects Randomly

```cpp
WiFi.setAutoReconnect(true);
WiFi.persistent(true);

// Also monitor connection with events:
WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("WiFi disconnected! Reconnecting...");
  WiFi.begin("SSID", "password");
}, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
```

> [!TIP]
> If WiFi drops coincide with high current draws (servo, motor, relay), it's a **power supply issue**. Add a 470–1000 µF electrolytic capacitor near VCC, or use a separate power supply for high-current loads.

### WiFi Scan Finds No Networks

- **Antenna issue**: Check the antenna connection. Some modules have an IPEX connector — make sure the external antenna is attached OR the PCB antenna trace is selected (usually via a 0-ohm resistor).
- **RF interference**: USB 3.0 hubs are notorious for interfering with 2.4 GHz. Move the ESP32 away from USB 3.0 devices.
- **Firmware bug**: Try updating your ESP32 board package to the latest version.

---

## 🔵 BLE Problems

### BLE Device Not Discoverable

| # | Check | Fix |
|---|---|---|
| 1 | Is advertising started? | Ensure `pAdvertising->start()` is called after service setup |
| 2 | Phone Bluetooth on? | Also check that the phone app supports BLE (not just Classic BT) |
| 3 | Name too long? | BLE advertising name is limited to ~29 bytes. Keep it short. |
| 4 | Service UUID in advertisement? | Add it: `pAdvertising->addServiceUUID(SERVICE_UUID)` |
| 5 | Running on ESP32-S2? | **ESP32-S2 has NO Bluetooth at all.** Use ESP32, S3, C3, C6, or H2. |

### BLE Connection Drops

- **Increase connection interval**: Faster intervals = more power, more instability
- **Reduce MTU if needed**: `BLEDevice::setMTU(185)` — some phones struggle with 512
- **Avoid WiFi + BLE simultaneously** on the original ESP32 — they share the radio and can interfere

> [!WARNING]
> On the original ESP32, WiFi and BLE share the same 2.4 GHz antenna. Running both at high throughput simultaneously causes packet loss. The ESP32-S3, C3, and C6 handle coexistence better.

---

## ⚡ Power & Boot Problems

### Brownout Detector Triggered

**Symptom:**
```
Brownout detector was triggered
```

This means the input voltage dropped below the safe threshold (~2.4V for ESP32).

**Causes & Fixes:**

| Cause | Fix |
|---|---|
| Weak USB port | Use a powered USB hub or wall adapter (2A+) |
| Long/thin USB cable | Use a short, thick cable (< 1m, 24AWG) |
| Power-hungry peripherals | Power servos/motors from a separate supply, not the board's 3.3V/5V pin |
| WiFi TX spike | Add a **470 µF** capacitor between VCC and GND close to the ESP32 |
| Bad LDO on cheap boards | Replace AMS1117 with ME6211 or HT7833 |

> [!CAUTION]
> Persistent brownouts can corrupt flash memory and cause permanent boot loops. Fix the power supply issue immediately.

### Boot Loop / Constant Resets

If your ESP32 keeps resetting every few seconds:

1. **Check the serial output** — it usually prints the reset reason before crashing
2. **Erase flash completely**: `esptool.py --chip esp32 erase_flash`
3. **Check GPIO 12 (MTDI)** — On original ESP32, this pin sets flash voltage at boot. If pulled HIGH with an external circuit, the wrong flash voltage is selected, causing a crash.
4. **Remove external wiring** — Strip all connections and test with a basic blink sketch

```bash
# Check reset reason in ESP-IDF
idf.py monitor
# Look for: rst:0x1 (POWERON_RESET), rst:0xc (SW_CPU_RESET), etc.
```

---

## 💥 Crash & Panic Errors

### Guru Meditation Error

```
Guru Meditation Error: Core  0 panic'ed (IllegalInstruction). Exception was unhandled.
```

This is the ESP32's crash handler. The key information is:

1. **Panic reason** — `IllegalInstruction`, `LoadProhibited`, `StoreProhibited`, `IntegerDivideByZero`
2. **Core** — Which CPU core crashed (0 or 1)
3. **Backtrace** — Memory addresses of the call stack

**Decode the backtrace** using the ESP Exception Decoder:
- Arduino IDE: Install "ESP Exception Decoder" from Tools menu
- PlatformIO: `pio run -t monitor` auto-decodes
- Manual: `xtensa-esp32-elf-addr2line -pfiaC -e firmware.elf 0x400d1234 0x400d5678`

| Panic Reason | Common Cause | Fix |
|---|---|---|
| `IllegalInstruction` | Corrupted firmware, bad pointer jump | Erase flash, re-upload |
| `LoadProhibited` | Null pointer dereference (read) | Check all pointer initializations |
| `StoreProhibited` | Null pointer dereference (write) | Check array bounds, pointer validity |
| `IntegerDivideByZero` | Division by zero | Add zero-checks before division |
| `InstrFetchProhibited` | Jump to invalid memory | Stack corruption, buffer overflow |
| `Unhandled debug exception` | Watchpoint/breakpoint triggered | Remove hardware breakpoints |

### Stack Overflow (Stack Canary)

```
***ERROR*** A stack overflow in task "main" has been detected.
```

The default Arduino `loop()` task has only **8 KB** of stack. Complex operations (deep recursion, large local arrays, JSON parsing) can overflow it.

**Fixes:**

```cpp
// Option 1: Increase Arduino loop stack (in platformio.ini)
// build_flags = -DARDUINO_LOOP_STACK_SIZE=16384

// Option 2: Create a task with a larger stack
void heavyTask(void *param) {
  // Your memory-hungry code here
  vTaskDelete(NULL);
}

void setup() {
  xTaskCreatePinnedToCore(heavyTask, "heavy", 32768, NULL, 1, NULL, 1);
}
```

> [!TIP]
> Monitor stack usage with `uxTaskGetStackHighWaterMark(NULL)` — it returns the minimum free stack bytes ever. If it's below 500, you need a bigger stack.

### Watchdog Timer (WDT) Reset

```
Task watchdog got triggered. The following tasks did not reset the watchdog in time: ...
E (12345) task_wdt: - IDLE0 (CPU 0)
```

The watchdog resets the chip when a task hogs the CPU for too long without yielding.

**Fixes:**

```cpp
// In long loops, yield to the system:
for (int i = 0; i < 1000000; i++) {
  // Intensive work
  if (i % 1000 == 0) {
    vTaskDelay(1);  // Let other tasks run
    // or: yield();  // Arduino equivalent
  }
}
```

| Scenario | Solution |
|---|---|
| Long computation loop | Add `vTaskDelay(1)` or `yield()` every ~100ms |
| Blocking I/O | Use async/non-blocking alternatives |
| Tight `while(true)` without delay | Always include a small delay or yield |
| Interrupt handler too long | Move heavy work to a task, signal from ISR |

### LoadProhibited / StoreProhibited

This almost always means a **null or invalid pointer**.

```cpp
// BAD — pointer never initialized
char *buffer;
buffer[0] = 'A';  // StoreProhibited!

// GOOD
char *buffer = (char*)malloc(100);
if (buffer != NULL) {
  buffer[0] = 'A';  // Safe
}
```

Also check: use-after-free, accessing arrays out of bounds, and forgetting to initialize objects.

---

## 🔒 Flash & Security Problems

### Flash Encryption Locked

> [!CAUTION]
> Flash encryption in **Release mode** is **permanent and irreversible**. Once all FLASH_CRYPT_CNT eFuse bits are burned, you **cannot re-flash** the device in plaintext. The chip can only boot encrypted firmware signed with the original key.

**Prevention:**
- Always use **Development mode** while prototyping: `idf.py menuconfig` → Security → Flash Encryption → Development
- In Development mode, you can still re-flash by decrementing the counter

**If already locked:**
- The only option is to flash new firmware **encrypted with the same key**
- If the key is lost, the chip is permanently bricked — it becomes e-waste

### Secure Boot Bricked

Similarly, **Secure Boot v2** in Release mode permanently burns eFuses. If you lose the signing key, the device cannot boot any new firmware.

> [!IMPORTANT]
> **Back up your keys** (flash encryption key, secure boot signing key) in multiple secure locations before enabling production security features.

---

## 🔀 Chip-Specific Issues

### ESP32 (Classic)

| Issue | Details |
|---|---|
| **GPIO 12 flash voltage** | GPIO 12 (MTDI) sets flash voltage at boot. External pull-up = 1.8V flash (crash on 3.3V flash modules). Burn eFuse to fix: `espefuse.py set_flash_voltage 3.3V` |
| **GPIO 6-11 — DO NOT USE** | Connected to internal flash SPI. Using them crashes the chip instantly. |
| **PSRAM and GPIO 16/17** | On modules with PSRAM (ESP32-WROVER), GPIO 16/17 are used by PSRAM. Do not use them for other purposes. |
| **ADC2 unusable with WiFi** | ADC2 pins cannot be used for analog reads when WiFi is active. Use ADC1 (GPIO 32-39) instead. |

### ESP32-S2

| Issue | Details |
|---|---|
| **No Bluetooth** | ESP32-S2 has **zero Bluetooth capability** — not BLE, not Classic. Use S3/C3 if you need BLE. |
| **Single core** | Only one Xtensa core. FreeRTOS tasks don't run in parallel. |
| **USB-OTG quirks** | Native USB can be temperamental. If upload fails, hold BOOT → plug USB → release BOOT. |

### ESP32-S3

| Issue | Details |
|---|---|
| **Two USB ports** | One is USB-UART (CH340/CP2102), the other is native USB-OTG. Use the **UART** port for reliable uploads. |
| **PSRAM config** | Must enable PSRAM in menuconfig/board config: `board_build.arduino.memory_type = qio_opi` for OPI PSRAM |
| **Large firmware** | With BLE + WiFi + TFT libraries, firmware can exceed the default 1.2 MB partition. Use `Huge APP (3 MB No OTA)` partition scheme. |

### ESP32-C3

| Issue | Details |
|---|---|
| **Single core RISC-V** | Only one core. CPU-bound tasks compete with WiFi/BLE stack. |
| **No analog on GPIO 0-4** | ADC channels are limited. Check the pinout before designing your circuit. |
| **USB-JTAG** | Native USB is USB-JTAG-Serial. Sometimes requires putting the chip in download mode manually: hold BOOT → press RESET → release BOOT. |

### ESP32-C6

| Issue | Details |
|---|---|
| **New chip, evolving support** | Arduino-ESP32 support is still maturing. Use ESP-IDF v5.1+ for full Thread/Zigbee/WiFi 6 features. |
| **Thread + WiFi coexistence** | Both radios work but require careful task prioritization and channel management. |

### ESP32-CAM

| Issue | Details |
|---|---|
| **No onboard USB** | Requires an external USB-to-serial adapter (FTDI/CP2102). Connect TX→RX, RX→TX, GND→GND. Use the **ESP32-CAM-MB** base board for a plug-and-play USB solution. |
| **GPIO 0 for boot mode** | Must be pulled **LOW** (connected to GND) during upload, then disconnected for normal boot. |
| **Limited GPIOs** | Most GPIOs are used by the camera. Only GPIO 2, 4, 12, 13, 14, 15, 33 are somewhat available, with caveats. |
| **Brownout on camera init** | Camera draws ~200 mA. Use a 5V / 2A power supply. Add a 470 µF capacitor. |
| **PSRAM required** | Frame sizes above SVGA (800×600) require PSRAM. Enable it in board settings. |
| **SD card conflicts** | The SD card and some GPIOs share pins. You often can't use the flash LED (GPIO 4) while using the SD card. |

---

## 🛠️ Useful Diagnostic Commands

```bash
# Check chip info
esptool.py chip_id

# Read flash size and type
esptool.py flash_id

# Erase entire flash (factory reset)
esptool.py --chip esp32 erase_flash

# Read eFuse values (check flash encryption, secure boot status)
espefuse.py summary

# Monitor serial output with timestamps
idf.py monitor
# or
python -m serial.tools.miniterm /dev/ttyUSB0 115200 --raw

# Decode backtrace addresses
xtensa-esp32-elf-addr2line -pfiaC -e .pio/build/esp32dev/firmware.elf 0x400d1234

# Check partition table
esptool.py read_flash 0x8000 0xC00 ptable.bin
gen_esp32part.py ptable.bin
```

> [!TIP]
> Add this to the top of your `setup()` for quick diagnostics:
> ```cpp
> Serial.printf("Chip: %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
> Serial.printf("CPU: %d MHz, Cores: %d\n", ESP.getCpuFreqMHz(), ESP.getChipCores());
> Serial.printf("Flash: %d MB, PSRAM: %d KB\n", ESP.getFlashChipSize()/1048576, ESP.getPsramSize()/1024);
> Serial.printf("Free Heap: %d, Min Heap: %d\n", ESP.getFreeHeap(), ESP.getMinFreeHeap());
> Serial.printf("Reset Reason: %d\n", esp_reset_reason());
> ```

---

## 📚 Resources

| Resource | Link |
|---|---|
| ESP32 Datasheet | [espressif.com](https://www.espressif.com/en/support/documents/technical-documents) |
| ESP-IDF Error Handling | [docs.espressif.com](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/esp_err.html) |
| esptool.py Documentation | [docs.espressif.com](https://docs.espressif.com/projects/esptool/en/latest/) |
| Espressif Technical Support | [espressif.com/en/contact-us](https://www.espressif.com/en/contact-us) |

---

<p align="center">Made with ❤️ by <a href="https://github.com/tushar2704">Tushar Aggarwal</a> | 📺 <a href="https://youtube.com/@IamNotIntoTech">@IamNotIntoTech</a> + <a href="https://youtube.com/@Makerrdesigns">@Makerrdesigns</a></p>
