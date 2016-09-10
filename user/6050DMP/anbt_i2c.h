/*****************************************************************************************
圆点博士STM32蓝牙4.0最小系统板AnBT库文件和例程源代码。和圆点博士STM32无线下载助手软件兼容。
该例程适用LGPL协议。用户可以自由转载使用该代码，但不得移除该部分版权信息
更多信息，请访问官方网站www.etootle.com
******************************************************************************************/
//
#include "includes.h"
#include <stdint.h>

	
#define AnBT_MPU6050_INT			GPIO_PIN_0		//PB3
#define AnBT_MPU6050_INT_PORT	GPIOB
//
#define ANBT_MPU6050_INT_STATE   GPIOPinRead(AnBT_MPU6050_INT_PORT, AnBT_MPU6050_INT)
//
#define ANBT_I2C_SDA 		GPIO_PIN_3 
#define ANBT_I2C_SCL 		GPIO_PIN_2
#define ANBT_I2C_PORT   GPIO_PORTB_BASE
//
#define ANBT_I2C_SCL_0 		GPIOPinWrite(GPIO_PORTB_BASE, ANBT_I2C_SCL, 0)
#define ANBT_I2C_SCL_1 		GPIOPinWrite(GPIO_PORTB_BASE, ANBT_I2C_SCL, ANBT_I2C_SCL)
#define ANBT_I2C_SDA_0 		sda_down()
#define ANBT_I2C_SDA_1   	sda_up()
//
#define ANBT_I2C_SDA_STATE   	Read_sda()
#define ANBT_I2C_DELAY 				ROM_SysCtlDelay(100000);//ANBT_I2C_Delay(100000)
#define ANBT_I2C_NOP					ROM_SysCtlDelay(100);//ANBT_I2C_Delay(10) 
//
#define ANBT_I2C_READY		0x00
#define ANBT_I2C_BUS_BUSY	0x01	
#define ANBT_I2C_BUS_ERROR	0x02
//
#define ANBT_I2C_NACK	  0x00 
#define ANBT_I2C_ACK		0x01
//
u8 Read_sda(void);
void sda_up(void);
void sda_down(void);
void ANBT_I2C_Configuration(void);
void ANBT_I2C_Delay(u32 dly);
u8 ANBT_I2C_START(void);
void ANBT_I2C_STOP(void);
void ANBT_I2C_SendACK(void);
void ANBT_I2C_SendNACK(void);
u8 ANBT_I2C_SendByte(u8 anbt_i2c_data);
u8 ANBT_I2C_ReceiveByte_WithACK(void);
u8 ANBT_I2C_ReceiveByte(void);
void ANBT_I2C_Receive12Bytes(u8 *anbt_i2c_data_buffer);
void ANBT_I2C_Receive6Bytes(u8 *anbt_i2c_data_buffer);
void ANBT_I2C_Receive14Bytes(u8 *anbt_i2c_data_buffer);
u8 AnBT_DMP_I2C_Write(u8 anbt_dev_addr, u8 anbt_reg_addr, u8 anbt_i2c_len, u8 *anbt_i2c_data_buf);
u8 AnBT_DMP_I2C_Read(u8 anbt_dev_addr, u8 anbt_reg_addr, u8 anbt_i2c_len, u8 *anbt_i2c_data_buf);
void AnBT_DMP_Delay_us(u32 dly);
void AnBT_DMP_Delay_ms(u32 dly);


