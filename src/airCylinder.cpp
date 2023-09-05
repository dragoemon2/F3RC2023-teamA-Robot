#include <mbed.h>
#include "airCylinder.hpp"


AirCylinder::AirCylinder(PinName pin): digitalOut(pin)
{
    
}

void AirCylinder::open(){
    digitalOut.write(0);
}

void AirCylinder::close(){
    digitalOut.write(1);
}