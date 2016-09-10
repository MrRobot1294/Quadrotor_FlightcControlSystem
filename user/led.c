#include "led.h"
#include "includes.h"
void led_init(void){  //��������������൱��ʹ��GPIOF2����
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//�൱��ʹ���ĸ��˿�GPIOF
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD);//���ó����������Ĭ��Ҳ��
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);//����ַ����д�룬��������GPIO_PIN_2
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
}
//��һ��led�ƿ���
void led1_on(void){
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); //��ĳ����������д��ֵ�
}

void led1_off(void){
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); 
}
//�ڶ���led�Ƶĳ�ʼ��
void led2_on(void){
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); //��ĳ����������д��ֵ�
}

void led2_off(void){
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); 
}
//������led�Ƶĳ�ʼ��
void led3_on(void){
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); //��ĳ����������д��ֵ�
}

void led3_off(void){
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0); 
}


//��ȡio�ڵĲ��Ժ���
void test_led(void){
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//�൱��ʹ���ĸ��˿�GPIOF
	GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_0,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_OD);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0);//����ַ����д�룬��������GPIO_PIN_0
}

//���һ��io����Ҫ�����Ķ�ȡ���������Ҫ���ϵ����������������״̬����һ����DMP��I2C�������к���Ҫ
u8 read_pin(void){
	ROM_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0);
  return GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0);
}


