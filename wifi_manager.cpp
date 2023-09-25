#include "wifi_manager.h"

WiFiManager::WiFiManager(const char* ssid, const char* password) 
    : _ssid(ssid), _password(password) {}

void WiFiManager::setup() {
    WiFi.begin(_ssid, _password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
}

void WiFiManager::loop() {
    // In diesem Beispiel gibt es nichts im Loop zu tun, da wir nur einmal eine Verbindung herstellen.
    // Sie könnten jedoch eine Logik hinzufügen, um die Verbindung regelmäßig zu überprüfen und ggf. wiederherzustellen.
}

bool WiFiManager::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

