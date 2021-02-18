/******************************************************************************
文件名称: User_Timer.c
文件标识: STC8A8K64S4A12
摘    要: Timer用户函数
当前版本: V1.0	
完成日期: 2021.02.06
*******************************************************************************/
#define USER_TIMER_GLOBAL
#include "include.h"

static unsigned long Timer_Value = 0;


/*****************************************************************************
函数名称 : timer0_init
功能描述 : timer0初始化
输入参数 : 无
返回参数 : 无
使用说明 : 10毫秒@22.1184MHz
*****************************************************************************/
void timer0_init(void)
{
	AUXR &= 0x7F;																																//定时器时钟12T模式
	TMOD &= 0xF0;																																//设置定时器模式
	TL0 = 0x00;																																	//设置定时初值
	TH0 = 0xB8;																																	//设置定时初值
	TF0 = 0;																																		//清除TF0标志
	TR0 = 1;																																		//定时器0开始计时
	ET0 = 1;
	//
	Timer_Value = 0;
	FlashBuffer.SAMPLING = 30;
}
/*****************************************************************************
函数名称 : timer0_isr_handle
功能描述 : timer0中断处理函数
输入参数 : 无
返回参数 : 无
使用说明 : 10毫秒@22.1184MHz
*****************************************************************************/
void timer0_isr_handle(void) interrupt 1
{
	extern uint8_t is_read_time;

	Timer_Value ++;
	if((Timer_Value % 100) == 0)
	{
		FlashBuffer.Timer_Sec_Value ++;
		if(FlashBuffer.SAMPLING <= 60)
		{
			if(FlashBuffer.Timer_Sec_Value % FlashBuffer.SAMPLING == 0)
			{
				//X秒计时
				is_read_time = 1;
			}
		}
		
		if(FlashBuffer.Timer_Sec_Value >= 60)
		{
			FlashBuffer.Timer_Sec_Value = 0;
			FlashBuffer.Timer_Min_Value ++;
			if(FlashBuffer.Timer_Min_Value % (FlashBuffer.SAMPLING/60) == 0)
			{
				//X分计时
				is_read_time = 1;
			}
		}
		
		if(FlashBuffer.Timer_Min_Value >= 60)
		{
			FlashBuffer.Timer_Min_Value = 0;
			FlashBuffer.Timer_Hour_Value ++;
		}
		
		if(FlashBuffer.Timer_Hour_Value >=24)
		{
			FlashBuffer.Timer_Hour_Value = 0;
		}
	}
}
/*****************************************************************************
函数名称 : get_count_value
功能描述 : 获取Timer0计数值
输入参数 : 无
返回参数 : Timer_Value
使用说明 : 无
*****************************************************************************/
unsigned long get_count_value(void)
{
	return Timer_Value;
}
/*****************************************************************************
函数名称 : show_time
功能描述 : 显示当前时间
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void show_time()
{
	OLED_ShowNum(40,0,FlashBuffer.Timer_Hour_Value,2,8);
	OLED_ShowChar(52,0,':',8);
	OLED_ShowNum(58,0,FlashBuffer.Timer_Min_Value,2,8);
	OLED_ShowChar(70,0,':',8);
	OLED_ShowNum(76,0,FlashBuffer.Timer_Sec_Value,2,8);
}