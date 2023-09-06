#include "parameters.hpp"
#include "pins.hpp"

#if USING_R1

#include "movementManager.hpp"
#include <string>

#include <chrono>



#define MOTION_TIME_OUT (500)
#define CONNECTION_TIME_OUT (5000)

/*
#define ADJUSTMENT_SPEED_LEFT (100.0f)
#define ADJUSTMENT_SPEED_RIGHT (30.0f)
#define ADJUSTMENT_ROTATE_SPEED (0.5f)

#define HAND_SPEED_LEFT (1500.0f)
#define HAND_SPEED_RIGHT (100.0f)
#define HAND_ROTATE_SPEED (0.5f)
*/

using namespace std::chrono;


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
MovementManager::MovementManager()
#if USING_CONTROLLER
    :serial(115200, ESP_UART_TX, ESP_UART_RX)
#endif
{
    flag = false; //flagを下げておく
    #if USING_CONTROLLER
    serial.attach([this](char* str) {update(str);});
    #else
    mode = COMPLETELY_AUTO_MODE;
    #endif

    #if !USE_AUTO_MODE
    mode = HAND_MODE;
    #endif
    timer.start();
}

void MovementManager::update(char* str){
    _s1 =1;
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
    left_rotate_button = (button_mask >> Button::L2) & 1;
    right_rotate_button = (button_mask >> Button::R2) & 1;

    //次の実行
    if(((button_mask >> Button::CIRCLE) & 1) && !last_next_button){
        #if USE_AUTO_MODE
        if(mode == HAND_MODE){
            flag = false;
            mode = AUTO_MODE;
        }else{
            flag = true;
        }
        #endif
    }
    last_next_button = (button_mask >> Button::CIRCLE) & 1;

    if(((button_mask >> Button::LEFT) & 1) && !last_decrement_button){
        if(!flag){
            if(movement_id > 0){
                movement_id--;
            }
        }
    }
    last_decrement_button = (button_mask >> Button::LEFT) & 1;


    if(((button_mask >> Button::RIGHT) & 1) && !last_increment_button){
        if(!flag){
            movement_id++;
        }
    }
    last_increment_button = (button_mask >> Button::RIGHT) & 1;

    /*


    if(((button_mask >> Button::TRIANGLE) & 1) && !last_hand_mode_button){
        mode = HAND_MODE;
    }
    last_hand_mode_button = (button_mask >> Button::TRIANGLE) & 1;
    */

    if(((button_mask >> Button::TRIANGLE) & 1) && !last_updown_button){
        updown_flag = true;
    }
    last_updown_button = (button_mask >> Button::TRIANGLE) & 1;


    if(((button_mask >> Button::CROSS) & 1) && !last_container_button){
        //mode = HAND_MODE;
        container_arm_flag = true;
    }
    last_container_button = (button_mask >> Button::CROSS) & 1;






    


    connected_before = true;
    last_connected_time = duration_cast<milliseconds>(timer.elapsed_time()).count();
}



void MovementManager::setTargetSpeed(){
    if(MotionTimeOutOccured()){
        targetSpeedx = 0;
        targetSpeedy = 0;
        targetSpeedD = 0;
        return;
    }

    if(mode == HAND_MODE){
        //手動モード中
        if(left_rotate_button && !right_rotate_button){
            targetSpeedD = HAND_ROTATE_SPEED;
        }else if(!left_rotate_button && right_rotate_button){
            targetSpeedD = -HAND_ROTATE_SPEED;
        }else{
            targetSpeedD = 0;
        }
        targetSpeedy = - joystick_filter(ljoystick_x)*HAND_SPEED_LEFT - joystick_filter(rjoystick_x)*HAND_SPEED_RIGHT;
        targetSpeedx = - joystick_filter(ljoystick_y)*HAND_SPEED_LEFT - joystick_filter(rjoystick_y)*HAND_SPEED_RIGHT;


    }else if(mode == AUTO_MODE && !flag){
        //調整中
        if(left_rotate_button && !right_rotate_button){
            targetSpeedD = ADJUSTMENT_ROTATE_SPEED;
        }else if(!left_rotate_button && right_rotate_button){
            targetSpeedD = -ADJUSTMENT_ROTATE_SPEED;
        }else{
            targetSpeedD = 0;
        }

        targetSpeedy = - joystick_filter(ljoystick_x)*ADJUSTMENT_SPEED_LEFT - joystick_filter(rjoystick_x)*ADJUSTMENT_SPEED_RIGHT;
        targetSpeedx = - joystick_filter(ljoystick_y)*ADJUSTMENT_SPEED_LEFT - joystick_filter(rjoystick_y)*ADJUSTMENT_SPEED_RIGHT;
    }
}


void MovementManager::sendMessageToController(int message){
    #if USING_CONTROLLER
    serial.writeline(to_string(message));
    #endif
}

bool MovementManager::MotionTimeOutOccured(){
    return (connected_before && duration_cast<milliseconds>(timer.elapsed_time()).count() - last_connected_time > MOTION_TIME_OUT);
}

bool MovementManager::ConnectionTimeOutOccured(){
    return (connected_before && duration_cast<milliseconds>(timer.elapsed_time()).count() - last_connected_time > CONNECTION_TIME_OUT);
}

bool  MovementManager::containerArmMove(){
    //return (connected_before && duration_cast<milliseconds>(timer.elapsed_time()).count() - last_connected_time > CONNECTION_TIME_OUT);
    if(container_arm_flag){
        container_arm_flag = false;
        return true;
    }else{
        return false;
    }
}

bool  MovementManager::upDownArmMove(){
    //return (connected_before && duration_cast<milliseconds>(timer.elapsed_time()).count() - last_connected_time > CONNECTION_TIME_OUT);
    if(updown_flag){
        updown_flag = false;
        return true;
    }else{
        return false;
    }
}



#endif