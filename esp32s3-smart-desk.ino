#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "relay_controller.h"
#include "motor_controller.h"
#include <PubSubClient.h>

constexpr char* ssid = "ssid";
constexpr char* password = "password";
WiFiManager wifiManager(ssid, password);
constexpr char* mqtt_server = "192.168.0.1";
constexpr int mqtt_port = 1024;
constexpr char* client_id = "ESP32S3_Schreibtisch";
constexpr int SER_Pin = 7;
constexpr int RCLK_Pin = 5;
constexpr int SRCLK_Pin = 6;
constexpr int numOfShiftRegisters = 1;

WiFiClient espClient;
PubSubClient client(espClient);

RelayController relayController(SER_Pin, RCLK_Pin, SRCLK_Pin, numOfShiftRegisters);
MotorController motorController(relayController);
MQTTManager mqttManager(client, wifiManager, relayController, motorController, mqtt_server, mqtt_port);

void setup() {
    Serial.begin(115200);
    wifiManager.setup();
    mqttManager.setup();
    relayController.setMQTTManager(&mqttManager);
    relayController.setup();
}

void loop() {
    wifiManager.loop();
    mqttManager.loop();
    motorController.update();
    relayController.update();

}
