/**
 * ESP32-Community: Real-Time Digital ESP-NOW Walkie-Talkie
 * 
 * Hardware:
 *   - 2x ESP32 DevKit boards
 *   - INMP441 I2S MEMS Microphone module
 *   - MAX98357A I2S 3W Class-D Amplifier DAC + 4Ω/8Ω Speaker
 *   - Push-To-Talk (PTT) tactile switch
 *   - Status LED (GPIO 2)
 *
 * Pin Connections:
 *   INMP441 Microphone (I2S Port 0 - Input):
 *     - SCK (BCLK) -> GPIO 14
 *     - WS (LRCLK) -> GPIO 15
 *     - SD (DATA)  -> GPIO 32
 *     - L/R        -> GND (Left channel)
 *     - VDD        -> 3.3V
 *     - GND        -> GND
 *
 *   MAX98357A DAC Amplifier (I2S Port 1 - Output):
 *     - BCLK -> GPIO 26
 *     - LRC  -> GPIO 27
 *     - DIN  -> GPIO 22
 *     - GAIN -> GND (12dB gain)
 *     - SD   -> Not connected (or tied to 3.3V)
 *     - VIN  -> 5V (or 3.3V)
 *     - GND  -> GND
 *
 *   Push-to-Talk (PTT) Switch:
 *     - GPIO 13 -> Button to GND (Internal Pull-Up)
 */

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <driver/i2s.h>

#define PIN_PTT         13
#define PIN_STATUS_LED  2

// I2S Microphone Configuration (I2S_NUM_0)
#define I2S_MIC_PORT    I2S_NUM_0
#define PIN_MIC_BCLK    14
#define PIN_MIC_WS      15
#define PIN_MIC_SD      32

// I2S Speaker DAC Configuration (I2S_NUM_1)
#define I2S_SPK_PORT    I2S_NUM_1
#define PIN_SPK_BCLK    26
#define PIN_SPK_LRC     27
#define PIN_SPK_DIN     22

#define SAMPLE_RATE     11025
#define CHUNK_SIZE      128    // Number of 16-bit audio samples per packet (250 bytes max payload)

// Broadcast MAC address
uint8_t broadcastAddress[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

// Packet structure for audio stream
struct AudioPacket {
  uint16_t seq;
  int16_t  samples[CHUNK_SIZE];
};

AudioPacket txPacket;
AudioPacket rxPacket;
uint16_t packetCounter = 0;
bool isTransmitting = false;

// Initialize I2S Microphone (Master RX)
void initI2SMic() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = CHUNK_SIZE,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = PIN_MIC_BCLK,
    .ws_io_num = PIN_MIC_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = PIN_MIC_SD
  };

  i2s_driver_install(I2S_MIC_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_MIC_PORT, &pin_config);
  i2s_zero_dma_buffer(I2S_MIC_PORT);
}

// Initialize I2S Speaker DAC (Master TX)
void initI2SSpeaker() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = CHUNK_SIZE,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = PIN_SPK_BCLK,
    .ws_io_num = PIN_SPK_LRC,
    .data_out_num = PIN_SPK_DIN,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install(I2S_SPK_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_SPK_PORT, &pin_config);
  i2s_zero_dma_buffer(I2S_SPK_PORT);
}

// ESP-NOW Receive Callback
void onDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
  if (isTransmitting) return; // Do not play back while transmitting (half-duplex)

  if (len == sizeof(AudioPacket)) {
    memcpy(&rxPacket, data, sizeof(AudioPacket));
    size_t bytesWritten = 0;
    i2s_write(I2S_SPK_PORT, rxPacket.samples, sizeof(rxPacket.samples), &bytesWritten, portMAX_DELAY);
    digitalWrite(PIN_STATUS_LED, !digitalRead(PIN_STATUS_LED));
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- [ESP32-Community] Digital ESP-NOW Walkie-Talkie ---");

  pinMode(PIN_PTT, INPUT_PULLUP);
  pinMode(PIN_STATUS_LED, OUTPUT);
  digitalWrite(PIN_STATUS_LED, LOW);

  // Initialize I2S Audio Peripherals
  initI2SMic();
  initI2SSpeaker();
  Serial.println("[Audio] I2S Microphone and Speaker DAC drivers active at 11,025 Hz.");

  // Wi-Fi Station Mode for ESP-NOW
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Serial.printf("[WiFi] MAC Address: %s\n", WiFi.macAddress().c_str());

  if (esp_now_init() != ESP_OK) {
    Serial.println("[ESP-NOW] Initialization failed!");
    return;
  }

  esp_now_register_recv_cb(onDataRecv);

  // Register Broadcast Peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("[ESP-NOW] Failed to add broadcast peer!");
  } else {
    Serial.println("[ESP-NOW] Ready! Hold PTT button on GPIO 13 to transmit audio.");
  }
}

void loop() {
  bool pttPressed = (digitalRead(PIN_PTT) == LOW);

  if (pttPressed) {
    if (!isTransmitting) {
      isTransmitting = true;
      digitalWrite(PIN_STATUS_LED, HIGH);
      Serial.println("[Walkie] >>> Transmitting audio...");
    }

    size_t bytesRead = 0;
    i2s_read(I2S_MIC_PORT, txPacket.samples, sizeof(txPacket.samples), &bytesRead, portMAX_DELAY);

    if (bytesRead > 0) {
      txPacket.seq = ++packetCounter;
      esp_now_send(broadcastAddress, (uint8_t *)&txPacket, sizeof(AudioPacket));
    }
  } else {
    if (isTransmitting) {
      isTransmitting = false;
      digitalWrite(PIN_STATUS_LED, LOW);
      Serial.println("[Walkie] <<< Listening...");
    }
    delay(10);
  }
}
