/*****************************************************************************
  Copyright (C), 2011-8-3  Skyray Instrment Co.,LTD.
  File name:      global.h
  Author:       Version: V2.5       Date: 2011-8-3 11:18:18
  Description:    �����û�������ͷ�ļ�
  Others:
  Function List:
  History:
*****************************************************************************/
#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "lpc17xx_libcfg.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_rtc.h"

#include <includes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef		TRUE
#define		TRUE	1
#endif

#ifndef		FALSE
#define		FALSE	0
#endif

#ifndef		ERR
#define		ERR		0
#endif

#ifndef     OK
#define		OK		1
#endif

//-------------------------Menu Lcd start---------------------------------------------------
#define  DIS_AREA_X           239u
#define  DIS_AREA_Y           159u
/*
//--------<<< Use Configuration Wizard in Context Menu >>>----
*/
/*
//<h> ��������
//	<o0> �˵���Ŀ <0-20:1>
//<o1> ��ʷ��¼���� <100-2000:100>
//</h>
*/
#define MENUADDSIZE	15			//�˵�����Ŀ
// #define HistroyPoint  1000


/***********************************/
/* EEPROM Address  Don't try to change*/
/************************************/
#define INIT_FLAG		2
#define RECORDPOINT2 	5  //��4���ֽ�

#define ConInf				10  //��λ���������á� 0��ֽ 1��  2 ������ 3������ 4���� 5��; 6-9IP��ַ
#define TESTADD				15  //����I2C�Լ��ַ��//20
#define RUNSETADD8   		25 //�������� ֵ
#define RUNSETADD32   		45 //�������� ֵ

//У׼���EEPROM ��ַ
#define JIAOZHUN_T1	100//У׼ֵ	
#define JIAOZHUN_T3	104//У׼ֵ	
#define JIAOZHUN_RH	108//У׼ֵ	
#define JIAOZHUN_P	112//У׼ֵ	
#define JIAOZHUN_F1	116//У׼ֵ	
#define JIAOZHUN_F2	120//У׼ֵ	
#define JIAOZHUN_BIAOPIAN	124//У׼ֵ	

#define JIAOZHUN_T1_TIME	130//У׼ֵ	  У׼��ʱ���
#define JIAOZHUN_T3_TIME	138//У׼ֵ	
#define JIAOZHUN_RH_TIME	146//У׼ֵ	
#define JIAOZHUN_P_TIME		154//У׼ֵ	
#define JIAOZHUN_F1_TIME	162//У׼ֵ	
#define JIAOZHUN_F2_TIME	170//У׼ֵ	
#define JIAOZHUN_BIAOPIAN_TIME	188//У׼ֵ	
#define SYSRECORDPOINT2 200	//��4���ֽ�
#define RESERVED_8 220	//
#define RESERVED_32 300	//

//������У׼ֵ��չ��ַ
#define JIAOZHUN_T4	       	500 //У׼ֵ
#define JIAOZHUN_T4_TIME	504 //У׼ֵ
#define JIAOZHUN_RH2	    512 //У׼ֵ
#define JIAOZHUN_RH2_TIME	516 //У׼ֵ

//��ɢkֵ��ַ
#define PM25_GS_32ADDR        520
#define PM10_GS_32ADDR        524
#define TSP_GS_32ADDR         528
#define SENSOR_SEL_8ADDR      532

#define RECORDSTAR			15500//����ʹ�ã�

/***********************************************
FLASH ��ַ����  ��Ҫ����
***********************************************/
#define RECORDFLASHSTAR 0x60000 //ǰ�涼����ֿ�ȥ��
#define RECORDSYSLOG    0XB0000 //ϵͳ��־��ַ��ʼ

#define JIAOZHUN_BIAOPIAN_TEST_TIME	  360//У׼uʱ����ʱ��	

#define JIAOZHUN_ON	1//У׼����	

/************************************************************************/

/**************************************************************
			���ƶ˿�����
��Ҫ��Դ����
AD :	T1  ����¶�
		T2	���ȹ��¶�
		T3	�������¶�
		RH	ʪ��
		P1	��ѹ
		F2	�������  ��F1	�������� ��F2�����
DA:
	1���ò���//������DA
	2���ɼ��������ֵĸ�ѹ����

TTL: ���õ���2�� TTL0  TTL2 ÿ��8·������0���������2��������

	 1��ѹֽ��� �̼�ֻ����TTL����� ����ʱֹͣҪ��ϻ�е�ṹȷ����
	 2����ֽ��� TTL��� �����ת���������á�
	 3�����ȿ���
	 4������
*****************************************************************/
#define	NUN_AD		 20
#define	NUN_SENSOR	 11
#define	NUN_DA		 10

//AD�˿�
#define	AD_SHIDU	   9      //                                   ���Բ�ʪ�ȴ�����ADC�˿ں�
#define	AD_WENDU1	   14	  //���������¶� �����¶�              �������¶ȴ�����ADC�˿ں�
#define	AD_WENDU2	   13	  //                                   ���ȹ��¶��¶ȴ�����adc�˿ں�
#define	AD_WENDU3	   10	  //�ռ�����Ʒ�¶� ���                ���Բ��¶ȴ�����ADC�˿ں�
#define	AD_LIULIANG	   6  //                                     ����������������ADC�˿ں�
#define	AD_QIYA		   11   //                                   ����ѹ��ѹ������ADC�˿ں�
#define AD_CYK_SHIDU   15 //                                     ������ʪ�ȴ�����ADC�˿ں�                                     
#define AD_TCQ_WENDU   12 //                                     ̽�����¶ȴ�����ADC�˿ں�  

#define	VCC_5		 0
#define	VCC_12		 1
#define	VSS_12		 2
#define	VCC_24		 3

//DA�˿�
#define	DA_PUMP	     2

//TTL�˿�����
#define TTL_YAZHIMOTO 	 2
#define TTL_ZHOUZHIMOTO  3
#define TTL_JIAREI 		 4
#define TTL_GAOYA 	     5
#define GAOYA_OPEN 	     0
#define GAOYA_CLOSE 	 1
#define TTL_PUMP 	     6

#define TTL_BRAY_Ctrl1 	     3
#define TTL_BRAY_Ctrl2 	     4


#define TTL_0 			 0 //�����
#define TTL_2 			 2 //�����
#define TTL_1 			 1 //������

#define TTL_HIGH 		 1
#define TTL_LOW  		 0

//������ ���ƶ���
#define	CGQ_WENDU1_KL	 0	 //�������¶� �㹤�������� ���غ���
#define	CGQ_SHIDU		 1   //���Բ�ʪ�ȴ�����
#define	CGQ_LIULIANG	 2
#define	CGQ_QIYA	     3
#define	CGQ_WENDU2		 4	 //���ȹ��¶ȣ�������
#define	CGQ_WENDU3		 5	 //���Բ��¶� 
#define CGQ_TCQ_WENDU    6   //̽�����¶ȴ�����
#define CGQ_CYK_SHIDU_KL 7   //������ʪ��  ���غ���
#define	CGQ_WENDU1_XY	 8	 //�������¶� �㹤�������� ��������
#define CGQ_CYK_SHIDU_XY 9   //������ʪ��  ��������
#define CGQ_LIULIANG_HONEYWELL 10   //���� ����Τ��

//�����������

/*
//Control END
 */

/***************************************************
�ж����ȼ�	  ��ֵ�͵����ȼ���
���ȼ��� ������ռ��Ƕ��
ͬ��䲻Ƕ�ס�
��5��λ�������ȼ�  ��2Ϊ���飬��3λ�������ȼ�
****************************************************/
//0��
#define PRIO_GROUP_I2C	      0X00
#define PRIO_GROUP_RTC	      0X00
#define PRIO_I2C  0X00
#define PRIO_RTC  0X01

//1��

//2��
#define PRIO_GROUP_UART0	  0X02
#define PRIO_GROUP_UART1	  0X02
#define PRIO_GROUP_UART2	  0X02
#define PRIO_GROUP_UART3	  0X02
#define PRIO_UART0	  0X00
#define PRIO_UART1	  0X01
#define PRIO_UART2	  0X02
#define PRIO_UART3	  0X03


//3��
#define PRIO_GROUP_T0	      0X03
#define PRIO_GROUP_T1	      0X03
#define PRIO_GROUP_T2	      0X03
#define PRIO_GROUP_T3	      0X03
#define PRIO_T0	      0X00
#define PRIO_T1	      0X01
#define PRIO_T2	      0X02
#define PRIO_T3	      0X03


#define FUJITIME	      1000



/*
//<e>	�汾����
// <s2> Change Firmware Version(En)
// <s3> Change Hardware Version(En)
// <s5> Change Firmware Version(Ch)
// <s6> Change Hardware Version(Ch)
//</e>
 */

#define  FI1_EN  "      Skyray-instrument Portable -"
#define  FI2_EN  "     Water Analyzer - Heavy metal,-"
#define  FI3_EN	 "firmware software  V1.183"
#define  HW_EN   "HW Version: PM2.5 B1"
#define  FI1_CH  "     ���������������ϵͳ"
#define  FI2_CH  "�����ǹ̼����� V1.183"
#define  HW_CH   "Ӳ���汾: PM2.5 B1"


#define  MENU_SIZE            (MENUADDSIZE*2 + 49)
#define  CAL_QUERY_PAGE       10

#define  CHINA_Y              13
#define  CHINA_SX             0
#define  CHINA_EX             127

#define  NUM6_SX              16                           	//24 24*12 6�������������X
#define  NUM6_SY              28                           	//24*12 6�������������Y

#define  EFONT_12             6
#define  EFONT_16             8
#define  EFONT_24             12

#define  BUTTON_SX            24
#define  BUTTON_SY            36

#define  MENU_SX              4
#define  MENU0_SY             1
#define  MENU1_SY             21
#define  MENU2_SY             36
#define  MENU3_SY             51

#define  WordNum             51			  //�Զ��庺�ֿ��С
#define  ADRANGESEL_10            0x03			  //�ɼ���Χ 0-10 �ɼ�����Ҫ����2����
#define  ADRANGESEL_5             0

#define  MAINDIS_SX           32      						//����TEM,��ʾ
#define  MAINDIS_SX24         22
#define  SPACE126X            6
#define  SPACE2412X           12
//-------------------------Menu Lcd end---------------------------------------------------

//-------------------------Key start---------------------------------------------------
#define KEY_REPEAT_TIME      (0xffE0)//(0xffd0) //(0xff05)
#define KEY_SERIES_TIME    	 (0xffE4)//(0xffE4)//(0xffef)   //(0xff24)  
#define RETURN_MENU0_TIME    (0x1500)

#define KEY_UP               0x000001b8          			//0x00001c30
#define KEY_DOWN             0x000001f0          			//0x00001870
#define KEY_START            0x000000f8          			//0x00001470
#define KEY_ENTER            0x00000178          			//0x00001c60
#define KEY_RIGHT            0x000001d8
#define KEY_NULL             0x000001f8
#define KEY_ESC              0x000001e8          			//0x000001e8

#define TDAIDIN              WenDuT1  // WenDuT3        ��ʽ�����汾��T1�������T1  			//0x000001e8

#define KEY_FILTER			 0x000001f8
#define FLASH_ADD_ZK1212	 216640//1212�ֿ⿪ʼ��ַ
#define FLASH_ADD_ZK1616	 0//1212�ֿ⿪ʼ��ַ

//-------------------------Key end---------------------------------------------------
#define ERRORRECORDPOINT 	15480

typedef unsigned char  uint8;                   			// �޷���8λ���ͱ���
typedef signed   char  int8;                    			// �з���8λ���ͱ���
typedef unsigned short uint16;                  			// �޷���16λ���ͱ���
typedef signed   short int16;                   			// �з���16λ���ͱ���
typedef unsigned int   uint32;                  			// �޷���32λ���ͱ���
typedef signed   int   int32;                   			// �з���32λ���ͱ���
typedef float          fp32;                    			// �����ȸ�����(32λ����)
typedef double         fp64;                    			// ˫���ȸ�����(64λ����)
typedef signed   short DATA_TYPE;

typedef struct
{
    uint16 year;
    uint8 month;
    uint8 date;
    uint8 hour;
    uint8 minute;
    uint8 second;
} TimeDat;

typedef struct {
    TimeDat		recordTimeStart;
    TimeDat		recordTimeEnd;
    fp32		recordResult;
    fp32		I;
    fp32		I0;
    fp32  		TV;
    fp32		RH;
    fp32		T1;
    fp32  		P ;//Ts;//��Ϊ����ѹ��
    fp32		recordResultG;//Te;//��Ϊ�������
    fp32		MAS;
} MEAS_RECORD;

typedef struct {
    fp32	 I;
    fp32   abs_offset;
    uint8  order;
} CNT_RECORD;

#include "dio.h"
#include "uarts.h"
#include "times.h"
#include "i2c.h"
#include "public.h"
#include "init.h"
#include "da.h"
#include "lcd.h"
#include "hzk.h"
#include "menu.h"
#include "picture.h"
#include "key.h"
#include "rtc.h"
#include "ds2438.h"
#include "Print.h"
#include "findBaseLine.h"
#include "FPGA_Contorl.h"
#include "Output.h"
#include "spi.h"
#include "Modbus.h"
#include "Measure.h"
#include "config.h"
#include "w5100_drive.h"
#include "tcpip.h"
#include "w5100.h"
#include "keyshow.h"
#include "store.h"
#include "DTU.h"
#include "SHELL.h"

extern OS_EVENT *OSLcdUsedMutex;	//�����ź���
extern OS_EVENT *OSI2CUsedMutex;
extern OS_EVENT *OSFlashUsedMutex;

#endif