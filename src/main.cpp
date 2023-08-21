#include "robot.hpp"

Robot robot;

//mbed_error.cのWeak void mbed_error_hook()をオーバーロードする
void mbed_error_hook(const mbed_error_ctx *error_context)
{
    //リブートする．
    robot.reboot();
}

int main(){
    //開始
    robot.game();
    return 0;
}
