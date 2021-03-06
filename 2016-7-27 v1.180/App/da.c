/****************************************Copyright (c)****************************************************
**                            		skyray-instrument Co.,LTD.
**
**                                 http://www.skyray-instrument.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           da.c
** Last modified Date:  2011-02-24
** Last Version:        V1.0
** Descriptions:
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
#include "da.h"

/*********************************************************************************************************
* Function name:        DelayDa
* Descriptions:         用于DA的延时函数,内部调用
* input parameters:     延时的数字
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
volatile void DelayDa(int32_t ulTime)
{
//	int32_t i=0;

    while (ulTime--)
    {
//		for (i = 0; i < 500; i++);
    }
}

/*********************************************************************************************************
* Function name:        DaIOInit
* Descriptions:         DA的IO初始化
* input parameters:     无
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void DaIOInit(void)
{
    GPIO_SetDir(1,(SDI_8831|CS_8831|SCK_8831|LDAC_8831),1);
}

/*********************************************************************************************************
* Function name:        DA8831Output
* Descriptions:         DA输出控制函数
* input parameters:     无
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void DA8831Output(uint16 digit)
{
    uint8 i;

    SET_LDAC_8831(1);
    DelayDa(1);
    SET_CS_8831(0);
    DelayDa(1);
    SET_SCK_8831(1);
    DelayDa(1);
    for(i=0; i<16; i++)
    {
        SET_SCK_8831(0);
        DelayDa(1);
        if((digit&0x8000)==0x8000)
        {
            SET_SDI_8831(1);
        }
        else
        {
            SET_SDI_8831(0);
        }
        DelayDa(1);
        SET_SCK_8831(1);
        DelayDa(1);
        digit=digit<<1;
    }
    SET_CS_8831(1);
    DelayDa(1);
    SET_LDAC_8831(0);
    DelayDa(5);
    SET_LDAC_8831(1);
    DelayDa(5);
}

/*********************************************************************************************************
* Function name:        AnalogOutput
* Descriptions:         模拟电压输出函数
* input parameters:     analogVoltage: 模拟电压值 mV
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void AnalogOutput(fp32 analogVoltage)
{
    fp32 voltage;

    if(analogVoltage>2500.0)
    {
        analogVoltage = 2500.0;
    }
    if(analogVoltage<-2500.0)
    {
        analogVoltage = -2500.0;
    }
    voltage = analogVoltage + 2500.0;
    DA8831Output((uint16)(voltage/5000.0*65535));
}

