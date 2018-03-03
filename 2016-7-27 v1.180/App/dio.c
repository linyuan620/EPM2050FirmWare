/****************************************Copyright (c)****************************************************
**                            		skyray-instrument Co.,LTD.
**
**                                 http://www.skyray-instrument.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           dio.c
** Last modified Date:  2011-02-24
** Last Version:        V1.0
** Descriptions:        ����ӿں���
**
**--------------------------------------------------------------------------------------------------------
** Created By:          ������
** Created date:        2011-02-24
** Version:             V1.0
** Descriptions:        First version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         ������
** Modified date:       2011-02-24
** Version:             V1.X
** Descriptions:
**
*********************************************************************************************************/
#include "dio.h"

/*********************************************************************************************************
* Function name:        KeyIOInit
* Descriptions:         KEY��IO��ʼ��
* input parameters:     ��
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void KeyIOInit(void)
{
    GPIO_SetDir(2,(KEY_NULL),0);
}

/*********************************************************************************************************
* Function name:        LEDIOInit
* Descriptions:         LED��IO��ʼ��
* input parameters:     ��
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void LEDIOInit(void)
{
//	GPIO_SetDir(0,(LCD_BL | LED |Print),1);
    GPIO_SetDir(0,(LCD_BL| LED),1);

}


/*********************************************************************************************************
* Function name:        SetLED
* Descriptions:         LED��ʾ����
* input parameters:     LEDNumber	LED���
*						value		0 �أ����� ��
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void SetLED(uint8 LEDNumber,uint8 value)
{
    switch (LEDNumber)
    {
    case 1:
        ;
        break;
    case 2:
        SET_LED(value);
        break;
    default:
        break;
    }
}

/*********************************************************************************************************
* Function name:        SWI1IOInit
* Descriptions:         SWI1��IO��ʼ��
* input parameters:     ��
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void SWI1IOInit(void)
{
    GPIO_SetDir(0,(SWI10),1);
    GPIO_SetDir(0,(SWI11),1);
    GPIO_SetDir(0,(SWI12),1);
}

/*********************************************************************************************************
* Function name:        SetSWI1
* Descriptions:         ����ģ�⿪��
* input parameters:     channel ����ͨ��ֵ
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void SetSWI1(uint8 channel)
{
    switch ( channel )
    {
    case 1:
        SET_SWI10(1);
        SET_SWI11(1);
        SET_SWI12(1);
        break;
    case 0:
    default:
        SET_SWI10(0);
        SET_SWI11(0);
        SET_SWI12(0);
        break;
    }
}
/*********************************************************************************************************
* Function name:        SWI2IOInit
* Descriptions:         SWI2��IO��ʼ��
* input parameters:     ��
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void SWI2IOInit(void)
{
    GPIO_SetDir(0,(SWI20 |SWI21 |SWI22),1);
    GPIO_SetDir(2,(DQ),1);		  //�˴�������
}

/*********************************************************************************************************
* Function name:        SetSWI2
* Descriptions:         ����ģ�⿪��
* input parameters:     channel ����ͨ��ֵ
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void SetSWI2(uint8 channel)
{
    switch ( channel )
    {
    case 1:
        SET_SWI20(1);
        SET_SWI21(0);
        SET_SWI22(0);
        break;
    case 2:
        SET_SWI20(0);
        SET_SWI21(1);
        SET_SWI22(0);
        break;
    case 3:
        SET_SWI20(1);
        SET_SWI21(1);
        SET_SWI22(0);
        break;
    case 4:
        SET_SWI20(0);
        SET_SWI21(0);
        SET_SWI22(1);
        break;
    case 5:
        SET_SWI20(1);
        SET_SWI21(0);
        SET_SWI22(1);
        break;
    case 6:
        SET_SWI20(0);
        SET_SWI21(1);
        SET_SWI22(1);
        break;
    case 7:
        SET_SWI20(1);
        SET_SWI21(1);
        SET_SWI22(1);
        break;
    case 0:
    default:
        SET_SWI20(0);
        SET_SWI21(0);
        SET_SWI22(0);
        break;
    }
}

/*********************************************************************************************************
* Function name:        MOTORIOInit
* Descriptions:         MOTOR��IO��ʼ��
* input parameters:     ��
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void MOTORIOInit(void)
{
    GPIO_SetDir(1,(MOTOR),1);
}

/*********************************************************************************************************
* Function name:        SetSWI1
* Descriptions:         ����ģ�⿪��
* input parameters:     channel ����ͨ��ֵ
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void SetMOTOR(uint8 OnOff)
{
    switch ( OnOff )
    {
    case 1:
        SET_MOTOR(1);
        break;
    case 0:
    default:
        SET_MOTOR(0);
        break;
    }
}

/*********************************************************************************************************
* Function name:        BuzzerIOInit
* Descriptions:         MOTOR��IO��ʼ��
* input parameters:     ��
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void BuzzerIOInit(uint8 OnOff)
{
    if(OnOff)
    {
        GPIO_SetDir(0,(BUZZER ),1);
    }
    else
    {
        GPIO_SetDir(0,(BUZZER ),0);
    }
}

/*********************************************************************************************************
* Function name:        MeasPowerInit
* Descriptions:         ģ����ϵ�Դ���ƿڣ���ʼ�����Կ���
* input parameters:     ��
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void MeasPowerInit(void)
{
    GPIO_SetDir(0,(MEA_POWER | MEA_POWER_3V3),1);
}

/*********************************************************************************************************
* Function name:        SetBuzzer
* Descriptions:         ���÷�����
* input parameters:     ����
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void SetBuzzer(uint8 OnOff)
{
    switch ( OnOff )
    {
    case 1:
        SET_BUZZER(1);
        break;
    case 0:
    default:
        SET_BUZZER(0);
        break;
    }
}

/*********************************************************************************************************
* Function name:        BuzzerCtrl
* Descriptions:         ���÷���������
* input parameters:     ������ʱ��
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void BuzzerCtrl(uint16 count,uint16 on,uint16 off)
{
    BPcount=count;
    BPoffTime=off;
    BPonTime=on;
}
/*********************************************************************************************************
* Function name:        CutOffMeasBoard
* Descriptions:         �رղ������ϵĵ�Դ������3.3V����10V��+5V���Լ�����I/O�õ�
* input parameters:     ctrl,1:�򿪣�2���ر�
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void CutOffMeasBoard(uint8 ctrl)
{
    if(ctrl)
    {
        MEA_POW_SWI(1);		 			 //��ģ����Դ
        //OSTimeDly(OS_TICKS_PER_SEC);	 //��ʱ1s
        Delay100uS(1000);				   /**/

        MEA_POW3V3_SWI(1);				 //��ģ���3.3V
    }
    else
    {
        MEA_POW_SWI(1);		  //�ر�ģ����Դ
        MEA_POW3V3_SWI(1);
//		SET_SWI10(0);
//		SET_SWI11(0);
//		SET_SWI12(0);
//
//		SET_SWI20(0);
//		SET_SWI21(0);
//		SET_SWI22(0);		   //����������һ���ñ�֤�����缫�Ͽ�
//
//		SET_SDI_8831(0);
//		SET_SCK_8831(0);
//		SET_LDAC_8831(0);
//		SET_CS_8831(0);
//
//		SET_SCK_7606(0);
//		SET_SDO_7606(0);
//		SET_CS_7606(0);
//		SET_CONVST_7606(0);
    }
}
