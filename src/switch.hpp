#pragma once
#include <mbed.h>
#include <functional>

class Switch
{
    public:
        InterruptIn interruptin;
        Ticker ticker;
        bool high_on_pushed;
        bool get(); //状態(押されてるかどうか)
        void riseAttachOnce(std::function<void(void)> f);
        void fallAttachOnce(std::function<void(void)> f);
        Switch(PinName pin,bool high_on_pushed=true, unsigned int interval=10, unsigned int detectionThreshold=3);
    private:
        unsigned int interval; //サンプリング周期
        unsigned int detectionThreshold; //検出回数(立ち上がり/立ち下がりを含まない)
        void riseInterrupt(); //立ち上がり割り込み
        void fallInterrupt(); //立ち下がり割り込み
        void riseCheck(); //サンプリング
        void fallCheck(); //サンプリング
        std::function<void(void)> riseFunc;
        std::function<void(void)> fallFunc;
        bool status; //状態(押されてるかどうか)
        bool monitoring;
        unsigned int counter=0;
};

class SimpleSwitch
{
    public:
        DigitalIn digitalin;
        Ticker ticker;
        SimpleSwitch(PinName pin, bool high_on_pushed=true);
        bool get();
        void check();
        void riseAttachOnce(std::function<void(void)> f);
        void fallAttachOnce(std::function<void(void)> f);
        bool high_on_pushed;
        int monitoring=0;
        std::function<void(void)> func;
};