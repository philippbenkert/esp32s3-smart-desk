
#include "motor_controller.h"

MotorController::MotorController(RelayController& relayController)
    : _relayController(relayController), _motorRunTimeUp(0), _motorRunTimeDown(0) {}

void MotorController::setup() {
    // Hier können Sie Initialisierungscode hinzufügen, falls erforderlich.
}

void MotorController::moveTableUp() {
    _relayController.setRelayState(RelayController::TABLE_UP, true);
    delay(_motorRunTimeUp);
    _relayController.setRelayState(RelayController::TABLE_UP, false);
}

void MotorController::moveTableDown() {
    _relayController.setRelayState(RelayController::TABLE_DOWN, true);
    delay(_motorRunTimeDown);
    _relayController.setRelayState(RelayController::TABLE_DOWN, false);
}

void MotorController::setMotorRunTimeUp(unsigned long duration) {
    _motorRunTimeUp = duration;
}

void MotorController::setMotorRunTimeDown(unsigned long duration) {
    _motorRunTimeDown = duration;
}
