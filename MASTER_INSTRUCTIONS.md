# 📋 MASTER INSTRUCTIONS — ESP32 Community

> **This file is for AI assistants, collaborators, and any automated tooling.**
> It defines the rules, conventions, and context needed to contribute to this repository.

---

## 🔐 Identity & Commit Rules

**ALL commits, pull requests, and authored content MUST use:**

```
Git Username: tushar2704
Git Email:    tushar.inseec@gmail.com
Full Name:    Tushar Aggarwal
GitHub:       https://github.com/tushar2704
```

**Configure git before any commit:**
```bash
git config user.name "tushar2704"
git config user.email "tushar.inseec@gmail.com"
```

**Never commit with any other identity.**

---

## 📺 YouTube Channels (ALWAYS Credit Both)

| Channel Handle | Channel Name | Focus |
|---------------|-------------|-------|
| `@IamNotIntoTech` | IamNotIntoTech | Tech projects with humor, ESP32 tutorials, IoT builds |
| `@Makerrdesigns` | Makerrdesigns | 3D design, 3D printing, enclosure designs for ESP32 boards |

**Links to include in every project README:**
```markdown
📺 **Tutorial**: [Watch on IamNotIntoTech](https://youtube.com/@IamNotIntoTech)
🖨️ **Enclosure**: [Watch on Makerrdesigns](https://youtube.com/@Makerrdesigns)
```

---

## 🏗️ Repository Context

### What This Repo Is
- The **#1 community hub for ALL ESP32 variants** — the most comprehensive ESP32 project collection on GitHub
- Covers the **entire Espressif ESP32 family**: ESP32 Classic, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C5, ESP32-C6, ESP32-H2, ESP32-P4
- Includes **ESP32-CAM** projects and camera-based builds
- Contains: projects, documentation, 3D enclosures, pin maps, Web Flasher, and community resources
- Supports **all major frameworks**: Arduino, PlatformIO, ESP-IDF, MicroPython, ESPHome

### What This Repo Is NOT
- NOT a fork or copy of any other repository
- NOT limited to a single ESP32 variant (we cover ALL)
- NOT affiliated with or crediting any other community project

---

## 📁 File & Folder Conventions

### Project Structure
Every project MUST follow this exact structure:
```
projects/{category}/{NN}-{project-slug}/
├── README.md            # REQUIRED: Description, screenshots, wiring, video link
├── {project-slug}.ino   # REQUIRED: Main Arduino sketch
├── platformio.ini       # REQUIRED: PlatformIO configuration
├── src/                 # Optional: Additional source files
│   └── main.cpp         # PlatformIO main (if different from .ino)
├── include/             # Optional: Header files
├── lib/                 # Optional: Local libraries
├── data/                # Optional: SPIFFS/LittleFS data
├── assets/              # Optional: Screenshots, demo GIFs
│   ├── screenshot.png
│   └── demo.gif
└── enclosure/           # Optional: 3D print files
    ├── case.stl
    ├── case.step
    └── README.md        # Print settings and assembly instructions
```

### Project Categories

| Folder | Focus Area | Description |
|--------|-----------|-------------|
| `beginner/` | Getting Started | No external components, basic code, first-time builds |
| `intermediate/` | Connected Projects | WiFi/API/MQTT, moderate complexity, sensor integrations |
| `advanced/` | Complex Systems | Multi-component, OS-level, AI/ML, mesh networks |
| `esp32-cam/` | Camera Projects | Security cameras, time-lapse, face detection, streaming |
| `esp32-s3/` | S3-Specific | USB Host/Device, TinyML, AI acceleration, RGB LED |
| `esp32-c3/` | C3-Specific | RISC-V projects, BLE-only, low-power applications |
| `esp32-c6/` | C6-Specific | Thread, Zigbee, Matter, WiFi 6 projects |
| `esp32-h2/` | H2-Specific | Thread/Zigbee border routers, 802.15.4 networking |
| `robotics/` | Robotics & Motion | Robot cars, drones, servo control, motor drivers |
| `smart-home/` | Home Automation | ESPHome, Home Assistant, Matter devices, voice control |
| `security/` | Security Tools | WiFi analysis, deauth detectors, secure boot, encryption |
| `audio/` | Audio & Sound | Internet radio, Bluetooth speakers, MIDI, audio processing |
| `games/` | Games & Fun | Retro games, emulators, game controllers, LED games |
| `industrial/` | Industrial IoT | Modbus, RS485, data acquisition, PLC alternatives |
| `wearables/` | Wearable Tech | Smartwatches, fitness trackers, wearable sensors |
| `displays/` | Display Projects | E-ink, OLED, TFT, LED matrix, HUB75 panels |
| `fun/` | Creative & Art | LED art, music visualizers, props, holiday decorations |

### Naming Conventions
- **Folders**: lowercase with hyphens (`weather-station`, not `WeatherStation`)
- **Arduino files**: lowercase with hyphens matching folder name
- **Documentation**: UPPERCASE with underscores (`SETUP.md`, `PINOUT.md`)
- **Images**: lowercase, descriptive (`esp32-s3-pinout.png`)

---

## 🏷️ ESP32 Chip Variant Reference

When writing code that supports multiple ESP32 variants, use these defines:

```cpp
// Define your target chip before including headers
// Uncomment ONE of the following:

// #define ESP32_CLASSIC       // ESP32-WROOM-32 / ESP32-DevKitC (Xtensa LX6, Dual-core)
// #define ESP32_S2            // ESP32-S2 (Xtensa LX7, Single-core, USB OTG)
// #define ESP32_S3            // ESP32-S3 (Xtensa LX7, Dual-core, AI/ML, USB OTG)
// #define ESP32_C3            // ESP32-C3 (RISC-V, Single-core, BLE 5.0)
// #define ESP32_C6            // ESP32-C6 (RISC-V, WiFi 6, Thread, Zigbee, Matter)
// #define ESP32_H2            // ESP32-H2 (RISC-V, Thread, Zigbee, BLE 5.0, NO WiFi)
// #define ESP32_P4            // ESP32-P4 (RISC-V, Dual-core, AI, MIPI-DSI/CSI, NO WiFi)
```

### Chip Quick Comparison

| Feature | Classic | S2 | S3 | C3 | C6 | H2 | P4 |
|---------|---------|----|----|----|----|----|----|
| Architecture | Xtensa LX6 | Xtensa LX7 | Xtensa LX7 | RISC-V | RISC-V | RISC-V | RISC-V |
| Cores | 2 | 1 | 2 | 1 | 1 | 1 | 2 |
| Max Clock | 240 MHz | 240 MHz | 240 MHz | 160 MHz | 160 MHz | 96 MHz | 400 MHz |
| WiFi | 4 | 4 | 4 | 4 | 6 | ❌ | ❌ |
| Bluetooth | 4.2 | ❌ | 5.0 | 5.0 | 5.0 | 5.0 | ❌ |
| Thread/Zigbee | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ | ❌ |
| USB Native | ❌ | OTG | OTG | ❌ | ❌ | ❌ | ✅ |
| AI/ML Accel. | ❌ | ❌ | ✅ | ❌ | ❌ | ❌ | ✅ |
| Best For | General | USB/LCD | AI/Camera | Low-cost BLE | Matter/Thread | Zigbee Hub | HMI/AI |

---

## 📝 Project README Template

Every project README.md MUST contain these sections:

```markdown
# 🚀 Project Name

> One-line description of what this project does.

![Screenshot or demo GIF](assets/screenshot.png)

## 📋 Overview
- What it does
- Which ESP32 chip(s) it supports
- Difficulty rating (⭐ to ⭐⭐⭐⭐⭐)

## 🎯 Supported Chips
| Chip | Tested | Notes |
|------|--------|-------|
| ESP32 Classic | ✅ | Primary target |
| ESP32-S3 | ✅ | Recommended |
| ESP32-C3 | ⚠️ | Limited GPIO |
| ESP32-C6 | ❌ | Not tested |

## 🔧 Requirements
### Hardware
- List of required hardware with purchase links

### Software / Libraries
- List of required libraries with links and versions

## 📺 Video Tutorial
- 🎬 [Watch the build on IamNotIntoTech](https://youtube.com/@IamNotIntoTech)
- 🖨️ [Enclosure design on Makerrdesigns](https://youtube.com/@Makerrdesigns)

## ⚡ Wiring / Pin Connections
| Component | ESP32 Pin | Notes |
|-----------|-----------|-------|
| LED       | GPIO 2    | Built-in |

## 🚀 Installation

### Arduino IDE
1. Step-by-step instructions

### PlatformIO
1. Step-by-step instructions

## 📸 Screenshots
Additional screenshots or GIFs

## 📜 License
AGPL-3.0 (code) | CC BY-NC-ND 4.0 (assets) — see [LICENSE](../../LICENSE)
```

---

## 🔍 SEO Rules

### README.md (Main)
- H1 MUST contain "ESP32"
- Use all ESP32 variant names naturally throughout (ESP32, ESP32-S3, ESP32-C3, ESP32-C6, etc.)
- Include chip model numbers for long-tail SEO
- Every section heading should contain a relevant keyword
- Include secondary keywords: "IoT projects", "Arduino", "PlatformIO", "smart home", "robotics"

### Target Keywords
Weave these naturally into headings, descriptions, and alt text:
```
esp32, esp32-s3, esp32-c3, esp32-c6, esp32-cam, esp32-projects,
arduino-esp32, platformio, iot-projects, smart-home, robotics,
tinyml, matter, thread, zigbee, esp-now, freertos, micropython,
esphome, home-assistant
```

### GitHub Repository Topics
Apply these topics/tags:
```
esp32, esp32-s3, esp32-c3, esp32-c6, esp32-cam, esp32-h2,
esp32-projects, arduino, platformio, esp-idf, micropython,
iot, smart-home, home-assistant, esphome, robotics, tinyml,
matter, thread, zigbee, freertos, maker, open-source
```

### Project READMEs
- Title should include "ESP32" and the specific chip variant
- Include the target chip and framework in the description
- Use descriptive alt text on all images

---

## 💰 Monetization Context

### Pricing Tiers
| Type | Price | Distribution |
|------|-------|-------------|
| Beginner projects | **Free** | GitHub (full source) |
| Intermediate projects | **$5** each | Gumroad (premium features) |
| Advanced projects | **$8–$10** each | Gumroad (premium features) |
| Enclosure design packs | **$10** | Gumroad |
| Project bundles | **$15–$45** | Gumroad |

### What "Sellable" Means
- **Free projects**: Full source code on GitHub, no restrictions — must be genuinely useful standalone
- **Paid projects**: Basic version on GitHub (free), premium version with extra features, enclosures, video walkthroughs, and advanced configurations on Gumroad
- **NEVER gate basic functionality** — the free version must be complete and functional
- **Premium adds**: polished enclosures, bonus features, priority support, extended tutorials

### Revenue Channels
- 🛒 **Gumroad**: Project packs, enclosure files, bundles
- ☕ **Buy Me a Coffee**: [buymeacoffee.com/iamnotintotech](https://buymeacoffee.com/iamnotintotech), [buymeacoffee.com/makerr](https://buymeacoffee.com/makerr)
- 🎖️ **Patreon**: [patreon.com/cw/IamNotIntoTech](https://patreon.com/cw/IamNotIntoTech)
- 💚 **Ko-fi**: [ko-fi.com/makerrdesigns](https://ko-fi.com/makerrdesigns)
- 💖 **GitHub Sponsors**: [github.com/sponsors/tushar2704](https://github.com/sponsors/tushar2704)

---

## ✅ Quality Checklist

Before committing any project:

- [ ] Code compiles without errors on target chip(s)
- [ ] Code tested on real hardware (or clearly marked as untested)
- [ ] README follows the project template above
- [ ] Both YouTube channels are credited
- [ ] Supported chips table is filled in accurately
- [ ] Screenshots/GIFs are included in `assets/`
- [ ] PlatformIO config is provided with correct board target
- [ ] Arduino .ino file is provided
- [ ] License header or reference is included
- [ ] SEO keywords are present in README headings
- [ ] Git author is `tushar2704` / `tushar.inseec@gmail.com`
- [ ] Pin assignments are documented and chip-compatible
- [ ] No hardcoded WiFi credentials in committed code
- [ ] platformio.ini specifies correct `board` and `framework`
- [ ] Any required library versions are pinned

---

## 🔄 Commit Message Convention

```
type(scope): description

Examples:
feat(projects): add ESP32-S3 weather station project
docs(setup): update Arduino IDE installation steps for ESP32-C6
fix(wifi-scanner): correct channel hopping on ESP32-C3
chore(ci): add GitHub Actions compile workflow for all chips
style(readme): improve SEO headings and keyword density
feat(enclosure): add snap-fit case for ESP32-DevKitC
refactor(mqtt): extract common MQTT handler to shared lib
test(ota): add OTA update integration test
```

### Types
| Type | Purpose |
|------|---------|
| `feat` | New feature or project |
| `fix` | Bug fix |
| `docs` | Documentation only changes |
| `style` | Formatting, missing semicolons, etc. (no code change) |
| `refactor` | Code refactoring (no feature change) |
| `test` | Adding or updating tests |
| `chore` | Build process, CI, tooling changes |
| `perf` | Performance improvements |

### Scopes
Use the project slug, category name, or general area:
```
projects, docs, readme, ci, enclosure, web-flasher,
beginner, intermediate, advanced, esp32-cam, smart-home,
robotics, security, audio, games, {project-slug}
```

---

## 🌐 Framework Support

### Arduino IDE
- Board Manager URL: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
- Select the correct board for your chip variant
- Use Arduino 2.x or later for ESP32-C6/H2 support

### PlatformIO
- Framework: `arduino` or `espidf`
- Platform: `espressif32`
- Specify the correct `board` in `platformio.ini`

### ESP-IDF
- Use ESP-IDF v5.x for full chip family support
- Follow Espressif's project structure conventions
- Include `sdkconfig.defaults` in project

### MicroPython
- Use official Espressif MicroPython builds
- Include `boot.py` and `main.py`
- Document required MicroPython version

---

## 🔒 Security Rules

- **NEVER** commit WiFi passwords, API keys, or secrets
- Use `secrets.h` or `config.h` with `.gitignore`
- Provide a `secrets.h.example` template
- Enable Secure Boot where applicable
- Document any network-facing services and their security implications

---

*Last updated: 2026 | Maintainer: [tushar2704](https://github.com/tushar2704) | ESP32 Community*
