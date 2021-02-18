#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#ifdef MAINWORK_GLOBALS
#define MAINWORK_EXT
#else
#define MAINWORK_EXT extern 
#endif

//==============================================================================
typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;

//==============================================================================
#include		<stdio.h>
#include		<stdlib.h>
#include		<string.h>
#include 		<intrins.h>

#include        "STC15Fxxxx.h"
#include        "user_uart.h"
#include 				"user_key.h"
#include 				"user_pwm.h"
#include 				"user_timer.h"
#include 				"user_flash.h"

//==============================================================================
//�ܽŶ���
//==============================================================================
sbit LED_R = P2^5;	//LED1
sbit LED_G = P2^6;	//LED2
sbit LED_B = P2^7;	//LED3

sbit DK1 = P3^3;		//�̵���
sbit BEEP = P3^4;		//������
sbit WIFI_VCC = P5^4;

sbit K1 = P1^4;			//����1
sbit K2 = P1^3;			//����2
sbit K3 = P1^5;			//����3
sbit K4 = P0^0;			//����4

sbit TEMPER = P1^6;		//��ʪ��

#define		wifi_power_on()			WIFI_VCC = 0
#define		wifi_power_off()		WIFI_VCC = 1
//
#define		Led_R_On()					LED_R = 0
#define		Led_R_Off()					LED_R = 1
//
#define		Led_G_On()					LED_G = 0
#define		Led_G_Off()					LED_G = 1
//
#define		Led_B_On()					LED_B = 0
#define		Led_B_Off()					LED_B = 1

//
#define		relay_on()					DK1 = 1
#define		relay_off()					DK1 = 0
//
#define		beep_on()						BEEP = 1
#define		beep_off()					BEEP = 0


//==============================================================================
//��������
//==============================================================================
#define 	PARA_ADDR 					0x0400

#define		SUCCESS							1
#define		ERROR								0

//============================================================================
//�������Ͷ���
//��λ����,�ɰ�λ��,ͬʱ�ϱ����ִ���
//============================================================================
#define		ERROR_NO					0x00
#define		ERROR_E1					0x01
#define		ERROR_E2					0x02
#define		ERROR_E3					0x04
#define		ERROR_E4					0x08

//==============================================================================
//��������
//==============================================================================
typedef struct {
  uint32_t magic_code;
  uint8_t power_switch;                                                         //����
  uint8_t work_mode;                                                            //ģʽ
  uint8_t fan_speed;                                                            //����
  uint8_t lock;                                                                 //ͯ��
  uint16_t pm_25;                                                               //PM2.5
  uint8_t filter;                                                               //��оʹ�ðٷֱ�
  signed char temper;                                                                //�¶�
  uint8_t humidity;                                                             //ʪ��
  uint16_t tvoc;                                                                //TVOC
  uint32_t run_time;                                                            //�ۼ�����ʱ��
  uint8_t falut;                                                                //����

} TYPE_BUFFER_S;

MAINWORK_EXT TYPE_BUFFER_S FlashBuffer;																											//flash�洢��
MAINWORK_EXT uint16_t temper_current;																												//��ǰ�¶�
MAINWORK_EXT uint8_t error_type;																														//��������

/*****************************************************************************
�������� : beep_control
�������� : ����������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void beep_control(void);


#endif