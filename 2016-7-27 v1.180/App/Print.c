/********************************************************************************************
  Copyright (C), Skyray Instrment Co.,LTD.
  FileName: Print.c
  Author:      heshunjie  Version :    v1.0      Date: 2011-8-3 14:47:46
  Description:     ������ӡ������
  Version:         v2.x
  Function List:
		static void UART3_SendByte(uint8_t Data)
		static void SendDataToPrinter(unsigned char *buffer,unsigned int len) �������ݵ���ӡ��
		void PrintValue(fp32 fValuel,uint8 length,uint8 decimal)   ��ӡ���ݸ�ʽ
		void PrintRecord_page(uint32 num)  ������ӡ
		void PrintRecord_one(uint32 num)   ���δ�ӡ
		void Research()   	������������
  History:         // ��ʷ�޸ļ�¼
    <author>      <time>           <version >          <desc>
	heshunjie     2011-8-3         1.0                 build this moudle
*********************************************************************************************/
#include "Print.h"

uint8 iii=0;
static uint8 count=0;
uint8  bt_st1[10]= {0x41,0x54,0x2B,0x53,0x45,0x54,0x54,0x49,0x4E,0x47}; //AT+SETTING����ģʽ1,����ģʽ,�˳�0��2
uint8  bt_st1r[]="OK+STATE:1";
uint8  bt_clear[8]= {0x41,0x54,0x2B,0x43,0x4C,0x45,0x41,0x52}; //������е�ַ
uint8  bt_clearr[]="OK+CLEAR";
uint8  bt_search[9]= {0x41,0x54,0x2B,0x53,0x45,0x41,0x52,0x43,0x48}; //����������ӡ��
uint8  bt_searchr[]="OK+SEARCHING";
uint8  bt_searchok[]="OK+CONNECTED";

//��������
static  uint8  midd[40];				//�����������ݻ�����

static void UART3_SendByte(uint8_t Data)
{
    LPC_UART3->/*RBTHDLR.*/THR = Data & UART_THR_MASKBIT;
    while(UART_CheckBusy(LPC_UART3));
}
//void SendDataToPrinter(unsigned char *buffer,unsigned int len)

static void SendDataToPrinter(unsigned char *buffer,unsigned int len)
{
    unsigned int i;

    for (i = 0; i < len; i++)
    {   /* ����ָ�����ֽ�����           */
        UART3_SendByte((*buffer++));
    }
}
/*********************************************************************************************************
* Function name: void PrintValue(void)
* Descriptions:  ��ӡŨ��
* input parameters:  fvaluel Ũ��ֵ
						length
						decimal ʮ����λ��
* output parameters:
* Returned value:
*********************************************************************************************************/
void PrintValue(fp32 fValuel,uint8 length,uint8 decimal)
{
    uint32 ulTemp;
    uint8  aDis[10];
    uint8 i;
    uint8 sign=0;                 //=0 +  ,=1 -
    unsigned char c[20]= {"OverFlow"}; //���
    int8 numLen;					 //��ֵ����
    fp32 fd;
    /*����
    		ȡ����ֵ
    		������־λ
    */
    if(fValuel<0)
    {
        fValuel=fabs(fValuel);
        sign=1;
    }
    /*ȡ���ݳ���*/
    numLen=GetNumLen(fValuel);
    if((numLen+decimal)<=length)
    {
        fd=fValuel*Num10n(decimal);

        ulTemp=(uint32)fd;
        LongIntegerToBit(ulTemp,aDis);

        if(1==sign)
        {
            UART3_SendByte('-');
        }
        else
        {
            UART3_SendByte(' ');
        }
        if(0==numLen)
        {
            UART3_SendByte('0');
        }
        else
        {
            for(i=1; i<=numLen; i++)        //��ʾ����
            {
                UART3_SendByte('0'+aDis[9-numLen-decimal+i]);
            }
        }
        if(0<decimal)
        {
            UART3_SendByte('.');
            for(i=1; i<=decimal; i++)
            {
                UART3_SendByte('0'+aDis[9-decimal+i]);
            }
        }
    }
    else if(numLen<=length)  //������С������С�ڳ���
    {
        fd=fValuel*Num10n(length-numLen);
        ulTemp=(uint32)fd;
        LongIntegerToBit(ulTemp,aDis);

        if(1==sign)
        {
            UART3_SendByte('-');
        }
        else
        {
            UART3_SendByte(' ');
        }

        for(i=1; i<=length; i++)  //��ʾ����
        {
            UART3_SendByte('0'+aDis[9-length+i]);

            if((i==numLen)&&(numLen<length))
            {
                UART3_SendByte('.');
            }
        }
    }
    else
    {
        SendDataToPrinter(c,4);	   //���
    }
}
/*********************************************************************************************************
* Function name:       PrintRecord
* Descriptions:        ��ӡ��ǰ��¼ �����ϵ����� ���10��
* input parameters:    num ����ӡ�� �����
						tnum ��ӡ���ݵ�����
* output parameters:
* Returned value:       ��
*********************************************************************************************************/
void PrintRecord_page(uint32 num)  /*��λ�� д����*/
{
    uint8 i,j,timedat[4];
    uint8 aDis[20];
    MEAS_RECORD rRecord;
    fp32 measResult;

    for(i=0; (i < (recordPoint - num) && (i < 10)); i++)
    {
        ReadRecord(num+i,&rRecord); /*��λ�� �� �����Ƶı���*/
        TimeDateKey = rRecord.recordTime;
        UART3_SendByte(0x0d);  		 //�س�
        UART3_SendByte(0x1b);    		 //��ӡλ��
        UART3_SendByte(0x44);
        UART3_SendByte(0x02);			//����λ��
        UART3_SendByte(0x10);    		 //����λ��
        UART3_SendByte(0x18);    		 //����λ��

        UART3_SendByte(0x00);    		 //�������

        SendDataToPrinter("- - - - - - - - - - - - - ",25);
        UART3_SendByte(0x0d);  		 //�س�


        /*��ӡelement*/
        UART3_SendByte(0x09);			//ht����	  �������õ�λ��
        SendDataToPrinter("Element:",10);
        /*��ӡԪ��*/
        UART3_SendByte(0x09);			//ht����	  �������õ�λ��
        StrCopyStr(aDis,rRecord.menuName,10);
        aDis[2] = 0;

        SendDataToPrinter(aDis,10);
        UART3_SendByte(0x0d);	  //�س�
        UART3_SendByte(0x09);			//ht����	  �������õ�λ��
        SendDataToPrinter("Concentration:",13);
        UART3_SendByte(0x09);			//ht����	  �������õ�λ��
        /*��ӡ����ֵ ����λ*/
        measResult = rRecord.recordResult;
        if(measResult<0)
        {
            SendDataToPrinter("��Ч����",10);
        }
        else if(measResult<10)
        {
            PrintValue(measResult,5,2);
            UART3_SendByte(0x09);	//ht����
            //SendDataToPrinter("΢��/��",8);
            SendDataToPrinter("��g/L",5);
        }
        else if(measResult<100)
        {
            PrintValue(measResult,5,2);
            UART3_SendByte(0x09);	//ht����
            SendDataToPrinter("��g/L",5);
            //SendDataToPrinter("΢��/��",8);
        }
        else if(measResult<1000)
        {
            PrintValue(measResult,5,2);
            UART3_SendByte(0x09);	//ht����

            SendDataToPrinter("��g/L",5);
            ////SendDataToPrinter("΢��/��",8);
        }
        else if(measResult<10000)
        {
            PrintValue(measResult/1000.0,5,2);
            UART3_SendByte(0x09);	//ht����
            SendDataToPrinter("mg/L",5);
        }
        else
        {
            PrintValue(measResult/1000.0,5,2);
            UART3_SendByte(0x09);	//ht����
            SendDataToPrinter("mg/L",5);
        }
        UART3_SendByte(0x0d);  		 //�س�

        /*��ӡtime*/
        UART3_SendByte(0x09);			//ht����	  �������õ�λ��
        SendDataToPrinter("Time:",6);

        /*��ӡ����*/
        UART3_SendByte(0x09);			//ht����	  �������õ�λ��
        timedat[0]=(TimeDateKey.year/1000);

        timedat[1]=TimeDateKey.year%1000/100;
        timedat[2]=(TimeDateKey.year%100/10);
        timedat[3]=(TimeDateKey.year%10);
        for(j = 0; j<4; j++)
        {
            timedat[j] += 0x30;
        }
        SendDataToPrinter(timedat,4);
        SendDataToPrinter("/",1);
        timedat[0]=TimeDateKey.month/10;
        timedat[1]=TimeDateKey.month%10;
        for(j = 0; j<2; j++)
        {
            timedat[j] += 0x30;
        }
        SendDataToPrinter(timedat,2);
        SendDataToPrinter("/",1);
        timedat[0]=TimeDateKey.date/10;
        timedat[1]=TimeDateKey.date%10;
        for(j = 0; j<2; j++)
        {
            timedat[j] += 0x30;
        }
        SendDataToPrinter(timedat,2);
        SendDataToPrinter(" ",1);
        timedat[0]=TimeDateKey.hour/10;
        timedat[1]=TimeDateKey.hour%10;
        for(j = 0; j<2; j++)
        {
            timedat[j] += 0x30;
        }
        SendDataToPrinter(timedat,2);
        SendDataToPrinter(":",1);
        timedat[0]=TimeDateKey.minute/10;
        timedat[1]=TimeDateKey.minute%10;
        for(j = 0; j<2; j++)
        {
            timedat[j] += 0x30;
        }
        SendDataToPrinter(timedat,2);
        UART3_SendByte(0x0d);  		 //�س�
        /*��ӡdevice:*/
        UART3_SendByte(0x09);			//ht����	  �������õ�λ��
        SendDataToPrinter("Device:",8);
        UART3_SendByte(0x09);			//ht����	  �������õ�λ��
        SendDataToPrinter("�������� HM-3000P",20);
        UART3_SendByte(0x0d);  		 //�س�
        SendDataToPrinter("- - - - - - - - - - - - - ",25);

    }
}

/*********************************************************************************************************
* Function name:       PrintRecord_one
* Descriptions:        ��ӡ��ǰ ѡ�е� ��¼ ����
* input parameters:    num ����ӡ�� �����
						tnum ��ӡ���ݵ�����
* output parameters:
* Returned value:       ��
*********************************************************************************************************/
void PrintRecord_one(uint32 num)  /*��λ�� д����*/
{
    uint8 j,timedat[4];
    uint8 aDis[20];
    MEAS_RECORD rRecord;
    fp32 measResult;

    ReadRecord(num,&rRecord); /*��λ�� �� �����Ƶı���*/
    TimeDateKey = rRecord.recordTime;
    UART3_SendByte(0x0d);  		 //�س�
    UART3_SendByte(0x1b);    		 //��ӡλ��
    UART3_SendByte(0x44);
    UART3_SendByte(0x02);			//����λ��
    UART3_SendByte(0x10);    		 //����λ��
    UART3_SendByte(0x18);    		 //����λ��

    UART3_SendByte(0x00);    		 //�������

    SendDataToPrinter("- - - - - - - - - - - - - ",25);
    UART3_SendByte(0x0d);  		 //�س�


    /*��ӡelement*/
    UART3_SendByte(0x09);			//ht����	  �������õ�λ��
    SendDataToPrinter("Element:",10);
    /*��ӡԪ��*/
    UART3_SendByte(0x09);			//ht����	  �������õ�λ��
    StrCopyStr(aDis,rRecord.menuName,10);
    aDis[2] = 0;

    SendDataToPrinter(aDis,10);
    UART3_SendByte(0x0d);	  //�س�
    UART3_SendByte(0x09);			//ht����	  �������õ�λ��
    SendDataToPrinter("Concentration:",13);
    UART3_SendByte(0x09);			//ht����	  �������õ�λ��
    /*��ӡ����ֵ ����λ*/
    measResult = rRecord.recordResult;

    if(measResult<0)
    {
        SendDataToPrinter("��Ч����",10);
    }
    else if(measResult<10)
    {
        PrintValue(measResult,5,2);
        UART3_SendByte(0x09);	//ht����
        SendDataToPrinter("��g/L",5);
        //SendDataToPrinter("΢��/��",8);

    }
    else if(measResult<100)
    {
        PrintValue(measResult,5,2);
        UART3_SendByte(0x09);	//ht����
        SendDataToPrinter("��g/L",5);
        //SendDataToPrinter("΢��/��",8);
    }
    else if(measResult<1000)
    {
        PrintValue(measResult,5,2);
        UART3_SendByte(0x09);	//ht����
        SendDataToPrinter("��g/L",5);
        //SendDataToPrinter("΢��/��",8);
    }
    else if(measResult<10000)
    {
        PrintValue(measResult/1000.0,5,2);
        UART3_SendByte(0x09);	//ht����
        SendDataToPrinter("mg/L",4);
    }
    else
    {
        PrintValue(measResult/1000.0,5,2);
        UART3_SendByte(0x09);	//ht����
        SendDataToPrinter("mg/L",4);
    }
    UART3_SendByte(0x0d);  		 //�س�

    /*��ӡtime*/
    UART3_SendByte(0x09);			//ht����	  �������õ�λ��
    SendDataToPrinter("Time:",6);

    /*��ӡ����*/
    UART3_SendByte(0x09);			//ht����	  �������õ�λ��
    timedat[0]=(TimeDateKey.year/1000);

    timedat[1]=TimeDateKey.year%1000/100;
    timedat[2]=(TimeDateKey.year%100/10);
    timedat[3]=(TimeDateKey.year%10);
    for(j = 0; j<4; j++)
    {
        timedat[j] += 0x30;
    }
    SendDataToPrinter(timedat,4);
    SendDataToPrinter("/",1);
    timedat[0]=TimeDateKey.month/10;
    timedat[1]=TimeDateKey.month%10;
    for(j = 0; j<2; j++)
    {
        timedat[j] += 0x30;
    }
    SendDataToPrinter(timedat,2);
    SendDataToPrinter("/",1);
    timedat[0]=TimeDateKey.date/10;
    timedat[1]=TimeDateKey.date%10;
    for(j = 0; j<2; j++)
    {
        timedat[j] += 0x30;
    }
    SendDataToPrinter(timedat,2);
    SendDataToPrinter(" ",1);
    timedat[0]=TimeDateKey.hour/10;
    timedat[1]=TimeDateKey.hour%10;
    for(j = 0; j<2; j++)
    {
        timedat[j] += 0x30;
    }
    SendDataToPrinter(timedat,2);
    SendDataToPrinter(":",1);
    timedat[0]=TimeDateKey.minute/10;
    timedat[1]=TimeDateKey.minute%10;
    for(j = 0; j<2; j++)
    {
        timedat[j] += 0x30;
    }
    SendDataToPrinter(timedat,2);
    UART3_SendByte(0x0d);  		 //�س�
    /*��ӡdevice:*/
    UART3_SendByte(0x09);			//ht����	  �������õ�λ��
    SendDataToPrinter("Device:",8);
    UART3_SendByte(0x09);			//ht����	  �������õ�λ��
    SendDataToPrinter("�������� HM-3000P",20);
    UART3_SendByte(0x0d);  		 //�س�
    SendDataToPrinter("- - - - - - - - - - - - - ",25);
}

/*********************************************************************************************************
* Function name:   ������������ʼ��
* Descriptions:
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
void int_midd(void)
{
    uint8 i;
    for(i=0; i<40; i++)
        midd[i]='\0';
    iii=0;
}
/*********************************************************************************************************
* Function name:    ������������
* Descriptions:
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
void bt_test(void)
{
    uint8 i;

    while(count<10)
    {
        int_midd();
        for(i=0; i<10; i++)
        {
            UART3_SendByte(bt_st1[i]);
        }
        OSTimeDlyHMSM(0,0,0,500);
        if(0!=strstr(midd,bt_st1r))
        {
            iii=0;
            count=200;
        }
        else
        {
            iii=0;
            count++;
        }
    }
    count=0;
    while(count<10)
    {
        int_midd();
        for(i=0; i<8; i++)
        {
            UART3_SendByte(bt_clear[i]);
        }
        OSTimeDlyHMSM(0,0,0,500);
        if(0!=strstr(midd,bt_clearr))
        {
            iii=0;
            count=200;
        }
        else
        {
            iii=0;
            count++;
        }
    }
    count=0;
    while(count<10)
    {
        int_midd();
        for(i=0; i<9; i++)
        {
            UART3_SendByte(bt_search[i]);
        }
        OSTimeDlyHMSM(0,0,0,500);
        if(0!=strstr(midd,bt_searchr))
        {
            iii=0;
            count=200;
        }
        else
        {
            iii=0;
            count++;
        }
    }
    count=0;
    int_midd();
    while(count<10)
    {
        OSTimeDlyHMSM(0,0,0,500);
        if(strstr(midd,"OK+C") != 0)
        {
            for(i=0; i<3; i++)
            {
                OSTimeDlyHMSM(0,0,0,500);
                OSTimeDlyHMSM(0,0,0,500);
            }
            for(i=0; i<211; i++)		 //���Ͳ�������
                UART3_SendByte(0xdf);
            count=255;
        }
        else
        {
            count++;
        }
    }
    count=0;
}

/*********************************************************************************************************
* Function name:   Research
* Descriptions:    �����������
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
void Research(void)
{
    uint8 slen;
    uint8 i;
    ClrDisBuf();
    slen = StringLen(NextPrompt[30+NePrOffset]);
    DisCList16((DIS_AREA_X-slen*8)/2,100,NextPrompt[30+NePrOffset]);//���Ժ�..
    LcdUpdata(DisBuf);
    OPEN_Print(1);                      //������ģ��
    PrintOnClose = 1;
    /* Enable UART Rx interrupt */
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART3, UART_INTCFG_RBR, ENABLE);
    /* Enable UART line status interrupt */
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART3, UART_INTCFG_THRE, ENABLE);
    Delay100uS(500);//5 ms

    RST_Print(1);  	//�Ͽ���������
    Delay100uS(150000);	//1.5s
    RST_Print(0);
    for(i=0; i<8; i++)
    {
        Delay100uS(50000);//0.5
    }

    bt_test();		//��������豸
    RST_Print(1);  	//�Ͽ���������
    OSTimeDlyHMSM(0, 0, 0, 500);
    OSTimeDlyHMSM(0, 0, 1, 0);
    RST_Print(0);
    /* DisAlbe UART Rx interrupt */
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART3, UART_INTCFG_RBR, DISABLE);
    /* Enable UART line status interrupt */
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART3, UART_INTCFG_THRE, DISABLE);

}
