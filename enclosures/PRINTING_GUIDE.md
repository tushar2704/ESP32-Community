# 🖨️ 3D Printing Master Guide & Enclosure Standards

> **Filament selection, slicer profiles, snap-fit tolerances, and CAD engineering standards for ESP32 project enclosures.**  
> Designed and curated by [@Makerrdesigns](https://youtube.com/@Makerrdesigns).

---

## 🧵 Material Selection Guide

| Filament | Recommended Bed / Nozzle Temp | UV Resistance | Heat Deflection Temp (HDT) | Best Application |
| :--- | :--- | :--- | :--- | :--- |
| **PLA / PLA+** | 55–60°C / 205–215°C | Low | ~55°C | Desktop stands, indoor sensors, fast prototypes |
| **PETG** | 75–85°C / 235–245°C | Moderate | ~75°C | Snap-fit latches, everyday enclosures, battery pods |
| **ABS / ASA** | 90–105°C / 245–255°C | High (ASA) | ~95°C | Automotive OBD-II gauges, outdoor solar stations |
| **TPU (95A)** | 40–50°C / 225–235°C | High | ~60°C | Bumper cases, gaskets, weather-resistant port plugs |

---

## 📐 Optimal Slicer Settings (0.4mm Nozzle)

- **Layer Height**: `0.20mm` (standard) or `0.16mm` (fine detail for button stems)
- **Wall Loops / Perimeters**: `4 walls` (Critical for rigid screw boss integrity)
- **Top / Bottom Layers**: `4 top / 4 bottom`
- **Infill Density**: `20% Gyroid` or `25% Grid`
- **Supports**: `Tree (Auto)` on build plate only (Avoid supports inside narrow snap joints)

---

## 🔒 Snap-Fit Engineering Tolerances

When designing or printing snap-joint lids (screwless):
- **Bambu Lab X1C / P1S / A1**: `0.15mm – 0.20mm` diametrical clearance
- **Creality Ender-3 / K1**: `0.25mm – 0.30mm` diametrical clearance
- **Prusa MK3S+ / MK4**: `0.18mm – 0.22mm` diametrical clearance

---

## 📁 Community Enclosure Catalog

- [`enclosures/esp32-devkit-desktop-stand/`](esp32-devkit-desktop-stand/) — 45° ergonomic desk wedge for standard 38-pin DevKitC
- [`enclosures/esp32-cam-surveillance-case/`](esp32-cam-surveillance-case/) — Ball-and-socket swivel mount for ESP32-CAM
- [`enclosures/modular-snap-box/`](modular-snap-box/) — Parametric screwless project box

---

<div align="center">

**Part of the [ESP32 Community Hub](https://github.com/tushar2704/ESP32-Community)**  
<sub>CAD Designs by [@Makerrdesigns](https://youtube.com/@Makerrdesigns) • Videos on [@IamNotIntoTech](https://youtube.com/@IamNotIntoTech)</sub>

</div>
