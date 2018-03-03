/*********************************************************************************************************
  Copyright (C), 2011-8-3  Skyray Instrment Co.,LTD.
  File name:      	dios.h
  Author:   		方卫龙    	Version:	V2.5        	Date:	2011-8-3 11:37:43
  Description:
  Others:
  Function List:
		extern void KeyIOInit(void);//KEY的IO初始化
		extern void LEDIOInit(void);//LED的IO初始化
		extern void SetLED(uint8 LEDNumber,uint8 value);LED显示控制
		extern void SWI1IOInit(void);SWI1的IO初始化
		extern void SetSWI1(uint8 channel);设置模拟开关
		extern void SWI2IOInit(void);
		extern void SetSWI2(uint8 channel);
		extern void MOTORIOInit(void);MOTOR的IO初始化
		extern void SetMOTOR(uint8 OnOff);
		extern void BuzzerIOInit(uint8 OnOff);
		extern void MeasPowerInit(void);
		extern void SetBuzzer(uint8 OnOff);
		extern void BuzzerCtrl(uint16 count,uint16 on,uint16 off);设置蜂鸣器鸣叫
		extern void CutOffMeasBoard(uint8 ctrl);关闭测量板上的电源，包括3.3V，±10V，+5V，以及所有I/O置低

  History:
   1. Date:                2011-8-3 12:52:43
       Author:             何顺杰
	   Modification:    规范了注释
************************************************************************************************************/
#ifndef __DIO_H
#define __DIO_H

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

    /*********************************************************************************************************
    **                    继电器的控制端口
    *********************************************************************************************************/
#define		LCD_BL			(1 << 27)					/*P0.7 LCD背光控制*/
#define		LED				((1 << 5)|(1<<6)|(1<<7))
#define		LED1				((1 << 5))			/*P0.30,正常工作指示灯*/
#define		LED2				((1<<6))			/*P0.30,正常工作指示灯*/
#define		LED3				((1<<7))			/*P0.30,正常工作指示灯*/

#define		SET_LCD_BL(x)	((x) ? GPIO_SetValue(0, LCD_BL): GPIO_ClearValue(0, LCD_BL))
#define		SET_LED(x)		((x) ? GPIO_SetValue(0, LED): GPIO_ClearValue(0, LED))
#define		SET_LED1(x)		((x) ? GPIO_SetValue(0, LED1): GPIO_ClearValue(0, LED1))
#define		SET_LED2(x)		((x) ? GPIO_SetValue(0, LED2): GPIO_ClearValue(0, LED2))
#define		SET_LED3(x)		((x) ? GPIO_SetValue(0, LED3): GPIO_ClearValue(0, LED3))


#define		SWI10			(1 << 22)					/*P0.22 对电极开关*/
#define		SWI11			(1 << 21)					/*P0.21 参比电极开关*/
#define		SWI12			(1 << 20)					/*P0.20 工作电极开关*/

#define		SET_SWI10(x)	((x) ? GPIO_SetValue(0, SWI10): GPIO_ClearValue(0, SWI10))
#define		SET_SWI11(x)	((x) ? GPIO_SetValue(0, SWI11): GPIO_ClearValue(0, SWI11))
#define		SET_SWI12(x)	((x) ? GPIO_SetValue(0, SWI12): GPIO_ClearValue(0, SWI12))

#define		SWI20			(1 << 17)					/*P0.17 8通道模拟开关A0*/
#define		SWI21			(1 << 19)					/*P0.19 8通道模拟开关A1*/
#define		SWI22			(1 << 18)					/*P0.18 8通道模拟开关A2*/

#define		SET_SWI20(x)	((x) ? GPIO_SetValue(0, SWI20): GPIO_ClearValue(0, SWI20))
#define		SET_SWI21(x)	((x) ? GPIO_SetValue(0, SWI21): GPIO_ClearValue(0, SWI21))
#define		SET_SWI22(x)	((x) ? GPIO_SetValue(0, SWI22): GPIO_ClearValue(0, SWI22))


#define		MOTOR			(1 << 18)					/*P1.18 搅拌器*/
#define		SET_MOTOR(x)	((x) ? GPIO_SetValue(1, MOTOR): GPIO_ClearValue(1, MOTOR))

#define		Print			(1 << 4)					/*P0.4 蓝牙打印开关*/
#define		OPEN_Print(x)	((x) ? GPIO_SetValue(0, Print): GPIO_ClearValue(0, Print))

#define		Print_RST		(1 << 12)					/*P1.27 蓝牙打印RST*/
#define		RST_Print(x)	((x) ? GPIO_SetValue(2, Print_RST): GPIO_ClearValue(2, Print_RST))

#define		BUZZER			(1 << 28)					/*P0.6 蜂鸣器*/
#define		SET_BUZZER(x)	((x) ? GPIO_SetValue(0, BUZZER): GPIO_ClearValue(0, BUZZER))

#define		MEA_POWER		(1 << 26)					/*P0.26 测量板电源 开/关*/
#define		MEA_POW_SWI(x)	((x) ? GPIO_SetValue(0, MEA_POWER):GPIO_ClearValue(0, MEA_POWER))

#define		MEA_POWER_3V3		(1 << 23)					/*P0.23 测量板电源3.3V 开/关*/
#define		MEA_POW3V3_SWI(x)	((x) ? GPIO_SetValue(0, MEA_POWER_3V3):GPIO_ClearValue(0, MEA_POWER_3V3))

#define		RES1			1.0F
#define		RES2			10.0F
#define		RES3			20.0F
#define		RES4			50.0F
#define		RES5			75.0F
#define		RES6			100.0F
#define		RES7			250.0F
#define		RES8			500.0F
    /*********************************************************************************************************
    * Function name:        KeyIOInit
    * Descriptions:         KEY的IO初始化
    * input parameters:     无
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void KeyIOInit(void);
    /*********************************************************************************************************
    * Function name:        LEDIOInit
    * Descriptions:         LED的IO初始化
    * input parameters:     无
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void LEDIOInit(void);

    /*********************************************************************************************************
    * Function name:        SetLED
    * Descriptions:         LED显示控制
    * input parameters:     LEDNumber	LED编号
    *						value		0 关，其他 开
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void SetLED(uint8 LEDNumber,uint8 value);

    /*********************************************************************************************************
    * Function name:        SWI1IOInit
    * Descriptions:         SWI1的IO初始化
    * input parameters:     无
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void SWI1IOInit(void);

    /*********************************************************************************************************
    * Function name:        SetSWI1
    * Descriptions:         设置模拟开关
    * input parameters:     channel 设置通道值
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void SetSWI1(uint8 channel);

    /*********************************************************************************************************
    * Function name:        SWI2IOInit
    * Descriptions:         SWI2的IO初始化
    * input parameters:     无
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void SWI2IOInit(void);

    /*********************************************************************************************************
    * Function name:        SetSWI2
    * Descriptions:         设置模拟开关
    * input parameters:     channel 设置通道值
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void SetSWI2(uint8 channel);

    /*********************************************************************************************************
    * Function name:        MOTORIOInit
    * Descriptions:         MOTOR的IO初始化
    * input parameters:     无
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void MOTORIOInit(void);

    /*********************************************************************************************************
    * Function name:        SetSWI1
    * Descriptions:         设置电机
    * input parameters:     channel 设置通道值
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void SetMOTOR(uint8 OnOff);

    /*********************************************************************************************************
    * Function name:        BuzzerIOInit
    * Descriptions:         蜂鸣的IO初始化
    * input parameters:     无
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void BuzzerIOInit(uint8 OnOff);

    /*********************************************************************************************************
    * Function name:        MeasPowerInit
    * Descriptions:         模拟板上电源控制口，初始化可以控制
    * input parameters:     无
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void MeasPowerInit(void);

    /*********************************************************************************************************
    * Function name:        SetBuzzer
    * Descriptions:         设置蜂鸣器
    * input parameters:     开关
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void SetBuzzer(uint8 OnOff);

    /*********************************************************************************************************
    * Function name:        BuzzerCtrl
    * Descriptions:         设置蜂鸣器鸣叫
    * input parameters:     个数，时间
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void BuzzerCtrl(uint16 count,uint16 on,uint16 off);

    /*********************************************************************************************************
    * Function name:        CutOffMeasBoard
    * Descriptions:         关闭测量板上的电源，包括3.3V，±10V，+5V，以及所有I/O置低
    * input parameters:     ctrl,1:打开，2：关闭
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void CutOffMeasBoard(uint8 ctrl);


#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif

/********************************************************************************************************
  End Of File
********************************************************************************************************/

