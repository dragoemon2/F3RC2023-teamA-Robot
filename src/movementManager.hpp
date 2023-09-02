#pragma once

#include "serialCommunication.hpp"
#include <mbed.h>

enum MESSAGE_TO_CONTROLLER{
    START_AUTO_MOVE_MODE,
    START_ADJUSTMENT_MODE,
    START_HAND_MODE,
    ERROR_RAISED,
};

/*
手動機R1の動きを決定するためのクラス

R1の動きをpublicメンバ変数にして(またはprivateにしてpublicメンバ関数でそれを読むのでも良い)
それをmainloopの中で読むことでR1が動く

受信があった時にupdate();が割り込みで実行されるので，その引数string strをもとにメンバ変数を書き換える
*/

using namespace std;

#define HAND_MODE (0)
#define AUTO_MODE (1)
#define COMPLETELY_AUTO_MODE (2)

#define USING_CONTROLLER (0)

class MovementManager{
    private:
        //正規化したジョイスティックの値
        float ljoystick_x = 0;
        float ljoystick_y = 0;

        float rjoystick_x = 0;
        float rjoystick_y = 0;

        bool left_rotate_button = false;
        bool right_rotate_button = false;

        bool last_next_button = false;
        bool last_increment_button = false;
        bool last_decrement_button = false;

        bool last_hand_mode_button = false;
        bool last_auto_mode_button = false;

        int last_connected_time;
        bool connected_before = false;

        Timer timer;


    public:
        #if USING_CONTROLLER
        SerialCommunication serial;
        #endif

        //ロボットがするべき動きを決めたメンバ変数を作って．以下は一例

        //動作番号
        int movement_id = 0;

        //自動モードか手動モードか
        int mode = AUTO_MODE;

        //ジョイスティックやボタンの値から検出した目標の速度
        float targetSpeedx = 0;
        float targetSpeedy = 0;
        float targetSpeedD = 0;

        //動作モードであることを示すflag
        //trueなら動作モード，falseなら調整モード
        bool flag = false;

        //受信があると実行される
        void update(char* str);
        void setTargetSpeed();
        void sendMessageToController(int message);
        bool MotionTimeOutOccured();
        bool ConnectionTimeOutOccured();

        MovementManager();
};