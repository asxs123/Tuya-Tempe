/******************************************************************************
�ļ�����: Main.c
�ļ���ʶ: STC8A8K64S4A12
ժ    Ҫ: ������
��ǰ�汾: V1.0.1  (�޸�����״̬��ʾ���޸Ŀ������������Ӵ���������ʱ������)	
�������: 2021.02.09
*******************************************************************************/
#define MAINWORK_GLOBALS
#include "include.h"
#include "wifi.h"


volatile uint8_t is_read_time = 0;

void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 6;
	j = 157;
	k = 59;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
/*****************************************************************************
�������� : Bsp_Init
�������� : ��ʼ��Ӳ��
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
static void Bsp_Init(void)
{
	uart3_init();        //����3��ʼ��
	wifi_protocol_init();//WiFiģ���ʼ��
	I2C_init();          //I2C��ʼ��
	ADC_init();          //ADC��ʼ��
	SHT_Init();          //SHT3X��ʼ��
	timer0_init();       //��ʱ����ʼ��	
	OLED_Init();         //OLED��ʼ��
	OLED_Clear();        //OLED����
	key_init();          //������ʼ��
}

/*****************************************************************************
�������� : welcome
�������� : ������ʾ
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void welcome()
{
	OLED_ShowCHinese(0,4,0);
  OLED_ShowCHinese(16,4,1);
	OLED_ShowCHinese(72,4,2);
	OLED_ShowCHinese(88,4,1);
	OLED_ShowCHinese(0,6,3);
	OLED_ShowCHinese(16,6,4);
	OLED_ShowCHinese(32,6,0);
	OLED_ShowCHinese(48,6,1);
}
/*****************************************************************************
�������� : wifi_work_state_led
�������� : wifi״̬��ѯ
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void wifi_work_state_led(void)
{
  uint8_t wifi_state = mcu_get_wifi_work_state();
	switch(wifi_state)
	{
	case SMART_CONFIG_STATE:            //smartconfig����״̬
	  OLED_ShowStr(0,0,"  S! ",8);
		break;
		
	case AP_STATE:                      //AP����״̬
	  OLED_ShowStr(0,0,"  A! ",8);
		break;
		
	case WIFI_NOT_CONNECTED:            //WIFI���óɹ���δ����·����
		OLED_DrawBMP(0,0,24,1,UI1);
		OLED_ShowStr(24,0,"  ",8);
		break;
		
	case WIFI_CONNECTED:                //WIFI���óɹ�������·����
		OLED_DrawBMP(0,0,24,1,UI2);
		OLED_ShowStr(24,0,"  ",8);
		break;
		
	case WIFI_CONN_CLOUD:               //WIFI�Ѿ��������Ʒ�����
		OLED_DrawBMP(0,0,24,1,UI3);
		OLED_ShowStr(24,0,"  ",8);
		break;
		
	case WIFI_LOW_POWER:                //WIFI���ڵ͹���ģʽ
		OLED_DrawBMP(0,0,24,1,UI4);
		OLED_ShowStr(24,0,"  ",8);
		break;
		
	case SMART_AND_AP_STATE:            //WIFI smartconfig&AP ģʽ
		OLED_ShowStr(0,0,"  SA ",8);
		break;
		
	default:                            //δ֪״̬
		OLED_ShowStr(0,0,"error",8);
	 break;
	}
}
/*****************************************************************************
�������� : read_sensor
�������� : ��ȡ������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
static void read_sensor(void)
{
	Get_Voltage();         //��ȡ��ص�ѹ
	sht30_read();          //��ȡ��ʪ��
}
/*****************************************************************************
�������� : Boot_animation
�������� : ��������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void Boot_animation()
{
	uint8_t i;
	
	for(i = 0; i<16; i++)
	{
		wifi_uart_service();
		OLED_DrawBMP(0+i*8,3,8+i*8,5,UI5);
		Delay100ms();
	}
	wifi_uart_service();
	OLED_Clear();
	welcome();
	read_sensor();
	wifi_work_state_led();
}

/*****************************************************************************
�������� : main
�������� : ������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void main(void)
{
	//��ʼ��
	Bsp_Init();
	
	//��������
	Boot_animation();
	
	//��ѭ��
	while(1)
	{
		//wifi���ݴ���
		wifi_uart_service();
		//����ɨ��
		key_scan();
		//wifi״ָ̬ʾ��
		wifi_work_state_led();
		if(is_read_time == 1)                       //Ĭ��15���ȡһ�Σ���ͨ��app����
		{
		  //��ȡ���������ϱ�
		  read_sensor();                                 
		  is_read_time = 0;
		}
	}
	
}