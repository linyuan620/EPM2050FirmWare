/*****************************************************************************
  Copyright (C), 2011-8-3  Skyray Instrment Co.,LTD.
  File name:      global.h
  Author:       Version: V2.5       Date: 2011-8-3 11:18:18
  Description:    所有用户函数总头文件
  Others:
  Function List:
  History:
*****************************************************************************/
#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "lpc17xx_libcfg.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_rtc.h"

#include <includes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef		TRUE
#define		TRUE	1
#endif

#ifndef		FALSE
#define		FALSE	0
#endif

#ifndef		ERR
#define		ERR		0
#endif

#ifndef     OK
#define		OK		1
#endif

//-------------------------Menu Lcd start---------------------------------------------------
#define  DIS_AREA_X           239u
#define  DIS_AREA_Y           159u
/*
//--------<<< Use Configuration Wizard in Context Menu >>>----
*/
/*
//<h> 仪器设置
//	<o0> 菜单数目 <0-20:1>
//<o1> 历史记录点数 <100-2000:100>
//</h>
*/
#define MENUADDSIZE	15			//菜单总数目
// #define HistroyPoint  1000


/***********************************/
/* EEPROM Address  Don't try to change*/
/************************************/
#define INIT_FLAG		2
#define RECORDPOINT2 	5  //用4个字节

#define ConInf				10  //下位机参数配置。 0滤纸 1无  2 蜂鸣器 3键盘锁 4语言 5用途 6-9IP地址
#define TESTADD				15  //开机I2C自检地址。//20
#define RUNSETADD8   		25 //运行设置 值
#define RUNSETADD32   		45 //运行设置 值

//校准相关EEPROM 地址
#define JIAOZHUN_T1	100//校准值	
#define JIAOZHUN_T3	104//校准值	
#define JIAOZHUN_RH	108//校准值	
#define JIAOZHUN_P	112//校准值	
#define JIAOZHUN_F1	116//校准值	
#define JIAOZHUN_F2	120//校准值	
#define JIAOZHUN_BIAOPIAN	124//校准值	

#define JIAOZHUN_T1_TIME	130//校准值	  校准的时间点
#define JIAOZHUN_T3_TIME	138//校准值	
#define JIAOZHUN_RH_TIME	146//校准值	
#define JIAOZHUN_P_TIME		154//校准值	
#define JIAOZHUN_F1_TIME	162//校准值	
#define JIAOZHUN_F2_TIME	170//校准值	
#define JIAOZHUN_BIAOPIAN_TIME	188//校准值	
#define SYSRECORDPOINT2 200	//用4个字节
#define RESERVED_8 220	//
#define RESERVED_32 300	//

//传感器校准值扩展地址
#define JIAOZHUN_T4	       	500 //校准值
#define JIAOZHUN_T4_TIME	504 //校准值
#define JIAOZHUN_RH2	    512 //校准值
#define JIAOZHUN_RH2_TIME	516 //校准值

//光散k值地址
#define PM25_GS_32ADDR        520
#define PM10_GS_32ADDR        524
#define TSP_GS_32ADDR         528
#define SENSOR_SEL_8ADDR      532

#define RECORDSTAR			15500//不再使用，

/***********************************************
FLASH 地址定义  不要更改
***********************************************/
#define RECORDFLASHSTAR 0x60000 //前面都存放字库去了
#define RECORDSYSLOG    0XB0000 //系统日志地址开始

#define JIAOZHUN_BIAOPIAN_TEST_TIME	  360//校准u时测试时间	

#define JIAOZHUN_ON	1//校准开关	

/************************************************************************/

/**************************************************************
			控制端口配置
主要资源包括
AD :	T1  入口温度
		T2	加热管温度
		T3	采样室温度
		RH	湿度
		P1	气压
		F2	标况流量  （F1	工况流量 由F2算出）
DA:
	1、泵操作//可能用DA
	2、采集计数部分的高压控制

TTL: 能用的有2组 TTL0  TTL2 每组8路，其中0用于输出，2用于输入

	 1、压纸电机 固件只负责TTL输出高 （何时停止要配合机械结构确定）
	 2、走纸电机 TTL输出 配合旋转编码器设置。
	 3、加热开关
	 4、风扇
*****************************************************************/
#define	NUN_AD		 20
#define	NUN_SENSOR	 11
#define	NUN_DA		 10

//AD端口
#define	AD_SHIDU	   9      //                                   测试仓湿度传感器ADC端口号
#define	AD_WENDU1	   14	  //大气环境温度 工况温度              采样口温度传感器ADC端口号
#define	AD_WENDU2	   13	  //                                   加热管温度温度传感器adc端口号
#define	AD_WENDU3	   10	  //收集室样品温度 标况                测试仓温度传感器ADC端口号
#define	AD_LIULIANG	   6  //                                     流量控制器传感器ADC端口号
#define	AD_QIYA		   11   //                                   大气压气压传感器ADC端口号
#define AD_CYK_SHIDU   15 //                                     采样口湿度传感器ADC端口号                                     
#define AD_TCQ_WENDU   12 //                                     探测器温度传感器ADC端口号  

#define	VCC_5		 0
#define	VCC_12		 1
#define	VSS_12		 2
#define	VCC_24		 3

//DA端口
#define	DA_PUMP	     2

//TTL端口配置
#define TTL_YAZHIMOTO 	 2
#define TTL_ZHOUZHIMOTO  3
#define TTL_JIAREI 		 4
#define TTL_GAOYA 	     5
#define GAOYA_OPEN 	     0
#define GAOYA_CLOSE 	 1
#define TTL_PUMP 	     6

#define TTL_BRAY_Ctrl1 	     3
#define TTL_BRAY_Ctrl2 	     4


#define TTL_0 			 0 //输出用
#define TTL_2 			 2 //输出用
#define TTL_1 			 1 //输入用

#define TTL_HIGH 		 1
#define TTL_LOW  		 0

//传感器 名称定义
#define	CGQ_WENDU1_KL	 0	 //采样口温度 算工况流量的 昆仑海岸
#define	CGQ_SHIDU		 1   //测试舱湿度传感器
#define	CGQ_LIULIANG	 2
#define	CGQ_QIYA	     3
#define	CGQ_WENDU2		 4	 //加热管温度，保护用
#define	CGQ_WENDU3		 5	 //测试舱温度 
#define CGQ_TCQ_WENDU    6   //探测器温度传感器
#define CGQ_CYK_SHIDU_KL 7   //采样口湿度  昆仑海岸
#define	CGQ_WENDU1_XY	 8	 //采样口温度 算工况流量的 北京星仪
#define CGQ_CYK_SHIDU_XY 9   //采样口湿度  北京星仪
#define CGQ_LIULIANG_HONEYWELL 10   //流量 霍尼韦尔

//对外输出开关

/*
//Control END
 */

/***************************************************
中断优先级	  数值低的优先级高
优先级组 可以抢占。嵌套
同组间不嵌套。
有5个位控制优先级  高2为是组，低3位是亚优先级
****************************************************/
//0组
#define PRIO_GROUP_I2C	      0X00
#define PRIO_GROUP_RTC	      0X00
#define PRIO_I2C  0X00
#define PRIO_RTC  0X01

//1组

//2组
#define PRIO_GROUP_UART0	  0X02
#define PRIO_GROUP_UART1	  0X02
#define PRIO_GROUP_UART2	  0X02
#define PRIO_GROUP_UART3	  0X02
#define PRIO_UART0	  0X00
#define PRIO_UART1	  0X01
#define PRIO_UART2	  0X02
#define PRIO_UART3	  0X03


//3组
#define PRIO_GROUP_T0	      0X03
#define PRIO_GROUP_T1	      0X03
#define PRIO_GROUP_T2	      0X03
#define PRIO_GROUP_T3	      0X03
#define PRIO_T0	      0X00
#define PRIO_T1	      0X01
#define PRIO_T2	      0X02
#define PRIO_T3	      0X03


#define FUJITIME	      1000



/*
//<e>	版本更换
// <s2> Change Firmware Version(En)
// <s3> Change Hardware Version(En)
// <s5> Change Firmware Version(Ch)
// <s6> Change Hardware Version(Ch)
//</e>
 */

#define  FI1_EN  "      Skyray-instrument Portable -"
#define  FI2_EN  "     Water Analyzer - Heavy metal,-"
#define  FI3_EN	 "firmware software  V1.183"
#define  HW_EN   "HW Version: PM2.5 B1"
#define  FI1_CH  "     天瑞大气颗粒物监测系统"
#define  FI2_CH  "分析仪固件软件 V1.183"
#define  HW_CH   "硬件版本: PM2.5 B1"


#define  MENU_SIZE            (MENUADDSIZE*2 + 49)
#define  CAL_QUERY_PAGE       10

#define  CHINA_Y              13
#define  CHINA_SX             0
#define  CHINA_EX             127

#define  NUM6_SX              16                           	//24 24*12 6个输入数字起点X
#define  NUM6_SY              28                           	//24*12 6个输入数字起点Y

#define  EFONT_12             6
#define  EFONT_16             8
#define  EFONT_24             12

#define  BUTTON_SX            24
#define  BUTTON_SY            36

#define  MENU_SX              4
#define  MENU0_SY             1
#define  MENU1_SY             21
#define  MENU2_SY             36
#define  MENU3_SY             51

#define  WordNum             51			  //自定义汉字库大小
#define  ADRANGESEL_10            0x03			  //采集范围 0-10 采集回来要乘以2倍。
#define  ADRANGESEL_5             0

#define  MAINDIS_SX           32      						//主屏TEM,显示
#define  MAINDIS_SX24         22
#define  SPACE126X            6
#define  SPACE2412X           12
//-------------------------Menu Lcd end---------------------------------------------------

//-------------------------Key start---------------------------------------------------
#define KEY_REPEAT_TIME      (0xffE0)//(0xffd0) //(0xff05)
#define KEY_SERIES_TIME    	 (0xffE4)//(0xffE4)//(0xffef)   //(0xff24)  
#define RETURN_MENU0_TIME    (0x1500)

#define KEY_UP               0x000001b8          			//0x00001c30
#define KEY_DOWN             0x000001f0          			//0x00001870
#define KEY_START            0x000000f8          			//0x00001470
#define KEY_ENTER            0x00000178          			//0x00001c60
#define KEY_RIGHT            0x000001d8
#define KEY_NULL             0x000001f8
#define KEY_ESC              0x000001e8          			//0x000001e8

#define TDAIDIN              WenDuT1  // WenDuT3        正式发货版本用T1必须接上T1  			//0x000001e8

#define KEY_FILTER			 0x000001f8
#define FLASH_ADD_ZK1212	 216640//1212字库开始地址
#define FLASH_ADD_ZK1616	 0//1212字库开始地址

//-------------------------Key end---------------------------------------------------
#define ERRORRECORDPOINT 	15480

typedef unsigned char  uint8;                   			// 无符号8位整型变量
typedef signed   char  int8;                    			// 有符号8位整型变量
typedef unsigned short uint16;                  			// 无符号16位整型变量
typedef signed   short int16;                   			// 有符号16位整型变量
typedef unsigned int   uint32;                  			// 无符号32位整型变量
typedef signed   int   int32;                   			// 有符号32位整型变量
typedef float          fp32;                    			// 单精度浮点数(32位长度)
typedef double         fp64;                    			// 双精度浮点数(64位长度)
typedef signed   short DATA_TYPE;

typedef struct
{
    uint16 year;
    uint8 month;
    uint8 date;
    uint8 hour;
    uint8 minute;
    uint8 second;
} TimeDat;

typedef struct {
    TimeDat		recordTimeStart;
    TimeDat		recordTimeEnd;
    fp32		recordResult;
    fp32		I;
    fp32		I0;
    fp32  		TV;
    fp32		RH;
    fp32		T1;
    fp32  		P ;//Ts;//改为大气压力
    fp32		recordResultG;//Te;//改为工况结果
    fp32		MAS;
} MEAS_RECORD;

typedef struct {
    fp32	 I;
    fp32   abs_offset;
    uint8  order;
} CNT_RECORD;

#include "dio.h"
#include "uarts.h"
#include "times.h"
#include "i2c.h"
#include "public.h"
#include "init.h"
#include "da.h"
#include "lcd.h"
#include "hzk.h"
#include "menu.h"
#include "picture.h"
#include "key.h"
#include "rtc.h"
#include "ds2438.h"
#include "Print.h"
#include "findBaseLine.h"
#include "FPGA_Contorl.h"
#include "Output.h"
#include "spi.h"
#include "Modbus.h"
#include "Measure.h"
#include "config.h"
#include "w5100_drive.h"
#include "tcpip.h"
#include "w5100.h"
#include "keyshow.h"
#include "store.h"
#include "DTU.h"
#include "SHELL.h"

extern OS_EVENT *OSLcdUsedMutex;	//互斥信号量
extern OS_EVENT *OSI2CUsedMutex;
extern OS_EVENT *OSFlashUsedMutex;

#endif
