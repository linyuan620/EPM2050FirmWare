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
**               ͷ�ļ�����
*********************************************************************************************************/
#include "global.h"

/*********************************************************************************************************
**               C++�������
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
    func:����x_data,y_data,Ѱ�����ߵĻ���
    input��x_data����������ꡣy_data:����y����
    	   length:���������loop_cnt��ѭ������
    output:y_baseline�����ߵ�y����
    return:0,�޴���-1������
    remarks����������ݱ����ǰ��պ�����˳�����еĵ�
    **********************************************/
    extern int8 find_baseline(int16 *y_data,uint8 loop_cnt);

    /**********************************************
    func:����x_data,y_data,���ߵ����
    input��x_data����������ꡣy_data:����y����
    	   length:���������
    output:���ߵ����
    return:0,��
    remarks����������ݱ����ǰ��պ�����˳�����еĵ�
    **********************************************/
    extern fp32 GetCalculationArea(DATA_TYPE *y_data);
    /**********************************************
    func:����x_data,y_data,���ߵķ��
    input��x_data����������ꡣy_data:����y����
    	   length:���������
    output:���ߵķ��
    return:0,��
    remarks����������ݱ����ǰ��պ�����˳�����еĵ�
    **********************************************/
    extern fp32 GetCalculationPeak(DATA_TYPE *y_data);

#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif

/********************************************************************************************************
  End Of File
********************************************************************************************************/
