#pragma once
#include <mbed.h>
#include "driveBase.hpp"
#include "driveMotor.hpp"
#include "laserUse.hpp"
#include "buzzer.hpp"
#include "bkpRegister.hpp"
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
        std::vector<LaserUse> laserUses = {};
        std::vector<LaserPairUse> laserPairUses = {};


        Buzzer buzzer;
        BKPRegister backUpRegister;

        //タイマー
        Timer timer;

        void game();

        void init_driveBase();
        void init_lasers();
        void init_arms();
        void init_communication();

        void reboot();
        void init_status();

        void loop();

        void run0();
        void run1();
        Robot();

        int movementNum = 0;
        int motionNum = 0;
    private:
        int next_localization;
};