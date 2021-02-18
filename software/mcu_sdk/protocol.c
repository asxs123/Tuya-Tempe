/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                                 http://www.tuya.com
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: protocol.c
**��        ��: �·�/�ϱ����ݴ�����
**ʹ �� ˵ �� :

                  *******�ǳ���Ҫ��һ��Ҫ��Ŷ������********

** 1���û��ڴ��ļ���ʵ�������·�/�ϱ�����
** 2��DP��ID/TYPE�����ݴ���������Ҫ�û�����ʵ�ʶ���ʵ��
** 3������ʼĳЩ�궨�����Ҫ�û�ʵ�ִ���ĺ����ڲ���#err��ʾ,��ɺ�������ɾ����#err
**
**--------------��ǰ�汾�޶�---------------------------------------------------
** ��  ��: v2.3.6
** �ա���: 2016��7��21��
** �衡��: 1:�޸���ȡ����ʱ�����
           2:���hex_to_bcdת������
		   
** ��  ��: v2.3.5
** �ա���: 2016��6��3��
** �衡��: 1:�޸ķ���Э��汾Ϊ0x01
           2:�̼���������ƫ�����޸�Ϊ4�ֽ�

** ��  ��: v2.3.4
** �ա���: 2016��5��26��
** �衡��: 1:�Ż����ڽ�������
           2:�Ż�������������,ȡ��enum���Ͷ���

** ��  ��: v2.3.3
** �ա���: 2016��5��24��
** �衡��: 1:�޸�mcu��ȡ����ʱ�亯��
           2:���wifi���ܲ���

** ��  ��: v2.3.2
** �ա���: 2016��4��23��
** �衡��: 1:�Ż��������ݽ���
           2:�Ż�MCU�̼���������
           3:�Ż��ϱ�����

** ��  ��: v2.3.1
** �ա���: 2016��4��15��
** �衡��: 1:�Ż��������ݽ���

** ��  ��: v2.3
** �ա���: 2016��4��14��
** �衡��: 1:֧��MCU�̼���������

** ��  ��: v2.2
** �ա���: 2016��4��11��
** �衡��: 1:�޸Ĵ������ݽ��շ�ʽ

** ��  ��: v2.1
** �ա���: 2016��4��8��
** �衡��: 1:����ĳЩ��������֧�ֺ���ָ�����ѡ��

** ��  ��: v2.0
** �ա���: 2016��3��29��
** �衡��: 1:�Ż�����ṹ
           2:��ʡRAM�ռ�
**
**-----------------------------------------------------------------------------
******************************************************************************/
#include "include.h"
#include "wifi.h"

extern TYPE_BUFFER_S FlashBuffer;

/******************************************************************************
                                ��ֲ��֪:
1:MCU������while��ֱ�ӵ���mcu_api.c�ڵ�wifi_uart_service()����
2:����������ʼ����ɺ�,���鲻���йش����ж�,�������ж�,���ж�ʱ������,���жϻ����𴮿����ݰ���ʧ
3:�������ж�/��ʱ���ж��ڵ����ϱ�����
******************************************************************************/

         
/******************************************************************************
                              ��һ��:��ʼ��
1:����Ҫʹ�õ�wifi����ļ����ļ���include "wifi.h"
2:��MCU��ʼ���е���mcu_api.c�ļ��е�wifi_protocol_init()����
3:��MCU���ڵ��ֽڷ��ͺ�������protocol.c�ļ���uart_transmit_output������,��ɾ��#error
4:��MCU���ڽ��պ����е���mcu_api.c�ļ��ڵ�uart_receive_input����,�������յ����ֽ���Ϊ��������
5:��Ƭ������whileѭ�������mcu_api.c�ļ��ڵ�wifi_uart_service()����
******************************************************************************/

/******************************************************************************
                        1:dp���ݵ��������Ͷ��ձ�
          **��Ϊ�Զ����ɴ���,���ڿ���ƽ̨������޸�����������MCU_SDK**         
******************************************************************************/
const DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_SWITCH, DP_TYPE_BOOL},
  {DPID_MODE, DP_TYPE_ENUM},
  {DPID_SPEED, DP_TYPE_ENUM},
  {DPID_PM25, DP_TYPE_VALUE},
  {DPID_FILTER, DP_TYPE_VALUE},
  {DPID_LOCK, DP_TYPE_BOOL},
  {DPID_TEMP, DP_TYPE_VALUE},
  {DPID_HUMIDITY, DP_TYPE_VALUE},
  {DPID_TVOC, DP_TYPE_VALUE},
  {DPID_TOTALTIME, DP_TYPE_VALUE},
  {DPID_FAULT, DP_TYPE_FAULT},
};


/******************************************************************************
                           2:���ڵ��ֽڷ��ͺ���
�뽫MCU���ڷ��ͺ�������ú�����,�������յ���������Ϊ�������봮�ڷ��ͺ���
******************************************************************************/

/*****************************************************************************
�������� : uart_transmit_output
�������� : �����ݴ���
������� : value:�����յ��ֽ�����
���ز��� : ��
ʹ��˵�� : �뽫MCU���ڷ��ͺ�������ú�����,�������յ���������Ϊ�������봮�ڷ��ͺ���
*****************************************************************************/
void uart_transmit_output(unsigned char value)
{
//  #error "�뽫MCU���ڷ��ͺ�������ú���,��ɾ������"
  //ʾ��:
  extern void Uart_PutChar(unsigned char value);
  Uart_PutChar(value);	                                //���ڷ��ͺ���

}
/******************************************************************************
                           �ڶ���:ʵ�־����û�����
1:APP�·����ݴ���
2:�����ϱ�����
******************************************************************************/

/******************************************************************************
                            1:���������ϱ�����
��ǰ��������ȫ�������ϱ�(�������·�/���ϱ���ֻ�ϱ�)
  ��Ҫ�û�����ʵ�����ʵ��:
  1:��Ҫʵ�ֿ��·�/���ϱ����ݵ��ϱ�
  2:��Ҫʵ��ֻ�ϱ����ݵ��ϱ�
�˺���ΪMCU�ڲ��������
�û�Ҳ�ɵ��ô˺���ʵ��ȫ�������ϱ�
******************************************************************************/

//�Զ������������ϱ�����

/*****************************************************************************
�������� : all_data_update
�������� : ϵͳ����dp����Ϣ�ϴ�,ʵ��APP��muc����ͬ��
������� : ��
���ز��� : ��
ʹ��˵�� : �˺���SDK�ڲ������;
           MCU����ʵ�ָú����������ϱ�����;����ֻ�ϱ��Ϳ��ϱ����·�������
*****************************************************************************/
void all_data_update(void)
{
//  #error "���ڴ˴�����·����ϱ����ݼ�ֻ�ϱ�����ʾ��,������ɺ�ɾ������"
  //�˴���Ϊƽ̨�Զ����ɣ��밴��ʵ�������޸�ÿ�����·����ϱ�������ֻ�ϱ����� 
  mcu_dp_bool_update(DPID_SWITCH,FlashBuffer.power_switch); //BOOL�������ϱ�;
  mcu_dp_enum_update(DPID_MODE,FlashBuffer.work_mode); //ö���������ϱ�;
  mcu_dp_enum_update(DPID_SPEED,FlashBuffer.fan_speed); //ö���������ϱ�;
  mcu_dp_value_update(DPID_PM25,FlashBuffer.pm_25); //VALUE�������ϱ�;
  mcu_dp_value_update(DPID_FILTER,FlashBuffer.filter); //VALUE�������ϱ�;
  mcu_dp_bool_update(DPID_LOCK,FlashBuffer.lock); //BOOL�������ϱ�;
  mcu_dp_value_update(DPID_TEMP,FlashBuffer.temper); //VALUE�������ϱ�;
  mcu_dp_value_update(DPID_HUMIDITY,FlashBuffer.humidity); //VALUE�������ϱ�;
  mcu_dp_value_update(DPID_TVOC,FlashBuffer.tvoc); //VALUE�������ϱ�;
  mcu_dp_value_update(DPID_TOTALTIME,FlashBuffer.run_time); //VALUE�������ϱ�;
  mcu_dp_fault_update(DPID_FAULT,FlashBuffer.falut); //�����������ϱ�;
}


/******************************************************************************
                                WARNING!!!    
                            2:���������ϱ�����
�Զ�������ģ�庯��,�������û�����ʵ�����ݴ���
******************************************************************************/


/*****************************************************************************
�������� : dp_download_switch_handle
�������� : ���DPID_SWITCH�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪBOOL
  unsigned char ret;
  //0:��/1:��
  unsigned char power_switch;
  
  power_switch = mcu_get_dp_download_bool(value,length);
  if(power_switch == 0)
  {
    //���ع�
    LED_RGB_Control(0,0,0);
//    LED_Control(LED2,0);
        
		//�ؼ̵���
		relay_off();
		
    //��������ʱ��
    Earse_Flash(PARA_ADDR);
    if(Write_Flash(PARA_ADDR,(unsigned char *)&FlashBuffer,sizeof(FlashBuffer)) == ERROR)
    {
      return ERROR;
    }  
  }
  else
  {
    //���ؿ�
    LED_RGB_Control(255,FlashBuffer.fan_speed * 95,FlashBuffer.work_mode * 42);
//    LED_Control(LED2,FlashBuffer.lock);
		
		//���̵���
		relay_on();
  }
  
  FlashBuffer.power_switch = power_switch;
  
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_bool_update(DPID_SWITCH,power_switch);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_mode_handle
�������� : ���DPID_MODE�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_mode_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪENUM
  unsigned char ret;
  unsigned char mode;
  
  mode = mcu_get_dp_download_enum(value,length);
  switch(mode)
  {
    case 0:
      
      break;
      
    case 1:
      
      break;
      
    case 2:
      
      break;
      
    case 3:
      
      break;
      
    case 4:
      
      break;
      
    case 5:
      
      break;
      
    case 6:
      
      break;
      
    default:
      
      break;
  }
    
  FlashBuffer.work_mode = mode;
 
  LED_RGB_Control(255,FlashBuffer.fan_speed * 95,FlashBuffer.work_mode * 42);
  
  Earse_Flash(PARA_ADDR);
  if(Write_Flash(PARA_ADDR,(unsigned char *)&FlashBuffer,sizeof(FlashBuffer)) == ERROR)
  {
    return ERROR;
  }    
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_enum_update(DPID_MODE,mode);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_speed_handle
�������� : ���DPID_SPEED�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_speed_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪENUM
  unsigned char ret;
  unsigned char speed;
  
  speed = mcu_get_dp_download_enum(value,length);
  switch(speed)
  {
    case 0:
      
      break;
      
    case 1:
      
      break;
      
    case 2:
      
      break;
      
    case 3:
      
      break;
      
    case 4:
      
      break;
      
    default:
      
      break;
  }
  
  FlashBuffer.fan_speed = speed;
  LED_RGB_Control(255,FlashBuffer.fan_speed * 95,FlashBuffer.work_mode * 42);
  
  Earse_Flash(PARA_ADDR);
  if(Write_Flash(PARA_ADDR,(unsigned char *)&FlashBuffer,sizeof(FlashBuffer)) == ERROR)
  {
    return ERROR;
  }    
  
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_enum_update(DPID_SPEED,speed);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_lock_handle
�������� : ���DPID_LOCK�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_lock_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪBOOL
  unsigned char ret;
  //0:��/1:��
  unsigned char lock;
  
  lock = mcu_get_dp_download_bool(value,length);
  if(lock == 0)
  {
    //���ع�
//    LED_Control(LED2,0);
  }
  else
  {
    //���ؿ�
//    LED_Control(LED2,1);
  }
  
  Earse_Flash(PARA_ADDR);
  FlashBuffer.lock = lock;
  
  if(Write_Flash(PARA_ADDR,(unsigned char *)&FlashBuffer,sizeof(FlashBuffer)) == ERROR)
  {
    return ERROR;
  }   
  
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_bool_update(DPID_LOCK,lock);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}


/******************************************************************************
                                WARNING!!!                     
�˴���ΪSDK�ڲ�����,�밴��ʵ��dp����ʵ�ֺ����ڲ�����
******************************************************************************/
#ifdef SUPPORT_MCU_RTC_CHECK
/*****************************************************************************
�������� : mcu_write_rtctime
�������� : MCUУ�Ա���RTCʱ��
������� : ��
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void mcu_write_rtctime(unsigned char time[])
{
//  #error "���������RTCʱ��д�����,��ɾ������"
  /*
  time[0]Ϊ�Ƿ��ȡʱ��ɹ���־��Ϊ 0 ��ʾʧ�ܣ�Ϊ 1��ʾ�ɹ�
  time[1] Ϊ �� �� , 0x00 �� ʾ2000 ��
  time[2]Ϊ�·ݣ��� 1 ��ʼ��12 ����
  time[3]Ϊ���ڣ��� 1 ��ʼ��31 ����
  time[4]Ϊʱ�ӣ��� 0 ��ʼ��23 ����
  time[5]Ϊ���ӣ��� 0 ��ʼ��59 ����
  time[6]Ϊ���ӣ��� 0 ��ʼ��59 ����
  time[7]Ϊ���ڣ��� 1 ��ʼ�� 7 ������1��������һ
 */
  if(time[0] == 1)
  {
    //��ȷ���յ�wifiģ�鷵�صı���ʱ������ 
	 
  }
  else
  {
  	//��ȡ����ʱ�����ݳ���,�п����ǵ�ǰwifiģ��δ����
  }
}
#endif

#ifdef WIFI_TEST_ENABLE
/*****************************************************************************
�������� : wifi_test_result
�������� : wifi���ܲ��Է���
������� : result:wifi���ܲ��Խ��;0:ʧ��/1:�ɹ�
           rssi:���Գɹ���ʾwifi�ź�ǿ��/����ʧ�ܱ�ʾ��������
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void wifi_test_result(unsigned char result,unsigned char rssi)
{
//  #error "������ʵ��wifi���ܲ��Գɹ�/ʧ�ܴ���,��ɺ���ɾ������"
  extern unsigned char wifi_test_mode;                                                  //����ģʽ

  if(result == 0)
  {
    //����ʧ��
    if(rssi == 0x00)
    {
      //δɨ�赽����Ϊtuya_mdev_test·����,����
      LED_RGB_Control(255,0,0);
    }
    else if(rssi == 0x01)
    {
      //ģ��δ��Ȩ
      LED_RGB_Control(0,0,255);
    }
  }
  else
  {
    //���Գɹ�
    //rssiΪ�ź�ǿ��(0-100, 0�ź���100�ź���ǿ)
    if(rssi >= 30)              //�ź�ֵ����30%��ʾ����
      LED_RGB_Control(0,255,0);
    else
      LED_RGB_Control(255,0,0);
    wifi_test_mode = 0;
  }
  
}
#endif

#ifdef SUPPORT_MCU_FIRM_UPDATE
/*****************************************************************************
�������� : mcu_firm_update_handle
�������� : MCU����̼�����ģʽ
������� : value:�̼�������
           position:��ǰ���ݰ����ڹ̼�λ��
           length:��ǰ�̼�������(�̼�������Ϊ0ʱ,��ʾ�̼����������)
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
//  #error "���������MCU�̼���������,��ɺ���ɾ������"
  unsigned long addr;
 
  if(length == 0)
  {
#ifdef ENABLE_BOOT
    //�̼����ݷ������
    FlashBuffer.magic_code = FIREWARE_UPDATE_FLAG;
    
    if(Earse_Flash(PARA_ADDR) == ERROR)
      return ERROR;
    
    //д��������־
    if(Write_Flash(PARA_ADDR,(unsigned char *)&FlashBuffer,sizeof(FlashBuffer)) == ERROR)
      return ERROR;
    
    Reset();
#endif
  }
  else
  {
#ifdef ENABLE_BOOT
    //�̼����ݴ���
    addr = FIREWARE_ADDR_H;
     
    if(position % 1024 == 0)
    {
      if(Earse_Flash(addr + position) == ERROR)
        return ERROR;
    }
    
    if(Write_Flash(addr + position,(unsigned char *)value,length) == ERROR)
      return ERROR;
#endif
  }

  return SUCCESS;
}
#endif
/******************************************************************************
                                WARNING!!!                     
���º����û������޸�!!
******************************************************************************/

/*****************************************************************************
�������� : dp_download_handle
�������� : dp�·�������
������� : dpid:DP���
           value:dp���ݻ�������ַ
           length:dp���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERRO
ʹ��˵�� : �ú����û������޸�
*****************************************************************************/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  /*********************************
  ��ǰ����������·�/���ϱ����ݵ���                    
  ���庯������Ҫʵ���·����ݴ���
  �������Ҫ��������������APP��,����APP����Ϊ�·�ʧ��
  ***********************************/
  unsigned char ret;
  switch(dpid)
  {
    case DPID_SWITCH:
      //���ش�����
      ret = dp_download_switch_handle(value,length);
      break;
    case DPID_MODE:
      //ģʽ������
      ret = dp_download_mode_handle(value,length);
      break;
    case DPID_SPEED:
      //���ٴ�����
      ret = dp_download_speed_handle(value,length);
      break;
    case DPID_LOCK:
      //ͯ��������
      ret = dp_download_lock_handle(value,length);
      break;

  default:
    break;
  }
  return ret;
}
/*****************************************************************************
�������� : get_download_cmd_total
�������� : ��ȡ����dp�����ܺ�
������� : ��
���ز��� : �·������ܺ�
ʹ��˵�� : �ú����û������޸�
*****************************************************************************/
unsigned char get_download_cmd_total(void)
{
  return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}
