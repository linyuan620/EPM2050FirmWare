#include "keyshow.h"

IPDat ip= {192 ,168,0,160};
int8 ipdat[13] = {1,9,2,1,6,8,0,0,0,1,6,0};

uint8 IpArea[][4] =
{
    {59,59,69,77},
    {67,59,77,77},
    {75,59,85,77},
    {91,59,101,77},
    {99,59,109,77},
    {107,59,117,77},

    {123,59,133,77},
    {131,59,141,77},
    {139,59,149,77},
    {155,59,165,77},
    {163,59,173,77},
    {171,59,181,77},
};

const uint8 SysPro[][10]=
{

    {"��������"},
    {"����ģʽ"},
    {"��ͣ    "},
    {"��ƬУ׼"},

    {"��ֽ����"},
    {"ʪ���ŷ�"},
    {"����    "},
    {"����  T1"},

    {"��ѹ�쳣"},
    {"����¶�"},
    {"        "},
    {"�¶�  T3"},

    {"�����쳣"},
    {"��������"},
    {"        "},
    {"ʪ�� RH1"},

    {"���ȴ�  "},
    {"��������"},
    {"        "},
    {"����ѹP1"},


    {"ʪ���쳣"},
    {"����ģʽ"},
    {"        "},
    {"У׼Ĥ  "},

    {"T1�쳣  "},
    {"DHS ʹ��"},
    {"        "},
    {"����  F1"},

    {"T2�쳣  "},
    {"        "},
    {"        "},
    {"Ĭ��У׼"},

    {"T3�쳣  "},
    {"        "},
    {"        "},
    {"�¶�  T4"},

    {"�ڴ����"},
    {"        "},
    {"        "},
    {"ʪ�� RH2"},
};


void RangePrompt(fp32 range)
{
    unsigned char are[4] = {183,0,239,17};

    if(0.0 == range)
    {
        return;
    }
    CReverseVideo(are);

    if(range<1000)
    {
        DisValue(range,183,0,2,3,0,0);
        Disppb_En1616(207,0);
    }
    else
    {
        DisValue(range/1000.0,183,0,2,3,0,0);
        DisCList16(207,0,"mg/L");
    }

    CReverseVideo(are);
}

/*********************************************************************************************************
* Function name:  DealYMD
* Descriptions:   ʱ��ֳ� ʮ���Ʒ��� ����
* input parameters:uint8 *arr,TimeDat dat   ����  ʱ��
* output parameters:
* Returned value:
*********************************************************************************************************/
void DealYMD(uint8 *arr,TimeDat dat)
{
    uint8 *p;
    uint8 i;
    uint8 cc[8] = "������";
    uint8 EE[8] = "YeMoDa";

    p = arr;
    if(language == 0)
    {
        i=(dat.year/1000);
        *p = i + 48;
        i=dat.year%1000/100;
        p++;
        *p = i + 48;
        i=(dat.year%100/10);
        p++;
        *p = i + 48;
        i=(dat.year%10);
        p++;
        *p = i + 48;

        p++;
        StrCopyStr(p,&cc[0],2);

        i=dat.month/10;
        p++;
        p++;
        *p = i + 48;
        i=dat.month%10;
        p++;
        *p = i + 48;

        p++;
        StrCopyStr(p,&cc[2],2);

        i=dat.date/10;
        p++;
        p++;
        *p = i + 48;
        i=dat.date%10;
        p++;
        *p = i + 48;

        p++;
        StrCopyStr(p,&cc[4],2);

        p++;
        p++;
        *p = 0;
    }
    else if(language ==1)
    {
        i=(dat.year/1000);
        *p = i + 48;
        i=dat.year%1000/100;
        p++;
        *p = i + 48;
        i=(dat.year%100/10);
        p++;
        *p = i + 48;
        i=(dat.year%10);
        p++;
        *p = i + 48;

        p++;
        StrCopyStr(p,&EE[0],2);

        i=dat.month/10;
        p++;
        p++;
        *p = i + 48;
        i=dat.month%10;
        p++;
        *p = i + 48;

        p++;
        StrCopyStr(p,&EE[2],2);

        i=dat.date/10;
        p++;
        p++;
        *p = i + 48;
        i=dat.date%10;
        p++;
        *p = i + 48;

        p++;
        StrCopyStr(p,&EE[4],2);

        p++;
        p++;
        *p = 0;
    }

}

void DealIP(uint8 *arr,IPDat dat)
{
    uint8 *p;
    uint8 i;
    const uint8 *cc = ".";

    p = arr;
    i=(dat.first/100);
    *p = i + 48;
    i=dat.first%100/10;
    p++;
    *p = i + 48;
    i=(dat.first%10);
    p++;
    *p = i + 48;
    p++;
    StrCopyStr(p,cc,1);

    i=dat.second/100;
    p++;
    *p = i + 48;
    i=dat.second%100/10;
    p++;
    *p = i + 48;
    i = dat.second%10;
    p++;
    *p = i +48;
    p++;
    StrCopyStr(p,cc,1);

    i=dat.third/100;
    p++;
    *p = i + 48;
    i=dat.third%100/10;
    p++;
    *p = i + 48;
    i = dat.third%10;
    p++;
    *p = i +48;
    p++;
    StrCopyStr(p,cc,1);


    i=dat.end/100;
    p++;
    *p = i + 48;
    i=dat.end%100/10;
    p++;
    *p = i + 48;
    i = dat.end%10;
    p++;
    *p = i +48;
    p++;
    *p = 0;
}

/*********************************************************************************************************
* Function name:  DealHMS
* Descriptions:   ʱ��ֳ� ʮ���Ʒ��� ����
* input parameters:uint8 *arr,TimeDat dat   ����  ʱ��
* output parameters:
* Returned value:
*********************************************************************************************************/
void DealHMS(uint8 *arr,TimeDat dat)
{
    uint8 *p;
    uint8 i;
    uint8 cc[8] = "ʱ����";
    uint8 EE[8] = "HoMiSc";

    p = arr;
    if(language == 0)
    {
        i=(dat.hour/10);
        *p = i + 48;
        i=(dat.hour%10);
        p++;
        *p = i + 48;

        p++;
        StrCopyStr(p,&cc[0],2);

        i=dat.minute/10;
        p++;
        p++;
        *p = i + 48;
        i=dat.minute%10;
        p++;
        *p = i + 48;

        p++;
        StrCopyStr(p,&cc[2],2);

        i=dat.second/10;
        p++;
        p++;
        *p = i + 48;
        i=dat.second%10;
        p++;
        *p = i + 48;

        p++;
        StrCopyStr(p,&cc[4],2);

        p++;
        p++;
        *p = 0;
    }
    else if(language == 1)
    {
        i=(dat.hour/10);
        *p = i + 48;
        i=(dat.hour%10);
        p++;
        *p = i + 48;

        p++;
        StrCopyStr(p,&EE[0],2);

        i=dat.minute/10;
        p++;
        p++;
        *p = i + 48;
        i=dat.minute%10;
        p++;
        *p = i + 48;

        p++;
        StrCopyStr(p,&EE[2],2);

        i=dat.second/10;
        p++;
        p++;
        *p = i + 48;
        i=dat.second%10;
        p++;
        *p = i + 48;

        p++;
        StrCopyStr(p,&EE[4],2);

        p++;
        p++;
        *p = 0;
    }

}

void ShowRecord2(uint16 tnum,uint8 action)
{
    uint8 i,Num;
    fp32 measResult;
    if(action == 1)	  //���ϼ�
    {
        action =0;
        if(OldAction != 2)//�ϴβ��������¼�
        {
            rRecordFlg--; //Ԥ��������ʾ��ʼ���
        }
        rRecordShowFlg-- ;
        OldAction = 1;
        if(rRecordFlg<2)
        {
            ReadFlashRecordSimple(recordShowFlg+1,&rRecord[6+rRecordFlg%8]);//	��̨��1��
        }
        else
        {
            ReadFlashRecordSimple(recordShowFlg+1,&rRecord[(rRecordFlg-2)%8]);//	��̨��1��
        }
    }
    else if(action == 2 ) //���¼�
    {
        action = 0;
        if(OldAction != 1)//�ϴβ��������¼�
        {
            rRecordFlg++;
        }
        rRecordShowFlg++;
        OldAction = 2;
        if(rRecordFlg<2)
        {
            ReadFlashRecordSimple(recordShowFlg-1,&rRecord[6+rRecordFlg%8]);//	��̨��1��
        }
        else
        {
            ReadFlashRecordSimple(recordShowFlg-1,&rRecord[(rRecordFlg-2)%8]);//	��̨��1��
        }
    }

    for(i=0; i<6; i++)
    {
        Num = ((rRecordShowFlg+i)>=0)?((rRecordShowFlg+i)%8):(8+(rRecordShowFlg+i)%7);
        TimeDateKey = rRecord[Num].recordTimeStart;
        //��ʾ����
        DisDate(MenuFormat.ChildXY[i][0],MenuFormat.ChildXY[i][1]+2,TimeDateKey,1);
        DisTime(MenuFormat.ChildXY[i][0]+9*8,MenuFormat.ChildXY[i][1]+2,TimeDateKey,3 );
        DisCList16(MenuFormat.ChildXY[i][0]+6*6+9*8,MenuFormat.ChildXY[i][1]+2,"~");
        TimeDateKey = rRecord[Num].recordTimeEnd;
        DisTime(MenuFormat.ChildXY[i][0]+9*8+8*6,MenuFormat.ChildXY[i][1]+2,TimeDateKey,3);
        //��ʾ����ֵ
        measResult = rRecord[Num].recordResult;

        if(measResult<0)
        {
            DisValue(0,MenuFormat.ChildXY[i][0]+239-10*6,MenuFormat.ChildXY[i][1]+2,1,4,1,0);
        }
        else if(measResult<10)
        {
            DisValue(measResult,MenuFormat.ChildXY[i][0]+239-10*6,MenuFormat.ChildXY[i][1]+2,1,4,1,0);
        }
        else if(measResult<100)
        {
            DisValue(measResult,MenuFormat.ChildXY[i][0]+239-10*6,MenuFormat.ChildXY[i][1]+2,1,4,1,0);
        }
        else if(measResult<1000)
        {
            DisValue(measResult,MenuFormat.ChildXY[i][0]+239-10*6,MenuFormat.ChildXY[i][1]+2,1,4,1,0);
        }
        else
        {
            DisCList12(MenuFormat.ChildXY[i][0]+239-10*6,MenuFormat.ChildXY[i][1]+2,"O.F.");
        }
    }
}



void ShowRecordDetail(uint16 RecordNum)
{
    fp32 measResult;
    MEAS_RECORD LocaRecord;
    area[0] = 0;
    area[1] = 22;
    area[2] = 239;
    area[3] = 159;
    CleanVideo(area);		/*��������*/
    ReadFlashRecordSimple(recordShowFlg,&LocaRecord);//
    TimeDateKey = LocaRecord.recordTimeStart;
    //��ʾ����
    DisDate(MenuFormat.ChildXY[0][0],MenuFormat.ChildXY[0][1]+2,TimeDateKey,1);
    DisTime(MenuFormat.ChildXY[0][0]+9*8,MenuFormat.ChildXY[0][1]+2,TimeDateKey,3 );
    DisCList16(MenuFormat.ChildXY[0][0]+6*6+9*8,MenuFormat.ChildXY[0][1]+2,"~");
    TimeDateKey = LocaRecord.recordTimeEnd;
    DisTime(MenuFormat.ChildXY[0][0]+9*8+8*6,MenuFormat.ChildXY[0][1]+2,TimeDateKey,3);
    //��ʾ����ֵ
    measResult = LocaRecord.recordResult;

    if(measResult<0)
    {
        DisValue(measResult,MenuFormat.ChildXY[0][0]+239-10*6,MenuFormat.ChildXY[0][1]+2,1,5,2,1);
    }
    else if(measResult<10)
    {
        DisValue(measResult,MenuFormat.ChildXY[0][0]+239-10*6,MenuFormat.ChildXY[0][1]+2,1,5,2,0);
    }
    else if(measResult<100)
    {
        DisValue(measResult,MenuFormat.ChildXY[0][0]+239-10*6,MenuFormat.ChildXY[0][1]+2,1,5,2,0);
    }
    else if(measResult<1000)
    {
        DisValue(measResult,MenuFormat.ChildXY[0][0]+239-10*6,MenuFormat.ChildXY[0][1]+2,1,5,2,0);
    }
    else
    {
        DisCList12(MenuFormat.ChildXY[0][0]+239-10*6,MenuFormat.ChildXY[0][1]+2,"O.F.");
    }
    DisCList16(12,47,  "T1:");
    DisCList16(12,69,  " P:");
    DisCList16(12,91,  "C2:");
    DisCList16(12,113, "RH:");
    DisDat.font = 2;
    DisDat.length = 3;
    DisDat.decimal = 1;
    DisDat.DisSign = 1;
    DisValue2(LocaRecord.T1,12+30,47,&DisDat," ��");

    DisDat.length = 4;
    DisDat.DisSign = 0;
    DisValue2(LocaRecord.P,12+30,69,&DisDat,"Kpa");
    DisValue2(LocaRecord.recordResultG,12+30,91,&DisDat,"ug/m3");
    DisDat.length = 5;
    DisDat.decimal = 1;
    DisDat.DisSign = 0;
    DisValue2(LocaRecord.RH,12+30,113,&DisDat," %");

    DisCList16(132,47, "I0:");
    DisCList16(132,69, "I1:");
    DisCList16(132,91, "TV:"); //�ۼ�����
    DisCList16(132,113,"MAS:");

    DisDat.length = 5;
    DisDat.DisSign = 0;
    DisDat.decimal = 0;
    DisValue2(LocaRecord.I0,132+24,47,&DisDat, " ");
    DisValue2(LocaRecord.I,132+24,69,&DisDat,   " ");
    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 0;
    DisValue2(LocaRecord.TV,132+54,91,&DisDat," L");
    DisDat.DisSign = 1;
    DisValue2(LocaRecord.MAS,132+54,113,&DisDat," mg");
    prompt(5);
}


void ShowSysRecord()
{
    uint8 i;
    SysRecord rRecord;

    for(i=0; i<10; i++)
    {
        ReadSysRecord(&rRecord,(SysrecordPoint-1-i));
        //showtime
        TimeDateKey = rRecord.recordTime;
        //��ʾ����
        DisDate(2,20+i*14,TimeDateKey,1);
        DisTime(2+11*6,20+i*14,TimeDateKey,3 );
        //	����
        switch(rRecord.Event)
        {
        case 0:
            WriteCode1212(2+18*6,20+i*14,"��    ��");	//
            break;
        case 1:
            WriteCode1212(2+18*6,20+i*14,"���в���");	//
            break;
        case 2:
            WriteCode1212(2+18*6,20+i*14,"��    ��");	//
            break;
        case 3:
            WriteCode1212(2+18*6,20+i*14,"У׼����");	//
            break;
        case 4:
            WriteCode1212(2+17*6,20+i*14,"ά������");	//
            break;

        default:
            break;
        }
        //����
        if(rRecord.Event == 4)
        {
            continue;
        }
        else if((rRecord.ContentL<15))
            WriteCode1212(2+28*6,20+i*14,SysPro[4*(rRecord.ContentL)+rRecord.Event]);	//
    }

}

uint8  SetEnterTime(void)
{
    if(0 == keyEnterFlg)/*û����ȷ��ʱ  ����ȷ�����ɶ���ֵ���б༭*/
    {
        prompt(1);
        switch(keyFlg)
        {

        case 1:	 /*�괦*/
            area[0] = 71;
            area[1] = 59;
            area[2] = 118;
            area[3] = 77;
            ReverseVideo(area);
            area[0] = 71;
            area[1] = 59;
            area[2] = 102;
            area[3] = 77;
            ReverseVideo(area);
            keyEnterFlg = 1;
            return 0;

        case 2:
            area[0] = 119;
            area[1] = 59;
            area[2] = 150;
            area[3] = 77;
            ReverseVideo(area);
            area[0] = 119;
            area[1] = 59;
            area[2] = 134;
            area[3] = 77;
            ReverseVideo(area);
            keyEnterFlg = 1;
            return 0;

        case 3:
            area[0] = 151;
            area[1] = 59;
            area[2] = 166;
            area[3] = 77;
            ReverseVideo(area);
            area[0] = 151;
            area[1] = 59;
            area[2] = 181;
            area[3] = 77;
            ReverseVideo(area);
            keyEnterFlg = 1;
            return 0;
        case 4:
            area[0] = 79;
            area[1] = 79;
            area[2] = 94;
            area[3] = 97;
            ReverseVideo(area);
            area[0] = 79;
            area[1] = 79;
            area[2] = 110;
            area[3] = 97;
            ReverseVideo(area);
            keyEnterFlg = 1;
            return 0;
        case 5:
            area[0] = 111;
            area[1] = 79;
            area[2] = 126;
            area[3] = 97;
            ReverseVideo(area);
            area[0] = 111;
            area[1] = 79;
            area[2] = 142;
            area[3] = 97;
            ReverseVideo(area);
            keyEnterFlg = 1;
            return 0;

        case 6:
            area[0] = 143;
            area[1] = 79;
            area[2] = 158;
            area[3] = 97;
            ReverseVideo(area);
            area[0] = 143;
            area[1] = 79;
            area[2] = 174;
            area[3] = 97;
            ReverseVideo(area);
            keyEnterFlg = 1;
            return 0;
        case 7:
            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_SECOND, TimeDateKey.second);
            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MINUTE, TimeDateKey.minute);
            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_HOUR, TimeDateKey.hour);
            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_YEAR, TimeDateKey.year);
            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MONTH, TimeDateKey.month);
            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, TimeDateKey.date);
            keyEnterFlg = 1;
            EscKeyAutoPorc(MenuId);
            keyEnterFlg = 0;
            keyFlg = 0;

            break;
        default:
            break;
        }
    }
    else   /*�Ѿ�����ȷ��  �ٰ�ȷ�� ����ֵȷ��*/
    {   prompt(5);

        switch(keyFlg)
        {
        case 1:
            area[0] = 71;
            area[1] = 59;
            area[2] = 102;
            area[3] = 77;
            ReverseVideo(area);
            area[0] = 71;
            area[1] = 59;
            area[2] = 118;
            area[3] = 77;
            ReverseVideo(area);
            keyEnterFlg = 0;
            //LcdUpdata(DisBuf);
            return 0;
        case 2:
            area[0] = 119;
            area[1] = 59;
            area[2] = 134;
            area[3] = 77;
            ReverseVideo(area);
            area[0] = 119;
            area[1] = 59;
            area[2] = 150;
            area[3] = 77;
            ReverseVideo(area);
            keyEnterFlg = 0;
            //LcdUpdata(DisBuf);
            return 0;
        case 3:

            area[0] = 151;
            area[1] = 59;
            area[2] = 181;
            area[3] = 77;
            ReverseVideo(area);
            area[0] = 151;
            area[1] = 59;
            area[2] = 166;
            area[3] = 77;
            ReverseVideo(area);
            keyEnterFlg = 0;
            //LcdUpdata(DisBuf);
            return 0;
        case 4:
            area[0] = 79;
            area[1] = 79;
            area[2] = 110;
            area[3] = 97;
            ReverseVideo(area);
            area[0] = 79;
            area[1] = 79;
            area[2] = 94;
            area[3] = 97;
            ReverseVideo(area);
            keyEnterFlg = 0;
            //LcdUpdata(DisBuf);
            return 0;

        case 5:
            area[0] = 111;
            area[1] = 79;
            area[2] = 126;
            area[3] = 97;
            ReverseVideo(area);
            area[0] = 111;
            area[1] = 79;
            area[2] = 142;
            area[3] = 97;
            ReverseVideo(area);
            keyEnterFlg = 0;
            //LcdUpdata(DisBuf);
            return 0;
        case 6:
            area[0] = 143;
            area[1] = 79;
            area[2] = 174;
            area[3] = 97;
            ReverseVideo(area);
            area[0] = 143;
            area[1] = 79;
            area[2] = 158;
            area[3] = 97;
            ReverseVideo(area);
            keyEnterFlg = 0;
            //LcdUpdata(DisBuf);
            return 0;

        default:
            break;
        }
    }
    return 0;
}

uint8  SetDownTime(void)
{
    if(0 == keyEnterFlg)
    {
        keyFlg = keyFlg + 1;
        if(keyFlg > 7)
        {
            keyFlg = 1;
        }
        switch(keyFlg) /*//������ ʱ���� ȷ�� ȡ�� ѡ��*/
        {
        case 1:
            ReverseVideo(areaMiddle);
            area[0] = 71;
            area[1] = 59;
            area[2] = 118;
            area[3] = 77;
            ReverseVideo(area);
            break;
        case 2:
            area[0] = 71;
            area[1] = 59;
            area[2] = 118;
            area[3] = 77;
            ReverseVideo(area);
            area[0] = 119;
            area[1] = 59;
            area[2] = 150;
            area[3] = 77;
            ReverseVideo(area);
            break;
        case 3:
            area[0] = 119;
            area[1] = 59;
            area[2] = 150;
            area[3] = 77;
            ReverseVideo(area);
            area[0] = 151;
            area[1] = 59;
            area[2] = 181;
            area[3] = 77;
            ReverseVideo(area);

            break;
        case 4:
            area[0] = 151;
            area[1] = 59;
            area[2] = 181;
            area[3] = 77;
            ReverseVideo(area);
            area[0] = 79;
            area[1] = 79;
            area[2] = 110;
            area[3] = 97;
            ReverseVideo(area);
            break;
        case 5:
            area[0] = 79;
            area[1] = 79;
            area[2] = 110;
            area[3] = 97;
            ReverseVideo(area);
            area[0] = 111;
            area[1] = 79;
            area[2] = 142;
            area[3] = 97;
            ReverseVideo(area);
            break;
        case 6:
            area[0] = 111;
            area[1] = 79;
            area[2] = 142;
            area[3] = 97;
            ReverseVideo(area);
            area[0] = 143;
            area[1] = 79;
            area[2] = 174;
            area[3] = 97;
            ReverseVideo(area);
            break;
        case 7:
            area[0] = 143;
            area[1] = 79;
            area[2] = 174;
            area[3] = 97;
            ReverseVideo(area);
            ReverseVideo(areaMiddle);
            break;
        default:
            break;
        }
    }
    /*//���� ȷ��  ��ֵ������  ��ֵ����*/
    else
    {   /*/ ʱ���� ������*/
        switch(keyFlg)
        {
        case 1:
            area[0] = 71;
            area[1] = 59;
            area[2] = 183;
            area[3] = 99;
            CleanVideo(area);
            if(TimeDateKey.year<=2010)
            {
                TimeDateKey.year = 2010;
            }
            else
            {
                TimeDateKey.year -=1;
            }
            DealYMD(&strKey[0],TimeDateKey);
            DisCList16(71,60,strKey);

            DealHMS(&strKey[0],TimeDateKey);
            DisCList16(79,80,strKey);
            area[0] = 71;
            area[1] = 59;
            area[2] = 102;
            area[3] = 77;
            ReverseVideo(area);
            break;
        case 2:
            area[0] = 71;
            area[1] = 59;
            area[2] = 183;
            area[3] = 99;
            CleanVideo(area);
            if(TimeDateKey.month<=0)
            {
                TimeDateKey.month = 12;
            }
            else
            {
                TimeDateKey.month -=1;
            }
            DealYMD(&strKey[0],TimeDateKey);
            DisCList16(71,60,strKey);

            DealHMS(&strKey[0],TimeDateKey);
            DisCList16(79,80,strKey);
            area[0] = 119;
            area[1] = 59;
            area[2] = 134;
            area[3] = 77;
            ReverseVideo(area);
            break;
        case 3:
            area[0] = 71;
            area[1] = 59;
            area[2] = 183;
            area[3] = 99;
            CleanVideo(area);
            if(TimeDateKey.date<=0)
            {
                TimeDateKey.date = 31;
            }
            else
            {
                TimeDateKey.date -=1;
            }
            DealYMD(&strKey[0],TimeDateKey);
            DisCList16(71,60,strKey);

            DealHMS(&strKey[0],TimeDateKey);
            DisCList16(79,80,strKey);
            area[0] = 151;
            area[1] = 59;
            area[2] = 166;
            area[3] = 77;
            ReverseVideo(area);
            break;
        case 4:
            area[0] = 71;
            area[1] = 59;
            area[2] = 183;
            area[3] = 99;
            CleanVideo(area);
            if(TimeDateKey.hour<=0)
            {
                TimeDateKey.hour = 23;
            }
            else
            {
                TimeDateKey.hour -=1;
            }
            DealYMD(&strKey[0],TimeDateKey);
            DisCList16(71,60,strKey);

            DealHMS(&strKey[0],TimeDateKey);
            DisCList16(79,80,strKey);
            area[0] = 79;
            area[1] = 79;
            area[2] = 94;
            area[3] = 97;
            ReverseVideo(area);
            break;
        case 5:
            area[0] = 71;
            area[1] = 59;
            area[2] = 183;
            area[3] = 99;
            CleanVideo(area);
            if(TimeDateKey.minute<=0)
            {
                TimeDateKey.minute = 59;
            }
            else
            {
                TimeDateKey.minute -=1;
            }
            DealYMD(&strKey[0],TimeDateKey);
            DisCList16(71,60,strKey);

            DealHMS(&strKey[0],TimeDateKey);
            DisCList16(79,80,strKey);
            area[0] = 111;
            area[1] = 79;
            area[2] = 126;
            area[3] = 97;
            ReverseVideo(area);
            break;
        case 6:
            area[0] = 71;
            area[1] = 59;
            area[2] = 183;
            area[3] = 99;
            CleanVideo(area);
            if(TimeDateKey.second<=0)
            {
                TimeDateKey.second = 59;
            }
            else
            {
                TimeDateKey.second -=1;
            }
            DealYMD(&strKey[0],TimeDateKey);
            DisCList16(71,60,strKey);

            DealHMS(&strKey[0],TimeDateKey);
            DisCList16(79,80,strKey);
            area[0] = 143;
            area[1] = 79;
            area[2] = 158;
            area[3] = 97;
            ReverseVideo(area);
            break;

        default:
            break;
        }
    }
    return 0;
}

uint8 SetRightIp(void)
{
    keyFlg = keyFlg + 1;
    if(keyFlg > 11)
    {
        keyFlg = 0;
    }
    ReverseVideo(IpArea[keyFlg]);
    if(keyFlg == 0)
        ReverseVideo(IpArea[11]);
    else
        ReverseVideo(IpArea[keyFlg-1]);
    return 0;
}

void ProIpDat(uint8 *dat)
{
    int i = 0;
    ip.first = (dat[i++]+0.1)+(dat[i++]*10+0.1)+(dat[i++]*100+0.1);
    ip.second = (dat[i++]+0.1)+(dat[i++]*10+0.1)+(dat[i++]*100+0.1);
    ip.third = (dat[i++]+0.1)+(dat[i++]*10+0.1)+(dat[i++]*100+0.1);
    ip.end = (dat[i++]+0.1)+(dat[i++]*10+0.1)+(dat[i++]*100+0.1);
}

int SetUpIp(void)
{
    ipdat[keyFlg]++;
    if(ipdat[keyFlg]>9)
        ipdat[keyFlg] = 0;
    ProIpDat(ipdat);
    ReverseVideo(IpArea[keyFlg]);
    DealIP(&strKey[0],ip); /*ip��ʾ*/
    DisCList16(60,60,strKey);
    ReverseVideo(IpArea[keyFlg]);
}

void SetDownIp(void)
{
    ipdat[keyFlg]--;
    if(ipdat[keyFlg]<0)
        ipdat[keyFlg] = 9;
    ProIpDat(ipdat);
    ReverseVideo(IpArea[keyFlg]);
    DealIP(&strKey[0],ip); /*ip��ʾ*/
    DisCList16(60,60,strKey);
    ReverseVideo(IpArea[keyFlg]);
}

uint8  SetUpTime(void)
{
    if(0 == keyEnterFlg)
    {
        keyFlg = keyFlg - 1;
        if(keyFlg <= 0)
        {
            keyFlg = 7;
        }
        /*
        6����ʾλ�� ��Ӧ ������ ʱ����
        */
        switch(keyFlg)
        {
        case 1:
            area[0] = 119;
            area[1] = 59;
            area[2] = 150;
            area[3] = 77;
            ReverseVideo(area);
            area[0] = 71;
            area[1] = 59;
            area[2] = 118;
            area[3] = 77;
            ReverseVideo(area);
            break;
        case 2:
            area[0] = 151;
            area[1] = 59;
            area[2] = 181;
            area[3] = 77;
            ReverseVideo(area);
            area[0] = 119;
            area[1] = 59;
            area[2] = 150;
            area[3] = 77;
            ReverseVideo(area);
            break;
        case 3:
            area[0] = 79;
            area[1] = 79;
            area[2] = 110;
            area[3] = 97;
            ReverseVideo(area);
            area[0] = 151;
            area[1] = 59;
            area[2] = 181;
            area[3] = 77;
            ReverseVideo(area);
            break;
        case 4:
            area[0] = 111;
            area[1] = 79;
            area[2] = 142;
            area[3] = 97;
            ReverseVideo(area);
            area[0] = 79;
            area[1] = 79;
            area[2] = 110;
            area[3] = 97;
            ReverseVideo(area);
            break;
        case 5:
            area[0] = 143;
            area[1] = 79;
            area[2] = 174;
            area[3] = 97;
            ReverseVideo(area);
            area[0] = 111;
            area[1] = 79;
            area[2] = 142;
            area[3] = 97;
            ReverseVideo(area);
            break;
        case 6:
            ReverseVideo(areaMiddle);
            area[0] = 143;
            area[1] = 79;
            area[2] = 174;
            area[3] = 97;
            ReverseVideo(area);
            break;

        case 7:
            area[0] = 71;
            area[1] = 59;
            area[2] = 118;
            area[3] = 77;
            ReverseVideo(area);
            ReverseVideo(areaMiddle);

            break;

        default:
            break;
        }
    }
    /*
    ����ȷ�� ����ֻ�Զ���ֵ���мӼ�
    		keyflg ��Ӧ ������ ʱ���� ����ֵ
    */
    else
    {
        switch(keyFlg)
        {
        case 1:
            area[0] = 71;
            area[1] = 59;
            area[2] = 183;
            area[3] = 99;
            CleanVideo(area);
            if(TimeDateKey.year>=2110)
            {
                TimeDateKey.year = 2010;
            }
            else
            {
                TimeDateKey.year +=1;
            }
            DealYMD(&strKey[0],TimeDateKey);
            DisCList16(71,60,strKey);

            DealHMS(&strKey[0],TimeDateKey);
            DisCList16(79,80,strKey);
            area[0] = 71;
            area[1] = 59;
            area[2] = 102;
            area[3] = 77;
            ReverseVideo(area);
            break;
        case 2:
            area[0] = 71;
            area[1] = 59;
            area[2] = 183;
            area[3] = 99;
            CleanVideo(area);
            if(TimeDateKey.month>=12)
            {
                TimeDateKey.month = 0;
            }
            else
            {
                TimeDateKey.month +=1;
            }
            DealYMD(&strKey[0],TimeDateKey);
            DisCList16(71,60,strKey);

            DealHMS(&strKey[0],TimeDateKey);
            DisCList16(79,80,strKey);
            area[0] = 119;
            area[1] = 59;
            area[2] = 134;
            area[3] = 77;
            ReverseVideo(area);
            break;
        case 3:
            area[0] = 71;
            area[1] = 59;
            area[2] = 183;
            area[3] = 99;
            CleanVideo(area);
            if(TimeDateKey.date>=31)
            {
                TimeDateKey.date = 0;
            }
            else
            {
                TimeDateKey.date +=1;
            }
            DealYMD(&strKey[0],TimeDateKey);
            DisCList16(71,60,strKey);

            DealHMS(&strKey[0],TimeDateKey);
            DisCList16(79,80,strKey);
            area[0] = 151;
            area[1] = 59;
            area[2] = 166;
            area[3] = 77;
            ReverseVideo(area);
            break;
        case 4:
            area[0] = 71;
            area[1] = 59;
            area[2] = 183;
            area[3] = 99;
            CleanVideo(area);
            if(TimeDateKey.hour>=23)
            {
                TimeDateKey.hour = 0;
            }
            else
            {
                TimeDateKey.hour +=1;
            }
            DealYMD(&strKey[0],TimeDateKey);
            DisCList16(71,60,strKey);

            DealHMS(&strKey[0],TimeDateKey);
            DisCList16(79,80,strKey);
            area[0] = 79;
            area[1] = 79;
            area[2] = 94;
            area[3] = 97;
            ReverseVideo(area);
            break;
        case 5:
            area[0] = 71;
            area[1] = 59;
            area[2] = 183;
            area[3] = 99;
            CleanVideo(area);
            if(TimeDateKey.minute>=59)
            {
                TimeDateKey.minute = 0;
            }
            else
            {
                TimeDateKey.minute +=1;
            }
            DealYMD(&strKey[0],TimeDateKey);
            DisCList16(71,60,strKey);

            DealHMS(&strKey[0],TimeDateKey);
            DisCList16(79,80,strKey);
            area[0] = 111;
            area[1] = 79;
            area[2] = 126;
            area[3] = 97;
            ReverseVideo(area);
            break;
        case 6:
            area[0] = 71;
            area[1] = 59;
            area[2] = 183;
            area[3] = 99;
            CleanVideo(area);
            if(TimeDateKey.second>=59)
            {
                TimeDateKey.second = 0;
            }
            else
            {
                TimeDateKey.second +=1;
            }
            DealYMD(&strKey[0],TimeDateKey);
            DisCList16(71,60,strKey);

            DealHMS(&strKey[0],TimeDateKey);
            DisCList16(79,80,strKey);
            area[0] = 143;
            area[1] = 79;
            area[2] = 158;
            area[3] = 97;
            ReverseVideo(area);
            break;

        default:
            break;
        }
    }

    return 0;
}


uint32 getDatLenth(uint8 wei)
{
    switch(wei)
    {
    case 0:
        return 1;
    case 1:
        return 10;
    case 2:
        return 100;
    case 3:
        return 1000;
    case 4:
        return 10000;
    case 5:
        return 100000;
    case 6:
        return 1000000;

    default:
        return 1;
    }
}


fp32 LoadSensorDat(fp32 dat ,uint8 ParamName,uint8 action)
{
    uint8 i,j;
    uint32 dat2;
    for(i=0; i<NUM_PARAM; i++)
    {
        if(ParamName == DisDat2[i].ParamName)
        {
            break;
        }
    }
    if(OffsetFlg == (DisDat2[i].length - DisDat2[i].decimal+DisDat2[i].DisSign))//ѡ��С���㣬������	?
        return dat;
    if(dat<0)
    {
        sign = -1;
    }
    else
    {
        sign = 1;
    }
    dat = fabs(dat);
    dat2 = (uint32)(dat*getDatLenth(DisDat2[i].decimal)+0.1);	 //��ֹ����ת����ʧ�档
    SensorDatS[0]=0;
    SensorDatS[1]=dat2/getDatLenth(DisDat2[i].length-1);
    for(j=1; j<DisDat2[i].length; j++)
    {
        SensorDatS[j+1] = (uint16)(dat2)/(uint16)(getDatLenth(DisDat2[i].length-1-j))%10;
    }
    if(action)
    {
        if(OffsetFlg>(DisDat2[i].length - DisDat2[i].decimal+DisDat2[i].DisSign)) //С��������Ҫ�Ӽ����ұ�һλ���Ǹ�����
        {
            //�� -325,7Ϊ��  S[0]��ʾ������ S[1]��ʾ3 S[2]��ʾ2 s[3]��ʾ5 s[4]��ʾ7.С����ֱ��������
            SensorDatS[OffsetFlg+1-DisDat2[i].DisSign-1]++;
            if(SensorDatS[OffsetFlg+1-DisDat2[i].DisSign-1]>9)
                SensorDatS[OffsetFlg+1-DisDat2[i].DisSign-1]=0;
        }
        else
        {
            SensorDatS[OffsetFlg+1-DisDat2[i].DisSign]++;
            if(SensorDatS[OffsetFlg+1-DisDat2[i].DisSign]>9)
                SensorDatS[OffsetFlg+1-DisDat2[i].DisSign]=0;
        }
    }
    else
    {
        if(OffsetFlg>(DisDat2[i].length - DisDat2[i].decimal+DisDat2[i].DisSign))
        {
            SensorDatS[OffsetFlg+1-DisDat2[i].DisSign-1]--;
            if(SensorDatS[OffsetFlg+1-DisDat2[i].DisSign-1]<0)
                SensorDatS[OffsetFlg+1-DisDat2[i].DisSign-1]=9;
        }
        else
        {
            SensorDatS[OffsetFlg+1-DisDat2[i].DisSign]--;
            if(SensorDatS[OffsetFlg+1-DisDat2[i].DisSign]<0)
                SensorDatS[OffsetFlg+1-DisDat2[i].DisSign]=9;
        }
    }
    sign = 	(SensorDatS[0]%2)?(-1*sign):(sign);
    dat = 0;
    for(j=1; j<DisDat2[i].length+1; j++)
    {
        dat= dat+ SensorDatS[j]*getDatLenth(DisDat2[i].length-j);
    }
    return sign*dat/getDatLenth(DisDat2[i].decimal);

}

