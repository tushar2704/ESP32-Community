# 🟡 Project INT-11: Wi-Fi Internet Radio & Web Streamer

> Stream live online Shoutcast/Icecast MP3 radio stations directly over Wi-Fi. Features 16-bit I2S digital audio output to a MAX98357A or PCM5102A DAC amplifier, volume control, station presets, and buffer health tracking.

---

## 📋 Overview
- **Category**: Intermediate (⭐⭐⭐ 3/5)
- **Supported Boards**: Classic ESP32, ESP32-S3
- **Primary Goals**:
  1. Connect to high-bitrate Shoutcast/Icecast HTTP audio streams over local Wi-Fi.
  2. Stream decoded PCM audio via the ESP32 hardware I2S DMA controller without audio stutter or frame drops.
  3. Support multiple pre-configured online radio stations with Serial console switching.

---

## ⚡ Hardware Connections (I2S DAC - MAX98357A)

```text
ESP32 VIN (5V) ────────► MAX98357A VIN
ESP32 GND      ────────► MAX98357A GND
ESP32 GPIO 25  ────────► MAX98357A LRC  (Word Select)
ESP32 GPIO 26  ────────► MAX98357A BCLK (Bit Clock)
ESP32 GPIO 22  ────────► MAX98357A DIN  (Data Input)
```

---

## 📺 Video Tutorial & Enclosures
- 🎬 **Watch the Tutorial**: [@IamNotIntoTech on YouTube](https://youtube.com/@IamNotIntoTech)
- 🖨️ **3D Vintage Retro Radio Enclosure**: [@Makerrdesigns on YouTube](https://youtube.com/@Makerrdesigns)

---

## 📜 License
Licensed under GNU AGPLv3. See [LICENSE](../../../LICENSE) for details.
