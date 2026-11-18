/*
 * ESP32-Community | Security Project 02: BLE Threat & Tracker Scanner
 * 
 * Performs active continuous Bluetooth Low Energy (BLE) scanning to discover
 * nearby BLE peripherals, tracking beacons, Apple AirTags / FindMy devices,
 * and smart wearables with real-time RSSI signal-strength analysis.
 * 
 * Part of ESP32-Community Hub: https://github.com/tushar2704/ESP32-Community
 * Video Tutorials & Community: @IamNotIntoTech | @Makerrdesigns
 */

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

const int SCAN_TIME_SECONDS = 5;
BLEScan *pBLEScan;

// Statistics
unsigned long totalDiscovered = 0;
unsigned long appleDevices = 0;
unsigned long microsoftDevices = 0;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    totalDiscovered++;
    int rssi = advertisedDevice.getRSSI();
    String addr = advertisedDevice.getAddress().toString().c_str();
    String name = advertisedDevice.haveName() ? advertisedDevice.getName().c_str() : "<Unknown>";

    bool isApple = false;
    bool isAirTag = false;

    // Check Manufacturer Data for Apple (0x004C)
    if (advertisedDevice.haveManufacturerData()) {
      std::string mData = advertisedDevice.getManufacturerData();
      if (mData.length() >= 2) {
        uint16_t companyId = (uint8_t)mData[0] | ((uint8_t)mData[1] << 8);
        if (companyId == 0x004C) { // Apple Inc.
          isApple = true;
          appleDevices++;
          // AirTag / FindMy payload type is 0x12 (Offline Finding)
          if (mData.length() >= 3 && (uint8_t)mData[2] == 0x12) {
            isAirTag = true;
          }
        } else if (companyId == 0x0006) { // Microsoft
          microsoftDevices++;
        }
      }
    }

    // Format output
    Serial.printf("[%s] RSSI: %4d dBm | MAC: %s | Name: %-18s %s%s\n",
                  isAirTag ? "!AIRTAG!" : (isApple ? " APPLE  " : " BLE-DEV"),
                  rssi,
                  addr.c_str(),
                  name.c_str(),
                  isAirTag ? "[TRACKER DETECTED]" : "",
                  advertisedDevice.haveServiceUUID() ? advertisedDevice.getServiceUUID().toString().c_str() : "");
  }
};

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("==================================================");
  Serial.println(" ESP32 Community: BLE Tracker & AirTag Scanner    ");
  Serial.println("==================================================");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); // Active scan requests scan response packets
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);

  Serial.println("[BLE] Continuous scan initialized. Scanning 2.4GHz BLE spectrum...\n");
}

void loop() {
  BLEScanResults foundDevices = pBLEScan->start(SCAN_TIME_SECONDS, false);
  
  Serial.println("--------------------------------------------------");
  Serial.printf("[SCAN CYCLE COMPLETE] Devices Found: %d | Apple/FindMy: %lu | Total Ever: %lu\n",
                foundDevices.getCount(), appleDevices, totalDiscovered);
  Serial.println("--------------------------------------------------\n");

  pBLEScan->clearResults(); // Clear buffer to prevent memory leakage
  delay(1000);
}
