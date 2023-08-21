#include <mbed.h>

class AirCylinder{
    public:
        DigitalOut digitalOut;
        AirCylinder(PinName pin);
        void open();
        void close();
};