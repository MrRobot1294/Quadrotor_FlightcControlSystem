#ifndef __SYSTEM_TI_H
#define __SYSTEM_TI_H
#include <stdint.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
void systime_init(void);
void systime_handler(void);
void systime_enable(void);//系统采样周期的使能函数
//延时函数
void delay_ms(u32 ms);
void delay_us(u32 us);
void delay_s(u32 s);
	
#endif

