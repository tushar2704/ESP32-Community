/*
 * ESP32-Community | Security Project 01: Wi-Fi Deauthentication & Disassociation Detector
 * 
 * Uses ESP32 802.11 Promiscuous (Monitor) Mode to sniff raw wireless frames
 * and alert when rogue deauth attacks (Type 0x00, Subtype 0x0C) or disassociation
 * frames (Subtype 0x0A) are detected targeting nearby access points or clients.
 * 
 * Part of ESP32-Community Hub: https://github.com/tushar2704/ESP32-Community
 * Video Tutorials & Community: @IamNotIntoTech | @Makerrdesigns
 */

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>

#ifndef LED_BUILTIN
  #define LED_BUILTIN 2
#endif

// Wi-Fi 2.4GHz channels: 1 to 13
const int MAX_CHANNELS = 13;
int currentChannel = 1;
unsigned long lastChannelHop = 0;
const unsigned long CHANNEL_HOP_INTERVAL_MS = 300;

// Deauth flood alert counters
volatile unsigned long deauthCount = 0;
volatile unsigned long disassocCount = 0;
unsigned long lastStatsPrint = 0;

// Format MAC address to readable string
void formatMac(const uint8_t *mac, char *out) {
  sprintf(out, "%02X:%02X:%02X:%02X:%02X:%02X",
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

// Promiscuous packet callback
void IRAM_ATTR snifferCallback(void *buf, wifi_promiscuous_pkt_type_t type) {
  // We only care about 802.11 Management frames (Type 0)
  if (type != WIFI_PKT_MGMT) return;

  const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
  const uint8_t *payload = pkt->payload;
  int len = pkt->rx_ctrl.sig_len;

  if (len < 24) return; // Management frame header must have at least 24 bytes

  // Frame Control field is the first 2 bytes (Little Endian)
  uint8_t frameType = payload[0];

  // 0xC0 = Deauthentication Frame (Type: Mgmt 00, Subtype: 1100)
  // 0xA0 = Disassociation Frame   (Type: Mgmt 00, Subtype: 1010)
  if (frameType == 0xC0 || frameType == 0xA0) {
    if (frameType == 0xC0) {
      deauthCount++;
    } else {
      disassocCount++;
    }

    // Trigger visual alarm LED
    digitalWrite(LED_BUILTIN, HIGH);

    // Receiver Address (Target): Bytes 4-9
    // Transmitter Address (Source/Attacker): Bytes 10-15
    // BSSID (Access Point): Bytes 16-21
    char targetMac[18];
    char sourceMac[18];
    char bssidMac[18];
    formatMac(&payload[4], targetMac);
    formatMac(&payload[10], sourceMac);
    formatMac(&payload[16], bssidMac);

    int8_t rssi = pkt->rx_ctrl.rssi;

    Serial.printf("\n[!ALERT!] %s Detected on Ch %d! RSSI: %d dBm\n",
                  (frameType == 0xC0) ? "DEAUTH ATTACK" : "DISASSOCIATION",
                  pkt->rx_ctrl.channel, rssi);
    Serial.printf("          Attacker/Transmitter: %s\n", sourceMac);
    Serial.printf("          Target Station:      %s\n", targetMac);
    Serial.printf("          Access Point (BSSID): %s\n", bssidMac);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("==================================================");
  Serial.println(" ESP32 Community: Wi-Fi Deauth Attack Detector    ");
  Serial.println("==================================================");

  // Initialize Wi-Fi in Station mode
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Configure Promiscuous Sniffer Filter
  wifi_promiscuous_filter_t filter = {
    .filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT
  };
  esp_wifi_set_promiscuous_filter(&filter);
  esp_wifi_set_promiscuous_rx_cb(&snifferCallback);
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(currentChannel, WIFI_SECOND_CHAN_NONE);

  Serial.printf("[SNIFFER] Promiscuous mode enabled! Scanning Ch 1-%d...\n", MAX_CHANNELS);
  Serial.println("--------------------------------------------------");
}

void loop() {
  unsigned long now = millis();

  // Channel hopping routine
  if (now - lastChannelHop > CHANNEL_HOP_INTERVAL_MS) {
    lastChannelHop = now;
    currentChannel++;
    if (currentChannel > MAX_CHANNELS) {
      currentChannel = 1;
    }
    esp_wifi_set_channel(currentChannel, WIFI_SECOND_CHAN_NONE);
  }

  // Turn off alarm LED if no attack in last 100ms
  static unsigned long lastLedOff = 0;
  if (now - lastLedOff > 100) {
    lastLedOff = now;
    digitalWrite(LED_BUILTIN, LOW);
  }

  // Periodic Telemetry every 5 seconds
  if (now - lastStatsPrint > 5000) {
    lastStatsPrint = now;
    Serial.printf("[STATUS] Channel: %2d | Deauth Frames: %lu | Disassoc Frames: %lu\n",
                  currentChannel, deauthCount, disassocCount);
  }

  delay(10);
}
