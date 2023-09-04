#pragma once
#include "parameters.hpp"
#if !USING_R1


#include "driveBase.hpp"
#include "laser.hpp"
#include "laserPos.hpp"
#include "laserUse.hpp"

class R2Lasers
{
    public:
        DriveBase& driveBase;

        //レーザー
        Laser laserCore1;
        Laser laserCore2;
        Laser laserCore3;

        //レーザーの位置
        LaserPos laser1;
        LaserPos laser2;
        LaserPos laser3;

        LaserPairUse pair;
        LaserUse back;

        void scan(float* X, float* Y, float* D);

        R2Lasers(DriveBase& driveBase);
};
#endif