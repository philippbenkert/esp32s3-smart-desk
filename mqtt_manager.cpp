#include "mqtt_manager.h"

MQTTManager::MQTTManager(WiFiManager& wifiManager, RelayController& relayController, MotorController& motorController)
    : _wifiManager(wifiManager), _relayController(relayController), _motorController(motorController), _client(_wifiManager.getClient()) {}

void MQTTManager::setup() {
    _client.setServer(MQTT_SERVER, MQTT_PORT);
    _client.setCallback([this](char* topic, byte* payload, unsigned int length) {
        this->callback(topic, payload, length);
    });
}

void MQTTManager::loop() {
    if (!_client.connected()) {
        // Hier können Sie einen Reconnect-Versuch hinzufügen
    }
    _client.loop();
}

void MQTTManager::callback(char* topic, byte* payload, unsigned int length) {
    char payloadStr[length + 1];
    strncpy(payloadStr, (char*)payload, length);
    payloadStr[length] = '\0';

    if (strncmp(topic, "/relay/", 7) == 0) {
        handleRelayCommands(topic, payloadStr);
    } else if (strncmp(topic, "/motor/", 7) == 0) {
        handleMotorCommands(topic, payloadStr);
    }
    // Weitere Topics können hier hinzugefügt werden
}

void MQTTManager::handleRelayCommands(const char* topic, const char* payload) {
    int relayIndex = topic[7] - '1';
    if (relayIndex >= 0 && relayIndex < 6) {
        bool state = (strcmp(payload, "true") == 0);
        _relayController.setRelayState(static_cast<RelayController::Relay>(relayIndex), state);
        _client.publish(topic, state ? "true" : "false");
    }
}

void MQTTManager::handleMotorCommands(const char* topic, const char* payload) {
    if (strcmp(topic, "/motor/tableUp") == 0) {
        _motorController.moveTableUp();
        _client.publish(topic, "moving up");
    } else if (strcmp(topic, "/motor/tableDown") == 0) {
        _motorController.moveTableDown();
        _client.publish(topic, "moving down");
    } else if (strcmp(topic, "/motor/setRunTimeUp") == 0) {
        _motorController.setMotorRunTimeUp(atoi(payload));
        _client.publish(topic, payload);
    } else if (strcmp(topic, "/motor/setRunTimeDown") == 0) {
        _motorController.setMotorRunTimeDown(atoi(payload));
        _client.publish(topic, payload);
    }
}
