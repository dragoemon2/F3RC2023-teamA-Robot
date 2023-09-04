#pragma once
#include <functional>
#include "servoMotor.hpp"
#include "airCylinder.hpp"

class BottleArm
{
    public:
        PwmOut pwmOut;
        DigitalOut dirOut;
        ServoMotor spinner;
        ServoMotor hand;
        void close(bool idle=true);
        void open(bool idle=true);
        void up(bool idle=true);
        void down(bool idle=true);
        void spinDown(bool idle=true);
        void spinUp(bool idle=true);
        BottleArm(std::function<void(int)> f);
    private:
        std::function<void(int)> wait = [](int x){return;};
        
};


class ContainerArm
{
    public:
        AirCylinder aircylinder;
        void close(bool idle=true);
        void open(bool idle=true);
        ContainerArm(std::function<void(int)> f);
    private:
        std::function<void(int)> wait;
};