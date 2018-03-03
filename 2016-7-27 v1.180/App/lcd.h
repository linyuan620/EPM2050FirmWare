/*********************************************************************************************************
Copyright (C), 2011-8-3 ,  Skyray Instrment Co.,LTD.
File name:      	LCD.h
Author:             ������       Version:       V2.5 Date:      2011-8-3 11:37:43
Description:

Others:
Function List:
		extern void SetVLCD(uint8 dat);	//�Աȶ�
		extern void LcdClr(void);		   //����
		extern void ClrDisBuf(void);			 //���buf
		//��ʾ ������СͼƬ
		extern void pic56x56(uint8 x,uint8 y,const uint8 *p);
		extern void pic40x16(uint8 x,uint8 y,const uint8 *p);
		extern void Write240x160(const uint8 *p);

		extern uint8 LcdUpdata(uint8 dat[20][240]);	//������ʾ
		extern uint8 LcdReadByte(uint8 x,uint8 y);	   //��д
		extern void LcdWriteByte(uint8 x,uint8 y,uint8 data);
		//��ʾ��ͬ������ַ���
		extern void DisBuf168(uint8 x,uint8 y,uint8 const p[]);
		extern void DisBuf126(uint8 x,uint8 y,uint8 const p[]);
		extern void DisBuf1212(uint8 x,uint8 y,uint8 const p[]);
		extern void DisBuf1616(uint8 x,uint8 y,uint8 const p[]);
		extern void DisBuf2412(uint8 x,uint8 y,uint8 const p[]);
		extern void DisCList12(uint8 x,uint8 y,uint8 const dat[21]);
		extern void DisCList16(uint8 x,uint8 y,uint8 dat[21]);
		extern void DisEList126(uint8 x,uint8 y,uint8 const dat[21]);
		extern void DisEList2412(uint8 x,uint8 y,uint8 const dat[36]);
		//������ʾ����
		extern void ReverseVideo(uint8 area[]);
		extern void CReverseVideo(uint8 const area[]);
		extern void CleanVideo(uint8 area[]);
		extern void DarwButton(uint8 x,uint8 y);
		extern void DarwScrollBar(uint8 tdat,uint8 ndat);
		extern void DrawRectangular(uint8 area[]);
		extern void DarwHLine(uint8 x1,uint8 x2,uint8 y);
		extern void DarwVLine(uint8 x1,uint8 x2,uint8 y);
		extern void DisEList168(uint8 x,uint8 y,uint8 dat[21]);
		extern void DisStr168(uint8 x,uint8 y,char *dat);
		extern void DisCStr16(uint8 x,uint8 y,uint8 *dat);
		//Ӧ�ú��� ��ʾʱ�� ���� ��ֵ
		extern void DisTime(uint16 x,uint8 y,TimeDat dat,uint8 Font);
		extern void DisDate(uint16 x,uint8 y,TimeDat dat,uint8 Font);
		extern void DisValue(fp32 fValuel,uint16 x,uint8 y,uint8 font,uint8 length,uint8 decimal,uint8 DisSign);
		extern int8 GetNumLen(fp32 fValuel);
		extern uint32 Num10n(uint8 n);

History:
   1. Date:                2011-8-3 12:52:43
       Author:             ��˳��
	   Modification:    �淶��ע��
************************************************************************************************************/
#ifndef __LCD_H
#define __LCD_H

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
    **                    240*160 LCD�Ŀ��ƶ˿�
    *********************************************************************************************************/

#define 	DATA_BUS	(0XFFul<<24)			//P1.24--P1.31

#define		CS			(1ul<<19)			//P1.19
#define		CD			(1ul<<20)			//P1.20;
#define		RD			(1ul<<21)			//P1.21;
#define 	WR			(1ul<<22)			//p1.22;
#define 	RST			(1ul<<23)			//p1.23;

#define		DATA_OUT		GPIO_SetDir(1, DATA_BUS,1)
#define		DATA_IN			GPIO_SetDir(1, DATA_BUS,0)
#define		DATA_PIN		GPIO_ReadValue(1, DATA_BUS)
#define		MASK_ON  		FIO_SetMask(1, DATA_BUS,1)
#define		MASK_OFF  		FIO_SetMask(1, DATA_BUS,0)



#define 	CMD_OUT 		GPIO_SetDir(1, (CD|RD|WR|RST|CS),1)		//���ƿ�ȫΪ���
#define  	CD_H 			GPIO_SetValue(1, CD)			//P1.17--CD �øߡ�
#define  	CD_L 			GPIO_ClearValue(1, CD)			//P1.17--CD �õ�

#define  	WR_H 			GPIO_SetValue(1, WR)			//P1.15--WR �øߡ�
#define  	WR_L 			GPIO_ClearValue(1, WR)			//P1.15--WR ��L��

#define  	CS_H 			GPIO_SetValue(1, CS)			//P1.10--CS �øߡ�
#define  	CS_L 			GPIO_ClearValue(1, CS)			//P1.10--CS ��L��

#define  	RD_H 			GPIO_SetValue(1, RD)			//P1.16--RD �øߡ�  
#define  	RD_L 			GPIO_ClearValue(1, RD)			//P1.16--RD ��L��

#define  	RES_H 			GPIO_SetValue(1, RST)			//P1.14--RES �øߡ�  
#define  	RES_L 			GPIO_ClearValue(1, RST)			//P1.14--RES ��L��
//typedef struct
//{
//	uint8 font;
//	uint8 length;
//	uint8 decimal;
//	uint8 DisSign;
//}
//DISDATSTR;
//
//DISDATSTR  DisDat1;
    extern uint8 volatile DatLength;

    extern uint8  DisBuf[21][240];
    /*********************************************************************************************************
    ** ��������: LCDIOIint
    ** ��������: lcd���ų�ʼ��
    ** ��ڲ���: ��
    ** �� �� ֵ: ��
    ** ˵    ��:
    *********************************************************************************************************/
    extern void LCDIOIint(void);

    /*********************************************************************************************************
    ** Function name:       lcd_write
    ** Descriptions:        lcdд���� ������
    ** input parameters:    cmd 0 ���� 1 ����
    						DATA  ���ݻ�����
    ** output parameters:
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void LCDWrite(uint8 cmd,uint8 data);

    /*********************************************************************************************************
    ** Function name:       Init1698
    ** Descriptions:        ��ʼlcd
    ** input parameters:
    ** output parameters:
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void InitLCD1698(void);

    /*********************************************************************************************************
    * Function name:        LcdClr
    * Descriptions:         LCD����
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void LcdClr(void);

    /*********************************************************************************************************
    * Function name:  SetVLCD
    * Descriptions:   �Աȶ�����
    * input parameters: dat
    * output parameters:
    * Returned value:
    *********************************************************************************************************/
    extern void SetVLCD(uint8 dat);

    /*********************************************************************************************************
    * Function name:  ClrDisBuf
    * Descriptions:   ��ʾbuf����
    * input parameters:
    * output parameters:
    * Returned value:
    *********************************************************************************************************/
    extern void ClrDisBuf(void);

    /*********************************************************************************************************
    ** Function name:      words56x56
    ** Descriptions:        56*56��ʾ
    ** input parameters:    x,y ��ַ
    ** output parameters:   *p��ģָ��
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void pic56x56(uint8 x,uint8 y,const uint8 *p);

    /*********************************************************************************************************
    ** Function name:      pic40x16
    ** Descriptions:        40*16��ʾ
    ** input parameters:    x,y ��ַ
    ** output parameters:   *p��ģָ��
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void pic40x16(uint8 x,uint8 y,const uint8 *p);

    /*********************************************************************************************************
    * Function name:LcdUpdata
    * Descriptions:   ������ʾ ��buf��ֵ��ʾ����
    * input parameters:
    * output parameters:
    * Returned value:
    *********************************************************************************************************/
    extern uint8 LcdUpdata(uint8 dat[20][240]);

    /*********************************************************************************************************
    * Function name:  LcdReadByte
    * Descriptions:   ����1byte��
    * input parameters:
    * output parameters:
    * Returned value:
    *********************************************************************************************************/
    extern uint8 LcdReadByte(uint8 x,uint8 y);

    /*********************************************************************************************************
    * Function name:   LcdWriteByte
    * Descriptions:    д��1byte��
    * input parameters:
    * output parameters:
    * Returned value:
    *********************************************************************************************************/
    extern void LcdWriteByte(uint8 x,uint8 y,uint8 data);

    /*********************************************************************************************************
    ** Function name:      DisBuf168
    ** Descriptions:        16 8��ʾ
    ** input parameters:    x,y ��ַ
    ** output parameters:   *p��ģָ��
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void DisBuf168(uint8 x,uint8 y,uint8 const p[]);

    /*********************************************************************************************************
    ** Function name:      DisBuf126
    ** Descriptions:        12*6��ʾ
    ** input parameters:    x,y ��ַ
    ** output parameters:   *p��ģָ��
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void DisBuf126(uint8 x,uint8 y,uint8 const p[]);

    /*********************************************************************************************************
    ** Function name:      	DisBuf1212
    ** Descriptions:        12*12��ʾ
    ** input parameters:    x,y ��ַ
    ** output parameters:   *p��ģָ��
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void DisBuf1212(uint8 x,uint8 y,uint8 const p[]);

    /*********************************************************************************************************
    ** Function name:     	DisBuf1616
    ** Descriptions:        16 16��ʾ
    ** input parameters:    x,y ��ַ
    ** output parameters:   *p��ģָ��
    ** Returned value:      ��
    *********************************************************************************************************/
//extern void DisBuf1616(uint8 x,uint8 y,uint8  p[]);

    /*********************************************************************************************************
    ** Function name:      DisBuf2412
    ** Descriptions:        2412��ʾ
    ** input parameters:    x,y ��ַ
    ** output parameters:   *p��ģָ��
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void DisBuf2412(uint8 x,uint8 y,uint8 const p[]);

    /*********************************************************************************************************
    ** Function name:      	DisCList12
    ** Descriptions:        ��ʾһ���ַ�
    ** input parameters:    x,y ��ַ
    ** output parameters:   dat�ַ���
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void DisCList12(uint8 x,uint8 y,uint8 const dat[21]);

    /*********************************************************************************************************
    ** Function name:      	DisCList16
    ** Descriptions:        ��ʾһ���ַ�
    ** input parameters:    x,y ��ַ
    ** output parameters:   dat�ַ���
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void DisCList16(uint8 x,uint8 y,uint8 const dat[21]);

    /*********************************************************************************************************
    ** Function name:       DisEList126
    ** Descriptions:        ��ʾ һ���ַ���  12 6
    ** input parameters:    x,y ��ַ
    ** output parameters:   ��ģָ��
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void DisEList126(uint8 x,uint8 y,uint8 const dat[21]);

    /*********************************************************************************************************
    ** Function name:        DisEList2412
    ** Descriptions:        ��ʾ һ���ַ���
    ** input parameters:    x,y ��ַ
    ** output parameters:   ��ģָ��
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void DisEList2412(uint8 x,uint8 y,uint8 const dat[36]);

    /*********************************************************************************************************
    ** Function name:       ReverseVideo
    ** Descriptions:        �Ի��򷴰�
    ** input parameters:    �Ի����ַ
    ** output parameters:
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void ReverseVideo(uint8 area[]);

    /*********************************************************************************************************
    ** Function name:    CReverseVideo
    ** Descriptions:        ��ʾ�˵�����
    ** input parameters:     �����ַ���
    ** output parameters:
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void CReverseVideo(uint8 const area[]);

    /*********************************************************************************************************
    ** Function name:    CleanVideo
    ** Descriptions:        ��������
    ** input parameters:    �����ĵ�ַ��Χ����
    ** output parameters:
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void CleanVideo(uint8 area[]);

    /*********************************************************************************************************
    ** Function name:       DarwButton
    ** Descriptions:        ��ʾ Ŧ�� ͼ��
    ** input parameters:    x,y ��ַ
    ** output parameters:
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void DarwButton(uint8 x,uint8 y);

    /*********************************************************************************************************
    ** Function name:    DarwScrollBar
    ** Descriptions:        ��ʾ ������
    ** input parameters:    tdat ��ǰ��	  ndat ����
    ** output parameters:
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void DarwScrollBar(uint16 tdat,uint16 ndat);

    /*********************************************************************************************************
    * Function name:   DrawRectangular
    * Descriptions:    ��һ�Ի����
    * input parameters:
    * output parameters:
    * Returned value:
    *********************************************************************************************************/
    extern void DrawRectangular(uint8 const area[]);

    /*********************************************************************************************************
    * Function name: DarwHLine
    * Descriptions:  ��һ������
    * input parameters: x1,x2,y  y���꣬x��ʼ����ֹ
    * output parameters:
    * Returned value:
    *********************************************************************************************************/
    extern void DarwHLine(uint8 x1,uint8 x2,uint8 y);

    /*********************************************************************************************************
    * Function name: DarwVLine
    * Descriptions:  ��һ������
    * input parameters:  x,y11,y2  x���꣬y��ʼ����ֹ
    * output parameters:
    * Returned value:
    *********************************************************************************************************/
    extern void DarwVLine(uint8 x1,uint8 x2,uint8 y);

    /*********************************************************************************************************
    ** Function name:       DisEList168
    ** Descriptions:        ��ʾ һ���ַ���  168
    ** input parameters:    x,y ��ַ
    ** output parameters:   ��ģָ��
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void DisEList168(uint8 x,uint8 y,uint8 dat[21]);

    /*********************************************************************************************************
    ** Function name:       DisStr168
    ** Descriptions:        ��ʾһ��12*8����
    						dat��Ԫ������Ϊ����ʾ������hzk1212k1��ƫ����
    ** input parameters:    x,y ��ַ
    ** output parameters:   ��ģָ��
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void DisStr168(uint8 x,uint8 y,char *dat);

    /*********************************************************************************************************
    ** Function name:       DisCStr16
    ** Descriptions:        ��ʾ һ���ַ���
    ** input parameters:    x,y ��ַ
    ** output parameters:   ��ģָ��
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void DisCStr16(uint8 x,uint8 y,const uint8 *dat);


    /*********************************************************************************************************
    ** Function name: DisTime
    ** Descriptions:        ��ʾ ����ʱ
    ** input parameters:    x,y ��ַ font��ʾ��С
    ** output parameters:
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void DisTime(uint16 x,uint8 y,TimeDat dat,uint8 Font);


    /*********************************************************************************************************
    ** Function name:       DisDate
    ** Descriptions:        ��ʾ ����
    ** input parameters:    x,y ��ַ font ��ʽ
    ** output parameters:
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void DisDate(uint16 x,uint8 y,TimeDat dat,uint8 Font);

    /*********************************************************************************************************
    * Function name: DisValue
    * Descriptions:  ��ʾ���
    * input parameters:  fp32 fValuel, ֵ
    					uint16 x,uint8 y, ��ʾ��ַ
    					uint8 font,		  ��ʾ��С��
    					uint8 length,	  ��ʾλ��
    					uint8 decimal,	  С�����ĸ���
    					uint8 DisSign     ���������֣�
    * output parameters:
    * Returned value:
    *********************************************************************************************************/

    extern void DisValue(fp32 fValuel,uint16 x,uint8 y,uint8 font,uint8 length,uint8 decimal,uint8 DisSign);

    /*********************************************************************************************************
    ** Function name:      write_row_strip
    ** Descriptions:       ��һ��ͼ	 320*240
    ** input parameters:    x:x��ַ
    						y:y��ַ
    ** output parameters:
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void Write240x160(const uint8 *p);

    /*********************************************************************************************************
    * Function name:   GetNumLen
    * Descriptions:     ȡ�����ݳ���
    * input parameters:  fValuel ����
    * output parameters:
    * Returned value:
    *********************************************************************************************************/
    extern int8 GetNumLen(fp32 fValuel);

    /*********************************************************************************************************
    * Function name:  Num10n
    * Descriptions://10��n�η�
    * input parameters:
    * output parameters:
    * Returned value:
    *********************************************************************************************************/
    extern uint32 Num10n(uint8 n);

    /*********************************************************************************************************
    ** Function name:       DisConfigState
    ** Descriptions:       ����״̬������ʾ
    ** input parameters:    OnOff ������״̬ 0�� 1 ��  �ź�״̬ʱ����
    						dat ����ֵ  ����״̬ʱ����
    						CurrId ��ǰmeuID
    						showplace���ڵ�һҳʱӦ�õ���ʾλ��
    						type ״̬���� 0�������� 1 ��������
    ** output parameters:
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void DisConfigState1(uint8 OnOff,fp32 dat,uint16 lan_offset,uint8 showplace,uint8 type,uint8 *danwei);
    /*********************************************************************************************************
    ** Function name:       DisConfigState
    ** Descriptions:       ����״̬��ʾ
    ** input parameters:
    ** output parameters:
    ** Returned value:      ��
    *********************************************************************************************************/

    extern void DisConfigState(void);

    /*********************************************************************************************************
    ** Function name:      Disppb_cn1212
    ** Descriptions:       ��ʾ���ֵ�λ ΢��ÿ��
    ** input parameters:
    ** output parameters:
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void Disppb_cn1212(uint8 x,uint8 y);

    /*********************************************************************************************************
    ** Function name:      Disppb_En1616
    ** Descriptions:       ��ʾӢ�ĵ�λ ug/l
    ** input parameters:
    ** output parameters:
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void Disppb_En1616(uint8 x,uint8 y);
    /*********************************************************************************************************
    ** Function name:      Disppb_cn1212
    ** Descriptions:       ��ʾӢ�ĵ�λ ug/l
    ** input parameters:
    ** output parameters:
    ** Returned value:      ��
    *********************************************************************************************************/
    extern void Disppb_En1212(uint8 x,uint8 y);
    extern void WriteCode1212(uint8 x,uint8 y,uint8 const dat[40]);
    extern void Disppb_En2412(uint8 x,uint8 y);
    extern void DisUg_M3_En2412(uint8 x,uint8 y);
    extern void ShowCoord(void);
    extern uint8 	RunStyleFlg	,HeatOn,TempCompensateFlag,PumpFlg ,HeatFlg,	ClampMotorFlg,ClampMotor,ClampRadio, RunMotor,  MeasureFlg;
    extern fp32 Humidity,HumiditySample	,CycleOfSampSet,MaxHeatTemp,MinHeatPower,FlowOfSampOut,FlowOfSampOut2,FlowOfSamp_B,FlowOfSamp_B_Set,FlowOfSamp ,
           HighV ,BiaoPianTest,CycleOfSamp,BiaoPian,WenDuT3,WenDuT1,WenDuT2,ShiDu,
           QiYa,LiuLiang_GK,LiuLiang_BK, Pump,Mass,PowerV1,PowerV2,PowerV3,PowerV4,TCQWenDuT4/* ̽�����¶� */,CYCShiDu,TestTimes/* ��Ĥ�ز���� */;
    extern uint8 keyEnterFlg;
    extern void WriteMyWord1212(uint8 x,uint8 y,uint8 const dat[40]) ;
    extern int8 OffsetFlg	;
    extern  float MoniTongDao ,ShuzhiTongDao;
    extern  fp32  MoniZhi,	 ShuzhiZhi;

#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif

/********************************************************************************************************
  End Of File
********************************************************************************************************/
