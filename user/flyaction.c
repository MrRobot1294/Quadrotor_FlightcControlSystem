#include "flyaction.h"
#include "my6050data.h"
#include "led.h"
#include "adc.h"
#include "system_ti.h"
//this file includes the setting of keys and wheel,
#define SequenceNum 	3	

extern float ROLL,PITCH,YAW;//��Ԫ�ص�����
extern short GYROX,GYROY,GYROZ;
extern float DESIRE_ROLL;
extern float DESIRE_PITCH;
extern float DESIRE_YAW;
extern float DESIRE_HEIGHT;

extern float PID_ROLL_OUT,PID_PITCH_OUT,PID_YAW_OUT,PID_HEIGHT_OUT;

extern u32 thrust_base;

extern u16 PWM1; //0%������
extern u16 PWM2; //0%������
extern u16 PWM3; //0%������
extern u16 PWM4; //0%������
extern float distance;
extern u16 ms5;
extern u16 ms200;


//����ͣ��
void fly_limit(void){
  if(ROLL>30||ROLL<-30||PITCH>30||PITCH<-30){
     while(1){set_compare(1000,1000,1000,1000);}
}
}

//�����ļ���������ŵĳ�ʼ���Լ�����
//���ó��� PC7  PD6  PD7
void key_init(void){
	//pc7  BUTTON 1
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//�൱��ʹ���ĸ��˿�GPIOF
	GPIOPadConfigSet(GPIO_PORTC_BASE,GPIO_PIN_7,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_OD);
	//ROM_GPIODirModeSet(GPIO_PORTC_BASE,GPIO_PIN_7,GPIO_DIR_MODE_IN);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_7);//����ַ����д�룬��������GPIO_PIN_7
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7,GPIO_PIN_7); 
	
	//pD6  BUTTON 2   
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//�൱��ʹ���ĸ��˿�GPIOF
	GPIOPadConfigSet(GPIO_PORTD_BASE,GPIO_PIN_6,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_OD);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_6);//
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6,GPIO_PIN_6); 
	
  //pD6  BUTTON 3
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//�൱��ʹ���ĸ��˿�GPIOF
	GPIOPadConfigSet(GPIO_PORTD_BASE,GPIO_PIN_7,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);
	ROM_GPIODirModeSet(GPIO_PORTD_BASE, GPIO_PIN_7, GPIO_DIR_MODE_IN);
	
}
//�����͵�ƽ������������,������������½��᷵��0�����򷵻�1
u8 read_key1(void){
	u8 key1_temp=1;
	ROM_GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_7);//����ַ����д�룬��������GPIO_PIN_7
  key1_temp=GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_7);
  if(key1_temp==0){
		delay_ms(60);
	key1_temp=GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_7);
		if(key1_temp==0){
		return 0;
		}
	}
	return 1;
}
//�����͵�ƽ������������,������������½��᷵��0�����򷵻�1
u8 read_key2(void){
	u8 key2_temp=1;
	ROM_GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_6);
  key2_temp=GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6);
  if(key2_temp==0){
		delay_ms(60);
	key2_temp=GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6);
		if(key2_temp==0){
		return 0;
		}
	}
	return 1;
}
//�����͵�ƽ������������,������������½��᷵��0�����򷵻�1
u8 read_key3(void){
  u8 key3_temp=1;
  key3_temp=GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7);
  if(!key3_temp){
		delay_ms(40);
	  while(!GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7));
		return 0;
	}else{
	return 1;
	}
}

//���ֳ�ʼ������
void Init_ADCWheel(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0);
	ADCSequenceConfigure(ADC0_BASE, SequenceNum, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, SequenceNum, 0, ADC_CTL_CH3 | ADC_CTL_IE |ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, SequenceNum);
	ADCIntClear(ADC0_BASE, SequenceNum);
}
//��ȡ���ֵ�����
unsigned long ADC_ValueGet(uint32_t ui32Base, uint32_t ui32SequenceNum)
{
    unsigned long value = 0;	
    uint32_t ADCValue[1];
    ADCProcessorTrigger(ui32Base, ui32SequenceNum);
    while(!ADCIntStatus(ui32Base, ui32SequenceNum, false))
    {
    }
    ADCIntClear(ui32Base, ui32SequenceNum);
    ADCSequenceDataGet(ui32Base, ui32SequenceNum, ADCValue);
    value = ADCValue[0];
    return value;
}

void URGENT_STOP(void){
  if(GPIOIntStatus(GPIO_PORTC_BASE,true) == GPIO_PIN_5)//�õ��жϵı�־λ
	{
		GPIOIntClear(GPIO_PORTC_BASE,GPIO_INT_PIN_5);
    led1_on();
	  led2_on();
		led3_on();	
			while(1)
			{
				set_compare(1000,1000,1000,1000);
			}
	}
}


void urgent_stop_init(void){
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//����A��ʱ��
	//GPIODirModeSet(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_DIR_MODE_IN);//����io���ŵķ����Լ�io�ڵ�ģʽ�����������
	GPIOPadConfigSet(GPIO_PORTC_BASE,GPIO_PIN_5,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPD);//���ŵ�����
	//GPIODirModeSet(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_PIN_TYPE_STD_WPU);//���Ų������������õ�ԭ������Ϊ�����������������ϸ���������������������
	
	GPIOIntTypeSet(GPIO_PORTC_BASE,GPIO_PIN_5,GPIO_RISING_EDGE);//�����ж����͵�����
	GPIOIntEnable(GPIO_PORTC_BASE,GPIO_INT_PIN_5);//�����жϵ�ʹ��
  IntPrioritySet(GPIO_INT_PIN_5,0);//�ж����ȼ�������
  GPIOIntRegister(GPIO_PORTC_BASE,URGENT_STOP);//Ϊһ�����������жϷ�����
	IntEnable(GPIO_INT_PIN_5);//ʹ��оƬ���ж�
}
//�������
void fly_load(void){
  while(1){
	  if(ms5>5){  
			ms5=0;
			send_signal();
		  yaw_roll_pitch_gyro_data(&ROLL,&PITCH,&YAW,&GYROX,&GYROY,&GYROZ);
			moving_fliter(ROLL,PITCH,YAW,GYROX,GYROY,GYROZ);
			control_motor(ROLL,PITCH,YAW,
	                  GYROX,GYROY,GYROZ,
				            distance, //ʵ�ʲ����ľ���
						        DESIRE_ROLL,DESIRE_PITCH,DESIRE_YAW,0,//Ŀ��߶�
						        &PID_ROLL_OUT,&PID_PITCH_OUT,&PID_YAW_OUT,&PID_HEIGHT_OUT);		
			set_motor(thrust_base,&PWM1,&PWM2,&PWM3,&PWM4,
	               PID_ROLL_OUT,PID_PITCH_OUT,PID_YAW_OUT,PID_HEIGHT_OUT);	
		}
//		if(ms200>200){
//		  ms200=0;
//			DESIRE_HEIGHT-=10;
//			if(DESIRE_HEIGHT<0){
//			  DESIRE_HEIGHT=0;
//			}
//		}
		fly_limit();
	}
		
}








