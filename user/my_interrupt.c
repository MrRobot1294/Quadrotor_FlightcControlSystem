#include "my_interrupt.h"
#include "includes.h"
#include "led.h"
#include "my_usart.h"
#include "timer.h"
//�ⲿ�жϷ������������ļ������й�����
void my_interrupt_handler(void){
	
	if(GPIOIntStatus(GPIO_PORTA_BASE,true) == GPIO_PIN_7)//�õ��жϵı�־λ
	{
		GPIOIntClear(GPIO_PORTA_BASE,GPIO_INT_PIN_7);
	  printf("��ã����Ǽ�ΰ\r\n");
	}
 
}

void my_interrupt_init(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//����A��ʱ��
	//GPIODirModeSet(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_DIR_MODE_IN);//����io���ŵķ����Լ�io�ڵ�ģʽ�����������
	GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);//���ŵ�����
	//GPIODirModeSet(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_PIN_TYPE_STD_WPU);//���Ų������������õ�ԭ������Ϊ�����������������ϸ���������������������
	
	GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_LOW_LEVEL);//�����ж����͵�����
	GPIOIntEnable(GPIO_PORTA_BASE,GPIO_INT_PIN_7);//�����жϵ�ʹ��
  IntPrioritySet(GPIO_INT_PIN_7,0);//�ж����ȼ�������
  GPIOIntRegister(GPIO_PORTA_BASE,my_interrupt_handler);//Ϊһ�����������жϷ�����
	IntEnable(GPIO_INT_PIN_7);//ʹ��оƬ���ж�
}









////��ʱ���жϵĳ�ʼ������,�����Ƶ�Լ�װ�ض�ʱ���ĳ�ֵ��֪��Ϊʲô�������ã���ͣ��
//�������ʹ����16λ�Ķ�ʱ��������װ��ֵ��65535����ˣ����ٴ�Ҳû�ã�ʹ��32λ�Ķ�ʱ���ɹ�1s��ʱ
//void timer_init(void){
//	ROM_IntMasterEnable();
//	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);//��ʱ��0
//	TimerConfigure(TIMER0_BASE,TIMER_CFG_A_PERIODIC|TIMER_CFG_SPLIT_PAIR);//����32/2��ʱ�� ���ڶ�ʱ��ʹ��֮������
////	ROM_TimerPrescaleSet(TIMER0_BASE, TIMER_A,199);//��Ƶ,��Ч����
////	ROM_TimerLoadSet(TIMER0_BASE,TIMER_A, 1);//��ʱ��ʱ������,���һ������������װ��ֵ	
//	
//	ROM_IntEnable(INT_TIMER0A);//ʹ���ж�
//	ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);//��ʱ����ʹ��
//	TimerIntRegister(TIMER0_BASE,TIMER_A,timer0_handler);//д��ں���
//	ROM_TimerEnable(TIMER0_BASE, TIMER_A);//��ʱ��ʹ��
//}

//////1ms�ļ���
//void super_voice_init(void){
//	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);//��ʱ��0
//	TimerConfigure(TIMER1_BASE,TIMER_CFG_PERIODIC_UP);//����32/2��ʱ�� ���ڶ�ʱ��ʹ��֮������
//	//ROM_TimerPrescaleSet(TIMER1_BASE, TIMER_BOTH,9);//��Ƶ,�������ֵ�����ڰ��ģʽ
//	//TimerPrescaleMatchSet(TIMER1_BASE,TIMER_BOTH,999);
//	ROM_TimerLoadSet(TIMER1_BASE,TIMER_BOTH,10000);
//	ROM_TimerEnable(TIMER1_BASE, TIMER_BOTH);//��ʱ��ʹ��
//}


//u32 get_timer1_value(void){
//  return TimerValueGet(TIMER1_BASE,TIMER_BOTH);
//}

//void TIMERA_IRQHandler(){
//	static u32 j,k;
//  if(TimerIntStatus(WTIMER0_BASE,true) == 1){
//    TimerIntClear(WTIMER0_BASE,TIMER_A);	
//		j++;
//		k=ROM_TimerLoadGet(WTIMER0_BASE,TIMER_A);
//		printf("j%d\r\n",j);
//		printf("reload value%d\r\n",k);
//	}
//}

//void super_voice_init(void){
//	ROM_IntMasterEnable();
//	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);//��ʱ��0
//	TimerConfigure(WTIMER0_BASE,TIMER_CFG_A_PERIODIC_UP|TIMER_CFG_SPLIT_PAIR);//����32/2��ʱ�� ���ڶ�ʱ��ʹ��֮������
//	//ROM_TimerPrescaleSet(WTIMER0_BASE, TIMER_A,2);//��Ƶ,�������ֵ�����ڰ��ģʽ
//	ROM_TimerLoadSet(WTIMER0_BASE,TIMER_A, 100000000);//��ʱ��ʱ������,���һ������������װ��ֵ	
//	
//	ROM_IntDisable(INT_WTIMER0A);//ʹ���ж�
//	ROM_TimerIntEnable(WTIMER0_BASE, TIMER_TIMA_TIMEOUT);//��ʱ����ʹ��
//	TimerIntRegister(WTIMER0_BASE,TIMER_A,TIMERA_IRQHandler);//д��ں���
//	IntPrioritySet(INT_WTIMER0A,1);  //���ȼ��Ŀ���
//	ROM_TimerEnable(WTIMER0_BASE, TIMER_A);//��ʱ��ʹ��
//	ROM_IntEnable(INT_WTIMER0A);
//	ROM_TimerEnable(WTIMER0_BASE, TIMER_A);	
//}

void super_voice_init(void){
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
	TimerConfigure(WTIMER0_BASE,TIMER_CFG_A_PERIODIC_UP|TIMER_CFG_SPLIT_PAIR);
  ROM_TimerEnable(WTIMER0_BASE, TIMER_A);	
}

u32 get_timer1_value(void){
  return TimerValueGet(WTIMER0_BASE,TIMER_A);
}







