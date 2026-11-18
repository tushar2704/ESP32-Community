/*
 * ESP32-Community | Security Project 03: Wi-Fi Wardriver & Geospatial Scanner
 * 
 * Conducts continuous 2.4GHz Wi-Fi wardriving surveys, cataloging every Access Point's
 * SSID, BSSID (MAC), Channel, RSSI signal strength, and Security Encryption Type
 * in standard WiGLE-compatible CSV format.
 * 
 * Part of ESP32-Community Hub: https://github.com/tushar2704/ESP32-Community
 * Video Tutorials & Community: @IamNotIntoTech | @Makerrdesigns
 */

#include <Arduino.h>
#include <WiFi.h>

#ifndef LED_BUILTIN
  #define LED_BUILTIN 2
#endif

// WiGLE CSV Header v1.4
const char *WIGLE_HEADER_1 = "WigleWifi-1.4,appRelease=1.0,model=ESP32,release=1.0,device=ESP32-Wardriver,display=none,board=ESP32,brand=ESP32-Community";
const char *WIGLE_HEADER_2 = "MAC,SSID,AuthMode,FirstSeen,Channel,RSSI,CurrentLatitude,CurrentLongitude,AltitudeMeters,AccuracyMeters,Type";

unsigned long scanIndex = 0;
unsigned long totalNetworksLogged = 0;

String getAuthModeString(wifi_auth_mode_t authMode) {
  switch (authMode) {
    case WIFI_AUTH_OPEN:            return "[ESS]";
    case WIFI_AUTH_WEP:             return "[WEP][ESS]";
    case WIFI_AUTH_WPA_PSK:         return "[WPA-PSK-CCMP][ESS]";
    case WIFI_AUTH_WPA2_PSK:        return "[WPA2-PSK-CCMP][ESS]";
    case WIFI_AUTH_WPA_WPA2_PSK:    return "[WPA-PSK-CCMP][WPA2-PSK-CCMP][ESS]";
    case WIFI_AUTH_WPA2_ENTERPRISE: return "[WPA2-EAP-CCMP][ESS]";
    case WIFI_AUTH_WPA3_PSK:        return "[WPA3-SAE-CCMP][ESS]";
    case WIFI_AUTH_WPA2_WPA3_PSK:   return "[WPA2-PSK-CCMP][WPA3-SAE-CCMP][ESS]";
    default:                        return "[UNKNOWN]";
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Initialize Wi-Fi in Station mode and disconnect
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  Serial.println("\n==================================================");
  Serial.println("  ESP32 Community: WiGLE Wi-Fi Wardrive Scanner   ");
  Serial.println("==================================================");
  Serial.println("// Log output formatted for https://wigle.net\n");

  // Print WiGLE standard file headers
  Serial.println(WIGLE_HEADER_1);
  Serial.println(WIGLE_HEADER_2);
}

void loop() {
  scanIndex++;
  digitalWrite(LED_BUILTIN, HIGH);

  // Scan all channels, include hidden networks (show_hidden = true)
  int n = WiFi.scanNetworks(false, true, false, 150);

  digitalWrite(LED_BUILTIN, LOW);

  if (n > 0) {
    totalNetworksLogged += n;
    
    // Get ISO-style simulated timestamp
    unsigned long sec = millis() / 1000;
    char timeBuf[32];
    sprintf(timeBuf, "2026-01-01 %02lu:%02lu:%02lu", (sec / 3600) % 24, (sec / 60) % 60, sec % 60);

    for (int i = 0; i < n; ++i) {
      String bssid = WiFi.BSSIDstr(i);
      String ssid = WiFi.SSID(i);
      int rssi = WiFi.RSSI(i);
      int channel = WiFi.channel(i);
      String authMode = getAuthModeString(WiFi.encryptionType(i));

      // Sanitize SSID to prevent CSV injection (escape commas)
      ssid.replace(",", " ");

      // Default simulated GPS coordinates (0.000000) or connect a NEO-6M GPS module via Serial2
      const char *lat = "0.000000";
      const char *lon = "0.000000";
      const char *alt = "0.0";
      const char *acc = "10.0";

      // WiGLE CSV output format
      // MAC,SSID,AuthMode,FirstSeen,Channel,RSSI,Latitude,Longitude,Altitude,Accuracy,Type
      Serial.printf("%s,%s,%s,%s,%d,%d,%s,%s,%s,%s,WIFI\n",
                    bssid.c_str(),
                    ssid.c_str(),
                    authMode.c_str(),
                    timeBuf,
                    channel,
                    rssi,
                    lat,
                    lon,
                    alt,
                    acc);
    }
  }

  // Free memory allocated during scan
  WiFi.scanDelete();

  delay(2000); // 2 second pause between full sweeps
}
