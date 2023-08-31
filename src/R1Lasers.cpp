#include "R1Lasers.hpp"

R1Lasers::R1Lasers(DriveBase& driveBase):
    driveBase(driveBase),

    laserCore1(1, A0),
    laserCore2(2, A1),
    laserCore3(3, A2),
    laserCore4(4, A3),

    laser1(130,-165,SOUTH,laserCore1),
    laser2(-130,-165,SOUTH,laserCore2),
    laser3(-165,0,WEST,laserCore3),
    laser4(165,-150,WEST,laserCore4),

    pair(EAST, EAST_WALL_X, laser1, laser2),
    back(NORTH, NORTH_WALL_Y, laser3),
    front(SOUTH, SOUTH_WALL_Y, laser4)

{
    init();
    driveBase.localization.addLocalization([this](float* X, float* Y, float* D) { scan(X, Y, D); }, 0);
}

void R1Lasers::init(){
    laserCore1.init();
    laserCore2.init();
    laserCore3.init();
    laserCore4.init();
}

void R1Lasers::scan(float* X, float* Y, float* D){
    pair.scan(X, Y, D);
    back.scan(X, Y, D);
    front.scan(X, Y, D);
}