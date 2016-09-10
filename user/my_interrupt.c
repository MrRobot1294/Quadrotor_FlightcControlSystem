#include "my_interrupt.h"
#include "includes.h"
#include "led.h"
#include "my_usart.h"
#include "timer.h"
//外部中断服务函数在启动文件里面有过声明
void my_interrupt_handler(void){
	
	if(GPIOIntStatus(GPIO_PORTA_BASE,true) == GPIO_PIN_7)//得到中断的标志位
	{
		GPIOIntClear(GPIO_PORTA_BASE,GPIO_INT_PIN_7);
	  printf("你好，我是纪伟\r\n");
	}
 
}

void my_interrupt_init(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//设置A的时钟
	//GPIODirModeSet(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_DIR_MODE_IN);//设置io引脚的方向以及io口的模式（输入输出）
	GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);//引脚的设置
	//GPIODirModeSet(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_PIN_TYPE_STD_WPU);//引脚不可以这样设置的原因是因为这个函数不能设置详细的引脚情况比如上拉下拉
	
	GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_LOW_LEVEL);//引脚中断类型的设置
	GPIOIntEnable(GPIO_PORTA_BASE,GPIO_INT_PIN_7);//引脚中断的使能
  IntPrioritySet(GPIO_INT_PIN_7,0);//中断优先级的设置
  GPIOIntRegister(GPIO_PORTA_BASE,my_interrupt_handler);//为一个引脚设置中断服务函数
	IntEnable(GPIO_INT_PIN_7);//使能芯片的中断
}









////定时器中断的初始化程序,这个分频以及装载定时器的初值不知道为什么不起作用，已停用
//这个程序使用了16位的定时器，他的装载值是65535溢出了，给再大也没用，使用32位的定时器成功1s定时
//void timer_init(void){
//	ROM_IntMasterEnable();
//	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);//定时器0
//	TimerConfigure(TIMER0_BASE,TIMER_CFG_A_PERIODIC|TIMER_CFG_SPLIT_PAIR);//配置32/2定时器 ，在定时器使能之后作用
////	ROM_TimerPrescaleSet(TIMER0_BASE, TIMER_A,199);//分频,有效果的
////	ROM_TimerLoadSet(TIMER0_BASE,TIMER_A, 1);//定时器时钟配置,最后一个参数是设置装载值	
//	
//	ROM_IntEnable(INT_TIMER0A);//使能中断
//	ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);//计时结束使能
//	TimerIntRegister(TIMER0_BASE,TIMER_A,timer0_handler);//写入口函数
//	ROM_TimerEnable(TIMER0_BASE, TIMER_A);//定时器使能
//}

//////1ms的计数
//void super_voice_init(void){
//	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);//定时器0
//	TimerConfigure(TIMER1_BASE,TIMER_CFG_PERIODIC_UP);//配置32/2定时器 ，在定时器使能之后作用
//	//ROM_TimerPrescaleSet(TIMER1_BASE, TIMER_BOTH,9);//分频,这个函数值工作在半角模式
//	//TimerPrescaleMatchSet(TIMER1_BASE,TIMER_BOTH,999);
//	ROM_TimerLoadSet(TIMER1_BASE,TIMER_BOTH,10000);
//	ROM_TimerEnable(TIMER1_BASE, TIMER_BOTH);//定时器使能
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
//	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);//定时器0
//	TimerConfigure(WTIMER0_BASE,TIMER_CFG_A_PERIODIC_UP|TIMER_CFG_SPLIT_PAIR);//配置32/2定时器 ，在定时器使能之后作用
//	//ROM_TimerPrescaleSet(WTIMER0_BASE, TIMER_A,2);//分频,这个函数值工作在半角模式
//	ROM_TimerLoadSet(WTIMER0_BASE,TIMER_A, 100000000);//定时器时钟配置,最后一个参数是设置装载值	
//	
//	ROM_IntDisable(INT_WTIMER0A);//使能中断
//	ROM_TimerIntEnable(WTIMER0_BASE, TIMER_TIMA_TIMEOUT);//计时结束使能
//	TimerIntRegister(WTIMER0_BASE,TIMER_A,TIMERA_IRQHandler);//写入口函数
//	IntPrioritySet(INT_WTIMER0A,1);  //优先级的控制
//	ROM_TimerEnable(WTIMER0_BASE, TIMER_A);//定时器使能
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







