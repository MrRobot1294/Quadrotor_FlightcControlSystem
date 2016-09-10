#include "includes.h"
#include "led.h"
#include "my_usart.h"
#include "timer.h"
#include "my_interrupt.h"
#include "pwm.h"
#include "stdint.h"
#include "my_pwm.h"
#include "my6050data.h"
u32 thrust_base=1265;
//��̬������ٶȵ�ȫ�ֱ���
float ROLL,PITCH,YAW;
short GYROX,GYROY,GYROZ;
//��������̬�ǣ�Ҳ�����⻷����
float DESIRE_ROLL=0;
float DESIRE_PITCH=0;
float DESIRE_YAW=0;

float DESIRE_HEIGHT=0;
//����pid���������Ҳ���ǵ��pwmռ�ձ����õ�������
float PID_ROLL_OUT,PID_PITCH_OUT,PID_YAW_OUT,PID_HEIGHT_OUT;

//ȫ�ֵ�ʱ�䳣��
u16 ms5=0;
int fly_time=0;
u16 ms200=0;
u16 ms100=0;

extern float distance;//�ڳ�����ͷ�ļ���������ľ���

//PWM�����ڷ����ʱ��۲�ֵ�Ĵ�С1000��2000�����ŵķ�Χ
u16 PWM1=1000; //0%������
u16 PWM2=1000; //0%������
u16 PWM3=1000; //0%������
u16 PWM4=1000; //0%������

int main(void){
	//���Ż���ֵ	
	led_init();//led�ĳ�ʼ��
	all_gpio_pin_unlock();//����ʹ�õ�������
	key_init();//������ʼ������
//	Init_ADCWheel();
	urgent_stop_init(); //��ͣ�ĳ�ʼ������
  lcd_screen_init();//��ʾ����ʼ������
  camear_gpio_init();//����ͷѰ���ӿڵĳ�ʼ��
	
	mpu6050_init();//6050�ĳ�ʼ������
	ultra_intergrate_init();//�������ĳ�ʼ������
	
	delay_ms(10000);//DMP����ʱ���򣬵ȴ����������ȶ�
  get_desire_value();//�õ�������̬	
//����������
	set_key();
//	Uart_init();//���ڳ�ʼ������,�����ն�	
	fill_array();//�⻷��̬���˲�����
//	printf("MY_DESIRE_DATA:    %f    %f    %f\r\n",DESIRE_ROLL,DESIRE_PITCH,DESIRE_YAW);
	motor_driver();//PWM�ĳ�ʼ��
	systime_init();//ϵͳ�������ڳ����ʼ��
	systime_enable();//ʹ��ϵͳ��ʱ��
	//����������س�ʼ����������������жϷ�������������ؾ���
	ms5=0;//
    while(1) //����������̬�����Ĵ�ѭ��
    {		
			if(ms5>5){  //ϵͳ�������ڣ�����������������ݵĲɼ����Լ�pid�Ŀ����㷨
				ms5=0;//���ϵͳ���ڵļ���ֵ	
				ms200=0;//��������ڽ�������вŻᷢ������
				send_signal();//���ͳ������Ĵ����ź�
				yaw_roll_pitch_gyro_data(&ROLL,&PITCH,&YAW,&GYROX,&GYROY,&GYROZ);//�Ѿ����������������
				moving_fliter(ROLL,PITCH,YAW,GYROX,GYROY,GYROZ);
				control_motor(ROLL,PITCH,YAW,
											GYROX,GYROY,GYROZ,
											distance,
											DESIRE_ROLL,(DESIRE_PITCH+0.6f),DESIRE_YAW,DESIRE_HEIGHT,  //�Ѿ��ڳ�������ǿ�н�����pitch����
											&PID_ROLL_OUT,&PID_PITCH_OUT,&PID_YAW_OUT,&PID_HEIGHT_OUT);
				set_motor(thrust_base,&PWM1,&PWM2,&PWM3,&PWM4,
									 PID_ROLL_OUT,PID_PITCH_OUT,PID_YAW_OUT,PID_HEIGHT_OUT);			
				fly_limit();
				
//				if(ms100>100){
//				  ms100=0;
//					findway();//��������˳�ʼ��̬
//				}
    }
        wait_fly_time_over();
	}	
}


/////////////////////////////////////////////���³���Ϊ����ģ�鹦��ʹ��/////////////////////////////////////////////////////////

//////////u32 temp;
////////////float angle;
//////////////���Ժ���...
//u32 ultra_data;
//u32 i;
//int main(){
////	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
////												SYSCTL_OSC_MAIN);//ʹ��pll��ʱ�ӣ�4��Ƶ100Mhz
////	led_init();
////	mpu6050_init();
//////	while(1){
//////	  led_on();
//////		ROM_SysCtlDelay(50000/3/1000);
//////		led_off();/
//////		ROM_SysCtlDelay(50000/3/1000);
//////	}
//////	HMC5883_INIT();

////	Uart_init();
//////	UARTprintf("Hello, world!\n");
//////	   motor_driver();
//////	   Uart_init();
//////      systime_init();
//////      systime_enable();
//////		  LCD_Enable();						
//////	  LCD_Init();							
//////  	LCD_ScreenClr();
//// 

////	while(1)
////	{
//////    led1_on();
//////		LCD_Draw_String("jiwei",0,0,0);
//////	  set_compare(PWM1,PWM2,PWM3,PWM4); 
////		yaw_roll_pitch_gyro_data(&ROLL,&PITCH,&YAW,&GYROX,&GYROY,&GYROZ);
//////		printf("    %f    %f    %f\r\n",ROLL,PITCH,YAW);
//////		ANGLE_data=GET_ANGLE();
//////		delay_s(2);
//////		led_off();
//////		delay_s(2);
////	//	UARTprintf("ms5%d\n",ms5);
//////		printf("ROLL�Ĵ�С%f\r\n",ROLL);
//////		get_desire(&DESIRE_ROLL,&DESIRE_PITCH,&DESIRE_YAW);
////	}

//		
//		
////		while(1){
//// 

//////			//ultra_data=get_timer1_value();
//////			ultra_data=HWREG(WTIMER0_BASE+TIMER_O_TAR);
//////			printf("ultra_data%d\r\n",ultra_data);
//////			if(ultra_data>200000000){
//////			  ROM_TimerDisable(WTIMER0_BASE, TIMER_A);	
//////				delay_ms(2000);
//////				printf("hahahahahahhahahahahhahahahhhahahahahahah\r\n");
//////				ROM_TimerEnable(WTIMER0_BASE, TIMER_A);	
//////				HWREG(WTIMER0_BASE+TIMER_O_TAV)=0;//����������������صļ����������ֵ
//////			}
////			
////			}
//    led_init();
//    Uart_init();
//		UARTprintf("Hello, world!\n");
//		all_gpio_pin_unlock();
//		ultra_intergrate_init();
//    while(1){
//			//printf("nihao\r\n");
//		  send_signal();
//			delay_ms(100);
//			
//		}		  
//}



//extern u8 direction_flag;



//int main(){
//	all_gpio_pin_unlock();//��������
//  led_init();
//	Uart_init();
//	UARTprintf("Hello, world!\n");
//	
//	//key_init();
//  camear_gpio_init();	
//	
//	while(1){
//	  findway();
//		led1_off();
//		led2_off();
//		led3_off();
//	}
//}






































