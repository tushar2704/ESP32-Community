/**
 * @file crypto-tracker.ino
 * @brief Project INT-03: Live Crypto Price Ticker & Sparkline Tracker
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

const char* COINGECKO_API = 
    "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin,ethereum,solana&vs_currencies=usd&include_24hr_change=true";

void fetchCryptoPrices() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("❌ Wi-Fi disconnected.");
        return;
    }

    WiFiClientSecure client;
    client.setInsecure(); // Public API SSL bypass

    HTTPClient http;
    http.begin(client, COINGECKO_API);
    http.setUserAgent("ESP32-Community-Tracker/1.0");

    Serial.println("\n📡 Contacting CoinGecko Price API...");
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
            float btc_price  = doc["bitcoin"]["usd"];
            float btc_change = doc["bitcoin"]["usd_24h_change"];

            float eth_price  = doc["ethereum"]["usd"];
            float eth_change = doc["ethereum"]["usd_24h_change"];

            float sol_price  = doc["solana"]["usd"];
            float sol_change = doc["solana"]["usd_24h_change"];

            Serial.println("==================================================");
            Serial.println("  🪙 Live Crypto Price Ticker (USD)");
            Serial.println("==================================================");
            Serial.printf("  ₿ Bitcoin  (BTC):  $%9.2f  |  24h: %s%+.2f %%\n", 
                          btc_price, btc_change >= 0 ? "▲ " : "▼ ", btc_change);
            Serial.printf("  Ξ Ethereum (ETH):  $%9.2f  |  24h: %s%+.2f %%\n", 
                          eth_price, eth_change >= 0 ? "▲ " : "▼ ", eth_change);
            Serial.printf("  ◎ Solana   (SOL):  $%9.2f  |  24h: %s%+.2f %%\n", 
                          sol_price, sol_change >= 0 ? "▲ " : "▼ ", sol_change);
            Serial.println("--------------------------------------------------");
        } else {
            Serial.printf("❌ JSON Deserialization error: %s\n", error.c_str());
        }
    } else {
        Serial.printf("❌ HTTP GET failed, status code: %d\n", httpCode);
    }
    http.end();
}

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("\n========================================================");
    Serial.println("  ⚡ ESP32 Community — Project INT-03: Crypto Ticker");
    Serial.println("========================================================");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(300);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi Connected!");

    fetchCryptoPrices();
}

void loop() {
    // Poll prices every 60 seconds (rate-limit safe)
    delay(60000);
    fetchCryptoPrices();
}
