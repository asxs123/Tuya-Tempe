/******************************************************************************
文件名称: Uart.c
文件标识: STC12SSA60S2
摘    要: Uart硬件操作函数
当前版本: V1.0	
完成日期: 2013.11.18
*******************************************************************************/
#define	USER_UART_GLOBALS
#include "include.h"
#include "wifi.h"



#define MAIN_Fosc		11059200L	//定义主时钟
#define	BaudRate		9600UL	//选择波特率
#define	Timer1_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate))		//Timer 1 重装值， 对应300KHZ
#define	Timer2_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate))		//Timer 2 重装值， 对应300KHZ


/*****************************************************************************
函数名称 : uart1_init
功能描述 : uart1初始化
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void uart1_init(void)
{
	S1_8bit();																																	//8位数据
	S1_USE_P30P31();																														//UART1 使用P30 P31口	默认
	AUXR &= ~(1<<4);																														//Timer stop		波特率使用Timer2产生
	AUXR |= 0x01;																																//S1 BRT Use Timer2;
	AUXR |=  (1<<2);																														//Timer2 set as 1T mode
	TH2 = (unsigned char)(Timer2_Reload >> 8);
	TL2 = (unsigned char)Timer2_Reload;
	AUXR |=  (1<<4);	//Timer run enable
	REN = 1;	//允许接收
	ES  = 1;	//允许中断
	EA = 1;		//允许全局中断
	PS=1;
	P3M1&=0XE7;
	P3M0|=0X18;
}
/*****************************************************************************
函数名称 : Uart_PutChar
功能描述 : 串口发送
输入参数 : dat:待发送数据
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void Uart_PutChar(unsigned char dat)
{
	SBUF = dat;
	while(!TI);
	TI = 0;
}
/*****************************************************************************
函数名称 : Uart1_ISR_Handle
功能描述 : Uart1串口中断处理函数
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void Uart1_ISR_Handle (void) interrupt UART1_VECTOR
{
	if(RI)
	{
		RI = 0;

		uart_receive_input(SBUF);		
	}
}
