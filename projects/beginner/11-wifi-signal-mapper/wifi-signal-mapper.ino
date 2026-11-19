/**
 * ESP32-Community: Portable WiFi Signal Mapper & Channel Analyzer
 * Features real-time RSSI graphing, 2.4GHz channel crowding histogram,
 * least-congested channel recommendation, and acoustic Geiger-style signal tracker.
 *
 * Compatible with ESP32-WROOM-32, ESP32-S3, ESP32-C3.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Pin Definitions ---
#if defined(CONFIG_IDF_TARGET_ESP32C3)
  #define I2C_SDA    4
  #define I2C_SCL    5
  #define PIN_BUZZER 6
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
  #define I2C_SDA    8
  #define I2C_SCL    9
  #define PIN_BUZZER 12
#else
  #define I2C_SDA    21
  #define I2C_SCL    22
  #define PIN_BUZZER 25
#endif

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Data Structures ---
struct AccessPointInfo {
  String ssid;
  int32_t rssi;
  int32_t channel;
};

const int MAX_NETWORKS = 30;
AccessPointInfo networks[MAX_NETWORKS];
int totalNetworks = 0;
int channelCounts[15] = {0}; // Channels 1 through 14

int currentDisplayPage = 0; // 0=List, 1=Spectrum Histogram, 2=Geiger Tracker
unsigned long lastPageSwitch = 0;
unsigned long lastScanTime = 0;
const unsigned long SCAN_INTERVAL_MS = 6000;

// Geiger-style audio tracker
int targetIndex = 0;
unsigned long lastGeigerClick = 0;

void performWiFiScan() {
  Serial.println("\n🔍 Scanning 2.4 GHz WiFi spectrum...");
  memset(channelCounts, 0, sizeof(channelCounts));

  int n = WiFi.scanNetworks(false, true); // Async=false, showHidden=true
  totalNetworks = 0;

  if (n > 0) {
    totalNetworks = min(n, MAX_NETWORKS);
    for (int i = 0; i < totalNetworks; i++) {
      networks[i].ssid = (WiFi.SSID(i).length() > 0) ? WiFi.SSID(i) : "[Hidden SSID]";
      networks[i].rssi = WiFi.RSSI(i);
      networks[i].channel = WiFi.channel(i);

      if (networks[i].channel >= 1 && networks[i].channel <= 14) {
        channelCounts[networks[i].channel]++;
      }

      Serial.printf("  [%2d] Ch %2d (%3d dBm): %s\n",
                    i + 1,
                    (int)networks[i].channel,
                    (int)networks[i].rssi,
                    networks[i].ssid.c_str());
    }
  } else {
    Serial.println("  No networks detected.");
  }

  WiFi.scanDelete();
}

int getRecommendedChannel() {
  // Common non-overlapping channels: 1, 6, 11
  int ch1 = channelCounts[1];
  int ch6 = channelCounts[6];
  int ch11 = channelCounts[11];

  if (ch1 <= ch6 && ch1 <= ch11) return 1;
  if (ch6 <= ch1 && ch6 <= ch11) return 6;
  return 11;
}

void renderNetworkList() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.printf("WiFi Scanner (%d APs)", totalNetworks);
  display.drawFastHLine(0, 9, 128, SSD1306_WHITE);

  if (totalNetworks == 0) {
    display.setCursor(16, 28);
    display.print("Scanning...");
    display.display();
    return;
  }

  // Display top 5 strongest APs
  int y = 13;
  for (int i = 0; i < min(5, totalNetworks); i++) {
    display.setCursor(0, y);
    String truncatedSSID = networks[i].ssid;
    if (truncatedSSID.length() > 10) {
      truncatedSSID = truncatedSSID.substring(0, 9) + "..";
    }
    display.printf("%-11s C%02d %ddB", 
                   truncatedSSID.c_str(), 
                   (int)networks[i].channel, 
                   (int)networks[i].rssi);
    y += 10;
  }

  display.display();
}

void renderSpectrumHistogram() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.printf("Channel Crowding (Rec: %d)", getRecommendedChannel());
  display.drawFastHLine(0, 9, 128, SSD1306_WHITE);

  // Draw 13 channel bars (Channels 1-13)
  int startX = 6;
  int barWidth = 6;
  int spacing = 9;
  int maxCount = 1;
  for (int ch = 1; ch <= 13; ch++) {
    if (channelCounts[ch] > maxCount) maxCount = channelCounts[ch];
  }

  for (int ch = 1; ch <= 13; ch++) {
    int x = startX + (ch - 1) * spacing;
    int barHeight = (channelCounts[ch] * 38) / maxCount;
    if (barHeight < 2 && channelCounts[ch] > 0) barHeight = 2;
    int y = 52 - barHeight;

    if (barHeight > 0) {
      display.fillRect(x, y, barWidth, barHeight, SSD1306_WHITE);
    }

    // Label key channels
    if (ch == 1 || ch == 6 || ch == 11) {
      display.setCursor(x, 56);
      display.print(ch);
    }
  }

  display.display();
}

void renderGeigerTracker() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("RSSI Geiger Tracker");
  display.drawFastHLine(0, 9, 128, SSD1306_WHITE);

  if (totalNetworks == 0) {
    display.setCursor(16, 28);
    display.print("Acquiring target...");
    display.display();
    return;
  }

  AccessPointInfo target = networks[0]; // Strongest network
  display.setCursor(0, 14);
  display.printf("Target: %s", target.ssid.substring(0, 14).c_str());
  display.setCursor(0, 24);
  display.printf("Channel: %d | RSSI: %d dBm", (int)target.channel, (int)target.rssi);

  // Progress / Signal Bar
  int signalPercent = constrain(map(target.rssi, -95, -30, 0, 100), 0, 100);
  display.drawRect(0, 36, 128, 12, SSD1306_WHITE);
  int fillWidth = (signalPercent * 124) / 100;
  display.fillRect(2, 38, fillWidth, 8, SSD1306_WHITE);

  display.setCursor(40, 52);
  display.printf("Signal: %d%%", signalPercent);

  display.display();

  // Geiger Click Sound: stronger RSSI = faster click rate
  // -30 dBm -> ~60ms period, -90 dBm -> ~1000ms period
  unsigned long clickInterval = map(constrain((int)target.rssi, -95, -30), -95, -30, 900, 60);
  if (millis() - lastGeigerClick >= clickInterval) {
    lastGeigerClick = millis();
    digitalWrite(PIN_BUZZER, HIGH);
    delayMicroseconds(600); // Sharp transient click
    digitalWrite(PIN_BUZZER, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n[ESP32 WiFi Signal Mapper & Channel Analyzer]");

  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  Wire.begin(I2C_SDA, I2C_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 OLED allocation failed (check address 0x3C/0x3D)");
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 18);
    display.println("ESP32 Community");
    display.setCursor(10, 32);
    display.println("WiFi Signal Mapper");
    display.display();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  performWiFiScan();
  lastScanTime = millis();
  lastPageSwitch = millis();
}

void loop() {
  // Refresh scan periodically
  if (millis() - lastScanTime >= SCAN_INTERVAL_MS) {
    performWiFiScan();
    lastScanTime = millis();
  }

  // Switch display pages every 4 seconds (List -> Histogram -> Geiger)
  if (millis() - lastPageSwitch >= 4000) {
    currentDisplayPage = (currentDisplayPage + 1) % 3;
    lastPageSwitch = millis();
  }

  // Render active page
  switch (currentDisplayPage) {
    case 0:
      renderNetworkList();
      break;
    case 1:
      renderSpectrumHistogram();
      break;
    case 2:
      renderGeigerTracker();
      break;
  }

  delay(20);
}
