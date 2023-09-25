#include "mqtt_manager.h"

MQTTManager::MQTTManager(const char* mqtt_server, int mqtt_port, const char* client_id) 
    : _client(_espClient) {
    _client.setServer(mqtt_server, mqtt_port);
    _client.setClientId(client_id);
}

void MQTTManager::setup() {
    while (!_client.connected()) {
        Serial.println("Connecting to MQTT...");

        if (_client.connect(_client.getClientId())) {
            Serial.println("Connected to MQTT");
        } else {
            Serial.print("Failed with state ");
            Serial.print(_client.state());
            delay(2000);
        }
    }
}

void MQTTManager::loop() {
    _client.loop();
}

bool MQTTManager::isConnected() const {
    return _client.connected();
}

void MQTTManager::setCallback(void (*callback)(char*, byte*, unsigned int)) {
    _client.setCallback(callback);
}

