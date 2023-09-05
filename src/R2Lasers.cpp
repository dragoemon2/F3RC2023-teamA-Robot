#include "parameters.hpp"
#if !USING_R1

#include "R2Lasers.hpp"

R2Lasers::R2Lasers(DriveBase& driveBase):
    driveBase(driveBase),

    laserCore1(LASER_1),
    laserCore2(LASER_2),
    laserCore3(LASER_3),

    laser1(130,-165,SOUTH,laserCore1),
    laser2(-130,-165,SOUTH,laserCore2),
    laser3(-165,150,WEST,laserCore3),

    pair(EAST, EAST_WALL_X, laser1, laser2),
    //back(NORTH, NORTH_WALL_Y, laser3),
    rightback(EAST, EAST_WALL_X, laser2),
    back(SOUTH, SOUTH_WALL_Y, laser3)
{
    driveBase.localization.addLocalization([this](float* X, float* Y, float* D) { scan(X, Y, D); }, 0);
}


void R2Lasers::scan(float* X, float* Y, float* D){
    rightback.scan(X, Y, D);
    //pair.scan(X, Y, D);
    //back.scan(X, Y, D);
    //back.scan(X, Y, D);
    //printf("%d %d %d\n", int(*X),int(*Y),int(180/PI* (*D)));
}

#endif