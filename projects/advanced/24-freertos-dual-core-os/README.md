# 🔴 Project ADV-24: Preemptive FreeRTOS Dual-Core Multitasking Engine

> Unleashing the full dual-core Xtensa architecture of the ESP32: Core 0 dedicated to high-frequency sensor telemetry and background audio daemons, Core 1 dedicated to UI processing, inter-core queues, mutex synchronization, and thread-safe ringbuffers.

---

## 📋 Overview
- **Category**: Advanced (⭐⭐⭐⭐⭐ 5/5)
- **Supported Boards**: Classic ESP32, ESP32-S3 (Dual-Core variants)
- **Architecture**:
  - **Core 0 Daemon**: High-priority real-time hardware polling task pinned to Core 0 with zero jitter.
  - **Core 1 UI Task**: Pinned to Core 1 handling serial interactions, console processing, and state machine transitions.
  - **Inter-Core Communication**: `xQueueSendToBack` and `xQueueReceive` passing structured messages between cores without memory corruption or race conditions.

---

## 📺 Video Tutorial & Enclosures
- 🎬 **Watch the FreeRTOS Masterclass**: [@IamNotIntoTech on YouTube](https://youtube.com/@IamNotIntoTech)
- 🖨️ **3D Rack-Mounted Multi-Core Enclosure**: [@Makerrdesigns on YouTube](https://youtube.com/@Makerrdesigns)

---

## 📜 License
Licensed under GNU AGPLv3. See [LICENSE](../../../LICENSE) for details.
