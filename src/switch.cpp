#include "switch.hpp"
#include <chrono>

Switch::Switch(PinName pin, unsigned int interval, unsigned int detectionThreshold): interruptin(pin), interval(interval), detectionThreshold(detectionThreshold)
{
    monitoring = false;
    status = interruptin.read();
    interruptin.rise([this] {riseInterrupt();});
    interruptin.fall([this] {riseInterrupt();});
}

//押されてるか否か
bool Switch::get(){
    return status;
}

//立ち上がり割り込み
void Switch::riseInterrupt(){
    if(monitoring || status){
        return;
    }
    monitoring = true;
    ticker.attach([this] {riseCheck();}, std::chrono::milliseconds(interval));
}

//立ち下がり割り込み
void Switch::fallInterrupt(){
    if(monitoring || !status){
        return;
    }
    monitoring = true;
    ticker.attach([this] {fallCheck();}, std::chrono::milliseconds(interval));
}

//サンプリング．チャタリング対策
void Switch::riseCheck(){
    if(interruptin.read()){
        counter++;
        if(counter >= detectionThreshold){
            status = true;
            ticker.detach();
            monitoring = false;
        }
    }else{
        counter=0;
        ticker.detach();
        monitoring = false;
    }
}

//サンプリング．チャタリング対策
void Switch::fallCheck(){
    if(!interruptin.read()){
        counter++;
        if(counter >= detectionThreshold){
            status = false;
            ticker.detach();
            monitoring = false;
        }
    }else{
        counter=0;
        ticker.detach();
        monitoring = false;
    }
}

