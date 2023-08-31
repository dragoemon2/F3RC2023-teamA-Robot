#pragma once
#include "tofLaser.hpp"
#include "laserPos.hpp"
#include "laserUse.hpp"
#include "driveBase.hpp"


class R1Lasers
{
    public:
        DriveBase& driveBase;

        //レーザー
        TOFLaser laserCore1;
        TOFLaser laserCore2;
        TOFLaser laserCore3;
        TOFLaser laserCore4;

        //レーザーの位置
        LaserPos laser1;
        LaserPos laser2;
        LaserPos laser3;
        LaserPos laser4;

        LaserPairUse pair;
        LaserUse back;
        LaserUse front;

        void scan(float* X, float* Y, float* D);

        void init();

        /*

        //東側の壁を右センサー2つで見る
        LaserPairUse  lu1;

        //北側の壁を右センサー2つで見る
        LaserPairUse  lu2;


        LaserUse      lu3;
        LaserUse      lu4;
        LaserUse      lu5;
        LaserUse      lu6;
        */

        R1Lasers(DriveBase& driveBase);
};