/**
 * ESP32_Easy — Basic Example
 * 
 * Demonstrates the simplest usage of the ESP32_Easy library:
 *   1. Initialize the library (auto-detects your chip)
 *   2. Print detailed chip information
 *   3. Connect to WiFi
 *   4. Scan nearby networks
 *   5. Print uptime and free heap every 5 seconds
 *
 * Author: Tushar Aggarwal (@tushar2704)
 * License: AGPL-3.0
 * https://github.com/tushar2704/ESP32-Community
 */

#include <ESP32_Easy.h>

void setup() {
  // Initialize ESP32_Easy — sets up Serial at 115200 and detects your chip
  ESP32_Easy.begin();

  // Print detailed chip info (model, cores, flash, PSRAM, MAC)
  ESP32_Easy.printChipInfo();

  // Connect to WiFi (replace with your credentials)
  ESP32_Easy.connectWiFi("YourSSID", "YourPassword");

  // Scan and display nearby WiFi networks
  ESP32_Easy.scanNetworks();
}

void loop() {
  // Print uptime and free heap memory every 5 seconds
  Serial.printf("Uptime: %s | Free Heap: %d bytes\n",
                ESP32_Easy.getUptime().c_str(),
                ESP32_Easy.getFreeHeap());
  delay(5000);
}
