#include "laser.hpp"
#include "laserPos.hpp"

LaserPos::LaserPos(int posX, int posY, float direction, _Laser& laser): laser(laser), posX(posX), posY(posY), direction(direction) {
    last_value = -1;
}

int LaserPos::read(){
    //return -1;
    int value = laser.read();
    if(value > 0){
        last_value = value;
    }
    //printf("%d\n", value);
    return value;
}

int LaserPos::readDenoise(){
    int value = laser.read();
    if(value > 0){
        last_value = value;
    }
    //printf("%d\n", value);
    return value;
}

int LaserPos::getLastValue(){
    return last_value;
}


