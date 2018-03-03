/****************************************Copyright (c)**************************************************
**                                  ������Զ�������޹�˾
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
// �������ƣ�OSReadCoils
// ���������ID���ӻ���ַ
//			 StartAddress����Ȧ��ʼ��ַ(0x0000~0xffff)
//			 Quantity����Ȧ����(0x001~0x07d0)
//			 ReaValuePtr��������Ȧֵ��ָ��
// �����������ɴ��룺(���쳣�����)
// ������������ȡ��Ȧֵ��0x01
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
// �������ƣ�OSReadDisInputs
// ���������ID���ӻ���ַ
//			 StartAddress����ɢ��������ʼ��ַ(0x0000~0xffff)
//			 Quantity����ɢ����������(0x001~0x07d0)
//			 ReaValuePtr��������ɢ������ֵ��ָ��
// �����������ɴ��룺(���쳣�����)
// ������������ȡ��ɢ������ֵ��0x02
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
// �������ƣ�OSReadHoldReg
// ���������ID���ӻ���ַ
//			 StartAddress���Ĵ�����ʼ��ַ(0x0000~0xffff)
//			 Quantity���Ĵ�������(0x0001~0x007d)
//			 ReaValuePtr����������Ĵ���ֵ��ָ��
// �����������ɴ��룺(���쳣�����)
// ������������ȡ���ּĴ���ֵ��0x03
//*********************************************************************************
 uint8 OSReadHoldReg(uint8 ID, 
 					 uint16 StartAddress,
 					 uint16 Quantity,
 					 uint16 *ReaValuePtr)		// �����ּĴ���
{
	READ_PARAMET Parameter;
	Parameter.RStartAddr = StartAddress;
   	Parameter.RQuantity = Quantity;
    Parameter.DataPtr =  ReaValuePtr;
    return ModbusPoll(ID,0x03,&Parameter);
}

//*********************************************************************************
// �������ƣ�OSReadInputReg
// ���������ID���ӻ���ַ
//			 StartAddress������Ĵ�����ʼ��ַ(0x0000~0xffff)
//			 Quantity������Ĵ�������(0x0001~0x007d)
//			 ReaValuePtr�������������Ĵ���ֵ��ָ��
// �����������ɴ��룺(���쳣�����)
// ������������ȡ����Ĵ���ֵ��0x04
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
// �������ƣ�OSWriteSingleCoil
// ���������ID���ӻ���ַ
//			 OutAddress��д��ĵ�ַ(0x0000~0xffff)
//			 OutValue��д���ֵCOIL_OFF(0x0000)��COIL_ON(0xff00)
// �����������ɴ��룺(���쳣�����)
// ����������д����Ȧ������0x05
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
// �������ƣ�OSWriteSingleReg
// ���������ID���ӻ���ַ
//			 OutAddress��д��ĵ�ַ(0x0000~0xffff)
//			 OutValue��д���ֵ(0x0000~0xffff)
// �����������ɴ��룺(���쳣�����)
// ����������д���Ĵ���������0x06
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
// �������ƣ�OSWriteMultipleCoils
// ���������ID���ӻ���ַ
//			 StartAddress��д��ĵ�ַ(0x0001~0xffff)
//			 Quantity��д�������(0x0001~0x07b0)
//			 ValuePtr��д�����ݵ�ָ��
// �����������ɴ��룺(���쳣�����)
// ����������д����Ȧ������0x0f
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
// �������ƣ�WriteMultipleReg
// ���������ID���ӻ���ַ
//			 StartAddress��д��ĵ�ַ(0x0001~0xffff)
//			 Quantity��д�������(0x0001~0x0078)
//			 ValuePtr��д�����ݵ�ָ��
// �����������ɴ��룺(���쳣�����)
// ����������д��Ĵ���������0x10
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
// �������ƣ�MaskWriteReg
// ���������ID���ӻ���ַ
//			 Addr��д��ĵ�ַ(0x0001~0xffff)
//			 And_Mask������д������
//			 Or_Mask������д������
// �����������ɴ��룺(���쳣�����)
// ��������������д�Ĵ������������Or_MaskֵΪ�㣬��ô����ǵ�ǰ���ݺ�And_Mask�ļ��߼�AND���룩��
//			 ���And_MaskֵΪ�㣬�������Or_Maskֵ��0x16
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
// �������ƣ�ReadWriteMultipleReg2PDU
// ���������ID���ӻ���ַ
//			 WStartAddr��д��Ĵ����ĵ�ַ(0x0001~0xffff)
//           WQuantity��д��Ĵ���������(1~121)
//			 RStartAddr�������Ĵ����ĵ�ַ(0x0001~0xffff)
//			 RQuantity�������Ĵ���������(1~125)
//			 DataPtr��д��Ĵ�����ָ��Ͷ������ݴ��ָ��
// �����������ɴ��룺(���쳣�����)
// ����������д��Ĵ���������0x17
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
