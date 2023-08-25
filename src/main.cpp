#include "R1.hpp"

R1 robot;


//mbed_error.cのWeak void mbed_error_hook()をオーバーロードする
void mbed_error_hook(const mbed_error_ctx *error_context)
{
    //リブートする．
    //robot.reboot();
}



int main(){
    //開始
    printf("hoge\n");
    robot.game();
    return 0;
}
