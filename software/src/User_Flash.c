/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                                 http://www.tuya.com
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: User_Flash.c
**��        ��: Timer�û�����
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
#define USER_FLASH_GLOBAL
#include "include.h"



/*****************************************************************************
�������� : IapIdle
�������� : �ر�IAP
������� : ��
���ز��� : ��
*****************************************************************************/
static void IapIdle(void)
{
	IAP_CONTR = 0;                  //�ر�IAP����
	IAP_CMD = 0;                    //�������Ĵ���
	IAP_TRIG = 0;                   //��������Ĵ���
	IAP_ADDRH = 0x80;               //����ַ���õ���IAP����
	IAP_ADDRL = 0;
}
/*****************************************************************************
�������� : IapReadByte
�������� : ��ISP/IAP/EEPROM�����ȡһ�ֽ�
������� : addr:��ʼ��ַ
���ز��� : ��
*****************************************************************************/
static unsigned char IapReadByte(unsigned long addr)
{
	unsigned char dat;                       //���ݻ�����

	IAP_CONTR = ENABLE_IAP;         //ʹ��IAP
	IAP_CMD = CMD_READ;             //����IAP����
	IAP_ADDRL = addr;               //����IAP�͵�ַ
	IAP_ADDRH = addr >> 8;          //����IAP�ߵ�ַ
	IAP_TRIG = 0x5a;                //д��������(0x5a)
	IAP_TRIG = 0xa5;                //д��������(0xa5)
	_nop_();                        //�ȴ�ISP/IAP/EEPROM�������
	dat = IAP_DATA;                 //��ISP/IAP/EEPROM����
	IapIdle();                      //�ر�IAP����

	return dat;                     //����
}
/*****************************************************************************
�������� : IapProgramByte
�������� : дһ�ֽ����ݵ�ISP/IAP/EEPROM����
������� : addr:��ʼ��ַ/dat:��д������
���ز��� : ��
*****************************************************************************/
static void IapProgramByte(unsigned long addr, unsigned char dat)
{
    IAP_CONTR = ENABLE_IAP;         //ʹ��IAP
    IAP_CMD = CMD_PROGRAM;          //����IAP����
    IAP_ADDRL = addr;               //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;          //����IAP�ߵ�ַ
    IAP_DATA = dat;                 //дISP/IAP/EEPROM����
    IAP_TRIG = 0x5a;                //д��������(0x5a)
    IAP_TRIG = 0xa5;                //д��������(0xa5)
    _nop_();                        //�ȴ�ISP/IAP/EEPROM�������
    IapIdle();
}
/*****************************************************************************
�������� : IapEraseSector
�������� : ��������
������� : addr:��ʼ��ַ
���ز��� : ��
*****************************************************************************/
static void IapEraseSector(unsigned long addr)
{
    IAP_CONTR = ENABLE_IAP;         //ʹ��IAP
    IAP_CMD = CMD_ERASE;            //����IAP����
    IAP_ADDRL = addr;               //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;          //����IAP�ߵ�ַ
    IAP_TRIG = 0x5a;                //д��������(0x5a)
    IAP_TRIG = 0xa5;                //д��������(0xa5)
    _nop_();                        //�ȴ�ISP/IAP/EEPROM�������
    IapIdle();
}
/*****************************************************************************
�������� : Write_Flash
�������� : ��Flash�����ֽڱ��
������� : Addr_Sour:д���Դ����
           Addr_Obj:FLASH��Ŀ����ʼ��ַ
           Length:д����ֽ���
���ز��� : SUCCESS:д��ɹ�
            ERROR:д��ʧ��
ʹ��˵�� : ��
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
�������� : Earse_Flash
�������� : ��Flash����
������� : Addr_Obj:FLASH��Ŀ����ʼ��ַ
���ز��� : SUCCESS:�����ɹ�
            ERROR:����ʧ��
ʹ��˵�� : ��
*****************************************************************************/
uint8_t Earse_Flash(uint32_t Addr_Obj)
{
	IapEraseSector(Addr_Obj);
	
	return SUCCESS;
}
/*****************************************************************************
�������� : Read_Flash
�������� : ��ȡFLASH������
������� : pulData:Դ����
           ulAddress:FLASH����ʼ��ַ
           Length:��ȡ���ֽ���
���ز��� : true:��ȡ�ɹ�
           false:��ȡʧ��
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