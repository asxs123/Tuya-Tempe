/******************************************************************************
文件名称: Main.c
文件标识: STC8A8K64S4A12
摘    要: 主函数
当前版本: V1.1
完成日期: 2021.02.18
*******************************************************************************/
#define MAINWORK_GLOBALS
#include "include.h"
#include "wifi.h"


volatile uint8_t is_read_time = 0;
/*****************************************************************************
函数名称 : Bsp_Init
功能描述 : 初始化硬件
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
static void Bsp_Init(void)
{
	uart3_init();        //串口3初始化
	wifi_protocol_init();//WiFi模组初始化
	I2C_init();          //I2C初始化
	ADC_init();          //ADC初始化
	SHT_Init();          //SHT3X初始化
	timer0_init();       //定时器初始化	
	OLED_Init();         //OLED初始化
	OLED_Clear();        //OLED清屏
	key_init();          //按键初始化
}

/*****************************************************************************
函数名称 : welcome
功能描述 : 界面显示
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void welcome()
{
	OLED_ShowStr(24,1," Tuya demo",16);
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
函数名称 : wifi_work_state_led
功能描述 : wifi状态查询
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void wifi_work_state_led(void)
{
  uint8_t wifi_state = mcu_get_wifi_work_state();
	switch(wifi_state)
	{
	case SMART_CONFIG_STATE:            //smartconfig配置状态
	  OLED_ShowStr(0,0,"  S! ",8);
		break;
		
	case AP_STATE:                      //AP配置状态
	  OLED_ShowStr(0,0,"  A! ",8);
		break;
		
	case WIFI_NOT_CONNECTED:            //WIFI配置成功但未连上路由器
		OLED_DrawBMP(0,0,24,1,UI1);
		OLED_ShowStr(24,0,"  ",8);
		break;
		
	case WIFI_CONNECTED:                //WIFI配置成功且连上路由器
		OLED_DrawBMP(0,0,24,1,UI2);
		OLED_ShowStr(24,0,"  ",8);
		break;
		
	case WIFI_CONN_CLOUD:               //WIFI已经连接上云服务器
		OLED_DrawBMP(0,0,24,1,UI3);
		OLED_ShowStr(24,0,"  ",8);
		break;
		
	case WIFI_LOW_POWER:                //WIFI处于低功耗模式
		OLED_DrawBMP(0,0,24,1,UI4);
		OLED_ShowStr(24,0,"  ",8);
		break;
		
	case SMART_AND_AP_STATE:            //WIFI smartconfig&AP 模式
		OLED_ShowStr(0,0,"  SA ",8);
		break;
		
	default:                            //未知状态
		OLED_ShowStr(0,0,"error",8);
	 break;
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
	Get_Voltage();         //读取电池电压
	sht30_read();          //读取温湿度
}
/*****************************************************************************
函数名称 : Boot_animation
功能描述 : 开机动画
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void Boot_animation()
{
	uint8_t i;
	uint16_t j;
	
	for(i = 0; i<16; i++)
	{
		OLED_DrawBMP(0+i*8,3,8+i*8,5,UI5);
		for(j = 0; j<30000; j++)
		{
			wifi_uart_service();
		}
	}
	OLED_Clear();
	welcome();
	read_sensor();
	wifi_uart_service();
	wifi_work_state_led();
	wifi_uart_service();
	mcu_get_system_time();
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
	//初始化
	Bsp_Init();
	
	//开机动画
	Boot_animation();
	
	//主循环
	while(1)
	{
		//wifi数据处理
		wifi_uart_service();
		//时间显示
		show_time();
		//按键扫描
		key_scan();
		//wifi状态指示灯
		wifi_work_state_led();
		
		if(is_read_time == 1)  //默认30秒读取一次，可通过app设置
		{
		  //读取传感器并上报
		  read_sensor();
			//获取服务器时间，用于本地校时
			mcu_get_system_time();
			//清除读标志
		  is_read_time = 0;
		}
	}
	
}