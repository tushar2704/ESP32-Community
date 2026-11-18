/**
 * @file pomodoro-timer.ino
 * @brief Project INT-16: Pomodoro Productivity Desk Timer
 * @author Tushar Aggarwal (@tushar2704)
 * @url https://github.com/tushar2704/ESP32-Community
 * @license AGPL-3.0
 */

#include <Arduino.h>

#define WORK_DURATION_SEC   (25 * 60)
#define SHORT_BREAK_SEC     (5 * 60)
#define LONG_BREAK_SEC      (15 * 60)
#define CYCLES_BEFORE_LONG  4

#define PIN_BUTTON 18
#define PIN_BUZZER 19

#if defined(CONFIG_IDF_TARGET_ESP32S3)
    #define PIN_LED 48
#elif defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C6)
    #define PIN_LED 8
#else
    #define PIN_LED 2
#endif

enum PomodoroState {
    IDLE,
    WORK,
    SHORT_BREAK,
    LONG_BREAK,
    PAUSED
};

PomodoroState currentState = IDLE;
PomodoroState savedState = WORK;
uint32_t timeRemaining = WORK_DURATION_SEC;
uint32_t sessionCount = 0;
unsigned long lastTick = 0;

void playChirp(int freq, int duration_ms) {
    pinMode(PIN_BUZZER, OUTPUT);
    for (long i = 0; i < (duration_ms * 1000L); i += (1000000L / freq) * 2) {
        digitalWrite(PIN_BUZZER, HIGH);
        delayMicroseconds(1000000L / (freq * 2));
        digitalWrite(PIN_BUZZER, LOW);
        delayMicroseconds(1000000L / (freq * 2));
    }
}

const char* getStateName(PomodoroState state) {
    switch (state) {
        case IDLE:        return "IDLE (Ready)";
        case WORK:        return "FOCUS (Work)";
        case SHORT_BREAK: return "SHORT BREAK ☕";
        case LONG_BREAK:  return "LONG BREAK 🌴";
        case PAUSED:      return "PAUSED ⏸️";
        default:          return "UNKNOWN";
    }
}

void transitionTo(PomodoroState newState) {
    currentState = newState;
    switch (newState) {
        case WORK:
            timeRemaining = WORK_DURATION_SEC;
            Serial.println("\n🎯 Focus Phase Started! 25 Minutes on the clock.");
            playChirp(880, 150);
            playChirp(1760, 250);
            break;
        case SHORT_BREAK:
            timeRemaining = SHORT_BREAK_SEC;
            Serial.println("\n☕ Short Break Time! Step away and stretch.");
            playChirp(600, 200);
            playChirp(800, 300);
            break;
        case LONG_BREAK:
            timeRemaining = LONG_BREAK_SEC;
            Serial.println("\n🌴 Great work! You completed 4 sessions. Take a 15-minute long break.");
            playChirp(523, 200);
            playChirp(659, 200);
            playChirp(784, 400);
            break;
        case PAUSED:
            Serial.println("\n⏸️ Timer Paused.");
            playChirp(440, 150);
            break;
        case IDLE:
            timeRemaining = WORK_DURATION_SEC;
            Serial.println("\n⏹️ Timer Reset to IDLE.");
            break;
    }
}

void setup() {
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    pinMode(PIN_BUZZER, OUTPUT);

    Serial.begin(115200);
    delay(500);

    Serial.println("\n========================================================");
    Serial.println("  ⚡ ESP32 Community — Project INT-16: Pomodoro Timer");
    Serial.println("========================================================");
    Serial.println("Commands via Serial:");
    Serial.println("  's' : Start / Resume");
    Serial.println("  'p' : Pause");
    Serial.println("  'r' : Reset");
    Serial.println("  'k' : Skip to next phase\n");

    transitionTo(WORK);
}

void loop() {
    unsigned long currentMillis = millis();

    // Check button tap (with basic software debounce)
    if (digitalRead(PIN_BUTTON) == LOW) {
        delay(50);
        if (digitalRead(PIN_BUTTON) == LOW) {
            if (currentState == PAUSED) {
                transitionTo(savedState);
            } else if (currentState == WORK || currentState == SHORT_BREAK || currentState == LONG_BREAK) {
                savedState = currentState;
                transitionTo(PAUSED);
            }
            while (digitalRead(PIN_BUTTON) == LOW) delay(10);
        }
    }

    // Check serial console input
    if (Serial.available()) {
        char c = Serial.read();
        if (c == 's') {
            if (currentState == PAUSED) transitionTo(savedState);
            else if (currentState == IDLE) transitionTo(WORK);
        } else if (c == 'p' && currentState != PAUSED) {
            savedState = currentState;
            transitionTo(PAUSED);
        } else if (c == 'r') {
            transitionTo(IDLE);
        } else if (c == 'k') {
            timeRemaining = 0; // Trigger completion
        }
    }

    // 1-second countdown tick
    if (currentMillis - lastTick >= 1000) {
        lastTick = currentMillis;

        if (currentState == WORK || currentState == SHORT_BREAK || currentState == LONG_BREAK) {
            if (timeRemaining > 0) {
                timeRemaining--;
                uint32_t mins = timeRemaining / 60;
                uint32_t secs = timeRemaining % 60;
                digitalWrite(PIN_LED, (secs % 2 == 0) ? HIGH : LOW);

                Serial.printf("[%s] %02u:%02u | Sessions Completed: %u\n",
                              getStateName(currentState), mins, secs, sessionCount);
            } else {
                // Phase completed!
                if (currentState == WORK) {
                    sessionCount++;
                    if (sessionCount % CYCLES_BEFORE_LONG == 0) {
                        transitionTo(LONG_BREAK);
                    } else {
                        transitionTo(SHORT_BREAK);
                    }
                } else {
                    transitionTo(WORK);
                }
            }
        }
    }
}
