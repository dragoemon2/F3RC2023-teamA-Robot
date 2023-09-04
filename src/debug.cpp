#include "R1.hpp"
#include "R2.hpp"
#include "parameters.hpp"
#include "driveBase.hpp"
#include "pins.hpp"


void dir_check(DriveBase& driveBase){
    driveBase.attachLoop([&driveBase]{printf("%d,%d,%d| %d %d %d %d\n",int(driveBase.localization.posX), int(driveBase.localization.posY), int(180/PI*driveBase.localization.direction), int(driveBase.motors[0]->encoder.getAmount()), int(driveBase.motors[1]->encoder.getAmount()), int(driveBase.motors[2]->encoder.getAmount()), int(driveBase.motors[3]->encoder.getAmount()));});
    //driveBase.goTo(1000,0,0);
    driveBase.runNoEncoder(0,0,0,0.3,5);
    //driveBase.runNoEncoder(0,0,0,0,5);
    while(1){

    }
}

//エンコーダー&自己位置推定．モーターなし
void show(DriveBase& driveBase){
    while(1){
        printf("%d,%d,%d| %d %d %d %d\n",int(driveBase.localization.posX), int(driveBase.localization.posY), int(180/PI*driveBase.localization.direction), int(driveBase.motors[0]->encoder.getAmount()), int(driveBase.motors[1]->encoder.getAmount()), int(driveBase.motors[2]->encoder.getAmount()), int(driveBase.motors[3]->encoder.getAmount()));
        wait_us(100000);
    }
}

//動作テスト
void drive(DriveBase &driveBase){
    //10秒待機
    wait_us(1000000*10);

    //出力を設定
    driveBase.attachLoop([&driveBase](){printf("%d,%d\n", int(driveBase.localization.posX), int(driveBase.localization.posY));});

    //引数: 目標X座標[mm],目標Y座標[mm],目標方角[rad](絶対値はPI未満にする)
    driveBase.goTo(3000, 0, 0);
    driveBase.goTo(3000, 3000, 0);
    driveBase.goTo(0, 0, 0);

    //driveBase.goTo(0, 0, PI/2);
    while(1){

    }
}


Timer timer;

//モーターの位置制御試す用
void move_test(DriveMotor& test_motor){
    test_motor.rotateTo(5000,false); 

    while(test_motor.moving){  
        printf("%d, %d, %d, %d\n", int(test_motor._s2), int(test_motor._s1), 5000, int(test_motor.encoder.getAmount()));
        wait_us(20000);
    }
}


//モーターの速度制御試す用
void speed_test(DriveMotor& test_motor){
    bool flag=false;
    timer.start();
    //test_motor.rotateTo(1000,false); 
    

    test_motor.rotatePermanent(1000,false);

    while(true){
        printf("%d, %d, 0, 0\n", int(test_motor._s2), int(test_motor._s1));
        wait_us(20000);
        if(chrono::duration<float>(timer.elapsed_time()).count() > 5 && !flag){
            flag = true;
            test_motor.rotatePermanent(0,false);
        }

        if(chrono::duration<float>(timer.elapsed_time()).count() > 8){
            break;
            
        }
    }

    test_motor.stop();
}

//一定duty比で動かす
void motor_test(DriveMotor& test_motor){
    timer.start();
    test_motor.setPWM(0.3);
    float l = test_motor.encoder.getAmount();
    while(chrono::duration<float>(timer.elapsed_time()).count() < 3000){
        int v = int((test_motor.encoder.getAmount() - l)*200);
        l = test_motor.encoder.getAmount();
        printf("%d\n", int(v));
        wait_us(5000);
    }
    test_motor.setPWM(0);
}

#if USING_R1

void laser_test(R1* r1){
    r1->driveBase.localization.setPosition(0,0,0);
    r1->lasers.front.activate(EAST, 0);
    r1->lasers.pair.activate(SOUTH, 0);
    //printf("%d\n", r1->lasers.laser2.last_value);
    r1->wait_seconds(3600);
}



void R1::debug(){
    //wait_seconds(10);
    /*
    driveBase.localization.setPosition(0,0,0);
    lasers.front.activate(EAST, 1000);
    driveBase.goTo(1000-ROBOTSIZE-100, 0, 0);
    wait_seconds(3600);
    */
    //laser_test(this);
    dir_check(driveBase);
    //drive(driveBase);
}

#else


void R2::debug(){
    //wait_seconds(10);
    /*
    driveBase.localization.setPosition(0,0,0);
    lasers.front.activate(EAST, 1000);
    driveBase.goTo(1000-ROBOTSIZE-100, 0, 0);
    wait_seconds(3600);
    */
    //laser_test(this);
    dir_check(driveBase);
    //drive(driveBase);
}

#endif