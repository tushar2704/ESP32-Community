# 📋 Changelog

All notable changes to the **ESP32 Community** project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [Unreleased]

### Planned
- Community project submissions
- Web Flasher integration for beginner projects
- Multi-language documentation (Spanish, French, German, Portuguese)
- CI/CD pipeline for automated compile checks across all chips
- ESP32-P4 project category

---

## [1.0.0] — 2026-11-18

### Added
- **Foundation Files**
  - `README.md` — Comprehensive project landing page with chip comparison, project tables, and community resources
  - `MASTER_INSTRUCTIONS.md` — Complete contributor and AI assistant instructions for the ESP32 family
  - `CONTRIBUTING.md` — Expanded contribution guide with multi-framework submission paths, testing matrix, and Web Flasher firmware process
  - `CODE_OF_CONDUCT.md` — Contributor Covenant v2.1 adapted for ESP32 Community
  - `LICENSE` — Dual license (AGPL-3.0 for code, CC BY-NC-ND 4.0 for assets) with commercial licensing tiers
  - `CHANGELOG.md` — This file (Keep a Changelog format)
  - `SECURITY.md` — Security policy with vulnerability reporting and responsible disclosure

- **Repository Structure**
  - Project categories: beginner, intermediate, advanced, esp32-cam, esp32-s3, esp32-c3, esp32-c6, esp32-h2, robotics, smart-home, security, audio, games, industrial, wearables, displays, fun
  - Standard project template: `projects/{category}/{NN}-{project-slug}/`
  - Cross-chip testing matrix template for all ESP32 variants

- **Supported ESP32 Chips**
  - ESP32 Classic (Xtensa LX6, Dual-core)
  - ESP32-S2 (Xtensa LX7, Single-core, USB OTG)
  - ESP32-S3 (Xtensa LX7, Dual-core, AI/ML, USB OTG)
  - ESP32-C3 (RISC-V, Single-core, BLE 5.0)
  - ESP32-C5 (RISC-V, WiFi 6, Bluetooth 5.0)
  - ESP32-C6 (RISC-V, WiFi 6, Thread, Zigbee, Matter)
  - ESP32-H2 (RISC-V, Thread, Zigbee, BLE 5.0)
  - ESP32-P4 (RISC-V, Dual-core, AI, MIPI-DSI/CSI)

- **Framework Support**
  - Arduino IDE with ESP32 Board Package
  - PlatformIO with espressif32 platform
  - ESP-IDF v5.x native development
  - MicroPython with Espressif builds

---

## Version History

| Version | Date | Highlights |
|---------|------|------------|
| [1.0.0] | 2026-11-18 | 🎉 Initial release — Phase 1 foundation |

---

[Unreleased]: https://github.com/tushar2704/ESP32-Community/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/tushar2704/ESP32-Community/releases/tag/v1.0.0
