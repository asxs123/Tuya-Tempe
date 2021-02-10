/******************************************************************************
�ļ�����: Uart.c
�ļ���ʶ: STC8A8K64S4A12
ժ    Ҫ: UartӲ����������
��ǰ�汾: V1.0	
�������: 2021.01.23
*******************************************************************************/
#define	USER_UART_GLOBALS
#include "include.h"
#include "wifi.h"



bit busy;
/*****************************************************************************
�������� : uart3_init
�������� : uart3��ʼ��
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void uart3_init()
{
	S3CON = 0X10;		//8λ����,�ɱ䲨����
	TH2 = BRT >> 8;
	TL2 = BRT;
	AUXR = 0X14;	//Timer2 set as 1T mode ,Timer run enable
	REN = 1;	//��������
	busy = 0;
	IE2 = ES3;  //ʹ�ܴ����ж�
	EA = 1;		//����ȫ���ж�
	//S3BUF = 0x5a;            //���Ͳ�������
}
/*****************************************************************************
�������� : Uart_PutChar
�������� : ���ڷ���
������� : dat:����������
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void Uart_PutChar(uint8_t dat)
{
	while (busy);
  busy = 1;
	S3BUF = dat;
}
/*****************************************************************************
�������� : Uart1_ISR_Handle
�������� : Uart1�����жϴ�������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void Uart3Isr() interrupt 17
{
	if (S3CON & 0x02)
    {
        S3CON &= ~0x02;
			  busy = 0;
			P12 = !P12;                             //���Զ˿�
    }
		if (S3CON & 0x01)
    {
        S3CON &= ~0x01;
			P11 = !P11;
			uart_receive_input(S3BUF);             //���Զ˿�
    }
}