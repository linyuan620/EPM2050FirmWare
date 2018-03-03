/****************************************Copyright (c)**************************************************
**                                  ������Զ�������޹�˾
**
**                                 http://www.21cm.com.cn
**
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			MBCommon.c
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


PDU_HANDLE 				PDUData;
volatile ADU_CONTROL 	ADUData;

#ifdef UCOSII
extern	OS_EVENT *mb_handle_event_ptr;
#endif

extern uint16 Modbus_CRC16(uint8 *Buff_addr,uint16 len);

//********************************************************************************************************
// �������ƣ�StartCountT15
// �����������
// �����������
// ����������ʹ��T1.5������T1.5��ʱ��ֵΪUART����1.5���ַ���ʱ�䡣
//			 ע�⣺�ú���ֻ��ʹ�ܼ�ʱ���������ں����в����κ���ʱ��
//			       ���T1.5ʱ�����ִ��T15EndHandle������
//********************************************************************************************************
extern void StartCountT15(void); 

//********************************************************************************************************
// �������ƣ�StartCountT35
// �����������
// �����������
// ����������ʹ��T3.5������T3.5��ʱ��ֵΪUART����3.5���ַ���ʱ�䡣
//			 ע�⣺�ú���ֻ��ʹ�ܼ�ʱ���������ں����в����κ���ʱ��
//			       ���T3.5ʱ�����ִ��T35EndHandle������
//********************************************************************************************************
extern void StartCountT35(void);


volatile uint32 g_10mS_count;
volatile uint8 	g_TimeEnd;				// Ӧ��ʱ
//******************************************************************************************
// �������ƣ�Get_10mS
// �����������
// ���������10mS�����Ĵ���
// ������������ȡ10mS����ֵ����ֵÿ10�����1
//******************************************************************************************
void Waite10mS(uint32 time)
{
	g_TimeEnd = FALSE;
	g_10mS_count=time;
	Start10mS();
}



//*****************************************************************************
// �������ƣ�OverTimeHandle
// �����������
// �����������
// ����������
//*****************************************************************************
void Time10mSHandle(void)
{
	if(g_10mS_count==0)	return; // g_10mS_count,ֵΪ0��ʾû�м����¼�,ֱ�ӷ���
	g_10mS_count--;				// ������1,ֱ�ӵ�0Ϊֱ
	if(g_10mS_count==0)
	{	g_TimeEnd = TRUE;
	#ifdef UCOSII
		OSSemPost(mb_handle_event_ptr);				// �����ź���,ʹMODBUS������������
	#endif
	}
}

//********************************************************************************************************
// �������ƣ�T15EndHandle
// �����������
// �����������
// ����������T1.5��ʱ����
//********************************************************************************************************
void T15EndHandle(void)
{
	uint16 RedundancyCheck;
	if(ADUData.EndT15)
		return;
	ADUData.EndT15 = TRUE;		// T1.5ʱ�����	

	if((*ADUData.ADUBuffPtr==BROADCAST_ADDRESS)||		// ��֤��ַ�Ƿ���Ч
	   (*ADUData.ADUBuffPtr==ADUData.Address))
	{
		//�Լ����
		if((ADUData.ADULength - 2)<0)
		{
			ADUData.FrameOK = FALSE;
			return;
		}
		
		
		//end   ���ܵ����� ����У�� 
		RedundancyCheck = *(ADUData.ADUBuffPtr+ADUData.ADULength-2)|
								  (*(ADUData.ADUBuffPtr+ADUData.ADULength-1)<<8);
		if(RedundancyCheck == Modbus_CRC16(ADUData.ADUBuffPtr,ADUData.ADULength-2))	// CRCУ��
		{
			ADUData.FrameOK = TRUE;
			return;
		}
	}
	ADUData.FrameOK = FALSE;
	
}

//********************************************************************************************************
// �������ƣ�T35EndHandle
// �����������
// �����������
// ����������T3.5��������
//********************************************************************************************************
void T35EndHandle(void)
{

	if(ADUData.FrameOK)
	{
		PDUData.PDUBuffPtr = ADUData.ADUBuffPtr+1;
		PDUData.PDULength = ADUData.ADULength-3;
		PDUData.FunctionCode = *PDUData.PDUBuffPtr; // ���湦�ܴ���
 		PDUData.FrameOK=TRUE;
	}
	else
	{
		PDUData.FrameOK=FALSE;
	}
   	ADUData.EndT15 = FALSE;		// �����յ��ַ�
	ADUData.ADULength = 0;
#ifdef UCOSII
	OSSemPost(mb_handle_event_ptr);				// �����ź���,ʹMODBUS������������
#endif
}

 
//********************************************************************************************************
// �������ƣ�ReceOneChar
// ���������ReceCharacter
// �����������
// ��������������һ���ַ�����
//           Ҫ��:���յ�һ���ַ�����������ú�������
//********************************************************************************************************
void ReceOneChar(uint8 ReceCharacter)
{

	if(ADUData.EndT15==TRUE)
	{	StartCountT35();			// ��T3.5��ʱ
		return;
	}
	ADUData.FrameOK = FALSE;
	StartCountT15();			// ��T1.5��ʱ
	StartCountT35();			// ��T3.5��ʱ

   	*(ADUData.ADUBuffPtr + ADUData.ADULength) = ReceCharacter;
	ADUData.ADULength++; 
}

//********************************************************************************************************
// �������ƣ�PutCharInPUD
// ���������ch,д����ַ�
//			 EndChar��
// �����������
// ��������������һ���ַ�����
//           Ҫ��:���յ�һ���ַ�����������ú�������
//********************************************************************************************************
uint8 PutDataInPUD(uint8 *CharPtr,uint16 Length)
{
	uint16 i;
	if(Length<=MAX_PDU_DATA_LENGTH)
	{
		for(i=0;i<Length;i++)
			*(PDUData.PDUBuffPtr+i)=*(CharPtr+i);
		PDUData.PDULength = Length ;
		PDUData.FrameOK = TRUE;
		return TRUE;
	}
	else
		return FALSE;
}


/*********************************************************************************************************
**                            OSModbusServe MODBUS��������
********************************************************************************************************/
#ifdef UCOSII
//	void OSModbusServe	(void *pdata)
//{	pdata = pdata;
//	while (1)
//	{	IdleModbus();
//	}
//}   
#endif
