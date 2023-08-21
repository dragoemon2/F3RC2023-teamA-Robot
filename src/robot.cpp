#include "robot.hpp"


Robot::Robot(){
    timer.start();
    init_driveBase();
    init_lasers();
    init_arms();
    init_buzzer();
    init_leds();
    recover();
}

//試合
Robot::game(){

}

//モーターの初期化
Robot::init_driveBase(){
    motor0(A0, A1, D10, D11, MOTOR_0_KP_1, MOTOR_0_KI_1, MOTOR_0_KD_1, MOTOR_0_KP_2, MOTOR_0_KI_2, MOTOR_0_KD_2);
    motor1(A2, A3, D4, D5, MOTOR_1_KP_1, MOTOR_1_KI_1, MOTOR_1_KD_1, MOTOR_1_KP_2, MOTOR_1_KI_2, MOTOR_1_KD_2);
    motor2(A4, A5, D6, D7, MOTOR_2_KP_1, MOTOR_2_KI_1, MOTOR_2_KD_1, MOTOR_2_KP_2, MOTOR_2_KI_2, MOTOR_2_KD_2);
    motor3(D2, D3, D8, D9, MOTOR_3_KP_1, MOTOR_3_KI_1, MOTOR_3_KD_1, MOTOR_3_KP_2, MOTOR_3_KI_2, MOTOR_3_KD_2);
    driveBase(&motor0, &motor1, &motor2, &motor3);

    driveBase.attachLoop(&loop);
}

//レーザーの初期化
Robot::init_lasers(){
    for(_LaserUse lu: laseruses){
        driveBase.localization.addLocalization(lu.scan)
    }
}

Robot::init_arms(){

}

Robot::init_buzzer(){
    
}

Robot::init_leds(){
    
}

Robot::init_communication(){
    
}

Robot::recover(){
    
}

Robot::loop(){
    driveBase.localization.loop();
}