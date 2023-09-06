#include "parameters.hpp"

#if !USING_R1


#include "R2Arms.hpp"
#include "pins.hpp"


Pusher::Pusher(std::function<void(float)> f, R2esp& esp32):
#if !USE_ESP_SW
    limit_sw(LS_1, false),
#endif
    pwmOut(PWM_6),
    dirOut(DIR_6),
    esp32(esp32)
{
    wait = f;
}

void Pusher::push(bool idle){
    dirOut.write(0);
    pwmOut.write(-0.3);

    timer.start();

    //limit_sw.riseAttachOnce([this](){pwmOut.write(0);});

    #if USE_ESP_SW
    if(true){
        while(esp32.limit_switch2){
            wait(0.001);
        }

    }
    #else
    while(!limit_sw.get()){
        wait(0.001);
    }
    #endif

    //wait(1);

    pwmOut.write(0);

    time = timer.read_ms();


    #if USE_ESP_SW

    esp32.attachPushed([this](){pwmOut.write(0);});

    timer.start();

    if(true){
        while(esp32.limit_switch2){
            wait(0.001);
        }

    }

    #endif

    
    
    
}

void Pusher::pull(bool idle){
    dirOut.write(1);
    pwmOut.write(0.3);

    timer.start();

    while(timer.read_ms() < time){
        //wait(0.001);
    }

    pwmOut.write(0);

    /*

    esp32.attachPulled([this](){pwmOut.write(0);});
    if(idle){
        while(esp32.limit_switch1){
            wait(0.001);
        }

    }
    */

}

R1Detector::R1Detector(std::function<void(float)> f): sw(PC_3)
{
    wait = f;
}

void R1Detector::waitForDetection(){
    while(!sw.get()){
        //wait(0.001);
    }
}

#endif
