#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "relay_controller.h"
#include "motor_controller.h"

const char* ssid = "Ihr_SSID";
const char* password = "Ihr_Passwort";
WiFiManager wifiManager(ssid, password);
const char* mqtt_server = "192.168.0.1";
const int mqtt_port = 1883;
const char* client_id = "ESP32S3_Client";
MQTTManager mqttManager(wifiManager, relayController, motorController);
const int SER_Pin = 7;
const int RCLK_Pin = 5;
const int SRCLK_Pin = 6;
const int numOfShiftRegisters = 1;
RelayController relayController(SER_Pin, RCLK_Pin, SRCLK_Pin, numOfShiftRegisters);
MotorController motorController(relayController);


void mqttCallback(char* topic, byte* payload, unsigned int length) {
    payload[length] = '\0';  // Null-terminate the payload
    String payloadStr = String((char*)payload);

    if (strcmp(topic, "/relay/monitor") == 0) {
        if (payloadStr == "on") {
            relayController.setRelayState(RelayController::Monitor, true);
        } else if (payloadStr == "off") {
            relayController.setRelayState(RelayController::Monitor, false);
        }
    } else if (strcmp(topic, "/relay/laptop") == 0) {
        if (payloadStr == "on") {
            relayController.setRelayState(RelayController::Laptop, true);
        } else if (payloadStr == "off") {
            relayController.setRelayState(RelayController::Laptop, false);
        }
    } else if (strcmp(topic, "/relay/reserve1") == 0) {
        if (payloadStr == "on") {
            relayController.setRelayState(RelayController::Reserve1, true);
        } else if (payloadStr == "off") {
            relayController.setRelayState(RelayController::Reserve1, false);
        }
    } else if (strcmp(topic, "/relay/reserve2") == 0) {
        if (payloadStr == "on") {
            relayController.setRelayState(RelayController::Reserve2, true);
        } else if (payloadStr == "off") {
            relayController.setRelayState(RelayController::Reserve2, false);
        }
    } else if (strcmp(topic, "/motor/table") == 0) {
        if (payloadStr == "up") {
            motorController.moveTable(MotorController::Direction::Up);
        } else if (payloadStr == "down") {
            motorController.moveTable(MotorController::Direction::Down);
        } else if (payloadStr == "stop") {
            motorController.stopTable();
        }
    } else if (strcmp(topic, "/motor/runtime") == 0) {
        int runtime = payloadStr.toInt();
        motorController.setRuntime(runtime);
    }

    // Feedback an den MQTT-Server senden
    else {
    mqttManager.publish("/error", ("Unknown command on topic: " + String(topic) + " with payload: " + payloadStr).c_str());
    }
}

void setup() {
    // Initialisierungen hier
    Serial.begin(115200);
    wifiManager.setup();
    mqttManager.setup();
    mqttManager.setCallback(mqttCallback);
    relayController.setup();
}

void loop() {
    // Hauptlogik hier
    wifiManager.loop();
    mqttManager.loop();
}
