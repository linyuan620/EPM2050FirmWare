/****************************************Copyright (c)**************************************************
**                                  ������Զ�������޹�˾
**
**                                 http://www.21cm.com.cn
**
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			MBSlave.c
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

#include	"config.h"
#include	"modbus.h"


#ifdef UCOSII
	OS_EVENT *mb_handle_event_ptr;
#endif



extern uint16 Modbus_CRC16(uint8 *Buff_addr,uint16 len);

extern SendResponse(uint8 *buff,uint16 len);

// ���ܴ��봦��������
extern AT_CODE FUNCTION_ARRAY	FCodeHandleArray[MAX_FUNCTION];			  


//********************************************************************************************************
// �������ƣ�MBSlaveIni
// ���������DevicInfPtr���ӻ�������Ϣ���ݽṹָ��
// �����������
// ����������MODBUS�ӻ���ʼ��
//********************************************************************************************************
void MBSlaveIni(SLAVE_INFORMATION *DevicInfPtr)
{
	
	ADUData.Address = DevicInfPtr->Address;
	ADUData.ADUBuffPtr = DevicInfPtr->RequestPtr;
	ADUData.ADULength = 0;

//	ADUData.RedundancyCheck = 0;
	ADUData.EndT15 = FALSE;
	ADUData.FrameOK = FALSE;
	
	PDUData.PDUBuffPtr = NULL;
//	PDUData.Response.ExceptionCode = 0;

#ifdef UCOSII
	mb_handle_event_ptr = OSSemCreate(0);			// MODBUS�����ź���
#endif
}


//********************************************************************************************************
// �������ƣ�FunctionHandle
// �����������
// �����������
// ��������������������
//********************************************************************************************************
void FunctionHandle(void)
{
	uint8 i;
	PDU_RESPONSE Response;
	for(i=0;i<MAX_FUNCTION;i++)
	{
		if(FCodeHandleArray[i].Code==PDUData.FunctionCode)
		{
			Response.PDUDataPtr = PDUData.PDUBuffPtr;
			Response.PDUByteLength = PDUData.PDULength;

			FCodeHandleArray[i].Function( &Response );		// ִ�д��������ݽṹָ��
			
		//	PDUData.PDUBuffPtr = Response.PDUDataPtr;
			PDUData.PDULength = Response.PDUByteLength;
			PDUData.ExceptionCode = Response.ExceptionCode;
			return;
		}	
	}
	PDUData.ExceptionCode = EXCEPTION_CODE_1;	//	���ܴ�����Ч
}

void PackPDU2ADU(void)
{
	if(PDUData.ExceptionCode)	// ������ʧ��
	{
		*PDUData.PDUBuffPtr = PDUData.FunctionCode|0x80;
		*(PDUData.PDUBuffPtr+1) = PDUData.ExceptionCode;
		PDUData.PDULength = 2;
	}
	ADUData.ADULength = PDUData.PDULength + 3;		// ADU = Adress + PDU + CRC/LRC = 1 + PDU + 2;
	
}

//********************************************************************************************************
// �������ƣ�TransfersReplyHandle
// �����������
// �����������
// ��������������Ӧ��֡����
//********************************************************************************************************
void TransfersReplyHandle(void)
{
	uint16	CRC_OR_LRC;
	*ADUData.ADUBuffPtr = ADUData.Address;	  								// �豸��ַ
	CRC_OR_LRC = Modbus_CRC16(ADUData.ADUBuffPtr,ADUData.ADULength-2);		// ����CRC (ADU��CRCǰ������)
	*(ADUData.ADUBuffPtr+ADUData.ADULength-1) = (uint8)(CRC_OR_LRC/0x100);	// CRC Hi or LRC Hi 
	*(ADUData.ADUBuffPtr+ADUData.ADULength-2) = (uint8)CRC_OR_LRC;			// CRC Hi or LRC Hi 

	SendResponse(ADUData.ADUBuffPtr,ADUData.ADULength);
}


//********************************************************************************************************
// �������ƣ�FunctionHandle
// �����������
// �����������
// ����������MODBUS������
//********************************************************************************************************
void IdleModbus(void)
{
	uint8 err;
	// ������Ч֡
	OSSemPend(mb_handle_event_ptr,0,&err);
	if(PDUData.FrameOK==FALSE)		// ����֡δ��������
		return;
	PDUData.FrameOK = FALSE;
	FunctionHandle();				// ���ܴ��봦����
	PackPDU2ADU();					// ��PDU֡��װ��ADU֡
	TransfersReplyHandle();			// ����Ӧ��֡
	// ׼���յ���һ֡

//	ADUData.ADUBuffPtr = SlaveDevice.RequestPtr;
	ADUData.ADULength = 0;
	PDUData.PDUBuffPtr = NULL;
}








