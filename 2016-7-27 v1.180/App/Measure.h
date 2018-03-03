#ifndef __Meas__h
#define __Meas__h
#include "global.h"

typedef struct
{
    uint8 Channel;
    float Gain;
}
ADStruct;

typedef struct
{
    uint8 DAChannel;
    float DAGain;
} DAStruct;

typedef struct
{
    uint8 SensorName;
    float Modulus[2];
    float CaliDat;
    float OriginalDat;
} SensorStruct;

extern volatile int32 CaliFlg;
extern void SetCaliDat(uint8 SensorName,float CaliDat);
extern float FindOriginalDat(uint8 SensorName);

extern DAStruct DAStructTable[NUN_DA];
extern ADStruct ADStructTable[NUN_AD];
extern float GetMeasureValue(uint8 SensorName,uint8 ADChannel);
extern void	Calibration(void);
extern void	Calibration2(void);
extern float GetADValue(uint8 ADChannel);

#endif











