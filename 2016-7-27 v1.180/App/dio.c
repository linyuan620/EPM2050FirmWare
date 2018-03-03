/****************************************Copyright (c)****************************************************
**                            		skyray-instrument Co.,LTD.
**
**                                 http://www.skyray-instrument.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           dio.c
** Last modified Date:  2011-02-24
** Last Version:        V1.0
** Descriptions:        输出接口函数
**
**--------------------------------------------------------------------------------------------------------
** Created By:          方卫龙
** Created date:        2011-02-24
** Version:             V1.0
** Descriptions:        First version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         方卫龙
** Modified date:       2011-02-24
** Version:             V1.X
** Descriptions:
**
*********************************************************************************************************/
#include "dio.h"

/*********************************************************************************************************
* Function name:        KeyIOInit
* Descriptions:         KEY的IO初始化
* input parameters:     无
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void KeyIOInit(void)
{
    GPIO_SetDir(2,(KEY_NULL),0);
}

/*********************************************************************************************************
* Function name:        LEDIOInit
* Descriptions:         LED的IO初始化
* input parameters:     无
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void LEDIOInit(void)
{
//	GPIO_SetDir(0,(LCD_BL | LED |Print),1);
    GPIO_SetDir(0,(LCD_BL| LED),1);

}


/*********************************************************************************************************
* Function name:        SetLED
* Descriptions:         LED显示控制
* input parameters:     LEDNumber	LED编号
*						value		0 关，其他 开
* output parameters:    无
* Returned value:       无
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
* Descriptions:         SWI1的IO初始化
* input parameters:     无
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void SWI1IOInit(void)
{
    GPIO_SetDir(0,(SWI10),1);
    GPIO_SetDir(0,(SWI11),1);
    GPIO_SetDir(0,(SWI12),1);
}

/*********************************************************************************************************
* Function name:        SetSWI1
* Descriptions:         设置模拟开关
* input parameters:     channel 设置通道值
* output parameters:    无
* Returned value:       无
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
* Descriptions:         SWI2的IO初始化
* input parameters:     无
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void SWI2IOInit(void)
{
    GPIO_SetDir(0,(SWI20 |SWI21 |SWI22),1);
    GPIO_SetDir(2,(DQ),1);		  //此处测试用
}

/*********************************************************************************************************
* Function name:        SetSWI2
* Descriptions:         设置模拟开关
* input parameters:     channel 设置通道值
* output parameters:    无
* Returned value:       无
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
* Descriptions:         MOTOR的IO初始化
* input parameters:     无
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void MOTORIOInit(void)
{
    GPIO_SetDir(1,(MOTOR),1);
}

/*********************************************************************************************************
* Function name:        SetSWI1
* Descriptions:         设置模拟开关
* input parameters:     channel 设置通道值
* output parameters:    无
* Returned value:       无
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
* Descriptions:         MOTOR的IO初始化
* input parameters:     无
* output parameters:    无
* Returned value:       无
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
* Descriptions:         模拟板上电源控制口，初始化可以控制
* input parameters:     无
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void MeasPowerInit(void)
{
    GPIO_SetDir(0,(MEA_POWER | MEA_POWER_3V3),1);
}

/*********************************************************************************************************
* Function name:        SetBuzzer
* Descriptions:         设置蜂鸣器
* input parameters:     开关
* output parameters:    无
* Returned value:       无
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
* Descriptions:         设置蜂鸣器鸣叫
* input parameters:     个数，时间
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void BuzzerCtrl(uint16 count,uint16 on,uint16 off)
{
    BPcount=count;
    BPoffTime=off;
    BPonTime=on;
}
/*********************************************************************************************************
* Function name:        CutOffMeasBoard
* Descriptions:         关闭测量板上的电源，包括3.3V，±10V，+5V，以及所有I/O置低
* input parameters:     ctrl,1:打开，2：关闭
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void CutOffMeasBoard(uint8 ctrl)
{
    if(ctrl)
    {
        MEA_POW_SWI(1);		 			 //打开模拟板电源
        //OSTimeDly(OS_TICKS_PER_SEC);	 //延时1s
        Delay100uS(1000);				   /**/

        MEA_POW3V3_SWI(1);				 //打开模拟板3.3V
    }
    else
    {
        MEA_POW_SWI(1);		  //关闭模拟板电源
        MEA_POW3V3_SWI(1);
//		SET_SWI10(0);
//		SET_SWI11(0);
//		SET_SWI12(0);
//
//		SET_SWI20(0);
//		SET_SWI21(0);
//		SET_SWI22(0);		   //这三个开关一定得保证三个电极断开
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
