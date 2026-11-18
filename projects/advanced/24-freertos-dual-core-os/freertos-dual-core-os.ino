/**
 * @file freertos-dual-core-os.ino
 * @brief Project ADV-24: Preemptive FreeRTOS Dual-Core Multitasking Engine
 * @author Tushar Aggarwal (@tushar2704)
 * @url https://github.com/tushar2704/ESP32-Community
 * @license AGPL-3.0
 */

#include <Arduino.h>

typedef struct {
    uint32_t sample_id;
    uint32_t timestamp_ms;
    float    core0_sensor_val;
    uint32_t core0_free_stack;
} SensorData;

// Thread-safe FreeRTOS Queue Handle
QueueHandle_t telemetryQueue;
SemaphoreHandle_t printMutex;

// ─── Task 1 (Pinned to CORE 0): Real-time High-Frequency Sensor Poller ───
void TaskCore0_Sensor(void *pvParameters) {
    uint32_t counter = 0;
    SensorData sample;

    while (true) {
        counter++;
        sample.sample_id = counter;
        sample.timestamp_ms = millis();
        sample.core0_sensor_val = (float)(analogRead(34)) * (3.3f / 4095.0f);
        sample.core0_free_stack = uxTaskGetStackHighWaterMark(NULL);

        // Send to queue with 10ms timeout
        xQueueSendToBack(telemetryQueue, &sample, pdMS_TO_TICKS(10));

        // Deterministic 100ms sample period (10 Hz rate)
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// ─── Task 2 (Pinned to CORE 1): UI & Telemetry Logger ──────────────────
void TaskCore1_UI(void *pvParameters) {
    SensorData received;

    while (true) {
        // Wait for incoming samples from Core 0
        if (xQueueReceive(telemetryQueue, &received, portMAX_DELAY) == pdPASS) {
            if (xSemaphoreTake(printMutex, pdMS_TO_TICKS(50)) == pdTRUE) {
                Serial.printf("[Core %d -> Core %d] Sample #%05u @ %6u ms | Value: %.2fV | Core 0 Stack: %u words\n",
                              0, xPortGetCoreID(),
                              received.sample_id,
                              received.timestamp_ms,
                              received.core0_sensor_val,
                              received.core0_free_stack);
                xSemaphoreGive(printMutex);
            }
        }
    }
}

void setup() {
    Serial.begin(115200);
    delay(500);

    printMutex = xSemaphoreCreateMutex();
    telemetryQueue = xQueueCreate(20, sizeof(SensorData)); // 20-element buffer

    Serial.println("\n========================================================");
    Serial.println("  ⚡ ESP32 Community — Project ADV-24: FreeRTOS Dual-Core");
    Serial.println("========================================================");
    Serial.printf("Main setup running on Core: %d\n", xPortGetCoreID());

    // Pin Sensor Task to Core 0 (Priority 2)
    xTaskCreatePinnedToCore(
        TaskCore0_Sensor,
        "Core0_Sensor",
        4096,
        NULL,
        2,
        NULL,
        0 // Pin to Core 0
    );

    // Pin UI Task to Core 1 (Priority 1)
    xTaskCreatePinnedToCore(
        TaskCore1_UI,
        "Core1_UI",
        4096,
        NULL,
        1,
        NULL,
        1 // Pin to Core 1
    );

    Serial.println(" Dual-core tasks scheduled successfully! Telemetry streaming below:\n");
}

void loop() {
    // Arduino loop runs on Core 1 at priority 1 (Idle here since tasks handle workload)
    vTaskDelay(pdMS_TO_TICKS(1000));
}
