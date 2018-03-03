/*********************************************************************************************************
Copyright (C), 2011-8-3 ,  Skyray Instrment Co.,LTD.
File name:      	Print.h
Author:              ��˳��       Version:       V2.5 Date:      2011-8-3 11:37:43
Description:
Others:
Function List:
		extern void PrintValue(fp32 fValuel,uint8 length,uint8 decimal);
		extern void PrintRecord_page(uint32 num) ;
		extern void PrintRecord_one(uint32 num)
		extern void Research();
History:
   1. Date:                2011-8-3 12:52:43
       Author:             ��˳��
	   Modification:    �淶��ע��
************************************************************************************************************/
#ifndef __Print_H
#define __print_H

#include "global.h"

extern unsigned char hanzi_com[];
extern unsigned char ascii_com[];

extern unsigned char heshunjie[];
extern unsigned char zifuji[];
extern unsigned char PrintEn;
extern void Research(void);
/*********************************************************************************************************
* Function name: void PrintValue(void)
* Descriptions:  ��ӡŨ��
* input parameters:  fvaluel Ũ��ֵ
						length
						decimal ʮ����λ��
* output parameters:
* Returned value:
*********************************************************************************************************/
extern void PrintValue(fp32 fValuel,uint8 length,uint8 decimal);
/*********************************************************************************************************
* Function name:       PrintRecord
* Descriptions:        ��ӡ��ǰ��¼ �����ϵ����� ���10��
* input parameters:    num ����ӡ�� �����
						tnum ��ӡ���ݵ�����
* output parameters:
* Returned value:       ��
*********************************************************************************************************/
extern void PrintRecord_page(uint32 num) ;

/*********************************************************************************************************
* Function name:       PrintRecord_one
* Descriptions:        ��ӡ��ǰ ѡ�е� ��¼ ����
* input parameters:    num ����ӡ�� �����
						tnum ��ӡ���ݵ�����
* output parameters:
* Returned value:       ��
*********************************************************************************************************/
extern void PrintRecord_one(uint32 num);  /*��λ�� д����*/

/*********************************************************************************************************
* Function name:   Research
* Descriptions:    �����������
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
//extern void Research();
#endif