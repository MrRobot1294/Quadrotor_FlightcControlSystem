#include "ultrasonic.h"
#include "includes.h"

//TRIG   PE1
//ECGO  PE2
//这个函数里面配置trig的引脚模式，并且将其拉低
float distance=0;
//经过测试，没有问题
////trig被拉低
//void ultra_trig_init(void){
//  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//相当于使能哪个端口GPIOF
//	GPIOPadConfigSet(GPIO_PORTE_BASE,GPIO_PIN_1,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_OD);//配置成了推挽输出默认也是
//	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1);
//	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);//将trig拉低
//}
////配置echo为输入模式
//void ultra_echo_init(void){
//  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//相当于使能哪个端口GPIOF
//	GPIOPadConfigSet(GPIO_PORTE_BASE,GPIO_PIN_2,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_OD);//配置成了推挽输出默认也是
//	ROM_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2);
//	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0);
//}


//TRIG  F0
//ECHO  F4

void ultra_trig_init(void){
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//相当于使能哪个端口GPIOF
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_OD);//配置成了推挽输出默认也是
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);//将trig拉低
}
//配置echo为输入模式
void ultra_echo_init(void){
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//相当于使能哪个端口GPIOF
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_OD);//配置成了推挽输出默认也是
	ROM_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0);
}



void ECHO_interrupt_handler(void){ //已经进入了中断
	static u8 read_flag=0;
  if(GPIOIntStatus(GPIO_PORTF_BASE,true) == GPIO_PIN_4)//得到中断的标志位
	{
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_4);
		if(read_flag==0){
		  HWREG(WTIMER0_BASE+TIMER_O_TAV)=0;//这个函数可以清楚相关的计数器里面的值
			read_flag=1;
			led1_on();
		}else{
		  distance=TimerValueGet(WTIMER0_BASE,TIMER_A)/4000;//cm为单位
			read_flag=0;
			//printf("distance===%f\r\n",distance);
		}
	}
}

//发送一个大于10us的高电平
void send_signal(void){
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
	delay_us(60);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
	delay_us(60);
}

//开漏输出
void echo_init(void){
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_OD);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0);
	ROM_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
	
	GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_BOTH_EDGES);//边沿中断，上升沿河下降沿
	GPIOIntEnable(GPIO_PORTF_BASE,GPIO_INT_PIN_4);
	IntPrioritySet(GPIO_INT_PIN_4,2);
	GPIOIntRegister(GPIO_PORTF_BASE,ECHO_interrupt_handler);
	IntEnable(GPIO_INT_PIN_4);
}


//超声波定时器的使能
void ultra_timer_init(void){
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
	TimerConfigure(WTIMER0_BASE,TIMER_CFG_A_PERIODIC_UP|TIMER_CFG_SPLIT_PAIR);
  ROM_TimerEnable(WTIMER0_BASE, TIMER_A);	
}
//所有函数的集合
void ultra_intergrate_init(void){
    ultra_timer_init();//超声波所使用的定时器
		ultra_trig_init();//初始化trig引脚
		ultra_echo_init();//初始化echo的引脚
		echo_init();//配置echo的中断，跳变沿中断，一次读取数据进入两次中断
}








