/*********************************************************************************************************
Copyright (C), 2011-8-3 ,  Skyray Instrment Co.,LTD.
File name:      	init.h
Author:                   ������       Version:       V2.5 Date:      2011-8-3 11:37:43
Description:    ��ʼ��������

Others:
Function List:
		extern void InitDevice1(void);
		extern void InitDevice2(void);

History:
   1. Date:                2011-8-3 12:52:43
       Author:             ��˳��
	   Modification:    �淶��ע��
************************************************************************************************************/
#ifndef __INIT_H
#define __INIT_H

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

    extern fp32  measResult;
    extern volatile uint32 recordPoint;
    extern  uint32 recordOutput;
    extern volatile uint32 SysrecordPoint;

    extern float MotorTime;
    extern uint8 FujiStartFlg,keyBuzzerBitFlg;
    extern uint8 LCDContrast;
    extern uint8 SensorSel;
    extern TimeDat TimeDate;
    extern TimeDat MeaserStartTime;
    extern TimeDat MeaserEndTime;
//����ʱ�䵹��ʱ
    extern int TimeMeasCount;
    extern int TimeCount1s;
//����ʱ�䵹��ʱ
    extern int TimeKeyCount;
    extern int TimeMotoCount ;

    /*********************************************************************************************************
    * Function name:         MachineFristRun
    * Descriptions:         ��һ�����У�run type Ϊ2 ��3�ǽ��ǻָ��������ù���
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void MachineFristRun(uint8 run_type);

    /*********************************************************************************************************
    * Function name:        IInitDevice1
    * Descriptions:         ��һҪ��ʼ���Ķ���
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void InitDevice1(void);

    /*********************************************************************************************************
    * Function name:        IInitDevice2
    * Descriptions:         �ڶ�Ҫ��ʼ���Ķ���
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void InitDevice2(void);

    extern void ComeBackConfig(uint8 type);

#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif

/********************************************************************************************************
  End Of File
********************************************************************************************************/
