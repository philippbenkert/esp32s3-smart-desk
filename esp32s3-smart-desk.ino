#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "relay_controller.h"
#include "motor_controller.h"
#include <PubSubClient.h>  // Fügen Sie diese Zeile am Anfang der Datei hinzu

WiFiClient espClient;
PubSubClient client(espClient);
const char* ssid = "ssid";
const char* password = "password";
WiFiManager wifiManager(ssid, password);
const char* mqtt_server = "192.168.0.1";
const int mqtt_port = 1024;
const char* client_id = "ESP32S3_Schreibtisch";
const int SER_Pin = 7;
const int RCLK_Pin = 5;
const int SRCLK_Pin = 6;
const int numOfShiftRegisters = 1;
RelayController relayController(SER_Pin, RCLK_Pin, SRCLK_Pin, numOfShiftRegisters);
MotorController motorController(relayController);
MQTTManager mqttManager(client, wifiManager, relayController, motorController, mqtt_server, mqtt_port);

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    payload[length] = '\0';  // Null-terminate the payload
    String payloadStr = String((char*)payload);
    bool isOn = (payloadStr == "true");

    if (strcmp(topic, "/relay/monitor") == 0) {
        relayController.setRelayState(RelayController::Relay::MONITOR, isOn);
    } else if (strcmp(topic, "/relay/laptop") == 0) {
        relayController.setRelayState(RelayController::Relay::LAPTOP, isOn);
    } else if (strcmp(topic, "/relay/fan") == 0) {
        relayController.setRelayState(RelayController::Relay::FAN, isOn);
    } else if (strcmp(topic, "/relay/reserve2") == 0) {
        relayController.setRelayState(RelayController::Relay::RESERVE2, isOn);
    } else if (strcmp(topic, "/motor/table") == 0) {
        if (isOn) {
            motorController.moveTableUp();
        } else {
            motorController.moveTableDown();
        }
    } else if (strcmp(topic, "/motor/runtimeup") == 0) {
        int runtime = payloadStr.toInt();
        motorController.setMotorRunTimeUp(runtime);
    } else if (strcmp(topic, "/motor/runtimedown") == 0) {
        int runtime = payloadStr.toInt();
        motorController.setMotorRunTimeDown(runtime);
    } else {
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
