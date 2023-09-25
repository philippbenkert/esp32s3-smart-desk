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
MQTTManager mqttManager(mqtt_server, mqtt_port, client_id);
const int SER_Pin = 7;
const int RCLK_Pin = 5;
const int SRCLK_Pin = 6;
const int numOfShiftRegisters = 1;
RelayController relayController(SER_Pin, RCLK_Pin, SRCLK_Pin, numOfShiftRegisters);

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    // Hier können Sie den Code aus Ihrem ursprünglichen Callback einfügen
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
