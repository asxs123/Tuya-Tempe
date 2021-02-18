#ifndef __USER_PWM_H__
#define __USER_PWM_H__

#ifdef USER_PWM_GLOBALS
#define USER_PWM_EXT
#else
#define USER_PWM_EXT extern 
#endif

/*****************************************************************************
�������� : pwm_init
�������� : pwm��ʼ��
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void pwm_init(void);
/*****************************************************************************
�������� : LED_RGB_Control
�������� : RGB_LED����
������� : r/g/b:����ֵ
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void LED_RGB_Control(unsigned char r,unsigned char g,unsigned char b);

#endif

