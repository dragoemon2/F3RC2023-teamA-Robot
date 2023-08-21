#pragma once

#include <mbed.h>
#include <vector>

enum class Error{
    UNPREDICTED,
    CONTROLLER,
    MEMORY,
    I2CLASER,
};

class Buzzer{

    
    public:
        DigitalOut digitalout;
        void message(std::vector<bool> list, unsigned int internal=300, unsigned int repeat=1);
        void morse(std::vector<bool> list, unsigned int internal=100, unsigned int repeat=1);

        void beep(unsigned int time);
        void alart(Error error);

        void stop();

        Buzzer(PinName pin);

        int _s1;
        int repeat_num = 0;
        
        
    private:
        void next();
        void getQueue(Error error, std::vector<bool>* list, unsigned int* repeat);
        std::vector<bool> array;
        int index;
        
        Ticker ticker;

};