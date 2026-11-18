# Project 15: ArduinoOTA Wireless Firmware Updates

Flash and upgrade your ESP32 boards wirelessly across your local network without connecting a physical USB cable.

---

## Overview

Once an ESP32 is deployed in an enclosure, embedded in a wall, or mounted in a robot, plugging in a USB cable becomes impractical. ArduinoOTA allows wireless firmware uploads via Wi-Fi with password authentication and live progress reporting.

| Feature | Specification |
|---|---|
| **Difficulty** | Beginner |
| **BOM Cost** | \$0 (ESP32 Board) |
| **Network Protocol** | UDP / TCP (ArduinoOTA) + mDNS (`esp32-ota.local`) |
| **Flashing Speed** | ~100–250 KB/s over 2.4GHz Wi-Fi |
| **Supported Chips** | ESP32 Classic, S2, S3, C3, C6 |

---

## How It Works

1. The initial flash **must be done once via USB** to load the OTA bootloader and sketch.
2. Once booted and connected to Wi-Fi, the ESP32 broadcasts an mDNS service advert.
3. In Arduino IDE or PlatformIO, the board appears as a network port:
   - `esp32-ota at 192.168.x.x`
4. When you click Upload, the toolchain pushes the new binary over Wi-Fi directly to the flash partition and restarts into the updated code.

---

## Flashing Wirelessly via PlatformIO

To upload your subsequent builds over the air, simply specify the upload protocol and IP or hostname in `platformio.ini`:

```ini
[env:esp32dev_ota]
platform = espressif32
board = esp32dev
framework = arduino
upload_protocol = espota
upload_port = esp32-ota.local
upload_flags =
    --auth=admin
```

Then run:
```bash
pio run -e esp32dev_ota -t upload
```

---

## Flashing Wirelessly via Arduino IDE

1. Ensure your PC and the ESP32 are on the **same Wi-Fi network**.
2. Go to **Tools > Port**.
3. Under **Network Ports**, select `esp32-ota at 192.168.x.x`.
4. Click **Upload**. If prompted for password, enter `admin`.

---

## Community & Credits

- Maintained by [ESP32 Community](https://github.com/tushar2704/ESP32-Community)
- Video guides & electronics builds: [@IamNotIntoTech](https://www.youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://www.youtube.com/@Makerrdesigns)
