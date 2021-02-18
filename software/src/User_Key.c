/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                                 http://www.tuya.com
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: User_Key.c
**��        ��: �����ӿڴ���
**ʹ �� ˵ �� : ��
**
**
**--------------��ǰ�汾�޶�---------------------------------------------------

** ��  ��: v2.0
** �ա���: 2016��3��29��
** �衡��: 1:�Ż�����ṹ
**
**-----------------------------------------------------------------------------
******************************************************************************/
#define USER_KEY_GLOBAL
#include "include.h"

#include "wifi.h"



static KEY_STATE_T Key_State[MAX_KEY];
uint8_t wifi_test_mode = 0;                                                  //????

/*****************************************************************************
�������� : Key1_ShortPress_Handle
�������� : Key1�̰�������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
static void Key1_ShortPress_Handle(void)
{
  //��ǰ����ģʽʧ��
  if(wifi_test_mode == 1)
    return;
  
  if(FlashBuffer.power_switch == 0)
  {
		beep_control();
    FlashBuffer.power_switch = 1;
    LED_RGB_Control(255,FlashBuffer.fan_speed * 95,FlashBuffer.work_mode * 42);

//    LED_Control(LED2,FlashBuffer.lock);
		
		//���̵���
		relay_on();
  }
  else
  {
    if(FlashBuffer.lock == 1)                                                   //���������ͯ����Ч
      return;
    
		beep_control();
    FlashBuffer.power_switch = 0;
    LED_RGB_Control(0,0,0);
//    LED_Control(LED2,0);

		//�ؼ̵���
		relay_off();
    
    //��������ʱ��
    Earse_Flash(PARA_ADDR);
    if(Write_Flash(PARA_ADDR,(unsigned char *)&FlashBuffer,sizeof(FlashBuffer)) == ERROR)
    {
      return;
    }  
  }

  mcu_dp_bool_update(DPID_SWITCH,FlashBuffer.power_switch);	
}
/*****************************************************************************
�������� : Key1_LongPress_Handle
�������� : Key1����������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
static void Key1_LongPress_Handle(void)
{
	volatile uint32_t t;

  //��λwifi
  mcu_reset_wifi();
	
	beep_on();	
	for(t = 0; t < 0x10000; t ++);
	beep_off();	
}
/*****************************************************************************
�������� : Key2_ShortPress_Handle
�������� : Key2�̰�������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
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
�������� : Key2_LongPress_Handle
�������� : Key2����������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
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
�������� : Key3_ShortPress_Handle
�������� : Key3�̰�������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
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
�������� : Key4_ShortPress_Handle
�������� : Key4�̰�������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
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
�������� : key_init
�������� : key��ʼ��
������� : ��
���ز��� : ��
ʹ��˵�� : ��
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
�������� : Get_Key
�������� : ��ȡ����
������� : ��
���ز��� : ReadKey:����ֵ
ʹ��˵�� : ��
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
�������� : Get_Key_Press_Time
�������� : ��ȡ��������ʱ��
������� : last_time:�ϴΰ���ʱ��
���ز��� : 2�ΰ���֮���ֵ
ʹ��˵�� : ��
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
    //Time2���
    time += ~last_time;
  } 

  return time;
}
/*****************************************************************************
�������� : key_scan
�������� : ɨ�谴��
������� : ��
���ز��� : ��
ʹ��˵�� : ��
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
        //�а�������
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
				//һֱ����
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
      //�ɿ�
			time = Get_Key_Press_Time(Key_State[i].Press_Time);
    
      Key_State[i].Press_Time = 0;
      Key_State[i].Status = KEY_NO;
      if((time >= TIME_PRESS_SHORT) && (time < TIME_PRESS_LONG))
      {
        //�̰�
        if(Key_State[i].Short_Press_Handle)
        {
          Key_State[i].Short_Press_Handle();
        }
      }
      else if(time >= TIME_PRESS_LONG)
      {
        //����
        if(Key_State[i].Long_Press_Handle)
        {
          Key_State[i].Long_Press_Handle();
        }
      }
			
    }
  }
}




