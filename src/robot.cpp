#include "robot.hpp"
#include "laserPos.hpp"

union FloatUIntUnion {
    float floatValue;
    unsigned int uintValue;
};

Robot::Robot()
 :  motor0(A0, A1, D10, D11, MOTOR_0_KP_1, MOTOR_0_KI_1, MOTOR_0_KD_1, MOTOR_0_KP_2, MOTOR_0_KI_2, MOTOR_0_KD_2),
    motor1(A2, A3, D4, D5, MOTOR_1_KP_1, MOTOR_1_KI_1, MOTOR_1_KD_1, MOTOR_1_KP_2, MOTOR_1_KI_2, MOTOR_1_KD_2),
    motor2(A4, A5, D6, D7, MOTOR_2_KP_1, MOTOR_2_KI_1, MOTOR_2_KD_1, MOTOR_2_KP_2, MOTOR_2_KI_2, MOTOR_2_KD_2),
    motor3(D2, D3, D8, D9, MOTOR_3_KP_1, MOTOR_3_KI_1, MOTOR_3_KD_1, MOTOR_3_KP_2, MOTOR_3_KI_2, MOTOR_3_KD_2),
    driveBase(&motor0, &motor1, &motor2, &motor3),
    buzzer(PA_0)
{
    timer.start();
    init_driveBase();
    init_lasers();
    init_arms();
    init_status();
}

//試合
void Robot::game(){

}

void Robot::run0(){
    //開始まで
}

void Robot::run1(){

}

//モーターの初期化
void Robot::init_driveBase(){
    driveBase.attachLoop([this]{loop();});
}

//レーザーの初期化
void Robot::init_lasers(){

    //DT35
    Laser laserCore1(PC_1);
    Laser laserCore2(PC_0);

    //VL53L0X
    TOFLaser laserCore3(0, PA_13, PB_3, PB_10);
    TOFLaser laserCore4(1, PA_13, PB_3, PB_10);

    //----ロボット上でのレーザーの位置の指定----

    LaserPos laser1(
        100, //DT35レーザー位置のx座標
        -150, //DT35レーザー位置のy座標
        SOUTH,
        laserCore1
    );

    LaserPos laser2(
        -100, //DT35レーザー位置のx座標
        -150, //DT35レーザー位置のy座標
        SOUTH,
        laserCore2
    );

    LaserPos laser3(
        -150, //VL53L0Xレーザー位置のx座標
        -100, //VL53L0Xレーザー位置のy座標
        WEST, //ロボットが東を向いた時の向き
        laserCore3
    );

    LaserPos laser4(
        -150, //VL53L0Xレーザー位置のx座標
        100, //VL53L0Xレーザー位置のy座標
        WEST, //ロボットが東を向いた時の向き
        laserCore4
    );


    //レーザー2つ
    LaserPairUse lu1(
        1000, //X座標の最小値
        2000, //X座標の最大値
        0, //Y座標の最小値
        2500, //Y座標の最大値
        PI/8, //方向のずれの範囲
        SOUTH, //壁の方角
        38, //原点から壁の距離
        laser1, //レーザー
        laser2 //レーザー
    );

    //レーザー1つ
    LaserUse lu2(
        1000, //X座標の最小値
        2000, //X座標の最大値
        0, //Y座標の最小値
        500, //Y座標の最大値
        PI/8, //方向のずれの範囲
        WEST, //壁の方角
        2000, //原点から壁の距離
        laser3 //レーザー
    );

    laserPairUses.push_back(lu1);
    laserUses.push_back(lu2);

    for(LaserUse lu: laserUses){
        driveBase.localization.addLocalization([this, &lu](float* X, float* Y, float* D) { lu.scan(X, Y, D); }, 0);
    }

    for(LaserPairUse lu: laserPairUses){
        driveBase.localization.addLocalization([this, &lu](float* X, float* Y, float* D) { lu.scan(X, Y, D); }, 1);
    }
}

void Robot::init_arms(){

}

void Robot::init_communication(){
    
}

void Robot::init_status(){
    backUpRegister.open(0);
    int errCode = backUpRegister.get(0,4);
    if(errCode){
        //前回エラーが起きたので復元

        //動作番号の復元

        //自己位置の復元
        backUpRegister.open(1);
        float X = float(backUpRegister.get(0,16));
        float Y = float(backUpRegister.get(16,16));
        FloatUIntUnion directionUnion;
        backUpRegister.open(2);   
        directionUnion.uintValue = backUpRegister.get(0,32);
        float D = directionUnion.floatValue;
        driveBase.localization.setPosition(X, Y, D);

        //警告音の再生
        buzzer.alart(Error::UNPREDICTED);
    }else{
        driveBase.localization.setPosition(0, 0, 0);
    }


    //正常と記録しておく
    backUpRegister.open(0);
    backUpRegister.set(0,4,0);
    backUpRegister.save();

}

void Robot::reboot(){
    //エラーコードなど
    backUpRegister.open(0);
    backUpRegister.set(0,4,1);
    //backUpRegister.set(4,6,動作番号);
    //backUpRegister.set(10,6,動作段階);
    backUpRegister.save();

    backUpRegister.open(1);
    backUpRegister.set(0,16,short(driveBase.localization.posX));
    backUpRegister.set(16,16,short(driveBase.localization.posY));
    backUpRegister.save();

    FloatUIntUnion directionUnion;
    directionUnion.floatValue = driveBase.localization.direction;
    backUpRegister.open(2);
    backUpRegister.set(0,32,reinterpret_cast<unsigned int>(directionUnion.uintValue));
    backUpRegister.save();

    //プログラムを再スタート
    NVIC_SystemReset();
}

void Robot::loop(){
    driveBase.localization.loop();
}