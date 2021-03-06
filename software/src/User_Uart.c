/******************************************************************************
文件名称: Uart.c
文件标识: STC8A8K64S4A12
摘    要: Uart硬件操作函数
当前版本: V1.0	
完成日期: 2021.01.23
*******************************************************************************/
#define	USER_UART_GLOBALS
#include "include.h"
#include "wifi.h"



bit busy;
/*****************************************************************************
函数名称 : uart3_init
功能描述 : uart3初始化
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void uart3_init()
{
	S3CON = 0X10;		//8位数据,可变波特率
	TH2 = BRT >> 8;
	TL2 = BRT;
	AUXR = 0X14;	//Timer2 set as 1T mode ,Timer run enable
	REN = 1;	//允许接收
	busy = 0;
	IE2 = ES3;  //使能串口中断
	EA = 1;		//允许全局中断
}
/*****************************************************************************
函数名称 : Uart_PutChar
功能描述 : 串口发送
输入参数 : dat:待发送数据
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void Uart_PutChar(uint8_t dat)
{
	while (busy);
  busy = 1;
	S3BUF = dat;
}
/*****************************************************************************
函数名称 : Uart1_ISR_Handle
功能描述 : Uart1串口中断处理函数
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void Uart3Isr() interrupt 17
{
	if (S3CON & 0x02)
    {
        S3CON &= ~0x02;
			  busy = 0;
    }
		if (S3CON & 0x01)
    {
        S3CON &= ~0x01;
				uart_receive_input(S3BUF);
    }
}
