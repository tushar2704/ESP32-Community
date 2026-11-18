/**
 * @file weather-station.ino
 * @brief Project INT-01: Wi-Fi Weather Station & Forecast Dashboard
 * @author Tushar Aggarwal (@tushar2704)
 * @url https://github.com/tushar2704/ESP32-Community
 * @license AGPL-3.0
 */

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";

// GPS Coordinates (Default: Paris, France - update for your location)
const float LATITUDE  = 48.8566;
const float LONGITUDE = 2.3522;

const char* decodeWMOWeather(int code) {
    switch (code) {
        case 0:  return "Clear Sky ☀️";
        case 1:  return "Mainly Clear 🌤️";
        case 2:  return "Partly Cloudy ⛅";
        case 3:  return "Overcast ☁️";
        case 45: return "Fog 🌫️";
        case 48: return "Depositing Rime Fog 🌫️";
        case 51: return "Light Drizzle 🌧️";
        case 53: return "Moderate Drizzle 🌧️";
        case 55: return "Dense Drizzle 🌧️";
        case 61: return "Slight Rain 🌦️";
        case 63: return "Moderate Rain 🌧️";
        case 65: return "Heavy Rain 🌧️";
        case 71: return "Slight Snow 🌨️";
        case 73: return "Moderate Snow 🌨️";
        case 75: return "Heavy Snow ❄️";
        case 80: return "Slight Rain Showers 🌦️";
        case 81: return "Moderate Showers 🌧️";
        case 82: return "Violent Showers ⛈️";
        case 95: return "Thunderstorm ⚡";
        case 96: return "Thunderstorm with Hail ⛈️";
        case 99: return "Severe Thunderstorm with Hail ⛈️";
        default: return "Unknown Weather 🌡️";
    }
}

void fetchWeatherData() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("❌ Wi-Fi disconnected. Cannot fetch weather.");
        return;
    }

    WiFiClientSecure client;
    client.setInsecure(); // Open-Meteo public endpoints

    HTTPClient http;
    char url[256];
    snprintf(url, sizeof(url), 
             "https://api.open-meteo.com/v1/forecast?latitude=%.4f&longitude=%.4f&current=temperature_2m,relative_humidity_2m,apparent_temperature,precipitation,weather_code,surface_pressure,wind_speed_10m",
             LATITUDE, LONGITUDE);

    Serial.println("\n📡 Contacting Open-Meteo REST API...");
    http.begin(client, url);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
            JsonObject current = doc["current"];
            float temp        = current["temperature_2m"];
            int humidity      = current["relative_humidity_2m"];
            float feels_like  = current["apparent_temperature"];
            float rain        = current["precipitation"];
            int weather_code  = current["weather_code"];
            float pressure    = current["surface_pressure"];
            float wind_speed  = current["wind_speed_10m"];

            Serial.println("==================================================");
            Serial.printf("  🌤️ Weather Report for Lat: %.2f, Lon: %.2f\n", LATITUDE, LONGITUDE);
            Serial.println("==================================================");
            Serial.printf("  Condition:          %s\n", decodeWMOWeather(weather_code));
            Serial.printf("  Temperature:        %.1f °C (Feels like: %.1f °C)\n", temp, feels_like);
            Serial.printf("  Humidity:           %d %%\n", humidity);
            Serial.printf("  Pressure:           %.1f hPa\n", pressure);
            Serial.printf("  Wind Speed:         %.1f km/h\n", wind_speed);
            Serial.printf("  Precipitation:      %.1f mm\n", rain);
            Serial.println("--------------------------------------------------");
        } else {
            Serial.printf("❌ JSON Deserialization error: %s\n", error.c_str());
        }
    } else {
        Serial.printf("❌ HTTP GET failed, error code: %d\n", httpCode);
    }
    http.end();
}

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("\n========================================================");
    Serial.println("  ⚡ ESP32 Community — Project INT-01: Weather Station");
    Serial.println("========================================================");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(300);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi successfully!");

    fetchWeatherData();
}

void loop() {
    // Refresh weather telemetry every 5 minutes
    delay(300000);
    fetchWeatherData();
}
