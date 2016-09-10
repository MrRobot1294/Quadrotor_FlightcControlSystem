#include "my_usart.h"
#include "includes.h"
//������������һ�ο���ʹ�ô��ڽ��з��ͺͽ��գ�����ʹ���������п��ơ������������һ���ж����͵�
//���ڳ����������Ҫ�ģ���һ�������ⲿ�ж�
//���ڵ�����
//������ڵ�ʹ�ܱȽϷ��ϳ��棬ʹ��IO��,ʹ�ܸ��ù���uart��
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

//���ڷ��ͺ���      UARTprintf("Hello, world!\n");
//���ڽ�������      unsigned char UARTgetc(void)
//ʹ���жϽ�������
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
	  UARTParityModeSet(UART0_BASE,UART_CONFIG_PAR_NONE);	//������ż��У��λ
    
	  UARTFIFOEnable(UART0_BASE);//ʹ��fifo
	  UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8,UART_FIFO_RX1_8);	
	  UARTFIFODisable(UART0_BASE);
	
	  UARTIntRegister(UART0_BASE,uart_handler);
	  //ROM_UARTIntEnable(UART0_BASE,UART_INT_RX); 
	  ROM_UARTIntEnable(UART0_BASE,UART_INT_RT); //���õ��ж��ǽ���fifo����0���ж�
	  ROM_UARTEnable(UART0_BASE);
	  //ROM_UARTConfigSetExpClk(UART0_BASE,16000000,9600,UART_CONFIG_WLEN_8);
}





int fputc(int ch,FILE *F){
  while(UARTBusy(UART0_BASE)==true);
	UARTCharPutNonBlocking(UART0_BASE,(unsigned int)(ch));
  return ch;
}











