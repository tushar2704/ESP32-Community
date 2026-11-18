/**
 * @file ble-beacon.ino
 * @brief Project 07: BLE Bluetooth Low Energy Proximity Beacon
 * @author Tushar Aggarwal (@tushar2704)
 * @url https://github.com/tushar2704/ESP32-Community
 * @license AGPL-3.0
 */

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLEBeacon.h>

#define BEACON_UUID "8ec76ea3-6668-49da-9866-75be9bc86f4d"
#define DEVICE_NAME "ESP32-Beacon"

BLEAdvertising *pAdvertising;

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("\n========================================================");
    Serial.println("  ⚡ ESP32 Community — Project 07: BLE Proximity Beacon");
    Serial.println("========================================================");

    // Initialize BLE Core
    BLEDevice::init(DEVICE_NAME);

    // Create Beacon instance
    BLEBeacon oBeacon = BLEBeacon();
    oBeacon.setManufacturerId(0x4C00); // 0x4C00 = Apple iBeacon format
    BLEUUID bleUUID = BLEUUID(BEACON_UUID);
    oBeacon.setProximityUUID(bleUUID);
    oBeacon.setMajor(100);
    oBeacon.setMinor(1);
    oBeacon.setSignalPower(-59); // Measured power at 1 meter in dBm

    BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
    BLEAdvertisementData oScanResponseData = BLEAdvertisementData();

    oAdvertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED
    std::string strServiceData = "";
    strServiceData += (char)26;     // Length
    strServiceData += (char)0xFF;   // Type: Manufacturer Specific
    strServiceData += oBeacon.getData(); 
    oAdvertisementData.addData(strServiceData);

    oScanResponseData.setName(DEVICE_NAME);

    pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->setAdvertisementData(oAdvertisementData);
    pAdvertising->setScanResponseData(oScanResponseData);
    pAdvertising->setMinInterval(160); // 160 * 0.625ms = 100ms
    pAdvertising->setMaxInterval(320); // 320 * 0.625ms = 200ms

    // Start advertising
    pAdvertising->start();
    Serial.println("📡 BLE Beacon is actively broadcasting!");
    Serial.printf("  - Device Name:   %s\n", DEVICE_NAME);
    Serial.printf("  - Proximity UUID: %s\n", BEACON_UUID);
    Serial.println("  - Major / Minor: 100 / 1");
    Serial.println("  - Open nRF Connect on your smartphone to detect the beacon.\n");
}

void loop() {
    delay(5000);
    Serial.println("📡 Beacon heart pulsing... advertising frames transmitted.");
}
