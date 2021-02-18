/******************************************************************************
文件名称: Main.c
文件标识: STC12SSA60S2
摘    要: 主函数
当前版本: V1.0	
完成日期: 2013.11.18
*******************************************************************************/
#define MAINWORK_GLOBALS
#include "include.h"

#include "wifi.h"

volatile uint8_t is_read_time = 0;
uint8_t read_pm25 = 0;
//==============================================================================
//故障类型定义
//按位定义
//==============================================================================
#define         FALUT_TMPER_SUPERHEAT                 0x01                      //温度过高
#define         FALUT_TMPER_UNEXIST                   0x02                      //温湿度传感器不存在
#define         FALUT_PM25_UNEXIST                    0x04                      //PM2.5传感器不存在

/*****************************************************************************
函数名称 : beep_control
功能描述 : 蜂鸣器控制
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void beep_control(void)
{
	volatile uint32_t t;

	beep_on();	
	for(t = 0; t < 0x5000; t ++);
	beep_off();	
}
/*****************************************************************************
函数名称 : Bsp_Init
功能描述 : 初始化硬件
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
static void Bsp_Init(void)
{
	wifi_power_on();		   																											//WIFI 电源受单片机控制，在异常的时候，单片机可以强制重启动ESP8266
	
	relay_on();
	
	key_init();
	uart1_init();
	timer0_init();
	pwm_init();
	
	beep_off();
	EA = 1;																																				//允许总中断
}
/*****************************************************************************
函数名称 : wifi_work_state_led
功能描述 : wifi状态指示灯控制
输入参数 : 无
返回参数 : 无
使用说明 : 无
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
函数名称 : get_pm25
功能描述 : 读取PM2.5
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
static void get_pm25(void)
{
	/*
  本示例未实现PM2.5传感器读取,请用户自行实现
  */
	read_pm25 = 1;
	FlashBuffer.pm_25 = 99;
  if(read_pm25 == 1)
  {
    //成功
    mcu_dp_value_update(DPID_PM25,FlashBuffer.pm_25);     //VALUE型数据上报;
  }
  else if(read_pm25 == 0)       
  {
    //失败
    FlashBuffer.falut |= FALUT_PM25_UNEXIST;
    mcu_dp_fault_update(DPID_FAULT,FlashBuffer.falut); //故障型数据上报;
  }
}
/*****************************************************************************
函数名称 : get_tvoc
功能描述 : 读取tvoc
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
static void get_tvoc(void)
{
  /*
  本示例未实现TVOC传感器读取,请用户自行实现
  */
  FlashBuffer.tvoc = 98;
  
  mcu_dp_value_update(DPID_TVOC,FlashBuffer.tvoc);      //VALUE型数据上报;
}
/*****************************************************************************
函数名称 : get_filter
功能描述 : 读取滤芯百分比
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
static void get_filter(void)
{
  /*
  本示例未实现滤芯百分比统计,请用户自行实现
  */
  FlashBuffer.filter = 100;
  
  mcu_dp_value_update(DPID_FILTER,FlashBuffer.filter); //VALUE型数据上报;
}
/*****************************************************************************
函数名称 : get_runtime
功能描述 : 读取运行时间统计
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
static void get_runtime(void)
{
  mcu_dp_value_update(DPID_TOTALTIME,FlashBuffer.run_time); //VALUE型数据上报;
}
/*****************************************************************************
函数名称 : get_temper_humidity
功能描述 : 读取温湿度
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
static void get_temper_humidity(void)
{
  uint8_t state;
  uint8_t temperature,humidity;
  
//  state = DHT11_Read_Data(&temperature,&humidity);
	state = 0;
  /*
  本示例未实现温湿度读取,请用户自行实现
  */
  if(state == 0)
  {
		temperature = 25;
		humidity = 45;
    //温度头读取正常
    FlashBuffer.temper = temperature;
    FlashBuffer.humidity = humidity;
    
    mcu_dp_value_update(DPID_HUMIDITY,FlashBuffer.humidity); //VALUE型数据上报;  
    if(FlashBuffer.temper <= 50)
    {
      FlashBuffer.falut = 0;
      //温度上报
      mcu_dp_value_update(DPID_TEMP,FlashBuffer.temper); //VALUE型数据上报;
    }
    else
    {
      //故障,温度过高
      FlashBuffer.falut &= ~FALUT_TMPER_UNEXIST;
      FlashBuffer.falut |= FALUT_TMPER_SUPERHEAT;
      mcu_dp_fault_update(DPID_FAULT,FlashBuffer.falut); //故障型数据上报;
    }
  }
  else
  {
    //故障,温度头未检测到
    FlashBuffer.falut &= ~FALUT_TMPER_SUPERHEAT;
    FlashBuffer.falut |= FALUT_TMPER_UNEXIST;
    mcu_dp_fault_update(DPID_FAULT,FlashBuffer.falut); //故障型数据上报;
  }
}
/*****************************************************************************
函数名称 : read_sensor
功能描述 : 读取传感器
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
static void read_sensor(void)
{
  get_temper_humidity();                                                    //读取温湿度
  get_pm25();                                                               //读取PM2.5
  get_runtime();                                                            //读取运行时间
  get_filter();                                                             //读取滤芯使用率
  get_tvoc();                                                               //读取TVOC	
}
/*****************************************************************************
函数名称 : read_parameter
功能描述 : 读取参数
输入参数 : 无
返回参数 : 无
使用说明 : 无
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
函数名称 : main
功能描述 : 主函数
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void main(void)
{
	Bsp_Init();
  read_parameter();

	//初始化wifi
	wifi_protocol_init();
	
  //读取参数并上报
	read_sensor();
  FlashBuffer.falut = 0;
  FlashBuffer.power_switch = 0;	
	
	while(1)
	{
		//wifi数据处理
		wifi_uart_service();

		//按键扫描
		key_scan();

		//wifi状态指示灯
		wifi_work_state_led();
		
		if(is_read_time == 1)                                                       //15秒读取一次
		{
		  //读取传感器并上报
		  read_sensor();                                 
		  /*
		  每15秒获取服务器时间,用于校对本地RTC
		  请在protocol.c的mcu_write_rtctime()函数内完成ＲＴＣ写入
		  */
		  mcu_get_system_time();
		  //
		  is_read_time = 0;
		}
	}
}