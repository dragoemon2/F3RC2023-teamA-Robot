#pragma once
#include "robot.hpp"
#include "movementManager.hpp"

class R1: public Robot
{
    R1();
    void moveHand();
    void loop();
    void run(unsigned int movement_id);
    void wait_seconds(float seconds);
    void next();

    bool auto_moving;
    MovementManager mm;
};