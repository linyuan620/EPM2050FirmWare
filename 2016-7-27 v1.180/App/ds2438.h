/*********************************************************************************************************
Copyright (C), 2011-8-3 ,  Skyray Instrment Co.,LTD.
File name:      	ds2438.h
Author:           方卫龙    xiahonghai     heshunjie      Version:       V2.5 Date:      2011-8-3 11:37:43
Description:    电池电量检测  显示 等
Others:
Function List:
		extern volatile void DelayDQ(uint32 ulTime);
		extern void DS2438Init(void);
		extern void DS2438Rst(void);
		extern uint8 DS2438Read(void);
		extern void DS2438Write(uint8 WriteData);
		extern void RecallMem(uint8 PageX);	 //对存储页的值复制
		extern uint8 ReadOnePageRom(uint8 PageX,uint8 *StorePage);//读取该页的值
		extern void StartConvVol(void);	   //电压转换启动
		extern void StartConvTemp(void);   //温度转换启动
		extern signed char DisBattery(volatile fp32 *Battery);//显示电池电量等
History:
   1. Date:                2011-8-3 12:52:43
       Author:             何顺杰
	   Modification:    规范了注释
************************************************************************************************************/

#ifndef __DS2438_H
#define __DS2438_H

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
    **                    DS2438的控制端口
    *********************************************************************************************************/
#define		DQ					(1ul<<11)			//P2.11 单总线;

#define		SET_DIR(x)			((x) ? GPIO_SetDir(2,DQ,1): GPIO_SetDir(2,DQ,0))
#define		SET_DQ(x)			((x) ? GPIO_SetValue(2, DQ): GPIO_ClearValue(2, DQ))
#define		DQ_READ				GPIO_ReadValue(2) & DQ

#define 	READ_ROM			0X33
#define 	MATCH_ROM			0X55
#define 	SEACH_ROM			0XF0
#define 	SKIP_ROM			0XCC

#define 	WRITE_SP			0X4E
#define 	READ_SP				0XBE
#define 	COPY_SP				0X48

#define 	CONVERT_TEMP		0X44
#define 	CONVERT_VOL			0XB4
#define 	RECALL_MEM			0XB8

#define 	V_power_max			8.0f	//显示的电压档位 分割点	  和公式绑定，勿改。
#define		V_power_2			7.68f
#define 	V_power_1			7.37f
#define 	V_power_0			7.23f	//


#define		PAGE(x)				(uint8)x
    extern float C_power_max;
    extern volatile fp32	ResultAll[4];

    extern volatile void DelayDQ(uint32 ulTime);

    /*********************************************************************************************************
    * Function name:        DS2438Init
    * Descriptions:         用于单总线初始化，主要是Sets ICA, CA, EE, AD Bits active
    * input parameters:     无
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void DS2438Init(void);

    /*********************************************************************************************************
    * Function name:        DS2438Rst
    * Descriptions:         用于单总线复位
    * input parameters:     无
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void DS2438Rst(void);

    /*********************************************************************************************************
    * Function name:        DS2438Read
    * Descriptions:         用于读函数
    * input parameters:     无
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern uint8 DS2438Read(void);

    /*********************************************************************************************************
    * Function name:        DS2438Write
    * Descriptions:         用于写8位数据函数
    * input parameters:     WriteData
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void DS2438Write(uint8 WriteData);

    /*********************************************************************************************************
    * Function name:        RecallMem
    * Descriptions:         把PageX中的内容复制到SP中，以供read_sp使用
    * input parameters:     pagex  =0~7
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void RecallMem(uint8 PageX);

    /*********************************************************************************************************
    * Function name:        ReadOnePageRom
    * Descriptions:         将某一PageX中的内容读出
    * input parameters:     pagex  =0~7，  *StorePage存储数据
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern uint8 ReadOnePageRom(uint8 PageX,uint8 *StorePage);

    /*********************************************************************************************************
    * Function name:        StartConvVo
    * Descriptions:         启动电压转换
    * input parameters:
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void StartConvVol(void);

    /*********************************************************************************************************
    * Function name:        StartConvTemp
    * Descriptions:         启动温度转换
    * input parameters:
    * output parameters:    无
    * Returned value:       无
    *********************************************************************************************************/
    extern void StartConvTemp(void);


    /*********************************************************************************************************
    * Function name:        signed char DisBattery(fp32 *Battery)
    * Descriptions:         电池电量LCD显示
    * input parameters:     *Battery 电源管理芯片读出的值  应该调用ResultAll数组
    						*(Battery+1)   电压
    						*(Battery+2)   电流
    						*(Battery+3)   电量
    * output parameters:    无
    * Returned value:       0
    						-1			error
    *********************************************************************************************************/
    extern signed char DisBattery(volatile fp32 *Battery);




#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif

/********************************************************************************************************
  End Of File
********************************************************************************************************/
