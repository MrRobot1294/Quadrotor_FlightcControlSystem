#include "system_ti.h"
#include "includes.h"
/*
该函数是嵌入到启动文件里面的，如果没有这个函数，工程将会无法编译成功。当系统复位之后会进入到这个程序
中调用SystemInit函数，在这个函数里面我启动了浮点预算单元，以及系统时钟的设置。
当前的系统时钟已经设置成为50mhz，请不要随意更改这个分频值，否则会引起相关延时程序的异常，包括I2C的异常
这个头文件里面做了几件事：
1.定时器0的中断初始化程序
2.延时程序（调用了系统的库函数进行简单的延时）
*/
//复位中断,启动文件里有这个函数的指针其他地方没有实现，我在这边实现浮点数的计算
//这里面可以做几件事
extern u16 ms5;
extern int fly_time;//飞机飞行时间长度设定
extern u16 ms200;
extern u16 ms100;


void SystemInit(){
  FPUEnable();
	FPULazyStackingEnable();//开启浮点数运算单元
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                       SYSCTL_OSC_MAIN);//使用pll的时钟，2分频100Mhz
}

////当系统10分频的时候，中断每次的点是时间是2.5us，所以在中断服务函数中有400000的技术达到1s
//整个系统的周期将在这个函数里面进行设置
void systime_handler(void){
  if(TimerIntStatus(TIMER0_BASE,true) == 1)//得到中断的标志位
	{			
		ms5++;
		fly_time--;
		ms200++;
		ms100++;
		TimerIntClear(TIMER0_BASE,TIMER_BOTH);	
		
	}
	
	
	
}

//定时器中断的初始化程序,使用了32位的定时器0,目前的时钟频率给了100Mhz,现在配置的是一个1ms的中断.通过逻辑分析仪
//不断的带入值来决定的
void systime_init(void){
	ROM_IntMasterEnable();
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);//定时器0
	TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);//配置32/2定时器 ，在定时器使能之后作用
//	ROM_TimerPrescaleSet(TIMER0_BASE, TIMER_A,5);//分频,这个函数值工作在半角模式
	ROM_TimerLoadSet(TIMER0_BASE,TIMER_A, (66799-1));//定时器时钟配置,最后一个参数是设置装载值	
	ROM_IntDisable(INT_TIMER0A);//使能中断
	ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);//计时结束使能
	TimerIntRegister(TIMER0_BASE,TIMER_BOTH,systime_handler);//写入口函数
	IntPrioritySet(INT_TIMER0A,1);  //优先级的控制
	ROM_TimerEnable(TIMER0_BASE, TIMER_BOTH);//定时器使能
}
//该函数将会在系统采样周期程序调用之前调用，防止计数值大于5无法满足条件进行定时器
void systime_enable(void){
  ROM_IntEnable(INT_TIMER0A);
	ROM_TimerEnable(TIMER0_BASE, TIMER_BOTH);
}



//根据T库所给出的函数所写的简单的延时函数
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





