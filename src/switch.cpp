#include "switch.hpp"
#include <chrono>

Switch::Switch(PinName pin, bool high_on_pushed, unsigned int interval, unsigned int detectionThreshold): interruptin(pin), high_on_pushed(high_on_pushed), interval(interval), detectionThreshold(detectionThreshold)
{
    
    monitoring = false;
    status = interruptin.read();
    riseFunc = []{return;};
    fallFunc = []{return;};
    if(high_on_pushed){
        interruptin.rise([this] {riseInterrupt();});
        interruptin.fall([this] {fallInterrupt();});
    }else{
        interruptin.fall([this] {riseInterrupt();});
        interruptin.rise([this] {fallInterrupt();});
    }
}

void Switch::riseAttachOnce(std::function<void(void)> f){
    riseFunc = f;
}

void Switch::fallAttachOnce(std::function<void(void)> f){
    fallFunc = f;
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
    if(interruptin.read() == high_on_pushed){
        counter++;
        if(counter >= detectionThreshold){
            status = true;
            ticker.detach();
            monitoring = false;
            riseFunc();
            riseFunc = []{return;};
        }
    }else{
        counter=0;
        ticker.detach();
        monitoring = false;
    }
}

//サンプリング．チャタリング対策
void Switch::fallCheck(){
    if(interruptin.read() != high_on_pushed){
        counter++;
        if(counter >= detectionThreshold){
            status = false;
            ticker.detach();
            monitoring = false;
            fallFunc();
            fallFunc = []{return;};
        }
    }else{
        counter=0;
        ticker.detach();
        monitoring = false;
    }
}

