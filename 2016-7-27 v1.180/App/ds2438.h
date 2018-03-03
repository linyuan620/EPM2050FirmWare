/*********************************************************************************************************
Copyright (C), 2011-8-3 ,  Skyray Instrment Co.,LTD.
File name:      	ds2438.h
Author:           ������    xiahonghai     heshunjie      Version:       V2.5 Date:      2011-8-3 11:37:43
Description:    ��ص������  ��ʾ ��
Others:
Function List:
		extern volatile void DelayDQ(uint32 ulTime);
		extern void DS2438Init(void);
		extern void DS2438Rst(void);
		extern uint8 DS2438Read(void);
		extern void DS2438Write(uint8 WriteData);
		extern void RecallMem(uint8 PageX);	 //�Դ洢ҳ��ֵ����
		extern uint8 ReadOnePageRom(uint8 PageX,uint8 *StorePage);//��ȡ��ҳ��ֵ
		extern void StartConvVol(void);	   //��ѹת������
		extern void StartConvTemp(void);   //�¶�ת������
		extern signed char DisBattery(volatile fp32 *Battery);//��ʾ��ص�����
History:
   1. Date:                2011-8-3 12:52:43
       Author:             ��˳��
	   Modification:    �淶��ע��
************************************************************************************************************/

#ifndef __DS2438_H
#define __DS2438_H

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
    **                    DS2438�Ŀ��ƶ˿�
    *********************************************************************************************************/
#define		DQ					(1ul<<11)			//P2.11 ������;

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

#define 	V_power_max			8.0f	//��ʾ�ĵ�ѹ��λ �ָ��	  �͹�ʽ�󶨣���ġ�
#define		V_power_2			7.68f
#define 	V_power_1			7.37f
#define 	V_power_0			7.23f	//


#define		PAGE(x)				(uint8)x
    extern float C_power_max;
    extern volatile fp32	ResultAll[4];

    extern volatile void DelayDQ(uint32 ulTime);

    /*********************************************************************************************************
    * Function name:        DS2438Init
    * Descriptions:         ���ڵ����߳�ʼ������Ҫ��Sets ICA, CA, EE, AD Bits active
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void DS2438Init(void);

    /*********************************************************************************************************
    * Function name:        DS2438Rst
    * Descriptions:         ���ڵ����߸�λ
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void DS2438Rst(void);

    /*********************************************************************************************************
    * Function name:        DS2438Read
    * Descriptions:         ���ڶ�����
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern uint8 DS2438Read(void);

    /*********************************************************************************************************
    * Function name:        DS2438Write
    * Descriptions:         ����д8λ���ݺ���
    * input parameters:     WriteData
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void DS2438Write(uint8 WriteData);

    /*********************************************************************************************************
    * Function name:        RecallMem
    * Descriptions:         ��PageX�е����ݸ��Ƶ�SP�У��Թ�read_spʹ��
    * input parameters:     pagex  =0~7
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void RecallMem(uint8 PageX);

    /*********************************************************************************************************
    * Function name:        ReadOnePageRom
    * Descriptions:         ��ĳһPageX�е����ݶ���
    * input parameters:     pagex  =0~7��  *StorePage�洢����
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern uint8 ReadOnePageRom(uint8 PageX,uint8 *StorePage);

    /*********************************************************************************************************
    * Function name:        StartConvVo
    * Descriptions:         ������ѹת��
    * input parameters:
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void StartConvVol(void);

    /*********************************************************************************************************
    * Function name:        StartConvTemp
    * Descriptions:         �����¶�ת��
    * input parameters:
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void StartConvTemp(void);


    /*********************************************************************************************************
    * Function name:        signed char DisBattery(fp32 *Battery)
    * Descriptions:         ��ص���LCD��ʾ
    * input parameters:     *Battery ��Դ����оƬ������ֵ  Ӧ�õ���ResultAll����
    						*(Battery+1)   ��ѹ
    						*(Battery+2)   ����
    						*(Battery+3)   ����
    * output parameters:    ��
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
