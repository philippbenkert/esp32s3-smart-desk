#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "relay_controller.h"
#include "ds18b20_sensor.h"
#include "motor_controller.h"

const char* ssid = "Ihr_SSID";
const char* password = "Ihr_Passwort";
WiFiManager wifiManager(ssid, password);
const char* mqtt_server = "192.168.0.1";
const int mqtt_port = 1883;
const char* client_id = "ESP32S3_Client";
MQTTManager mqttManager(mqtt_server, mqtt_port, client_id);

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    // Hier können Sie den Code aus Ihrem ursprünglichen Callback einfügen
}
void setup() {
    // Initialisierungen hier
    Serial.begin(115200);
    wifiManager.setup();
    mqttManager.setup();
    mqttManager.setCallback(mqttCallback);
    setupRelays();
    setupDS18B20();
    setupMotors();
}

void loop() {
    // Hauptlogik hier
    wifiManager.loop();
    mqttManager.loop();
    loopRelays();
    loopDS18B20();
    loopMotors();
}
