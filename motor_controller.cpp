
#include "motor_controller.h"

MotorController::MotorController(RelayController& relayController)
    : _relayController(relayController), _motorRunTimeUp(0), _motorRunTimeDown(0) {}

void MotorController::setup() {
    // Hier können Sie Initialisierungscode hinzufügen, falls erforderlich.
}

void MotorController::moveTableUp() {
    // Stellen Sie sicher, dass das "runter"-Relais deaktiviert ist
    _relayController.setRelayState(RelayController::TABLE_DOWN, false);
    delay(50);  // Ein kurzes Delay, um sicherzustellen, dass das Relais vollständig deaktiviert ist

    _relayController.setRelayState(RelayController::TABLE_UP, true);
    delay(_motorRunTimeUp);
    _relayController.setRelayState(RelayController::TABLE_UP, false);
}

void MotorController::stopTable() {
    _relayController.setRelayState(RelayController::TABLE_UP, false);
    _relayController.setRelayState(RelayController::TABLE_DOWN, false);
}

void MotorController::moveTableDown() {
    // Stellen Sie sicher, dass das "hoch"-Relais deaktiviert ist
    _relayController.setRelayState(RelayController::TABLE_UP, false);
    delay(50);  // Ein kurzes Delay, um sicherzustellen, dass das Relais vollständig deaktiviert ist

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
