/****************************************Copyright (c)**************************************************
**                                  ������Զ�������޹�˾
**
**                                 http://www.21cm.com.cn
**
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			MBSlave.h
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

#ifndef __MBSLAVE_H
#define __MBSLAVE_H
#include    "modbus.h"
/***************************************************
** ʹ��MODBUS���ܴ���
****************************************************/
#define	READ_COILS_EN		0	// ����Ȧ
#define	READ_DIS_INPUT_EN	0	// ����ɢ������
#define	READ_HOLD_REG_EN	0	// �����ּĴ���
#define	READ_INPUT_REG_EN	0	// ������Ĵ���
#define	WRITE_SING_COIL_EN	1	// д������Ȧ
#define	WRITE_SING_REG_EN	0	// д�����Ĵ���
#define	WRITE_MULT_COIL_EN	0	// д�����Ȧ
#define	WRITE_MULT_REG_EN	0	// д����Ĵ���
#define	MASK_WRITE_REG_EN	0	// �������μĴ���ָ��
#define	READ_WRITE_REG_EN	0	// ��д����Ĵ���


/***************************************************
** ������Ȧ�Ĳ�����������Ȧ������
****************************************************/
#define END_COILS_ADDR			8
/***************************************************
** ������ɢ����Ĳ�����������ɢ������������
****************************************************/
#define END_DISC_INPUT_ADDR		8
/***************************************************
** ��������Ĵ����Ĳ�������������Ĵ���������
****************************************************/
#define END_INPUT_REG_ADDR		123
/***************************************************
** ���ñ��ּĴ����Ĳ��������屣�ּĴ�������
****************************************************/
#define END_HOLDING_REG_ADDR	123				



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

/* �ӻ����� */
typedef struct __SLAVE_INFORMATION{
	uint8   Address;		  			// ��ַ
	uint8	LineProtocol;	  			// ��·��Э��
	uint16	BaudRate;	  				// ������ = BaudRate * 100	

	uint8	Parity;						// ��żУ��
	uint8	StopBit;					// ֹͣλ

	uint8*	RequestPtr;					// ���������ָ֡��
}SLAVE_INFORMATION;

// �ӻ�������Ϣ
extern SLAVE_INFORMATION SlaveDevice;

// ���ܴ���ṹ
typedef struct __FUNCTION_ARRAY{
	uint8	 Code;
	void	 (*Function)(PDU_RESPONSE *Response);
}FUNCTION_ARRAY;



void MBSlaveIni(SLAVE_INFORMATION *DevicInfPtr);
void IdleModbus(void);

#endif