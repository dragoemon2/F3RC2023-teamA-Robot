#include "parameters.hpp"
#if !USING_R1

#include "R2esp.hpp"

R2esp::R2esp(): serial(9600, ESP_UART_TX, ESP_UART_RX)
{
    limit_switch1 = true;
    limit_switch2 = false;
    pushed_func = [](){return;};
    pulled_func = [](){return;};
    serial.attach([this](char* str){onReceive(str);});
}

void R2esp::onReceive(char* str){
    //リミットスイッチ
    if(*str == 's'){
        int status;
        int N = sscanf(str, "s %d\n", &status);
        if(N!=1){return;}
        bool limit_switch1_last = limit_switch1;
        bool limit_switch2_last = limit_switch2;
        limit_switch1 = (status >> 0) & 1;
        limit_switch2 = (status >> 1) & 1;

        if(!limit_switch1_last && limit_switch1){
            pulled_func();
            pulled_func = [](){return;};
        }

        if(!limit_switch2_last && limit_switch2){
            pushed_func();
            pushed_func = [](){return;};
        }
    }
}

void R2esp::attachPushed(std::function<void(void)> f){
    pushed_func = f;
}


void R2esp::attachPulled(std::function<void(void)> f){
    pulled_func = f;
}


#endif