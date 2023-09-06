#include "parameters.hpp"

#if USING_R1

#include "R1.hpp"
#include "pin_check.hpp"




//mbed_error.cのWeak void mbed_error_hook()をオーバーロードする
void mbed_error_hook(const mbed_error_ctx *error_context)
{
    //リブートする．
    //robot.reboot();
}


R1 r1;

int main(){
    //開始
    printf("start\n");
    r1.debug();
    //pin_check();
    

    //robot.game();
    //robot.debug();
    return 0;
}

#else

#include "R2.hpp"

//mbed_error.cのWeak void mbed_error_hook()をオーバーロードする
void mbed_error_hook(const mbed_error_ctx *error_context)
{
    //リブートする．
    //robot.reboot();
}

R2 r2;

int main(){
    //printf("hoge\n");
    r2.debug();
    //r2.init_led();
    //r2.debug();
    //r2.debug();
    while(1){

    }
}



#endif