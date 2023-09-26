#include "mqtt_manager.h"

MQTTManager::MQTTManager(PubSubClient& client, WiFiManager& wifiManager, RelayController& relayController, MotorController& motorController, const char* mqtt_server, int mqtt_port)
    : _client(client), _wifiManager(wifiManager), _relayController(relayController), _motorController(motorController), _mqtt_server(mqtt_server), _mqtt_port(mqtt_port) {
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
        handleRelayCommands(topic, payloadStr.c_str());  // Verwenden Sie .c_str(), um es als const char* zu übergeben
    } else if (strncmp(topic, "/motor/", 7) == 0) {
        handleMotorCommands(topic, payloadStr.c_str());  // Verwenden Sie .c_str(), um es als const char* zu übergeben
    }
    // Weitere Topics können hier hinzugefügt werden
}


void MQTTManager::handleRelayCommands(const char* topic, const char* payload) {
    RelayController::Relay relay;
    if (strcmp(topic, "/relay/monitor") == 0) {
        relay = RelayController::Relay::MONITOR;
    } else if (strcmp(topic, "/relay/laptop") == 0) {
        relay = RelayController::Relay::LAPTOP;
    } else if (strcmp(topic, "/relay/table_up") == 0) {
        relay = RelayController::Relay::TABLE_UP;
    } else if (strcmp(topic, "/relay/table_down") == 0) {
        relay = RelayController::Relay::TABLE_DOWN;
    } else if (strcmp(topic, "/relay/fan") == 0) {
        relay = RelayController::Relay::FAN;
    } else if (strcmp(topic, "/relay/reserve2") == 0) {
        relay = RelayController::Relay::RESERVE2;
    } else {
        return;  // Ungültiges Topic
    }

    bool state = (strcmp(payload, "true") == 0);
    _relayController.setRelayState(relay, state);
    _client.publish(topic, state ? "true" : "false");
}


bool MQTTManager::publish(const char* topic, const char* payload) {
    return _client.publish(topic, payload);
}

void MQTTManager::setCallback(void (*callback)(char*, byte*, unsigned int)) {
    _client.setCallback(callback);
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
