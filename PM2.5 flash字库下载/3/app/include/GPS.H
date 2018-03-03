#ifndef GPS_h_
#define GPS_h_

#include"Globle.h"

typedef  struct 
{
	uint8 valid;
	uint32 time;
	uint32 latitude_1;
	uint32 latitude_2;
	uint8 N_S ;
	uint32 longitude_1;
	uint32 longitude_2;
	uint8 E_W;
	float speed;
	float heading;
	uint32 data;
}GPRMC;
extern volatile uint8 	 ucGpsCommandStatus;
extern void  GPS_ProcessNMEA(void);
extern int32 unInBufCntComm1;
extern int8	ucGPSDataBuff[82];//GPSÊý¾Ý»º´æ

#endif
