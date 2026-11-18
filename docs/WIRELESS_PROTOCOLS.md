# 📡 ESP32 Wireless Protocols — Complete Guide

> **The definitive guide to every wireless protocol supported by the ESP32 family — WiFi, BLE, ESP-NOW, Classic Bluetooth, Thread, Zigbee, and Matter.**

---

## 📑 Table of Contents

- [Protocol Support by Chip](#-protocol-support-by-chip)
- [WiFi (802.11 b/g/n)](#-wifi-80211-bgn)
  - [Station Mode (STA)](#station-mode-sta)
  - [Access Point Mode (AP)](#access-point-mode-ap)
  - [ESP-Mesh (WiFi Mesh)](#esp-mesh-wifi-mesh)
- [Bluetooth Low Energy (BLE)](#-bluetooth-low-energy-ble)
  - [BLE Peripheral (Server)](#ble-peripheral-server)
  - [BLE Central (Client)](#ble-central-client)
  - [BLE Beacon (iBeacon)](#ble-beacon-ibeacon)
- [ESP-NOW](#-esp-now)
  - [Point-to-Point](#point-to-point)
  - [Multicast / Broadcast](#multicast--broadcast)
- [Classic Bluetooth (BR/EDR)](#-classic-bluetooth-bredr)
- [Thread (802.15.4)](#-thread-802154)
- [Zigbee (802.15.4)](#-zigbee-802154)
- [Matter](#-matter)
- [Choosing the Right Protocol](#-choosing-the-right-protocol)
- [Resources](#-resources)

---

## 🧩 Protocol Support by Chip

Not every ESP32 variant supports every protocol. Use this table as your quick reference:

| Protocol | ESP32 | ESP32-S2 | ESP32-S3 | ESP32-C3 | ESP32-C6 | ESP32-H2 |
|---|:---:|:---:|:---:|:---:|:---:|:---:|
| WiFi 4 (2.4 GHz) | ✅ | ✅ | ✅ | ✅ | ✅ | ❌ |
| WiFi 6 (2.4 GHz) | ❌ | ❌ | ❌ | ❌ | ✅ | ❌ |
| BLE 4.2 | ✅ | ❌ | — | — | — | — |
| BLE 5.0 | ❌ | ❌ | ✅ | ✅ | ✅ | ✅ |
| Classic Bluetooth | ✅ | ❌ | ❌ | ❌ | ❌ | ❌ |
| ESP-NOW | ✅ | ✅ | ✅ | ✅ | ✅ | ❌ |
| Thread (802.15.4) | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ |
| Zigbee (802.15.4) | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ |
| Matter | ✅¹ | ✅¹ | ✅¹ | ✅¹ | ✅ | ✅ |

> ¹ Matter over WiFi (no Thread). ESP32-C6 and ESP32-H2 support Matter over both WiFi and Thread.

> [!IMPORTANT]
> The **ESP32-H2** has **no WiFi** — it is a dedicated 802.15.4 (Thread/Zigbee) + BLE chip. Use it as a Thread border router or Zigbee coordinator alongside a WiFi-capable chip.

---

## 📶 WiFi (802.11 b/g/n)

WiFi is the most commonly used protocol on ESP32. All variants except ESP32-H2 include a 2.4 GHz WiFi radio. The ESP32-C6 adds WiFi 6 (802.11ax) support.

### Station Mode (STA)

Connect your ESP32 to an existing WiFi network (like your home router).

```cpp
#include <WiFi.h>

const char* ssid     = "YourNetwork";
const char* password = "YourPassword";

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {}
```

> [!TIP]
> Use `WiFi.setAutoReconnect(true)` and handle `WiFi.onEvent()` callbacks for robust, production-grade connections that survive temporary network drops.

### Access Point Mode (AP)

Turn the ESP32 into its own WiFi hotspot so other devices can connect to it.

```cpp
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.softAP("ESP32-Hotspot", "password123");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP()); // Usually 192.168.4.1
}

void loop() {}
```

You can also run **STA + AP simultaneously** with `WiFi.mode(WIFI_AP_STA);` to bridge networks.

### ESP-Mesh (WiFi Mesh)

ESP-MESH (now called **ESP-WiFi-Mesh**) lets multiple ESP32 nodes form a self-healing, self-organizing mesh network — ideal for large-area sensor deployments.

```cpp
#include "esp_mesh.h"
// ESP-IDF mesh example — see Espressif's official esp-mesh examples
// for the full initialization sequence (too long for a snippet).
```

> [!NOTE]
> The **painlessMesh** Arduino library is the easiest way to get started with mesh networking. Install it via the Arduino Library Manager.

| Feature | Value |
|---|---|
| Max nodes | ~1000 (official) |
| Max layers | 25 |
| Latency | Higher per hop |
| Root node | Auto-elected or fixed |

---

## 🔵 Bluetooth Low Energy (BLE)

BLE is available on ESP32, ESP32-S3, ESP32-C3, ESP32-C6, and ESP32-H2. The original ESP32 supports BLE 4.2; the newer chips support BLE 5.0 with extended advertising and 2M PHY.

### BLE Peripheral (Server)

Expose a GATT service so phones/tablets can read sensor data.

```cpp
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

void setup() {
  Serial.begin(115200);
  BLEDevice::init("ESP32-Sensor");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pChar = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );
  pChar->setValue("Hello from ESP32");
  pService->start();

  BLEAdvertising *pAdv = BLEDevice::getAdvertising();
  pAdv->addServiceUUID(SERVICE_UUID);
  pAdv->start();
  Serial.println("BLE Server ready!");
}

void loop() { delay(2000); }
```

### BLE Central (Client)

Scan for and connect to nearby BLE peripherals.

```cpp
#include <BLEDevice.h>
#include <BLEScan.h>

void setup() {
  Serial.begin(115200);
  BLEDevice::init("ESP32-Scanner");
  BLEScan* pScan = BLEDevice::getScan();
  pScan->setActiveScan(true);
  BLEScanResults results = pScan->start(5); // Scan for 5 seconds

  for (int i = 0; i < results.getCount(); i++) {
    Serial.println(results.getDevice(i).toString().c_str());
  }
}

void loop() {}
```

### BLE Beacon (iBeacon)

Broadcast an iBeacon-compatible advertisement for proximity detection.

```cpp
#include <BLEDevice.h>
#include <BLEBeacon.h>

void setup() {
  BLEDevice::init("");
  BLEBeacon beacon;
  beacon.setManufacturerId(0x4C00); // Apple
  beacon.setProximityUUID(BLEUUID("12345678-1234-1234-1234-123456789ABC"));
  beacon.setMajor(1);
  beacon.setMinor(100);
  beacon.setSignalPower(-59);

  BLEAdvertisementData advData;
  advData.setFlags(0x06);
  advData.setManufacturerData(beacon.getData());

  BLEAdvertising *pAdv = BLEDevice::getAdvertising();
  pAdv->setAdvertisementData(advData);
  pAdv->start();
}

void loop() {}
```

---

## ⚡ ESP-NOW

ESP-NOW is Espressif's proprietary connectionless protocol — **no router needed**. Data frames go directly between ESP32s at up to **1 Mbps** with extremely low latency (~1-3 ms). Max payload: **250 bytes**.

> [!TIP]
> ESP-NOW is perfect for remote controls, sensor networks, and robotics where you need fast, low-overhead communication without the complexity of WiFi.

### Point-to-Point

**Sender:**

```cpp
#include <esp_now.h>
#include <WiFi.h>

uint8_t peerMAC[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

typedef struct {
  float temperature;
  float humidity;
} SensorData;

void onSent(const uint8_t *mac, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sent ✓" : "Send FAIL");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, peerMAC, 6);
  peer.channel = 0;
  peer.encrypt = false;
  esp_now_add_peer(&peer);

  SensorData data = {25.3, 60.1};
  esp_now_send(peerMAC, (uint8_t*)&data, sizeof(data));
}

void loop() {}
```

**Receiver:**

```cpp
#include <esp_now.h>
#include <WiFi.h>

typedef struct {
  float temperature;
  float humidity;
} SensorData;

void onReceive(const uint8_t *mac, const uint8_t *data, int len) {
  SensorData *d = (SensorData*)data;
  Serial.printf("Temp: %.1f°C  Humidity: %.1f%%\n", d->temperature, d->humidity);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_recv_cb(onReceive);
}

void loop() {}
```

### Multicast / Broadcast

Send to **all** ESP-NOW peers by using the broadcast MAC address:

```cpp
uint8_t broadcastMAC[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
// Add as peer, then esp_now_send(broadcastMAC, ...) to reach everyone
```

| Feature | Value |
|---|---|
| Max peers (encrypted) | 10 |
| Max peers (unencrypted) | 20 |
| Max payload | 250 bytes |
| Range | ~200m (line of sight) |
| Latency | 1–3 ms typical |
| Encryption | CCMP (optional) |

---

## 🔷 Classic Bluetooth (BR/EDR)

> [!WARNING]
> Classic Bluetooth is **only available on the original ESP32** (dual-core). It is NOT supported on ESP32-S2, S3, C3, C6, or H2.

Classic Bluetooth supports Serial Port Profile (SPP) — essentially a wireless serial port.

```cpp
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32-BT"); // Bluetooth device name
  Serial.println("Bluetooth started! Pair from your phone.");
}

void loop() {
  if (SerialBT.available()) {
    char c = SerialBT.read();
    Serial.write(c);        // Echo to Serial Monitor
  }
  if (Serial.available()) {
    char c = Serial.read();
    SerialBT.write(c);      // Echo to Bluetooth
  }
}
```

Classic Bluetooth supports A2DP (audio streaming) and HID (keyboard/mouse) profiles too — useful for media players and input devices.

---

## 🧵 Thread (802.15.4)

Thread is a low-power IPv6 mesh networking protocol built on IEEE 802.15.4. It's the foundation for Matter-over-Thread smart home devices.

**Supported chips:** ESP32-C6, ESP32-H2 only.

```c
// ESP-IDF Thread example (simplified)
#include "esp_openthread.h"
#include "esp_openthread_netif_glue.h"

// Full initialization requires ESP-IDF v5.1+ and OpenThread component
// See: examples/openthread/ in ESP-IDF
```

| Role | Description |
|---|---|
| **Leader** | Manages router assignments |
| **Router** | Forwards packets in the mesh |
| **End Device** | Leaf node, can sleep |
| **Border Router** | Bridges Thread ↔ WiFi/Ethernet |

> [!TIP]
> The **ESP32-C6** makes an excellent Thread Border Router because it has both WiFi and 802.15.4 radios on a single chip.

---

## 🐝 Zigbee (802.15.4)

Zigbee is a mature, low-power mesh protocol widely used in smart home products (Philips Hue, IKEA TRÅDFRI, etc.). ESP32-C6 and ESP32-H2 support Zigbee 3.0 via the **esp-zigbee-sdk**.

**Supported chips:** ESP32-C6, ESP32-H2 only.

```c
// ESP-IDF Zigbee Light example (simplified)
#include "esp_zigbee_core.h"

// Zigbee requires the esp-zigbee-sdk component
// See: examples/esp_zigbee_* in Espressif's esp-zigbee-sdk repository
```

| Role | Description |
|---|---|
| **Coordinator** | Forms the network, assigns addresses |
| **Router** | Extends the mesh range |
| **End Device** | Battery-powered leaf node |

> [!NOTE]
> Zigbee and Thread share the same 802.15.4 radio, but they use **different network stacks** and cannot run simultaneously on the same chip. Choose one per device.

---

## 🏠 Matter

Matter is the **unified smart home standard** backed by Apple, Google, Amazon, and Samsung. It runs over WiFi, Thread, or Ethernet and provides cross-ecosystem compatibility.

**Matter over WiFi:** ESP32, ESP32-S3, ESP32-C3, ESP32-C6
**Matter over Thread:** ESP32-C6, ESP32-H2

```c
// Matter Light example (ESP-IDF + esp-matter SDK)
#include <esp_matter.h>
#include <esp_matter_core.h>

// Full example requires esp-matter component from Espressif
// Configured via ESP-IDF components directory
```

The typical workflow:
1. Install **esp-matter** SDK (layered on top of ESP-IDF + connectedhomeip)
2. Create a device type (light, switch, sensor, lock, etc.)
3. Commission via BLE (phone scans a QR code)
4. Device joins your Matter fabric and works with Apple Home, Google Home, Alexa, etc.

> [!IMPORTANT]
> Matter development requires **ESP-IDF v5.1+** and significant flash/RAM. Use boards with at least **4 MB flash** and **PSRAM** for a comfortable development experience.

---

## 🎯 Choosing the Right Protocol

| Scenario | Recommended Protocol | Why |
|---|---|---|
| Connect to internet | **WiFi (STA)** | Direct cloud/API access |
| Phone ↔ ESP32 control | **BLE** | Low power, no router needed |
| ESP32 ↔ ESP32 fast link | **ESP-NOW** | <3ms latency, no infrastructure |
| Wireless serial terminal | **Classic BT (SPP)** | Simple, stream-based |
| Smart home device | **Matter** | Cross-ecosystem, future-proof |
| Battery mesh sensors | **Thread** | IPv6 mesh, low power |
| Legacy smart home devices | **Zigbee** | Mature ecosystem, Hue/IKEA |
| Long range, many nodes | **ESP-Mesh** | WiFi-based self-healing mesh |
| Remote control / gamepad | **ESP-NOW** | Minimal latency, tiny payload |

> [!TIP]
> You can combine protocols! Common combos:
> - **WiFi + BLE** — Use BLE for provisioning, WiFi for data (Matter does this)
> - **ESP-NOW + WiFi** — ESP-NOW for sensor → gateway, WiFi for gateway → cloud
> - **Thread + WiFi** — On ESP32-C6, run a Thread Border Router bridging both networks

---

## 📚 Resources

| Resource | Link |
|---|---|
| ESP-IDF WiFi Docs | [docs.espressif.com](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_wifi.html) |
| ESP-IDF BLE Docs | [docs.espressif.com](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/bluetooth/index.html) |
| ESP-NOW Docs | [docs.espressif.com](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html) |
| Espressif Matter Solution | [docs.espressif.com](https://docs.espressif.com/projects/esp-matter/en/latest/) |
| Espressif Zigbee SDK Docs | [docs.espressif.com](https://docs.espressif.com/projects/esp-zigbee-sdk/en/latest/) |

---

<p align="center">Made with ❤️ by <a href="https://github.com/tushar2704">Tushar Aggarwal</a> | 📺 <a href="https://youtube.com/@IamNotIntoTech">@IamNotIntoTech</a> + <a href="https://youtube.com/@Makerrdesigns">@Makerrdesigns</a></p>
