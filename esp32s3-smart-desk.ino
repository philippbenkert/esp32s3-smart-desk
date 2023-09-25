#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "relay_controller.h"
#include "ds18b20_sensor.h"
#include "motor_controller.h"

void setup() {
    // Initialisierungen hier
    setupWiFi();
    setupMQTT();
    setupRelays();
    setupDS18B20();
    setupMotors();
}

void loop() {
    // Hauptlogik hier
    loopWiFi();
    loopMQTT();
    loopRelays();
    loopDS18B20();
    loopMotors();
}
