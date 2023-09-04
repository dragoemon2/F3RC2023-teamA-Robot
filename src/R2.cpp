#include "parameters.hpp"

#if !USING_R1
#include "R2.hpp"
enum R2_MOVEMENT{
    GOTO_FIRST_RECEIVE,
    WAIT_FOR_FIRST_RECEIVE,
    GOTO_POSTZONE2,
    GOTO_SECOND_RECEIVE,
    WAIT_FOR_SECOND_RECEIVE,
    GOTO_POSTZONE3,
};


R2::R2():
    pass_detector([this](float x){wait_seconds(x);})
{

}

#if 0
//位置設定

    driveBase.goTo(2200,1250,PI);
    
    //受け取り

    driveBase.goTo(2864,1920,PI/2);
    driveBase.goTo(2864,2210,PI/2);

    //設置

    driveBase.goTo(2200,1250,PI);

    //受け取り

    driveBase.goTo(4050,213,0);
    driveBase.goTo(4280,213,0);

#endif

void R2::game(){
    for(int i=0;i<=R2_MOVEMENT::GOTO_POSTZONE3;i++){
        run(i);
    }
}


void R2::run(unsigned int movement_id){
    switch (movement_id)
    {
    case R2_MOVEMENT::GOTO_FIRST_RECEIVE:
        driveBase.localization.setPosition(WEST_WALL_X-ROBOTSIZE,NORTH_WALL_Y-ROBOTSIZE,PI);
        driveBase.goTo(2200,SOUTH_WALL_Y+1200,PI);
        break;

    case R2_MOVEMENT::WAIT_FOR_FIRST_RECEIVE:
        pass_detector.waitForDetection();
        break;

    case R2_MOVEMENT::GOTO_POSTZONE2:
        driveBase.goTo(2864,1920,PI/2);
        driveBase.goTo(2864,2210,PI/2);
        break;

    case R2_MOVEMENT::GOTO_SECOND_RECEIVE:
        driveBase.goTo(2200,SOUTH_WALL_Y+1200,PI);
        break; 

    case R2_MOVEMENT::WAIT_FOR_SECOND_RECEIVE:
        pass_detector.waitForDetection();
        break;

    case R2_MOVEMENT::GOTO_POSTZONE3:
        driveBase.goTo(4050,213,0);
        driveBase.goTo(4280,213,0);
        break;

    default:
        break;
    }
}

#endif