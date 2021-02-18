/******************************************************************************
�ļ�����: ADC.c
�ļ���ʶ: STC8A8K64S4A12
ժ    Ҫ: ADCӲ����������
��ǰ�汾: V1.0.1  ��������ʾ��Ϊ�ٷֱȸ�ʽ��
�������: 2021.02.09
*******************************************************************************/
#define	USER_ADC_GLOBALS
#include "include.h"
#include "wifi.h"



//#define Interrupt //�жϷ���
#define Query     //��ѯ����




#ifdef Interrupt

/*****************************************************************************
�������� : ADC_init
�������� : ADC��ʼ��
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void ADC_init()
{
	P1M0 = 0x00;                                //����P1.0ΪADC��
	P1M1 = 0x01;
	ADCCFG = 0x2f;                              //����ADCʱ��Ϊϵͳʱ��/2/16/16
	ADC_CONTR = 0x80;                           //ʹ��ADCģ��
	EADC = 1;                                   //ʹ��ADC�ж�
	EA = 1;
	ADC_CONTR |= 0x40;                          //����ADת��
}

/*****************************************************************************
�������� : ADC_Isr
�������� : ADC�жϴ�����
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void ADC_Isr() interrupt 5
{
	ADC_CONTR &= ~0x20;                         //���жϱ�־
	ADC_CONTR |= 0x40;                          //����ADת��
}



#elif defined(Query)

/*****************************************************************************
�������� : ADC_init
�������� : ADC��ʼ��
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void ADC_init()
{
	P1M0 = 0x00;                                //����P1.0ΪADC��
	P1M1 = 0x01;
	ADCCFG = 0x2f;                              //����ADCʱ��Ϊϵͳʱ��/2/16/16
	ADC_CONTR = 0x80;                           //ʹ��ADCģ��
}
/*****************************************************************************
�������� : ADC_read
�������� : ADC���ݲ�ѯ
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
int ADC_read()
{
	int res;
	
	ADC_CONTR |= 0x40;                      //����ADת��
	_nop_();
	_nop_();
	//_nop_();
	while (!(ADC_CONTR & 0x20));            //��ѯADC��ɱ�־
	ADC_CONTR &= ~0x20;                     //����ɱ�־
	res = (ADC_RES<<8)|ADC_RESL;            //��ȡADC���
	
	return res;
}
#endif



/*****************************************************************************
�������� : Get_Voltage
�������� : ��ص���������
������� : command��ָ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void Get_Voltage()
{
	int res;
	int vcc;  //��ص�ѹ����λmv
	int i;

	ADC_read();
	ADC_read();
	res = 0;
	for(i=0;i<8;i++)
	{
		res += ADC_read();
	}
	res >>= 3;
	vcc = (int)((6600L*res>>12)+50);  //��ѹ����
	
	if((vcc <= 4200) && (vcc >= 4088))
	{
		FlashBuffer.Power = 100;
		OLED_ShowNum(96,0,FlashBuffer.Power,3,8);
		OLED_ShowChar(120,0,'%',8);
	}
	else if((vcc <= 4088) && (vcc >= 4012))
	{
		FlashBuffer.Power = 90;
		OLED_ShowNum(96,0,FlashBuffer.Power,3,8);
		OLED_ShowChar(120,0,'%',8);
	}
	else if((vcc <= 4012) && (vcc >= 3956))
	{
		FlashBuffer.Power = 80;
		OLED_ShowNum(96,0,FlashBuffer.Power,3,8);
		OLED_ShowChar(120,0,'%',8);
	}
	else if((vcc <= 3956) && (vcc >= 3910))
	{
		FlashBuffer.Power = 70;
		OLED_ShowNum(96,0,FlashBuffer.Power,3,8);
		OLED_ShowChar(120,0,'%',8);
	}
	else if((vcc <= 3910) && (vcc >= 3870))
	{
		FlashBuffer.Power = 60;
		OLED_ShowNum(96,0,FlashBuffer.Power,3,8);
		OLED_ShowChar(120,0,'%',8);
	}
	else if((vcc <= 3870) && (vcc >= 3830))
	{
		FlashBuffer.Power = 50;
		OLED_ShowNum(96,0,FlashBuffer.Power,3,8);
		OLED_ShowChar(120,0,'%',8);
	}
	else if((vcc <= 3830) && (vcc >= 3802))
	{
		FlashBuffer.Power = 40;
		OLED_ShowNum(96,0,FlashBuffer.Power,3,8);
		OLED_ShowChar(120,0,'%',8);
	}
	else if((vcc <= 3802) && (vcc >= 3782))
	{
		FlashBuffer.Power = 30;
		OLED_ShowNum(96,0,FlashBuffer.Power,3,8);
		OLED_ShowChar(120,0,'%',8);
	}
	else if((vcc <= 3782) && (vcc >= 3764))
	{
		FlashBuffer.Power = 20;
		OLED_ShowNum(96,0,FlashBuffer.Power,3,8);
		OLED_ShowChar(120,0,'%',8);
	}
	else if((vcc <= 3764) && (vcc >= 3740))
	{
		FlashBuffer.Power = 10;
		OLED_ShowNum(96,0,FlashBuffer.Power,3,8);
		OLED_ShowChar(120,0,'%',8);
	}
	else if(vcc <= 3740)
	{
		FlashBuffer.Power = 0;
		OLED_ShowNum(96,0,FlashBuffer.Power,3,8);
		OLED_ShowChar(120,0,'%',8);
	}
	mcu_dp_value_update(DPID_BATTERY_PERCENTAGE,FlashBuffer.Power); //VALUE�������ϱ�;
}
