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
//姿态角与角速度的全局变量
float ROLL,PITCH,YAW;
short GYROX,GYROY,GYROZ;
//期望的姿态角，也就是外环输入
float DESIRE_ROLL=0;
float DESIRE_PITCH=0;
float DESIRE_YAW=0;

float DESIRE_HEIGHT=0;
//串级pid的输出量，也就是电机pwm占空比设置的输入量
float PID_ROLL_OUT,PID_PITCH_OUT,PID_YAW_OUT,PID_HEIGHT_OUT;

//全局的时间常数
u16 ms5=0;
int fly_time=0;
u16 ms200=0;
u16 ms100=0;

extern float distance;//在超声波头文件里面测量的距离

//PWM，便于仿真的时候观察值的大小1000到2000是油门的范围
u16 PWM1=1000; //0%的油门
u16 PWM2=1000; //0%的油门
u16 PWM3=1000; //0%的油门
u16 PWM4=1000; //0%的油门

int main(void){
	//油门基础值	
	led_init();//led的初始化
	all_gpio_pin_unlock();//解锁使用到的引脚
	key_init();//按键初始化程序
//	Init_ADCWheel();
	urgent_stop_init(); //急停的初始化程序
  lcd_screen_init();//显示屏初始化程序
  camear_gpio_init();//摄像头寻迹接口的初始化
	
	mpu6050_init();//6050的初始化程序
	ultra_intergrate_init();//超声波的初始化函数
	
	delay_ms(10000);//DMP的延时程序，等待数据数据稳定
  get_desire_value();//得到期望姿态	
//按键的设置
	set_key();
//	Uart_init();//串口初始化程序,包括终端	
	fill_array();//外环姿态的滤波程序
//	printf("MY_DESIRE_DATA:    %f    %f    %f\r\n",DESIRE_ROLL,DESIRE_PITCH,DESIRE_YAW);
	motor_driver();//PWM的初始化
	systime_init();//系统采样周期程序初始化
	systime_enable();//使能系统定时器
	//超声波的相关初始化函数，在里面的中断服务函数读出了相关距离
	ms5=0;//
    while(1) //程序里面姿态调整的大循环
    {		
			if(ms5>5){  //系统采样周期，里面包括传感器数据的采集，以及pid的控制算法
				ms5=0;//清除系统周期的计数值	
				ms200=0;//这个函数在降落程序中才会发挥作用
				send_signal();//发送超声波的触发信号
				yaw_roll_pitch_gyro_data(&ROLL,&PITCH,&YAW,&GYROX,&GYROY,&GYROZ);//已经调整过方向的数据
				moving_fliter(ROLL,PITCH,YAW,GYROX,GYROY,GYROZ);
				control_motor(ROLL,PITCH,YAW,
											GYROX,GYROY,GYROZ,
											distance,
											DESIRE_ROLL,(DESIRE_PITCH+0.6f),DESIRE_YAW,DESIRE_HEIGHT,  //已经在程序里面强行矫正了pitch期望
											&PID_ROLL_OUT,&PID_PITCH_OUT,&PID_YAW_OUT,&PID_HEIGHT_OUT);
				set_motor(thrust_base,&PWM1,&PWM2,&PWM3,&PWM4,
									 PID_ROLL_OUT,PID_PITCH_OUT,PID_YAW_OUT,PID_HEIGHT_OUT);			
				fly_limit();
				
//				if(ms100>100){
//				  ms100=0;
//					findway();//里面调整了初始姿态
//				}
    }
        wait_fly_time_over();
	}	
}


/////////////////////////////////////////////以下程序为测试模块功能使用/////////////////////////////////////////////////////////

//////////u32 temp;
////////////float angle;
//////////////测试函数...
//u32 ultra_data;
//u32 i;
//int main(){
////	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
////												SYSCTL_OSC_MAIN);//使用pll的时钟，4分频100Mhz
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
//////		printf("ROLL的大小%f\r\n",ROLL);
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
//////				HWREG(WTIMER0_BASE+TIMER_O_TAV)=0;//这个函数可以清楚相关的计数器里面的值
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
//	all_gpio_pin_unlock();//解锁引脚
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






































