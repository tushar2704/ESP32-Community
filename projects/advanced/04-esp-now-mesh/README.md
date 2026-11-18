# 🔴 Project ADV-04: ESP-NOW Off-Grid Mesh Communicator & P2P Protocol

> Ultra-low latency (< 5ms) routerless peer-to-peer communication system operating via raw Espressif 2.4 GHz ESP-NOW frames. Features automatic peer discovery, structured binary packet exchange, packet ACK validation, and multi-node broadcast.

---

## 📋 Overview
- **Category**: Advanced (⭐⭐⭐⭐ 4/5)
- **Supported Boards**: All ESP32 variants (Classic, S2, S3, C3, C6)
- **Primary Goals**:
  1. Transmit and receive encrypted telemetry packets between 2 or more ESP32 devices without any Wi-Fi router or internet.
  2. Implement bidirectional request-response pairing with automatic peer registration.
  3. Deliver reliable point-to-point packet delivery confirmation via hardware ACK callbacks.

---

## ⚡ How It Works

```text
┌────────────────────────┐      Raw 2.4 GHz Frame      ┌────────────────────────┐
│     Node A (Master)    │  ────────────────────────►  │    Node B (Responder)  │
│  - Reads local sensors │     < 5ms Latency           │  - Executes command    │
│  - Broadcasts payload  │  ◄────────────────────────  │  - Returns ACK metrics │
└────────────────────────┘      Instant HW Callback    └────────────────────────┘
```

---

## 📺 Video Tutorial & Enclosures
- 🎬 **Watch the Tutorial**: [@IamNotIntoTech on YouTube](https://youtube.com/@IamNotIntoTech)
- 🖨️ **3D Handheld Communicator Shell**: [@Makerrdesigns on YouTube](https://youtube.com/@Makerrdesigns)

---

## 📜 License
Licensed under GNU AGPLv3. See [LICENSE](../../../LICENSE) for details.
