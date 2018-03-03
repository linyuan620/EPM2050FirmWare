/****************************************Copyright (c)**************************************************
**                               Guangzou ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.zlgmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name: 			SST25VF016BDrv.h
** Last modified Date: 	2007-04-25
** Last Version: 		1.0
** Descriptions: 		SST25VF016B的操作函数
**
**------------------------------------------------------------------------------------------------------
** Created 	 By: 		Litiantian
** Created date: 		2007-04-25
** Version: 1.0
** Descriptions: 		First version
**
**------------------------------------------------------------------------------------------------------
** Modified by: 	
** Modified date: 	
** Version:
** Descriptions:  	
**
********************************************************************************************************/

#ifndef __F016BDRV_H 
#define __F016BDRV_H
#include"Globle.h"

/* 定义操作SST25VF016B所需的数据 */
enum PARAMETER_TYPE {Invalid};		// 定义1个常量,代表无效参数
enum OptResult{ERROR1,OK};											

typedef enum ERTYPE{Sec1,Sec8,Sec16,Chip} ErType;  
typedef enum IDTYPE{Manu_ID,Dev_ID,Jedec_ID} idtype;

#define MAX_ADDR		0x1FFFFF	// 定义芯片内部最大地址 
#define	SEC_MAX     	512			// 定义最大扇区号
#define SEC_SIZE		0x1000      // 扇区大小
/* 			宏,用于定义F016B的片选引脚 				*/
#define F016B_CS        (1 << 8) 		// P2.8口为25VF016B的片选，该脚为低时，SST25VF016B芯片被使能


/*******************************************************************************************
** 函数名称：SPIInit															
** 功能说明：初始化LPC2300系列ARM和SST25VF016B芯片的控制和通讯接口。				
** 入口参数：无																
** 出口参数：无																
*******************************************************************************************/
extern	void  SPIInit(void);

/************************************************************************
** 函数名称:SSTF016B_RD																										
** 函数功能:SST25VF016B的读函数,可选择读ID和读数据操作				
** 入口参数:
**			uint32 Dst：目标地址,范围 0x0 - MAX_ADDR（MAX_ADDR = 0x1FFFFF）				
**      	uint32 NByte:	要读取的数据字节数
**			uint8* RcvBufPt:接收缓存的指针			
** 出口参数:操作成功则返回OK,失败则返回ERROR		
** 注	 意:若某功能下,某一入口参数无效,可在该入口参数处填Invalid，该参数将被忽略
************************************************************************/
extern 	uint8 SSTF016B_RD(uint32 Dst, uint32 NByte,uint8* RcvBufPt);

/************************************************************************
** 函数名称:SSTF016B_RdID																										
** 函数功能:SST25VF016B的读ID函数,可选择读ID和读数据操作				
** 入口参数:
**			idtype IDType:ID类型。用户可在Jedec_ID,Dev_ID,Manu_ID三者里选择
**			uint32* RcvbufPt:存储ID变量的指针
** 出口参数:操作成功则返回OK,失败则返回ERROR		
** 注	 意:若填入的参数不符合要求，则返回ERROR
************************************************************************/
extern 	uint8 SSTF016B_RdID(idtype IDType, uint32* RcvbufPt);

/************************************************************************
** 函数名称:SSTF016B_WR											
** 函数功能:SST25VF016B的写函数，可写1个和多个数据到指定地址									
** 入口参数:
**			uint32 Dst：目标地址,范围 0x0 - MAX_ADDR（MAX_ADDR = 0x1FFFFF）				
**			uint8* SndbufPt:发送缓存区指针
**      	uint32 NByte:要写的数据字节数
** 出口参数:操作成功则返回OK,失败则返回ERROR		
** 注	 意:若某功能下,某一入口参数无效,可在该入口参数处填Invalid，该参数将被忽略
************************************************************************/
extern 	uint8 SSTF016B_WR(uint32 Dst,uint8* SndbufPt,uint32 NByte);

/************************************************************************
** 函数名称:SSTF016B_Erase												
** 函数功能:根据指定的扇区号选取最高效的算法擦除								
** 入口参数:
**			uint32 sec1：起始扇区号,范围(0~499)
**			uint32 sec2：终止扇区号,范围(0~499)
** 出口参数:操作成功则返回OK,失败则返回ERROR		
************************************************************************/
extern	uint8 SSTF016B_Erase(uint32 sec1, uint32 sec2);




#endif