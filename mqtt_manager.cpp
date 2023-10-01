#include <unordered_map>
#include <string>
#include "mqtt_manager.h"
#include "relay_controller.h"
#include "motor_controller.h"

// Vermeiden Sie Magische Zahlen
constexpr size_t PAYLOAD_BUFFER_SIZE = 256;
constexpr int PWM_MIN = 0;   // 0% PWM
constexpr int PWM_MAX = 255; // 100% PWM

MQTTManager::MQTTManager(PubSubClient& client, WiFiManager& wifiManager, RelayController& relayController, MotorController& motorController, const char* mqtt_server, int mqtt_port)
    : _client(client), 
      _wifiManager(wifiManager), 
      _relayController(relayController), 
      _motorController(motorController), 
      _mqtt_server(mqtt_server), 
      _mqtt_port(mqtt_port), 
      errorHandler(_client) {
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
        reconnect();
    }
    _client.loop();
}

void MQTTManager::reconnect() {
    while (!_client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (_client.connect("ESP32S3_Schreibtisch")) {
            Serial.println("connected");
            _client.subscribe("/relay/monitor");
            _client.subscribe("/relay/laptop");
            _client.subscribe("/relay/fan");
            _client.subscribe("/relay/kofferraum");
            _client.subscribe("/relay/table_Up");
            _client.subscribe("/relay/table_Down");
            _client.subscribe("/relay/kofferraum");
            _client.subscribe("/relay/kofferraumDuration");
            _client.subscribe("/relay/ssrfan");
            _client.subscribe("/motor/tableDown");
            _client.subscribe("/motor/tableUp");
            _client.subscribe("/motor/runtimeup");
            _client.subscribe("/motor/runtimedown");


        } else {
            Serial.print("failed, rc=");
            Serial.print(_client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void MQTTManager::callback(char* topic, byte* payload, unsigned int length) {
    char payloadStr[PAYLOAD_BUFFER_SIZE];

    if (topic == nullptr || payload == nullptr || length >= PAYLOAD_BUFFER_SIZE - 1) {
        return;
    }

    payload[length] = '\0';
    strncpy(payloadStr, (char*)payload, PAYLOAD_BUFFER_SIZE - 1);

    if (strncmp(topic, "/relay/", 7) == 0) {
        Serial.print("Received relay command on topic: ");
        Serial.println(topic);
        handleRelayCommands(topic, payloadStr);
    } else if (strncmp(topic, "/motor/", 7) == 0) {
        Serial.print("Received motor command on topic: ");
        Serial.println(topic);
        handleMotorCommands(topic, payloadStr);
    } else {
        char errorMsg[512];
        snprintf(errorMsg, sizeof(errorMsg), "Unknown command on topic: %s with payload: %s", topic, payloadStr);
        _client.publish("/error", errorMsg);
        Serial.println(errorMsg);
    }

    
}

void MQTTManager::handleRelayCommands(const char* topic, const char* payload) {
    static const std::unordered_map<std::string, RelayController::Relay> relayMap = {
        {"/relay/monitor", RelayController::Relay::MONITOR},
        {"/relay/laptop", RelayController::Relay::LAPTOP},
        {"/relay/fan", RelayController::Relay::FAN},
        {"/relay/kofferraum", RelayController::Relay::KOFFERRAUM},
        {"/relay/table_Up", RelayController::Relay::TABLE_UP},
        {"/relay/table_Down", RelayController::Relay::TABLE_DOWN},
        {"/relay/ssrfan", RelayController::Relay::SSR_FAN},
    };
        // Überprüfen Sie, ob das Topic für die Kofferraum-Dauer ist
    if (strcmp(topic, "/relay/kofferraumDuration") == 0) {
        unsigned long newDuration = strtoul(payload, nullptr, 10); // Konvertieren Sie das Payload in eine Zahl (in Millisekunden)
        _relayController.setKofferraumDuration(newDuration); // Aktualisieren Sie die Dauer im RelayController
        Serial.print("Kofferraum duration updated to: ");
        Serial.println(newDuration);
    }
    bool state = (strncmp(payload, "true", 4) == 0) || (strncmp(payload, "1", 1) == 0);
    auto it = relayMap.find(topic);
    if (it != relayMap.end()) {
        _relayController.setRelayState(static_cast<int>(it->second), state);
        Serial.print("Relay command processed for topic: ");
        Serial.println(topic);
        if (strcmp(topic, "/relay/ssrfan") == 0) {
        int percentage = atoi(payload); // Konvertiert das Payload in einen Prozentsatz
        if (percentage > 0 && percentage < 30) {
            percentage = 30; // Setzen Sie den Prozentsatz auf 30, wenn er zwischen 1 und 29 liegt
        }
        if (percentage >= 0 && percentage <= 100) {
            int pwmValue = map(percentage, 0, 100, PWM_MIN, PWM_MAX); // Mappt den Prozentsatz auf den PWM-Bereich
            _relayController.setSsrFanPwm(pwmValue);
            Serial.print("Set SSR Fan PWM to: ");
            Serial.println(pwmValue);
        } else {
            Serial.println("Invalid percentage for SSR Fan.");
        }
    }
        // Wenn KOFFERRAUM aktiviert wird, starten Sie einen Timer, um es später automatisch zu deaktivieren
        if (it->second == RelayController::Relay::KOFFERRAUM && state) {
            unsigned long duration = _relayController.getKofferraumDuration();
            delay(duration); // Warten Sie die angegebene Zeitdauer
            _relayController.setRelayState(static_cast<int>(it->second), false); // Deaktivieren Sie das Relay
            publish("/relay/kofferraum", "false"); // Setzen Sie das MQTT-Topic auf "false"
        }
    
    } else {
        Serial.print("No relay mapping found for topic: ");
        Serial.println(topic);
    }
}


void MQTTManager::handleMotorCommands(const char* topic, const char* payload) {
    // Zuerst überprüfen wir die Laufzeitbefehle
    if (strncmp(topic, "/motor/runtimeup", 15) == 0) { // Beachten Sie, dass ich die Länge auf 15 geändert habe
        unsigned long duration = strtoul(payload, NULL, 10);
        _motorController.setMotorRunTimeUp(duration);
        Serial.print("Set motor run time up to: ");
        Serial.println(duration);
        return; // Beenden Sie die Funktion nachdem Sie die Laufzeit gesetzt haben
    } else if (strncmp(topic, "/motor/runtimedown", 17) == 0) { // Beachten Sie, dass ich die Länge auf 17 geändert habe
        unsigned long duration = strtoul(payload, NULL, 10);
        _motorController.setMotorRunTimeDown(duration);
        Serial.print("Set motor run time down to: ");
        Serial.println(duration);
        return; // Beenden Sie die Funktion nachdem Sie die Laufzeit gesetzt haben
    }

    // Dann überprüfen wir die anderen Motorbefehle
    if (strncmp(topic, "/motor/tableUp", 14) == 0) {
        if (strncmp(payload, "true", 4) == 0 || strncmp(payload, "1", 1) == 0) {
            _client.publish("/motor/tableDown", "false"); // Setzt tableDown auf false, wenn tableUp auf true gesetzt wird
            _motorController.moveTableUp();
            Serial.println("Received command to move table up");
        } else {
            _motorController.stopTable();
            Serial.println("Received command to stop table");
        }
    } else if (strncmp(topic, "/motor/tableDown", 16) == 0) {
        if (strncmp(payload, "true", 4) == 0 || strncmp(payload, "1", 1) == 0) {
            _client.publish("/motor/tableUp", "false"); // Setzt tableUp auf false, wenn tableDown auf true gesetzt wird
            _motorController.moveTableDown();
            Serial.println("Received command to move table down");
        } else {
            _motorController.stopTable();
            Serial.println("Received command to stop table");
        }
    } else {
        char errorMsg[512];
        snprintf(errorMsg, sizeof(errorMsg), "Unknown motor command on topic: %s with payload: %s", topic, payload);
        _client.publish("/error", errorMsg);
        Serial.println(errorMsg);
    }
}


bool MQTTManager::publish(const char* topic, const char* payload) {
    return _client.publish(topic, payload);
}

void MQTTManager::setCallback(void (*callback)(char*, byte*, unsigned int)) {
    _client.setCallback(callback);
}
