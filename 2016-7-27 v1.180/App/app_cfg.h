/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-EVAL Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.10
* Programmer(s) : BAN
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/

#define  APP_OS_PROBE_EN                         DEF_DISABLED
#define  APP_PROBE_COM_EN                        DEF_DISABLED

/*
*********************************************************************************************************
*                                              TASKS NAMES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/
#define	 MUTEX_LCDUSED_PRIO   3                             //互斥资源LCD防锁调解占用的优先级
#define  MUTEX_I2CUSED_PRIO   4                             //互斥资源I2C防锁调解占用的优先级
#define  MUTEX_FLASHUSED_PRIO 5                             //互斥资源uart0防锁调解占用的优先级
#define  MUTEX_UART0USED_PRIO 6                             //互斥资源uart0防锁调解占用的优先级

#define  APP_TASK_START_PRIO                               	7
#define  APP_TASK_HistoryMemoryRead_PRIO					8
#define  APP_TASK_DataGather_PineTempCtrl_PRIO				9
#define  APP_TASK_MEAS_PRIO								   	10
#define  APP_TASK_KBD_PRIO                                 	11
#define  APP_TASK_ModbusMaster_PRIO							13
#define  APP_TASK_ModbusSlave_PRIO                          12
#define  APP_TASK_Refresh_Lcd_AnalogOutput_PRIO				14
#define  APP_TASK_DTU_PRIO							   		15
#define  APP_TASK_SHELL_PRIO							   	16//本来16

#define  OS_PROBE_TASK_PRIO              (OS_LOWEST_PRIO - 3)
#define  OS_TASK_TMR_PRIO                (OS_LOWEST_PRIO - 2)

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE                         	128
#define  APP_TASK_KBD_STK_SIZE                           	128
#define  APP_TASK_MEAS_STK_SIZE							 							256
#define  APP_TASK_HistoryMemoryRead_STK_SIZE							 							256
#define  APP_TASK_DataGather_PineTempCtrl_STK_SIZE						 						256
#define	 APP_TASK_ModbusMaster_STK_SIZE					     					256
#define  APP_TASK_ModbusSlave_STK_SIZE                       	256
#define  APP_TASK_Refresh_Lcd_AnalogOutput_STK_SIZE                       	128
#define  APP_TASK_DTU_STK_SIZE                           	256
#define  APP_TASK_SHELL_STK_SIZE                           	256

#define  OS_PROBE_TASK_STK_SIZE                          	128

/*
*********************************************************************************************************
*                                                  LIB
*********************************************************************************************************
*/

#define  uC_CFG_OPTIMIZE_ASM_EN                 DEF_ENABLED
#define  LIB_STR_CFG_FP_EN                      DEF_DISABLED

/*
*********************************************************************************************************
*                                                 PROBE
*********************************************************************************************************
*/

#define  OS_PROBE_TASK                                     1    /* Task will be created for uC/Probe OS Plug-In.        */
#define  OS_PROBE_TMR_32_BITS                              0    /* uC/Probe OS Plugin timer is a 16-bit timer.          */
#define  OS_PROBE_TIMER_SEL                                2    /* Select timer 2.                                      */
#define  OS_PROBE_HOOKS_EN                                 1    /* Hooks to update OS_TCB profiling members included.   */
#define  OS_PROBE_USE_FP                                   1




#endif
