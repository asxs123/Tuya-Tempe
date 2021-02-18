/****************************************Copyright (c)*************************
**                               版权所有 (C), 2015-2020, 涂鸦科技
**
**                                 http://www.tuya.com
**
**--------------文件信息-------------------------------------------------------
**文   件   名: User_Flash.c
**描        述: Timer用户函数
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
#define USER_FLASH_GLOBAL
#include "include.h"



/*****************************************************************************
函数名称 : IapIdle
功能描述 : 关闭IAP
输入参数 : 无
返回参数 : 无
*****************************************************************************/
static void IapIdle(void)
{
	IAP_CONTR = 0;                  //关闭IAP功能
	IAP_CMD = 0;                    //清除命令寄存器
	IAP_TRIG = 0;                   //清除触发寄存器
	IAP_ADDRH = 0x80;               //将地址设置到非IAP区域
	IAP_ADDRL = 0;
}
/*****************************************************************************
函数名称 : IapReadByte
功能描述 : 从ISP/IAP/EEPROM区域读取一字节
输入参数 : addr:起始地址
返回参数 : 无
*****************************************************************************/
static unsigned char IapReadByte(unsigned long addr)
{
	unsigned char dat;                       //数据缓冲区

	IAP_CONTR = ENABLE_IAP;         //使能IAP
	IAP_CMD = CMD_READ;             //设置IAP命令
	IAP_ADDRL = addr;               //设置IAP低地址
	IAP_ADDRH = addr >> 8;          //设置IAP高地址
	IAP_TRIG = 0x5a;                //写触发命令(0x5a)
	IAP_TRIG = 0xa5;                //写触发命令(0xa5)
	_nop_();                        //等待ISP/IAP/EEPROM操作完成
	dat = IAP_DATA;                 //读ISP/IAP/EEPROM数据
	IapIdle();                      //关闭IAP功能

	return dat;                     //返回
}
/*****************************************************************************
函数名称 : IapProgramByte
功能描述 : 写一字节数据到ISP/IAP/EEPROM区域
输入参数 : addr:起始地址/dat:待写入数据
返回参数 : 无
*****************************************************************************/
static void IapProgramByte(unsigned long addr, unsigned char dat)
{
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_PROGRAM;          //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_DATA = dat;                 //写ISP/IAP/EEPROM数据
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
    IapIdle();
}
/*****************************************************************************
函数名称 : IapEraseSector
功能描述 : 扇区擦除
输入参数 : addr:起始地址
返回参数 : 无
*****************************************************************************/
static void IapEraseSector(unsigned long addr)
{
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_ERASE;            //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
    IapIdle();
}
/*****************************************************************************
函数名称 : Write_Flash
功能描述 : 对Flash进行字节编程
输入参数 : Addr_Sour:写入的源数据
           Addr_Obj:FLASH的目标起始地址
           Length:写入的字节数
返回参数 : SUCCESS:写入成功
            ERROR:写入失败
使用说明 : 无
*****************************************************************************/
uint8_t Write_Flash(uint32_t Addr_Obj,uint8_t *Addr_Sour,uint16_t Length)
{
	uint16_t i;
	for(i = 0; i < Length; i ++)
	{
		IapProgramByte(Addr_Obj,*Addr_Sour);
		Addr_Obj ++;
		Addr_Sour ++;
	}
	return SUCCESS;
}
/*****************************************************************************
函数名称 : Earse_Flash
功能描述 : 对Flash擦除
输入参数 : Addr_Obj:FLASH的目标起始地址
返回参数 : SUCCESS:擦除成功
            ERROR:擦除失败
使用说明 : 无
*****************************************************************************/
uint8_t Earse_Flash(uint32_t Addr_Obj)
{
	IapEraseSector(Addr_Obj);
	
	return SUCCESS;
}
/*****************************************************************************
函数名称 : Read_Flash
功能描述 : 读取FLASH的数据
输入参数 : pulData:源数据
           ulAddress:FLASH的起始地址
           Length:读取的字节数
返回参数 : true:读取成功
           false:读取失败
*****************************************************************************/
uint8_t Read_Flash(uint8_t *pulData,uint32_t ulAddress,uint32_t Length)
{
	uint16_t i;
	for(i = 0; i < Length; i ++)
	{
		*pulData = IapReadByte(ulAddress);
		ulAddress ++;
		pulData ++;
	}
		
	return SUCCESS;
}