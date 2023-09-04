#pragma once

#include <mbed.h>
#include "pins.hpp"
#include "parameters.hpp"

#if USING_R1

inline void pin_check(){
    #if 1
    printf("PWM\n");
    PwmOut pwm1(PWM_1);
    PwmOut pwm2(PWM_2);
    PwmOut pwm3(PWM_3);
    PwmOut pwm4(PWM_4);
    PwmOut pwm5(PWM_5);
    #endif

    #if 1
    printf("DIR\n");
    DigitalOut dir1(DIR_1);
    DigitalOut dir2(DIR_2);
    DigitalOut dir3(DIR_3);
    DigitalOut dir4(DIR_4);
    DigitalOut dir5(DIR_5);
    #endif

    #if 1
    printf("RE_A\n");
    InterruptIn rea1(RE_1_A);
    InterruptIn rea2(RE_2_A);
    InterruptIn rea3(RE_3_A);
    InterruptIn rea4(RE_4_A);
    #endif

    #if 1
    printf("RE_B\n");
    DigitalIn reb1(RE_1_B);
    DigitalIn reb2(RE_2_B);
    DigitalIn reb3(RE_3_B);
    DigitalIn reb4(RE_4_B);
    #endif

    #if 1
    printf("I2C\n");
    I2C i2c(SDA,SCL);
    #endif

    #if 1
    printf("XSHUT\n");
    DigitalOut xshut1(XSHUT_1);
    DigitalOut xshut2(XSHUT_2);
    DigitalOut xshut3(XSHUT_3);
    DigitalOut xshut4(XSHUT_4);
    #endif


    printf("ALL PINS CHECKED\n");
}

#else

inline void pin_check(){
    #if 1
    printf("PWM\n");
    PwmOut pwm1(PWM_1);
    PwmOut pwm2(PWM_2);
    PwmOut pwm3(PWM_3);
    PwmOut pwm4(PWM_4);
    PwmOut pwm5(PWM_5);
    #endif

    #if 1
    printf("DIR\n");
    DigitalOut dir1(DIR_1);
    DigitalOut dir2(DIR_2);
    DigitalOut dir3(DIR_3);
    DigitalOut dir4(DIR_4);
    DigitalOut dir5(DIR_5);
    #endif

    #if 1
    printf("RE_A\n");
    InterruptIn rea1(RE_1_A);
    InterruptIn rea2(RE_2_A);
    InterruptIn rea3(RE_3_A);
    InterruptIn rea4(RE_4_A);
    #endif

    #if 1
    printf("RE_B\n");
    DigitalIn reb1(RE_1_B);
    DigitalIn reb2(RE_2_B);
    DigitalIn reb3(RE_3_B);
    DigitalIn reb4(RE_4_B);
    #endif

    #if 1
    printf("DT35\n");
    AnalogIn laser1(LASER_1);
    AnalogIn laser2(LASER_2);
    AnalogIn laser3(LASER_3);
    #endif
    
    printf("ALL PINS CHECKED\n");
}


#endif