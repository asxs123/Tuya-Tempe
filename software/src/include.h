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
#include 		<intrins.h>
#include    <math.h>

#include        "STC8.H"
#include        "user_uart.h"
#include 				"user_key.h"
#include 				"user_timer.h"
#include 				"user_I2C.h"
#include 				"user_OLED.h"
#include 				"user_SHT30.h"
#include 				"user_ADC.h"

//==============================================================================
//�ܽŶ���
//==============================================================================
sbit K1 = P0^7;			//����1

#define MAIN_Fosc		22118400UL	//������ʱ��

//**************I2C*************
#define	Speed		    100000UL	  //����I2Cʱ��Ƶ��
#define MSSPEED     (MAIN_Fosc/Speed/2-4)/2

#define OLED_Address 0x78
#define SHT30_Address 0x88

//*************����*************
#define	BaudRate		9600UL	//ѡ������
#define BRT         (65536-MAIN_Fosc/BaudRate/4)


//==============================================================================
//��������
//==============================================================================
typedef struct {
	
	signed int temper;                                        //�¶�
  uint8_t humidity;                                         //ʪ��
	uint8_t Power;                                            //����
	uint16_t SAMPLING;                                        //����ʱ��
	uint32_t Timer_Sec_Value;                                 //��
	uint32_t Timer_Min_Value;                                 //��
	uint32_t Timer_Hour_Value;                                //ʱ
	
	
} TYPE_BUFFER_S;

MAINWORK_EXT TYPE_BUFFER_S FlashBuffer;


#endif