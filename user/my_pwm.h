#ifndef __MY_PWM_H
#define __MY_PWM_H
#include "includes.h"
//���Žṹ��Ķ���

void pwm_init(void);
//����ռ�ձȵĺ������ú����Ĵ����������ռ�ձȵ�ֵ
void set_compare(u16 motor1,u16 motor2,u16 motor3,u16 motor4);

u32 my_test(void);
void motor_driver(void);
#endif


