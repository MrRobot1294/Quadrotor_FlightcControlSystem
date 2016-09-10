#ifndef __MY_PWM_H
#define __MY_PWM_H
#include "includes.h"
//引脚结构体的定义

void pwm_init(void);
//设置占空比的函数，该函数的带入参数就是占空比的值
void set_compare(u16 motor1,u16 motor2,u16 motor3,u16 motor4);

u32 my_test(void);
void motor_driver(void);
#endif


