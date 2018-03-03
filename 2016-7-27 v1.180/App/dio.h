/*********************************************************************************************************
  Copyright (C), 2011-8-3  Skyray Instrment Co.,LTD.
  File name:      	dios.h
  Author:   		������    	Version:	V2.5        	Date:	2011-8-3 11:37:43
  Description:
  Others:
  Function List:
		extern void KeyIOInit(void);//KEY��IO��ʼ��
		extern void LEDIOInit(void);//LED��IO��ʼ��
		extern void SetLED(uint8 LEDNumber,uint8 value);LED��ʾ����
		extern void SWI1IOInit(void);SWI1��IO��ʼ��
		extern void SetSWI1(uint8 channel);����ģ�⿪��
		extern void SWI2IOInit(void);
		extern void SetSWI2(uint8 channel);
		extern void MOTORIOInit(void);MOTOR��IO��ʼ��
		extern void SetMOTOR(uint8 OnOff);
		extern void BuzzerIOInit(uint8 OnOff);
		extern void MeasPowerInit(void);
		extern void SetBuzzer(uint8 OnOff);
		extern void BuzzerCtrl(uint16 count,uint16 on,uint16 off);���÷���������
		extern void CutOffMeasBoard(uint8 ctrl);�رղ������ϵĵ�Դ������3.3V����10V��+5V���Լ�����I/O�õ�

  History:
   1. Date:                2011-8-3 12:52:43
       Author:             ��˳��
	   Modification:    �淶��ע��
************************************************************************************************************/
#ifndef __DIO_H
#define __DIO_H

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

    /*********************************************************************************************************
    **                    �̵����Ŀ��ƶ˿�
    *********************************************************************************************************/
#define		LCD_BL			(1 << 27)					/*P0.7 LCD�������*/
#define		LED				((1 << 5)|(1<<6)|(1<<7))
#define		LED1				((1 << 5))			/*P0.30,��������ָʾ��*/
#define		LED2				((1<<6))			/*P0.30,��������ָʾ��*/
#define		LED3				((1<<7))			/*P0.30,��������ָʾ��*/

#define		SET_LCD_BL(x)	((x) ? GPIO_SetValue(0, LCD_BL): GPIO_ClearValue(0, LCD_BL))
#define		SET_LED(x)		((x) ? GPIO_SetValue(0, LED): GPIO_ClearValue(0, LED))
#define		SET_LED1(x)		((x) ? GPIO_SetValue(0, LED1): GPIO_ClearValue(0, LED1))
#define		SET_LED2(x)		((x) ? GPIO_SetValue(0, LED2): GPIO_ClearValue(0, LED2))
#define		SET_LED3(x)		((x) ? GPIO_SetValue(0, LED3): GPIO_ClearValue(0, LED3))


#define		SWI10			(1 << 22)					/*P0.22 �Ե缫����*/
#define		SWI11			(1 << 21)					/*P0.21 �αȵ缫����*/
#define		SWI12			(1 << 20)					/*P0.20 �����缫����*/

#define		SET_SWI10(x)	((x) ? GPIO_SetValue(0, SWI10): GPIO_ClearValue(0, SWI10))
#define		SET_SWI11(x)	((x) ? GPIO_SetValue(0, SWI11): GPIO_ClearValue(0, SWI11))
#define		SET_SWI12(x)	((x) ? GPIO_SetValue(0, SWI12): GPIO_ClearValue(0, SWI12))

#define		SWI20			(1 << 17)					/*P0.17 8ͨ��ģ�⿪��A0*/
#define		SWI21			(1 << 19)					/*P0.19 8ͨ��ģ�⿪��A1*/
#define		SWI22			(1 << 18)					/*P0.18 8ͨ��ģ�⿪��A2*/

#define		SET_SWI20(x)	((x) ? GPIO_SetValue(0, SWI20): GPIO_ClearValue(0, SWI20))
#define		SET_SWI21(x)	((x) ? GPIO_SetValue(0, SWI21): GPIO_ClearValue(0, SWI21))
#define		SET_SWI22(x)	((x) ? GPIO_SetValue(0, SWI22): GPIO_ClearValue(0, SWI22))


#define		MOTOR			(1 << 18)					/*P1.18 ������*/
#define		SET_MOTOR(x)	((x) ? GPIO_SetValue(1, MOTOR): GPIO_ClearValue(1, MOTOR))

#define		Print			(1 << 4)					/*P0.4 ������ӡ����*/
#define		OPEN_Print(x)	((x) ? GPIO_SetValue(0, Print): GPIO_ClearValue(0, Print))

#define		Print_RST		(1 << 12)					/*P1.27 ������ӡRST*/
#define		RST_Print(x)	((x) ? GPIO_SetValue(2, Print_RST): GPIO_ClearValue(2, Print_RST))

#define		BUZZER			(1 << 28)					/*P0.6 ������*/
#define		SET_BUZZER(x)	((x) ? GPIO_SetValue(0, BUZZER): GPIO_ClearValue(0, BUZZER))

#define		MEA_POWER		(1 << 26)					/*P0.26 �������Դ ��/��*/
#define		MEA_POW_SWI(x)	((x) ? GPIO_SetValue(0, MEA_POWER):GPIO_ClearValue(0, MEA_POWER))

#define		MEA_POWER_3V3		(1 << 23)					/*P0.23 �������Դ3.3V ��/��*/
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
    * Descriptions:         KEY��IO��ʼ��
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void KeyIOInit(void);
    /*********************************************************************************************************
    * Function name:        LEDIOInit
    * Descriptions:         LED��IO��ʼ��
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void LEDIOInit(void);

    /*********************************************************************************************************
    * Function name:        SetLED
    * Descriptions:         LED��ʾ����
    * input parameters:     LEDNumber	LED���
    *						value		0 �أ����� ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void SetLED(uint8 LEDNumber,uint8 value);

    /*********************************************************************************************************
    * Function name:        SWI1IOInit
    * Descriptions:         SWI1��IO��ʼ��
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void SWI1IOInit(void);

    /*********************************************************************************************************
    * Function name:        SetSWI1
    * Descriptions:         ����ģ�⿪��
    * input parameters:     channel ����ͨ��ֵ
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void SetSWI1(uint8 channel);

    /*********************************************************************************************************
    * Function name:        SWI2IOInit
    * Descriptions:         SWI2��IO��ʼ��
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void SWI2IOInit(void);

    /*********************************************************************************************************
    * Function name:        SetSWI2
    * Descriptions:         ����ģ�⿪��
    * input parameters:     channel ����ͨ��ֵ
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void SetSWI2(uint8 channel);

    /*********************************************************************************************************
    * Function name:        MOTORIOInit
    * Descriptions:         MOTOR��IO��ʼ��
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void MOTORIOInit(void);

    /*********************************************************************************************************
    * Function name:        SetSWI1
    * Descriptions:         ���õ��
    * input parameters:     channel ����ͨ��ֵ
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void SetMOTOR(uint8 OnOff);

    /*********************************************************************************************************
    * Function name:        BuzzerIOInit
    * Descriptions:         ������IO��ʼ��
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void BuzzerIOInit(uint8 OnOff);

    /*********************************************************************************************************
    * Function name:        MeasPowerInit
    * Descriptions:         ģ����ϵ�Դ���ƿڣ���ʼ�����Կ���
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void MeasPowerInit(void);

    /*********************************************************************************************************
    * Function name:        SetBuzzer
    * Descriptions:         ���÷�����
    * input parameters:     ����
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void SetBuzzer(uint8 OnOff);

    /*********************************************************************************************************
    * Function name:        BuzzerCtrl
    * Descriptions:         ���÷���������
    * input parameters:     ������ʱ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void BuzzerCtrl(uint16 count,uint16 on,uint16 off);

    /*********************************************************************************************************
    * Function name:        CutOffMeasBoard
    * Descriptions:         �رղ������ϵĵ�Դ������3.3V����10V��+5V���Լ�����I/O�õ�
    * input parameters:     ctrl,1:�򿪣�2���ر�
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void CutOffMeasBoard(uint8 ctrl);


#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif

/********************************************************************************************************
  End Of File
********************************************************************************************************/

