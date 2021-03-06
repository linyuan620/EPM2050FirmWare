#ifndef __FPGCONTOR_H
#define __FPGCONTOR_H

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
#define SHISHI 0
#define ZHOUQI 1

//#define NEW_YAZHI   	//新旧压纸宏

#define START 0XEF
#define STOP 0XEF
#define ESC 0XEC
#define FPGA_RESET 0XE9
#define ACK 0XE8
#define ENDFRAME 0XE7
#define ADCCTRLS 0XDF
#define ADCCTRLM 0XDE
#define ADCCTRLC 0XDD
#define DACCTRLS 0XDC
#define DACCTRLM 0XDB
#define DACCTRLC 0XDA
#define ADDACTRL 0XD9
#define TTLCTRLI 0XD8
#define TTLCTRLO 0XD7
#define SPICTRLI 0XD6
#define SPICTRLO 0XD5
#define ADBUFFCTRL 0XD3
#define ADBUFFREAD 0XD2
#define ADBUFFRLS  0XD1
#define USERBUFF   0XD0
#define USERWRITE  0XCF
#define USERREAD   0XCE
#define USEINTEST  0XCD
#define ADDASELFTEST  0XCC
#define ADRANGESEL  0XCB
#define ROTARY  0XC8
#define MOCTRL  0XC9
#define BUSSEL  0XC4
#define CNT  0XC3

#define COUNT_START 0x53
#define COUNT_END   0x45
#define COUNT_READ   0xc2

#define ERR_OK 0X00
#define ERR1	0X01
#define ERR2	0X02
#define ERR3	0X03
#define ERR4	0X04
#define ERR5	0X05
#define ERR6	0X06
#define ERR7	0X07
#define ERR8	0X08
#define ERR9	0X09
#define ERR10	0X0A
#define ERR11	0X0B
#define ERR12	0X0C
#define ERR13	0X10

#define USB_STYLE	1
#define UART_STYLE	2

#define DIR_RIGHT	0X00
#define DIR_LEFT	0X80

#define  FANDA2_5	 2.5
#define  FANDA1_22	 1.22
#define  FANDA2	  2.0
#define DACONVERT  0x3e80  // 32768/2.048
#define FLOWRANGE  4.84
#define FLOWSTART  1.4
    extern uint32 CountNow,CountAver,CountTotal ,CountTimes;
    extern uint8 counterr,RestErr;
    extern uint8 C14MotMoveTime ;
    extern void FPGA_Contorl(void);
    extern void Rest(void);
    extern void ACKpro(uint8 errorcode);
    extern void Rotary(uint32 Distance);
    extern uint16  EEORStuationOld;
    extern void ENDpro(void);
    extern void CommunicationSet(uint8 style);

//该函数用于控制步进电机，我们用来控制直流电机 后面的参数无关紧要。
    extern void MotorCtrl(uint8 MOIndex,uint32 Distance);

    extern void ADMCSSampling(uint16 ChanMap,uint16 StartTime1,uint8 AvgPoint1);

    extern void ADMCMSampling(uint16 ChanMap,uint16 StartTime1,uint16 interval ,uint16 NumOfSamples);

    extern volatile uint8 ACKFlg,CUpdateFlg;

    extern void  DAMCSOutPutAll(void) ;
    extern void  DAMCSOutPutSetOne(uint8 DACChan,float DACValue);

    extern void DASCMOutput(uint8 DAChanIndex,uint16 StartValue,uint16 Interval,uint16 Step,uint16 StopValue);
    extern void DASCCOutput(uint8 DAChanIndex,uint16 StartValue,uint16 Interval,uint16 Step,uint16 StopValue);
    extern void TTLInput (uint8 ChanIndex)	  ;
    extern void TTLOutput (uint8 ChanIndex,uint8 action,uint8 Tongdao);
    extern void  yajingMoto(void);
    extern void  songkaiMoto(void);

    extern void SPIIput(uint8 ChanIndex,uint8 BufferIndex);
    extern void SPIoutput(uint8 ChanIndex,uint8 BufferIndex,uint8 FreqFlag,uint16 SizeFlag);
    extern void ADBufferStatus(uint16 ChanMap);
    extern void ADBufferRead(uint16 ChanMap,uint8 RdSel1,uint8 RdSel2);
    void getShishiLiuliang(void);
    extern void ADBufferRLS(uint8 RLScode);
    extern void InquiryUserBufferState(uint8 BuffIndex);
    extern void ReadUserBuffer(uint8 BuffIndex , uint16 DatLength);
    extern void WriteUserBuffer (uint8 ChanIndex,uint16 DatLength);
    extern uint8 CheackDat(volatile uint8 *dat);
    extern void  ADRangeSel(uint8 Rangecode);
    extern void  SelfCheak(void);
    extern void  RunPaperStep(void);
    extern int 	 MeasureStep(void);
    extern void  FujiStep(void);
    extern void  NomalMeasure(void);
    extern uint8 StopFlg,StartKeyFlg,TimeType,MeasureStepFlg;
    extern volatile uint8 ADBufferStart1,ADBufferStart2,ReadBegain,ReadBuffer,CheakDat;
    extern uint32 PaperTime;

    extern volatile uint16	 ReadNum;
    extern uint8 volatile ADBufferDatReadFlg, ADBufferDatReadFlg2;
    extern uint8 volatile TTLReciveFlg;
    extern float LoadK(uint8 SensorName,float k);
    extern volatile float Parg_I,Parg_x,Parg_m,Parg_v_BK,Parg_I0,Parg_I0_Upload,Parg_I_ReCnt,Parg_I0_ReCnt,
           Parg_PM25,Parg_u/*校准值*/,Parg_PM25_GK,Parg_u1/*第一次校准值*/,
           Parg_u2/*第二次校准值*/,Parg_u3/*第三次校准值*/;
    extern CNT_RECORD Cnt_Record[200];
    extern volatile float PM25_GS_M , PM10_GS_M ,TSP_GS_M;
    extern volatile float PM25_GS_H,PM10_GS_H,TSP_GS_H;
    extern volatile uint16 GS_Cnt;

    extern uint8_t Sent_Buffer[50];
    extern int16  ADDat[16];
    extern int16  ADDat_1[8];
    extern int16  ADDat_2[8];
    extern int16  ADDat2;
    extern float addat[16];

    extern float Vcc_5,Parg_k0, Parg_k;
    extern float TSP_GS_k,PM10_GS_k,PM25_GS_k;
    extern fp32  GS_Cycle;
    extern float MeasureTs,MeasureTe;
    extern float ErrorDat;
    extern char  Parg_b,Parg_b0;
    extern uint8 CmErrorState[16];
    extern void  CountCtrl(uint8 action);
    extern void  CountRead(void);
    extern float TV_GK,TV_BK;
    extern int8  errtimemax;
    extern int8  errtimemax0;
    extern int8  UseFor ;
    extern int8  UseFor0 ;
    extern int   CountStartFlg;
    extern uint32 ComminiStyle0 ,ComminiStyle;

#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif

/********************************************************************************************************
  End Of File
********************************************************************************************************/

