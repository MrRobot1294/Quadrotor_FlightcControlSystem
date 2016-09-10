#include "my_pwm.h"
#include "pwm.h"
#include "timer.h"

//PWM0 PB6
//PWM1 PB7
//PWM2 PB4
//PWM3 PB5

void pwm_init(void){
	
  SysCtlPWMClockSet(SYSCTL_PWMDIV_2);//pwm��ʱ�Ӳ���Ƶ��25Mhz��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//pwmʹ��PB6 PB7
	GPIOPinConfigure(GPIO_PB6_M0PWM0);
  GPIOPinConfigure(GPIO_PB7_M0PWM1);
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
  GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);
	GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_6|GPIO_PIN_7,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD);
	//���ö�ʱ������
	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN |PWM_GEN_MODE_NO_SYNC);//pwm������ģʽ
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 66666);//����PWM���������ڣ�25000��1k 
	//pwm�����������ã����÷�Ƶֵ
//  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 5000);
// PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,
//                     (PWMGenPeriodGet(PWM0_BASE, PWM_OUT_0)-1)0);
	
	//��ʼռ�ձȵ�����
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,0);//�������0������ռ��,��ʵ����Ӧ���Ǽ���ʱ�ӵļ���ֵ
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,0);//�������0������ռ��,��ʵ����Ӧ���Ǽ���ʱ�ӵļ���ֵ
		PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT | PWM_OUT_0_BIT, true);
		PWMGenEnable(PWM0_BASE, PWM_GEN_0);
//��������ͨ��������
    SysCtlPWMClockSet(SYSCTL_PWMDIV_2);//pwm��ʱ�Ӳ���Ƶ��25Mhz��
		SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//pwmʹ��PB6 PB7
		GPIOPinConfigure(GPIO_PB4_M0PWM2);
    GPIOPinConfigure(GPIO_PB5_M0PWM3);
		GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);
	  GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_4|GPIO_PIN_5,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD);
	  PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN |PWM_GEN_MODE_NO_SYNC);//pwm������ģ�
	  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, 66666);//����PWM����������
	
	  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2,0);//�������0������ռ��,��ʵ����Ӧ���Ǽ���ʱ�ӵļ���ֵ
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3,0);//�������0������ռ��,��ʵ����Ӧ���Ǽ���ʱ�ӵļ���ֵ
		PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT | PWM_OUT_3_BIT, true);
		PWMGenEnable(PWM0_BASE, PWM_GEN_1);
}



////����ռ�ձȵĺ������ú����Ĵ����������ռ�ձȵ�ֵ
void set_compare(u16 motor1,u16 motor2,u16 motor3,u16 motor4){
   PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,66666/2000*motor1);//���һ��������ʾ����λ�Ŀ��
	 PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,66666/2000*motor2);
	 PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2,66666/2000*motor3);
	 PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3,66666/2000*motor4);
}

//����ǲ��Եĵ�ǰPWM��ֵʱ��ļ���ֵ��64000
u32 my_test(void){
  u32 temp;
	temp=PWMGenPeriodGet(PWM0_BASE, PWM_OUT_0);
	return temp;
}


void motor_driver(void){
  pwm_init();
	set_compare(2000,2000,2000,2000);//��������
	delay_ms(2000);
	set_compare(1000,1000,1000,1000);
  delay_ms(2000);
	set_compare(1100,1100,1100,1100);
  delay_ms(2000);
	

  
	
}



