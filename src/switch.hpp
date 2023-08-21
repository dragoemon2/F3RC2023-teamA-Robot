#pragma once
#include <mbed.h>

class Switch
{
    public:
        InterruptIn interruptin;
        Ticker ticker;
        bool get(); //状態(押されてるかどうか)
        Switch(PinName pin, unsigned int interval=10, unsigned int detectionThreshold=3);
    private:
        unsigned int interval; //サンプリング周期
        unsigned int detectionThreshold; //検出回数(立ち上がり/立ち下がりを含まない)
        void riseInterrupt(); //立ち上がり割り込み
        void fallInterrupt(); //立ち下がり割り込み
        void riseCheck(); //サンプリング
        void fallCheck(); //サンプリング
        bool status; //状態(押されてるかどうか)
        bool monitoring;
        unsigned int counter=0;
};