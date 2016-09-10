#ifndef __INCLUDES_H
#define __INCLUDES_H
#include "stdint.h"  //里面有关于字大小的定义
#include "system_ti.h"  //复位中断里面必须执行的函数
#include "fpu.h"  //浮点数计算的头文件

#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include "string.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_uart.h"
#include "debug.h"
#include "interrupt.h"
#include "rom.h"
#include "rom_map.h"
#include "sysctl.h"
#include "uart.h"
#include "uartstdio.h"
#include "pin_map.h"
#include "gpio.h"
#include "sysctl.h"
#include "timer.h"
#include "ssi.h"
#include "adc.h"
#include "hw_timer.h"
#include "hw_gpio.h"
#include "hw_ints.h"

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

//我自己的库函数
#include "my6050data.h"
#include "led.h"
#include "my_usart.h"
#include "my_interrupt.h"
#include "my_pwm.h"
#include "control.h"
#include "pid.h"
#include "hmc5883l.h"
#include "LCDDriver.h"
#include "flyaction.h"
#include "fliter.h"
#include "prepare.h"
#include "ultrasonic.h"
#include "camearfindway.h"
#include "unlock.h"

#endif


