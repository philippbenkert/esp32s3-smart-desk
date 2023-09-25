#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h>
#include <WiFi.h>

class MQTTManager {
public:
    MQTTManager(const char* mqtt_server, int mqtt_port, const char* client_id);

    void setup();
    void loop();

    bool isConnected() const;

    void setCallback(void (*callback)(char*, byte*, unsigned int));

private:
    WiFiClient _espClient;
    PubSubClient _client;
};

#endif // MQTT_MANAGER_H

