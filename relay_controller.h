#ifndef RELAY_CONTROLLER_H
#define RELAY_CONTROLLER_H

#include <ShiftRegister74HC595.h>

class RelayController {
public:
    RelayController(int serPin, int rclkPin, int srclkPin, int numOfShiftRegisters);

    void setup();

    void setRelayState(int relayIndex, bool state);
    bool getRelayState(int relayIndex) const;

    enum Relay {
        MONITOR = 0,
        LAPTOP,
        TABLE_UP,
        TABLE_DOWN,
        RESERVE1,
        RESERVE2
    };

private:
    ShiftRegister74HC595<numOfShiftRegisters> _shiftRegister;
    std::array<bool, 6> _relayStates;
};

#endif // RELAY_CONTROLLER_H

