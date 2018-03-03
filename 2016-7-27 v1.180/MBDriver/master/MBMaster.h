/****************************************Copyright (c)**************************************************
**                                  广州致远电子有限公司
**
**                                 http://www.21cm.com.cn
**
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			MBMaster.h
** Last modified Date:  2005-01-11
** Last Version:		1.0
** Descriptions:		
**
**------------------------------------------------------------------------------------------------------
** Created by:			ZhouLishan
** Created date:		2005-01-11
** Version:				1.0
** Descriptions:		
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
********************************************************************************************************/
#ifndef __MBMASTER_H
#define	__MBMASTER_H

#include 	"MB_MFunction.h"
#include	"MODBUS2.h"
#include 	"OSFUNfile.h"

/***************************************************
** 使能MODBUS功能代码
****************************************************/
#define	READ_COILS_EN		1	// 读线圈
#define	READ_DIS_INPUT_EN	1	// 读离散量输入
#define	READ_HOLD_REG_EN	1	// 读保持寄存器
#define	READ_INPUT_REG_EN	1	// 读输入寄存器
#define	WRITE_SING_COIL_EN	1	// 写单个线圈
#define	WRITE_SING_REG_EN	1	// 写单个寄存器
#define	WRITE_MULT_COIL_EN	1	// 写多个线圈
#define	WRITE_MULT_REG_EN	1	// 写多个寄存器
#define	MASK_WRITE_REG_EN	1	// 处理屏蔽寄存器指令
#define	READ_WRITE_REG_EN	1	// 读写多个寄存器

#define	MAX_FUNCTION		(READ_COILS_EN     	+\
							 READ_DIS_INPUT_EN	+\
							 READ_HOLD_REG_EN	+\
							 READ_INPUT_REG_EN	+\
							 WRITE_SING_COIL_EN	+\
							 WRITE_SING_REG_EN	+\
							 WRITE_MULT_COIL_EN	+\
							 WRITE_MULT_REG_EN	+\
							 MASK_WRITE_REG_EN	+\
							 READ_WRITE_REG_EN	+\
							 1)


typedef struct __FUNCTION_ARRAY{
	uint8	 Code;
	void	 (*Function)(void *Response);
}FUNCTION_PACK_PDU;

typedef struct __FUNCTION_DATA_HANDLE{
	uint8	 Code;
	void	 (*Function)(uint8 *PDUPtr,void *ParameterPtr);
}FUNCTION_DATA_HANDLE;

/* 主机描述 */
typedef struct __MASTER_INFORMATION{
	uint8   Address;		  			// 地址
	uint8	LineProtocol;	  			// 链路层协议
	uint16	BaudRate;	  				// 波特率 = BaudRate * 100	

	uint8	Parity;						// 奇偶校验
	uint8	StopBit;					// 停止位

	uint8*	RequestPtr;					// 主机请求从帧指针
	uint8*  ReplyPtr;					// 应答帧指针

	uint8	Status;						// 主机状态
	void	*ParPtr;					// 参数结构指针

}MASTER_INFORMATION;


uint8 ModbusPoll(uint8 NodeID,uint8 FunctionCode,void *ParameterPtr);

void MBMasterIni(void);
//uint8 MReadHoldingReg(uint8 Adrress,uint8 StartAddr,uint16 Quantity,uint16 *DataPtr);
uint8 IdleModbus(void);

#endif
