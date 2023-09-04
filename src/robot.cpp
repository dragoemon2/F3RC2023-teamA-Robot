#include "robot.hpp"
#include "laserPos.hpp"
#include "pins.hpp"
#include "parameters.hpp"
#include <chrono>

using namespace std::chrono;

union FloatUIntUnion {
    float floatValue;
    unsigned int uintValue;
};

Robot::Robot()
#if USING_R1
 :  motor0(RE_1_A, RE_1_B, PWM_1, DIR_1, MOTOR_0_KP_1, MOTOR_0_KI_1, MOTOR_0_KD_1, MOTOR_0_KP_2, MOTOR_0_KI_2, MOTOR_0_KD_2, 0),
    motor1(RE_2_A, RE_2_B, PWM_2, DIR_2, MOTOR_1_KP_1, MOTOR_1_KI_1, MOTOR_1_KD_1, MOTOR_1_KP_2, MOTOR_1_KI_2, MOTOR_1_KD_2),
    motor2(RE_3_A, RE_3_B, PWM_3, DIR_3, MOTOR_2_KP_1, MOTOR_2_KI_1, MOTOR_2_KD_1, MOTOR_2_KP_2, MOTOR_2_KI_2, MOTOR_2_KD_2),
    motor3(RE_4_A, RE_4_B, PWM_4, DIR_4, MOTOR_3_KP_1, MOTOR_3_KI_1, MOTOR_3_KD_1, MOTOR_3_KP_2, MOTOR_3_KI_2, MOTOR_3_KD_2),
    driveBase(&motor0, &motor1, &motor2, &motor3),
    buzzer(BUZZER)
#else
 :  motor0(RE_1_A, RE_1_B, PWM_1, DIR_1, MOTOR_0_KP_1, MOTOR_0_KI_1, MOTOR_0_KD_1, MOTOR_0_KP_2, MOTOR_0_KI_2, MOTOR_0_KD_2),
    motor1(RE_2_A, RE_2_B, PWM_2, DIR_2, MOTOR_1_KP_1, MOTOR_1_KI_1, MOTOR_1_KD_1, MOTOR_1_KP_2, MOTOR_1_KI_2, MOTOR_1_KD_2),
    motor2(RE_3_A, RE_3_B, PWM_3, DIR_3, MOTOR_2_KP_1, MOTOR_2_KI_1, MOTOR_2_KD_1, MOTOR_2_KP_2, MOTOR_2_KI_2, MOTOR_2_KD_2),
    motor3(RE_4_A, RE_4_B, PWM_4, DIR_4, MOTOR_3_KP_1, MOTOR_3_KI_1, MOTOR_3_KD_1, MOTOR_3_KP_2, MOTOR_3_KI_2, MOTOR_3_KD_2),
    driveBase(&motor0, &motor1, &motor2, &motor3),
    buzzer(BUZZER)
#endif
{
    timer.start();
    driveBase.attachLoop([this]{loop();});
    init_arms();
    init_status();
}

//試合
void Robot::game(){

}

void Robot::run(unsigned int movement_id){

}

void Robot::stopCurrentMovement(){
    driveBase.stopMovement();
    //アームとかも止める
}


void Robot::init_arms(){

}


void Robot::wait_seconds(float seconds){
    int started = duration_cast<microseconds>(timer.elapsed_time()).count();
    while(duration_cast<microseconds>(timer.elapsed_time()).count() - started < 1000000*seconds){
        loop();
        wait_ns(1);
    }
}



void Robot::init_status(){
    backUpRegister.open(0);
    int errCode = backUpRegister.get(0,4);
    if(errCode){
        //前回エラーが起きたので復元

        //動作番号の復元

        //自己位置の復元
        backUpRegister.open(1);
        float X = float(backUpRegister.get(0,16));
        float Y = float(backUpRegister.get(16,16));
        FloatUIntUnion directionUnion;
        backUpRegister.open(2);   
        directionUnion.uintValue = backUpRegister.get(0,32);
        float D = directionUnion.floatValue;
        driveBase.localization.setPosition(X, Y, D);

        //警告音の再生
        buzzer.alart(Error::UNPREDICTED);
    }else{
        driveBase.localization.setPosition(0, 0, 0);
    }


    //正常と記録しておく
    backUpRegister.open(0);
    backUpRegister.set(0,4,0);
    backUpRegister.save();

}

void Robot::reboot(){
    //エラーコードなど
    backUpRegister.open(0);
    backUpRegister.set(0,4,1);
    //backUpRegister.set(4,6,動作番号);
    //backUpRegister.set(10,6,動作段階);
    backUpRegister.save();

    backUpRegister.open(1);
    backUpRegister.set(0,16,short(driveBase.localization.posX));
    backUpRegister.set(16,16,short(driveBase.localization.posY));
    backUpRegister.save();

    FloatUIntUnion directionUnion;
    directionUnion.floatValue = driveBase.localization.direction;
    backUpRegister.open(2);
    backUpRegister.set(0,32,reinterpret_cast<unsigned int>(directionUnion.uintValue));
    backUpRegister.save();

    //プログラムを再スタート
    NVIC_SystemReset();
}

void Robot::loop(){
    if(duration_cast<milliseconds>(timer.elapsed_time()).count() > next_localization_time){
        driveBase.localization.loop();
        next_localization_time += 1000/SPEED_ADJUSTMENT_FREQUENCY;
    }
}