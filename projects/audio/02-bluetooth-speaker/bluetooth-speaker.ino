/*
 * ESP32-Community | Audio Project 02: Bluetooth A2DP Audio Sink (Hi-Fi Speaker)
 * 
 * Streams lossless wireless audio from any smartphone, tablet, or PC to an
 * external I2S Digital-to-Analog Converter (MAX98357A or PCM5102A).
 * 
 * Part of ESP32-Community Hub: https://github.com/tushar2704/ESP32-Community
 * Video Tutorials & Community: @IamNotIntoTech | @Makerrdesigns
 */

#include <Arduino.h>
#include "BluetoothA2DPSink.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN 2
#endif

// I2S DAC Pinout (MAX98357A / PCM5102A / UDA1334A)
const int I2S_BCLK_PIN = 26;  // Bit Clock
const int I2S_LRC_PIN  = 25;  // Word Select / Left-Right Clock
const int I2S_DOUT_PIN = 22;  // Serial Data Out

BluetoothA2DPSink a2dp_sink;

// State callback
void connection_state_changed(esp_a2d_connection_state_t state, void *ptr) {
  Serial.print("[A2DP] Connection state changed: ");
  if (state == ESP_A2D_CONNECTION_STATE_DISCONNECTED) {
    Serial.println("Disconnected");
    digitalWrite(LED_BUILTIN, LOW);
  } else if (state == ESP_A2D_CONNECTION_STATE_CONNECTING) {
    Serial.println("Connecting...");
  } else if (state == ESP_A2D_CONNECTION_STATE_CONNECTED) {
    Serial.println("CONNECTED! Audio stream ready.");
    digitalWrite(LED_BUILTIN, HIGH);
  } else if (state == ESP_A2D_CONNECTION_STATE_DISCONNECTING) {
    Serial.println("Disconnecting...");
  }
}

// Audio state callback
void audio_state_changed(esp_a2d_audio_state_t state, void *ptr) {
  if (state == ESP_A2D_AUDIO_STATE_STARTED) {
    Serial.println("[A2DP] Audio playback STARTED");
  } else if (state == ESP_A2D_AUDIO_STATE_STOPPED) {
    Serial.println("[A2DP] Audio playback STOPPED");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("==================================================");
  Serial.println("  ESP32 Community: Bluetooth A2DP Hi-Fi Speaker   ");
  Serial.println("==================================================");
  Serial.printf("I2S BCLK: GPIO %d | LRC: GPIO %d | DOUT: GPIO %d\n",
                I2S_BCLK_PIN, I2S_LRC_PIN, I2S_DOUT_PIN);

  // Configure custom I2S pins
  i2s_pin_config_t my_pin_config = {
    .bck_io_num = I2S_BCLK_PIN,
    .ws_io_num = I2S_LRC_PIN,
    .data_out_num = I2S_DOUT_PIN,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
  a2dp_sink.set_pin_config(my_pin_config);

  // Register telemetry callbacks
  a2dp_sink.set_on_connection_state_changed(connection_state_changed);
  a2dp_sink.set_on_audio_state_changed(audio_state_changed);

  // Start Bluetooth A2DP Sink broadcast
  a2dp_sink.start("ESP32-Community-Speaker");
  Serial.println("[A2DP] Advertising as 'ESP32-Community-Speaker'");
  Serial.println("Pair your smartphone or laptop now!");
  Serial.println("--------------------------------------------------");
}

void loop() {
  // A2DP Sink runs asynchronously on Core 0 via FreeRTOS background tasks.
  // We can perform non-blocking telemetry or UI updates here.
  delay(1000);
}
