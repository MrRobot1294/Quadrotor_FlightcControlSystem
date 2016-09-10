
#ifndef LCDDRIVER_H_
#define LCDDRIVER_H_
//###############################################################################################
// 头文件声明
#include "hw_memmap.h"
#include "hw_types.h"
#include "gpio.h"
#include "sysctl.h"
#include "rom.h"
#include "rom_map.h"

//宏定义
#define LCD_DRAW_NORMAL				0		// 正常显示
#define LCD_DRAW_HIGHLIGHT			1		// 背光显示
#define LCD_MAX_RAW					8		// 液晶最大行数
#define NUM_CHAR					8
#define CHAR_WIDTH					7
#define NUM_WIDTH					7
#define LCD_MAX_COL					17		// 液晶最大列数
#define WORD_WIDTH					16
//------------------------------------------------------------------------------------------
//液晶显示相关变量的定义
enum
{
	ENUM_LINE_ONE,
	ENUM_LINE_TWO,
	ENUM_LINE_THREE,
	ENUM_LINE_FOUR,
	ENUM_LINE_FIVE,
	ENUM_LINE_SIX,
	ENUM_LINE_SEVEN,
	ENUM_LINE_EIGHT,

	ENUM_LINE_NUM
};


enum
{
	DIRECTOR_RAW,
	DIRECTOR_COL,

	DIRECTOR_NUM
};//director_define;

enum
{
	SSI1_UNITS = 101,
	SSI1_TENS = 85,
	SSI1_HUNDREDS = 69,
	SSI1_THOUSANDS = 53
};//cursor_define_for_MDAC;


// LCD清屏
void LCD_ScreenClr(void);

// 使能LCD所用到的端口
void LCD_Enable(void);

// 延时
void LCD_Delay2(unsigned int i);

// 延时
void LCD_Delay3(unsigned int i);

// 显示字模
void LCD_TransferData(unsigned char data);

// LCD发送指令
void LCD_TransferCmd(unsigned char command);

// LCD初始化
void LCD_Init(void);

// 显示8*16字模
unsigned char LCD_Draw_Char_8x16(char c,unsigned char page,unsigned char col);

// 显示8*8字模
unsigned char LCD_Draw_Char_8x8(char c, unsigned char page,unsigned char col, bool IsReverse);

// 显示字符串
unsigned char LCD_Draw_String(char * c, unsigned int page,unsigned int col, bool IsReverse);

// 显示数字
unsigned char LCD_Draw_Num(float number, int scale, unsigned int page, unsigned int col, bool IsReverse);

void LCD_Draw_Line_Clear(int line, bool IsReverse);

void LCD_Draw_Clear_8x8(unsigned int page,unsigned int col);

// 显示8*8矩阵字模
void LCD_Draw_Byte(bool b_invert);

// 显示汉字
unsigned char LCD_Draw_Chinese(unsigned char index, unsigned char raw, unsigned char col);

// 整屏翻转设置
void LCD_InvertLight_All(void);

// 全屏显示图形
void DisplayGraphic1(void);

void Init_Int_Key(void);

void Init_ADCWheel(void);

unsigned char scan_key(void);

#endif
//-------------------------------------------------------------------------
