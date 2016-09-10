#include "camearfindway.h"
//将摄像头的数据引脚初始化
//使用了PA3 PA4 PA5 PA6,开漏输出，输入模式

u8 direction_flag=0x00;
extern float DESIRE_ROLL;
extern float DESIRE_PITCH;
extern float DESIRE_YAW;


void camear_gpio_init(void){
	all_gpio_pin_unlock();//解锁引脚
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	ROM_GPIODirModeSet(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD_WPU);	
//  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2);
//  GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_PIN_2); 

	//ROM_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2);
}

u8 read_first_gpio(void){
  return GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2);
}

u8 read_second_gpio(void){
  return GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_3);
}

u8 read_third_gpio(void){
  return GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4);
}

u8 read_forth_gpio(void){
  return GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5);
}


void findway(void){
	u8 direction=0xff;
  if(read_first_gpio()==0){
		led1_on();
	  direction&=0xfe;
	}
	if(read_second_gpio()==0){
	  direction&=0xfd;
		led2_on();
	}
	if(read_third_gpio()==0){
	  direction&=0xfb;
		led3_on();
	}
	if(read_forth_gpio()==0){
	  direction&=0xf7;
		led1_on();
		led2_on();
		led3_on();
	}
//已经获取到了方向数据,进行姿态控制
	switch(direction){
	  case 0xff: DESIRE_PITCH+=0.1f;led1_on();break;
		case 0xfe: DESIRE_PITCH-=0.1f;led1_off();break;
		case 0xfd: DESIRE_ROLL+=0.1f;led2_on();break;
		case 0xfc: DESIRE_ROLL-=0.1f;led2_off();break;
		case 0xfb: DESIRE_PITCH+=0.1f;DESIRE_ROLL+=0.1f;led1_on();led2_on();break;
		case 0xfa: DESIRE_PITCH+=0.1f;DESIRE_ROLL-=0.1f;led1_on();led2_off();break;
		case 0xf9: DESIRE_PITCH-=0.1f;DESIRE_ROLL+=0.1f;led1_off();led2_on();break;
		case 0xf8: DESIRE_PITCH-=0.1f;DESIRE_ROLL-=0.1f;led1_off();led2_off();break;
		case 0xf7: break;
	}
	direction_flag=direction;//全局的姿态
}




