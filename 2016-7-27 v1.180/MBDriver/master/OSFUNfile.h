/****************************************Copyright (c)**************************************************
**                                  广州致远电子有限公司
**
**                                 http://www.21cm.com.cn
**
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			OSFUNfile.h
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

#ifndef OSFUNFILE_H
#define OSFUNFILE_H
//*********************************************************************************
// 函数名称：OSReadCoils
// 输入参数：ID，从机地址
//			 StartAddress，线圈起始地址
//			 Quantity，线圈数量
//			 ReaValuePtr，保存线圈值的指针
// 输出参数：完成代码：(见异常代码表)
// 功能描述：读取线圈值。0x01
//*********************************************************************************
 uint8 OSReadCoils(uint8 ID, 
 					 uint16 StartAddress,
 					 uint16 Quantity,
 					 uint8 *ReaValuePtr);	
//*********************************************************************************
// 函数名称：OSReadCoils
// 输入参数：ID，从机地址
//			 StartAddress，线圈起始地址
//			 Quantity，线圈数量
//			 ReaValuePtr，保存线圈值的指针
// 输出参数：完成代码：(见异常代码表)
// 功能描述：读取线圈值。0x02
//*********************************************************************************
 uint8 OSReadDisInputs(uint8 ID, 
 					 uint16 StartAddress,
 					 uint16 Quantity,
 					 uint8 *ReaValuePtr);
//*********************************************************************************
// 函数名称：OSReadHoldReg
// 输入参数：ID，从机地址
//			 StartAddress，寄存器起始地址
//			 Quantity，寄存器数量
//			 ReaValuePtr，保存读出寄存器值的指针
// 输出参数：完成代码：(见异常代码表)
// 功能描述：读取保持寄存器值，指令代码为：0x03。
//*********************************************************************************
 uint8 OSReadHoldReg(uint8 ID, 
 					 uint16 StartAddress,
 					 uint16 Quantity,
 					 uint16 *ReaValuePtr);		// 读保持寄存器

//*********************************************************************************
// 函数名称：OSReadInputReg
// 输入参数：ID，从机地址
//			 StartAddress，寄存器起始地址
//			 Quantity，寄存器数量
//			 ReaValuePtr，保存读出寄存器值的指针
// 输出参数：完成代码：(见异常代码表)
// 功能描述：读取输入寄存器值，指令代码为：0x04。
//*********************************************************************************
 uint8 OSReadInputReg(uint8 ID, 
 					 uint16 StartAddress,
 					 uint16 Quantity,
 					 uint16 *ReaValuePtr);		

//*********************************************************************************
// 函数名称：OSWriteSingleCoil
// 输入参数：ID，从机地址
//			 OutAddress，写入的地址
//			 OutValue，写入的值COIL_OFF、COIL_ON
// 输出参数：完成代码：(见异常代码表)
// 功能描述：写单线圈操作。
//*********************************************************************************
 uint8 OSWriteSingleCoil(uint8  ID,
 						 uint16 OutAddress,
 						 uint16 OutValue); 					 

//*********************************************************************************
// 函数名称：OSWriteMultipleCoils
// 输入参数：ID，从机地址
//			 StartAddress，写入的地址
//			 Quantity，写入的数量
//			 ValuePtr，写入数据的指针
// 输出参数：完成代码：(见异常代码表)
// 功能描述：写多线圈操作。0x0f
//*********************************************************************************
uint8  OSWriteMultipleCoils(uint8 ID,uint16 StartAddress,uint16 Quantity,uint8 *ValuePtr);

//*********************************************************************************
// 函数名称：MaskWriteReg
// 输入参数：ID，从机地址
//			 Addr，写入的地址(0x0001~0xffff)
//			 And_Mask，屏蔽写与数据
//			 Or_Mask，屏蔽写或数据
// 输出参数：完成代码：(见异常代码表)
// 功能描述：屏蔽写寄存器操作。如果Or_Mask值为零，那么结果是当前内容和And_Mask的简单逻辑AND（与）；
//			 如果And_Mask值为零，结果等于Or_Mask值。0x16
//*********************************************************************************
uint8  OSMaskWriteReg(uint8 ID, 
 					uint16 Addr,
 					uint16 And_Mask,
 					uint16 Or_Mask);
//*********************************************************************************
// 函数名称：ReadWriteMultipleReg2PDU
// 输入参数：ID，从机地址
//			 WStartAddr，写入寄存器的地址(0x0001~0xffff)
//           WQuantity，写入寄存器的数量
//			 RStartAddr，读出寄存器的地址(0x0001~0xffff)
//			 RQuantity，读出寄存器的数量
//			 DataPtr，写入寄存器的指针和读出数据存放指针
// 输出参数：完成代码：(见异常代码表)
// 功能描述：写多寄存器操作。0x17
//*********************************************************************************
uint8  OSReadWriteMultipleReg(uint8 ID, 
 					 	uint16 WStartAddr,
 					 	uint16 WQuantity,
 					 	uint16 RStartAddr,
 					 	uint16 RQuantity,
 					 	uint16 *DataPtr); 		

uint8  OSWriteMultipleReg(uint8 ID, 
 					 	uint16 StartAddress,
 					 	uint16 Quantity,
 					 	uint16 *ValuePtr);
#endif
