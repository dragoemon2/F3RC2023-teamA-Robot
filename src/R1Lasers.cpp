#include "R1Lasers.hpp"

R1Lasers::R1Lasers(DriveBase& driveBase):
    driveBase(driveBase),

    laserCore1(66, PB_12),
    laserCore2(65, PA_12),
    //laserCore3(3, A2),
    laserCore4(64, PA_11),

    laser1(130,-165,SOUTH,laserCore1),
    laser2(-130,-165,SOUTH,laserCore2),
    //laser3(-165,0,WEST,laserCore3),
    laser4(165,-150,EAST,laserCore4),

    pair(EAST, EAST_WALL_X, laser1, laser2),
    //back(NORTH, NORTH_WALL_Y, laser3),
    front(SOUTH, SOUTH_WALL_Y, laser4)
{
    init();
    driveBase.localization.addLocalization([this](float* X, float* Y, float* D) { scan(X, Y, D); }, 0);
}

void R1Lasers::init(){
    laserCore1.init();
    laserCore2.init();
    //laserCore3.init();
    laserCore4.init();
}

void R1Lasers::scan(float* X, float* Y, float* D){
    pair.scan(X, Y, D);
    //back.scan(X, Y, D);
    front.scan(X, Y, D);
    //printf("%d %d %d\n", int(*X),int(*Y),int(180/PI* (*D)));
}