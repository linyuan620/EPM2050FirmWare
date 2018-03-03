/****************************************Copyright (c)**************************************************
**                                  ������Զ�������޹�˾
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
** ʹ��MODBUS���ܴ���
****************************************************/
#define	READ_COILS_EN		1	// ����Ȧ
#define	READ_DIS_INPUT_EN	1	// ����ɢ������
#define	READ_HOLD_REG_EN	1	// �����ּĴ���
#define	READ_INPUT_REG_EN	1	// ������Ĵ���
#define	WRITE_SING_COIL_EN	1	// д������Ȧ
#define	WRITE_SING_REG_EN	1	// д�����Ĵ���
#define	WRITE_MULT_COIL_EN	1	// д�����Ȧ
#define	WRITE_MULT_REG_EN	1	// д����Ĵ���
#define	MASK_WRITE_REG_EN	1	// �������μĴ���ָ��
#define	READ_WRITE_REG_EN	1	// ��д����Ĵ���

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

/* �������� */
typedef struct __MASTER_INFORMATION{
	uint8   Address;		  			// ��ַ
	uint8	LineProtocol;	  			// ��·��Э��
	uint16	BaudRate;	  				// ������ = BaudRate * 100	

	uint8	Parity;						// ��żУ��
	uint8	StopBit;					// ֹͣλ

	uint8*	RequestPtr;					// ���������ָ֡��
	uint8*  ReplyPtr;					// Ӧ��ָ֡��

	uint8	Status;						// ����״̬
	void	*ParPtr;					// �����ṹָ��

}MASTER_INFORMATION;


uint8 ModbusPoll(uint8 NodeID,uint8 FunctionCode,void *ParameterPtr);

void MBMasterIni(void);
//uint8 MReadHoldingReg(uint8 Adrress,uint8 StartAddr,uint16 Quantity,uint16 *DataPtr);
uint8 IdleModbus(void);

#endif
