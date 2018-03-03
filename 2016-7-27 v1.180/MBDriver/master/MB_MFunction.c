/****************************************Copyright (c)**************************************************
**                                  广州致远电子有限公司
**
**                                 http://www.21cm.com.cn
**
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			MB_MFunction.c
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

 void Test(void *ParameterPtr)
 {
 
 }


 void Test2(uint8 *PDUPtr,void *ParameterPtr)
 {
 
 }
 //***************************************************************
// 函数名称：ReadOper2PDU
// 输入参数：ParameterPtr，数据结构指针
// 输出参数：无
// 功能描述：封装读操作帧。0x01
//***************************************************************
void ReadOper2PDU( uint8 FunCode,void *ParameterPtr )
{
 	 READ_PARAMET *PPtr;
 	 uint8 Request[5];
 	 PPtr = (READ_PARAMET *)ParameterPtr;

 	 Request[0] = FunCode;

 	 Request[1] = (uint8)(PPtr->RStartAddr>>8);
 	 Request[2] = (uint8)(PPtr->RStartAddr);
 	 
 	 Request[3] = (uint8)(PPtr->RQuantity>>8);
 	 Request[4] = (uint8)(PPtr->RQuantity);
 	 
 	 PutDataInPUD(Request,5);
 
}
//***************************************************************
// 函数名称：ReadRegPRASD
// 输入参数：PDUPtr，保存收到的数据的起始指针
//			 ParameterPtr，数据结构指针
// 输出参数：无
// 功能描述：处理收到的寄存器（保存寄存器/输入寄存器）数据。
//***************************************************************
 void ReadRegPRASD(uint8 *PDUPtr,void *ParameterPtr)
 {
 	uint8 ByteCount;
 	uint8 i;
 	uint8 *DataPtr;
 	READ_PARAMET *PPtr;
 	
 	PPtr = (READ_PARAMET *)ParameterPtr;	// 指令参数结构指针
 	DataPtr = &PDUPtr[2];					// 数据或起始指针
 	ByteCount = PDUPtr[1];					// 寄存器个数
	for(i=0;i<(ByteCount/2);i++)
	{
		*(PPtr->DataPtr+i) = (DataPtr[i*2]<<8)|DataPtr[i*2+1];
	}
 }
 
 //***************************************************************
// 函数名称：ReadDiscretePRASD
// 输入参数：PDUPtr，保存收到的数据的起始指针
//			 ParameterPtr，数据结构指针
// 输出参数：无
// 功能描述：处理收到的离散或线圈数据。
//***************************************************************
 void ReadDiscretePRASD(uint8 *PDUPtr,void *ParameterPtr)
 {
 	uint8 ByteCount;
 	uint8 i;
 	uint8 *DataPtr;
 	READ_PARAMET *PPtr;
 	
 	PPtr = (READ_PARAMET *)ParameterPtr;	// 指令参数结构指针
 	ByteCount = PDUPtr[1];					// 寄存器个数
	DataPtr = (uint8 *)PPtr->DataPtr;
	for(i=0;i<ByteCount;i++)
	{
		DataPtr[i] = PDUPtr[2+i];
	}
 }
 
 
 // 数据处理函数
 void NoDataPRASD(uint8 *PDUPtr,void *ParameterPtr) 
 {
	PDUPtr = PDUPtr;
 	ParameterPtr = ParameterPtr;
 }
 
 /****************************************************************/
 void ReadCoils2PDU(void *ParameterPtr);			// 读线圈寄存器
 void ReadDiscreteInputs2PDU(void *ParameterPtr);	// 读离散输入寄存器
 void ReadHoldReg2PDU(void *ParameterPtr);			// 读保持寄存器
 void ReadInputReg2PDU(void *ParameterPtr);			// 读输入寄存器

 void WriteSingleCoil2PDU(void *ParameterPtr);		// 写单个线圈
 void WriteSingleRegPDU(void *ParameterPtr);		// 写单寄存器
 void WriteMultipleCoils2PDU(void *ParameterPtr);	// 写多线圈
 void WriteMultipleReg2PDU(void *ParameterPtr);		// 写多寄存器

 void MaskWriteReg2PDU(void *ParameterPtr);			// 处理屏蔽寄存器指令
 void ReadWriteMultipleReg2PDU(void *ParameterPtr);	// 读写多个寄存器
FUNCTION_PACK_PDU FunPDUPackHanlde[MAX_FUNCTION]={
	{0x01,ReadCoils2PDU},			// 读线圈
	{0x02,ReadDiscreteInputs2PDU},	// 读离散量输入
	{0x03,ReadHoldReg2PDU},			// 读保持寄存器
	{0x04,ReadInputReg2PDU},		// 读输入寄存器
	
	{0x05,WriteSingleCoil2PDU},		// 写单个线圈
	{0x06,WriteSingleRegPDU},		// 写单个寄存器
	{0x0f,WriteMultipleCoils2PDU},
	{0x10,WriteMultipleReg2PDU},
	
	{0x16,MaskWriteReg2PDU},		// 处理屏蔽寄存器指令
	{0x17,ReadWriteMultipleReg2PDU}	// 读写多个寄存器
		
};


 void ReadOper2PDU( uint8 FunCode,void *ParameterPtr );
// Function of Parse Reponse And StoreData


FUNCTION_DATA_HANDLE FunParRepAndStData[MAX_FUNCTION]={
	{0x01,ReadDiscretePRASD},// 读线圈
	{0x02,ReadDiscretePRASD},// 读离散量输入,与读线圈处理方式一样
	{0x03,ReadRegPRASD},	 // 读保持寄存器
	{0x04,ReadRegPRASD},	 // 读输入寄存器处理函数
	
	{0x05,NoDataPRASD},
	{0x06,NoDataPRASD},
	{0x0f,NoDataPRASD},
	{0x10,NoDataPRASD},
	
	{0x16,NoDataPRASD},		// 处理屏蔽寄存器指令
	{0x17,ReadRegPRASD}		// 读写多个寄存器

};



//***************************************************************
// 函数名称：ReadCoils2PDU
// 输入参数：ParameterPtr，数据结构指针
// 输出参数：无
// 功能描述：封装读线圈帧。0x01
//***************************************************************
void ReadCoils2PDU(void *ParameterPtr)
{
 	ReadOper2PDU( 0x01,ParameterPtr );
}

//***************************************************************
// 函数名称：ReadDiscreteInputs2PDU
// 输入参数：ParameterPtr，数据结构指针
// 输出参数：无
// 功能描述：封装读离散输入帧。0x02
//***************************************************************
void ReadDiscreteInputs2PDU(void *ParameterPtr)
{
 	ReadOper2PDU( 0x02,ParameterPtr );
}

//***************************************************************
// 函数名称：ReadHoldReg2PDU
// 输入参数：ParameterPtr，数据结构指针
// 输出参数：无
// 功能描述：封装读保持寄存器帧。0x03
//***************************************************************
 void ReadHoldReg2PDU(void *ParameterPtr)
 {
 	ReadOper2PDU( 0x03,ParameterPtr );
 }
 
//***************************************************************
// 函数名称：ReadInputReg2PDU
// 输入参数：ParameterPtr，数据结构指针
// 输出参数：无
// 功能描述：封装读输入寄存器帧。0x04
//***************************************************************
 void ReadInputReg2PDU(void *ParameterPtr)
 {
 	ReadOper2PDU( 0x04,ParameterPtr );
 }
 
 
//***************************************************************
// 函数名称：WriteSingleCoil2PDU
// 输入参数：ParameterPtr，数据结构指针
// 输出参数：无
// 功能描述：封装写单个线圈帧。0x05
//***************************************************************
 void WriteSingleCoil2PDU(void *ParameterPtr)
{
	 RW_PARAMET *PPtr;
 	 uint8 Request[5];
 	 PPtr = (READ_PARAMET *)ParameterPtr;

 	 Request[0] = 0x05;

 	 Request[1] = (uint8)(PPtr->WStartAddr>>8);
 	 Request[2] = (uint8)(PPtr->WStartAddr);
 	 
 	 Request[3] = (uint8)((*PPtr->DataPtr)>>8);
 	 Request[4] = (uint8)(*PPtr->DataPtr);
 	 
 	 PutDataInPUD(Request,5);	
}

//***************************************************************
// 函数名称：WriteSingleRegPDU
// 输入参数：ParameterPtr，数据结构指针
// 输出参数：无
// 功能描述：封装写单个寄存器帧。0x06
//***************************************************************
 void WriteSingleRegPDU(void *ParameterPtr)
{
	
	 RW_PARAMET *PPtr;
 	 uint8 Request[5];
 	 PPtr = (READ_PARAMET *)ParameterPtr;

 	 Request[0] = 0x06;

 	 Request[1] = (uint8)(PPtr->WStartAddr>>8);
 	 Request[2] = (uint8)(PPtr->WStartAddr);
 	 
 	 Request[3] = (uint8)((*PPtr->DataPtr)>>8);
 	 Request[4] = (uint8)(*PPtr->DataPtr);
 	 
 	 PutDataInPUD(Request,5);	
}
 
//***************************************************************
// 函数名称：WriteMultipleCoils2PDU
// 输入参数：ParameterPtr，数据结构指针
// 输出参数：无
// 功能描述：封装写多个线圈帧。0x0f
//***************************************************************
 void WriteMultipleCoils2PDU(void *ParameterPtr)	// 写多线圈
 {
 	 WRITE_PARAMET *PPtr;
 	 uint8 *DataPtr;
 	 uint8 Request[256],i;
 	 uint8 Bytes;
 	 
 	 PPtr = (WRITE_PARAMET *)ParameterPtr;
	 Bytes = (PPtr->WQuantity/8)+(PPtr->WQuantity%8)?1:0;
 	
 	 Request[0] = 0x0f;
 	 Request[1] = (uint8)(PPtr->WStartAddr>>8);
 	 Request[2] = (uint8)(PPtr->WStartAddr);
 	 Request[3] = (uint8)(PPtr->WQuantity>>8);
 	 Request[4] = (uint8)(PPtr->WQuantity);
 	 Request[5] = Bytes;	// 字符个数
 	 
 	 DataPtr = (uint8*)PPtr->DataPtr;

 	 
 	 for(i=0;i<Bytes;i++)
 	 {
 	 	Request[6+i] = *(DataPtr+i) ;
 	 }
 	 
 	 PutDataInPUD(Request,6+Bytes);
 }
 
//***************************************************************
// 函数名称：WriteSingleCoil2PDU
// 输入参数：ParameterPtr，数据结构指针
// 输出参数：无
// 功能描述：封装写多个寄存器帧。0x10
//***************************************************************
 void WriteMultipleReg2PDU(void *ParameterPtr)
 {
  	 WRITE_PARAMET *PPtr;
 	 uint8 Request[256],i;
 	 
 	 PPtr = (WRITE_PARAMET *)ParameterPtr;
	
 	 Request[0] = 0x10;
 	 Request[1] = (uint8)(PPtr->WStartAddr>>8);
 	 Request[2] = (uint8)(PPtr->WStartAddr);
 	 Request[3] = (uint8)(PPtr->WQuantity>>8);
 	 Request[4] = (uint8)(PPtr->WQuantity);
 	 Request[5] = (uint8)(PPtr->WQuantity*2);	// 字符个数
 	 
 	 for(i=0;i<PPtr->WQuantity;i++)
 	 {
 	 	Request[6+i*2] = *(PPtr->DataPtr+i)>>8;
 	 	Request[7+i*2] = *(PPtr->DataPtr+i);
 	 }
 	 
 	 PutDataInPUD(Request,6+(PPtr->WQuantity)*2);
 } 

 //***************************************************************
// 函数名称：MaskWriteReg2PDU
// 输入参数：ParameterPtr，数据结构指针
// 输出参数：无
// 功能描述：封装屏蔽写寄存器帧。0x16
//***************************************************************
 void MaskWriteReg2PDU(void *ParameterPtr)			// 处理屏蔽寄存器指令
{
	MASK_WRITE_REG *PPtr;
	uint8 Request[7];
	
	PPtr = (MASK_WRITE_REG *) ParameterPtr;
	Request[0] = 0x16;
	Request[1] =  (uint8)(PPtr->RefAddress>>8);
	Request[2] =  (uint8)(PPtr->RefAddress);
	Request[3] =  (uint8)(PPtr->And_Mask>>8);
	Request[4] =  (uint8)(PPtr->And_Mask);
	Request[5] =  (uint8)(PPtr->OR_Mask>>8);
	Request[6] =  (uint8)(PPtr->OR_Mask);

 	 PutDataInPUD(Request,7);						// 封装PDU包
}


//***************************************************************
// 函数名称：ReadWriteMultipleReg2PDU
// 输入参数：ParameterPtr，数据结构指针
// 输出参数：无
// 功能描述：封装读和写多个寄存器帧。0x17
//***************************************************************
 void ReadWriteMultipleReg2PDU(void *ParameterPtr)	// 读写多个寄存器
{
	RW_PARAMET *PPtr;
	uint8 Request[256];
	uint8 i;
	
	PPtr = (RW_PARAMET *) ParameterPtr;
	Request[0] = 0x17;
	Request[1] =  (uint8)(PPtr->RStartAddr>>8);		// 读出的起始指针
	Request[2] =  (uint8)(PPtr->RStartAddr);
	Request[3] =  (uint8)(PPtr->RQuantity>>8);		// 读出的数量
	Request[4] =  (uint8)(PPtr->RQuantity);
	Request[5] =  (uint8)(PPtr->WStartAddr>>8);		// 写入的起始地址
	Request[6] =  (uint8)(PPtr->WStartAddr);
	Request[7] =  (uint8)(PPtr->WQuantity>>8);		// 写入的数量
	Request[8] =  (uint8)(PPtr->WQuantity);
	
	Request[9] =  (uint8)(PPtr->WQuantity*2);		// 写入的字节数
	
	for(i=0;i<PPtr->WQuantity;i++)					// 写入的数据
 	{
 	 	Request[10+i*2] = *(PPtr->DataPtr+i)>>8;
 	 	Request[11+i*2] = *(PPtr->DataPtr+i);
 	}
 	PutDataInPUD(Request,10+(PPtr->WQuantity)*2);	// 封装PDU包
}
 

 
//***************************************************************

//***************************************************************
