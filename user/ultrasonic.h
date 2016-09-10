#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H
#include "includes.h"
void ultra_trig_init(void);
void ECHO_interrupt_handler(void);
void echo_init(void);
void send_signal(void);
void ultra_timer_init(void);
void ultra_echo_init(void);
void ultra_intergrate_init(void);//超声波相关函数的初始化集合

#endif


