/**
 * @file esp-now-mesh.ino
 * @brief Project ADV-04: ESP-NOW Routerless Peer-to-Peer Communicator
 * @author Tushar Aggarwal (@tushar2704)
 * @url https://github.com/tushar2704/ESP32-Community
 * @license AGPL-3.0
 */

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_mac.h>

// Forward declaration for compatibility across Arduino core versions
#if !defined(ESP_IDF_VERSION_MAJOR) || (ESP_IDF_VERSION_MAJOR < 5)
struct esp_now_recv_info_t {};
#endif

// Global Broadcast Address (FF:FF:FF:FF:FF:FF sends to all listening ESP32s on same channel)
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Structured telemetry packet
typedef struct __attribute__((packed)) {
    uint32_t message_id;
    uint8_t  sender_mac[6];
    float    temperature;
    uint32_t uptime_seconds;
    char     payload_text[32];
} MeshPacket;

MeshPacket tx_packet;
MeshPacket rx_packet;

// Hardware Transmission Callback (Compatible with Arduino Core v2 and v3)
#if defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3)
void onDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
    Serial.printf("📤 [TX] Packet Transmission Completed — Status: %s\n",
                  status == ESP_NOW_SEND_SUCCESS ? "SUCCESS (ACK Received) ✅" : "FAILED (No ACK) ❌");
}
#else
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.printf("📤 [TX] Packet sent to %02X:%02X:%02X:%02X:%02X:%02X — Delivery Status: %s\n",
                  mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5],
                  status == ESP_NOW_SEND_SUCCESS ? "SUCCESS (ACK Received) ✅" : "FAILED (No ACK) ❌");
}
#endif

// Hardware Reception Callback (Compatible with Arduino Core v2 and v3)
#if defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3)
void onDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
    const uint8_t *mac_addr = recv_info->src_addr;
#else
void onDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
#endif
    if (len == sizeof(MeshPacket)) {
        memcpy(&rx_packet, incomingData, sizeof(MeshPacket));
        Serial.println("\n📥 [RX] Incoming ESP-NOW Packet Received!");
        Serial.printf("  - From Node MAC:   %02X:%02X:%02X:%02X:%02X:%02X\n",
                      mac_addr[0], mac_addr[1], mac_addr[2],
                      mac_addr[3], mac_addr[4], mac_addr[5]);
        Serial.printf("  - Msg ID:          #%u\n", rx_packet.message_id);
        Serial.printf("  - Node Temp:       %.2f °C\n", rx_packet.temperature);
        Serial.printf("  - Node Uptime:     %u sec\n", rx_packet.uptime_seconds);
        Serial.printf("  - Payload Message: '%s'\n", rx_packet.payload_text);
        Serial.println("--------------------------------------------------");
    }
}

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("\n========================================================");
    Serial.println("  ⚡ ESP32 Community — Project ADV-04: ESP-NOW P2P Mesh");
    Serial.println("========================================================");

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    Serial.printf("🆔 Local Device MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("❌ Error initializing ESP-NOW");
        return;
    }
    Serial.println(" ESP-NOW protocol engine initialized!");

    // Register Callbacks
    esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataRecv);

    // Register Broadcast Peer
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  // 0 = current Wi-Fi channel
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("❌ Failed to register broadcast peer");
        return;
    }
    Serial.println(" Broadcast peer registered! Ready to transmit & receive.\n");
}

void loop() {
    static uint32_t counter = 0;
    counter++;

    tx_packet.message_id = counter;
    esp_read_mac(tx_packet.sender_mac, ESP_MAC_WIFI_STA);
    tx_packet.temperature = 24.5f + (float)(rand() % 50) / 10.0f;
    tx_packet.uptime_seconds = millis() / 1000;
    snprintf(tx_packet.payload_text, sizeof(tx_packet.payload_text), "ESP32 Heartbeat #%u", counter);

    // Send packet over raw 2.4 GHz
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&tx_packet, sizeof(MeshPacket));
    if (result != ESP_OK) {
        Serial.printf("❌ Send error: %d\n", result);
    }

    delay(5000); // Send heartbeat every 5s
}
