#include "R1Arms.hpp"


BottleArm::BottleArm(std::function<void(int)> f): slider(D1), hand(D2)
{
    wait = f;
}


void BottleArm::open(bool idle){
    //hand.moveTo();
    if(idle){
        wait(1000);
    }
}

void BottleArm::close(bool idle){
    //hand.moveTo();
    if(idle){
        wait(1000);
    }
}


void BottleArm::up(bool idle){
    //slider.moveTo();
    if(idle){
        wait(1000);
    }
}

void BottleArm::down(bool idle){
    //slider.moveTo();
    if(idle){
        wait(1000);
    }
}



ContainerArm::ContainerArm(std::function<void(int)> f): aircylinder(A0)
{
    wait = f;
}


void ContainerArm::open(bool idle){
    aircylinder.open();
    if(idle){
        wait(1000);
    }
}

void ContainerArm::close(bool idle){
    aircylinder.close();
    if(idle){
        wait(1000);
    }
}
