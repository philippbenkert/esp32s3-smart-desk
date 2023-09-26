#include "wifi_manager.h"

WiFiManager::WiFiManager(const char* ssid, const char* password) 
    : _ssid(ssid), _password(password) {}

void WiFiManager::setup() {
    connectToWiFi();
}

void WiFiManager::loop() {
    if (!isConnected() && (millis() - _lastAttemptTime > _connectionTimeout)) {
        _lastAttemptTime = millis();
        connectToWiFi();
    }
}

bool WiFiManager::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::connectToWiFi() {
    Serial.println("Connecting to WiFi...");
    WiFi.begin(_ssid, _password);

    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < _connectionTimeout) {
        delay(1000);
        Serial.print(".");
    }

    if (isConnected()) {
        Serial.println("\nConnected to WiFi");
    } else {
        Serial.println("\nFailed to connect to WiFi");
    }
}
