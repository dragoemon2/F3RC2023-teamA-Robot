#pragma once
#include "parameters.hpp"

#if !USING_R1
#include "robot.hpp"

class R2: public Robot
{
    public:
        void game();
        void run(unsigned int movement_id);
        R2();
};

#endif