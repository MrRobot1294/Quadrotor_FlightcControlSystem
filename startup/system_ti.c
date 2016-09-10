#include "system_ti.h"
#include "includes.h"
/*
�ú�����Ƕ�뵽�����ļ�����ģ����û��������������̽����޷�����ɹ�����ϵͳ��λ֮�����뵽�������
�е���SystemInit��������������������������˸���Ԥ�㵥Ԫ���Լ�ϵͳʱ�ӵ����á�
��ǰ��ϵͳʱ���Ѿ����ó�Ϊ50mhz���벻Ҫ������������Ƶֵ����������������ʱ������쳣������I2C���쳣
���ͷ�ļ��������˼����£�
1.��ʱ��0���жϳ�ʼ������
2.��ʱ���򣨵�����ϵͳ�Ŀ⺯�����м򵥵���ʱ��
*/
//��λ�ж�,�����ļ��������������ָ�������ط�û��ʵ�֣��������ʵ�ָ������ļ���
//�����������������
extern u16 ms5;
extern int fly_time;//�ɻ�����ʱ�䳤���趨
extern u16 ms200;
extern u16 ms100;


void SystemInit(){
  FPUEnable();
	FPULazyStackingEnable();//�������������㵥Ԫ
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                       SYSCTL_OSC_MAIN);//ʹ��pll��ʱ�ӣ�2��Ƶ100Mhz
}

////��ϵͳ10��Ƶ��ʱ���ж�ÿ�εĵ���ʱ����2.5us���������жϷ���������400000�ļ����ﵽ1s
//����ϵͳ�����ڽ���������������������
void systime_handler(void){
  if(TimerIntStatus(TIMER0_BASE,true) == 1)//�õ��жϵı�־λ
	{			
		ms5++;
		fly_time--;
		ms200++;
		ms100++;
		TimerIntClear(TIMER0_BASE,TIMER_BOTH);	
		
	}
	
	
	
}

//��ʱ���жϵĳ�ʼ������,ʹ����32λ�Ķ�ʱ��0,Ŀǰ��ʱ��Ƶ�ʸ���100Mhz,�������õ���һ��1ms���ж�.ͨ���߼�������
//���ϵĴ���ֵ��������
void systime_init(void){
	ROM_IntMasterEnable();
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);//��ʱ��0
	TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);//����32/2��ʱ�� ���ڶ�ʱ��ʹ��֮������
//	ROM_TimerPrescaleSet(TIMER0_BASE, TIMER_A,5);//��Ƶ,�������ֵ�����ڰ��ģʽ
	ROM_TimerLoadSet(TIMER0_BASE,TIMER_A, (66799-1));//��ʱ��ʱ������,���һ������������װ��ֵ	
	ROM_IntDisable(INT_TIMER0A);//ʹ���ж�
	ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);//��ʱ����ʹ��
	TimerIntRegister(TIMER0_BASE,TIMER_BOTH,systime_handler);//д��ں���
	IntPrioritySet(INT_TIMER0A,1);  //���ȼ��Ŀ���
	ROM_TimerEnable(TIMER0_BASE, TIMER_BOTH);//��ʱ��ʹ��
}
//�ú���������ϵͳ�������ڳ������֮ǰ���ã���ֹ����ֵ����5�޷������������ж�ʱ��
void systime_enable(void){
  ROM_IntEnable(INT_TIMER0A);
	ROM_TimerEnable(TIMER0_BASE, TIMER_BOTH);
}



//����T���������ĺ�����д�ļ򵥵���ʱ����
void delay_ms(u32 ms){
	int i;
	for(i=0;i<ms;i++){
	  ROM_SysCtlDelay(100000/3);
	} 

}

void delay_us(u32 us){
	int i;
	for(i=0;i<us;i++){
	  ROM_SysCtlDelay(100000/3/1000);
	}
}

void delay_s(u32 s){
  int i;
	for(i=0;i<s;i++){
	  ROM_SysCtlDelay(100000/3*1000);
	}
}





