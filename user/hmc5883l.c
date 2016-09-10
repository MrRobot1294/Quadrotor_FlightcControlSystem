#include "hmc5883l.h"



//引脚配置
void HMC5883L_I2C_Configuration(void)			//初始化i2c的引脚
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//相当于使能哪个端口GPIOF
	GPIOPadConfigSet(GPIO_PORTE_BASE,HMC5883L_I2C_SDA|HMC5883L_I2C_SCL,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_OD);//配置开漏输出
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, HMC5883L_I2C_SDA|HMC5883L_I2C_SCL);//往地址里面写入，，引脚是GPIO_PIN_2
	//
	HMC5883L_I2C_SCL_1; 
	HMC5883L_I2C_SDA_1;
	HMC5883L_I2C_DELAY;
}

u8 HMC5883L_Read_sda(void){
	u8 data_temp=0;
  ROM_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE,HMC5883L_I2C_SDA);
	data_temp=GPIOPinRead(GPIO_PORTE_BASE, HMC5883L_I2C_SDA);
	return data_temp;
}

void HMC5883L_sda_down(void){
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,HMC5883L_I2C_SDA);
	GPIOPinWrite(GPIO_PORTE_BASE, HMC5883L_I2C_SDA, 0);//sda为0
}

void HMC5883L_sda_up(void){
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,HMC5883L_I2C_SDA);
	GPIOPinWrite(GPIO_PORTE_BASE, HMC5883L_I2C_SDA, HMC5883L_I2C_SDA);//sda为1
}

u8 HMC5883L_I2C_START(void)
{ 
	HMC5883L_I2C_SDA_1; 
 	HMC5883L_I2C_NOP;
  // 
 	HMC5883L_I2C_SCL_1; 
 	HMC5883L_I2C_NOP;    
	//
 	if(!HMC5883L_I2C_SDA_STATE) return HMC5883L_I2C_BUS_BUSY;
	//
 	HMC5883L_I2C_SDA_0;
 	HMC5883L_I2C_NOP;
  //
 	HMC5883L_I2C_SCL_0;  
 	HMC5883L_I2C_NOP; 
	//
 	if(HMC5883L_I2C_SDA_STATE) return HMC5883L_I2C_BUS_ERROR;
	//
 	return HMC5883L_I2C_READY;
}

void HMC5883L_I2C_STOP(void)
{
 	HMC5883L_I2C_SDA_0; 
 	HMC5883L_I2C_NOP;
  // 
 	HMC5883L_I2C_SCL_1; 
 	HMC5883L_I2C_NOP;    
	//
 	HMC5883L_I2C_SDA_1;
 	HMC5883L_I2C_NOP;
}

void HMC5883L_I2C_SendACK(void)
{
 	HMC5883L_I2C_SDA_0;
 	HMC5883L_I2C_NOP;
 	HMC5883L_I2C_SCL_1;
 	HMC5883L_I2C_NOP;
 	HMC5883L_I2C_SCL_0; 
 	HMC5883L_I2C_NOP;  
}

void HMC5883L_I2C_SendNACK(void)
{
	HMC5883L_I2C_SDA_1;
	HMC5883L_I2C_NOP;
	HMC5883L_I2C_SCL_1;
	HMC5883L_I2C_NOP;
	HMC5883L_I2C_SCL_0; 
	HMC5883L_I2C_NOP;  
}

u8 HMC5883L_I2C_SendByte(u8 HMC5883L_i2c_data)
{
 	u8 i;
 	
	HMC5883L_I2C_SCL_0;
 	for(i=0;i<8;i++)
 	{  
  		if(HMC5883L_i2c_data&0x80) HMC5883L_I2C_SDA_1;
   		else HMC5883L_I2C_SDA_0;
			//
  		HMC5883L_i2c_data<<=1;
  		HMC5883L_I2C_NOP;
			//
  		HMC5883L_I2C_SCL_1;
  		HMC5883L_I2C_NOP;
  		HMC5883L_I2C_SCL_0;
  		HMC5883L_I2C_NOP; 
 	}
	//
 	HMC5883L_I2C_SDA_1; 
 	HMC5883L_I2C_NOP;
 	HMC5883L_I2C_SCL_1;
 	HMC5883L_I2C_NOP;   
 	if(HMC5883L_I2C_SDA_STATE)
 	{
  		HMC5883L_I2C_SCL_0;
  		return HMC5883L_I2C_NACK;
 	}
 	else
 	{
  		HMC5883L_I2C_SCL_0;
  		return HMC5883L_I2C_ACK;  
 	}    
}

u8 HMC5883L_I2C_ReceiveByte(void)
{
	u8 i,HMC5883L_i2c_data;
	//
 	HMC5883L_I2C_SDA_1;
 	HMC5883L_I2C_SCL_0; 
 	HMC5883L_i2c_data=0;
	//
 	for(i=0;i<8;i++)
 	{
  		HMC5883L_I2C_SCL_1;
  		HMC5883L_I2C_NOP; 
  		HMC5883L_i2c_data<<=1;
			//
  		if(HMC5883L_I2C_SDA_STATE)	HMC5883L_i2c_data|=0x01; 
  
  		HMC5883L_I2C_SCL_0;  
  		HMC5883L_I2C_NOP;         
 	}
	HMC5883L_I2C_SendNACK();
 	return HMC5883L_i2c_data;
}

u8 HMC5883L_I2C_ReceiveByte_WithACK(void)
{
	u8 i,HMC5883L_i2c_data;
	//
 	HMC5883L_I2C_SDA_1;
 	HMC5883L_I2C_SCL_0; 
 	HMC5883L_i2c_data=0;
	//
 	for(i=0;i<8;i++)
 	{
  		HMC5883L_I2C_SCL_1;
  		HMC5883L_I2C_NOP; 
  		HMC5883L_i2c_data<<=1;
			//
  		if(HMC5883L_I2C_SDA_STATE)	HMC5883L_i2c_data|=0x01; 
  
  		HMC5883L_I2C_SCL_0;  
  		HMC5883L_I2C_NOP;         
 	}
	HMC5883L_I2C_SendACK();
 	return HMC5883L_i2c_data;
}

//连续读取6个数据
void HMC5883L_I2C_Receive6Bytes(u8 *HMC5883L_i2c_data_buffer)
{
	u8 i,j;
	u8 HMC5883L_i2c_data;

	for(j=0;j<5;j++)
	{
		HMC5883L_I2C_SDA_1;
		HMC5883L_I2C_SCL_0; 
		HMC5883L_i2c_data=0;
		//
		for(i=0;i<8;i++)
		{
  		HMC5883L_I2C_SCL_1;
  		HMC5883L_I2C_NOP; 
  		HMC5883L_i2c_data<<=1;
			//
  		if(HMC5883L_I2C_SDA_STATE)	HMC5883L_i2c_data|=0x01; 
  
  		HMC5883L_I2C_SCL_0;  
  		HMC5883L_I2C_NOP;         
		}
		HMC5883L_i2c_data_buffer[j]=HMC5883L_i2c_data;
		HMC5883L_I2C_SendACK();
	}
	//
	HMC5883L_I2C_SDA_1;
	HMC5883L_I2C_SCL_0; 
	HMC5883L_i2c_data=0;
	for(i=0;i<8;i++)
	{
  	HMC5883L_I2C_SCL_1;
  	HMC5883L_I2C_NOP; 
  	HMC5883L_i2c_data<<=1;
			//
  	if(HMC5883L_I2C_SDA_STATE)	HMC5883L_i2c_data|=0x01; 
  
  	HMC5883L_I2C_SCL_0;  
  	HMC5883L_I2C_NOP;         
	}
	HMC5883L_i2c_data_buffer[5]=HMC5883L_i2c_data;
	HMC5883L_I2C_SendNACK();
}

//写入寄存器数值
void HMC5883L_write_reg(u8 reg_arr,u8 reg_data){
	HMC5883L_I2C_START();
	HMC5883L_I2C_SendByte(HMC5883L_WRITE_ADDR);
	HMC5883L_I2C_SendByte(reg_arr);
  HMC5883L_I2C_SendByte(reg_data);
	HMC5883L_I2C_STOP();
}
//读取5883的值
void HMC5883_ReadData(u8 reg_add,u8 *Read)
{
	
	HMC5883L_I2C_START();
	HMC5883L_I2C_SendByte(HMC5883L_WRITE_ADDR);
	HMC5883L_I2C_SendByte(reg_add);
	
	HMC5883L_I2C_START();
	HMC5883L_I2C_SendByte(HMC5883L_READ_ADDR);
	
	HMC5883L_I2C_Receive6Bytes(Read);
	HMC5883L_I2C_STOP();
}
//5883的初始化寄存器额设置
void HMC5883_INIT(void){
  HMC5883L_I2C_Configuration();//引脚初始化
	HMC5883L_write_reg(CONFIG_A_REGISTER,0x70);
	HMC5883L_write_reg(MODE_REGISTER,0x00);//修改寄存器的值
}

float GET_ANGLE(void){
	float angle;
	u8 HMC_5883L_FIFO[6]={0,0,0,0,0,0};
	s16 hmc5883l_x,hmc5883l_y;//hmc5883l_z;
  HMC5883_ReadData(X_OUTPUT_MSB,HMC_5883L_FIFO);
	hmc5883l_x=(HMC_5883L_FIFO[0]<<8)+HMC_5883L_FIFO[1];
	hmc5883l_y=(HMC_5883L_FIFO[2]<<8)+HMC_5883L_FIFO[1];
//	hmc5883l_z=(HMC_5883L_FIFO[4]<<8)+HMC_5883L_FIFO[1];
	angle=atan2((double)hmc5883l_y,(double)hmc5883l_x) * (180 / 3.14159265)+180;
	return angle;
}






