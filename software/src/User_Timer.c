/****************************************Copyright (c)*************************
**                               版权所有 (C), 2015-2020, 涂鸦科技
**
**                                 http://www.tuya.com
**
**--------------文件信息-------------------------------------------------------
**文   件   名: User_Timer.c
**描        述: Timer用户函数
**使 用 说 明 : 无
**
**
**--------------当前版本修订---------------------------------------------------

** 版  本: v2.0
** 日　期: 2016年3月29日
** 描　述: 1:优化代码结构
**
**-----------------------------------------------------------------------------
******************************************************************************/
#define USER_TIMER_GLOBAL
#include "include.h"

#include "wifi.h"

static unsigned long Timer_Value = 0;
static unsigned long Timer_250ms_Value = 0;
static unsigned long Timer_Sec_Value = 0;


/*****************************************************************************
函数名称 : timer0_init
功能描述 : timer0初始化
输入参数 : 无
返回参数 : 无
使用说明 : 10毫秒@11.0592MHz
*****************************************************************************/
void timer0_init(void)
{
	AUXR &= 0x7F;																																//定时器时钟12T模式
	TMOD &= 0xF0;																																//设置定时器模式
	TL0 = 0x00;																																	//设置定时初值
	TH0 = 0xDC;																																	//设置定时初值
	TF0 = 0;																																		//清除TF0标志
	TR0 = 1;																																		//定时器0开始计时
	ET0 = 1;
	//
	Timer_Value = 0;
	Timer_250ms_Value = 0;
}
/*****************************************************************************
函数名称 : timer0_isr_handle
功能描述 : timer0中断处理函数
输入参数 : 无
返回参数 : 无
使用说明 : 10毫秒@11.0592MHz
*****************************************************************************/
void timer0_isr_handle(void) interrupt 1
{
	extern uint8_t is_read_time;

	Timer_Value ++;
	if((Timer_Value % 25) == 0)
	{
		Timer_250ms_Value ++;
		if(Timer_250ms_Value % 4 == 0)
		{
			Timer_Sec_Value ++;
			if(Timer_Sec_Value % 15 == 0)
			{
				//15秒计时
        is_read_time = 1;
			}
			
			if(Timer_Sec_Value >= 60)
			{
				Timer_Sec_Value = 0;
				
				FlashBuffer.run_time ++;
				if(FlashBuffer.run_time % 30 == 0)
				{
					//保存运行时间
					Earse_Flash(PARA_ADDR);
					if(Write_Flash(PARA_ADDR,(unsigned char *)&FlashBuffer,sizeof(FlashBuffer)) == ERROR)
					{
					}
				}
			}
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
函数名称 : get_timer_250ms_value
功能描述 : 获取250ms计数值
输入参数 : 无
返回参数 : Timer_250ms_Value
使用说明 : 无
*****************************************************************************/
unsigned long get_timer_250ms_value(void)
{
	return Timer_250ms_Value;
}