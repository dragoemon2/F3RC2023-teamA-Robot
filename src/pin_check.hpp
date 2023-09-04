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
    #endif

    #if 1
    printf("DIR\n");
    DigitalOut dir1(DIR_1);
    DigitalOut dir2(DIR_2);
    DigitalOut dir3(DIR_3);
    DigitalOut dir4(DIR_4);
    #endif

    
    printf("ALL PINS CHECKED\n");
}


#endif