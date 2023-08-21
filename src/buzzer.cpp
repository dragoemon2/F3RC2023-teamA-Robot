#include "buzzer.hpp"
#include <mbed.h>
#include <chrono>

//初期化
Buzzer::Buzzer(PinName pin): digitalout(pin)
{
    stop();
}

//警告音
void Buzzer::alart(Error error){
    std::vector<bool> list;
    unsigned int repeat;
    getQueue(error, &list, &repeat);
    morse(list, 100, repeat);
}

//エラーメッセージから信号を取得
void Buzzer::getQueue(Error error, std::vector<bool>* list, unsigned int* repeat){
    switch (error)
    {
        case Error::UNPREDICTED:
            *list = {0,0,1};//U
            *repeat = 4;
            break;

        case Error::CONTROLLER:
            *list = {1,0,1,0};//C
            *repeat = 3;
            break;
        
        case Error::MEMORY:
            *list = {1,1};//M
            *repeat = 5;
            break;

        case Error::I2CLASER:
            *list = {0,1,0,0};//L
            *repeat = 2;
            break;
    
        default:
            *list = {0,0,1};//U
            *repeat = 4;
            break;
    }
}

void Buzzer::beep(unsigned int time){
    message({1}, time);
}

//止める
void Buzzer::stop(){
    ticker.detach();
    array = {};
    repeat_num = 0;
    digitalout.write(0);
    _s1 = 0;
}

//ツーとトンの信号
void Buzzer::morse(std::vector<bool> list, unsigned int internal, unsigned int repeat){
    std::vector<bool> newlist={};
    for(bool b: list){
        for(int i=0;i<(b ? 3:1);i++){
            newlist.push_back(1);
        }
        newlist.push_back(0);
    }
    message(newlist, internal, repeat);
}

//HIGHかLOWかの信号
void Buzzer::message(std::vector<bool> list, unsigned int internal, unsigned int repeat){
    stop();
    index = 0;
    array = list;
    repeat_num = repeat;
    ticker.attach([this] {next();}, std::chrono::milliseconds(internal));
}

//タイマー割り込みで実行
void Buzzer::next(){
    if(index >= int(array.size())){
        if(repeat_num <= 0){
            stop();
        }else{
            repeat_num--;
            index = -10;
        }
    }else if(index >= 0){
        digitalout.write(array[index]);
        _s1 = array[index];
    }else{
        digitalout.write(0);
    }
    index++;
}


