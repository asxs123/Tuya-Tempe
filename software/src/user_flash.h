#ifndef __USER_FLASH_H__
#define __USER_FLASH_H__

#ifdef USER_FLASH_GLOBALS
#define USER_FLASH_EXT
#else
#define USER_FLASH_EXT extern 
#endif


//==============================================================================
//��������
//==============================================================================
#define CMD_IDLE    0               //����ģʽ
#define CMD_READ    1               //IAP�ֽڶ�����
#define CMD_PROGRAM 2               //IAP�ֽڱ������
#define CMD_ERASE   3               //IAP������������
#define ENABLE_IAP  0x82            //if SYSCLK<20MHz


/*****************************************************************************
�������� : write_flash
�������� : ��Flash�����ֽڱ��
������� : Addr_Sour:д���Դ����
           Addr_Obj:FLASH��Ŀ����ʼ��ַ
           Length:д����ֽ���
���ز��� : SUCCESS:д��ɹ�
            ERROR:д��ʧ��
ʹ��˵�� : ��
*****************************************************************************/
uint8_t Write_Flash(uint32_t Addr_Obj,uint8_t *Addr_Sour,uint16_t Length);

/*****************************************************************************
�������� : Earse_Flash
�������� : ��Flash����
������� : Addr_Obj:FLASH��Ŀ����ʼ��ַ
���ز��� : SUCCESS:�����ɹ�
            ERROR:����ʧ��
ʹ��˵�� : ��
*****************************************************************************/
uint8_t Earse_Flash(uint32_t Addr_Obj);

/*****************************************************************************
�������� : Read_Flash
�������� : ��ȡFLASH������
������� : pulData:Դ����
           ulAddress:FLASH����ʼ��ַ
           Length:��ȡ���ֽ���
���ز��� : true:��ȡ�ɹ�
           false:��ȡʧ��
*****************************************************************************/
uint8_t Read_Flash(uint8_t *pulData,uint32_t ulAddress,uint32_t Length);


#endif