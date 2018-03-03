#ifndef oup_h___
#define oup_h___

#include "config.h"
#include "global.h"

#define  ADD_I_V_CAIL 		400000
#define  ADD_I_V_CHANNEL 	400001
#define  ADD_I_V_CAILSTY 	400002
#define  ADD_I_V_CAILDAT1 	400003
#define  ADD_I_V_CAILDAT2 	400004
#define  ADD_I_V_TRUEDAT1 	400005
#define  ADD_I_V_TRUEDAT2 	400006

#define  EVENTERRO 			0
#define  EVENTRUNPARM 		1
#define  EVENTMEASURE 		2
#define  EVENTCALIBRATION 	3
#define  EVENTMAINTENANCE 	4

#define  YXMS 				0
#define  SDMF 				1
#define  ZDJRWD 			2
#define  CYLL 				3

#define  CYZQ 				4
#define  ZDMS 				5
#define  DHSSN 				6

#define SYS_NUM 			1228
#define  BPJZ 				0
#define  T1__ 				1
#define  T3__ 				2
#define  RH__ 				3

#define  P1__ 				4
#define  JIAOZHUNMO 		5
#define  F1__ 				6
#define  MRJZ 				7
#define  T4__ 				8
#define  RH2__ 				9

typedef struct
{
    float CONC;
    float paper;
    float RH;
    float temperature1;
    float temperature2;
    float temperature3;
    float flow1;
    float flow2;
}
OUTPUTSTRUCT;

typedef struct
{
    TimeDat recordTime;
    uint8 Event;
    uint8 ContentH;
    uint8 ContentL;
}
SysRecord;

extern uint8 		SysrecordReadFlg;

extern OUTPUTSTRUCT outputstruct1;
extern void 		OutputCONC(OUTPUTSTRUCT dat);
extern void 		testboard(uint8 action);
extern void 		DigitalOutput(void);
extern void 		ReadHistory(void);
extern uint16 		inputDat[16];
extern uint16 		outputDat[8];
extern void 		ReadHistoryM(void);
extern void 		ReadHistorySys(void);
extern SysRecord 	goSysRecor;
extern void 		FlashSysDatChange(uint8 *dat,SysRecord Record);
extern void  		ReadSysRecord(SysRecord *record ,uint32 recordP);

extern void 		ReadHistoryDat(void);
extern void 		ReadHistorySysMin(void);


#endif


