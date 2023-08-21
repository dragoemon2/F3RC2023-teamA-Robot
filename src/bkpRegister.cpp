#include "bkpRegister.hpp"
#include <mbed.h>

BKPRegister::BKPRegister(){
    HAL_PWR_EnableBkUpAccess();
}

void BKPRegister::open(unsigned int reg){
    if(reg > 5){
        return;
    }
    buffer = LL_RTC_BAK_GetRegister(RTC, LL_RTC_BKP_DR0 + reg);
    register_num = reg;
}

void BKPRegister::set(unsigned int begin, unsigned int size, unsigned int value){
    if(begin + size>32){
        return;
    }
    value = value & (0xFFFFFFFFU >> (32-size));
    buffer &= ~((0xFFFFFFFFU >> (32-size)) << (32-begin-size)); 
    buffer |= (value << (32-begin-size));
}

int BKPRegister::get(unsigned int begin, unsigned int size){
    if(begin + size>32){
        return -1;
    }
    return (buffer >> (32-begin-size)) & (0xFFFFFFFFU >> (32-size));
}

void BKPRegister::save(){
    LL_RTC_BAK_SetRegister(RTC, LL_RTC_BKP_DR0 + register_num, buffer);
}