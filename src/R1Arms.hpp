#pragma once
#include <functional>
#include "servoMotor.hpp"
#include "airCylinder.hpp"

class BottleArm
{
    public:
        ServoMotor slider;
        ServoMotor hand;
        void close(bool idle=true);
        void open(bool idle=true);
        void up(bool idle=true);
        void down(bool idle=true);
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
        void attachLoop(std::function<void(void)> f);
        ContainerArm(std::function<void(int)> f);
    private:
        std::function<void(int)> wait;
};