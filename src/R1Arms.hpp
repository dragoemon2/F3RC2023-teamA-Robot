#pragma once
#include "parameters.hpp"

#if USING_R1
#include <functional>
#include "servoMotor.hpp"
#include "airCylinder.hpp"
#include "switch.hpp"

class BottleArm
{
    public:
        PwmOut pwmOut;
        DigitalOut dirOut;
        ServoMotor spinner;
        ServoMotor hand;
        //Switch limit_switch1;
        //Switch limit_switch2;
        SimpleSwitch limit_switch1;
        SimpleSwitch limit_switch2;
        void close(bool idle=true);
        void open(bool idle=true);
        void up(bool idle=true);
        void down(bool idle=true);
        void spinDown(bool idle=true);
        void spinUp(bool idle=true);
        BottleArm(std::function<void(float)> f);
    private:
        std::function<void(float)> wait = [](float x){return;};
        
};


class ContainerArm
{
    public:
        AirCylinder aircylinder;
        void close(bool idle=true);
        void open(bool idle=true);
        ContainerArm(std::function<void(int)> f);
        bool opened=true;
    private:
        std::function<void(int)> wait;
};
#endif