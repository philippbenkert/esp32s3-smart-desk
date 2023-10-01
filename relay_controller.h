#ifndef RELAY_CONTROLLER_H
#define RELAY_CONTROLLER_H

#include <ShiftRegister74HC595.h>

class MQTTManager;

class RelayController {
    MQTTManager* _mqttManager;

public:
    RelayController(int serPin, int rclkPin, int srclkPin, int numOfShiftRegisters, MQTTManager* mqttManager = nullptr);
    void setMQTTManager(MQTTManager* mqttManager);
    void setup();
    unsigned long getKofferraumDuration() const;
    void setKofferraumDuration(unsigned long duration);
    void setRelayState(int relayIndex, bool state);
    bool getRelayState(int relayIndex) const;
    void update(); 
    void setSsrFanPwm(int pwmValue);

    enum class Relay {
    MONITOR,
    LAPTOP,
    TABLE_UP,
    TABLE_DOWN,
    FAN,
    SSR_FAN,  // Hinzufügen dieser Zeile
    KOFFERRAUM,
    NUM_RELAYS
};

private:
    ShiftRegister74HC595<1> _shiftRegister;
    std::array<bool, static_cast<int>(Relay::NUM_RELAYS)> _relayStates;
    unsigned long _kofferraumDuration = 0; // Zeitdauer für KOFFERRAUM
    unsigned long _kofferraumStartTime = 0;
    static constexpr int SSR_FAN_PIN = 8;
    static constexpr int PWM_MIN = 0; // 1% entspricht 30 PWM
    static constexpr int PWM_MAX = 255; // 100% entspricht 255 PWM
    int _ssrFanPwmValue = 0; // Aktueller PWM-Wert für den SSR-Fan
};

#endif // RELAY_CONTROLLER_H
