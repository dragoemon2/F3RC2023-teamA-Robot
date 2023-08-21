
#include <mbed.h>
#include "driveMotor.hpp"
#include "driveBase.hpp"
#include "parameters.hpp"
#include "serialCommunication.hpp"

#include "simulation.hpp"

void mbed_error_hook(const mbed_error_ctx *error_context)
{
    //エラー発生時の処理
    

    //プログラムを再スタート
    NVIC_SystemReset();
}
