#include "mqtt_manager.h"

MQTTManager::MQTTManager(PubSubClient& client, WiFiManager& wifiManager, RelayController& relayController, MotorController& motorController, const char* mqtt_server, int mqtt_port)
    : _client(client), 
      _wifiManager(wifiManager), 
      _relayController(relayController), 
      _motorController(motorController), 
      _mqtt_server(mqtt_server), 
      _mqtt_port(mqtt_port), 
      errorHandler(_client) {  // Hier initialisieren Sie errorHandler
    // ... restlicher Konstruktor-Code ...
}


void MQTTManager::setup() {
    _client.setServer(_mqtt_server, _mqtt_port);
    _client.setCallback([this](char* topic, byte* payload, unsigned int length) {
        this->callback(topic, payload, length);
    });
}

void MQTTManager::loop() {
    if (!_client.connected()) {
        reconnect();  // Reconnect-Logik hinzufügen
    }
    _client.loop();
}

void MQTTManager::reconnect() {
    while (!_client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (_client.connect("ESP32S3_Schreibtisch")) {
            Serial.println("connected");
            
            // Abonnieren Sie alle notwendigen Topics hier
            _client.subscribe("/relay/monitor");
            _client.subscribe("/relay/laptop");
            _client.subscribe("/relay/table_up");
            _client.subscribe("/relay/table_down");
            _client.subscribe("/relay/fan");
            _client.subscribe("/relay/reserve2");
            _client.subscribe("/motor/tableUp");
            _client.subscribe("/motor/tableDown");
            _client.subscribe("/motor/setRunTimeUp");
            _client.subscribe("/motor/setRunTimeDown");
            
        } else {
            Serial.print("failed, rc=");
            Serial.print(_client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}


void MQTTManager::callback(char* topic, byte* payload, unsigned int length) {
    payload[length] = '\0';
    String payloadStr = String((char*)payload);

    Serial.print("Received message on topic: ");
    Serial.println(topic);
    Serial.print("Payload: ");
    Serial.println(payloadStr);

    if (strncmp(topic, "/relay/", 7) == 0) {
        handleRelayCommands(topic, payloadStr.c_str());
    } else if (strncmp(topic, "/motor/", 7) == 0) {
        handleMotorCommands(topic, payloadStr.c_str());
    } else {
        _client.publish("/error", ("Unknown command on topic: " + String(topic) + " with payload: " + payloadStr).c_str());
    }
}

void MQTTManager::handleRelayCommands(const char* topic, const char* payload) {
    RelayController::Relay relay;
    bool state = (strcmp(payload, "true") == 0);
    
    if (strcmp(topic, "/relay/monitor") == 0) {
        relay = RelayController::Relay::MONITOR;
    } else if (strcmp(topic, "/relay/laptop") == 0) {
        relay = RelayController::Relay::LAPTOP;
    } else if (strcmp(topic, "/relay/fan") == 0) {
        relay = RelayController::Relay::FAN;
    } else if (strcmp(topic, "/relay/reserve2") == 0) {
        relay = RelayController::Relay::RESERVE2;
    } else {
        return;  // Ungültiges Topic
    }

    _relayController.setRelayState(relay, state);
    _client.publish(topic, state ? "true" : "false");
}

void MQTTManager::handleMotorCommands(const char* topic, const char* payload) {
    if (strcmp(topic, "/motor/tableUp") == 0) {
        if (strcmp(payload, "true") == 0) {
            _client.publish("/motor/tableDown", "false");
            _motorController.moveTableUp();
            _client.publish(topic, "moving up");
        } else {
            _motorController.stopTable();
        }
    } else if (strcmp(topic, "/motor/tableDown") == 0) {
        if (strcmp(payload, "true") == 0) {
            _client.publish("/motor/tableUp", "false");
            _motorController.moveTableDown();
            _client.publish(topic, "moving down");
        } else {
            _motorController.stopTable();
        }
    } else if (strcmp(topic, "/motor/setRunTimeUp") == 0) {
        _motorController.setMotorRunTimeUp(atoi(payload));
        _client.publish(topic, payload);
    } else if (strcmp(topic, "/motor/setRunTimeDown") == 0) {
        _motorController.setMotorRunTimeDown(atoi(payload));
        _client.publish(topic, payload);
    }

}


bool MQTTManager::publish(const char* topic, const char* payload) {
    return _client.publish(topic, payload);
}

void MQTTManager::setCallback(void (*callback)(char*, byte*, unsigned int)) {
    _client.setCallback(callback);
}


