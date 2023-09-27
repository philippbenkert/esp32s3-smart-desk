#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include "relay_controller.h"

class MotorController {
public:
    MotorController(RelayController& relayController);
    void setup();
    void moveTableUp();
    void moveTableDown();
    void stopTable();
    void setMotorRunTimeUp(unsigned long duration);
    void setMotorRunTimeDown(unsigned long duration);
    unsigned long getMotorRunTimeUp() const;
    unsigned long getMotorRunTimeDown() const;
    void update();

private:
    enum State {
        IDLE,
        WAITING_TO_MOVE_UP,
        MOVING_UP,
        WAITING_TO_MOVE_DOWN,
        MOVING_DOWN
    };

    RelayController& _relayController;
    unsigned long _motorRunTimeUp;
    unsigned long _motorRunTimeDown;
    State _currentState;
    unsigned long _startTime;
};

#endif // MOTOR_CONTROLLER_H
