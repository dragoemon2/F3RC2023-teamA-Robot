#pragma once
#include "parameters.hpp"

#if !USING_R1
#include "robot.hpp"
#include "R2esp.hpp"
#include "R2Lasers.hpp"
#include "R2Arms.hpp"

class R2: public Robot
{
    public:
        R2esp esp32;
        R1Detector pass_detector;
        Pusher pusher;
        R2Lasers lasers;
        void game();
        void run(unsigned int movement_id);
        void loop();
        void wait_seconds(float seconds);
        void debug();
        R2();
};

#endif