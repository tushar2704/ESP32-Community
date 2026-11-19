# 🌟 Community Project & Build Showcase

> **A curated visual gallery of outstanding hardware projects, 3D printed enclosures, smart home controllers, and robotics built by the global ESP32 Community.**

---

## 📸 Featured Community Builds

### 1. Dual-Core FreeRTOS Asymmetric Multiprocessing OS
- **Creator**: Tushar Aggarwal / [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech)
- **Hardware**: ESP32 Classic Dual-Core (Xtensa LX6) + FreeRTOS
- **Architecture**: Core 0 dedicated to real-time sensor sampling, Core 1 executing web UI & network communications via thread-safe queues.
- **Project Link**: [`projects/advanced/24-freertos-dual-core-os/`](../projects/advanced/24-freertos-dual-core-os/)

### 2. Standalone Wi-Fi Robot Car & 4-DOF Web Robotic Arm
- **Hardware**: ESP32 + L298N Dual H-Bridge + 4x SG90/MG90S servos + Custom 3D Printed Chassis designed by [@Makerrdesigns](https://youtube.com/@Makerrdesigns).
- **Features**: Standalone SoftAP web server, zero-latency touch D-Pad and angle sliders with software servo easing.
- **Project Links**:
  - [`projects/robotics/01-wifi-robot-car/`](../projects/robotics/01-wifi-robot-car/)
  - [`projects/robotics/04-robotic-arm/`](../projects/robotics/04-robotic-arm/)
  - [`projects/robotics/02-line-follower/`](../projects/robotics/02-line-follower/)

### 3. ESP32-CAM High-Framerate Security Surveillance Camera
- **Hardware**: AI-Thinker ESP32-CAM (OV2640 2MP sensor) + Onboard Flash LED
- **Features**: Real-time MJPEG browser streaming, dynamic UX resolution/quality switching, motion detection trigger.
- **Project Link**: [`projects/esp32-cam/01-security-camera/`](../projects/esp32-cam/01-security-camera/)

### 4. Decentralized Zero-Infrastructure ESP-NOW Sensor Mesh
- **Hardware**: 2+ ESP32 boards communicating over peer-to-peer 2.4GHz raw radio frames.
- **Features**: Broadcast telemetry without a central Wi-Fi router or access point, automated node discovery.
- **Project Link**: [`projects/advanced/04-esp-now-mesh/`](../projects/advanced/04-esp-now-mesh/)

### 5. Wi-Fi Deauthentication Detector & WiGLE Wardriver
- **Hardware**: ESP32 DevKit in 802.11 Promiscuous Sniffer Mode
- **Features**: Active detection of 0xC0 deauth and 0xA0 disassociation frames with LED alarm; automated 2.4GHz RF survey logging to WiGLE CSV format.
- **Project Links**:
  - [`projects/security/01-wifi-deauth-detector/`](../projects/security/01-wifi-deauth-detector/)
  - [`projects/security/02-ble-scanner/`](../projects/security/02-ble-scanner/)
  - [`projects/security/03-wardriver/`](../projects/security/03-wardriver/)

### 6. Hi-Fi Bluetooth A2DP Wireless Speaker & Internet Radio
- **Hardware**: ESP32 + MAX98357A I2S DAC amplifier + 3W 4Ω full-range speaker.
- **Features**: Lossless 44.1 kHz 16-bit audio streaming from smartphones and worldwide MP3/AAC internet radio station playback.
- **Project Links**:
  - [`projects/audio/02-bluetooth-speaker/`](../projects/audio/02-bluetooth-speaker/)
  - [`projects/intermediate/11-internet-radio/`](../projects/intermediate/11-internet-radio/)

### 7. Retro OLED Arcade Suite (Pong & Snake with NVS High Score)
- **Hardware**: ESP32 + 0.96" SSD1306 OLED (128x64) + dual tactile buttons + piezo buzzer.
- **Features**: 60 FPS 2D physics engine, adaptive AI opponent paddle, and flash non-volatile memory score preservation.
- **Project Links**:
  - [`projects/games/01-pong/`](../projects/games/01-pong/)
  - [`projects/games/02-snake/`](../projects/games/02-snake/)

### 8. Smart Home Automation Suite (Garage Door, Energy Monitor & WS2812B)
- **Hardware**: ESP32 + Relays + SCT-013 AC Current Clamp + WS2812B NeoPixel Strip.
- **Features**: Animated web dashboard roll-up garage door with magnetic reed tracking, non-invasive True RMS power & kWh consumption metering, and multi-mode 16.7M color NeoPixel controller.
- **Project Links**:
  - [`projects/smart-home/03-garage-door/`](../projects/smart-home/03-garage-door/)
  - [`projects/smart-home/07-energy-monitor/`](../projects/smart-home/07-energy-monitor/)
  - [`projects/smart-home/09-led-strip-controller/`](../projects/smart-home/09-led-strip-controller/)

### 9. ESP32-S3 Native USB HID Macro Pad & Pocket WiFi Signal Mapper
- **Hardware**: ESP32-S3 native USB-OTG + EC11 rotary encoder + mechanical hotkeys + SSD1306 OLED.
- **Features**: Driverless hardware USB media control with push-to-mute and dev hotkeys; real-time 2.4GHz spectrum crowding analyzer with acoustic Geiger signal locator.
- **Project Links**:
  - [`projects/advanced/08-usb-hid/`](../projects/advanced/08-usb-hid/)
  - [`projects/beginner/11-wifi-signal-mapper/`](../projects/beginner/11-wifi-signal-mapper/)

### 10. Smart Plant Watering & Water Leak Prevention Suite
- **Hardware**: ESP32 + Capacitive Soil Moisture Sensor v1.2 + 5V Submersible Relay Pump + Multi-zone Rain/Conductivity Water Sensors + Solenoid Valve.
- **Features**: Automatic soil moisture threshold pump actuation with dry-run safety lockouts; sub-second multi-probe water leak sensing with audible alarm, emergency shutoff solenoid, and web dashboard telemetry.
- **Project Links**:
  - [`projects/intermediate/14-smart-plant-watering/`](../projects/intermediate/14-smart-plant-watering/)
  - [`projects/smart-home/13-water-leak-detector/`](../projects/smart-home/13-water-leak-detector/)

### 11. MicroSD High-Speed Telemetry Logger & PN532 NFC Access System
- **Hardware**: ESP32 + SPI MicroSD Module + PN532 NFC RFID Reader/Writer + Status LEDs.
- **Features**: High-frequency buffered CSV sensor logging with fail-safe file sync; MIFARE Classic and Ultralight NFC UID authorization database with NDEF message parsing.
- **Project Links**:
  - [`projects/intermediate/09-data-logger/`](../projects/intermediate/09-data-logger/)
  - [`projects/advanced/31-nfc-reader/`](../projects/advanced/31-nfc-reader/)

### 12. ESP32-CAM Time-Lapse & Native ESPHome Integration
- **Hardware**: AI-Thinker ESP32-CAM + MicroSD card + ESP32 DevKit with DHT22 & Relay.
- **Features**: Interval-based JPEG image capture with auto-incrementing file indexing directly to FAT32 SD card; plug-and-play Home Assistant auto-discovery via ESPHome native API.
- **Project Links**:
  - [`projects/esp32-cam/04-time-lapse/`](../projects/esp32-cam/04-time-lapse/)
  - [`projects/smart-home/01-esphome-basics/`](../projects/smart-home/01-esphome-basics/)

---

## 🎮 Interactive Wokwi Browser Simulations

No hardware on hand? You can simulate select community projects directly in your browser with zero setup:
- **Hello ESP32**: [Simulate in Wokwi](https://wokwi.com/) with serial console and status LED.
- **Blink RGB**: [Simulate in Wokwi](https://wokwi.com/) with multi-channel LEDC smooth color cycling.
- **Retro Pong**: [Simulate in Wokwi](https://wokwi.com/) with dual-button controls and 60 FPS SSD1306 OLED display.

Each simulated project includes `diagram.json` and `wokwi.toml` pre-configured in its project folder.

---

## 🎨 Submit Your Build to the Showcase

We want to feature your unique creations! To submit:
1. Open an issue or pull request using the **[Community Project Submission Template](https://github.com/tushar2704/ESP32-Community/issues/new?template=project_submission.md)**.
2. Include 2–4 high-resolution photos or a video clip showing your build in action.
3. List the bill of materials (BOM), wiring diagram, and firmware repository.
4. If you designed a 3D printable case, share the STL/STEP files in the `enclosures/` directory!

---

<div align="center">

**Part of the [ESP32 Community Hub](https://github.com/tushar2704/ESP32-Community)**  
<sub>Maintained by Tushar Aggarwal ([@tushar2704](https://github.com/tushar2704)) • Videos on [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech) & [@Makerrdesigns](https://youtube.com/@Makerrdesigns)</sub>

</div>
