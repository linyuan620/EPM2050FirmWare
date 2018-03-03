/****************************************Copyright (c)**************************************************
**                                  广州致远电子有限公司
**
**                                 http://www.21cm.com.cn
**
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			OSFUNfile.c
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
#include "config.h"


//*********************************************************************************
// 函数名称：OSReadCoils
// 输入参数：ID，从机地址
//			 StartAddress，线圈起始地址(0x0000~0xffff)
//			 Quantity，线圈数量(0x001~0x07d0)
//			 ReaValuePtr，保存线圈值的指针
// 输出参数：完成代码：(见异常代码表)
// 功能描述：读取线圈值。0x01
//*********************************************************************************
 uint8 OSReadCoils(uint8 ID, 
 					 uint16 StartAddress,
 					 uint16 Quantity,
 					 uint8 *ReaValuePtr)		
{
	READ_PARAMET Parameter;
	Parameter.RStartAddr = StartAddress;
   	Parameter.RQuantity = Quantity;
    Parameter.DataPtr = (uint16 *)ReaValuePtr;
    return ModbusPoll(ID,0x01,&Parameter);
}

//*********************************************************************************
// 函数名称：OSReadDisInputs
// 输入参数：ID，从机地址
//			 StartAddress，离散量输入起始地址(0x0000~0xffff)
//			 Quantity，离散量输入数量(0x001~0x07d0)
//			 ReaValuePtr，保存离散量输入值的指针
// 输出参数：完成代码：(见异常代码表)
// 功能描述：读取离散量输入值。0x02
//*********************************************************************************
 uint8 OSReadDisInputs(uint8 ID, 
 					 uint16 StartAddress,
 					 uint16 Quantity,
 					 uint8 *ReaValuePtr)		
{
	READ_PARAMET Parameter;
	Parameter.RStartAddr = StartAddress;
   	Parameter.RQuantity = Quantity;
    Parameter.DataPtr = (uint16 *)ReaValuePtr;
    return ModbusPoll(ID,0x02,&Parameter);
}

//*********************************************************************************
// 函数名称：OSReadHoldReg
// 输入参数：ID，从机地址
//			 StartAddress，寄存器起始地址(0x0000~0xffff)
//			 Quantity，寄存器数量(0x0001~0x007d)
//			 ReaValuePtr，保存读出寄存器值的指针
// 输出参数：完成代码：(见异常代码表)
// 功能描述：读取保持寄存器值。0x03
//*********************************************************************************
 uint8 OSReadHoldReg(uint8 ID, 
 					 uint16 StartAddress,
 					 uint16 Quantity,
 					 uint16 *ReaValuePtr)		// 读保持寄存器
{
	READ_PARAMET Parameter;
	Parameter.RStartAddr = StartAddress;
   	Parameter.RQuantity = Quantity;
    Parameter.DataPtr =  ReaValuePtr;
    return ModbusPoll(ID,0x03,&Parameter);
}

//*********************************************************************************
// 函数名称：OSReadInputReg
// 输入参数：ID，从机地址
//			 StartAddress，输入寄存器起始地址(0x0000~0xffff)
//			 Quantity，输入寄存器数量(0x0001~0x007d)
//			 ReaValuePtr，保存读出输入寄存器值的指针
// 输出参数：完成代码：(见异常代码表)
// 功能描述：读取输入寄存器值。0x04
//*********************************************************************************
 uint8 OSReadInputReg(uint8 ID, 
 					 uint16 StartAddress,
 					 uint16 Quantity,
 					 uint16 *ReaValuePtr)		
{
	READ_PARAMET Parameter;
	Parameter.RStartAddr = StartAddress;
   	Parameter.RQuantity = Quantity;
    Parameter.DataPtr =  ReaValuePtr;
    return ModbusPoll(ID,0x04,&Parameter);
}

//*********************************************************************************
// 函数名称：OSWriteSingleCoil
// 输入参数：ID，从机地址
//			 OutAddress，写入的地址(0x0000~0xffff)
//			 OutValue，写入的值COIL_OFF(0x0000)、COIL_ON(0xff00)
// 输出参数：完成代码：(见异常代码表)
// 功能描述：写单线圈操作。0x05
//*********************************************************************************
 uint8 OSWriteSingleCoil(uint8  ID,
 						 uint16 OutAddress,
 						 uint16 OutValue)
{
	WRITE_PARAMET Parameter;
	Parameter.WStartAddr = OutAddress;
	Parameter.DataPtr = &OutValue;
    return ModbusPoll(ID,0x05,&Parameter);
}

//*********************************************************************************
// 函数名称：OSWriteSingleReg
// 输入参数：ID，从机地址
//			 OutAddress，写入的地址(0x0000~0xffff)
//			 OutValue，写入的值(0x0000~0xffff)
// 输出参数：完成代码：(见异常代码表)
// 功能描述：写单寄存器操作。0x06
//*********************************************************************************
 uint8 OSWriteSingleReg(uint8  ID,
 						 uint16 OutAddress,
 						 uint16 OutValue)
{
	WRITE_PARAMET Parameter;
	Parameter.WStartAddr = OutAddress;
	Parameter.DataPtr = &OutValue;
    return ModbusPoll(ID,0x06,&Parameter);
}

//*********************************************************************************
// 函数名称：OSWriteMultipleCoils
// 输入参数：ID，从机地址
//			 StartAddress，写入的地址(0x0001~0xffff)
//			 Quantity，写入的数量(0x0001~0x07b0)
//			 ValuePtr，写入数据的指针
// 输出参数：完成代码：(见异常代码表)
// 功能描述：写多线圈操作。0x0f
//*********************************************************************************
uint8  OSWriteMultipleCoils(uint8 ID, 
 					 	uint16 StartAddress,
 					 	uint16 Quantity,
 					 	uint8 *ValuePtr)	
{
	WRITE_PARAMET Parameter;
	Parameter.WStartAddr = StartAddress;
   	Parameter.WQuantity = Quantity;
    Parameter.DataPtr =  (uint16*)ValuePtr;
    return ModbusPoll(ID,0x0f,&Parameter);
}

//*********************************************************************************
// 函数名称：WriteMultipleReg
// 输入参数：ID，从机地址
//			 StartAddress，写入的地址(0x0001~0xffff)
//			 Quantity，写入的数量(0x0001~0x0078)
//			 ValuePtr，写入数据的指针
// 输出参数：完成代码：(见异常代码表)
// 功能描述：写多寄存器操作。0x10
//*********************************************************************************
uint8  OSWriteMultipleReg(uint8 ID, 
 					 	uint16 StartAddress,
 					 	uint16 Quantity,
 					 	uint16 *ValuePtr)	
{
	WRITE_PARAMET Parameter;
	Parameter.WStartAddr = StartAddress;
   	Parameter.WQuantity = Quantity;
    Parameter.DataPtr =  ValuePtr;
    return ModbusPoll(ID,0x10,&Parameter);
}

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
 					uint16 Or_Mask)	
{
	MASK_WRITE_REG Parameter;
	Parameter.RefAddress = Addr;
   	Parameter.And_Mask = And_Mask;
    Parameter.OR_Mask =  Or_Mask;
    return ModbusPoll(ID,0x16,&Parameter);
}


//*********************************************************************************
// 函数名称：ReadWriteMultipleReg2PDU
// 输入参数：ID，从机地址
//			 WStartAddr，写入寄存器的地址(0x0001~0xffff)
//           WQuantity，写入寄存器的数量(1~121)
//			 RStartAddr，读出寄存器的地址(0x0001~0xffff)
//			 RQuantity，读出寄存器的数量(1~125)
//			 DataPtr，写入寄存器的指针和读出数据存放指针
// 输出参数：完成代码：(见异常代码表)
// 功能描述：写多寄存器操作。0x17
//*********************************************************************************
uint8  OSReadWriteMultipleReg(uint8 ID, 
 					 	uint16 WStartAddr,
 					 	uint16 WQuantity,
 					 	uint16 RStartAddr,
 					 	uint16 RQuantity,
 					 	uint16 *DataPtr)	
{
	WRITE_PARAMET Parameter;
	
	if(RQuantity>125)
		return MB_PARAMETER_ERR;
	if(WQuantity>121)
		return MB_PARAMETER_ERR;
 	Parameter.RStartAddr = RStartAddr;
 	Parameter.RQuantity = RQuantity;
 	Parameter.WStartAddr = WStartAddr;
 	Parameter.WQuantity = WQuantity;
  	Parameter.DataPtr = DataPtr;
    return ModbusPoll(ID,0x17,&Parameter);
}
