#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h>
#include "wifi_manager.h"
#include "relay_controller.h"
#include "motor_controller.h"

class MQTTManager {
public:
    MQTTManager(PubSubClient& client, WiFiManager& wifiManager, RelayController& relayController, MotorController& motorController, const char* mqtt_server, int mqtt_port);
    bool publish(const char* topic, const char* payload);
    void setCallback(void (*callback)(char*, byte*, unsigned int));
    void setup();
    void loop();
    void callback(char* topic, byte* payload, unsigned int length);

private:
    const char* _mqtt_server;
    int _mqtt_port;
    WiFiManager& _wifiManager;
    RelayController& _relayController;
    MotorController& _motorController;
    PubSubClient& _client;
    void reconnect();
    void handleRelayCommands(const char* topic, const char* payload);
    void handleMotorCommands(const char* topic, const char* payload);
};

#endif // MQTT_MANAGER_H
