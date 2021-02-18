#ifndef __USER_PWM_H__
#define __USER_PWM_H__

#ifdef USER_PWM_GLOBALS
#define USER_PWM_EXT
#else
#define USER_PWM_EXT extern 
#endif

/*****************************************************************************
函数名称 : pwm_init
功能描述 : pwm初始化
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void pwm_init(void);
/*****************************************************************************
函数名称 : LED_RGB_Control
功能描述 : RGB_LED控制
输入参数 : r/g/b:亮度值
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void LED_RGB_Control(unsigned char r,unsigned char g,unsigned char b);

#endif

