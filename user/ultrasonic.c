#include "ultrasonic.h"
#include "includes.h"

//TRIG   PE1
//ECGO  PE2
//���������������trig������ģʽ�����ҽ�������
float distance=0;
//�������ԣ�û������
////trig������
//void ultra_trig_init(void){
//  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//�൱��ʹ���ĸ��˿�GPIOF
//	GPIOPadConfigSet(GPIO_PORTE_BASE,GPIO_PIN_1,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_OD);//���ó����������Ĭ��Ҳ��
//	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1);
//	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);//��trig����
//}
////����echoΪ����ģʽ
//void ultra_echo_init(void){
//  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//�൱��ʹ���ĸ��˿�GPIOF
//	GPIOPadConfigSet(GPIO_PORTE_BASE,GPIO_PIN_2,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_OD);//���ó����������Ĭ��Ҳ��
//	ROM_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2);
//	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0);
//}


//TRIG  F0
//ECHO  F4

void ultra_trig_init(void){
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//�൱��ʹ���ĸ��˿�GPIOF
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_OD);//���ó����������Ĭ��Ҳ��
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);//��trig����
}
//����echoΪ����ģʽ
void ultra_echo_init(void){
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//�൱��ʹ���ĸ��˿�GPIOF
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_OD);//���ó����������Ĭ��Ҳ��
	ROM_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0);
}



void ECHO_interrupt_handler(void){ //�Ѿ��������ж�
	static u8 read_flag=0;
  if(GPIOIntStatus(GPIO_PORTF_BASE,true) == GPIO_PIN_4)//�õ��жϵı�־λ
	{
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_4);
		if(read_flag==0){
		  HWREG(WTIMER0_BASE+TIMER_O_TAV)=0;//����������������صļ����������ֵ
			read_flag=1;
			led1_on();
		}else{
		  distance=TimerValueGet(WTIMER0_BASE,TIMER_A)/4000;//cmΪ��λ
			read_flag=0;
			//printf("distance===%f\r\n",distance);
		}
	}
}

//����һ������10us�ĸߵ�ƽ
void send_signal(void){
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
	delay_us(60);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
	delay_us(60);
}

//��©���
void echo_init(void){
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_OD);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0);
	ROM_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
	
	GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_BOTH_EDGES);//�����жϣ������غ��½���
	GPIOIntEnable(GPIO_PORTF_BASE,GPIO_INT_PIN_4);
	IntPrioritySet(GPIO_INT_PIN_4,2);
	GPIOIntRegister(GPIO_PORTF_BASE,ECHO_interrupt_handler);
	IntEnable(GPIO_INT_PIN_4);
}


//��������ʱ����ʹ��
void ultra_timer_init(void){
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
	TimerConfigure(WTIMER0_BASE,TIMER_CFG_A_PERIODIC_UP|TIMER_CFG_SPLIT_PAIR);
  ROM_TimerEnable(WTIMER0_BASE, TIMER_A);	
}
//���к����ļ���
void ultra_intergrate_init(void){
    ultra_timer_init();//��������ʹ�õĶ�ʱ��
		ultra_trig_init();//��ʼ��trig����
		ultra_echo_init();//��ʼ��echo������
		echo_init();//����echo���жϣ��������жϣ�һ�ζ�ȡ���ݽ��������ж�
}








