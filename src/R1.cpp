#include "R1.hpp"
#include "parameters.hpp"

#include "tofLaser.hpp"
#include "laserPos.hpp"
#include "laserUse.hpp"

using namespace std::chrono;

#define STR(var) #var



//自動で動かす箇所．この間に手動調整を入れる
enum AUTO_MOVEMENT{
    GOTO_FIRST_PRODUCT_STORAGE, //商品置き場まで移動1
    GOTO_FIRST_CONTAINER_STORAGE, //ボトルを持ち，コンテナ置き場まで移動1
    GOTO_FIRST_DELIVERY, //コンテナに梱包し，受け渡し地点の手前まで移動1
    GOTO_SECOND_PRODUCT_STORAGE, //受け渡しを行い，商品置き場まで移動2
    GOTO_SECOND_CONTAINER_STORAGE, //ボトルを持ち，コンテナ置き場まで移動2
    GOTO_SECOND_DELIVERY, //コンテナに梱包し，受け渡し地点の手前まで移動2
    GOTO_THIRD_PRODUCT_STORAGE, //受け渡しを行い，商品置き場まで移動3
    GOTO_THIRD_CONTAINER_STORAGE, //ボトルを持ち，コンテナ置き場まで移動3
    GO_FRONTOF_POSTZONE, //ポストゾーンの手前まで移動
    GOTO_POSTZONE1, //ボトルを持ち，ポストゾーン1に移動
};

R1::R1(): Robot(), bottleArm([this](int x){wait_seconds(x);}), containerArm([this](int x){wait_seconds(x);})
{
    init_lasers();
}

    


void R1::init_lasers(){
    //レーザー
    TOFLaser laserCore1(1, A0);
    TOFLaser laserCore2(2, A1);
    TOFLaser laserCore3(3, A2);
    TOFLaser laserCore4(4, A3);

    //レーザーの位置
    LaserPos laser1(130,-165,SOUTH,laserCore1);
    LaserPos laser2(-130,-165,SOUTH,laserCore2);
    LaserPos laser3(-165,0,WEST,laserCore3);
    LaserPos laser4(165,-150,WEST,laserCore4);

    //レーザーの使用パターン
    //-----------------------------------------------X座標の最小値-----------------------------------X座標の最大値--------------------------------------Y座標の最小値------------------------------Y座標の最大値----------角度の許容誤差--壁の方角----壁の位置----レーザー位置---レーザー位置---
    LaserPairUse  laserpairuse_on_westwall(     WEST_WALL_X + ROBOTSIZE,                   WEST_WALL_X + ROBOTSIZE + 800,                    0,                                      2400,                           0.5f,      WEST,   WEST_WALL_X,   laser1,     laser2);
    LaserPairUse  laserpairuse_on_northwall(    WEST_WALL_X + ROBOTSIZE + 850 + 38 + 150,  WEST_WALL_X + 850 + 38 + 1200 - ROBOTSIZE - 150,  SOUTH_WALL_Y + 500,                     2400,                           0.1f,      NORTH,  NORTH_WALL_Y,  laser1,     laser2);
    LaserUse      laseruse_on_westwall(         WEST_WALL_X + ROBOTSIZE,                   WEST_WALL_X + ROBOTSIZE + 3000,                   SOUTH_WALL_Y + 550 + ROBOTSIZE,         NORTH_WALL_Y - 550 - ROBOTSIZE, 0.1f,      WEST,   WEST_WALL_X,   laser3);
    LaserUse      laseruse_on_product_storage(  WEST_WALL_X + ROBOTSIZE,                   WEST_WALL_X + 850 - ROBOTSIZE,                    SOUTH_WALL_Y,                           SOUTH_WALL_Y + 500,             0.1f,      SOUTH,  SOUTH_WALL_Y,  laser4);
    LaserUse      laseruse_on_container_storage(WEST_WALL_X + 850 + 38 + ROBOTSIZE,        WEST_WALL_X + 850 + 38 + 1200 - ROBOTSIZE,        SOUTH_WALL_Y,                           SOUTH_WALL_Y + 500,             0.1f,      SOUTH,  SOUTH_WALL_Y,  laser4);
    LaserUse      laseruse_on_postzone1(        WEST_WALL_X + 2126 + ROBOTSIZE,            WEST_WALL_X + 2126 + 500 - ROBOTSIZE,             SOUTH_WALL_Y,                           SOUTH_WALL_Y + 1000,            0.05f,     SOUTH,  SOUTH_WALL_Y,  laser4);

    driveBase.localization.addLocalization([this, &laserpairuse_on_westwall](float* X, float* Y, float* D) { laserpairuse_on_westwall.scan(X, Y, D); }, 0);
    driveBase.localization.addLocalization([this, &laserpairuse_on_northwall](float* X, float* Y, float* D) { laserpairuse_on_northwall.scan(X, Y, D); }, 1);
    driveBase.localization.addLocalization([this, &laseruse_on_westwall](float* X, float* Y, float* D) { laseruse_on_westwall.scan(X, Y, D); }, 2);
    driveBase.localization.addLocalization([this, &laseruse_on_product_storage](float* X, float* Y, float* D) { laseruse_on_product_storage.scan(X, Y, D); }, 3);
    driveBase.localization.addLocalization([this, &laseruse_on_container_storage](float* X, float* Y, float* D) { laseruse_on_container_storage.scan(X, Y, D); }, 4);
    driveBase.localization.addLocalization([this, &laseruse_on_postzone1](float* X, float* Y, float* D) { laseruse_on_postzone1.scan(X, Y, D); }, 5);
}


void R1::game(){
    next();
}

void R1::run(unsigned int movement_id){
    auto_moving = true;
    printf("auto_movement %d\n", movement_id);
    //動線をここに書く．
    switch (movement_id)
    {
    case GOTO_FIRST_PRODUCT_STORAGE:
        driveBase.localization.setPosition(WEST_WALL_X + ROBOTSIZE, NORTH_WALL_Y - ROBOTSIZE, -PI/2); //壁当てでスタート
        bottleArm.down(false);
        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 100, SOUTH_WALL_Y + ROBOTSIZE + 400, -PI/2); //ペットボトル置き場手前
        break;

    case GOTO_FIRST_CONTAINER_STORAGE:
        //ペットボトル起き場まで
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 300, -PI/2);

        //ペットボトル回収
        bottleArm.close(true);
        bottleArm.up(false);

        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 550 + 300, -PI/2);
        driveBase.goTo(WEST_WALL_X+850+38+ROBOTSIZE+100, SOUTH_WALL_Y + ROBOTSIZE + 550 + 300, -PI/2);
        driveBase.goTo(WEST_WALL_X+850+38+ROBOTSIZE+100, SOUTH_WALL_Y + ROBOTSIZE + 450, -PI/2);//コンテナ置き場手前に移動
        break;

    case GOTO_FIRST_DELIVERY:
        
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 300, -PI/2);
        
        containerArm.close(true);
        bottleArm.down(false);
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + 1200, -PI/2);
        driveBase.rotateTo(0);
        bottleArm.open(true);
        driveBase.goTo(WEST_WALL_X + 1900, SOUTH_WALL_Y + 1200, 0);
        break;
    
    case GOTO_SECOND_PRODUCT_STORAGE:
        driveBase.localization.setPosition(driveBase.localization.posX, SOUTH_WALL_Y + 1200, 0);

        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 100, SOUTH_WALL_Y + ROBOTSIZE + 750, -PI/2);
        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 100, SOUTH_WALL_Y + ROBOTSIZE + 300, -PI/2);

        //受け渡し
        //R1から受け渡し検知信号を受信
        wait_seconds(1); 
        containerArm.open(false);

        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 100, SOUTH_WALL_Y + ROBOTSIZE + 550, -PI/2);
        bottleArm.down(false);
        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 100, SOUTH_WALL_Y + ROBOTSIZE + 300, -PI/2);
        break;

    case GOTO_SECOND_CONTAINER_STORAGE:
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 200, -PI/2);

        bottleArm.close(true);
        bottleArm.up(false);

        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 550 + 300, -PI/2);
        driveBase.goTo(WEST_WALL_X+850+38+ROBOTSIZE+100, SOUTH_WALL_Y + ROBOTSIZE + 550 + 300, -PI/2);
        driveBase.goTo(WEST_WALL_X+850+38+ROBOTSIZE+100, SOUTH_WALL_Y + ROBOTSIZE + 450, -PI/2);//コンテナ置き場手前に移動

        break;

    case GOTO_SECOND_DELIVERY:
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 200, -PI/2);
        
        containerArm.close(true);
        bottleArm.down(false);
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + 1200, -PI/2);
        driveBase.rotateTo(0);
        bottleArm.open(true);
        driveBase.goTo(WEST_WALL_X + 1900, SOUTH_WALL_Y + 1200, 0);
        break;

    case GOTO_THIRD_PRODUCT_STORAGE:
        driveBase.localization.setPosition(driveBase.localization.posX, SOUTH_WALL_Y + 1200, 0);

        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 100, SOUTH_WALL_Y + ROBOTSIZE + 750, -PI/2);
        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 100, SOUTH_WALL_Y + ROBOTSIZE + 300, -PI/2);

        //受け渡し
        //R1から受け渡し検知信号を受信
        wait_seconds(1); 
        containerArm.open(false);

        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 100, SOUTH_WALL_Y + ROBOTSIZE + 550, -PI/2);
        bottleArm.down(false);
        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 100, SOUTH_WALL_Y + ROBOTSIZE + 200, -PI/2);
        break;

    case GOTO_THIRD_CONTAINER_STORAGE:
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 100, -PI/2);
        
        //ペットボトル回収
        bottleArm.close(true);
        bottleArm.up(false);

        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 550 + 300, -PI/2);
        driveBase.goTo(WEST_WALL_X+850+38+ROBOTSIZE+700, SOUTH_WALL_Y + ROBOTSIZE + 550 + 300, -PI/2);
        driveBase.goTo(WEST_WALL_X+850+38+ROBOTSIZE+700, SOUTH_WALL_Y + ROBOTSIZE + 450, -PI/2);//コンテナ置き場手前に移動
        break;

    case GO_FRONTOF_POSTZONE:
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 200, -PI/2);//コンテナ置き場手前に移動
        containerArm.close(true);
        bottleArm.down(false);
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + 1200, -PI/2);
        bottleArm.open(true);
        driveBase.goTo(WEST_WALL_X+2200, SOUTH_WALL_Y + ROBOTSIZE + 150, -PI/2);

        
        break;

    case GOTO_POSTZONE1:
        driveBase.localization.setPosition(WEST_WALL_X+2126+250, driveBase.localization.posY, -PI/2);
        driveBase.goTo(WEST_WALL_X+2126+250, SOUTH_WALL_Y + ROBOTSIZE + 200, -PI/2);
        break;
        
    
    default:
        printf("pass");
        break;
    }

    //デバッグ用
    wait_seconds(3);


    auto_moving = false;
}


void R1::next(){
    if(mm.mode == HAND_MODE){
        mm.sendMessageToController(MESSAGE_TO_CONTROLLER::START_HAND_MODE);
        moveHand();
    }else if(mm.mode == AUTO_MODE && !mm.flag){
        mm.sendMessageToController(MESSAGE_TO_CONTROLLER::START_ADJUSTMENT_MODE);
        adjustment();
    }else if((mm.mode == AUTO_MODE && mm.flag) || mm.mode == COMPLETELY_AUTO_MODE){
        //自動
        mm.sendMessageToController(MESSAGE_TO_CONTROLLER::START_AUTO_MOVE_MODE);
        run(mm.movement_id);
        if(mm.mode != HAND_MODE){
            mm.movement_id++;
            mm.flag = false;
        }
    }
    next();
}

void R1::moveHand(){
    driveBase.stopMovement();
    driveBase.goPtrStart(&mm.targetSpeedx, &mm.targetSpeedy, &mm.targetSpeedD, false, false);
    while(mm.mode == HAND_MODE){
        mm.setTargetSpeed();
        wait_ns(1);
        loop();
        printf("\rhand_mode target_speed: %d %d %d          ", int(mm.targetSpeedx), int(mm.targetSpeedy), int(180/PI*mm.targetSpeedD));
        fflush(stdout);
    }
    printf("\n");
}

void R1::adjustment(){
    driveBase.stopMovement();
    driveBase.goPtrStart(&mm.targetSpeedx, &mm.targetSpeedy, &mm.targetSpeedD, false, false);
    while(mm.mode == AUTO_MODE && !mm.flag){
        mm.setTargetSpeed();
        wait_ns(1);
        loop();
        printf("\radjustment target_speed: %d %d %d          ", int(mm.targetSpeedx), int(mm.targetSpeedy), int(180/PI*mm.targetSpeedD));
        fflush(stdout);
    }
    printf("\n");
}

void R1::loop(){
    if(duration_cast<milliseconds>(timer.elapsed_time()).count() > next_localization_time){
        driveBase.localization.loop();
        next_localization_time += 1000/SPEED_ADJUSTMENT_FREQUENCY;
    }

    if(mm.mode == HAND_MODE && auto_moving){
        driveBase.stopMovement();
    }

    if(mm.ConnectionTimeOutOccured()){
        mm.mode = COMPLETELY_AUTO_MODE;
    }
}

void R1::wait_seconds(float seconds){
    int started = duration_cast<microseconds>(timer.elapsed_time()).count();
    while(duration_cast<microseconds>(timer.elapsed_time()).count() - started < 1000000*seconds){
        loop();
        wait_ns(1);
        if(mm.mode == HAND_MODE && auto_moving){break;};
    }
}
