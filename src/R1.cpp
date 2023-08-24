#include "R1.hpp"

using namespace std::chrono;


//自動で動かす箇所．この間に手動調整を入れる
enum AUTO_MOVEMENT{
    WAIT_FOR_LAUNCH, //試合開始まで待機
    GOTO_FIRST_PRODUCT_STORAGE, //商品置き場まで移動1
    GOTO_FIRST_CONTAINER_STORAGE, //ボトルを持ち，コンテナ置き場まで移動1
    GOTO_FIRST_DELIVERY, //コンテナに梱包し，受け渡し地点の手前まで移動1
    GOTO_SECOND_PRODUCT_STORAGE, //受け渡しを行い，商品置き場まで移動2
    GOTO_SECOND_CONTAINER_STORAGE, //ボトルを持ち，コンテナ置き場まで移動2
    GOTO_SECOND_DELIVERY, //コンテナに梱包し，受け渡し地点の手前まで移動2
    GOTO_THIRD_PRODUCT_STORAGE, //受け渡しを行い，商品置き場まで移動3
    GOTO_THIRD_CONTAINER_STORAGE, //ボトルを持ち，コンテナ置き場まで移動3
    GOTO_POSTZONE1, //ボトルを持ち，ポストゾーン1に移動
};

R1::R1(): Robot()
{
    next();
}

void R1::run(unsigned int movement_id){
    auto_moving = true;
    switch (movement_id)
    {
    case WAIT_FOR_LAUNCH:
        /* code */
        //特になし
        if(mm.mode==HAND_MODE){break;}; //これを各動作の間に挟まなきゃいけない
        break;

    case GOTO_FIRST_PRODUCT_STORAGE:
        /* code */
        break;

    case GOTO_FIRST_CONTAINER_STORAGE:
        /* code */
        break;

    case GOTO_FIRST_DELIVERY:
        /* code */
        break;
    
    case GOTO_SECOND_PRODUCT_STORAGE:
        /* code */
        break;

    case GOTO_SECOND_CONTAINER_STORAGE:
        /* code */
        break;

    case GOTO_SECOND_DELIVERY:
        /* code */
        break;

    case GOTO_THIRD_PRODUCT_STORAGE:
        /* code */
        break;

    case GOTO_THIRD_CONTAINER_STORAGE:
        /* code */
        break;

    case GOTO_POSTZONE1:
        /* code */
        break;
    
    default:
        break;
    }
    auto_moving = false;
}


void R1::next(){
    if(mm.mode == HAND_MODE || (mm.mode == AUTO_MODE && !mm.flag)){
        //手動
        moveHand();
    }else if((mm.mode == AUTO_MODE && mm.flag) || mm.mode == COMPLETELY_AUTO_MODE){
        //自動
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
    while((mm.mode == AUTO_MODE && !mm.flag) || mm.mode == HAND_MODE){
        wait_ns(1);
        loop();
    }
}

void R1::loop(){
    if(duration_cast<milliseconds>(timer.elapsed_time()).count() > next_localization_time){
        driveBase.localization.loop();
        next_localization_time += 1000/SPEED_ADJUSTMENT_FREQUENCY;
    }

    if(mm.mode == HAND_MODE && auto_moving){
        driveBase.stopMovement();
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
