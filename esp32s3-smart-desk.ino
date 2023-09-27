#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "relay_controller.h"
#include "motor_controller.h"
#include <PubSubClient.h>

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
unsigned long previousMillis = 0;
unsigned long motorRunTime = 0;
bool isMovingUp = false;
bool isMovingDown = false;
RelayController relayController(SER_Pin, RCLK_Pin, SRCLK_Pin, numOfShiftRegisters);
MotorController motorController(relayController);
MQTTManager mqttManager(client, wifiManager, relayController, motorController, mqtt_server, mqtt_port);

void setup() {
    // Initialisierungen hier
    Serial.begin(115200);
    wifiManager.setup();
    mqttManager.setup();
    relayController.setup();
}

void loop() {
    // Hauptlogik hier
    wifiManager.loop();
    mqttManager.loop();
    motorController.update();

    // Timer-Logik für Tischbewegung
    unsigned long currentMillis = millis();
    if (isMovingUp && (currentMillis - previousMillis >= motorRunTime)) {
        motorController.stopTable();
        isMovingUp = false;
    } else if (isMovingDown && (currentMillis - previousMillis >= motorRunTime)) {
        motorController.stopTable();
        isMovingDown = false;
    }
}
