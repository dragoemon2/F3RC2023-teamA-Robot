#include "parameters.hpp"

#include "simulation.hpp"

#if !USING_R1

#include <chrono>
using namespace std::chrono;

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
    pass_detector([this](float x){wait_seconds(x);}),
    pusher([this](float x){wait_seconds(x);}, esp32),
    lasers(driveBase)
{
    driveBase.attachLoop([this](){loop();});

    #if SIMULATION
    MotorSimulation simulation0(&motor0);
    MotorSimulation simulation1(&motor1);
    MotorSimulation simulation2(&motor2);
    MotorSimulation simulation3(&motor3);
    #endif
}


void R2::game(){
    for(int i=0;i<=R2_MOVEMENT::GOTO_POSTZONE3;i++){
        run(i);
    }
}


void R2::run(unsigned int movement_id){
    switch (movement_id)
    {
    case R2_MOVEMENT::GOTO_FIRST_RECEIVE:
        driveBase.localization.setPosition(EAST_WALL_X-ROBOTSIZE-50,NORTH_WALL_Y-ROBOTSIZE,PI);

        lasers.pair.activate(NORTH, NORTH_WALL_Y);
        lasers.back.activate(EAST, EAST_WALL_X);

        driveBase.goTo(EAST_WALL_X-1798-500+ROBOTSIZE+150,SOUTH_WALL_Y+1200,PI);

        lasers.pair.inactivate();

        driveBase.goTo(2200,SOUTH_WALL_Y+1200,PI);

        lasers.back.inactivate();
        break;

    case R2_MOVEMENT::WAIT_FOR_FIRST_RECEIVE:
        pass_detector.waitForDetection();
        break;

    case R2_MOVEMENT::GOTO_POSTZONE2:
        driveBase.goTo(2864,1920,PI/2);

        lasers.pair.activate(EAST, EAST_WALL_X);
        lasers.back.activate(SOUTH, -SOUTH_WALL_Y);

        driveBase.goTo(2864,2210,PI/2);

        lasers.pair.inactivate();
        lasers.back.inactivate();

        //pusher.push(true);
        //pusher.pull(false);

        break;

    case R2_MOVEMENT::GOTO_SECOND_RECEIVE:
        driveBase.goTo(2864,1920,PI/2);

        lasers.pair.activate(NORTH, NORTH_WALL_Y);
        lasers.back.activate(EAST, EAST_WALL_X);

        driveBase.goTo(2200,SOUTH_WALL_Y+1200,PI);

        lasers.pair.inactivate();
        lasers.back.inactivate();
        break; 

    case R2_MOVEMENT::WAIT_FOR_SECOND_RECEIVE:
        pass_detector.waitForDetection();
        break;

    case R2_MOVEMENT::GOTO_POSTZONE3:
        driveBase.goTo(4050, SOUTH_WALL_Y+1200, 0);
        driveBase.rotateTo(-PI/2);
        driveBase.goTo(4050,500,0);
        lasers.pair.activate(SOUTH, -SOUTH_WALL_Y);
        driveBase.goTo(4280,213,0);

        lasers.pair.inactivate();

        //pusher.push(true);
        //pusher.pull(false);

        break;

    default:
        break;
    }
}

void R2::loop(){
    if(duration_cast<milliseconds>(timer.elapsed_time()).count() > next_localization_time){
        driveBase.localization.loop();
        next_localization_time += 1000/SPEED_ADJUSTMENT_FREQUENCY;
    }
    printf("%d,%d,%d| %d %d %d %d\n",int(driveBase.localization.posX), int(driveBase.localization.posY), int(180/PI*driveBase.localization.direction), int(motor0.encoder.getAmount()), int(motor1.encoder.getAmount()), int(motor2.encoder.getAmount()), int(motor3.encoder.getAmount()));
}

void R2::wait_seconds(float seconds){
    int started = duration_cast<microseconds>(timer.elapsed_time()).count();
    while(duration_cast<microseconds>(timer.elapsed_time()).count() - started < 1000000*seconds){
        loop();
        wait_ns(1);
    }
}

#endif