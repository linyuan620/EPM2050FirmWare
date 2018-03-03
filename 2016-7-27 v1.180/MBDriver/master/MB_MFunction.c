/****************************************Copyright (c)**************************************************
**                                  ������Զ�������޹�˾
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
// �������ƣ�ReadOper2PDU
// ���������ParameterPtr�����ݽṹָ��
// �����������
// ������������װ������֡��0x01
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
// �������ƣ�ReadRegPRASD
// ���������PDUPtr�������յ������ݵ���ʼָ��
//			 ParameterPtr�����ݽṹָ��
// �����������
// ���������������յ��ļĴ���������Ĵ���/����Ĵ��������ݡ�
//***************************************************************
 void ReadRegPRASD(uint8 *PDUPtr,void *ParameterPtr)
 {
 	uint8 ByteCount;
 	uint8 i;
 	uint8 *DataPtr;
 	READ_PARAMET *PPtr;
 	
 	PPtr = (READ_PARAMET *)ParameterPtr;	// ָ������ṹָ��
 	DataPtr = &PDUPtr[2];					// ���ݻ���ʼָ��
 	ByteCount = PDUPtr[1];					// �Ĵ�������
	for(i=0;i<(ByteCount/2);i++)
	{
		*(PPtr->DataPtr+i) = (DataPtr[i*2]<<8)|DataPtr[i*2+1];
	}
 }
 
 //***************************************************************
// �������ƣ�ReadDiscretePRASD
// ���������PDUPtr�������յ������ݵ���ʼָ��
//			 ParameterPtr�����ݽṹָ��
// �����������
// ���������������յ�����ɢ����Ȧ���ݡ�
//***************************************************************
 void ReadDiscretePRASD(uint8 *PDUPtr,void *ParameterPtr)
 {
 	uint8 ByteCount;
 	uint8 i;
 	uint8 *DataPtr;
 	READ_PARAMET *PPtr;
 	
 	PPtr = (READ_PARAMET *)ParameterPtr;	// ָ������ṹָ��
 	ByteCount = PDUPtr[1];					// �Ĵ�������
	DataPtr = (uint8 *)PPtr->DataPtr;
	for(i=0;i<ByteCount;i++)
	{
		DataPtr[i] = PDUPtr[2+i];
	}
 }
 
 
 // ���ݴ�����
 void NoDataPRASD(uint8 *PDUPtr,void *ParameterPtr) 
 {
	PDUPtr = PDUPtr;
 	ParameterPtr = ParameterPtr;
 }
 
 /****************************************************************/
 void ReadCoils2PDU(void *ParameterPtr);			// ����Ȧ�Ĵ���
 void ReadDiscreteInputs2PDU(void *ParameterPtr);	// ����ɢ����Ĵ���
 void ReadHoldReg2PDU(void *ParameterPtr);			// �����ּĴ���
 void ReadInputReg2PDU(void *ParameterPtr);			// ������Ĵ���

 void WriteSingleCoil2PDU(void *ParameterPtr);		// д������Ȧ
 void WriteSingleRegPDU(void *ParameterPtr);		// д���Ĵ���
 void WriteMultipleCoils2PDU(void *ParameterPtr);	// д����Ȧ
 void WriteMultipleReg2PDU(void *ParameterPtr);		// д��Ĵ���

 void MaskWriteReg2PDU(void *ParameterPtr);			// �������μĴ���ָ��
 void ReadWriteMultipleReg2PDU(void *ParameterPtr);	// ��д����Ĵ���
FUNCTION_PACK_PDU FunPDUPackHanlde[MAX_FUNCTION]={
	{0x01,ReadCoils2PDU},			// ����Ȧ
	{0x02,ReadDiscreteInputs2PDU},	// ����ɢ������
	{0x03,ReadHoldReg2PDU},			// �����ּĴ���
	{0x04,ReadInputReg2PDU},		// ������Ĵ���
	
	{0x05,WriteSingleCoil2PDU},		// д������Ȧ
	{0x06,WriteSingleRegPDU},		// д�����Ĵ���
	{0x0f,WriteMultipleCoils2PDU},
	{0x10,WriteMultipleReg2PDU},
	
	{0x16,MaskWriteReg2PDU},		// �������μĴ���ָ��
	{0x17,ReadWriteMultipleReg2PDU}	// ��д����Ĵ���
		
};


 void ReadOper2PDU( uint8 FunCode,void *ParameterPtr );
// Function of Parse Reponse And StoreData


FUNCTION_DATA_HANDLE FunParRepAndStData[MAX_FUNCTION]={
	{0x01,ReadDiscretePRASD},// ����Ȧ
	{0x02,ReadDiscretePRASD},// ����ɢ������,�����Ȧ����ʽһ��
	{0x03,ReadRegPRASD},	 // �����ּĴ���
	{0x04,ReadRegPRASD},	 // ������Ĵ���������
	
	{0x05,NoDataPRASD},
	{0x06,NoDataPRASD},
	{0x0f,NoDataPRASD},
	{0x10,NoDataPRASD},
	
	{0x16,NoDataPRASD},		// �������μĴ���ָ��
	{0x17,ReadRegPRASD}		// ��д����Ĵ���

};



//***************************************************************
// �������ƣ�ReadCoils2PDU
// ���������ParameterPtr�����ݽṹָ��
// �����������
// ������������װ����Ȧ֡��0x01
//***************************************************************
void ReadCoils2PDU(void *ParameterPtr)
{
 	ReadOper2PDU( 0x01,ParameterPtr );
}

//***************************************************************
// �������ƣ�ReadDiscreteInputs2PDU
// ���������ParameterPtr�����ݽṹָ��
// �����������
// ������������װ����ɢ����֡��0x02
//***************************************************************
void ReadDiscreteInputs2PDU(void *ParameterPtr)
{
 	ReadOper2PDU( 0x02,ParameterPtr );
}

//***************************************************************
// �������ƣ�ReadHoldReg2PDU
// ���������ParameterPtr�����ݽṹָ��
// �����������
// ������������װ�����ּĴ���֡��0x03
//***************************************************************
 void ReadHoldReg2PDU(void *ParameterPtr)
 {
 	ReadOper2PDU( 0x03,ParameterPtr );
 }
 
//***************************************************************
// �������ƣ�ReadInputReg2PDU
// ���������ParameterPtr�����ݽṹָ��
// �����������
// ������������װ������Ĵ���֡��0x04
//***************************************************************
 void ReadInputReg2PDU(void *ParameterPtr)
 {
 	ReadOper2PDU( 0x04,ParameterPtr );
 }
 
 
//***************************************************************
// �������ƣ�WriteSingleCoil2PDU
// ���������ParameterPtr�����ݽṹָ��
// �����������
// ������������װд������Ȧ֡��0x05
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
// �������ƣ�WriteSingleRegPDU
// ���������ParameterPtr�����ݽṹָ��
// �����������
// ������������װд�����Ĵ���֡��0x06
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
// �������ƣ�WriteMultipleCoils2PDU
// ���������ParameterPtr�����ݽṹָ��
// �����������
// ������������װд�����Ȧ֡��0x0f
//***************************************************************
 void WriteMultipleCoils2PDU(void *ParameterPtr)	// д����Ȧ
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
 	 Request[5] = Bytes;	// �ַ�����
 	 
 	 DataPtr = (uint8*)PPtr->DataPtr;

 	 
 	 for(i=0;i<Bytes;i++)
 	 {
 	 	Request[6+i] = *(DataPtr+i) ;
 	 }
 	 
 	 PutDataInPUD(Request,6+Bytes);
 }
 
//***************************************************************
// �������ƣ�WriteSingleCoil2PDU
// ���������ParameterPtr�����ݽṹָ��
// �����������
// ������������װд����Ĵ���֡��0x10
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
 	 Request[5] = (uint8)(PPtr->WQuantity*2);	// �ַ�����
 	 
 	 for(i=0;i<PPtr->WQuantity;i++)
 	 {
 	 	Request[6+i*2] = *(PPtr->DataPtr+i)>>8;
 	 	Request[7+i*2] = *(PPtr->DataPtr+i);
 	 }
 	 
 	 PutDataInPUD(Request,6+(PPtr->WQuantity)*2);
 } 

 //***************************************************************
// �������ƣ�MaskWriteReg2PDU
// ���������ParameterPtr�����ݽṹָ��
// �����������
// ������������װ����д�Ĵ���֡��0x16
//***************************************************************
 void MaskWriteReg2PDU(void *ParameterPtr)			// �������μĴ���ָ��
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

 	 PutDataInPUD(Request,7);						// ��װPDU��
}


//***************************************************************
// �������ƣ�ReadWriteMultipleReg2PDU
// ���������ParameterPtr�����ݽṹָ��
// �����������
// ������������װ����д����Ĵ���֡��0x17
//***************************************************************
 void ReadWriteMultipleReg2PDU(void *ParameterPtr)	// ��д����Ĵ���
{
	RW_PARAMET *PPtr;
	uint8 Request[256];
	uint8 i;
	
	PPtr = (RW_PARAMET *) ParameterPtr;
	Request[0] = 0x17;
	Request[1] =  (uint8)(PPtr->RStartAddr>>8);		// ��������ʼָ��
	Request[2] =  (uint8)(PPtr->RStartAddr);
	Request[3] =  (uint8)(PPtr->RQuantity>>8);		// ����������
	Request[4] =  (uint8)(PPtr->RQuantity);
	Request[5] =  (uint8)(PPtr->WStartAddr>>8);		// д�����ʼ��ַ
	Request[6] =  (uint8)(PPtr->WStartAddr);
	Request[7] =  (uint8)(PPtr->WQuantity>>8);		// д�������
	Request[8] =  (uint8)(PPtr->WQuantity);
	
	Request[9] =  (uint8)(PPtr->WQuantity*2);		// д����ֽ���
	
	for(i=0;i<PPtr->WQuantity;i++)					// д�������
 	{
 	 	Request[10+i*2] = *(PPtr->DataPtr+i)>>8;
 	 	Request[11+i*2] = *(PPtr->DataPtr+i);
 	}
 	PutDataInPUD(Request,10+(PPtr->WQuantity)*2);	// ��װPDU��
}
 

 
//***************************************************************

//***************************************************************
