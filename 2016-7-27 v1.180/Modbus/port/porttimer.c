/*
 * FreeModbus Libary: LPC214X Port
 * Copyright (C) 2007 Tiago Prado Lone <tiago@maxwellbohr.com.br>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer.c,v 1.1 2007/04/24 23:15:18 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{

//		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	
//		TIM_TimeBaseStructure.TIM_Period = 50 * usTim1Timerout50us;		  //50us = 1/1000000/50us
//		TIM_TimeBaseStructure.TIM_Prescaler = 0x71;		  //72  分频 1Mhz 1us
//		TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//	  TIM_Cmd(TIM2, ENABLE);

	TIM_TIMERCFG_Type TIM_ConfigStruct;
	TIM_MATCHCFG_Type TIM_MatchConfigStruct ;
	// Initialize timer 0, prescale count time of 10uS
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 1;//10;

	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 3;
	// Disable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = FALSE;
	//Nothing MR0.0 pin if MR0 matches it
	TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_NOTHING;
	// Set Match value, count value of 1000 (10 * 100uS = 1mS --> 10Hz)
	//TIM_MatchConfigStruct.MatchValue   = 10000;
	TIM_MatchConfigStruct.MatchValue   = 1750;// 100;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIM3, TIM_TIMER_MODE,&TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TIM3,&TIM_MatchConfigStruct);
	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(TIMER3_IRQn, ((0x01<<3)|0x01));
	/* Enable interrupt for timer 1 */
	NVIC_EnableIRQ(TIMER3_IRQn);
	TIM_Cmd(LPC_TIM3,ENABLE);

    return TRUE;
}


void
vMBPortTimersEnable(  )
{	
//	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	TIM_SetCounter(TIM2, 0);
//	TIM_Cmd(TIM2, ENABLE);
////	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	TIM_ClearIntPending(LPC_TIM3,TIM_MR3_INT);
	TIM_ResetCounter(LPC_TIM3);
	TIM_Cmd(LPC_TIM3,ENABLE);
}

void
vMBPortTimersDisable(  )
{
////	TIM_SetCounter(TIM2, 0);
//	TIM_Cmd(TIM2, DISABLE);
////	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);

	TIM_Cmd(LPC_TIM3,DISABLE);

}


void
TIMERExpiredISR( void )
{
    (void)pxMBPortCBTimerExpired();
}


/*********************************************************************************************************
* Function name:	    TIMER3_IRQHandler
* Descriptions:	    	TIMER3 interrupt handler sub-routine
* input parameters:    	无	
* output parameters:   	无
* Returned value:      	无
*********************************************************************************************************/
void TIMER3_IRQHandler(void)
{
	if (TIM_GetIntStatus(LPC_TIM3, TIM_MR3_INT)== SET)
	{
		TIMERExpiredISR();
	}
	TIM_ClearIntPending(LPC_TIM3, TIM_MR3_INT);
}
