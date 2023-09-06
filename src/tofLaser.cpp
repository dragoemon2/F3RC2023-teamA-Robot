#include "tofLaser.hpp"


#define DEFAULT_ADDRESS (0x29)
#define READ_DENOISE_N (15)


TOFLaser::TOFLaser(int address, PinName xshut_pin, PinName sda_pin, PinName scl_pin): xshut(xshut_pin), i2c(sda_pin, scl_pin), sensor(i2c, timer), address(address)
{
    xshut.output();
    xshut.write(0);
    //address = DEFAULT_ADDRESS + 2 + 2*id;
    //init();
    last_value = -1; //デバッグ用
}

//初期化，アドレスの書き込み
void TOFLaser::init(){
    bool result;

    xshut.input();
    //xshut.write(1);

    //タイムアウト(ミリ秒)
    sensor.setTimeout(100);

    //測定時間の上限
    sensor.setMeasurementTimingBudget(33000);
    

    //アドレスの書き込み
    sensor.setAddress(address);

    //センサー初期化
    result = sensor.init();

    //デバッグ用
    printf("%d\n",result);

    //測定開始
    sensor.startContinuous(0);
}

void TOFLaser::restart(){
    //i2c.write(DEFAULT_ADDRESS, (const char *)buffer, 2, 0);
    //再接続
    sensor.stopContinuous();
    sensor.setAddress(DEFAULT_ADDRESS);
    init();
}

//値の読み取り
int TOFLaser::read(){
    return -1;
    /*
    int value = last_value;
    if(sensor.readReg(VL53L0X::RESULT_INTERRUPT_STATUS) & 0x07){
        value = sensor.readRangeContinuousMillimeters(false);
        last_value = value;

        #if 1
        if(value > 8000){
            
            value = -1;
        }
        #endif
        if(value < 30){
            value = -1;
        }
    }else{
        value = -2;
        //restart();
    }

    if(sensor.timeoutOccurred()){
        restart();
        return -3;
    }

    if(value == 0x0b01){
        //通信遮断時
        //再接続を試みる
        restart();
        return -4;
    }

    return value;
    */
}

int TOFLaser::readDenoise(){
    return read();
}