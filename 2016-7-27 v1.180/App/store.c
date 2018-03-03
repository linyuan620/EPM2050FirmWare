

#include "store.h"



void WriteRecordFlash(uint32 recordP,uint8 record[])
{
    uint8 err;
    recordP = (recordP*64)%0x44000;// 4352组数据  68个扇区  0x44000个字节。半年多的数据。
    OSMutexPend(OSFlashUsedMutex,0,&err);

    if( (((recordP)%0x44000)/SEC_SIZE) !=  (((recordP+64)%0x44000)/SEC_SIZE)  )//下一组将要占用新的一个扇区。
    {
        //擦除下一扇区
        SSTF016B_Erase( (((recordP+64)%0x44000+RECORDFLASHSTAR)/SEC_SIZE), (((recordP+64)%0x44000+RECORDFLASHSTAR)/SEC_SIZE));
    }
    recordP = recordP+RECORDFLASHSTAR;
    SSTF016B_WR( recordP, record, 50);
    OSMutexPost(OSFlashUsedMutex);
}

void WriteRecord(uint32 recordP,MEAS_RECORD *record)
{


    recordP = (recordP*50)%0x1c200;

    recordP = recordP+RECORDSTAR;


    if(recordP<RECORDSTAR)
    {
        recordP = RECORDSTAR;
    }
    WriteData16(recordP,record->recordTimeStart.year);
    C1025_WriteByte(recordP+2,record->recordTimeStart.month);
    C1025_WriteByte(recordP+3,record->recordTimeStart.date);
    C1025_WriteByte(recordP+4,record->recordTimeStart.hour);
    C1025_WriteByte(recordP+5,record->recordTimeStart.minute);
    C1025_WriteByte(recordP+6,record->recordTimeStart.second);

    WriteData16(recordP+7,record->recordTimeEnd.year);
    C1025_WriteByte(recordP+9,record->recordTimeEnd.month);
    C1025_WriteByte(recordP+10,record->recordTimeEnd.date);
    C1025_WriteByte(recordP+11,record->recordTimeEnd.hour);
    C1025_WriteByte(recordP+12,record->recordTimeEnd.minute);
    C1025_WriteByte(recordP+13,record->recordTimeEnd.second);


    WriteFp32(recordP+14,record->recordResult);
    WriteFp32(recordP+18,record->I);
    WriteFp32(recordP+22,record->I0);
    WriteFp32(recordP+26,record->TV);

    WriteFp32(recordP+30,record->RH);
    WriteFp32(recordP+34,record->T1);
    WriteFp32(recordP+38,record->P);
    WriteFp32(recordP+42,record->recordResultG);
    WriteFp32(recordP+46,record->MAS);


}





void ReadRecord(uint32 recordP,MEAS_RECORD *record)
{
    recordP = (recordP*50)%0x1c200;

    recordP = recordP+RECORDSTAR;
    if(recordP<RECORDSTAR)
    {
        recordP = RECORDSTAR;
    }

    record->recordTimeStart.year=ReadData16(recordP);
    record->recordTimeStart.month=C1025_ReadByte(recordP+2);
    record->recordTimeStart.date=C1025_ReadByte(recordP+3);
    record->recordTimeStart.hour=C1025_ReadByte(recordP+4);
    record->recordTimeStart.minute=C1025_ReadByte(recordP+5);
    record->recordTimeStart.second=C1025_ReadByte(recordP+6);

    record->recordTimeEnd.year=ReadData16(recordP+7);
    record->recordTimeEnd.month=C1025_ReadByte(recordP+9);
    record->recordTimeEnd.date=C1025_ReadByte(recordP+10);
    record->recordTimeEnd.hour=C1025_ReadByte(recordP+11);
    record->recordTimeEnd.minute=C1025_ReadByte(recordP+12);
    record->recordTimeEnd.second=C1025_ReadByte(recordP+13);
    record->recordResult= ReadFp32(recordP+14);
//	record->I= ReadFp32(recordP+18);
//	record->I0= ReadFp32(recordP+22);
}
void ReadFlashRecordSimple(uint32 recordP,MEAS_RECORD *record)
{
    uint8 *dat,err;
    dat = &DatForRecord[0];
    recordP = (recordP*64)%0x44000;// 4352组数据  68个扇区  0x44000个字节。半年多的数据。
    recordP = recordP+RECORDFLASHSTAR;
    OSMutexPend(OSFlashUsedMutex,0,&err);
    SSTF016B_RD( recordP,50,dat);
    OSMutexPost(OSFlashUsedMutex);


    record->recordTimeStart.year=(dat[0])|(dat[1]<<8);
    record->recordTimeStart.month=dat[2];
    record->recordTimeStart.date=dat[3];
    record->recordTimeStart.hour=dat[4];
    record->recordTimeStart.minute=dat[5];
    record->recordTimeStart.second=dat[6];

    record->recordTimeEnd.year=(dat[7])|(dat[8]<<8);
    record->recordTimeEnd.month=dat[9];
    record->recordTimeEnd.date=dat[10];
    record->recordTimeEnd.hour=dat[11];
    record->recordTimeEnd.minute=dat[12];
    record->recordTimeEnd.second=dat[13];

    record->recordResult= ChArrayToFloat(&dat[14]);
    record->I= ChArrayToFloat(&dat[18]);
    record->I0= ChArrayToFloat(&dat[22]);

    record->TV= ChArrayToFloat(&dat[26]);
    record->RH= ChArrayToFloat(&dat[30]);
    record->T1= ChArrayToFloat(&dat[34]);
    record->P= ChArrayToFloat(&dat[38]);
    record->recordResultG= ChArrayToFloat(&dat[42]);
    record->MAS= ChArrayToFloat(&dat[46]);

}



void ReadRecordSimple(uint32 recordP,MEAS_RECORD *record)
{

    recordP = (recordP*50)%0x1c200;
    recordP = recordP+RECORDSTAR;
    if(recordP<RECORDSTAR)
    {
        recordP = RECORDSTAR;
    }

// 	record->recordTimeStart.year=ReadData16(recordP);
// 	record->recordTimeStart.month=C1025_ReadByte(recordP+2);
// 	record->recordTimeStart.date=C1025_ReadByte(recordP+3);
// 	record->recordTimeStart.hour=C1025_ReadByte(recordP+4);
// 	record->recordTimeStart.minute=C1025_ReadByte(recordP+5);
// 	record->recordTimeStart.second=C1025_ReadByte(recordP+6);

    record->recordTimeEnd.year=ReadData16(recordP+7);
    record->recordTimeEnd.month=C1025_ReadByte(recordP+9);
    record->recordTimeEnd.date=C1025_ReadByte(recordP+10);
    record->recordTimeEnd.hour=C1025_ReadByte(recordP+11);
// 	record->recordTimeEnd.minute=C1025_ReadByte(recordP+12);
// 	record->recordTimeEnd.second=C1025_ReadByte(recordP+13);

    record->recordResult= ReadFp32(recordP+14);
//	record->I= ReadFp32(recordP+18);
//	record->I0= ReadFp32(recordP+22);
}

/*********************************************************************************************************
* Function name:  ShowRecord
* Descriptions:   显示记录内容
* input parameters:  uint8 num,	  显示内容的存储位置
					uint8 tnum    一页显示的多少
* output parameters:     无
* Returned value:         无
*********************************************************************************************************/


void ReadRecordDetail(uint32 recordP,MEAS_RECORD *record)
{
    recordP = (recordP*50)%0x1c200;
    recordP = recordP+RECORDSTAR;
    if(recordP<RECORDSTAR)
    {
        recordP = RECORDSTAR;
    }

    record->recordTimeStart.year=ReadData16(recordP);
    record->recordTimeStart.month=C1025_ReadByte(recordP+2);
    record->recordTimeStart.date=C1025_ReadByte(recordP+3);
    record->recordTimeStart.hour=C1025_ReadByte(recordP+4);
    record->recordTimeStart.minute=C1025_ReadByte(recordP+5);
    record->recordTimeStart.second=C1025_ReadByte(recordP+6);

    record->recordTimeEnd.year=ReadData16(recordP+7);
    record->recordTimeEnd.month=C1025_ReadByte(recordP+9);
    record->recordTimeEnd.date=C1025_ReadByte(recordP+10);
    record->recordTimeEnd.hour=C1025_ReadByte(recordP+11);
    record->recordTimeEnd.minute=C1025_ReadByte(recordP+12);
    record->recordTimeEnd.second=C1025_ReadByte(recordP+13);
    record->recordResult= ReadFp32(recordP+14);
    record->I= ReadFp32(recordP+18);
    record->I0= ReadFp32(recordP+22);

    record->TV= ReadFp32(recordP+26);
    record->RH= ReadFp32(recordP+30);
    record->T1= ReadFp32(recordP+34);
    record->P= ReadFp32(recordP+38);
    record->recordResultG= ReadFp32(recordP+42);
    record->MAS= ReadFp32(recordP+46);
}

