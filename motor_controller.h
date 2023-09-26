#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include "relay_controller.h"

class MotorController {
public:
    MotorController(RelayController& relayController);

    void setup();

    // Bewegt die Tischplatte nach oben oder unten
    void moveTableUp();
    void moveTableDown();
    void stopTable();

    // Setzt die Laufzeit der Motoren
    void setMotorRunTimeUp(unsigned long duration);
    void setMotorRunTimeDown(unsigned long duration);

private:
    RelayController& _relayController;
    unsigned long _motorRunTimeUp;
    unsigned long _motorRunTimeDown;
};

#endif // MOTOR_CONTROLLER_H

