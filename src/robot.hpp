#pragma once
#include <mbed.h>
#include "driveBase.hpp"
#include "driveMotor.hpp"
#include "laserUse.hpp"
#include "buzzer.hpp"
#include <vector>
//R1,R2の基底クラス

class Robot
{
    public:
        //足回り
        DriveMotor motor0;
        DriveMotor motor1;
        DriveMotor motor2;
        DriveMotor motor3;
        DriveBase driveBase;

        
        //レーザー
        std::vector<_LaserUse> laseruses;


        Buzzer buzzer;

        //タイマー
        Timer timer;

        void game();

        void init_driveBase();
        void init_lasers();
        void init_arms();
        void init_buzzers();
        void init_leds();
        void init_communication();
        void recover();

        void loop();
        Robot();
    private:
        int next_localization;
}