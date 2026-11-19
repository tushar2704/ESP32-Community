/**
 * ESP32-Community: Native USB HID Macro Pad & Media Volume Controller
 * Powered by ESP32-S3 native USB OTG stack (USBHIDKeyboard & USBHIDConsumerControl).
 *
 * Hardware:
 *   - Rotary Encoder: CLK (GPIO 4), DT (GPIO 5), SW / Mute Button (GPIO 6)
 *   - 4 Mechanical Switches:
 *       Key 1 (GPIO 7): Play / Pause
 *       Key 2 (GPIO 8): Next Track
 *       Key 3 (GPIO 9): VSCode Command Palette (Ctrl+Shift+P / Cmd+Shift+P)
 *       Key 4 (GPIO 10): Discord / Slack Mute Toggle (Ctrl+Shift+M / Cmd+Shift+M)
 *
 * Compatible with ESP32-S3 and ESP32-S2.
 */

#include <Arduino.h>
#include <FS.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDConsumerControl.h"

// --- Pin Definitions ---
#define PIN_ENC_CLK  4
#define PIN_ENC_DT   5
#define PIN_ENC_SW   6   // Encoder push button (Mute)

#define PIN_KEY_1    7   // Play / Pause
#define PIN_KEY_2    8   // Next Track
#define PIN_KEY_3    9   // Macro 1: Dev Palette
#define PIN_KEY_4    10  // Macro 2: Discord Mute

#define PIN_LED      2   // Activity LED

USBHIDKeyboard Keyboard;
USBHIDConsumerControl ConsumerControl;

// Rotary Encoder State
volatile int lastEncoded = 0;
volatile long encoderValue = 0;
long lastEncoderReported = 0;

void IRAM_ATTR handleEncoderISR() {
  int MSB = digitalRead(PIN_ENC_CLK);
  int LSB = digitalRead(PIN_ENC_DT);

  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 2) | encoded;

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue--;

  lastEncoded = encoded;
}

// Button Debounce Helpers
struct ButtonState {
  uint8_t pin;
  bool lastState;
  unsigned long lastDebounceTime;
};

ButtonState buttons[] = {
  {PIN_ENC_SW, HIGH, 0},
  {PIN_KEY_1, HIGH, 0},
  {PIN_KEY_2, HIGH, 0},
  {PIN_KEY_3, HIGH, 0},
  {PIN_KEY_4, HIGH, 0}
};

const unsigned long DEBOUNCE_DELAY_MS = 50;

void pulseLED() {
  digitalWrite(PIN_LED, HIGH);
  delayMicroseconds(2000);
  digitalWrite(PIN_LED, LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_ENC_CLK, INPUT_PULLUP);
  pinMode(PIN_ENC_DT, INPUT_PULLUP);
  pinMode(PIN_ENC_SW, INPUT_PULLUP);

  pinMode(PIN_KEY_1, INPUT_PULLUP);
  pinMode(PIN_KEY_2, INPUT_PULLUP);
  pinMode(PIN_KEY_3, INPUT_PULLUP);
  pinMode(PIN_KEY_4, INPUT_PULLUP);

  // Attach rotary interrupts
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_CLK), handleEncoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_DT), handleEncoderISR, CHANGE);

  // Initialize Native USB HID stack
  Keyboard.begin();
  ConsumerControl.begin();
  USB.begin();

  Serial.println("\n[ESP32-S3 USB HID Media Controller Ready]");
}

void loop() {
  // 1. Process Rotary Encoder Volume Turns
  if (encoderValue != lastEncoderReported) {
    long delta = encoderValue - lastEncoderReported;
    if (abs(delta) >= 2) { // 2 quadrature pulses per physical detent
      if (delta > 0) {
        Serial.println("🔊 Volume UP");
        ConsumerControl.press(CONSUMER_CONTROL_VOLUME_INCREMENT);
        ConsumerControl.release();
      } else {
        Serial.println("🔉 Volume DOWN");
        ConsumerControl.press(CONSUMER_CONTROL_VOLUME_DECREMENT);
        ConsumerControl.release();
      }
      pulseLED();
      lastEncoderReported = encoderValue;
    }
  }

  // 2. Process Debounced Push Buttons
  for (int i = 0; i < 5; i++) {
    bool reading = digitalRead(buttons[i].pin);
    if (reading != buttons[i].lastState) {
      if ((millis() - buttons[i].lastDebounceTime) > DEBOUNCE_DELAY_MS) {
        buttons[i].lastDebounceTime = millis();
        buttons[i].lastState = reading;

        // Button Pressed (Falling edge on INPUT_PULLUP)
        if (reading == LOW) {
          pulseLED();

          switch (buttons[i].pin) {
            case PIN_ENC_SW: // Encoder push -> Mute / Unmute
              Serial.println("🔇 Mute Toggle");
              ConsumerControl.press(CONSUMER_CONTROL_MUTE);
              ConsumerControl.release();
              break;

            case PIN_KEY_1: // Play / Pause
              Serial.println("⏯ Play/Pause");
              ConsumerControl.press(CONSUMER_CONTROL_PLAY_PAUSE);
              ConsumerControl.release();
              break;

            case PIN_KEY_2: // Next Track
              Serial.println("⏭ Next Track");
              ConsumerControl.press(CONSUMER_CONTROL_SCAN_NEXT);
              ConsumerControl.release();
              break;

            case PIN_KEY_3: // VSCode Command Palette (Ctrl+Shift+P)
              Serial.println("⌨ Macro: Dev Command Palette");
              Keyboard.press(KEY_LEFT_CTRL);
              Keyboard.press(KEY_LEFT_SHIFT);
              Keyboard.press('p');
              delay(50);
              Keyboard.releaseAll();
              break;

            case PIN_KEY_4: // Discord/Slack Mute Toggle (Ctrl+Shift+M)
              Serial.println("🎙 Macro: Discord/Slack Mute");
              Keyboard.press(KEY_LEFT_CTRL);
              Keyboard.press(KEY_LEFT_SHIFT);
              Keyboard.press('m');
              delay(50);
              Keyboard.releaseAll();
              break;
          }
        }
      }
    }
  }

  delay(5);
}
