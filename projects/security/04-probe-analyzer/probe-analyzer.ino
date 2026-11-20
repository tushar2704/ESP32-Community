/**
 * ESP32-Community: 802.11 Wi-Fi Probe Request Analyzer & Privacy Monitor
 * 
 * Captures 802.11 Management Probe Request frames in promiscuous mode.
 * Identifies mobile devices searching for saved Wi-Fi networks, evaluates
 * randomized vs persistent MAC addresses, and tracks RSSI signal proximity.
 *
 * For educational, authorized network analysis, and privacy auditing only.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#define PIN_STATUS_LED 2
#define MAX_HISTORY 25

struct ProbeRecord {
  char mac[18];
  char ssid[33];
  int8_t rssi;
  uint8_t channel;
  bool isRandomized;
  uint32_t timestamp;
};

ProbeRecord history[MAX_HISTORY];
size_t historyCount = 0;
uint32_t totalProbes = 0;
uint32_t randomizedCount = 0;
uint32_t broadcastProbes = 0;

WebServer server(80);

// Format MAC into string
void formatMac(const uint8_t *mac, char *outStr) {
  snprintf(outStr, 18, "%02X:%02X:%02X:%02X:%02X:%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

// 802.11 Promiscuous Packet Sniffer Callback
void snifferCallback(void *buf, wifi_promiscuous_pkt_type_t type) {
  if (type != WIFI_PKT_MGMT) return;

  const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
  const uint8_t *payload = pkt->payload;
  uint16_t len = pkt->rx_ctrl.sig_len;

  if (len < 26) return;

  // Frame Control Check: Type 0 (Mgmt), Subtype 4 (Probe Request)
  // Byte 0: [Subtype (4 bits) | Type (2 bits) | Version (2 bits)]
  uint8_t frameControl = payload[0];
  uint8_t frameType = (frameControl >> 2) & 0x03;
  uint8_t frameSubtype = (frameControl >> 4) & 0x0F;

  if (frameType != 0 || frameSubtype != 4) return;

  // Transmitter MAC address is at offset 10..15 in 802.11 header
  const uint8_t *srcMac = &payload[10];
  bool randomized = (srcMac[0] & 0x02) != 0;

  // Extract SSID from Tagged Information Elements (starts at offset 24)
  char ssid[33] = {0};
  size_t offset = 24;
  while (offset + 2 <= len) {
    uint8_t tagNum = payload[offset];
    uint8_t tagLen = payload[offset + 1];
    offset += 2;

    if (tagNum == 0) { // Tag 0 = SSID parameter
      if (tagLen > 0 && tagLen <= 32 && (offset + tagLen) <= len) {
        memcpy(ssid, &payload[offset], tagLen);
        ssid[tagLen] = '\0';
      } else {
        strcpy(ssid, "<Wildcard/Broadcast>");
      }
      break;
    }
    offset += tagLen;
  }

  if (strlen(ssid) == 0) {
    strcpy(ssid, "<Wildcard/Broadcast>");
  }

  // Update Statistics
  totalProbes++;
  if (randomized) randomizedCount++;
  if (strcmp(ssid, "<Wildcard/Broadcast>") == 0) broadcastProbes++;

  digitalWrite(PIN_STATUS_LED, HIGH);

  // Store in circular history buffer
  char macStr[18];
  formatMac(srcMac, macStr);

  // Shift if full
  if (historyCount < MAX_HISTORY) {
    historyCount++;
  }
  for (size_t i = historyCount - 1; i > 0; i--) {
    history[i] = history[i - 1];
  }

  strncpy(history[0].mac, macStr, 18);
  strncpy(history[0].ssid, ssid, 33);
  history[0].rssi = pkt->rx_ctrl.rssi;
  history[0].channel = pkt->rx_ctrl.channel;
  history[0].isRandomized = randomized;
  history[0].timestamp = millis();

  Serial.printf("[Probe] MAC: %s (%s) | SSID: %-20s | RSSI: %3d dBm | Ch: %2d\n",
                macStr, randomized ? "Random" : "Global", ssid, pkt->rx_ctrl.rssi, pkt->rx_ctrl.channel);

  digitalWrite(PIN_STATUS_LED, LOW);
}

// Web Server API & Dashboard
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Wi-Fi Probe Analyzer</title>
  <style>
    body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, monospace; background: #0b0f19; color: #f8fafc; margin: 0; padding: 20px; }
    .container { max-width: 900px; margin: 0 auto; }
    h1 { color: #f43f5e; margin-bottom: 5px; }
    .subtitle { color: #94a3b8; font-size: 0.9rem; margin-bottom: 20px; }
    .stats-bar { display: grid; grid-template-columns: repeat(auto-fit, minmax(180px, 1fr)); gap: 12px; margin-bottom: 20px; }
    .stat-card { background: #1e293b; border: 1px solid #334155; border-radius: 8px; padding: 14px; text-align: center; }
    .stat-card h4 { margin: 0 0 6px 0; font-size: 0.75rem; text-transform: uppercase; color: #94a3b8; }
    .stat-card .val { font-size: 1.6rem; font-weight: bold; color: #38bdf8; }
    table { width: 100%; border-collapse: collapse; background: #1e293b; border-radius: 8px; overflow: hidden; border: 1px solid #334155; }
    th, td { padding: 10px 14px; text-align: left; font-size: 0.85rem; border-bottom: 1px solid #334155; }
    th { background: #0f172a; color: #94a3b8; text-transform: uppercase; font-size: 0.75rem; }
    .badge { padding: 2px 8px; border-radius: 4px; font-size: 0.75rem; font-weight: 600; }
    .badge-rand { background: #0284c7; color: white; }
    .badge-glob { background: #e11d48; color: white; }
  </style>
</head>
<body>
  <div class="container">
    <h1>📡 Wi-Fi Probe Request Analyzer</h1>
    <div class="subtitle">Promiscuous 802.11 Sniffer & Privacy Awareness Monitor</div>
    <div class="stats-bar">
      <div class="stat-card"><h4>Total Probes</h4><div class="val" id="total">0</div></div>
      <div class="stat-card"><h4>Randomized MACs</h4><div class="val" id="rand">0</div></div>
      <div class="stat-card"><h4>Broadcast Probes</h4><div class="val" id="bcast">0</div></div>
    </div>
    <table>
      <thead>
        <tr>
          <th>Time</th>
          <th>Source MAC</th>
          <th>Type</th>
          <th>Target SSID</th>
          <th>RSSI</th>
          <th>Channel</th>
        </tr>
      </thead>
      <tbody id="table-body">
        <tr><td colspan="6" style="text-align:center;">Listening for probe requests...</td></tr>
      </tbody>
    </table>
  </div>
  <script>
    function update() {
      fetch('/api/probes')
        .then(r => r.json())
        .then(d => {
          document.getElementById('total').innerText = d.total;
          document.getElementById('rand').innerText = d.randomized;
          document.getElementById('bcast').innerText = d.broadcast;
          const tbody = document.getElementById('table-body');
          if (d.probes.length === 0) return;
          tbody.innerHTML = d.probes.map(p => `
            <tr>
              <td>${Math.floor((Date.now()/1000) - (p.ago/1000))}s ago</td>
              <td><code>${p.mac}</code></td>
              <td><span class="badge ${p.isRandom ? 'badge-rand' : 'badge-glob'}">${p.isRandom ? 'Random' : 'Global OUI'}</span></td>
              <td><strong>${p.ssid}</strong></td>
              <td>${p.rssi} dBm</td>
              <td>Ch ${p.channel}</td>
            </tr>
          `).join('');
        });
    }
    setInterval(update, 2000);
    update();
  </script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html", html);
}

void handleApiProbes() {
  StaticJsonDocument<2048> doc;
  doc["total"] = totalProbes;
  doc["randomized"] = randomizedCount;
  doc["broadcast"] = broadcastProbes;

  JsonArray arr = doc.createNestedArray("probes");
  for (size_t i = 0; i < historyCount; i++) {
    JsonObject obj = arr.createNestedObject();
    obj["mac"] = history[i].mac;
    obj["ssid"] = history[i].ssid;
    obj["rssi"] = history[i].rssi;
    obj["channel"] = history[i].channel;
    obj["isRandom"] = history[i].isRandomized;
    obj["ago"] = millis() - history[i].timestamp;
  }

  String res;
  serializeJson(doc, res);
  server.send(200, "application/json", res);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(PIN_STATUS_LED, OUTPUT);

  Serial.println("\n--- [ESP32-Community] 802.11 Probe Request Analyzer ---");

  // SoftAP for Web Dashboard
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("ESP32-Probe-Analyzer", "12345678");
  Serial.printf("[AP] Web Dashboard active at http://%s/\n", WiFi.softAPIP().toString().c_str());

  server.on("/", handleRoot);
  server.on("/api/probes", handleApiProbes);
  server.begin();

  // Enable Wi-Fi Promiscuous Sniffer mode
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&snifferCallback);

  wifi_promiscuous_filter_t filter;
  filter.filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT;
  esp_wifi_set_promiscuous_filter(&filter);

  Serial.println("[Sniffer] Promiscuous 802.11 sniffer started on 2.4GHz.");
}

void loop() {
  server.handleClient();

  // Channel hopping across channels 1..13 every 500ms
  static uint32_t lastHop = 0;
  static uint8_t ch = 1;
  if (millis() - lastHop >= 500) {
    lastHop = millis();
    ch = (ch % 13) + 1;
    esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
  }
}
