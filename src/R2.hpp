#pragma once
#include "robot.hpp"

class R2: public Robot
{
    public:
        void game();
        void run(unsigned int movement_id);
        R2();
};