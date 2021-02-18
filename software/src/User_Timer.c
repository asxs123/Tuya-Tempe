/******************************************************************************
�ļ�����: User_Timer.c
�ļ���ʶ: STC8A8K64S4A12
ժ    Ҫ: Timer�û�����
��ǰ�汾: V1.0	
�������: 2021.02.06
*******************************************************************************/
#define USER_TIMER_GLOBAL
#include "include.h"

static unsigned long Timer_Value = 0;


/*****************************************************************************
�������� : timer0_init
�������� : timer0��ʼ��
������� : ��
���ز��� : ��
ʹ��˵�� : 10����@22.1184MHz
*****************************************************************************/
void timer0_init(void)
{
	AUXR &= 0x7F;																																//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;																																//���ö�ʱ��ģʽ
	TL0 = 0x00;																																	//���ö�ʱ��ֵ
	TH0 = 0xB8;																																	//���ö�ʱ��ֵ
	TF0 = 0;																																		//���TF0��־
	TR0 = 1;																																		//��ʱ��0��ʼ��ʱ
	ET0 = 1;
	//
	Timer_Value = 0;
	FlashBuffer.SAMPLING = 30;
}
/*****************************************************************************
�������� : timer0_isr_handle
�������� : timer0�жϴ�����
������� : ��
���ز��� : ��
ʹ��˵�� : 10����@22.1184MHz
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
				//X���ʱ
				is_read_time = 1;
			}
		}
		
		if(FlashBuffer.Timer_Sec_Value >= 60)
		{
			FlashBuffer.Timer_Sec_Value = 0;
			FlashBuffer.Timer_Min_Value ++;
			if(FlashBuffer.Timer_Min_Value % (FlashBuffer.SAMPLING/60) == 0)
			{
				//X�ּ�ʱ
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
�������� : get_count_value
�������� : ��ȡTimer0����ֵ
������� : ��
���ز��� : Timer_Value
ʹ��˵�� : ��
*****************************************************************************/
unsigned long get_count_value(void)
{
	return Timer_Value;
}
/*****************************************************************************
�������� : show_time
�������� : ��ʾ��ǰʱ��
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void show_time()
{
	OLED_ShowNum(40,0,FlashBuffer.Timer_Hour_Value,2,8);
	OLED_ShowChar(52,0,':',8);
	OLED_ShowNum(58,0,FlashBuffer.Timer_Min_Value,2,8);
	OLED_ShowChar(70,0,':',8);
	OLED_ShowNum(76,0,FlashBuffer.Timer_Sec_Value,2,8);
}