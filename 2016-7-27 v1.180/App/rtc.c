/****************************************Copyright (c)****************************************************
**                            		skyray-instrument Co.,LTD.
**
**                                 http://www.skyray-instrument.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           rtc.c
** Last modified Date:  2011-06-10
** Last Version:        V1.0
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Created By:          方卫龙
** Created date:        2011-06-10
** Version:             V1.0
** Descriptions:        First version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         方卫龙
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include "rtc.h"
#include <ucos_ii.h>

uint32 		WEBxintiao;

RTC_TIME_Type RTCFullTimeData;


/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		RTC interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void RTC_IRQHandler(void)
{
    OS_CPU_SR  cpu_sr = 0u;
    if (RTC_GetIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE))
    {
        RTC_GetFullTime (LPC_RTC, &RTCFullTimeData);
        OS_ENTER_CRITICAL();
        TimeDate.year = RTCFullTimeData.YEAR;
        TimeDate.month = RTCFullTimeData.MONTH;
        TimeDate.date = RTCFullTimeData.DOM;
        TimeDate.hour = RTCFullTimeData.HOUR;
        TimeDate.minute = RTCFullTimeData.MIN;
        TimeDate.second = RTCFullTimeData.SEC;
        if((TimeMeasCount!=0)&&FujiStartFlg )
        {
            TV_GK += LiuLiang_GK;
            TV_BK += LiuLiang_BK;
        }
        if(TimeMeasCount)
        {
            TimeMeasCount-=1;
        }
        if(CountReadTime)
        {
            CountReadTime-=1;
        }
        TimeCount1s = 1;
        WEBxintiao++;//检测到数据 当心跳包用

        OS_EXIT_CRITICAL();
        RTC_ClearIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE);
    }
    if (RTC_GetIntPending(LPC_RTC, RTC_INT_ALARM))
    {
        RTC_ClearIntPending(LPC_RTC, RTC_INT_ALARM);
    }
}

void RTCInit(void)
{
    RTC_TIME_Type RTCFullTime;

    /* RTC Block section ------------------------------------------------------ */
    // Init RTC module
    RTC_Init(LPC_RTC);

    /* Disable RTC interrupt */
    NVIC_DisableIRQ(RTC_IRQn);
    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(RTC_IRQn, ((PRIO_GROUP_RTC<<3)|PRIO_RTC));

    /* Enable rtc (starts increase the tick counter and second counter register) */
    RTC_ResetClockTickCounter(LPC_RTC);
    RTC_Cmd(LPC_RTC, ENABLE);
    RTC_CalibCounterCmd(LPC_RTC, DISABLE);

    /* Set current time for RTC */
    // Current time is 8:00:00PM, 2009-04-24
//	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_SECOND, 0);
//	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MINUTE, 0);
//	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_HOUR, 20);
//	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MONTH, 4);
//	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_YEAR, 2009);
//	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, 24);

    /* Set ALARM time for second */
    RTC_SetAlarmTime (LPC_RTC, RTC_TIMETYPE_SECOND, 10);

    // Get and print current time
    RTC_GetFullTime (LPC_RTC, &RTCFullTime);

    /* Set the CIIR for second counter interrupt*/
    RTC_CntIncrIntConfig (LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);
    /* Set the AMR for 10s match alarm interrupt */
    RTC_AlarmIntConfig (LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);

    /* Enable RTC interrupt */
    NVIC_EnableIRQ(RTC_IRQn);
}
