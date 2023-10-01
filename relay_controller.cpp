#include "relay_controller.h"
#include "mqtt_manager.h"

constexpr int NUM_RELAYS = 6;

RelayController::RelayController(int serPin, int rclkPin, int srclkPin, int numOfShiftRegisters, MQTTManager* mqttManager)
    : _mqttManager(mqttManager), _shiftRegister(serPin, rclkPin, srclkPin) {
    _relayStates.fill(false);
    Serial.println("RelayController initialized");
}

void RelayController::setup() {
    for (int i = 0; i < static_cast<int>(Relay::NUM_RELAYS); i++) {
        _shiftRegister.set(i, LOW);
    }
    Serial.println("RelayController setup done");
    pinMode(SSR_FAN_PIN, OUTPUT);
    analogWrite(SSR_FAN_PIN, 0); // Setzt den PWM-Wert zu Beginn auf 0
}

void RelayController::setRelayState(int relayIndex, bool state) {
    
    if (relayIndex >= 0 && relayIndex < static_cast<int>(Relay::NUM_RELAYS)) {
        _relayStates[relayIndex] = state;
        _shiftRegister.set(relayIndex, state ? HIGH : LOW);
        Serial.print("Setting relay ");
        Serial.print(relayIndex);
        Serial.print(" to ");
        Serial.println(state ? "HIGH" : "LOW");
        if (relayIndex == static_cast<int>(Relay::KOFFERRAUM) && state) {
        _kofferraumStartTime = millis();
    }
    } else {
        if (_mqttManager) {
            const char* errorMsg = "Invalid relay index.";
            _mqttManager->reportError(errorMsg);
            Serial.println(errorMsg);
        }
    }
}

void RelayController::setSsrFanPwm(int pwmValue) {
    _ssrFanPwmValue = pwmValue;
    analogWrite(SSR_FAN_PIN, _ssrFanPwmValue);
}




unsigned long RelayController::getKofferraumDuration() const {
    return _kofferraumDuration;
}

void RelayController::setKofferraumDuration(unsigned long duration) {
    _kofferraumDuration = duration;
}

void RelayController::update() {
    // Implementierung der Methode hier
}

bool RelayController::getRelayState(int relayIndex) const {
    if (relayIndex >= 0 && relayIndex < static_cast<int>(Relay::NUM_RELAYS)) {
        return _relayStates[relayIndex];
    }
    return false;
}

void RelayController::setMQTTManager(MQTTManager* mqttManager) {
    _mqttManager = mqttManager;
}
