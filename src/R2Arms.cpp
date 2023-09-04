#include "parameters.hpp"

#if !USING_R1


#include "R2Arms.hpp"
#include "pins.hpp"


Pusher::Pusher(std::function<void(int)> f, R2esp& esp32):
    pwmOut(PWM_5),
    dirOut(DIR_5),
    esp32(esp32)
{
    wait = f;
}

void Pusher::push(bool idle){
    dirOut.write(0);
    pwmOut.write(0.3);

    esp32.attachPushed([this](){pwmOut.write(0);});

    if(idle){
        while(esp32.limit_switch2){
            wait(0.001);
        }

    }
    
    
}

void Pusher::pull(bool idle){
    dirOut.write(1);
    pwmOut.write(0.3);

    esp32.attachPulled([this](){pwmOut.write(0);});
    if(idle){
        while(esp32.limit_switch1){
            wait(0.001);
        }

    }
}

R1Detector::R1Detector(std::function<void(int)> f): sw(PC_3)
{
    wait = f;
}

void R1Detector::waitForDetection(){
    while(!sw.get()){
        wait(0.001);
    }
}

#endif
