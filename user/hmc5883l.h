#ifndef __HMC5883L_H
#define __HMC5883L_H
#include "includes.h"
//¼Ä´æÆ÷
#define HMC5883L_WRITE_ADDR 0X3C
#define HMC5883L_READ_ADDR  0X3D

#define CONFIG_A_REGISTER        0X00
#define CONFIG_B_REGISTER        0X01
#define MODE_REGISTER            0X02
#define X_OUTPUT_MSB             0X03
#define X_OUTPUT_LSB             0X04
#define Y_OUTPUT_MSB             0X05
#define Y_OUTPUT_LSB             0X06
#define Z_OUTPUT_MSB             0X07
#define Z_OUTPUT_LSB             0X08
#define STATE_REGISTER           0X09
#define RECOGNISE_A_REGISTER     0X10
#define RECOGNISE_B_REGISTER     0X11
#define RECOGNISE_C_REGISTER     0X12
// 
#define HMC5883L_DRDY			GPIO_PIN_3		//PB3
#define HMC5883L_DRDY_PORT	GPIO_PORTE_BASE
#define HMC5883L_DRDY_STATE   GPIOPinRead(HMC5883L_DRDY_PORT, HMC5883L_DRDY)

//
#define HMC5883L_I2C_SDA 		GPIO_PIN_2	 
#define HMC5883L_I2C_SCL 		GPIO_PIN_1
#define HMC5883L_I2C_PORT   GPIO_PORTE_BASE
//
#define HMC5883L_I2C_SCL_0 		GPIOPinWrite(GPIO_PORTE_BASE, HMC5883L_I2C_SCL, 0)
#define HMC5883L_I2C_SCL_1 		GPIOPinWrite(GPIO_PORTE_BASE, HMC5883L_I2C_SCL, HMC5883L_I2C_SCL)
#define HMC5883L_I2C_SDA_0 		HMC5883L_sda_down()
#define HMC5883L_I2C_SDA_1   	HMC5883L_sda_up()
//
#define HMC5883L_I2C_SDA_STATE   	HMC5883L_Read_sda()
#define HMC5883L_I2C_DELAY 				delay_us(6);//HMC5883L_I2C_Delay(100000)
#define HMC5883L_I2C_NOP					delay_ms(10);//HMC5883L_I2C_Delay(10) 
//
#define HMC5883L_I2C_READY		0x00
#define HMC5883L_I2C_BUS_BUSY	0x01	
#define HMC5883L_I2C_BUS_ERROR	0x02
//
#define HMC5883L_I2C_NACK	  0x00 
#define HMC5883L_I2C_ACK		0x01

void HMC5883L_I2C_Configuration(void)	;
u8 HMC5883L_Read_sda(void);
void HMC5883L_sda_down(void);
void HMC5883L_sda_up(void);
u8 HMC5883L_I2C_START(void);
void HMC5883L_I2C_STOP(void);
void HMC5883L_I2C_SendACK(void);
void HMC5883L_I2C_SendNACK(void);
u8 HMC5883L_I2C_SendByte(u8 HMC5883L_i2c_data);
u8 HMC5883L_I2C_ReceiveByte(void);
u8 HMC5883L_I2C_ReceiveByte_WithACK(void);
void HMC5883L_I2C_Receive6Bytes(u8 *HMC5883L_i2c_data_buffer);

void HMC5883L_write_reg(u8 reg_arr,u8 reg_data);
void HMC5883_ReadData(u8 reg_add,unsigned char*Read);
void HMC5883_INIT(void);
float GET_ANGLE(void);

#endif


