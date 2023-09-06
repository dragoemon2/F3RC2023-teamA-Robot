#include "parameters.hpp"

#if USING_R1

#include "R1Arms.hpp"
#include "pins.hpp"


BottleArm::BottleArm(std::function<void(float)> f): pwmOut(PWM_5), dirOut(DIR_5), spinner(SV_2_SIGNAL), hand(SV_1_SIGNAL), limit_switch1(LS_1, false), limit_switch2(LS_2, false)
{
    wait = f;
}


void BottleArm::open(bool idle){
    #if USING_PETBOTTLE

    #if !USING_SECOND_F446
    hand.moveTo(0);
    if(idle){
        wait(1);
    }

    #endif

    #endif
}

void BottleArm::close(bool idle){
    #if USING_PETBOTTLE

    #if !USING_SECOND_F446
    hand.moveTo(90);
    if(idle){
        wait(1);
    }
    #endif
    
    #endif
}


void BottleArm::up(bool idle){
    //slider.moveTo();
    #if USING_PETBOTTLE
    dirOut.write(0);
    pwmOut.write(0.1);
    


    //bool finisihed=false;
    
    limit_switch1.riseAttachOnce([this](){pwmOut.write(0);});
    if(idle){
        while(!limit_switch1.get()){
            wait(0.001);
        }
    }
    #endif
}

void BottleArm::down(bool idle){
    //slider.moveTo();
    #if USING_PETBOTTLE

    
    dirOut.write(1);
    pwmOut.write(0.1);
    limit_switch2.riseAttachOnce([this](){pwmOut.write(0);});
    if(idle){
        while(!limit_switch2.get()){
            wait(0.001);
        }
    }

    #endif
}

void BottleArm::spinDown(bool idle){
    #if USING_PETBOTTLE

    #if !USING_SECOND_F446

    spinner.moveTo(90);
    if(idle){
        wait(1);
    }

    #endif

    #endif
}

void BottleArm::spinUp(bool idle){
    #if USING_PETBOTTLE

    #if !USING_SECOND_F446

    spinner.moveTo(0);
    if(idle){
        wait(1);
    }

    #endif


    #endif
}



ContainerArm::ContainerArm(std::function<void(int)> f): aircylinder(SOLENOID)
{
    wait = f;
}


void ContainerArm::open(bool idle){
    aircylinder.open();
    opened = true;
    if(idle){
        wait(1);
    }
}

void ContainerArm::close(bool idle){
    aircylinder.close();
    opened = false;
    if(idle){
        wait(1);
    }
}

#endif