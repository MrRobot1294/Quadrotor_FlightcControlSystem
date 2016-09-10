#include "my_usart.h"
#include "includes.h"
//程序测试完毕这一段可以使用串口进行发送和接收，可以使用蓝牙进行控制。但是如何配置一个中断类型的
//串口程序才是所需要的，下一个任务外部中断
//串口的配置
//这个串口的使能比较符合常规，使能IO口,使能复用功能uart。
u8 bluetooth_data=0;//
void ConfigureUART(void)
{
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);// Enable the GPIO Peripheral used by the UART.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);// Enable UART0
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);// Configure GPIO Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);// Use the internal 16MHz oscillator as the UART clock source.
    UARTStdioConfig(0, 115200, 16000000);// Initialize the UART for console I/O.
}

//串口发送函数      UARTprintf("Hello, world!\n");
//串口接收数据      unsigned char UARTgetc(void)
//使用中断接收数据
void uart_handler(void){
	  if(ROM_UARTIntStatus(UART0_BASE,true)==UART_INT_RT){
			ROM_UARTIntClear(UART0_BASE,UART_INT_RT);  
			bluetooth_data=ROM_UARTCharGet(UART0_BASE);
			//bluetooth_data=UARTgetc();
	 }
		
		
}

void Uart_init(void){
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);// Enable the GPIO Peripheral used by the UART.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);// Enable UART0
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);// Configure GPIO Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	  GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_0 ,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_OD);
	  GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_1,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_OD);
	
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);// Use the internal 16MHz oscillator as the UART clock source.
    UARTStdioConfig(0, 115200, 16000000);// Initialize the UART for console I/O.  
	  UARTParityModeSet(UART0_BASE,UART_CONFIG_PAR_NONE);	//无奇数偶数校验位
    
	  UARTFIFOEnable(UART0_BASE);//使能fifo
	  UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8,UART_FIFO_RX1_8);	
	  UARTFIFODisable(UART0_BASE);
	
	  UARTIntRegister(UART0_BASE,uart_handler);
	  //ROM_UARTIntEnable(UART0_BASE,UART_INT_RX); 
	  ROM_UARTIntEnable(UART0_BASE,UART_INT_RT); //配置的中断是接收fifo不是0的中断
	  ROM_UARTEnable(UART0_BASE);
	  //ROM_UARTConfigSetExpClk(UART0_BASE,16000000,9600,UART_CONFIG_WLEN_8);
}





int fputc(int ch,FILE *F){
  while(UARTBusy(UART0_BASE)==true);
	UARTCharPutNonBlocking(UART0_BASE,(unsigned int)(ch));
  return ch;
}











