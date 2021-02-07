#ifndef __USER_OLED_H__
#define __USER_OLED_H__


#ifdef USER_OLED_GLOBALS
#define USER_OLED_EXT
#else
#define USER_OLED_EXT extern 
#endif



#include "codetab.h"

#define Max_Column	128
/*****************************************************************************
函数名称 : OLED_WR_Byte
功能描述 : OLED写一字节
输入参数 : dat：要写入的数据 cmd=1写指令，cmd=0写数据
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void OLED_WR_Byte(unsigned dat,unsigned cmd);
/*****************************************************************************
函数名称 : OLED_OLED_WR_Byte_con
功能描述 : 连续写入count个数据
输入参数 : dat[]：要写入的数据  count写入的个数
返回参数 : 无
使用说明 : 无
*****************************************************************************/ 
void OLED_OLED_WR_Byte_con(uint8_t dat[], uint16_t count);
/*****************************************************************************
函数名称 : OLED_Init
功能描述 : OLED初始化
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void OLED_Init();
/*****************************************************************************
函数名称 : OLED_Clear
功能描述 : OLED清屏
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void OLED_Clear(void);
/*****************************************************************************
函数名称 : OLED_SetPos
功能描述 : 设置开始的光标位置
输入参数 : x，y：位置
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void OLED_SetPos(uint8_t x, uint8_t y);
/*****************************************************************************
函数名称 : OLED_ShowChar
功能描述 : 显示6*8或8*16一个标准ASCII字符串
输入参数 : x：起始列
					 y：起始页，TextSize = 16占两页；TextSize = 8占1页
					 ch：字符
返回参数 : 无
使用说明 : 无
*****************************************************************************/ 
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t ch, uint8_t TextSize);
/*****************************************************************************
函数名称 : OLED_ShowStr
功能描述 : 显示字符串
输入参数 : x：起始列
					 y：起始页
					 *ch：第一个字符首地址
           TextSize：字符大小(1:6*8 ; 2:8*16)
返回参数 : 无
使用说明 : 无
*****************************************************************************/ 
void OLED_ShowStr(uint8_t x, uint8_t y, uint8_t ch[], uint8_t TextSize);
/*****************************************************************************
函数名称 : OLED_ShowNum
功能描述 : 在指定的位置，显示一个指定长度大小的数
输入参数 : x：起始列
					 y：起始页
					 num：数字
					 len：数字的长度
					 TextSize：字符大小(1:6*8 ; 2:8*16)
返回参数 : 无
使用说明 : 无
*****************************************************************************/ 
void OLED_ShowNum(uint8_t x,uint8_t y,int num,uint8_t len,uint8_t size2);
/*****************************************************************************
函数名称 : OLED_ShowCHinese
功能描述 : 显示中文
输入参数 : x：起始列，一个字占16列 y：起始页，一个字占两页 no：字体的序列号
返回参数 : 无
使用说明 : 无
*****************************************************************************/ 
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
/*****************************************************************************
函数名称 : OLED_DrawBMP
功能描述 : 显示图片
输入参数 : x0：起始列地址（0~127）
					 y0：起始页地址（0~7）
					 x1：终止列地址（1~128）
					 y1：终止页地址（1~8）
				   BMP[]：存放图片代码的数组
返回参数 : 无
使用说明 : 无
*****************************************************************************/ 
void OLED_DrawBMP(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t BMP[]);



#endif

