/*********************************************************************************************************
Copyright (C), 2011-8-3 ,  Skyray Instrment Co.,LTD.
File name:      	findBaseLine.h
Author:                 Version:       V2.5 Date:      2011-8-3 11:37:43
Description:

Others:
Function List:
	extern int8 find_baseline(int16 *y_data,uint8 loop_cnt);
	extern fp32 GetCalculationArea(DATA_TYPE *y_data);
	extern fp32 GetCalculationPeak(DATA_TYPE *y_data);
************************************************************************************************************/

#ifndef __FINDBASELINE_H
#define __FINDBASELINE_H

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


//#define Debug
#ifdef Debug
#define Print_Debug(x) printf("%s",(x))
#else
#define Print_Debug(x) {}
#endif




    typedef enum {false = 0x0,true} bool;

    /**********************************************
    func:根据x_data,y_data,寻找曲线的基线
    input：x_data，输入横坐标。y_data:输入y坐标
    	   length:坐标点数。loop_cnt：循环次数
    output:y_baseline，基线的y坐标
    return:0,无错误。-1：出错
    remarks：输入的数据必须是按照横坐标顺序排列的点
    **********************************************/
    extern int8 find_baseline(int16 *y_data,uint8 loop_cnt);

    /**********************************************
    func:根据x_data,y_data,基线的面积
    input：x_data，输入横坐标。y_data:输入y坐标
    	   length:坐标点数。
    output:基线的面积
    return:0,无
    remarks：输入的数据必须是按照横坐标顺序排列的点
    **********************************************/
    extern fp32 GetCalculationArea(DATA_TYPE *y_data);
    /**********************************************
    func:根据x_data,y_data,基线的峰高
    input：x_data，输入横坐标。y_data:输入y坐标
    	   length:坐标点数。
    output:基线的峰高
    return:0,无
    remarks：输入的数据必须是按照横坐标顺序排列的点
    **********************************************/
    extern fp32 GetCalculationPeak(DATA_TYPE *y_data);

#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif

/********************************************************************************************************
  End Of File
********************************************************************************************************/
