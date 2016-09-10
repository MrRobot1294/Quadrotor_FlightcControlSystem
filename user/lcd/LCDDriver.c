#ifndef PART_TM4C123GH6PM
#define PART_TM4C123GH6PM
#endif
#include "LCDMatrix.h"
#include "includes.h"
#include "LCDDriver.h"
#include "includes.h"
// 低电平片选
#define setCS  GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_PIN_1);	//PB1
#define rstCS  GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0);

// 寄存器选择信号
#define setCD  GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_PIN_6);	//PC6
#define rstCD  GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);

// 低电平复位，复位完成后，回到高电平，液晶模块开始工作
#define setRES GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_PIN_5);	//PE5
#define rstRES GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0);

/******************************************************************************
 * @brief  清屏函数，清楚所有的显示内容
 * @param
 * @return none
 ******************************************************************************/
void LCD_ScreenClr() {
	unsigned char i, j;
	for (i = 0; i < 9; i++)				//循环第0~8页面
			{
		rstCS
		LCD_TransferCmd(0xb0 + i);		//选择“页”
		LCD_TransferCmd(0x10);			//选择“列”
		LCD_TransferCmd(0x00);
		for (j = 0; j < 132; j++) {
			LCD_TransferData(0x00);		//输入数据0x00，显示0x00
		}
	}
}

/******************************************************************************
 * @brief  液晶五个端口初始化
 * @param
 * @return none
 ******************************************************************************/
void LCD_Enable() {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);					//各个端口使能
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
//	-----------------------------------------------------------------------------
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_1);		//各个端口设置为输出，PB1->CS
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6);				//PC6->CD
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_5);				//PE5->RST

//	-----------------------------------------------------------------------------

	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);						//系统外设使能
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	GPIOPinConfigure(GPIO_PD0_SSI3CLK);								//PD0，PD3两线
	GPIOPinConfigure(GPIO_PD3_SSI3TX);

	GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_3);		//SSI端口功能使能

	SSIConfigSetExpClk(SSI3_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
			SSI_MODE_MASTER, 50000, 8);	//SSI3,端口模式:50K,8位数据，SSI_FRF_MOTO_MODE_0模式

	SSIEnable(SSI3_BASE);
//  -----------------------------------------------------------------------------
}

/******************************************************************************
 * @brief  较长时间延时
 * @param
 * @return none
 ******************************************************************************/
void LCD_Delay2(unsigned int i) {
	int j, k;
	for (j = 0; j < i; j++)
		for (k = 0; k < 990; k++)
			;
}

/******************************************************************************
 * @brief  较短时间延时
 * @param
 * @return none
 ******************************************************************************/
void LCD_Delay3(unsigned int i) {
	int j, k;
	for (j = 0; j < i; j++)
		for (k = 0; k < 10; k++)
			;
}

/******************************************************************************
 * @brief  	显示8*8的矩阵像素
 * @param	data ：输入的显示数据指针
 * @param	type ：是否需要反相显示
 * @return  none
 ******************************************************************************/
void LCD_TransferData(unsigned char data) {

	rstCS
	setCD

	while (SSIBusy(SSI3_BASE))
		;
	SSIDataPut(SSI3_BASE, data);
	while (SSIBusy(SSI3_BASE))
		;
}

/******************************************************************************
 * @brief  	发送液晶控制命令
 * @param	command ：命令字符
 * @return  none
 ******************************************************************************/
void LCD_TransferCmd(unsigned char command) {

	rstCS
	rstCD

	while (SSIBusy(SSI3_BASE))
		;
	SSIDataPut(SSI3_BASE, command);
	while (SSIBusy(SSI3_BASE))
		;
}

/******************************************************************************
 * @brief  	初始化液晶
 * @param	none
 * @return  none
 ******************************************************************************/
void LCD_Init() {
	LCD_Enable();
	rstRES
	LCD_Delay2(50);                 //delay
	setRES
	LCD_Delay2(50);                 //delay
	LCD_TransferCmd(0xe2); /*软复位*/
	LCD_TransferCmd(0x2c); /*升压步聚1*/
	LCD_TransferCmd(0x2e); /*升压步聚2*/
	LCD_TransferCmd(0x2f); /*升压步聚3*/
	LCD_TransferCmd(0x23); /*粗调对比度，可设置范围20～27*/
	LCD_TransferCmd(0x81); /*微调对比度*/
	LCD_TransferCmd(0x28); /*微调对比度的值，可设置范围0～63*/
	LCD_TransferCmd(0xa2); /*1/9 偏压比（bias）*/
	LCD_TransferCmd(0xc8); /*行扫描顺序：从上到下c0 c8 */
	LCD_TransferCmd(0xa0); /*列扫描顺序：从左到右 a0 a1*/
	LCD_TransferCmd(0xa6); /*正显 0xa6，反显 0xa7*/
	LCD_TransferCmd(0xaf); /*开显示*/

}

/******************************************************************************
 * @brief  	对液晶的8*16字模矩阵做顺时针90度的旋转，来贴合液晶的显示模式
 * @param	c	需要显示的数字或字符
 * @param	row	字符显示的行数 取值范围0~8
 * @param	col	字符显示的列数 取值范围0~21
 * @param	IsReverse	字符是否翻转显示
 * @return  0 ， 表示输入参数出错
 *			1，	 表示显示成功
 ******************************************************************************/
unsigned char LCD_Draw_Char_8x16(char c, unsigned char row, unsigned char col) 
{
	const unsigned char * add = asc + (c - 0x20) * 16;
	unsigned int i, j;							//计数器
	unsigned char col_0 = col;					// 低四位
	unsigned char col_1 = col >> 4;  			// 高四位
	add = asc + (c - 0x20) * 16;
	//----------------输入检测-------------
	//--------------返回0表示输入出错---------------
	if (col > LCD_MAX_COL - 1) {
		return 0;
	}
	if (row > LCD_MAX_RAW - 1) {
		return 0;
	}

	for (i = 0; i < 2; i++) {
		rstCS
		LCD_TransferCmd(0xb0 + row * 2 + i);            //设置“页”
		LCD_TransferCmd(0x10 + (col_1 & 0x0f));			//设置“列”以及列的高四位
		LCD_TransferCmd(0x00 + (col_0 & 0x0f));			//设置“列”以及列的低四位
		for (j = 0; j < 8; j++) {
			LCD_TransferData(*add);
			add++;
		}
	}

	return 1; 								//返回1表示执行成功！
}

/******************************************************************************
 * @brief  	对液晶的8*8字模矩阵做顺时针90度的旋转，来贴合液晶的显示模式
 * @param	c	需要显示的数字或字符
 * @param	row	字符显示的行数 取值范围0~8
 * @param	col	字符显示的列数 取值范围0~21
 * @return  0 ， 表示输入参数出错
 *			1， 表示显示成功
 ******************************************************************************/
unsigned char LCD_Draw_Char_8x8(char c, unsigned char row, unsigned char col,
		bool IsReverse) {

	const unsigned char * add = acFont8ASCII[c - 0x20];
	unsigned int i, j;							//计数器
	unsigned char col_1 = col * CHAR_WIDTH >> 4;  			// 高四位
	unsigned char col_0 = col * CHAR_WIDTH;  				// 高四位
	unsigned char data_ori[NUM_CHAR][NUM_CHAR] = { 0 };
	unsigned char data_cha[NUM_CHAR][NUM_CHAR] = { 0 };
	unsigned char show_chr[NUM_CHAR] = { 0 };
	unsigned char ori_chr[NUM_CHAR] = { 0 };
	//----------------输入检测-------------
	//--------------返回0表示输入出错---------------
	if (col > LCD_MAX_COL) {
		return 0;
	}
	if (row > ENUM_LINE_NUM) {
		return 0;
	}

	rstCS
	LCD_TransferCmd(0xb0 + row);            		//设置“页”
	LCD_TransferCmd(0x10 + (col_1 & 0x0f));			 //设置“列”以及列的高四位
	LCD_TransferCmd(0x00 + (col_0 & 0x0f));			 //设置“列”以及列的低四位

	for (i = 0; i < NUM_CHAR; i++) {
		ori_chr[i] = *add;
		for (j = 0; j < NUM_CHAR; j++) {
			data_ori[i][NUM_CHAR - j] = (char) (ori_chr[i] >> j) & 0x01;
		}
		add++;
	}

	for (i = 0; i < NUM_CHAR; i++) {
		for (j = 0; j < NUM_CHAR; j++) {
			data_cha[j][i] = data_ori[i][j];
		}
	}

	for (i = 0; i < NUM_CHAR; i++) {
		unsigned char temp = 0;
		for (j = 0; j < NUM_CHAR; j++) {
			if (IsReverse) {
				if (data_cha[i][j] == 0)
					temp = temp | (0x01 << j);
				else
					temp = temp | (0x00 << j);
			} else {
				if (data_cha[i][j] == 1)
					temp = temp | (0x01 << j);
				else
					temp = temp | (0x00 << j);
			}
		}
		show_chr[i] = temp;
	}

	for (j = 0; j < NUM_CHAR; j++) {
		LCD_TransferData(show_chr[j]);
//		add++;
	}

	return 1; 								//返回1表示执行成功！
}

/******************************************************************************
 * @brief  	显示一串8*8大小的字符串
 * @param	c	需要显示的数字或字符
 * @param	raw	字符显示的行数 取值范围0~8
 * @param	col	字符显示的列数 取值范围0~21
 * @param	IsReverse	字符是否翻转显示
 * @return  0 ， 表示输入参数出错
 *			1，	 表示显示成功
 ******************************************************************************/
unsigned char LCD_Draw_String(char * c, unsigned int raw, unsigned int col,
		bool IsReverse) {
	int i = 0;

	int len = strlen(c);

	for (i = 0; i < len; i++) {
		LCD_Draw_Char_8x8(*c, raw, (col + i), IsReverse);
		c++;
	}
	return 0;
}


/******************************************************************************
 * @brief  	显示一个浮点型的数字
 * @param	c	需要显示的数字或字符
 * @param	raw	字符显示的行数 取值范围0~8
 * @param	col	字符显示的列数 取值范围0~21
 * @param	IsReverse	字符是否翻转显示
 * @return  0 ， 表示输入参数出错
 *		   len:返回数据长度值，该值在两次数据显示确定清除位时使用
 ******************************************************************************/
unsigned char LCD_Draw_Num(float number, int scale, unsigned int raw,
		unsigned int col, bool IsReverse) {
	

	//通过小数点保留位数算所乘系数
	unsigned char zero = '0';

	int show_number = 0;
	int temp = show_number;
	int Cur_len = 0;					//当前数据长度
	int multi = 1;
	int i = 0;
			
	if (scale > 10 || scale < 0)
	{
		return 0;
	}
	if(scale > 0 ){

		for (i = 0; i < scale; i++) {
			multi *= 10;
		}
	}else{
		multi = 1;
	}
		// 计算显示的数字位数
	show_number = (int) (number * multi);
	temp = show_number;
	for (i = 0; i < 10; i++) {
		if (i == 9)	// 数据过大返回
			return 0;

		temp = temp / 10;
		Cur_len++;
		if (temp == 0) {
			break;
		}
	}

	//处理小于1的数据的正常显示，能处理最小数值0.001.
	if(number < 1 && number > 0.1f)
	{
		Cur_len = Cur_len + 1;
	}
	else if(number < 0.1f && number > 0.01f)
	{
		Cur_len = Cur_len + 2;
	}
	else if(number < 0.01f && number > 0.001f)
	{
		Cur_len = Cur_len + 3;
	}

	//显示数字
	for (i = Cur_len - 1; i >= 0; i--) {

		if (scale == 0)		//显示整数
		{

			LCD_Draw_Char_8x8(zero + show_number % 10, raw, (col + i), IsReverse);
		} else {				//带小数点显示
			if (i < Cur_len - scale) {
				LCD_Draw_Char_8x8(zero + show_number % 10, raw, (col + i),
						IsReverse);
			} else {
				LCD_Draw_Char_8x8(zero + show_number % 10, raw, (col + i + 1),
						IsReverse);
			}

		}
		show_number = show_number / 10;
	}

	if (scale != 0 && Cur_len > scale)
		LCD_Draw_Char_8x8('.', raw, (col + Cur_len - scale), IsReverse);

	if(scale != 0)
	{
		Cur_len = Cur_len + 1;
	}
	return Cur_len;
}

void LCD_Draw_Line_Clear(int line, bool IsReverse) {
	unsigned char j;

	rstCS
	LCD_TransferCmd(0xb0 + line);		//选择“页”
	LCD_TransferCmd(0x10);				//选择“列”
	LCD_TransferCmd(0x00);
	for (j = 0; j < 132; j++) {
		if (IsReverse)
			LCD_TransferData(0xFF);		//输入数据0xFF，显示0xFF
		else
			LCD_TransferData(0x00);		//输入数据0x00，显示0x00
	}
}

void LCD_Draw_Clear_8x8(unsigned int page,unsigned int col)
{
	unsigned char j;
	unsigned char col_1 = col * CHAR_WIDTH >> 4;  			// 高四位
	unsigned char col_0 = col * CHAR_WIDTH;  				// 高四位
	rstCS
	LCD_TransferCmd(0xb0 + page);		//选择“页”
	LCD_TransferCmd(0x10+(col_1 & 0x0f));		//设置“列”以及列的高四位
	LCD_TransferCmd(0x00+(col_0 & 0x0f));		//设置“列”以及列的低四位
	for (j = 0; j < NUM_CHAR; j++) {
			LCD_TransferData(0x00);		//输入数据0x00，显示0x00
		}
}

/******************************************************************************
 * @brief  	对液晶的8*8字模矩阵做顺时针90度的旋转，来贴合液晶的显示模式
 * @param	index	需要显示的汉字的字模编号
 * @param	row	字符显示的行数 ,取值范围0~3
 * @param	col	字符显示的列数 ,取值范围0~119
 * @return  0 ， 表示输入参数出错
 *			1， 表示显示成功
 ******************************************************************************/
unsigned char LCD_Draw_Chinese(unsigned char index, unsigned char row,
		unsigned char col) {
	const unsigned char * chinese = CHINESE_MATRIX + 32 * index;
	unsigned int i, j;					//计数器
	unsigned char col_0 = col;		//列低四位
	unsigned char col_1 = col >> 4;  //列高四位

	//----------------输入检测-------------
	//-----------返回0表示输入出错------------
	if (col > 111) {
		return 0;
	}
	if (row > 3) {
		return 0;
	}

	for (i = 0; i < 2; i++) {
		rstCS
		LCD_TransferCmd(0xb0 + row);             //设置“页”
		LCD_TransferCmd(0x10 + (col_1 & 0x0f));			 //设置“列”以及列的高四位
		LCD_TransferCmd(0x00 + (col_0 & 0x0f));			 //设置“列”以及列的低四位
		for (j = 0; j < 16; j++) {
			LCD_TransferData(*chinese);
			chinese++;
		}
	}
	return 1; //返回1表示执行成功！
}

/************************************************************************
 * 初始化ADC获取滚轮电压值
 * 		————————
 * 				|
 //			 PC7|<--Button1
 * 	TIVA	 PD6|<--Button2
 * 			 PD7|<--Button3
 //		________|
 *
 *  @return	0x00
 *  		0x01
 *  		0x02
 ***********************************************************************/
unsigned char scan_key(void) {
	int32_t ping1 = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6);
	int32_t ping2 = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7);
	int32_t ping3 = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_7);
	if (GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_7) == 0x00) {

		SysCtlDelay(10 * (SysCtlClockGet() / 3000)); 		// 延时约10ms，消除按键抖动
		while (GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_7) == 0x00)
			; 		// 等待KEY抬起
		SysCtlDelay(10 * (SysCtlClockGet() / 3000)); 		// 延时约10ms，消除松键抖动

		// 	do something

		return 0x01;
	}

	if (GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6) == 0x00) {
		SysCtlDelay(10 * (SysCtlClockGet() / 3000)); 		// 延时约10ms，消除按键抖动
		while (GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6) == 0x00)
			; 		// 等待KEY抬起
		SysCtlDelay(10 * (SysCtlClockGet() / 3000)); 		// 延时约10ms，消除松键抖动

		// 	do something

		return 0x02;
	}


	return 0;
}


