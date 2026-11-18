/**
 * @file blink-rgb.ino
 * @brief Project 03: RGB LED Controller & LEDC PWM Engine
 * @author Tushar Aggarwal (@tushar2704)
 * @url https://github.com/tushar2704/ESP32-Community
 * @license AGPL-3.0
 */

#include <Arduino.h>

// Default RGB GPIO pins
#if defined(CONFIG_IDF_TARGET_ESP32S3)
    #define PIN_RED   4
    #define PIN_GREEN 5
    #define PIN_BLUE  6
#else
    #define PIN_RED   25
    #define PIN_GREEN 26
    #define PIN_BLUE  27
#endif

// PWM Parameters
#define PWM_FREQ 5000
#define PWM_RES  8

// PWM Channels (for Core v2)
#define CH_RED   0
#define CH_GREEN 1
#define CH_BLUE  2

void setRGB(uint8_t r, uint8_t g, uint8_t b) {
#if defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3)
    ledcWrite(PIN_RED, r);
    ledcWrite(PIN_GREEN, g);
    ledcWrite(PIN_BLUE, b);
#else
    ledcWrite(CH_RED, r);
    ledcWrite(CH_GREEN, g);
    ledcWrite(CH_BLUE, b);
#endif
}

// Convert HSV color space (0-359 hue) to RGB
void setHSV(uint16_t hue, uint8_t sat, uint8_t val) {
    uint8_t r, g, b;
    if (sat == 0) {
        r = g = b = val;
    } else {
        uint8_t base = ((255 - sat) * val) >> 8;
        uint8_t region = hue / 60;
        uint8_t remainder = (hue - (region * 60)) * 6;
        uint8_t p = ((val - base) * remainder) >> 8;
        
        switch (region) {
            case 0:  r = val; g = base + p; b = base; break;
            case 1:  r = val - p; g = val; b = base; break;
            case 2:  r = base; g = val; b = base + p; break;
            case 3:  r = base; g = val - p; b = val; break;
            case 4:  r = base + p; g = base; b = val; break;
            default: r = val; g = base; b = val - p; break;
        }
    }
    setRGB(r, g, b);
}

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("\n========================================================");
    Serial.println("  ⚡ ESP32 Community — Project 03: RGB LED PWM Controller");
    Serial.println("========================================================");
    Serial.printf("Pins Configured: Red: %d | Green: %d | Blue: %d\n", PIN_RED, PIN_GREEN, PIN_BLUE);
    Serial.println("Commands available via Serial: Type 'R<0-255> G<0-255> B<0-255>' or let the rainbow run!");

    // Compatible LEDC setup for both Arduino Core v2 and v3
#if defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3)
    ledcAttach(PIN_RED, PWM_FREQ, PWM_RES);
    ledcAttach(PIN_GREEN, PWM_FREQ, PWM_RES);
    ledcAttach(PIN_BLUE, PWM_FREQ, PWM_RES);
#else
    ledcSetup(CH_RED, PWM_FREQ, PWM_RES);
    ledcAttachPin(PIN_RED, CH_RED);
    ledcSetup(CH_GREEN, PWM_FREQ, PWM_RES);
    ledcAttachPin(PIN_GREEN, CH_GREEN);
    ledcSetup(CH_BLUE, PWM_FREQ, PWM_RES);
    ledcAttachPin(PIN_BLUE, CH_BLUE);
#endif
}

void loop() {
    // Automated rainbow color cycle
    static uint16_t hue = 0;
    setHSV(hue, 255, 200);
    hue = (hue + 1) % 360;

    delay(20);
}
