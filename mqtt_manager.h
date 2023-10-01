// mqtt_manager.h

#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include "wifi_manager.h"
#include <PubSubClient.h>
#include "error_handler.h"

class RelayController;
class MotorController;

class MQTTManager {
public:
    MQTTManager(PubSubClient& client, WiFiManager& wifiManager, RelayController& relayController, MotorController& motorController, const char* mqtt_server, int mqtt_port);
    bool publish(const char* topic, const char* payload);
    void setCallback(void (*callback)(char*, byte*, unsigned int));
    void setup();
    void loop();
    void callback(char* topic, byte* payload, unsigned int length);
    void reportError(const char* errorMsg) {
        _client.publish("/error", errorMsg);
    }

private:
    const char* _mqtt_server;
    int _mqtt_port;
    WiFiManager& _wifiManager;
    RelayController& _relayController;
    MotorController& _motorController;
    PubSubClient& _client;
    ErrorHandler errorHandler;
    void reconnect();
    void handleRelayCommands(const char* topic, const char* payload);
    void handleMotorCommands(const char* topic, const char* payload);
};

#endif // MQTT_MANAGER_H
