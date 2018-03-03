/*********************************************************************************************************
Copyright (C), 2011-8-3 ,  Skyray Instrment Co.,LTD.
File name:      	key.h
Author:         ������ �ĺ麣 ��˳��      Version:       V2.5 Date:      2011-8-3 11:37:43
Description:   ������غ���

Others:
Function List:
	extern uint32 PorcEnterKey(uint32 KeyVol);
	extern uint8 PorcKey(uint8 KeyCtl);

History:
   1. Date:                2011-8-3 12:52:43
       Author:             ��˳��
	   Modification:    �淶��ע��
************************************************************************************************************/
#ifndef __KEY_H
#define __KEY_H

/*********************************************************************************************************
**               ͷ�ļ�����
*********************************************************************************************************/
#include "global.h"

/*********************************************************************************************************
**               C++�������
*********************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif                                                                  /*  __cplusplus                 */


    typedef struct
    {
        uint8 ParamName;
        uint8 font;		//
        uint8 length;
        uint8 decimal;
        uint8 DisSign;
    }
    DISDATSTR;


#define NUM_PARAM 30
//��������
#define PARAM_NOMAL	   				0
#define PARAM_SHIDUMENFA	   		1
#define PARAM_MAXHEAT		   		2
#define PARAM_MINHEATPOWER	   		3
#define PARAM_LIULIANG		   		4
#define PARAM_CYCLEOFSAMP	   		5
#define PARAM_HIGHV			   		6
#define PARAM_BIAOPIAN_JIAOZHUN	 	7
#define PARAM_T1_JIAOZHUN			8
#define PARAM_DUIBIDU				9
#define PARAM_RH_JIAOZHUN			10
#define	PARAM_QIYA_JIAOZHUN			11
#define	PARAM_LIULIANG_JIAOZHUN	 	12
#define PARAM_PUMP_JIAOZHUN		 	13
#define PARAM_MONITONGDAO		 	14
#define PARAM_MONIZHI		 		15
#define PARAM_T3_JIAOZHUN			16
#define PARAM_MASS					17
#define PARAM_MOTO					18
#define PARAM_ERR	   				19
#define PARAM_B	   					20
#define PARAM_K	   			 		21
#define PARAM_TESTTIMES	   			22
#define PARAM_GUANGSAN_CYCLE	   	23
#define PARAM_GUANGSAN_PM25_K	   	24
#define PARAM_GUANGSAN_PM10_K	   	25
#define PARAM_GUANGSAN_TSP_K	   	26
#define PARAM_SENSOR                27



    extern void DisValue2(fp32 fValuel,uint16 x,uint8 y, DISDATSTR *DisDat ,uint8 *DanWei);
    extern DISDATSTR  DisDat;
    extern  DISDATSTR const DisDat2[NUM_PARAM];

    extern MEAS_RECORD measRecord;

    extern uint16 cleanTime;

    extern uint16 sweepTime;

    extern uint16 currentGain;

    extern volatile uint8 CheckCount;   //��Ƭ������

    /*********************************************************************************************************
    * Function name:  PorcEnterKey
    * Descriptions:   ȷ��������
    * input parameters: KEYvol  no use
    * output parameters:   ��
    * Returned value:   0
    *********************************************************************************************************/
    extern uint32 	PorcEnterKey(uint32 KeyVol);
    extern void 	DealHMS(uint8 *arr,TimeDat dat)	;

    /*********************************************************************************************************
    * Function name:      PorcKey
    * Descriptions:       ��������
    * input parameters:     keyctl ����
    * output parameters:     ��
    * Returned value:        0
    *********************************************************************************************************/
    extern uint8 	PorcKey(uint8 KeyCtl);
    extern uint8 	strKey[30];
    extern uint8 	DatForRecord[64];
    extern uint8 	DatForSysRecord[11];

    extern TimeDat 	TimeDateKey;
    extern int16 	FilterRemain;

    extern uint8 	firstlook;  //��һ�ζ�e2prom ��ʷ����
    extern uint8 	NoBuzzry;	//��־���ǵ�һ�β���
    extern uint16 	KeyTimer;
    extern uint8 	KeyLockFlg;
    extern uint8 	KeyLockFlg1;
    extern volatile uint8 OnlineFlg;
    extern volatile uint8 NePrOffset; //������ʾƫ��
    extern volatile uint32 StandardMeasFlg;
    extern uint8 	MenuOffset;
    extern float	CaliParT1,CaliParT3,CaliParRH, CaliParP,  CaliParF1,	 CaliParF2,	CaliParPump,CaliParT4,CaliParRH2;
    extern float	CaliSetT1,CaliSetT3,CaliSetRH, CaliSetP,  CaliSetF1,	 CaliSetF2,	CaliSetPump, CaliSetT4, CaliSetRH2;
    extern void 	WriteRecord(uint32 recordP,MEAS_RECORD *record);
    extern MEAS_RECORD measRecord;
    extern void 	WriteRecordFlash(uint32 recordP,uint8 record[]);
    extern void 	ReadFlashRecordSimple(uint32 recordP,MEAS_RECORD *record);
    extern void 	WriteSysRecord(uint32 recordP,uint8 record[]);
    extern uint32 	EscKeyAutoPorc(uint32 allId);

#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif

/********************************************************************************************************
  End Of File
********************************************************************************************************/
