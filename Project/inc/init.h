/**
  *************************************************************************
  * @file    init.h
  * @author  HYX.Huaqian
  * @version V1.0.0
  * @date    2021/02/17
  * @brief
  *************************************************************************
  * @attention
  *
  * <h2><center>&Copyright(c) 2014-2021 Xiamen HuaQian Studio</center></h2>
  *
  *************************************************************************
  */

/*************************     防重复包含定义     *************************/
#ifndef __INIT_H__
#define __INIT_H__

/*************************     系统头文件包含     *************************/


/*************************     私有头文件包含     *************************/
#include "common_cfg.h"
/*************************      导入命名空间      *************************/

/*************************      C++ 兼容定义      *************************/
#ifdef __cplusplus 
#if __cplusplus 
extern "C" {
#endif 
#endif

/*************************        全局枚举        *************************/

/*************************       全局宏定义       *************************/

/*************************     全局结构体定义     *************************/

/*************************      全局变量定义      *************************/

/*************************        函数声明        *************************/
	extern void p_init_gpio(void);
	extern void p_init_clk(void);
	extern void p_init_timer(void);
	extern void p_init_eeprom(void);
	extern void p_init_led(void);
	extern void p_init_IWDG(void);
	extern void p_init_BC20_uart(uint32_t BaudRate);
	extern void p_init_judge_reset(void);

/*************************      C++ 兼容定义      *************************/
#ifdef __cplusplus 
#if __cplusplus 
}
#endif 
#endif

/*************************     防重复包含定义     *************************/
#endif