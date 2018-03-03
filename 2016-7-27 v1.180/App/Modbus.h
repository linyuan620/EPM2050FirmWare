/*********************************************************************************************************
Copyright (C), 2011-8-3 ,  Skyray Instrment Co.,LTD.
File name:      MODBUS_H
Author:                   ������       Version:       V2.5 Date:      2011-8-3 11:37:43
Description:   modbus��������غ���

Others:
Function List:
History:
   1. Date:                2011-8-3 12:52:43
       Author:             ��˳��
	   Modification:    �淶��ע��
************************************************************************************************************/
#ifndef	MODBUS_H
#define MODBUS_H
/* ----------------------- Modbus includes ----------------------------------*/
#include "port.h"
#include "mb.h"
#include "mbport.h"

#define AD_COUNT_START	100
/* ----------------------- Defines ------------------------------------------*/
#define DISCRETE_INPUT_START        1
#define DISCRETE_INPUT_NDISCRETES   10
#define COIL_START                  1
#define COIL_NCOILS                 10
#define REG_INPUT_START             1
#define REG_INPUT_NREGS             16
#define REG_HOLDING_START           1
#define REG_HOLDING_NREGS           512
//#define FALSH_USER_START_ADDRESS    (uint32_t)0x08010000           //�����û�������ʹ�õ��ڲ��������ʼ��ַ

//�ڱ��ּĴ����У�������ַ��Ӧ�Ĺ��ܶ���
#define          HD_SYSTEM_TASK_FLAG            0		  //ϵͳ�����־
#define          HD_USER_PASSWORD_START         1         //�û�������ʼ��ַ��5����ͨ��1����������Ա��
#define          HD_SUPER_ADMIN_PASSWORD        6         //��������Ա������
#define          HD_CUR_USER_PASSWORD		    7		  //��ǰ�û����������
#define          HD_CUR_USER_NUMBER			    8         //��ǰ�û��ı��
#define          HD_CUR_INTERFACE			    91        //��ǰ����
#define          HD_TIME_YEAR                  10		  //ʱ�䣺��
#define          HD_TIME_MONTH                 11		  //ʱ�䣺��
#define          HD_TIME_DAY                   12		  //ʱ�䣺��
#define          HD_TIME_WEEK                  13		  //ʱ�䣺����
#define          HD_TIME_HOUR                  14		  //ʱ�䣺ʱ
#define          HD_TIME_MIN                   15		  //ʱ�䣺��
#define          HD_TIME_SEC                   16		  //ʱ�䣺��

/* ----------------------- Variables ---------------------------------*/
extern USHORT   usRegInputBuf[REG_INPUT_NREGS];
extern USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];
/*--------------------------Extern Functions------------------------------------*/
extern UCHAR xMBUtilGetBits( UCHAR * ucByteBuf, USHORT usBitOffset, UCHAR ucNBits );
extern void  xMBUtilSetBits( UCHAR * ucByteBuf, USHORT usBitOffset, UCHAR ucNBits,UCHAR ucValue );
extern void ReadRecordSimple(uint32 recordP,MEAS_RECORD *record);

void ModbusRegInit(uint8_t Fun);

#endif
