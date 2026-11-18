/**
 * @file deep-sleep-timer.ino
 * @brief Project 09: Ultra-Low-Power Deep Sleep & Timer Wakeup
 * @author Tushar Aggarwal (@tushar2704)
 * @url https://github.com/tushar2704/ESP32-Community
 * @license AGPL-3.0
 */

#include <Arduino.h>
#include <esp_sleep.h>

#define TIME_TO_SLEEP_SEC 10 // Sleep for 10 seconds between waking
#define uS_TO_S_FACTOR    1000000ULL

// Stored in RTC Fast/Slow memory — survives deep sleep!
RTC_DATA_ATTR int bootCount = 0;

void printWakeupReason() {
    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
    switch (wakeup_reason) {
        case ESP_SLEEP_WAKEUP_EXT0:     Serial.println("⏰ Wakeup caused by: External signal using RTC_IO (EXT0)"); break;
        case ESP_SLEEP_WAKEUP_EXT1:     Serial.println("⏰ Wakeup caused by: External signal using RTC_CNTL (EXT1)"); break;
        case ESP_SLEEP_WAKEUP_TIMER:    Serial.println("⏰ Wakeup caused by: Internal RTC Timer"); break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("⏰ Wakeup caused by: Touchpad contact"); break;
        case ESP_SLEEP_WAKEUP_ULP:      Serial.println("⏰ Wakeup caused by: ULP Coprocessor program"); break;
        default:                        Serial.printf("⏰ Wakeup caused by: Normal boot / Reset button (Code: %d)\n", wakeup_reason); break;
    }
}

void setup() {
    Serial.begin(115200);
    delay(500);

    bootCount++;
    Serial.println("\n========================================================");
    Serial.println("  ⚡ ESP32 Community — Project 09: Deep Sleep Engine");
    Serial.println("========================================================");
    Serial.printf("🔢 Boot Sequence Number: #%d\n", bootCount);
    printWakeupReason();

    // Perform rapid task (e.g. read analog sensor)
    float batteryVoltage = (float)analogRead(34) * (3.3f / 4095.0f) * 2.0f; // Voltage divider
    Serial.printf("🔋 Simulated Battery Check: %.2f V\n", batteryVoltage > 0 ? batteryVoltage : 3.95f);

    Serial.printf("💤 Entering deep sleep for %d seconds. Total sleep current: ~15µA\n", TIME_TO_SLEEP_SEC);
    Serial.flush(); // Ensure serial buffer empty before power gate

    // Configure RTC timer wake
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_SEC * uS_TO_S_FACTOR);
    esp_deep_sleep_start();
}

void loop() {
    // Loop never executes — ESP32 boots directly into setup() upon wake
}
