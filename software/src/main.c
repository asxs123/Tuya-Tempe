/******************************************************************************
�ļ�����: Main.c
�ļ���ʶ: STC12SSA60S2
ժ    Ҫ: ������
��ǰ�汾: V1.0	
�������: 2013.11.18
*******************************************************************************/
#define MAINWORK_GLOBALS
#include "include.h"

#include "wifi.h"

volatile uint8_t is_read_time = 0;
uint8_t read_pm25 = 0;
//==============================================================================
//�������Ͷ���
//��λ����
//==============================================================================
#define         FALUT_TMPER_SUPERHEAT                 0x01                      //�¶ȹ���
#define         FALUT_TMPER_UNEXIST                   0x02                      //��ʪ�ȴ�����������
#define         FALUT_PM25_UNEXIST                    0x04                      //PM2.5������������

/*****************************************************************************
�������� : beep_control
�������� : ����������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void beep_control(void)
{
	volatile uint32_t t;

	beep_on();	
	for(t = 0; t < 0x5000; t ++);
	beep_off();	
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
	wifi_power_on();		   																											//WIFI ��Դ�ܵ�Ƭ�����ƣ����쳣��ʱ�򣬵�Ƭ������ǿ��������ESP8266
	
	relay_on();
	
	key_init();
	uart1_init();
	timer0_init();
	pwm_init();
	
	beep_off();
	EA = 1;																																				//�������ж�
}
/*****************************************************************************
�������� : wifi_work_state_led
�������� : wifi״ָ̬ʾ�ƿ���
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void wifi_work_state_led(void)
{
  uint8_t wifi_state = mcu_get_wifi_work_state();
  static uint8_t wifi_state_bak = WIFI_SATE_UNKNOW;
	
	switch(wifi_state)
	{
	case SMART_CONFIG_STATE:
		if(get_timer_250ms_value() % 2 == 0)
		{
			LED_RGB_Control(0,0,255);
		}
		else
		{
			LED_RGB_Control(0,0,0);
		}
		wifi_state_bak = SMART_CONFIG_STATE;
		break;
		
	case AP_STATE:
		if((get_timer_250ms_value() % 12) == 0)
		{
			LED_RGB_Control(0,0,255);
		}
		else if((get_timer_250ms_value() % 12) == 6)
		{
			LED_RGB_Control(0,0,0);
		}
		wifi_state_bak = AP_STATE;
		break;
		
	case WIFI_NOT_CONNECTED:
		if((wifi_state_bak == SMART_CONFIG_STATE) || (wifi_state_bak == AP_STATE))
		{
			LED_RGB_Control(0,0,0);
		}
		break;
		
	case WIFI_CONNECTED:
		if((wifi_state_bak == SMART_CONFIG_STATE) || (wifi_state_bak == AP_STATE))
		{		
			LED_RGB_Control(0,0,255);
			wifi_state_bak = WIFI_CONNECTED;
		}
		break;
		
	default:
		if((wifi_state_bak == SMART_CONFIG_STATE) || (wifi_state_bak == AP_STATE))
		{
			LED_RGB_Control(0,0,0);
		}
		break;
	}
}
/*****************************************************************************
�������� : get_pm25
�������� : ��ȡPM2.5
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
static void get_pm25(void)
{
	/*
  ��ʾ��δʵ��PM2.5��������ȡ,���û�����ʵ��
  */
	read_pm25 = 1;
	FlashBuffer.pm_25 = 99;
  if(read_pm25 == 1)
  {
    //�ɹ�
    mcu_dp_value_update(DPID_PM25,FlashBuffer.pm_25);     //VALUE�������ϱ�;
  }
  else if(read_pm25 == 0)       
  {
    //ʧ��
    FlashBuffer.falut |= FALUT_PM25_UNEXIST;
    mcu_dp_fault_update(DPID_FAULT,FlashBuffer.falut); //�����������ϱ�;
  }
}
/*****************************************************************************
�������� : get_tvoc
�������� : ��ȡtvoc
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
static void get_tvoc(void)
{
  /*
  ��ʾ��δʵ��TVOC��������ȡ,���û�����ʵ��
  */
  FlashBuffer.tvoc = 98;
  
  mcu_dp_value_update(DPID_TVOC,FlashBuffer.tvoc);      //VALUE�������ϱ�;
}
/*****************************************************************************
�������� : get_filter
�������� : ��ȡ��о�ٷֱ�
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
static void get_filter(void)
{
  /*
  ��ʾ��δʵ����о�ٷֱ�ͳ��,���û�����ʵ��
  */
  FlashBuffer.filter = 100;
  
  mcu_dp_value_update(DPID_FILTER,FlashBuffer.filter); //VALUE�������ϱ�;
}
/*****************************************************************************
�������� : get_runtime
�������� : ��ȡ����ʱ��ͳ��
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
static void get_runtime(void)
{
  mcu_dp_value_update(DPID_TOTALTIME,FlashBuffer.run_time); //VALUE�������ϱ�;
}
/*****************************************************************************
�������� : get_temper_humidity
�������� : ��ȡ��ʪ��
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
static void get_temper_humidity(void)
{
  uint8_t state;
  uint8_t temperature,humidity;
  
//  state = DHT11_Read_Data(&temperature,&humidity);
	state = 0;
  /*
  ��ʾ��δʵ����ʪ�ȶ�ȡ,���û�����ʵ��
  */
  if(state == 0)
  {
		temperature = 25;
		humidity = 45;
    //�¶�ͷ��ȡ����
    FlashBuffer.temper = temperature;
    FlashBuffer.humidity = humidity;
    
    mcu_dp_value_update(DPID_HUMIDITY,FlashBuffer.humidity); //VALUE�������ϱ�;  
    if(FlashBuffer.temper <= 50)
    {
      FlashBuffer.falut = 0;
      //�¶��ϱ�
      mcu_dp_value_update(DPID_TEMP,FlashBuffer.temper); //VALUE�������ϱ�;
    }
    else
    {
      //����,�¶ȹ���
      FlashBuffer.falut &= ~FALUT_TMPER_UNEXIST;
      FlashBuffer.falut |= FALUT_TMPER_SUPERHEAT;
      mcu_dp_fault_update(DPID_FAULT,FlashBuffer.falut); //�����������ϱ�;
    }
  }
  else
  {
    //����,�¶�ͷδ��⵽
    FlashBuffer.falut &= ~FALUT_TMPER_SUPERHEAT;
    FlashBuffer.falut |= FALUT_TMPER_UNEXIST;
    mcu_dp_fault_update(DPID_FAULT,FlashBuffer.falut); //�����������ϱ�;
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
  get_temper_humidity();                                                    //��ȡ��ʪ��
  get_pm25();                                                               //��ȡPM2.5
  get_runtime();                                                            //��ȡ����ʱ��
  get_filter();                                                             //��ȡ��оʹ����
  get_tvoc();                                                               //��ȡTVOC	
}
/*****************************************************************************
�������� : read_parameter
�������� : ��ȡ����
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
static void read_parameter(void)
{
	Read_Flash((uint8_t *)&FlashBuffer,PARA_ADDR,sizeof(TYPE_BUFFER_S));
  if((FlashBuffer.magic_code != 0) && (FlashBuffer.magic_code != 0xffffffff))
  {
    memset(&FlashBuffer,0,sizeof(FlashBuffer));
		
		Earse_Flash(PARA_ADDR);
    if(Write_Flash(PARA_ADDR,(unsigned char *)&FlashBuffer,sizeof(FlashBuffer)) == ERROR)
    {
      
    }	
	}
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
	Bsp_Init();
  read_parameter();

	//��ʼ��wifi
	wifi_protocol_init();
	
  //��ȡ�������ϱ�
	read_sensor();
  FlashBuffer.falut = 0;
  FlashBuffer.power_switch = 0;	
	
	while(1)
	{
		//wifi���ݴ���
		wifi_uart_service();

		//����ɨ��
		key_scan();

		//wifi״ָ̬ʾ��
		wifi_work_state_led();
		
		if(is_read_time == 1)                                                       //15���ȡһ��
		{
		  //��ȡ���������ϱ�
		  read_sensor();                                 
		  /*
		  ÿ15���ȡ������ʱ��,����У�Ա���RTC
		  ����protocol.c��mcu_write_rtctime()��������ɣңԣ�д��
		  */
		  mcu_get_system_time();
		  //
		  is_read_time = 0;
		}
	}
}