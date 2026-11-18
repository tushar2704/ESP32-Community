# Project 02: Bluetooth A2DP Audio Sink (Hi-Fi Speaker)

Convert any vintage stereo, amplifier, or active loudspeaker into a wireless Bluetooth 5.0 speaker with crystal-clear 44.1 kHz / 16-bit I2S audio output.

---

## Overview

While basic microcontrollers lack the processing horsepower to stream audio, the ESP32 features dedicated hardware I2S (Inter-IC Sound) peripherals and a dual-core Xtensa LX6 architecture capable of decoding real-time Bluetooth A2DP (Advanced Audio Distribution Profile) SBC audio streams with zero stutter.

| Feature | Specification |
|---|---|
| **Difficulty** | Intermediate |
| **BOM Cost** | ~\$8–\$12 (ESP32 Dev Board + MAX98357A I2S DAC Module + Speaker) |
| **Audio Format** | Stereo 44.1 kHz, 16-bit PCM |
| **DAC Compatibility** | MAX98357A (mono 3W amp), PCM5102A (line out stereo), UDA1334A |
| **Bluetooth Profile** | A2DP Sink (Slave) |
| **Supported Chips** | ESP32 Classic (Bluetooth Classic required; S2/S3/C3/C6 lack A2DP Sink) |

---

## Wiring Diagram

### ESP32 to MAX98357A I2S Amplifier:
| MAX98357A Pin | ESP32 GPIO | Function |
|---|---|---|
| **LRC** | GPIO 25 | Word Select (Left/Right Clock) |
| **BCLK** | GPIO 26 | Bit Clock |
| **DIN** | GPIO 22 | Serial Audio Data In |
| **GAIN** | GND | Gain configuration (12dB) |
| **SD** | Not Connected | Shutdown / Channel Select |
| **VIN** | 5V / VIN | 3.3V to 5V (5V provides full 3.2W output) |
| **GND** | GND | Common Ground |

---

## Pairing Instructions

1. Power on the ESP32 connected to your amplifier or speaker.
2. Open Bluetooth Settings on your smartphone, tablet, or PC.
3. Select and pair with:
   - **Device Name:** `ESP32-Community-Speaker`
4. Play music from Spotify, Apple Music, YouTube, or your music library!

---

## Building and Flashing

### Using PlatformIO CLI
```bash
pio run -d projects/audio/02-bluetooth-speaker -e esp32dev -t upload
pio device monitor -d projects/audio/02-bluetooth-speaker
```

### Using Arduino IDE
1. Open `projects/audio/02-bluetooth-speaker/bluetooth-speaker.ino`.
2. Install **ESP32-A2DP** via Arduino Library Manager (by Phil Schatzmann).
3. Select **ESP32 Dev Module** under **Tools > Board**.
4. Upload and open Serial Monitor at **115200 baud**.

---

## Community & Credits

- Maintained by [ESP32 Community](https://github.com/tushar2704/ESP32-Community)
- Video guides & electronics builds: [@IamNotIntoTech](https://www.youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://www.youtube.com/@Makerrdesigns)
