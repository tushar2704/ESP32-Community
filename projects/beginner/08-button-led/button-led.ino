/*
 * ESP32-Community | Beginner Project 08: Button Input & Debouncing
 * 
 * Demonstrates rock-solid digital button handling with:
 * - Internal Pull-up resistor (no external resistor required)
 * - Software debounce algorithm
 * - Multi-action detection: Single Click, Double Click, and Long Press (>1 sec)
 * 
 * Part of ESP32-Community Hub: https://github.com/tushar2704/ESP32-Community
 * Video Tutorials & Community: @IamNotIntoTech | @Makerrdesigns
 */

#include <Arduino.h>

#ifndef LED_BUILTIN
  #define LED_BUILTIN 2
#endif

// Button connected between GPIO 0 (or GPIO 4) and GND
#define BUTTON_PIN 0  // Built-in BOOT button on almost all ESP32 boards

// Timing thresholds (milliseconds)
const unsigned long DEBOUNCE_TIME_MS = 50;
const unsigned long DOUBLE_CLICK_TIME_MS = 350;
const unsigned long LONG_PRESS_TIME_MS = 1000;

// Button state variables
int lastSteadyState = HIGH;      // Previous steady state from input pin
int lastFlickerableState = HIGH;  // Previous flickerable state from input pin
unsigned long lastDebounceTime = 0;

unsigned long buttonPressTime = 0;
unsigned long buttonReleaseTime = 0;
bool isLongPress = false;
int clickCount = 0;

// LED Modes: 0 = OFF, 1 = ON, 2 = BLINK_FAST, 3 = BLINK_SLOW
enum LedMode {
  LED_MODE_OFF,
  LED_MODE_ON,
  LED_MODE_BLINK_FAST,
  LED_MODE_BLINK_SLOW
};
LedMode currentMode = LED_MODE_OFF;

void setMode(LedMode newMode) {
  currentMode = newMode;
  Serial.print("[MODE CHANGED] ");
  switch (currentMode) {
    case LED_MODE_OFF:        Serial.println("LED OFF"); break;
    case LED_MODE_ON:         Serial.println("LED SOLID ON"); break;
    case LED_MODE_BLINK_FAST: Serial.println("LED BLINK FAST (100ms)"); break;
    case LED_MODE_BLINK_SLOW: Serial.println("LED BLINK SLOW (500ms)"); break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Configure button with internal pull-up
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println("==================================================");
  Serial.println("   ESP32 Community: Debounced Multi-Action Button ");
  Serial.println("==================================================");
  Serial.printf("Button Pin: GPIO %d (Internal PULLUP active)\n", BUTTON_PIN);
  Serial.println("Instructions:");
  Serial.println(" - 1 Click:       Toggle LED On/Off");
  Serial.println(" - 2 Clicks:      Fast Blink Mode");
  Serial.println(" - Long Press:    Slow Pulse Mode");
  Serial.println("--------------------------------------------------");
}

void loop() {
  int currentState = digitalRead(BUTTON_PIN);

  // Check if button state changed due to noise or pressing
  if (currentState != lastFlickerableState) {
    lastDebounceTime = millis();
    lastFlickerableState = currentState;
  }

  // Once signal remains stable for DEBOUNCE_TIME_MS
  if ((millis() - lastDebounceTime) > DEBOUNCE_TIME_MS) {
    if (lastSteadyState != currentState) {
      lastSteadyState = currentState;

      if (lastSteadyState == LOW) {
        // Button Pressed down
        buttonPressTime = millis();
        isLongPress = false;
      } else {
        // Button Released
        buttonReleaseTime = millis();
        unsigned long pressDuration = buttonReleaseTime - buttonPressTime;

        if (pressDuration >= LONG_PRESS_TIME_MS) {
          // Trigger Long Press
          isLongPress = true;
          clickCount = 0;
          Serial.printf("[BUTTON EVENT] Long Press (%lu ms)\n", pressDuration);
          setMode(LED_MODE_BLINK_SLOW);
        } else {
          // Potential click
          clickCount++;
        }
      }
    }
  }

  // Handle multi-click timeout
  if (clickCount > 0 && !isLongPress) {
    if ((millis() - buttonReleaseTime) > DOUBLE_CLICK_TIME_MS) {
      if (clickCount == 1) {
        Serial.println("[BUTTON EVENT] Single Click");
        if (currentMode == LED_MODE_ON) {
          setMode(LED_MODE_OFF);
        } else {
          setMode(LED_MODE_ON);
        }
      } else if (clickCount >= 2) {
        Serial.printf("[BUTTON EVENT] Double Click (%d clicks)\n", clickCount);
        setMode(LED_MODE_BLINK_FAST);
      }
      clickCount = 0;
    }
  }

  // Update LED according to current active mode
  static unsigned long lastBlink = 0;
  static bool blinkState = false;

  switch (currentMode) {
    case LED_MODE_OFF:
      digitalWrite(LED_BUILTIN, LOW);
      break;

    case LED_MODE_ON:
      digitalWrite(LED_BUILTIN, HIGH);
      break;

    case LED_MODE_BLINK_FAST:
      if (millis() - lastBlink >= 100) {
        lastBlink = millis();
        blinkState = !blinkState;
        digitalWrite(LED_BUILTIN, blinkState ? HIGH : LOW);
      }
      break;

    case LED_MODE_BLINK_SLOW:
      if (millis() - lastBlink >= 500) {
        lastBlink = millis();
        blinkState = !blinkState;
        digitalWrite(LED_BUILTIN, blinkState ? HIGH : LOW);
      }
      break;
  }
}
