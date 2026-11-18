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
