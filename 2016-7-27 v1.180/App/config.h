/****************************************Copyright (c)**************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.zyinside.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name:          config.h
** Last modified Date: 2006-01-06
** Last Version:       v2.0
** Descriptions:       用户配置头文件
**
**------------------------------------------------------------------------------------------------------
** Created By:         黄绍斌
** Created date:       2005-12-31
** Version:            v1.0
** Descriptions:       创建
**
**------------------------------------------------------------------------------------------------------
** Modified by:        甘达
** Modified date:      2006-01-06
** Version:            v2.0
** Descriptions:       修改用于S3C2410
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
********************************************************************************************************/
#ifndef _CONFIG_H
#define _CONFIG_H

typedef unsigned char  uint8;                   // 无符号8位整型变量
typedef signed   char  int8;                    // 有符号8位整型变量
typedef unsigned short uint16;                  // 无符号16位整型变量
typedef signed   short int16;                   // 有符号16位整型变量
typedef unsigned int   uint32;                  // 无符号32位整型变量
typedef signed   int   int32;                   // 有符号32位整型变量
typedef float          fp32;                    // 单精度浮点数(32位长度)
typedef double         fp64;                    // 双精度浮点数(64位长度)


/********************************/
/*      uC/OS-II的特殊代码      */
/********************************/

#include   "INCLUDES.H"


/********************************/
/*        ARM的特殊代码         */
/********************************/
// 这一段无需改动
#include    <stdio.h>
#include    <ctype.h>
#include    <stdlib.h>


/********************************/
/*      应用程序配置            */
/********************************/
#include    <stdio.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <setjmp.h>
#include    <rt_misc.h>

// 以下根据需要改动
//#include    "uart.h"

/********************************/
/*       用户包含文件           */
/********************************/
// 以下根据需要改动
/**********************************
** ZLG/MODBUS TRU相关头文件与配置
***********************************/
#define 	UCOSII						// 在uC/OS-II下使用本必须定义该宏
#include	"MBMaster.h"					// MODBUS从机栈头文件

void IniUART1(uint32 bps);					// Modbus使用的串行口
void  UART1_Exception(void);				// Modbus使用串行口的中断服务函数

void TimersInit(void);						// Modbus使用定时器初始化
void T15_Exception(void);					// Modbus T15定时器中断服务函数
void T35_Exception(void);					// Modbus T35定时器中断服务函数
void T10ms_Exception(void);				// Modbus T10MS定时器中断服务函数



/********************************/
/*       用户定义内容           */
/********************************/
// 以下根据需要改动
extern uint8 OSWriteSingleReg(uint8  ID,
                              uint16 OutAddress,
                              uint16 OutValue);



#endif


/********************************************************************************************************
**                            End Of File
********************************************************************************************************/
