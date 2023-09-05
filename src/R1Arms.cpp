#include "parameters.hpp"

#if USING_R1

#include "R1Arms.hpp"
#include "pins.hpp"


BottleArm::BottleArm(std::function<void(int)> f): pwmOut(PWM_6), dirOut(DIR_6), spinner(SV_1_SIGNAL), hand(SV_2_SIGNAL), limit_switch1(LS_1, false), limit_switch2(LS_2, false)
{
    wait = f;
}


void BottleArm::open(bool idle){
    hand.moveTo(90);
    if(idle){
        wait(1000);
    }
}

void BottleArm::close(bool idle){
    hand.moveTo(0);
    if(idle){
        wait(1000);
    }
}


void BottleArm::up(bool idle){
    //slider.moveTo();
    dirOut.write(0);
    pwmOut.write(0.3);
    limit_switch1.riseAttachOnce([this](){pwmOut.write(0);});
    if(idle){
        while(!limit_switch1.get()){
            wait(0.001);
        }
    }
}

void BottleArm::down(bool idle){
    //slider.moveTo();
    dirOut.write(1);
    pwmOut.write(0.3);
    limit_switch2.riseAttachOnce([this](){pwmOut.write(0);});
    if(idle){
        while(!limit_switch2.get()){
            wait(0.001);
        }
    }
}

void BottleArm::spinDown(bool idle){
    spinner.moveTo(0);
    if(idle){
        wait(1000);
    }
}

void BottleArm::spinUp(bool idle){
    spinner.moveTo(90);
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

#endif