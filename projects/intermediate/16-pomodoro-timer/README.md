# 🟡 Project INT-16: Pomodoro Productivity Desk Timer

> A tactile productivity desk timer implementing the proven Pomodoro Technique (25m Focus / 5m Short Break / 15m Long Break). Features non-blocking FreeRTOS state timing, acoustic audio chirps, progress percentages, and pause/skip button controls.

---

## 📋 Overview
- **Category**: Intermediate (⭐⭐ 2/5)
- **Supported Boards**: All ESP32 variants (Classic, S2, S3, C3, C6, H2)
- **Primary Goals**:
  1. Build a robust finite state machine (IDLE, WORK, SHORT_BREAK, LONG_BREAK, PAUSED).
  2. Implement acoustic buzzer frequencies for start, pause, and phase completion alerts.
  3. Track focus sessions completed and daily productivity metrics.

---

## ⚡ Hardware Connections (Optional Buzzer & Button)

```text
ESP32 GPIO 19 ───► Passive Buzzer (+) (GND to GND)
ESP32 GPIO 18 ───► Tactile Pushbutton (Active LOW to GND, internal PULLUP)
```

---

## 📺 Video Tutorial & Enclosures
- 🎬 **Watch the Tutorial**: [@IamNotIntoTech on YouTube](https://youtube.com/@IamNotIntoTech)
- 🖨️ **3D Desktop Desk Wedge Case**: [@Makerrdesigns on YouTube](https://youtube.com/@Makerrdesigns)

---

## 📜 License
Licensed under GNU AGPLv3. See [LICENSE](../../../LICENSE) for details.
