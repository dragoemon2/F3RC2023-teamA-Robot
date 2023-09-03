#pragma once
#include <mbed.h>
#include "driveMotor.hpp"
#include "switch.hpp"
#include <functional>


class Pusher
{
    public:
        PwmOut pwmOut;
        DigitalOut dirOut;
        Switch limit_switch;
        Pusher(std::function<void(int)> f);

    private:
        std::function<void(int)> wait;

};


class R1Detector
{
    public:
        R1Detector();
        void waitForDetection();

    private:
        std::function<void(int)> wait;

};