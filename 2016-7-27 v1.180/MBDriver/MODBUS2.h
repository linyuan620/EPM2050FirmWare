/****************************************Copyright (c)**************************************************
**                                  ������Զ�������޹�˾
**
**                                 http://www.21cm.com.cn
**
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			MODBUS.h
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

//#include "config.h"
#ifndef __MODBUS2_H
#define __MODBUS2_H





#define	MAX_PDU_DATA_LENGTH		 253
#define	MAX_PDU_LENGTH		 	(MAX_PDU_DATA_LENGTH+1)
#define MAX_ADU_LENGTH  		(MAX_PDU_LENGTH+2)


//#define MAX_FUNCTION			10
#define EXCEPTION_CODE_1		1
#define EXCEPTION_CODE_2		2
#define EXCEPTION_CODE_3		3
#define EXCEPTION_CODE_4		4
#define EXCEPTION_CODE_5		5
#define EXCEPTION_CODE_6		6

#define MB_NO_ERR					0x00
#define ILLEGAL_FUNCTION			0x01
#define ILLEGAL_DATA_ADDR			0x02
#define ILLEGAL_DATA_VALUE			0x03	
#define SLAVE_DEVICE_FAILURE		0x04
#define ACKNOWLEDGE					0x05
#define SLAVE_DEVICE_BUSY			0x06
#define MEMORY_PARITY_ERR			0x08
#define GATEWAY_PATH_UNABLE			0x0A
#define GATEWAY_TARGET_DEV_FAIL		0x0B

 #define MB_FUN_NOT_DEF				0x80
#define MB_MASTER_BUSY				0x81
#define MB_MASTER_IDLE				0x82
#define MB_RESPONSE_TIME_OUT		0x82
#define MB_PARAMETER_ERR			0x83

#define	BROADCAST_ADDRESS	0


	
#ifndef AT_CODE
#define AT_CODE
#endif

typedef struct __PDU_RESPONSE{
	uint8*	PDUDataPtr; 	// ��������ָ��,���ܴ��봦�������轫��������ݰ����ڸ�ָ��Ŀ�ʼλʼ
  	uint8	PDUByteLength; 		// ��������������
	uint8	ExceptionCode;	// ��Ϊ������쳣���룬����������Ϊ0
 }PDU_RESPONSE;

/* PDU������ */
typedef struct __PDU_HANDLE{
	uint8*			PDUBuffPtr;					// PUD����ָ��,ΪNULLʱ��ʾ��Ч��PDU

	uint8	FrameOK;

	uint8 			FunctionCode;				// �����ܴ���
	uint8			PDULength;					// PDU�ֽڳ���
				   	
 //   PDU_RESPONSE   	Response;
	uint8	ExceptionCode;
}PDU_HANDLE;

extern PDU_HANDLE PDUData;


typedef struct __ADU_CONTROL{
	uint8* 	ADUBuffPtr;	  				// ADU������ָ��

	uint8	Address;
	uint16	ADULength;					// ADU�ֽڳ���

//	uint16	RedundancyCheck;			// ������У��,CRC/LRC

	uint8	EndT15;
	uint8	FrameOK;
//	uint8   TimeOut;					// ��ʱ��־
			
}ADU_CONTROL;

extern volatile ADU_CONTROL ADUData;

extern volatile uint8 g_TimeEnd;

void Start10mS(void);
void StartCountT35(void);
void StartCountT15(void);

void ReceOneChar(uint8 ReceCharacter);
uint8 PutDataInPUD(uint8 *CharPtr,uint16 Length);

extern uint8 PDUBuffer[256];//���ͣ�

void T15EndHandle(void);
void T35EndHandle(void);
void Waite10mS(uint32 time);
void Time10mSHandle(void);
// ����UC/OSII��������
extern void OSModbusServe	(void *pdata);
#endif

