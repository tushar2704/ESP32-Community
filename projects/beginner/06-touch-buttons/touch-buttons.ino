/*
 * ESP32-Community | Beginner Project 06: Capacitive Touch Buttons
 * 
 * Demonstrates reading the built-in capacitive touch sensing hardware
 * on the ESP32. Touch pins detect tiny changes in capacitance caused by
 * human skin contact with a wire or metal pad, eliminating the need
 * for physical moving pushbuttons!
 * 
 * Hardware Compatibility:
 * - ESP32 Classic: Touch pins T0 - T9 (GPIO 4, 0, 2, 15, 13, 12, 14, 27, 33, 32)
 * - ESP32-S2 / S3: Touch pins T1 - T14
 * - ESP32-C3 / C6 / H2: (Touch sensing hardware is not present on RISC-V variants)
 * 
 * Part of ESP32-Community Hub: https://github.com/tushar2704/ESP32-Community
 * Video Tutorials & Community: @IamNotIntoTech | @Makerrdesigns
 */

#include <Arduino.h>

// Onboard LED pin (GPIO 2 on classic ESP32 DevKit)
#ifndef LED_BUILTIN
  #define LED_BUILTIN 2
#endif

// Touch pad pin definition
// T0 = GPIO 4, T2 = GPIO 2, T3 = GPIO 15, T4 = GPIO 13, T7 = GPIO 27, T8 = GPIO 33, T9 = GPIO 32
#if defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3)
  const int TOUCH_PIN_1 = 1;  // GPIO 1 (T1) on S2/S3
  const int TOUCH_PIN_2 = 2;  // GPIO 2 (T2) on S2/S3
#else
  const int TOUCH_PIN_1 = 4;   // GPIO 4 (T0) on Classic ESP32
  const int TOUCH_PIN_2 = 14;  // GPIO 14 (T6) on Classic ESP32
#endif

// Sensitivity Threshold: Raw values drop significantly when touched.
// Untouched: ~70-90. Touched: ~15-35.
const int TOUCH_THRESHOLD = 30;

// State tracking
bool ledState = false;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_DELAY_MS = 250;

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("==================================================");
  Serial.println("  ESP32 Community: Capacitive Touch Sensing Demo  ");
  Serial.println("==================================================");
  Serial.printf("Touch Pad 1 attached to GPIO %d\n", TOUCH_PIN_1);
  Serial.printf("Touch Pad 2 attached to GPIO %d\n", TOUCH_PIN_2);
  Serial.printf("Detection Threshold set to: %d\n", TOUCH_THRESHOLD);
  Serial.println("Touch a jumper wire plugged into these pins to trigger!");
  Serial.println("--------------------------------------------------");
}

void loop() {
#if defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C6) || defined(CONFIG_IDF_TARGET_ESP32H2)
  Serial.println("[WARN] Capacitive touch hardware is not available on RISC-V (C3/C6/H2) chips.");
  delay(3000);
#else
  // Read raw capacitance levels (lower value = stronger touch)
  int val1 = touchRead(TOUCH_PIN_1);
  int val2 = touchRead(TOUCH_PIN_2);

  // Periodic debug telemetry
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 500) {
    lastPrint = millis();
    Serial.printf("[TOUCH SENSOR] Pad 1 (GPIO %d): %3d | Pad 2 (GPIO %d): %3d | LED: %s\n",
                  TOUCH_PIN_1, val1, TOUCH_PIN_2, val2, ledState ? "ON" : "OFF");
  }

  // Handle Touch Pad 1: Toggle LED
  if (val1 < TOUCH_THRESHOLD && (millis() - lastDebounceTime > DEBOUNCE_DELAY_MS)) {
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);
    lastDebounceTime = millis();
    Serial.printf(">>> Touch Pad 1 Touched! LED toggled to %s (Val: %d)\n",
                  ledState ? "HIGH" : "LOW", val1);
  }

  // Handle Touch Pad 2: Momentary Flash
  if (val2 < TOUCH_THRESHOLD) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.printf(">>> Touch Pad 2 Touched (Flash Action)! (Val: %d)\n", val2);
    delay(50);
    digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);
  }

  delay(20);
#endif
}
