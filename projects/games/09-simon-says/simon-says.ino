/**
 * ESP32-Community: Classic Simon Says Hardware Memory Game
 * 
 * Hardware:
 *   - ESP32 DevKit (or S3 / C3)
 *   - 4x Tactile Pushbuttons (Red, Green, Blue, Yellow)
 *   - 4x 5mm LEDs (Red, Green, Blue, Yellow) with 220Ω series resistors
 *   - 1x Passive Piezo Buzzer
 *
 * Pin Connections:
 *   - Button Red    -> GPIO 18 (GND on press)
 *   - Button Green  -> GPIO 19 (GND on press)
 *   - Button Blue   -> GPIO 23 (GND on press)
 *   - Button Yellow -> GPIO 5  (GND on press)
 *   - LED Red       -> GPIO 16 (via 220Ω to GND)
 *   - LED Green     -> GPIO 17 (via 220Ω to GND)
 *   - LED Blue      -> GPIO 21 (via 220Ω to GND)
 *   - LED Yellow    -> GPIO 22 (via 220Ω to GND)
 *   - Piezo Buzzer  -> GPIO 25
 */

#include <Arduino.h>
#include <Preferences.h>

#define NUM_BUTTONS 4

const uint8_t BTN_PINS[NUM_BUTTONS] = { 18, 19, 23, 5 };
const uint8_t LED_PINS[NUM_BUTTONS] = { 16, 17, 21, 22 };
const int     TONES[NUM_BUTTONS]    = { 261, 329, 392, 523 }; // C4, E4, G4, C5

#define PIN_BUZZER 25
#define MAX_SEQUENCE 100

Preferences prefs;
uint8_t sequence[MAX_SEQUENCE];
uint16_t currentLevel = 0;
uint16_t highScore = 0;

enum GameState {
  STATE_WELCOME,
  STATE_PLAY_SEQUENCE,
  STATE_WAIT_INPUT,
  STATE_GAME_OVER,
  STATE_VICTORY
};

GameState state = STATE_WELCOME;
uint16_t playerStep = 0;
uint32_t lastInputTime = 0;

void playTone(int freq, int durationMs) {
  #if defined(ESP32)
  tone(PIN_BUZZER, freq, durationMs);
  #endif
}

void activateColor(int index, int durationMs) {
  digitalWrite(LED_PINS[index], HIGH);
  playTone(TONES[index], durationMs);
  delay(durationMs);
  digitalWrite(LED_PINS[index], LOW);
  delay(50);
}

void allLeds(bool on) {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    digitalWrite(LED_PINS[i], on ? HIGH : LOW);
  }
}

void playWelcomeAnimation() {
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < NUM_BUTTONS; i++) {
      activateColor(i, 80);
    }
  }
}

void startNewGame() {
  currentLevel = 1;
  playerStep = 0;
  // Generate first step
  sequence[0] = random(0, NUM_BUTTONS);
  state = STATE_PLAY_SEQUENCE;
  Serial.printf("\n--- [Simon Says] New Game Started! High Score: %u ---\n", highScore);
  delay(600);
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0) + esp_random());

  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(BTN_PINS[i], INPUT_PULLUP);
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }
  pinMode(PIN_BUZZER, OUTPUT);

  prefs.begin("simon", false);
  highScore = prefs.getUInt("high", 0);

  Serial.println("\n--- [ESP32-Community] Simon Says Memory Game ---");
  Serial.printf("All-time High Score: %u\n", highScore);
  Serial.println("Press any button to begin!");

  playWelcomeAnimation();
}

void loop() {
  switch (state) {
    case STATE_WELCOME: {
      for (int i = 0; i < NUM_BUTTONS; i++) {
        if (digitalRead(BTN_PINS[i]) == LOW) {
          activateColor(i, 150);
          startNewGame();
          return;
        }
      }
      break;
    }

    case STATE_PLAY_SEQUENCE: {
      delay(400);
      Serial.printf("[Level %u] Sequence Length: %u\n", currentLevel, currentLevel);
      
      // Speed scales up with higher levels
      int noteDuration = max(120, 350 - (currentLevel * 10));
      for (int i = 0; i < currentLevel; i++) {
        activateColor(sequence[i], noteDuration);
      }

      playerStep = 0;
      lastInputTime = millis();
      state = STATE_WAIT_INPUT;
      break;
    }

    case STATE_WAIT_INPUT: {
      // 5-second timeout for player reaction
      if (millis() - lastInputTime > 5000) {
        Serial.println("[Timeout] Player failed to respond in time!");
        state = STATE_GAME_OVER;
        return;
      }

      for (int i = 0; i < NUM_BUTTONS; i++) {
        if (digitalRead(BTN_PINS[i]) == LOW) {
          lastInputTime = millis();
          activateColor(i, 200);

          if (sequence[playerStep] == i) {
            // Correct step
            playerStep++;
            if (playerStep >= currentLevel) {
              // Level Completed!
              Serial.printf("✅ Level %u Complete!\n", currentLevel);
              currentLevel++;
              if (currentLevel > highScore) {
                highScore = currentLevel - 1;
                prefs.putUInt("high", highScore);
              }

              if (currentLevel >= MAX_SEQUENCE) {
                state = STATE_VICTORY;
                return;
              }

              // Append next random color
              sequence[currentLevel - 1] = random(0, NUM_BUTTONS);
              allLeds(true);
              playTone(880, 150);
              delay(150);
              allLeds(false);
              state = STATE_PLAY_SEQUENCE;
              return;
            }
          } else {
            // Mistake made
            Serial.printf("❌ Wrong button pressed! Expected %u, got %d.\n", sequence[playerStep], i);
            state = STATE_GAME_OVER;
            return;
          }

          // Debounce delay
          while (digitalRead(BTN_PINS[i]) == LOW) delay(10);
          delay(50);
        }
      }
      break;
    }

    case STATE_GAME_OVER: {
      Serial.printf("Game Over! Score: %u | High Score: %u\n", currentLevel - 1, highScore);
      allLeds(true);
      playTone(130, 600);
      delay(600);
      allLeds(false);
      delay(300);

      // Flash LEDs twice
      for (int k = 0; k < 2; k++) {
        allLeds(true);
        delay(150);
        allLeds(false);
        delay(150);
      }

      state = STATE_WELCOME;
      Serial.println("Press any button to play again!");
      break;
    }

    case STATE_VICTORY: {
      Serial.println("🏆 CONGRATULATIONS! You completed all 100 levels!");
      for (int v = 0; v < 5; v++) {
        for (int i = 0; i < NUM_BUTTONS; i++) {
          activateColor(i, 80);
        }
      }
      state = STATE_WELCOME;
      break;
    }
  }
}
