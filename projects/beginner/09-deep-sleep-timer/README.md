# 🟢 Project 09: Ultra-Low-Power Deep Sleep & Timer Wakeup

> Optimize ESP32 power consumption from ~100mA down to microamps (~10–15µA). Features RTC timer wakeups, RTC fast memory boot counter persistence across resets, and wake cause diagnostics.

---

## 📋 Overview
- **Category**: Beginner (⭐ 1/5)
- **Supported Boards**: All ESP32 variants
- **Features**:
  - `RTC_DATA_ATTR` variable storage in RTC Slow/Fast memory that survives deep sleep.
  - Queries `esp_sleep_get_wakeup_cause()` to differentiate cold boot from timer, touchpad, or external GPIO wakeup.
  - Ideal baseline for battery-powered or solar IoT field sensors.

---

## 📺 Video Tutorial & Enclosures
- 🎬 **Watch the Tutorial**: [@IamNotIntoTech on YouTube](https://youtube.com/@IamNotIntoTech)
- 🖨️ **3D Solar Battery Enclosure**: [@Makerrdesigns on YouTube](https://youtube.com/@Makerrdesigns)

---

## 📜 License
Licensed under GNU AGPLv3. See [LICENSE](../../../LICENSE) for details.
