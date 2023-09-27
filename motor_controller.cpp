#include "motor_controller.h"

MotorController::MotorController(RelayController& relayController)
    : _relayController(relayController), _motorRunTimeUp(0), _motorRunTimeDown(0), _currentState(IDLE), _startTime(0) {}

unsigned long MotorController::getMotorRunTimeUp() const {
    return _motorRunTimeUp;
}

unsigned long MotorController::getMotorRunTimeDown() const {
    return _motorRunTimeDown;
}

void MotorController::setup() {
    // Initialization code if needed
}

void MotorController::moveTableUp() {
    _relayController.setRelayState(RelayController::TABLE_DOWN, false);
    _currentState = WAITING_TO_MOVE_UP;
    _startTime = millis();
}

void MotorController::moveTableDown() {
    _relayController.setRelayState(RelayController::TABLE_UP, false);
    _currentState = WAITING_TO_MOVE_DOWN;
    _startTime = millis();
}

void MotorController::stopTable() {
    _relayController.setRelayState(RelayController::TABLE_UP, false);
    _relayController.setRelayState(RelayController::TABLE_DOWN, false);
    _currentState = IDLE;
}

void MotorController::setMotorRunTimeUp(unsigned long duration) {
    _motorRunTimeUp = duration;
}

void MotorController::setMotorRunTimeDown(unsigned long duration) {
    _motorRunTimeDown = duration;
}

void MotorController::update() {
    unsigned long elapsed = millis() - _startTime;

    switch (_currentState) {
        case WAITING_TO_MOVE_UP:
            if (elapsed >= 50) {
                _relayController.setRelayState(RelayController::TABLE_UP, true);
                _startTime = millis();
                _currentState = MOVING_UP;
            }
            break;

        case MOVING_UP:
            if (elapsed >= _motorRunTimeUp) {
                _relayController.setRelayState(RelayController::TABLE_UP, false);
                _currentState = IDLE;
            }
            break;

        case WAITING_TO_MOVE_DOWN:
            if (elapsed >= 50) {
                _relayController.setRelayState(RelayController::TABLE_DOWN, true);
                _startTime = millis();
                _currentState = MOVING_DOWN;
            }
            break;

        case MOVING_DOWN:
            if (elapsed >= _motorRunTimeDown) {
                _relayController.setRelayState(RelayController::TABLE_DOWN, false);
                _currentState = IDLE;
            }
            break;

        default:
            break;
    }
}
