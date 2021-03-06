/*********************************************************************************************************
Copyright (C), 2011-8-3 ,  Skyray Instrment Co.,LTD.
File name:      	init.h
Author:                   方卫龙       Version:       V2.5 Date:      2011-8-3 11:37:43
Description:    初始化函数集

Others:
Function List:
		extern void InitDevice1(void);
		extern void InitDevice2(void);

History:
   1. Date:                2011-8-3 12:52:43
       Author:             何顺杰
	   Modification:    规范了注释
************************************************************************************************************/
#ifndef __INIT_H
#define __INIT_H

/*********************************************************************************************************
**               头文件包含
*********************************************************************************************************/
#include "global.h"

/*********************************************************************************************************
**               C++代码兼容
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
//测量时间倒计时
    extern int TimeMeasCount;
    extern int TimeCount1s;
//按键时间倒计时
    extern int TimeKeyCount;
    extern int TimeMotoCount ;

    /*********************************************************************************************************
    * Function name:         MachineFristRun
    * Descriptions:         第一次运行，run type 为2 或3是将是恢复厂家设置功能
    * input parameters:     无
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void MachineFristRun(uint8 run_type);

    /*********************************************************************************************************
    * Function name:        IInitDevice1
    * Descriptions:         第一要初始化的对象
    * input parameters:     无
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void InitDevice1(void);

    /*********************************************************************************************************
    * Function name:        IInitDevice2
    * Descriptions:         第二要初始化的对象
    * input parameters:     无
    * output parameters:    无
    * Returned value:       无
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

