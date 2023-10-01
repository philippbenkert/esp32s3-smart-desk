#include "motor_controller.h"
#include "relay_controller.h"

constexpr unsigned long WAIT_DURATION = 50;

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
    _relayController.setRelayState(static_cast<int>(RelayController::Relay::TABLE_DOWN), false);
    _currentState = WAITING_TO_MOVE_UP;
    _startTime = millis();
        Serial.println("Moving table up");

}

void MotorController::moveTableDown() {
    _relayController.setRelayState(static_cast<int>(RelayController::Relay::TABLE_UP), false);
    _currentState = WAITING_TO_MOVE_DOWN;
    _startTime = millis();
        Serial.println("Moving table down");

}

void MotorController::stopTable() {
_relayController.setRelayState(static_cast<int>(RelayController::Relay::TABLE_UP), false);
_relayController.setRelayState(static_cast<int>(RelayController::Relay::TABLE_DOWN), false);
    _currentState = IDLE;
        Serial.println("Stopping table movement");

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
            if (!_relayController.getRelayState(static_cast<int>(RelayController::Relay::TABLE_UP))) {
                _relayController.setRelayState(static_cast<int>(RelayController::Relay::TABLE_UP), true);
                // ... (restlicher Code bleibt unverändert)
            }
            break;

        case MOVING_UP:
            if (_relayController.getRelayState(static_cast<int>(RelayController::Relay::TABLE_UP))) {
    _relayController.setRelayState(static_cast<int>(RelayController::Relay::TABLE_UP), false);
    _currentState = IDLE;
}
            break;

        case WAITING_TO_MOVE_DOWN:
            if (!_relayController.getRelayState(static_cast<int>(RelayController::Relay::TABLE_DOWN))) {
                _relayController.setRelayState(static_cast<int>(RelayController::Relay::TABLE_DOWN), true);
                // ... (restlicher Code bleibt unverändert)
            }
            break;

        case MOVING_DOWN:
            if (elapsed >= _motorRunTimeDown) {
                    if (_relayController.getRelayState(static_cast<int>(RelayController::Relay::TABLE_DOWN))) {
                    _relayController.setRelayState(static_cast<int>(RelayController::Relay::TABLE_DOWN), false);
                }
                _currentState = IDLE;
            }
            break;

        default:
            break;
    }
}

