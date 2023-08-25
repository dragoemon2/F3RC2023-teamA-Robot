#pragma once
#include "robot.hpp"
#include "movementManager.hpp"

class R1: public Robot
{
    public:
        R1();
        void moveHand();
        void adjustment();
        void loop();
        void run(unsigned int movement_id);
        void wait_seconds(float seconds);
        void next();
        void game();

        bool auto_moving;
        MovementManager mm;
};