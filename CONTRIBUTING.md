# 🤝 Contributing to ESP32 Community

First off, **thank you** for considering contributing to the ESP32 Community! Whether you're sharing a project, fixing a bug, improving documentation, designing a 3D enclosure, or translating content — every contribution makes this the best ESP32 resource on the planet.

---

## 📋 Table of Contents

- [Ways to Contribute](#ways-to-contribute)
- [Submission Paths](#submission-paths)
  - [Arduino IDE Projects](#arduino-ide-projects)
  - [PlatformIO Projects](#platformio-projects)
  - [ESP-IDF Projects](#esp-idf-projects)
  - [MicroPython Projects](#micropython-projects)
- [Project Structure](#project-structure)
- [Cross-Chip Testing Matrix](#cross-chip-testing-matrix)
- [Web Flasher Firmware Submission](#web-flasher-firmware-submission)
- [3D Enclosure Submissions](#3d-enclosure-submissions)
- [Translation Contributions](#translation-contributions)
- [Reporting Bugs](#reporting-bugs)
- [Improving Documentation](#improving-documentation)
- [Discussion & Community Engagement](#discussion--community-engagement)
- [Code Style Guidelines](#code-style-guidelines)
- [Pull Request Process](#pull-request-process)
- [Recognition](#recognition)

---

## Ways to Contribute

| Type | Description | Difficulty |
|------|-------------|------------|
| 🐛 Bug Report | Found a bug? Let us know! | Easy |
| 📝 Documentation | Fix typos, improve guides, add examples | Easy |
| 🌍 Translation | Translate docs and READMEs to other languages | Easy–Medium |
| 💬 Community | Answer questions, help newcomers in Discussions | Easy |
| 🔧 Arduino Project | Submit a new project using the Arduino framework | Medium |
| 🛠️ PlatformIO Project | Submit a PlatformIO-based project | Medium |
| ⚙️ ESP-IDF Project | Submit a project using the ESP-IDF framework | Hard |
| 🐍 MicroPython Project | Submit a MicroPython-based project | Medium |
| 🖨️ Enclosure | Design a 3D printable case for an ESP32 board | Medium |
| 📦 Web Flasher Firmware | Submit pre-compiled firmware for the Web Flasher | Hard |
| 💡 Feature Request | Suggest a new project or feature | Easy |

---

## Submission Paths

### Arduino IDE Projects

The most common and beginner-friendly path.

**Requirements:**
- Main `.ino` sketch file (name matches project folder)
- Compiles with Arduino IDE 2.x and ESP32 Board Package v2.x+
- Specify required board selection in README
- List all library dependencies with version numbers

**Board Manager URL:**
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

### PlatformIO Projects

Preferred for reproducible builds and dependency management.

**Requirements:**
- `platformio.ini` with correct `board`, `framework`, and `lib_deps`
- `src/main.cpp` (can coexist with `.ino`)
- Pin all library versions in `lib_deps`
- Use `platform = espressif32`

**Example `platformio.ini`:**
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps =
    bblanchon/ArduinoJson@^7.0.0
    knolleary/PubSubClient@^2.8
monitor_speed = 115200
```

### ESP-IDF Projects

For advanced projects using the Espressif IoT Development Framework.

**Requirements:**
- Follow ESP-IDF project structure (`main/`, `components/`, `CMakeLists.txt`)
- Include `sdkconfig.defaults` (not full `sdkconfig`)
- Specify minimum ESP-IDF version in README
- Document any `menuconfig` settings that must be changed

**Structure:**
```
projects/advanced/{NN}-{slug}/
├── README.md
├── CMakeLists.txt
├── sdkconfig.defaults
├── main/
│   ├── CMakeLists.txt
│   └── main.c
├── components/        # Optional custom components
├── assets/
└── enclosure/
```

### MicroPython Projects

For Python-based ESP32 projects.

**Requirements:**
- Include `boot.py` and `main.py`
- Specify required MicroPython firmware version and download link
- List any required MicroPython packages (`upip` / `mip`)
- Include `requirements.txt` if using external packages

**Structure:**
```
projects/{category}/{NN}-{slug}/
├── README.md
├── boot.py
├── main.py
├── lib/              # Required MicroPython libraries
├── config.py.example # Template for user configuration
├── assets/
└── enclosure/
```

---

## Project Structure

Every project MUST follow this directory structure:

```
projects/{category}/{NN}-{project-slug}/
├── README.md            # REQUIRED: Full project documentation
├── {project-slug}.ino   # REQUIRED (Arduino): Main sketch
├── platformio.ini       # REQUIRED (PlatformIO): Build config
├── src/                 # Optional: Additional source files
│   └── main.cpp
├── include/             # Optional: Header files
├── lib/                 # Optional: Local libraries
├── data/                # Optional: SPIFFS/LittleFS data
├── assets/              # Recommended: Screenshots, demo GIFs
│   ├── screenshot.png
│   └── demo.gif
└── enclosure/           # Optional: 3D print files
    ├── case.stl
    ├── case.step
    └── README.md
```

### Categories
```
beginner, intermediate, advanced, esp32-cam, esp32-s3, esp32-c3,
esp32-c6, esp32-h2, robotics, smart-home, security, audio,
games, industrial, wearables, displays, fun
```

### Naming Rules
- **Folders**: lowercase with hyphens (`weather-station`)
- **Arduino files**: match the folder name (`weather-station.ino`)
- **Docs**: UPPERCASE (`README.md`, `SETUP.md`)
- **Images**: lowercase, descriptive (`esp32-wiring-diagram.png`)

---

## Cross-Chip Testing Matrix

When submitting a project, fill in this testing matrix in your README:

```markdown
## 🧪 Testing Matrix

| Chip | Board | Framework | Status | Tester | Notes |
|------|-------|-----------|--------|--------|-------|
| ESP32 Classic | ESP32-DevKitC | Arduino | ✅ Passed | @username | Primary target |
| ESP32-S2 | ESP32-S2-Saola | Arduino | ⬜ Untested | — | — |
| ESP32-S3 | ESP32-S3-DevKitC | Arduino | ✅ Passed | @username | USB CDC works |
| ESP32-C3 | ESP32-C3-DevKitM | Arduino | ⚠️ Partial | @username | GPIO limited |
| ESP32-C6 | ESP32-C6-DevKitC | Arduino | ⬜ Untested | — | — |
| ESP32-H2 | ESP32-H2-DevKitM | Arduino | ⬜ Untested | — | No WiFi |
```

**Status Legend:**
- ✅ **Passed** — Fully functional, tested on real hardware
- ⚠️ **Partial** — Works with limitations (document them)
- ❌ **Failed** — Does not work (explain why)
- ⬜ **Untested** — Not tested yet (community help welcome!)

You don't need to test on every chip — just be honest about what you've tested. The community will help fill in the rest!

---

## Web Flasher Firmware Submission

To have your project included in the [ESP32 Community Web Flasher](https://tushar2704.github.io/ESP32-Community/):

### Requirements
1. **Pre-compiled binary** (`.bin`) for each supported chip
2. **Partition table** if non-default
3. **Manifest file** (`manifest.json`) following the [ESP Web Tools format](https://esphome.github.io/esp-web-tools/)
4. **Tested on real hardware** — Web Flasher firmware must be verified working

### Submission Process
1. Build your project with the **release** configuration
2. Export the compiled `.bin` files
3. Create a `webflasher/` directory in your project:
   ```
   webflasher/
   ├── manifest.json
   ├── firmware-esp32.bin
   ├── firmware-esp32s3.bin    # If S3 is supported
   └── bootloader.bin          # If custom bootloader
   ```
4. Test flashing with [ESP Web Tools](https://esphome.github.io/esp-web-tools/) locally
5. Submit via Pull Request

### Manifest Template
```json
{
  "name": "Project Name",
  "version": "1.0.0",
  "builds": [
    {
      "chipFamily": "ESP32",
      "parts": [
        { "path": "firmware-esp32.bin", "offset": 0 }
      ]
    },
    {
      "chipFamily": "ESP32-S3",
      "parts": [
        { "path": "firmware-esp32s3.bin", "offset": 0 }
      ]
    }
  ]
}
```

---

## 3D Enclosure Submissions

We love 3D printable enclosure designs! Enclosures are featured on the [@Makerrdesigns](https://youtube.com/@Makerrdesigns) YouTube channel.

### Required Files
| File | Format | Purpose |
|------|--------|---------|
| `case.stl` | STL | Print-ready mesh |
| `case.step` | STEP | Editable CAD source |
| `README.md` | Markdown | Print settings & assembly |
| `photo.jpg` | JPEG/PNG | Photo of printed result |

### README Must Include
```markdown
## 🖨️ Print Settings

| Setting | Value |
|---------|-------|
| Layer Height | 0.2mm |
| Infill | 20% |
| Supports | Yes/No |
| Material | PLA / PETG / ABS |
| Print Time | ~2 hours |
| Filament | ~30g |

## Compatible Boards
- ESP32-DevKitC v4
- ESP32-S3-DevKitC-1

## Assembly
1. Step-by-step assembly instructions
2. Include photos of each step if possible
```

### Design Standards
- **Tolerance**: 0.3mm clearance for board fitment
- **Wall thickness**: Minimum 1.2mm
- **Screw holes**: M2.5 or M3 heat-set inserts preferred
- **Ventilation**: Include ventilation slots for boards that run hot
- **Access**: USB port, reset button, and GPIO headers must be accessible
- **License**: All enclosure designs fall under CC BY-NC-ND 4.0

---

## Translation Contributions

Help make ESP32 projects accessible worldwide!

### Supported Languages
We welcome translations in any language. Priority languages:
🇪🇸 Spanish | 🇫🇷 French | 🇩🇪 German | 🇧🇷 Portuguese | 🇯🇵 Japanese | 🇨🇳 Chinese | 🇰🇷 Korean | 🇮🇳 Hindi

### How to Translate
1. Create a folder: `translations/{language-code}/` (e.g., `translations/es/`)
2. Translate the project README, keeping the same structure
3. Keep all code blocks, links, and image paths unchanged
4. Add a language badge at the top: `🌍 Translated from [English](../../README.md)`
5. Submit via Pull Request with the `translation` label

### Translation Guidelines
- Translate prose, not code or technical commands
- Keep brand names untranslated (ESP32, Arduino, PlatformIO)
- Preserve all markdown formatting
- Link back to the original English version

---

## Reporting Bugs

Use the [Bug Report template](.github/ISSUE_TEMPLATE/bug_report.md) and include:

1. **Which ESP32 chip/board** you're using (e.g., ESP32-S3-DevKitC-1)
2. **Which project** has the bug (link to the project folder)
3. **Your development environment** (Arduino IDE version, PlatformIO version, ESP-IDF version)
4. **ESP32 Board Package version** (e.g., v2.0.17)
5. **What you expected** to happen
6. **What actually happened** (include serial monitor output)
7. **Steps to reproduce**
8. **Error messages or screenshots**

---

## Improving Documentation

Documentation contributions are **always welcome**! This includes:

- Fixing typos or grammar
- Adding missing information to guides
- Improving code examples and comments
- Adding pin maps or wiring diagrams
- Improving the chip comparison tables
- Adding "Getting Started" guides for new chips
- Writing tutorials for different frameworks

For documentation changes, simply submit a Pull Request.

---

## Discussion & Community Engagement

### GitHub Discussions
We use [GitHub Discussions](../../discussions) for community interaction:

| Category | Use For |
|----------|---------|
| 💡 Ideas | Suggest new projects or features |
| ❓ Q&A | Ask questions, get help |
| 🏗️ Show & Tell | Share your builds and modifications |
| 📢 Announcements | Stay updated on new releases |

### Engagement Guidelines
- **Be helpful**: If you know the answer, share it! Every response helps.
- **Search first**: Check if your question has been asked before
- **Share your builds**: We love seeing community creations in Show & Tell
- **Tag correctly**: Use chip-specific tags (e.g., `esp32-s3`, `esp32-c6`)
- **Link your work**: If you've created a tutorial or blog post, share it!
- **Be patient**: Maintainers and contributors are volunteers

### YouTube Community
- **Watch & Like**: Support the channels that power this community
  - 📺 [IamNotIntoTech](https://youtube.com/@IamNotIntoTech) — ESP32 tutorials
  - 🖨️ [Makerrdesigns](https://youtube.com/@Makerrdesigns) — 3D design & printing
- **Comment**: Share your results, ask questions, suggest improvements
- **Subscribe**: Help us grow and create more content

---

## Code Style Guidelines

### Arduino / C++ / ESP-IDF

```cpp
// File header (required in every source file)
/**
 * @file    project-name.ino
 * @brief   One-line description of what this file does
 * @author  Tushar Aggarwal (@tushar2704)
 * @repo    https://github.com/tushar2704/ESP32-Community
 * @license AGPL-3.0
 */

// Indentation: 4 spaces (no tabs)
// Braces: same line for functions and control structures
void setup() {
    Serial.begin(115200);
    if (condition) {
        doSomething();
    }
}

// Variable naming: camelCase for locals, UPPER_CASE for constants
const int LED_PIN = 2;
int sensorValue = 0;

// Comment non-obvious code
// Use descriptive variable names — avoid single-letter names (except loop counters)
```

### MicroPython

```python
# File header (required in every source file)
"""
project-name - One-line description
Author: Tushar Aggarwal (@tushar2704)
Repo: https://github.com/tushar2704/ESP32-Community
License: AGPL-3.0
"""

# Use snake_case for variables and functions
# Use PascalCase for classes
# 4 spaces indentation
# Type hints encouraged
```

### Configuration Files
- Use `secrets.h.example` or `config.py.example` as templates
- **NEVER** commit actual credentials, API keys, or WiFi passwords
- Document every configurable parameter

### Documentation
- Use standard GitHub-Flavored Markdown
- Include alt text on all images
- Use relative links within the repo
- Keep line length reasonable (~120 chars for prose)

---

## Pull Request Process

### Before You Start
1. **Check existing issues/PRs** to avoid duplicating work
2. **Open an issue first** for large changes or new projects
3. **Read** [MASTER_INSTRUCTIONS.md](MASTER_INSTRUCTIONS.md) for conventions

### Step by Step
1. **Fork** the repository
2. **Clone** your fork locally
3. **Configure git identity:**
   ```bash
   git config user.name "your-github-username"
   git config user.email "your@email.com"
   ```
4. **Create a branch** from `main`:
   ```bash
   git checkout -b feat/my-awesome-project
   ```
5. **Make your changes** following the guidelines above
6. **Test** your code compiles and runs
7. **Commit** with descriptive messages (see [commit convention](MASTER_INSTRUCTIONS.md#-commit-message-convention))
8. **Push** to your fork
9. **Open a Pull Request** against `main`

### PR Checklist

Copy this checklist into your PR description:

```markdown
## PR Checklist

### General
- [ ] I have read [CONTRIBUTING.md](CONTRIBUTING.md)
- [ ] My changes follow the project structure conventions
- [ ] Commit messages follow the `type(scope): description` convention
- [ ] No unnecessary files committed (no `.DS_Store`, `__pycache__`, etc.)

### For Project Submissions
- [ ] README follows the project template
- [ ] Code compiles without errors
- [ ] Tested on real hardware (specify which board)
- [ ] Testing matrix is filled in
- [ ] Screenshots/GIFs included in `assets/`
- [ ] Both YouTube channels credited in README
- [ ] No hardcoded credentials in code
- [ ] `secrets.h.example` provided if credentials are needed
- [ ] platformio.ini provided with correct board target
- [ ] Arduino .ino file provided

### For Enclosure Submissions
- [ ] STL file included
- [ ] STEP file included
- [ ] Photo of printed enclosure included
- [ ] Print settings documented
- [ ] Compatible boards listed

### For Documentation
- [ ] Markdown renders correctly
- [ ] All links work
- [ ] Images have alt text
```

### Review Process
1. A maintainer will review your PR within **48 hours**
2. Automated checks will verify code compiles (CI)
3. Feedback will be provided as review comments
4. Once approved, your PR will be merged and you'll be credited!

---

## 🏆 Recognition

All contributors are recognized in the project! Contributions are tracked and credited in:

- **README.md** — Contributors section
- **Project READMEs** — Co-author credit
- **YouTube videos** — Shout-outs for community contributions
- **GitHub** — Contributor badge on the repository

---

## 💬 Questions?

If you're unsure about anything:

- 💬 Open a [Discussion](../../discussions)
- 🐛 Open an [Issue](../../issues)
- 📧 Email: [tushar.inseec@gmail.com](mailto:tushar.inseec@gmail.com)

We're happy to help you get started!

---

<p align="center">
  <b>Thank you for making the ESP32 Community awesome! ⚡</b><br>
  <sub>Built by <a href="https://github.com/tushar2704">Tushar Aggarwal</a> and the community</sub>
</p>
