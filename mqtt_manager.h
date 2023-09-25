#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h>
#include "wifi_manager.h"
#include "relay_controller.h"
#include "motor_controller.h"

class MQTTManager {
public:
    MQTTManager(WiFiManager& wifiManager, RelayController& relayController, MotorController& motorController);

    void setup();
    void loop();

    void callback(char* topic, byte* payload, unsigned int length);

private:
    WiFiManager& _wifiManager;
    RelayController& _relayController;
    MotorController& _motorController;
    PubSubClient _client;

    void handleRelayCommands(const char* topic, const char* payload);
    void handleMotorCommands(const char* topic, const char* payload);
};

#endif // MQTT_MANAGER_H
