/****************************************Copyright (c)**************************************************
**                                  ������Զ�������޹�˾
**
**                                 http://www.21cm.com.cn
**
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			MBMaster.c
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

#define ERR_ADRRESS 	0xff		// �豸��ַ��Ч
#define ERR_PARAMETER	0xfe		// ���������Ч����

#define ERR_TIMEOUT		0xfd		// ûӦ��ʱ����
#define ERR_FRAME		0xfc		// ���յ���֡����CRC����
#define ERR_NON			0x00		// ��������


#define MB_POLL_TIMEOUT	 1//	100
#define MB_POLL_DELAY	1//	5


extern FUNCTION_PACK_PDU FunPDUPackHanlde[MAX_FUNCTION];
extern FUNCTION_DATA_HANDLE FunParRepAndStData[MAX_FUNCTION];

uint16 Modbus_CRC16(uint8 *Buff_addr,uint16 len);
void SendResponse(uint8 *buff,uint16 len);

//*****************************************************************************
// �������ƣ�
// ���������
// ���������
// ����������
//*****************************************************************************

// uint8 PDUBuffer[256];//���ͣ�

uint8 ADUBuffer[256];//���գ�

extern int g_WaitN10mS;

#ifdef UCOSII
	OS_EVENT *mb_handle_event_ptr;
	OS_EVENT *mb_reply_event_ptr;
	OS_EVENT *mb_idle_event_ptr;
#endif

MASTER_INFORMATION MB_Master;
//********************************************************************************************************
// �������ƣ�MBSlaveIni
// �����������
// �����������
// ����������MODBUS�ӻ���ʼ��
//********************************************************************************************************
void MBMasterIni(void)
{

	MB_Master.Status = MB_MASTER_IDLE;	// ��������
	ADUData.Address = 0;
	ADUData.ADUBuffPtr = ADUBuffer;
	ADUData.ADULength = 0;

//	ADUData.RedundancyCheck = 0;
	ADUData.EndT15 = FALSE;
	ADUData.FrameOK = FALSE;
//	ADUData.OverTime = FALSE;
	PDUData.PDUBuffPtr = NULL;
//	PDUData.Response.ExceptionCode = 0;
#ifdef UCOSII
	mb_idle_event_ptr   = OSSemCreate(1);			// MODBUS���������ź���
	mb_handle_event_ptr = OSSemCreate(0);			// MODBUS�����ź���
	mb_reply_event_ptr  = OSMboxCreate((void *)0);	// MODBUS����Ӧ����Ϣ
#endif
}


//******************************************************************************************
// �������ƣ�ModbusPoll
// ���������NodeID,�ڵ�ID���豸��ַ��
// 			 FunctionCode�����ܴ���
//			 ParameterPtr���������
// ���������err,ָ��ִ��״̬
// ����������
//******************************************************************************************

uint8 ModbusPoll(uint8 NodeID,uint8 FunctionCode,void *ParameterPtr)
{
	uint8 i,err;
	ADUData.Address = NodeID;
	

	for(i=0;i<MAX_FUNCTION;i++)							// �����Ч�Ĺ��ܴ���			
	{	if(FunPDUPackHanlde[i].Code==FunctionCode)		// ���ҹ��ܴ��봦����
		{
		#ifdef UCOSII
			OSSemPend(mb_idle_event_ptr,0,&err);		// �ȴ����Բ���MODBUS������һ���ź���	
		#else
			if(MB_Master.Status!=(uint8)MB_MASTER_IDLE)
				return (uint8)MB_MASTER_BUSY;			// ��Чָ���
		#endif	
			PDUData.PDUBuffPtr = ADUData.ADUBuffPtr+1;	// ����PDU��ʼָ��
			FunPDUPackHanlde[i].Function(ParameterPtr);	// ���ù��ܴ��봦����
			PDUData.FrameOK = TRUE;						// ����֡׼������
			MB_Master.ParPtr = ParameterPtr;			// ��������ṹָ��
			
		#ifdef UCOSII
			OSSemPost(mb_handle_event_ptr);				// �����ź���,ʹMODBUS������������
			err = *(uint8 *)OSMboxPend(mb_reply_event_ptr,0,&err);	// �ȴ�ִ�����	
		#else
			do
			{
				err = IdleModbus();
			}while(MB_Master.Status==(uint8)MB_MASTER_BUSY);
		#endif		
			return err;	// ��Чָ���

		}
	}
	return (uint8)MB_FUN_NOT_DEF;
}


//******************************************************************************************
// �������ƣ�ParseReponseAndStoreData
// �����������
// �����������
// ��������������Ӧ��֡������������
//******************************************************************************************
uint8 ParseReponseAndStoreData(void)
{
	uint8 i,err;
	err = MB_FUN_NOT_DEF;
	for(i=0;i<MAX_FUNCTION;i++)							
		if(FunParRepAndStData[i].Code==PDUData.FunctionCode)		// ���ҹ��ܴ��봦����
		{
			if(PDUData.PDUBuffPtr[0]&0x80)	// �쳣����
			{	err = PDUData.PDUBuffPtr[1];}
			else	
			{	err = MB_NO_ERR;
				FunParRepAndStData[i].Function(PDUData.PDUBuffPtr,MB_Master.ParPtr);	// ���ù��ܴ��봦����
			
			}
			break;
		}
	return err;// δ���幦����
}

//******************************************************************************************
// �������ƣ�CheckModbusMessage
// �����������
// ���������ADU������
// ��������������Ƿ�����Ҫ���͵ĵ�֡
//******************************************************************************************
ADU_CONTROL *CheckModbusMessage(void)
{
	uint16 CRC16;
	uint8 *ADUPtr;
	if(PDUData.FrameOK == TRUE)			// ����֡��PDU�Ƿ�׼������
	{
		PDUData.FrameOK = FALSE;		// ��PDU�Ƿ�׼��������־�������յ���Ч��PDU�󽫱��ٴ���1
		ADUPtr = ADUData.ADUBuffPtr;
		ADUPtr[0] = ADUData.Address;	// ����ADU��ַ���ڵ㣩
		
		CRC16 = Modbus_CRC16(ADUPtr,PDUData.PDULength+1);	// ��CRC16ֵ
		ADUPtr[PDUData.PDULength+1] = CRC16;				// ����CRC16ֵ
		ADUPtr[PDUData.PDULength+2] = CRC16>>8;
		ADUData.ADULength=PDUData.PDULength+3;		// ADU���ȵ���PDU���ȼ�1����ַ��2��CRC�ַ�
		ADUData.FrameOK = TRUE;						// ���ADU����֡׼������
		
		return (ADU_CONTROL *) &ADUData;			
	}
	else
		return NULL;
}








//******************************************************************************************
// �������ƣ�IdleModbus
// �����������
// ���������MODBUS����״̬
// ����������
//******************************************************************************************
uint8 IdleModbus(void)
{
	static uint8 err;
	static uint8  modbus_state=0;
	
#ifdef	UCOSII
	OSSemPend(mb_handle_event_ptr,0,&err);
#endif	
	
	err = MB_MASTER_BUSY;
	switch ( modbus_state )
	{
      	case 0:
         	/******************************************************************/
         	/*       CHECK FOR THE NEXT MESSAGE TO SEND                       */
         	/*       �����Ҫ���͵���Ϣ������������֡						  */
         	/******************************************************************/
        	if ( CheckModbusMessage() )
			{     
				/******************************************************************/
	         	/*       SEND THE POLL                                            */
	         	/*		��������֡												  */
	         	/******************************************************************/
	         	SendResponse(ADUData.ADUBuffPtr,ADUData.ADULength);// ����֡
	        	ADUData.FrameOK = FALSE;
	        	ADUData.ADULength = 0;
   				MB_Master.Status = (uint8)MB_MASTER_BUSY;				// ��������æ״̬
	        	Waite10mS(MB_POLL_TIMEOUT);			// ����Ӧ���ʱ 
	            modbus_state++;
 				
            }
            else
            	err = MB_MASTER_IDLE;	// û�й��ܴ����贫��
         	break;
      	case 1:
         	/******************************************************************/
         	/*       GET THE PACKET AND MAKE SURE IT IS COMPLETE              */
         	/******************************************************************/
         	if ( PDUData.FrameOK )	// �ɹ�����PDU֡
            {
                /******************************************************************/
         		/*       PARSE THE RESPONSE AND STORE THE DATA                    */
         		/*       ����Ӧ��֡������������									  */
         		/******************************************************************/
         		err = ParseReponseAndStoreData();
         		PDUData.FrameOK = FALSE;
         		Waite10mS(MB_POLL_DELAY);	
    		#ifdef	UCOSII
				OSMboxPost(mb_reply_event_ptr,&err);  		   
			#endif		 
            	modbus_state++;
            	break;
            }

         	/******************************************************************/
         	/*       IF NO RESPONSE RECEIVED TIME OUT AND SEND NEXT POLL      */
         	/******************************************************************/
         	if (g_TimeEnd)
            {
            	modbus_state = 0;
            	MB_Master.Status = MB_MASTER_IDLE;	// ��������
            	err = MB_RESPONSE_TIME_OUT;
 			#ifdef	UCOSII
				OSMboxPost(mb_reply_event_ptr,&err);  		   
			#endif	
            }
         	break;
      	case 2:
         	/******************************************************************/
         	/*       POLL DELAY                                               */
         	/*		��ѭ��ʱ												  */
         	/******************************************************************/

         	if (g_TimeEnd)
            {
               	modbus_state=0;
          		MB_Master.Status = MB_MASTER_IDLE;	// ��������
            }
      	default:
         	break;
      }
      
#ifdef	UCOSII
	if(MB_Master.Status == MB_MASTER_IDLE)
		OSSemPost(mb_idle_event_ptr);     
#endif
    return err;
}







