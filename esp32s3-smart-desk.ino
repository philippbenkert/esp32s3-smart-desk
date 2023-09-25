#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "relay_controller.h"
#include "ds18b20_sensor.h"
#include "motor_controller.h"

const char* ssid = "Ihr_SSID";
const char* password = "Ihr_Passwort";
WiFiManager wifiManager(ssid, password);

void setup() {
    
}

void setup() {
    // Initialisierungen hier
    Serial.begin(115200);
    wifiManager.setup();
    setupMQTT();
    setupRelays();
    setupDS18B20();
    setupMotors();
}

void loop() {
    // Hauptlogik hier
    wifiManager.loop();
    setupMQTT();
    loopRelays();
    loopDS18B20();
    loopMotors();
}
