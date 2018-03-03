#ifndef _KEYSHOW
#define _KEYSHOW
#include "global.h"

typedef struct
{
    uint16	first;
    uint16 second;
    uint16 	third;
    uint16 	end;
} IPDat;

extern IPDat ip;
extern  uint8 IpArea[][4];

extern uint8 OldAction;
extern int16 rRecordFlg ;
extern int16 rRecordShowFlg;
extern int32 recordShowFlg;
extern MEAS_RECORD rRecord[8];
extern uint8 keyFlg;
extern int8 sign,SensorDatS[10];
extern void RangePrompt(fp32 range);
extern void DealYMD(uint8 *arr,TimeDat dat);
extern void DealIP(uint8 *arr,IPDat dat);
extern void DealHMS(uint8 *arr,TimeDat dat);
extern void ShowRecord2(uint16 tnum,uint8 action);
extern void ShowRecordDetail(uint16 RecordNum);
extern void ShowSysRecord(void);
extern uint8  SetEnterTime(void);
extern uint8  SetDownTime(void);
extern uint8  SetUpTime(void);
extern uint32 getDatLenth(uint8 wei);
extern fp32   LoadSensorDat(fp32 dat ,uint8 ParamName,uint8 action);
extern uint8  SetRightIp(void);
extern int    SetUpIp(void);
extern void   SetDownIp(void);

#endif

