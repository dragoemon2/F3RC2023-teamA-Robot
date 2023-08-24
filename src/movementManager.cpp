#include "movementManager.hpp"

#define ADJUSTMENT_SPEED_LEFT (100.0f)
#define ADJUSTMENT_SPEED_RIGHT (10.0f)
#define ADJUSTMENT_ROTATE_SPEED (0.5f)

#define HAND_SPEED_LEFT (1500.0f)
#define HAND_SPEED_RIGHT (100.0f)
#define HAND_ROTATE_SPEED (0.5f)

enum Button{
    UP,
    DOWN,
    RIGHT,
    LEFT,
    CROSS,
    CIRCLE,
    RECTANGLE,
    TRIANGLE,
    L1,
    R1,
    L2,
    R2,
    LJ,
    RJ,
};

//なんかいい感じのフィルタ
inline float joystick_filter(float value, float dead=0.3f){
    if(-dead < value && value < dead){
        return 0;
    }else{
        return value;
    }
}




//初期化
MovementManager::MovementManager(){
    flag = false; //flagを下げておく
    serial.attach([this](char* str) {update(str);});
}

void MovementManager::update(char* str){
    int lx;
    int ly;
    int rx;
    int ry;
    unsigned int button_mask;
    int N = sscanf(str, "%d %d %d %d %d", &lx, &ly, &rx, &ry, &button_mask);
    if(N!=5){
        return;
    }

    //ジョイスティック
    ljoystick_x = (float(lx))/128;
    ljoystick_y = (float(ly))/128;
    rjoystick_x = (float(rx))/128;
    rjoystick_y = (float(ry))/128;

    //回転
    left_rotate_button = (button_mask << Button::L2) & 1;
    right_rotate_button = (button_mask << Button::R2) & 1;

    //次の実行
    if(((button_mask << Button::CIRCLE) & 1) && !last_next_button){
        if(mode == HAND_MODE){
            mode = AUTO_MODE;
        }else{
            flag = true;
        }
    }
    last_next_button = (button_mask << Button::CIRCLE) & 1;

    if(((button_mask << Button::L1) & 1) && !last_decrement_button){
        if(!flag){
            movement_id++;
        }
    }
    last_decrement_button = (button_mask << Button::L1) & 1;


    if(((button_mask << Button::L1) & 1) && !last_decrement_button){
        if(!flag){
            movement_id--;
        }
    }
    last_increment_button = (button_mask << Button::L1) & 1;


    if(((button_mask << Button::TRIANGLE) & 1) && !last_hand_mode_button){
        mode = HAND_MODE;
    }
    last_hand_mode_button = (button_mask << Button::TRIANGLE) & 1;
}



void MovementManager::setTargetSpeed(){
    if(mode == HAND_MODE){
        //手動モード中
        if(left_rotate_button && !right_rotate_button){
            targetSpeedD = HAND_ROTATE_SPEED;
        }else if(!left_rotate_button && right_rotate_button){
            targetSpeedD = -HAND_ROTATE_SPEED;
        }

        targetSpeedx = joystick_filter(ljoystick_x)*HAND_SPEED_LEFT + joystick_filter(ljoystick_x)*HAND_SPEED_RIGHT;
        targetSpeedy = joystick_filter(ljoystick_y)*HAND_SPEED_LEFT + joystick_filter(ljoystick_y)*HAND_SPEED_RIGHT;

    }else if(mode == AUTO_MODE && !flag){
        //調整中
        if(left_rotate_button && !right_rotate_button){
            targetSpeedD = ADJUSTMENT_ROTATE_SPEED;
        }else if(!left_rotate_button && right_rotate_button){
            targetSpeedD = -ADJUSTMENT_ROTATE_SPEED;
        }

        targetSpeedx = joystick_filter(ljoystick_x)*ADJUSTMENT_SPEED_LEFT + joystick_filter(ljoystick_x)*ADJUSTMENT_SPEED_RIGHT;
        targetSpeedy = joystick_filter(ljoystick_y)*ADJUSTMENT_SPEED_LEFT + joystick_filter(ljoystick_y)*ADJUSTMENT_SPEED_RIGHT;
    }
}
