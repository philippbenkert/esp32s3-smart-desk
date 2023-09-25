#include "relay_controller.h"

RelayController::RelayController(int serPin, int rclkPin, int srclkPin, int numOfShiftRegisters)
    : _shiftRegister(numOfShiftRegisters, serPin, rclkPin, srclkPin) {
    _relayStates.fill(false);
}

void RelayController::setup() {
    for (int i = 0; i < 6; i++) {
        _shiftRegister.set(i, LOW);
    }
}

void RelayController::setRelayState(int relayIndex, bool state) {
    if (relayIndex >= 0 && relayIndex < 6) {
        _relayStates[relayIndex] = state;
        _shiftRegister.set(relayIndex, state ? HIGH : LOW);
    }
}

bool RelayController::getRelayState(int relayIndex) const {
    if (relayIndex >= 0 && relayIndex < 6) {
        return _relayStates[relayIndex];
    }
    return false;
}

