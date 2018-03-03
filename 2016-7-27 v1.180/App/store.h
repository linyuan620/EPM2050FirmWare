#ifndef STORE__H
#define STORE__H


#include "global.h"

extern void WriteRecordFlash(uint32 recordP,uint8 record[]);
extern  void WriteRecord(uint32 recordP,MEAS_RECORD *record);
extern void ReadRecord(uint32 recordP,MEAS_RECORD *record);
extern void ReadFlashRecordSimple(uint32 recordP,MEAS_RECORD *record);
extern void ReadRecordSimple(uint32 recordP,MEAS_RECORD *record);
extern void ReadRecordDetail(uint32 recordP,MEAS_RECORD *record);

#endif

