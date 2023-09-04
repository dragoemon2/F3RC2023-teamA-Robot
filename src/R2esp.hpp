#pragma once

#include "parameters.hpp"
#if !USING_R1

#include "serialCommunication.hpp"
#include "pins.hpp"
#include <functional>


class R2esp
{
    public:
        SerialCommunication serial;
        R2esp();
        void attachPushed(std::function<void(void)> f);
        void attachPulled(std::function<void(void)> f);
        void onReceive(char* str);
        bool limit_switch1;
        bool limit_switch2;
    private:
        std::function<void(void)> pushed_func;
        std::function<void(void)> pulled_func;
};


#endif