#include "led.h"
#include "includes.h"
void led_init(void){  //Õâ¸öº¯ÊýµÄÒâÒåÏàµ±ÓÚÊ¹ÄÜGPIOF2Òý½Å
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//Ïàµ±ÓÚÊ¹ÄÜÄÄ¸ö¶Ë¿ÚGPIOF
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD);//ÅäÖÃ³ÉÁËÍÆÍìÊä³öÄ¬ÈÏÒ²ÊÇ
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);//ÍùµØÖ·ÀïÃæÐ´Èë£¬£¬Òý½ÅÊÇGPIO_PIN_2
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
}
//µÚÒ»¸öledµÆ¿ªÆô
void led1_on(void){
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); //ÏòÄ³¸öÒý½ÅÀïÃæÐ´ÈëÖµ£
}

void led1_off(void){
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); 
}
//µÚ¶þ¸öledµÆµÄ³õÊ¼»¯
void led2_on(void){
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); //ÏòÄ³¸öÒý½ÅÀïÃæÐ´ÈëÖµ£
}

void led2_off(void){
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); 
}
//µÚÈý¸öledµÆµÄ³õÊ¼»¯
void led3_on(void){
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); //ÏòÄ³¸öÒý½ÅÀïÃæÐ´ÈëÖµ£
}

void led3_off(void){
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0); 
}


//¶ÁÈ¡io¿ÚµÄ²âÊÔº¯Êý
void test_led(void){
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//Ïàµ±ÓÚÊ¹ÄÜÄÄ¸ö¶Ë¿ÚGPIOF
	GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_0,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_OD);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0);//ÍùµØÖ·ÀïÃæÐ´Èë£¬£¬Òý½ÅÊÇGPIO_PIN_0
}

//Èç¹ûÒ»¸öio¿ÚÐèÒª·´¸´µÄ¶ÁÈ¡ºÍÊä³ö£¬¾ÍÒª²»¶ÏµÄÉèÖÃÆäÊäÈëÊä³öµÄ×´Ì¬£¬ÕâÒ»µãÔÚDMPµÄI2CµÄÅäÖÃÖÐºÜÖØÒª
u8 read_pin(void){
	ROM_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0);
  return GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0);
}


