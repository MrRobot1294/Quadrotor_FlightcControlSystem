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

//�жϷ�����
void URGENT_STOP(void);
//��ͣ�ĳ�ʼ������
void urgent_stop_init(void);

void fly_load(void);//�Զ��������




#endif

