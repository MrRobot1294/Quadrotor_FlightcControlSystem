#ifndef __FLYACTION_H
#define __FLYACTION_H
#include "includes.h"
void fly_limit(void);
void key_init(void);
u8 read_key1(void);
u8 read_key2(void);
u8 read_key3(void);

void Init_ADCWheel(void);
unsigned long ADC_ValueGet(uint32_t ui32Base, uint32_t ui32SequenceNum);

//中断服务函数
void URGENT_STOP(void);
//急停的初始化程序
void urgent_stop_init(void);

void fly_load(void);//自动降落程序




#endif

