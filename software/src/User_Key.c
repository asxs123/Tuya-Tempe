/****************************************Copyright (c)*************************
**                               版权所有 (C), 2015-2020, 涂鸦科技
**
**                                 http://www.tuya.com
**
**--------------文件信息-------------------------------------------------------
**文   件   名: User_Key.c
**描        述: 按键接口处理
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
#define USER_KEY_GLOBAL
#include "include.h"

#include "wifi.h"



static KEY_STATE_T Key_State[MAX_KEY];
uint8_t wifi_test_mode = 0;                                                  //????

/*****************************************************************************
函数名称 : Key1_ShortPress_Handle
功能描述 : Key1短按处理函数
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
static void Key1_ShortPress_Handle(void)
{
  //当前产测模式失败
  if(wifi_test_mode == 1)
    return;
  
  if(FlashBuffer.power_switch == 0)
  {
		beep_control();
    FlashBuffer.power_switch = 1;
    LED_RGB_Control(255,FlashBuffer.fan_speed * 95,FlashBuffer.work_mode * 42);

//    LED_Control(LED2,FlashBuffer.lock);
		
		//开继电器
		relay_on();
  }
  else
  {
    if(FlashBuffer.lock == 1)                                                   //开机情况下童锁有效
      return;
    
		beep_control();
    FlashBuffer.power_switch = 0;
    LED_RGB_Control(0,0,0);
//    LED_Control(LED2,0);

		//关继电器
		relay_off();
    
    //保存运行时间
    Earse_Flash(PARA_ADDR);
    if(Write_Flash(PARA_ADDR,(unsigned char *)&FlashBuffer,sizeof(FlashBuffer)) == ERROR)
    {
      return;
    }  
  }

  mcu_dp_bool_update(DPID_SWITCH,FlashBuffer.power_switch);	
}
/*****************************************************************************
函数名称 : Key1_LongPress_Handle
功能描述 : Key1长按处理函数
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
static void Key1_LongPress_Handle(void)
{
	volatile uint32_t t;

  //复位wifi
  mcu_reset_wifi();
	
	beep_on();	
	for(t = 0; t < 0x10000; t ++);
	beep_off();	
}
/*****************************************************************************
函数名称 : Key2_ShortPress_Handle
功能描述 : Key2短按处理函数
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
static void Key2_ShortPress_Handle(void)
{

  if(FlashBuffer.power_switch == 0)
    return;
  
  if(FlashBuffer.lock == 1)
    return;
  
	beep_control();	
  FlashBuffer.work_mode ++;
  if(FlashBuffer.work_mode >= 7)
    FlashBuffer.work_mode = 0;
  
  LED_RGB_Control(255,FlashBuffer.fan_speed * 95,FlashBuffer.work_mode * 42);

  Earse_Flash(PARA_ADDR);
  if(Write_Flash(PARA_ADDR,(unsigned char *)&FlashBuffer,sizeof(FlashBuffer)) == ERROR)
  {
    return;
  }  
  
  mcu_dp_enum_update(DPID_MODE,FlashBuffer.work_mode);
}
/*****************************************************************************
函数名称 : Key2_LongPress_Handle
功能描述 : Key2长按处理函数
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
static void Key2_LongPress_Handle(void)
{
  volatile uint32_t t;
  
  wifi_test_mode = 0;
  LED_RGB_Control(0,0,0);
  
  //
  LED_RGB_Control(255,0,0);
  for(t = 0; t < 0x20000; t ++);
  //
  LED_RGB_Control(0,255,0);
  for(t = 0; t < 0x20000; t ++);
  //
  LED_RGB_Control(0,0,255);
  for(t = 0; t < 0x20000; t ++);
  //
  LED_RGB_Control(255,255,255);
  //
  wifi_test_mode = 1;
  mcu_start_wifitest();
  //
  for(t = 0; t < 0x20000; t ++);
  LED_RGB_Control(0,0,0);
}
/*****************************************************************************
函数名称 : Key3_ShortPress_Handle
功能描述 : Key3短按处理函数
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
static void Key3_ShortPress_Handle(void)
{
	if(FlashBuffer.power_switch == 0)
    return;
  
   if(FlashBuffer.lock == 1)
    return; 
   
	beep_control();	
  FlashBuffer.fan_speed ++;
  if(FlashBuffer.fan_speed >= 5)
    FlashBuffer.fan_speed = 0;

  LED_RGB_Control(255,FlashBuffer.fan_speed * 95,FlashBuffer.work_mode * 42);

  Earse_Flash(PARA_ADDR);
  if(Write_Flash(PARA_ADDR,(unsigned char *)&FlashBuffer,sizeof(FlashBuffer)) == ERROR)
  {
    return;
  }  
  
  mcu_dp_enum_update(DPID_SPEED,FlashBuffer.fan_speed);
}
/*****************************************************************************
函数名称 : Key4_ShortPress_Handle
功能描述 : Key4短按处理函数
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
static void Key4_ShortPress_Handle(void)
{
  if(FlashBuffer.power_switch == 0)
    return;
  
	beep_control();	

  if(FlashBuffer.lock == 0)
  {
    FlashBuffer.lock = 1;
  }
  else
  {
    FlashBuffer.lock = 0;
  }  
//  LED_Control(LED2,FlashBuffer.lock);
  
  Earse_Flash(PARA_ADDR);
  if(Write_Flash(PARA_ADDR,(unsigned char *)&FlashBuffer,sizeof(FlashBuffer)) == ERROR)
  {
    return;
  }  
  
  mcu_dp_bool_update(DPID_LOCK,FlashBuffer.lock);
}
/*****************************************************************************
函数名称 : key_init
功能描述 : key初始化
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void key_init(void)
{
  memset(Key_State,0,sizeof(Key_State));
  //
  Key_State[0].Long_Press_Handle = Key1_LongPress_Handle;
  Key_State[1].Long_Press_Handle = Key2_LongPress_Handle;
  //
  Key_State[0].Short_Press_Handle = Key1_ShortPress_Handle;
  Key_State[1].Short_Press_Handle = Key2_ShortPress_Handle;
  Key_State[2].Short_Press_Handle = Key3_ShortPress_Handle;
  Key_State[3].Short_Press_Handle = Key4_ShortPress_Handle;
}
/*****************************************************************************
函数名称 : Get_Key
功能描述 : 读取按键
输入参数 : 无
返回参数 : ReadKey:按键值
使用说明 : 无
*****************************************************************************/
static uint8_t Get_Key(void) 
{
  uint8_t ReadKey = 0;
  
  if(!K1)
  {
    ReadKey |= PRESS_KEY1;
  }
  
  if(!K2)
  {
    ReadKey |= PRESS_KEY2;
  }

  if(!K3)
  {
    ReadKey |= PRESS_KEY3;
  }
  
  if(!K4)
  {
    ReadKey |= PRESS_KEY4;
  }		
  
  return ReadKey;
}
/*****************************************************************************
函数名称 : Get_Key_Press_Time
功能描述 : 获取按键按下时间
输入参数 : last_time:上次按下时间
返回参数 : 2次按键之间差值
使用说明 : 无
*****************************************************************************/
static uint32_t Get_Key_Press_Time(uint32_t last_time)
{
  uint32_t time;
  
  time = get_count_value();
  if(time >= last_time)
  {
    time -= last_time;
  }
  else
  {
    //Time2溢出
    time += ~last_time;
  } 

  return time;
}
/*****************************************************************************
函数名称 : key_scan
功能描述 : 扫描按键
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void key_scan(void)
{
  uint8_t i;
  uint32_t time;
  uint8_t key;
  
  key = Get_Key();
  for(i = 0; i < MAX_KEY; i ++)
  {
    switch(Key_State[i].Status)
    {
      case KEY_NO:
        //有按键按下
        if((key >> i) & 0x01)
        {
          Key_State[i].Status = KEY_DOWN;
          Key_State[i].Press_Time = get_count_value();
        }
        break;
        
      case KEY_DOWN:
        if(((key >> i) & 0x01) == 0)
        {
          Key_State[i].Status = KEY_UP;
        }
        break;
                
      case KEY_LONG:
        if(((key >> i) & 0x01) == 0)
        {
          Key_State[i].Press_Time = 0;
          Key_State[i].Status = KEY_NO;
        }
        break;  
    }

    if((Key_State[i].Status == KEY_DOWN) || (Key_State[i].Status == KEY_LONG))
    {
      time = Get_Key_Press_Time(Key_State[i].Press_Time);
      if(time >= TIME_PRESS_LONG)
      {
				//一直长按
				Key_State[i].Press_Time = 0;
				Key_State[i].Status = KEY_NO;				
        if(Key_State[i].Long_Press_Handle)
        {
          Key_State[i].Long_Press_Handle();
        }
          
      }          
    }
    else if(Key_State[i].Status == KEY_UP)
    {
      //松开
			time = Get_Key_Press_Time(Key_State[i].Press_Time);
    
      Key_State[i].Press_Time = 0;
      Key_State[i].Status = KEY_NO;
      if((time >= TIME_PRESS_SHORT) && (time < TIME_PRESS_LONG))
      {
        //短按
        if(Key_State[i].Short_Press_Handle)
        {
          Key_State[i].Short_Press_Handle();
        }
      }
      else if(time >= TIME_PRESS_LONG)
      {
        //长按
        if(Key_State[i].Long_Press_Handle)
        {
          Key_State[i].Long_Press_Handle();
        }
      }
			
    }
  }
}




