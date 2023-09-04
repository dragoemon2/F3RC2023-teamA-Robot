#pragma once
#include "parameters.hpp"

#if !USING_R1

#include <mbed.h>
#include "driveMotor.hpp"
#include "switch.hpp"
#include "R2esp.hpp"
#include <functional>


class Pusher
{
    public:
        PwmOut pwmOut;
        DigitalOut dirOut;
        R2esp& esp32;
        Pusher(std::function<void(int)> f, R2esp& esp32);
        void push(bool idle=true);
        void pull(bool idle=true);

    private:
        std::function<void(int)> wait;

};


class R1Detector
{
    public:
        Switch sw;
        R1Detector(std::function<void(int)> f);
        void waitForDetection();

    private:
        
        std::function<void(int)> wait;

};

#endif