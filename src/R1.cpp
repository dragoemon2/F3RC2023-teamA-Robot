#include "R1.hpp"
#include "parameters.hpp"

#include "simulation.hpp"

#if USING_R1

using namespace std::chrono;

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

#if 0
R1::R1(): Robot(), bottleArm([this](int x){wait_seconds(x);}), containerArm([this](int x){wait_seconds(x);}), lasers(driveBase)
{
    
}
#else
R1::R1(): Robot(), containerArm([this](int x){wait_seconds(x);}),lasers(driveBase), bottleArm([this](int x){wait_seconds(x);})
{
    driveBase.attachLoop([this](){loop();});

    #if SIMULATION
    new MotorSimulation(&motor0);
    new MotorSimulation(&motor1);
    new MotorSimulation(&motor2);
    new MotorSimulation(&motor3);
    #endif
}
#endif

void R1::game(){
    #if !USING_CONTROLLER
    //コントローラー接続タイムアウトが発生するようにする．
    mm.connected_before = true;
    #endif

    
    mm.mode = HAND_MODE;

    next();
}


void R1::run(unsigned int movement_id){
    auto_moving = true;
    printf("auto_movement %d\n", movement_id);
    //動線をここに書く．

    switch (movement_id)
    {
    //ペットボトル置き場手前まで移動する
    case GOTO_FIRST_PRODUCT_STORAGE:
        driveBase.localization.setPosition(WEST_WALL_X + ROBOTSIZE, NORTH_WALL_Y - ROBOTSIZE, -PI/2); //壁当てでスタート
        //lasers.pair.activate(WEST, -WEST_WALL_X);
        //lasers.front.activate(SOUTH, -SOUTH_WALL_Y);
        bottleArm.down(false);
        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 200, SOUTH_WALL_Y + ROBOTSIZE + 400, -PI/2); //ペットボトル置き場手前

        break;

    case GOTO_FIRST_CONTAINER_STORAGE:
        //ペットボトル起き場まで
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 300, -PI/2);

        //ペットボトル回収
        bottleArm.close(true);
        bottleArm.up(false);

        lasers.front.inactivate();
        lasers.pair.inactivate();

        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 550 + 300, -PI/2);
        driveBase.goTo(WEST_WALL_X+850+38+ROBOTSIZE+100, SOUTH_WALL_Y + ROBOTSIZE + 550 + 300, -PI/2);
        //lasers.front.activate(SOUTH, SOUTH_WALL_Y);
        driveBase.goTo(WEST_WALL_X+850+38+ROBOTSIZE+100, SOUTH_WALL_Y + ROBOTSIZE + 500, -PI/2);//コンテナ置き場手前に移動
        break;

    case GOTO_FIRST_DELIVERY:
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 400, -PI/2);
        
        containerArm.close(true);
        bottleArm.down(false);
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + 1200, -PI/2);
        driveBase.rotateTo(0);
        //bottleArm.open(true);

        driveBase.goTo(WEST_WALL_X + 1900, SOUTH_WALL_Y + 1200, 0);

        break;
    
    case GOTO_SECOND_PRODUCT_STORAGE:
        if(mm.mode != COMPLETELY_AUTO_MODE){
            driveBase.localization.setPosition(driveBase.localization.posX, SOUTH_WALL_Y + 1200, 0);
        }
        

        driveBase.goTo(WEST_WALL_X + 2000, SOUTH_WALL_Y + 1200, -PI/2);

        //受け渡し
        //R1から受け渡し検知信号を受信
        wait_seconds(1); 
        containerArm.open(false);

        driveBase.rotateTo(-PI/2);

        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 200, SOUTH_WALL_Y + ROBOTSIZE + 750, -PI/2);
        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 200, SOUTH_WALL_Y + ROBOTSIZE + 400, -PI/2);

        #if !GO_POSTZONE3
        mm.movement_id = GOTO_THIRD_CONTAINER_STORAGE - 1;
        #endif

        break;

    case GOTO_SECOND_CONTAINER_STORAGE:
        
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 200, -PI/2);

        bottleArm.close(true);
        bottleArm.up(false);

        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 550 + 300, -PI/2);
        driveBase.goTo(WEST_WALL_X+850+38+ROBOTSIZE+100, SOUTH_WALL_Y + ROBOTSIZE + 550 + 300, -PI/2);
        driveBase.goTo(WEST_WALL_X+850+38+ROBOTSIZE+100, SOUTH_WALL_Y + ROBOTSIZE + 500, -PI/2);//コンテナ置き場手前に移動

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
        if(mm.mode != COMPLETELY_AUTO_MODE){
            driveBase.localization.setPosition(driveBase.localization.posX, SOUTH_WALL_Y + 1200, 0);
        }
        

        driveBase.goTo(WEST_WALL_X + 2000, SOUTH_WALL_Y + 1200, -PI/2);

        //受け渡し
        //R1から受け渡し検知信号を受信
        wait_seconds(1); 
        //containerArm.open(false);

        driveBase.rotateTo(-PI/2);

        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 200, SOUTH_WALL_Y + ROBOTSIZE + 750, -PI/2);
        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 200, SOUTH_WALL_Y + ROBOTSIZE + 400, -PI/2);

        break;

    case GOTO_THIRD_CONTAINER_STORAGE:
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 100, -PI/2);
        
        //ペットボトル回収
        bottleArm.close(true);
        bottleArm.up(false);

        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 550 + 300, -PI/2);
        driveBase.goTo(WEST_WALL_X+850+38+ROBOTSIZE+700, SOUTH_WALL_Y + ROBOTSIZE + 550 + 300, -PI/2);
        driveBase.goTo(WEST_WALL_X+850+38+ROBOTSIZE+700, SOUTH_WALL_Y + ROBOTSIZE + 500, -PI/2);//コンテナ置き場手前に移動
        break;

    case GO_FRONTOF_POSTZONE:
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 200, -PI/2);//コンテナ置き場手前に移動
        containerArm.close(true);
        bottleArm.down(false);
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + 1200, -PI/2);
        bottleArm.open(true);
        driveBase.goTo(WEST_WALL_X+2200, SOUTH_WALL_Y + 1200, -PI/2);

        
        break;

    case GOTO_POSTZONE1:
        if(mm.mode != COMPLETELY_AUTO_MODE){
            driveBase.localization.setPosition(WEST_WALL_X+ROBOTSIZE+2126+250, driveBase.localization.posY, -PI/2);
        }else{
            driveBase.goTo(WEST_WALL_X+ROBOTSIZE+2126+250, SOUTH_WALL_Y + ROBOTSIZE + 150, -PI/2);
        }
        
        driveBase.goTo(WEST_WALL_X+ROBOTSIZE+2126+250, SOUTH_WALL_Y + ROBOTSIZE + 200, -PI/2);
        break;
        
    
    default:
        //printf("pass");
        break;
    }



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
        if(mm.containerArmMove()){
            if(containerArm.opened){
                containerArm.close(false);
                printf("close\n");
            }else{
                containerArm.open(false);
                printf("open\n");
            }
        }
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
        //mm.mode = COMPLETELY_AUTO_MODE;
    }
    //printf("%d\n", mm._s1);
    //printf("%d,%d,%d| %d %d %d %d\n",int(driveBase.localization.posX), int(driveBase.localization.posY), int(180/PI*driveBase.localization.direction), int(motor0.encoder.getAmount()), int(motor1.encoder.getAmount()), int(motor2.encoder.getAmount()), int(motor3.encoder.getAmount()));
}

void R1::wait_seconds(float seconds){
    int started = duration_cast<microseconds>(timer.elapsed_time()).count();
    while(duration_cast<microseconds>(timer.elapsed_time()).count() - started < 1000000*seconds){
        loop();
        wait_ns(1);
        if(mm.mode == HAND_MODE && auto_moving){break;};
    }
}

#endif