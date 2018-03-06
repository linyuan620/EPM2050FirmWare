#include"FPGA_Contorl.h"
#ifdef CH320
#define AREA	  1.02
#else
#define AREA	  1.539
#endif
#define HIGH 1.3
int8 errtimemax 	= 5;
int8 errtimemax0 	= 5;
int8 UseFor 	= 	1;//������;(����) 0  PM10 1  PM2.5  2 TSP
int8 UseFor0 	= 	1;
uint32 ComminiStyle,ComminiStyle0;//0 ���ڣ�1����
float ErrorDat;
uint32 cheak;
uint8 volatile ACKFlg = 0xff,CUpdateFlg;
uint8 C14MotMoveTime = 3;
uint8 volatile ADBufferDatReadFlg2, ADBufferDatReadFlg,ADRead1,ADRead2;
uint8 volatile TTLReciveFlg;
volatile float Parg_I,Parg_I_ReCnt,Parg_x,Parg_x_Recnt,Parg_m,Parg_v_BK,Parg_v_GK,Parg_I0,
         Parg_I0_Upload,Parg_I0_ReCnt,Parg_PM25_GK,Parg_PM25,Parg_u;
CNT_RECORD Cnt_Record[200];
volatile float PM25_GS_M=0, PM10_GS_M=0,TSP_GS_M=0;
volatile float PM25_GS_H=0,PM10_GS_H=0,TSP_GS_H=0;
volatile uint16 GS_Cnt=0;

float Parg_k =1.25,Parg_k0 = 1.25;
float TSP_GS_k=14.3,PM10_GS_k=9.2,PM25_GS_k=4.1;//��ɢ����ϵ��
fp32 GS_Cycle;

char Parg_b=0,Parg_b0 = 0;
uint8 kaijiTime;
uint8 FilterErrTimes;
static uint8_t Send_Bytes=7;
uint8_t Sent_Buffer[50]= {'a','b','c','d'}; //���ͻ�����
uint8 DACChanMap,ADreadErr,ADreadErr2;
uint8 ADStatBusyFlg;
uint16	DACChanValue[8];
volatile uint8	ADBufferStart1,ADBufferStart2,ReadBegain,ReadBuffer,CheakDat;
volatile uint16	 ReadNum;
int16  ADDat[16];
int16  ADDat_1[8];
int16  ADDat_2[8];
float PargRealI[16];
float PargRealTv2[16];
float PargRealTv1[16];//����
float MeasureTs,MeasureTe,MeasureTsS,MeasureTsE;
int16  ADDat2;
float addat[16];
uint32 NumOfI;
volatile uint8 TTLOutPut[8]= {0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03};
uint8 CmErrorState[16];//={1,2,3,4,5};
uint8  TTLDat;
uint8 StopFlg,StartKeyFlg,ADBufeerCleanFlg;
uint8  StatuErrorFlg;
uint8 TimeType,MeasureStepFlg;	 //����ģʽflg
float Vcc_5;
int  CountReadFlg,CountReaderr,CountStartFlg,CountTimer;
uint32 CountNow,CountAver	,CountTotal ,CountTimes  ;
uint8 ADRangesel =1;
#define CODE_R  1.8
#define PAPER_MOVE_TIME (2*5000)/(3.14*CODE_R)       //60�� ����50��	
uint32 PaperTime;
uint8 PaperOverFlg;

#define LEN  4   //�ƶ�����ƽ���ĸ���+2=SHIFT<<2+2
#define SHIFT 2  //^SHIFT
//uint8 pmax,pmin;//��¼���ݱ������ֵ����Сֵ��λ��,
//��һ������ݲɼ�ϵͳ��,���ݵĳ���>=8,
//�����ָ���¼������ֱ�Ӽ�¼���ֵ����Сֵ
uint16 AvrT[LEN] = {25,25,25,25};
uint16 AvrP[LEN] = {101,101,101,101};
uint16 AvrL[LEN];
uint16 AvrHR[LEN] = {30,30,30,30};
uint8 pdata_T;    //�ƶ�ָ��
uint8 pdata_P;    //�ƶ�ָ��
uint8 pdata_L;    //�ƶ�ָ��
uint8 CountCodeSendFlg,pdata_HR;    //�ƶ�ָ��
uint16  EEORStuationOld;

float TV_GK,TV_BK;
uint8 CheackDat(volatile uint8 *dat)
{
    uint8 tmpc = 0,step = 0;
    int32 i,j;
    int32 ADall=0 ;
    //�������Ҫ�����̫����ô�ͻ�һ�𷢹�����

    OSTimeDlyHMSM(0, 0, 0,50);
    for(i = 0; i<UART_RING_BUFSIZE; i++)
    {
        tmpc = dat[i];
        switch(step)
        {
        case 0:
            if(START==tmpc)
            {
                step=1;
            }
            break;	//zheng  kai shi.
        case 1:   //�������
            switch(tmpc)
            {
            case ACK:
                step = 2;
                break;
            case ENDFRAME:
                break;
            case ADCCTRLS:
                break;
            case ADCCTRLM:
                break;
            case ADCCTRLC:
                break;
            case DACCTRLM:
                break;
            case DACCTRLC:
                break;
            case ADDACTRL:
                break;

            case TTLCTRLI:
                break;
            case TTLCTRLO:
                break;
            case SPICTRLI:
                break;
            case SPICTRLO:
                break;
            case ADBUFFCTRL:
                step = 4;
                break;
            case ADBUFFREAD:
                break;
            case ADBUFFRLS:
                break;
            case USERBUFF:
                break;

            case USERWRITE:
                break;
            case USERREAD:
                break;
            case ADDASELFTEST:
                break;
            case COUNT_READ:
                step = 8;
                break;
            default :
                step = 0;
                break;
            }
            break;
            //ACK
        case 2:
            switch (tmpc)
            {
            case ERR_OK:
                if(ADBufeerCleanFlg==1)
                {
                    ADBufeerCleanFlg = 0;
                }
                if(CountCodeSendFlg==1)
                {
                    CountCodeSendFlg = 0;
                }
                break;
            case ERR1: //ͨ������Ч
                break;

            case ERR2: // �������
                break;
            case ERR3: //��żУ��λ��׼ȷ
                break;
            case ERR4: //������δ����
                break;
            case ERR5: //���ݻ�����Ȳ���ȷ
                break;
            case ERR6: //����δ��λ
                break;
            case ERR7: // ����������ݳ���
                break;
            case ERR8: // �޷���Ӧ������ ͨ��0--7æ
                break;
            case ERR9: // 8--15 æ
//						ADStatBusyFlg = 1;
                break;
            case ERR10:// 0--15æ
//						ADStatBusyFlg = 1;
                break;
            case ERR11:// dacæ
                break;
            case ERR12://  SPIæ
                break;
            case ERR13:// ��ֽ���˻�û��
                EeorMear |= 0x20;
                FilterErrTimes++;
                if(FilterErrTimes>errtimemax)
                {
                    StopFlg = 1;
                    FilterRemain = 0;
                    PaperTime = PAPER_MOVE_TIME;
                }
                break;
            default:
                //����������
                break;
            }
            if(tmpc <0x11)
                CmErrorState[tmpc] = tmpc;
            step = 3;
            break;
            //֡����
        case 3 :
            if(tmpc == STOP )
            {
                step = 0;//֡����������
                if(ADBufferDatReadFlg == 1)
                {   ADBufferDatReadFlg = 0;
                    step=5;
                }
                if(ADBufferDatReadFlg2==1)
                {
                    ADBufferDatReadFlg2=0;
                    step=9;
                }
//					if(ADBufferDatReadFlg2==1)
//					{
//					 ADBufferDatReadFlg2=0;
//					 step=7;
//					}
                if(TTLReciveFlg == 1)
                {   TTLReciveFlg = 0;
                    step=6;
                }
            }
            else
            {
                step = 0;
                //wrong
            }
            break;
            //ADBUFFCTRL
        case 4:
            ADBufferStart1 =  dat[i+1];
            ADBufferStart2 =  dat[i+2];
            i= i+2;
            step = 3;
            break;
            //AD���ݲɼ�
        case 5:
            for(j=0; j<8; j++)
            {
                ADDat_1[j] =(int16)(dat[i]*256+dat[i+1]);
                i=i+2;
            }
            return 0;
//				break;
            //TTL���ݲɼ�
        case 6:
            TTLDat = dat[i];
            step = 0;
            break;
            //AD���ݶ���ɼ�
        case 7:
            for(j=0; j<50; j++)
            {
                ADDat2 =(int16)(dat[i]*256+dat[i+1]);
                i= i+2;
                ADall = ADall+	ADDat2;
            }
            ADDat2 =  (int16)(ADall/50);
            return 0;
//				break;
            //��ܼ���ֵ
        case 8:
            if(tmpc == 0x0b)//���ݳ�����ȷ
            {
                if(CountReadFlg == 1)
                {
                    CountReadFlg = 0;
                }
                CountNow = (dat[i+1]<<24)|(dat[i+2]<<16)|(dat[i+3]<<8)|(dat[i+4]);
                CountTotal =(dat[i+4+1]<<24)|(dat[i+4+2]<<16)|(dat[i+4+3]<<8)|(dat[i+4+4]);
                //�����Ǵ�1�ο�ʼ��
                CountTimes =(dat[i+8+1]<<24)|(dat[i+8+2]<<16)|(dat[i+8+3]<<8)|(dat[i+8+4]);
//					if(CountTimes<199)
//					{
//						Cnt_Record[CountTimes].I = CountNow;
//					}
//					else
//					{
//						Cnt_Record[CountTimes].I = -1;
//					}
            }
            return 0;
            //AD���ݲɼ�
        case 9:
            for(j=0; j<8; j++)
            {
                ADDat_2[j] =(int16)(dat[i]*256+dat[i+1]);
                i=i+2;
            }
            return 0;

        default :
            step = 0;
            break;
        }
    }
    return 0;
}

void CleanSent_Buffer()
{
    int i;
    for(i = 0; i<50; i++)
    {
        Sent_Buffer[i] = 0;
    }
}

void CleanRbrx()
{
    int i;
    for(i = 0; i<UART_RING_BUFSIZE; i++)
    {
        rb.rx[i] = 0;
    }
    __BUF_RESET(rb.rx_head);
    __BUF_RESET(rb.rx_tail);
}

//��������
void Rest()
{   //CPU_INT08U   err;
    CleanSent_Buffer();
    Sent_Buffer[0] = START;
    Sent_Buffer[1] = FPGA_RESET;
    Sent_Buffer[2] = 0;
    Sent_Buffer[3] = STOP;
    Send_Bytes = 	4;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

//Ӧ��֡
void ACKpro(uint8 errorcode)
{   //CPU_INT08U   err;
    CleanSent_Buffer();

    Sent_Buffer[0] = START;
    Sent_Buffer[1] = ACK;
    Sent_Buffer[2] = errorcode;
    Sent_Buffer[3] = STOP;
    Send_Bytes = 	4;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

//����֡
void ENDpro()
{   //CPU_INT08U   err;
    CleanSent_Buffer();

    Sent_Buffer[0] = START;
    Sent_Buffer[1] = ENDFRAME;
    Sent_Buffer[2] = 0x00;
    Sent_Buffer[3] = STOP;
    Send_Bytes = 	4;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

//ת�庯��
void DatChange(uint8 *BufferLocal,uint8 Dat)
{
    switch(Dat)
    {
    case 0xef:
        *BufferLocal++ = 0xec;
        *BufferLocal++ = 0xc0;
        break;
    case 0xec:
        *BufferLocal++ = 0xec;
        *BufferLocal++ = 0x75;
        break;
    default:
        *BufferLocal++ = 0x00;
        *BufferLocal++ = Dat;
        break;
    }
}

//��ת������
void Rotary(uint32 Distance)
{   //CPU_INT08U   err;
    uint8 Dat;
    uint8 length=0;
    CleanSent_Buffer();
    Dat = (uint8)(Distance>>24);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(Distance>>16);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(Distance>>8);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(Distance);
    DatChange(&Sent_Buffer[length+3],Dat);

    length= length +2;

    Sent_Buffer[0] = START;
    Sent_Buffer[1] = ROTARY;
    Sent_Buffer[2] = length;
    Sent_Buffer[length +3] = STOP;
    Send_Bytes = 	length+4;
    EeorMear &= 0xdf;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

//MOIndex �����λʱ���Ʒ���ġ�
void MotorCtrl(uint8 MOIndex,uint32 Distance)//�ú������ڿ��Ʋ��������������������ֱ����� ����Ĳ����޹ؽ�Ҫ��
{   //CPU_INT08U   err;
    uint8 Dat;
    uint8 length=0;	//startVelocity = EA60	200HZ	maxVelocity  2EE0 1K
    // 0X0010 16US    31������ �Ӽ���ʱ��Ϊ16*31US
    uint16 StartVelocity = 0xea60,MaxVelocity = 0x2ee0,TaccStep = 0x10;

    CleanSent_Buffer();

    Dat = (uint8)(Distance>>24);
    DatChange(&Sent_Buffer[length+4],Dat);
    length= length +2;

    Dat = (uint8)(Distance>>16);
    DatChange(&Sent_Buffer[length+4],Dat);
    length= length +2;

    Dat = (uint8)(Distance>>8);
    DatChange(&Sent_Buffer[length+4],Dat);
    length= length +2;

    Dat = (uint8)(Distance);
    DatChange(&Sent_Buffer[length+4],Dat);
    length= length +2;

    Dat = (uint8)(StartVelocity>>8);
    DatChange(&Sent_Buffer[length+4],Dat);
    length= length +2;

    Dat = (uint8)(StartVelocity);
    DatChange(&Sent_Buffer[length+4],Dat);
    length= length +2;

    Dat = (uint8)(MaxVelocity>>8);
    DatChange(&Sent_Buffer[length+4],Dat);
    length= length +2;

    Dat = (uint8)(MaxVelocity);
    DatChange(&Sent_Buffer[length+4],Dat);
    length= length +2;

    Dat = (uint8)(TaccStep>>8);
    DatChange(&Sent_Buffer[length+4],Dat);
    length= length +2;

    Dat = (uint8)(TaccStep);
    DatChange(&Sent_Buffer[length+4],Dat);
    length= length +2;

    Sent_Buffer[0] = START;
    Sent_Buffer[1] = MOCTRL;
    Sent_Buffer[2] = length+1;
    Sent_Buffer[3] = MOIndex;

    Sent_Buffer[length +4] = STOP;
    Send_Bytes = 	length+5;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}


void CommunicationSet(uint8 style)
{   //CPU_INT08U   err;
    CleanSent_Buffer();

    Sent_Buffer[0] = START;
    Sent_Buffer[1] = BUSSEL;
    Sent_Buffer[2] = 1;		 //length

    Sent_Buffer[3] = style;
    Sent_Buffer[4] = STOP;
    Send_Bytes = 	5;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

//AD��ͨ������ɼ�
void ADMCSSampling(uint16 ChanMap,uint16 StartTime1,uint8 AvgPoint1)
{   //CPU_INT08U   err;
    uint8 Dat;
    uint8 length=0;

    CleanSent_Buffer();

    Dat = (uint8)(ChanMap>>8);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(ChanMap);
    DatChange(&Sent_Buffer[length+3],Dat);

    length= length +2;


    Dat = (uint8)(StartTime1>>8);
    DatChange(&Sent_Buffer[length+3],Dat);

    length= length +2;

    Dat = (uint8)(StartTime1);
    DatChange(&Sent_Buffer[length+3],Dat);

    length= length +2;

    Dat = AvgPoint1;
    DatChange(&Sent_Buffer[length+3],Dat);

    length= length +2;

    Sent_Buffer[0] = START;
    Sent_Buffer[1] = ADCCTRLS;
    Sent_Buffer[2] = length;
    Sent_Buffer[length +3] = STOP;
    Send_Bytes = 	length+4;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

////AD��ͨ�����ɼ�
void ADMCMSampling(uint16 ChanMap,uint16 StartTime1,uint16 interval ,uint16 NumOfSamples)
{   //CPU_INT08U   err;
    uint8 Dat;
    uint8 length=0;
    CleanSent_Buffer();
    Dat = (uint8)(ChanMap>>8);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;
    Dat = (uint8)(ChanMap);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2 ;
    Dat = StartTime1>>8;
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(StartTime1);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = interval>>8;
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(interval);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = NumOfSamples>>8;
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(NumOfSamples);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;
    Sent_Buffer[0] = START;
    Sent_Buffer[1] = ADCCTRLM;
    Sent_Buffer[2] = length;
    Sent_Buffer[length +3] = STOP;
    Send_Bytes = 	length+4;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

//DA ��ͨ����������
void DAMCSOutPutSetOne(uint8 DACChan,float DACValue)
{

    int32 i;
    DACChanMap = DACChanMap | (1<<DACChan);
    for(i=0; i<NUN_DA; i++)
    {
        if(DACChan == DAStructTable[i].DAChannel)
        {
            DACValue = DACValue*DACONVERT/DAStructTable[i].DAGain;
            break ;
        }
    }
    DACChanValue[DACChan] = (uint16)(DACValue);
}

//da ��ͨ��������� ȫ�������
void DAMCSOutPutAll()
{
    //CPU_INT08U   err;
    int32 Dat,i;
    uint8 length=0;
    CleanSent_Buffer();

    Dat = DACChanMap ;
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;
    for(i = 0; i<8; i++)
    {
        Dat = (uint8)(DACChanValue[i]>>8);
        DatChange(&Sent_Buffer[length+3],Dat);
        length= length +2;
        Dat = (uint8)(DACChanValue[i]);
        DatChange(&Sent_Buffer[length+3],Dat);
        length= length +2;
    }
    Sent_Buffer[0] = START;
    Sent_Buffer[1] = DACCTRLS;
    Sent_Buffer[2] = length;
    Sent_Buffer[length +3] = STOP;
    Send_Bytes = 	length+4;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

//DA��ͨ��������
void  DASCMOutput(uint8 DAChanIndex,uint16 StartValue,uint16 Interval,uint16 Step,uint16 StopValue)
{
    //CPU_INT08U   err;
    uint8 Dat;
    uint8 length=0;
    CleanSent_Buffer();

    Dat = (uint8)(DAChanIndex);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(StartValue>>8);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(StartValue);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2 ;

    Dat = Interval>>8;
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(Interval);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = Step>>8;
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(Step);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = StopValue>>8;
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(StopValue);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;
    Sent_Buffer[0] = START;
    Sent_Buffer[1] = DACCTRLM;
    Sent_Buffer[2] = length;
    Sent_Buffer[length +3] = STOP;
    Send_Bytes = 	length+4;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

//DA��ͨ�����������
void DASCCOutput(uint8 DAChanIndex,uint16 StartValue,uint16 Interval,uint16 Step,uint16 StopValue)
{
    //CPU_INT08U   err;
    uint8 Dat;
    uint8 length=0;
    CleanSent_Buffer();
    Dat = (uint8)(DAChanIndex);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(StartValue>>8);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(StartValue);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = Interval>>8;
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(Interval);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = Step>>8;
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(Step);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = StopValue>>8;
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(StopValue);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;
    Sent_Buffer[0] = START;
    Sent_Buffer[1] = DACCTRLC;
    Sent_Buffer[2] = length;
    Sent_Buffer[length +3] = STOP;
    Send_Bytes = 	length+4;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}


//DA��ADͬ������
//void DAC_ADC_Synchronize ();


//TTL����
void TTLInput (uint8 ChanIndex)
{
    //CPU_INT08U   err;
    CleanSent_Buffer();
    Sent_Buffer[0] = START;
    Sent_Buffer[1] = TTLCTRLI;
    Sent_Buffer[2] = 1;
    Sent_Buffer[3] = ChanIndex;
    Sent_Buffer[4] = STOP;
    Send_Bytes = 	5;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

/***********
name:
function:
*************/
void TTLOutput (uint8 ChanIndex,uint8 action,uint8 Tongdao)
{
    //CPU_INT08U   err;
    int32 i;
    CleanSent_Buffer();
    for(i=0; i<8; i++)
    {
        TTLOutPut[i] = 0x03;
    }
    TTLOutPut[Tongdao] = action;

    for(i=0; i<8; i++)
    {
        Sent_Buffer[i+4] = TTLOutPut[i];
    }
    Sent_Buffer[0] = START;
    Sent_Buffer[1] = TTLCTRLO;
    Sent_Buffer[2] = 9;
    Sent_Buffer[3] = ChanIndex;
    Sent_Buffer[12] = STOP;

    Send_Bytes = 	13;

    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

/***********
name:
function:
*************/
void SPIIput(uint8 ChanIndex,uint8 BufferIndex)
{
    //CPU_INT08U   err;
    CleanSent_Buffer();
    Sent_Buffer[0] = START;
    Sent_Buffer[1] = SPICTRLI;
    Sent_Buffer[2] = 2;
    Sent_Buffer[3] = ChanIndex;
    Sent_Buffer[4] = BufferIndex;
    Sent_Buffer[5] = STOP;
    Send_Bytes = 	6;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}


/***********
name:
function:
*************/
void SPIoutput(uint8 ChanIndex,uint8 BufferIndex,uint8 FreqFlag,uint16 SizeFlag)
{
    //CPU_INT08U   err;
    CleanSent_Buffer();
    Sent_Buffer[0] = START;
    Sent_Buffer[1] = SPICTRLO;
    Sent_Buffer[2] = 7;
    Sent_Buffer[3] = ChanIndex;
    Sent_Buffer[4] = BufferIndex;
    Sent_Buffer[5] = FreqFlag;

    switch(SizeFlag>>8)
    {
    case 0xef:
        Sent_Buffer[6] = 0xec;
        Sent_Buffer[7] = 0xc0;
        break;
    case 0xec:
        Sent_Buffer[6] = 0xec;
        Sent_Buffer[7] = 0x75;
        break;
    default:
        Sent_Buffer[6] = 0;
        Sent_Buffer[7] = (SizeFlag>>8);
        break;
    }
    switch((uint8)(SizeFlag))
    {
    case 0xef:
        Sent_Buffer[8] = 0xec;
        Sent_Buffer[9] = 0xc0;
        break;
    case 0xec:
        Sent_Buffer[8] = 0xec;
        Sent_Buffer[9] = 0x75;
        break;
    default:
        Sent_Buffer[8] = 0;
        Sent_Buffer[9] = (uint8)(SizeFlag);
        break;
    }

    Sent_Buffer[10] = STOP;
    Send_Bytes = 	11;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

/***********
name:
function:AD buffer״̬��ѯ
*************/
void ADBufferStatus(uint16 ChanMap)
{

    //CPU_INT08U   err;
    uint8 Dat;
    uint8 length=0;
    CleanSent_Buffer();

    Dat = (uint8)(ChanMap>>8);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(ChanMap);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Sent_Buffer[0] = START;
    Sent_Buffer[1] = ADBUFFCTRL;
    Sent_Buffer[2] = length;
    Sent_Buffer[length +3] = STOP;
    Send_Bytes = 	length+4;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

/***********
name:
function: AD�����
*************/
void ADBufferRead(uint16 ChanMap,uint8 RdSel1,uint8 RdSel2)
{
    //CPU_INT08U   err;
    uint8 Dat;
    uint8 length=0;
    CleanSent_Buffer();

    Dat = (uint8)(ChanMap>>8);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;

    Dat = (uint8)(ChanMap);
    DatChange(&Sent_Buffer[length+3],Dat);
    length= length +2;
    Sent_Buffer[length+3] = RdSel2;
    Sent_Buffer[length+4] = RdSel1;
    length = length +2;
    Sent_Buffer[0] = START;
    Sent_Buffer[1] = ADBUFFREAD;
    Sent_Buffer[2] = length;
    Sent_Buffer[length +3] = STOP;
    Send_Bytes = 	length+4;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

/***********
name:
function:
*************/
void ADBufferRLS(uint8 RLScode)
{
    //CPU_INT08U   err;
    CleanSent_Buffer();
    Sent_Buffer[0] = START;
    Sent_Buffer[1] = ADBUFFRLS;
    Sent_Buffer[2] = RLScode;
    Sent_Buffer[3] = STOP;
    Send_Bytes = 	4;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    ADBufeerCleanFlg = 1;
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

void ADRangeSel(uint8 Rangecode)
{
    //CPU_INT08U   err;
    CleanSent_Buffer();
    Sent_Buffer[0] = START;
    Sent_Buffer[1] = ADRANGESEL;
    Sent_Buffer[2] = 1;
    Sent_Buffer[3] = Rangecode;
    Sent_Buffer[4] = STOP;
    Send_Bytes = 	5;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

void CountCtrl(uint8 action)
{
    //CPU_INT08U   err;
    CleanSent_Buffer();
    Sent_Buffer[0] = START;
    Sent_Buffer[1] = CNT;
    Sent_Buffer[2] = 2;
    Sent_Buffer[3] = action;
    Sent_Buffer[4] = 0x0a;
    Sent_Buffer[5] = STOP;

    Send_Bytes = 	6;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    CountCodeSendFlg = 1;
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

void CountRead(void)
{
    //CPU_INT08U   err;
    CleanSent_Buffer();
    Sent_Buffer[0] = START;
    Sent_Buffer[1] = COUNT_READ;
    Sent_Buffer[2] = 0;
    Sent_Buffer[3] = STOP;
    Send_Bytes = 	4;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    CountReadFlg = 1;
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}


/***********
name:
function:
*************/
void InquiryUserBufferState(uint8 BuffIndex)
{
    //CPU_INT08U   err;
    CleanSent_Buffer();
    Sent_Buffer[0] = START;
    Sent_Buffer[1] = USERBUFF;
    Sent_Buffer[2] = 1;
    Sent_Buffer[3] = BuffIndex;
    Sent_Buffer[4] = STOP;

    Send_Bytes = 	5;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

/***********
name:
function:
*************/
void  ReadUserBuffer(uint8 BuffIndex , uint16 DatLength)
{
    //CPU_INT08U   err;
    uint8 Dat;
    uint8 length=0;
    CleanSent_Buffer();
    Dat = (uint8)(DatLength>>8);
    DatChange(&Sent_Buffer[length+4],Dat);
    length= length +2;

    Dat = (uint8)(DatLength);
    DatChange(&Sent_Buffer[length+4],Dat);
    length= length +2;

    Sent_Buffer[0] = START;
    Sent_Buffer[1] = USERREAD;
    Sent_Buffer[2] = length+1;
    Sent_Buffer[3] = BuffIndex;

    Sent_Buffer[length +4] = STOP;
    Send_Bytes = 	length+5;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

/***********
name:
function:
*************/
void WriteUserBuffer (uint8 ChanIndex,uint16 DatLength)
{
    //CPU_INT08U   err;
    uint8 Dat;
    uint8 length=0;
    CleanSent_Buffer();
    Dat = (uint8)(DatLength>>8);
    DatChange(&Sent_Buffer[length+4],Dat);
    length= length +2;

    Dat = (uint8)(DatLength);
    DatChange(&Sent_Buffer[length+4],Dat);
    length= length +2;

    Sent_Buffer[0] = START;
    Sent_Buffer[1] = USERWRITE;
    Sent_Buffer[2] = length+1;
    Sent_Buffer[3] = ChanIndex;
    Sent_Buffer[length +4] = STOP;
    Send_Bytes = 	length+5;
    //OSMutexPend(OSUart0UsedMutex,1,&err);
    CleanRbrx();
    UART_Send(LPC_UART0, Sent_Buffer, Send_Bytes,BLOCKING);
    CheackDat(rb.rx);
}

uint16 AverT(float _data)
{
    /****************************/
    /* �ڵ��ô��ӳ���ǰ����� */
    /* pdata,datas[]����,  */
    /* pmax,pmin���г�ʼ��  */
    /****************************/
    uint8 i;
    uint16 average=0;  //���㣬��������ƽ��ֵ
    pdata_T=(pdata_T+1)%LEN; //ָ���±���0��LEN-1�ϻ���
    AvrT[pdata_T]=_data;  //�����������ݴ������ݱ���
    for(i=0; i<LEN; i++)
        average+=AvrT[i]; //�����������ܺ�
    return (average>>SHIFT);    //������ƽ��ֵ
}

uint16 AverP(float _data)
{
    /****************************/
    /* �ڵ��ô��ӳ���ǰ����� */
    /* pdata,datas[]����,  */
    /* pmax,pmin���г�ʼ��  */
    /****************************/
    uint8 i;
    uint16 average=0;  //���㣬��������ƽ��ֵ
    pdata_P=(pdata_P+1)%LEN; //ָ���±���0��LEN-1�ϻ���
    AvrP[pdata_P]=_data;  //�����������ݴ������ݱ���
    for(i=0; i<LEN; i++)
        average+=AvrP[i]; //�����������ܺ�
    return (average>>2);    //������ƽ��ֵ
}

uint16 AverL(float _data)
{
    /****************************/
    /* �ڵ��ô��ӳ���ǰ����� */
    /* pdata,datas[]����,  */
    /* pmax,pmin���г�ʼ��  */
    /****************************/
    uint8 i;
    uint16 average=0;  //���㣬��������ƽ��ֵ
    pdata_L=(pdata_L+1)%LEN; //ָ���±���0��LEN-1�ϻ���
    AvrL[pdata_L]=_data;  //�����������ݴ������ݱ���
    for(i=0; i<LEN; i++)
        average+=AvrL[i]; //�����������ܺ�
    return (average>>2);    //������ƽ��ֵ
}

uint16 AverHR(float _data)
{
    /****************************/
    /* �ڵ��ô��ӳ���ǰ����� */
    /* pdata,datas[]����,  */
    /* pmax,pmin���г�ʼ��  */
    /****************************/
    uint8 i;
    uint16 average=0;  //���㣬��������ƽ��ֵ
    pdata_HR=(pdata_HR+1)%LEN; //ָ���±���0��LEN-1�ϻ���
    AvrHR[pdata_HR]=_data;  //�����������ݴ������ݱ���
    for(i=0; i<LEN; i++)
        average+=AvrHR[i]; //�����������ܺ�
    return (average>>2);    //������ƽ��ֵ
}

void ADget(void)
{
    CPU_INT08U   err;
    OSMutexPend(OSUart0UsedMutex,0,&err);
    if(ADBufferStart2 != 0x03)
    {
        ADMCSSampling(0x00ff,0x01,2); //AD��ͨ������ɼ�
    }
    ADBufferStart2 = 0;
    ADBufferStatus(0x00ff);
    if(0!=ADBufferStart2)
    {   ADRead1 = 1;
        ADBufferDatReadFlg = 1;
        ADBufferRead(0x00ff,1,1);	// ad2 ad5  ad6  ad 3Ҫ����2��  ad1 ad7	 ad8 ����  ad4Ҫ���� 4.92��
        ADBufferRLS(0x02);	//���buffer
        while(ADBufeerCleanFlg&&(ADreadErr<5))
        {
            ADreadErr++;
            ADBufferRLS(0x02);	//���buffer
        }
    }
    if(ADBufferStart1 != 0x03)
    {
        ADMCSSampling(0xff00,0x01,2);
    }
    ADBufferStart1 = 0;
    ADBufferStatus(0xff00);
    if(0!=ADBufferStart1)
    {   ADRead2	 = 1;
        ADBufferDatReadFlg2 = 1;
        ADBufferRead(0xFF00,1,1);	// ad2 ad5  ad6  ad 3Ҫ����2��  ad1 ad7	 ad8 ����  ad4Ҫ���� 4.92��
        ADBufferRLS(0x02);	//���buffer
        while(ADBufeerCleanFlg&&(ADreadErr<5))
        {
            ADreadErr++;
            ADBufferRLS(0x02);	//���buffer
        }
    }
    OSMutexPost(OSUart0UsedMutex);
}

void getShishiLiuliang(void)
{
    uint8 err;
    int32 i;
    float dat;
    ADget();
    if((1==ADRead1)&& (1==ADRead2))
    {
        ADRead1 = 0;
        ADRead2 = 0;
        for(i = 0; i<8; i++)
        {
            ADDat[i] = ADDat_1[i];
        }
        for(i = 0; i<8; i++)
        {
            ADDat[8+i] = ADDat_2[i];
        }
        //�ƶ�ƽ��
        //ADDat[AD_LIULIANG] = AverL(ADDat[AD_LIULIANG]);
//		ADDat[AD_SHIDU] = AverHR(ADDat[AD_SHIDU]);
//		ADDat[AD_QIYA] = AverP(ADDat[AD_QIYA]);
//		ADDat[AD_WENDU1] = AverT(ADDat[AD_WENDU1]);
        //���������ѹֵ
        if(ADRangesel == 1)
        {
            for(i=0; i<16; i++)
            {
                addat[i] = 10.0*((int16)ADDat[i])/(32768);  // ADDat[i]*0.00015198;	   //Vin = 10*code*2.5/(32768*ref)	ref = 2.5;
            }
        }
        else
        {
            for(i=0; i<16; i++)
            {
                addat[i] = 5.0*((int16)ADDat[i])/(32768);//ADDat[i]*0.00030396;	   //Vin = 5*code*2.5/(32768*ref)	ref = 2.5;
            }
        }
//		Vcc_5 = GetADValue(VCC_5);
//		LoadK(CGQ_QIYA,(1/(Vcc_5*0.009)));//������ѹ
//		//��Դ��ѹ
//		PowerV1=GetADValue(VCC_5);             //   +5v
//		PowerV2=GetADValue(VCC_12);             //   +12v
//		PowerV3=GetADValue(VSS_12);             //   -12v
//		PowerV4=GetADValue(VCC_24);             //   +24v

//		TCQWenDuT4 		= 	GetMeasureValue(CGQ_TCQ_WENDU,AD_TCQ_WENDU);    //����̽�����¶ȴ����� δ������ƽ��

//		switch(SensorSel)
//		{
//			case 1:
//				CYCShiDu		=	GetMeasureValue(CGQ_CYK_SHIDU_XY,AD_CYK_SHIDU); //����������ʪ�ȴ����� δ������ƽ��
//				WenDuT1 		=	GetMeasureValue(CGQ_WENDU1_XY,AD_WENDU1);		//�������¶�ֵ
//				break;
//			default:
//				CYCShiDu		=	GetMeasureValue(CGQ_CYK_SHIDU_KL,AD_CYK_SHIDU); //����������ʪ�ȴ����� δ������ƽ��
//				WenDuT1 		=	GetMeasureValue(CGQ_WENDU1_KL,AD_WENDU1);		//�������¶�ֵ
//			break;
//		}

//		WenDuT2 		=  	GetMeasureValue(CGQ_WENDU2,AD_WENDU2);          //DHS�¶�ֵ
//		WenDuT3 		=  	GetMeasureValue(CGQ_WENDU3,AD_WENDU3);		    //���Բ��¶�
//		HumiditySample 	=  	GetMeasureValue(CGQ_SHIDU,AD_SHIDU);            //���Բ�ʪ��
//		QiYa 			=	GetMeasureValue(CGQ_QIYA,AD_QIYA);              //��ѹ
//		LiuLiang_BK		=	GetMeasureValue(CGQ_LIULIANG,AD_LIULIANG);      //����
        LiuLiang_BK		=	GetMeasureValue(CGQ_LIULIANG_HONEYWELL,AD_LIULIANG);      //����������
        dat = addat[AD_LIULIANG]*ADStructTable[AD_LIULIANG].Gain;//���ѹֵ
        printf("LiuLiang_BK = %f\t",LiuLiang_BK);
        printf("LiuLiang_ad = %f\r\n",dat);
// 		if(LiuLiang_BK<0.3)
// 		{
// 			LiuLiang_BK = 0;
// 		}
        //��������=�������*���ѹ��/��273+����¶�(��))*��273+�����¶�(��))/����ѹ��
//		LiuLiang_GK=LiuLiang_BK*101.325*(273.15+TDAIDIN)/(273.15*QiYa);
    }
}


void 	NomalMeasure(void)
{
    uint8 err;
    int32 i;
    ADget();
    if((1==ADRead1)&& (1==ADRead2))
    {
        kaijiTime++;
        if(kaijiTime > 20)
        {
            kaijiTime = 20;
        }
        ADRead1 = 0;
        ADRead2 = 0;
        for(i = 0; i<8; i++)
        {
            ADDat[i] = ADDat_1[i];
        }
        for(i = 0; i<8; i++)
        {
            ADDat[8+i] = ADDat_2[i];
        }
        //�ƶ�ƽ��
        ADDat[AD_LIULIANG] = AverL(ADDat[AD_LIULIANG]);
        ADDat[AD_SHIDU] = AverHR(ADDat[AD_SHIDU]);
        ADDat[AD_QIYA] = AverP(ADDat[AD_QIYA]);
        ADDat[AD_WENDU1] = AverT(ADDat[AD_WENDU1]);
        //���������ѹֵ
        if(ADRangesel == 1)
        {
            for(i=0; i<16; i++)
            {
                addat[i] = 10.0*((int16)ADDat[i])/(32768);  // ADDat[i]*0.00015198;	   //Vin = 10*code*2.5/(32768*ref)	ref = 2.5;
            }
        }
        else
        {
            for(i=0; i<16; i++)
            {
                addat[i] = 5.0*((int16)ADDat[i])/(32768);//ADDat[i]*0.00030396;	   //Vin = 5*code*2.5/(32768*ref)	ref = 2.5;
            }
        }
        //Vcc_5 = GetADValue(VCC_5);

        //LoadK(CGQ_QIYA,(1/(Vcc_5*0.009)));//������ѹ
        //��Դ��ѹ
        PowerV1=GetADValue(VCC_5);             //   +5v
        PowerV2=GetADValue(VCC_12);             //   +12v
        PowerV3=GetADValue(VSS_12);             //   -12v
        PowerV4=GetADValue(VCC_24);             //   +24v

        TCQWenDuT4 		= 	GetMeasureValue(CGQ_TCQ_WENDU,AD_TCQ_WENDU);    //����̽�����¶ȴ����� δ������ƽ��

        switch(SensorSel)
        {
        case 1:
            CYCShiDu		=	GetMeasureValue(CGQ_CYK_SHIDU_XY,AD_CYK_SHIDU); //����������ʪ�ȴ����� δ������ƽ��
            WenDuT1 		=	GetMeasureValue(CGQ_WENDU1_XY,AD_WENDU1);		//�������¶�ֵ
            break;
        default:
            CYCShiDu		=	GetMeasureValue(CGQ_CYK_SHIDU_KL,AD_CYK_SHIDU); //����������ʪ�ȴ����� δ������ƽ��
            WenDuT1 		=	GetMeasureValue(CGQ_WENDU1_KL,AD_WENDU1);		//�������¶�ֵ
            break;
        }

        WenDuT2 		=  	GetMeasureValue(CGQ_WENDU2,AD_WENDU2);          //DHS�¶�ֵ
        WenDuT3 		=  	GetMeasureValue(CGQ_WENDU3,AD_WENDU3);		    //���Բ��¶�
        HumiditySample 	=  	GetMeasureValue(CGQ_SHIDU,AD_SHIDU);            //���Բ�ʪ��
        QiYa 			=	GetMeasureValue(CGQ_QIYA,AD_QIYA);              //��ѹ
        LiuLiang_BK		=	GetMeasureValue(CGQ_LIULIANG,AD_LIULIANG);      //����
// 		if(LiuLiang_BK<0.3)
// 		{
// 			LiuLiang_BK = 0;
// 		}
        //��������=�������*���ѹ��/��273+����¶�(��))*��273+�����¶�(��))/����ѹ��
        LiuLiang_GK=LiuLiang_BK*101.325*(273.15+TDAIDIN)/(273.15*QiYa);
    }
    else
    {
        ADreadErr2++;
        if(ADreadErr2 > 10)
        {
            ADreadErr2 = 0;
            RestErr ++;
            OSMutexPend(OSUart0UsedMutex,0,&err);
            Rest();   //FPGA��λ
            ADRangeSel(ADRANGESEL_10);     //�趨ADC���뷶Χ
            TTLOutPut[TTL_YAZHIMOTO] = 0;
            TTLOutPut[TTL_PUMP] = 1;
            TTLOutPut[TTL_JIAREI] = 1;
            for(i=0; i<8; i++)
            {
                Sent_Buffer[i+4] = TTLOutPut[i];
            }
            Sent_Buffer[0] = START;
            Sent_Buffer[1] = TTLCTRLO;
            Sent_Buffer[2] = 9;
            Sent_Buffer[3] = 0;
            Sent_Buffer[12] = STOP;
            UART_Send(LPC_UART0, Sent_Buffer, 13,BLOCKING);
            ClampMotor = 0;	// ά����������ó�ʼ״̬��λ��
            OSMutexPost(OSUart0UsedMutex);

            TimeMotoCount = 18*50;
            while(TimeMotoCount)
            {
                OSTimeDlyHMSM(0, 0, 0,50);	//����ɼ�ʱ��
            }
            MeasureStepFlg = 0;
            return ;
        }
    }

    //���ȴ�����
    if(HeatOn)
    {//���Բ�ʪ�ȴ�����ֵ
        if((HumiditySample > Humidity ) &&(PumpFlg==0)/*�ô�*/ /*&& (abs(CYCShiDu - WenDuT3)<5)*/ && (WenDuT2<MaxHeatTemp))	 // ʪ�ȴ�����ֵ�Ҳ�����������²�С��5����DHS�¶�С��60��
        //if((CYCShiDu > Humidity ) &&(PumpFlg==0)/*�ô�*/ /*&& (abs(CYCShiDu - WenDuT3)<5)*/ && (WenDuT2<60))	 // ʪ�ȴ�����ֵ�Ҳ�����������²�С��5����DHS�¶�С��60��
        {
           // if(HeatFlg==1) { //���ȴ��ر�
                HeatFlg = 0;//�򿪰��ȴ�
                OSMutexPend(OSUart0UsedMutex,0,&err);
                TTLOutput (TTL_0,HeatFlg,TTL_JIAREI);
                OSMutexPost(OSUart0UsedMutex);
           // }
        }
        else
        {
            //if(HeatFlg ==0) {
                HeatFlg = 1;//�رհ��ȴ�����
                OSMutexPend(OSUart0UsedMutex,0,&err);
                TTLOutput (TTL_0,HeatFlg,TTL_JIAREI);
                OSMutexPost(OSUart0UsedMutex);
            //}
        }
    }

    RunStuation=0;
    WarnStuation=0;
    EeorT=0;
    EeorH=0;
    EeorL=0;
    EeorP=0;
    if(WenDuT1<-20 || WenDuT1>100)
    {
        EeorT |= 0x01;	//������
    }
    if(WenDuT3<-20 || WenDuT3>100)
    {
        EeorT |= 0x04;	//������
    }
    if((WenDuT2<0) || (WenDuT2>(MaxHeatTemp+10)))
    {
        EeorT |= 0x02;	//������
        HeatFlg = 1;
        OSMutexPend(OSUart0UsedMutex,0,&err);
        TTLOutput (TTL_0,HeatFlg,TTL_JIAREI);
        OSMutexPost(OSUart0UsedMutex);
    }
    if(HumiditySample<0 || HumiditySample>100)
    {
        EeorH |= 0x02;
    }
    if(QiYa<90 || QiYa>120)
    {
        EeorP |= 0x01;
    }
    if(FujiStartFlg)
    {
        if(LiuLiang_GK<FlowOfSamp*(0.95) || LiuLiang_GK>FlowOfSamp*(1.05))
            EeorL |= 0x01;
    }
    if(kaijiTime < 15)
    {
        return ;
    }
    ErrDatChange();
    if(EEORStuation1)	  //�д�
    {

        SET_LED1(1);
        if( EEORStuation1!= EEORStuationOld)//�µĴ����¼
        {
            //ϵͳ��־ cuowu
            goSysRecor.recordTime = TimeDate;
            goSysRecor.Event = EVENTERRO;
            for(i=0; i<16; i++)
            {
                if((0x01<<i)&EEORStuation1)
                {
                    goSysRecor.ContentH = 0;
                    goSysRecor.ContentL = i;
                    FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
                    WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
                    SysrecordPoint +=1;
                    SysrecordReadFlg++;
                    OSMutexPend(OSI2CUsedMutex,0,&err);
                    WriteData32(SYSRECORDPOINT2,SysrecordPoint );/*��¼��¼����*/
                    OSMutexPost(OSI2CUsedMutex);
                    OSTimeDlyHMSM(0, 0, 0,300);
                }
            }
        }
    }
    else  //�ָ�������
    {
        SET_LED1(0);
        FilterErrTimes = 0;
    }

    if(HumiditySample > (Humidity*1.1))
    {
        WarnStuation |= 0x01;
    }
    else
    {
        WarnStuation &= 0xfe;
    }
    (WarnStuation != 0)?(SET_LED2(1)):(SET_LED2(0));//���ơ�
    (WarnStuation|EEORStuation1)?(SET_LED3(0)):(SET_LED3(1));
}

//�Լ�
void 	SelfCheak(void)
{
//uint8 err;
//����ص������Ƿ����  ���Դ��ѹȷ��ѹ��������ϵ����


//	OSMutexPend(OSUart0UsedMutex,0,&err);
//	Rest();
//	TTLOutput (TTL_0,0,TTL_YAZHIMOTO);	 //�� ����
//	OSMutexPost(OSUart0UsedMutex);
//	TimeMeasCount = 8;
//	while(TimeMeasCount)
//	{
//		OSTimeDly(50);//0.25��;
//	}
}

void  RunPaperStep(void)
{
    CPU_INT08U   err,i;
// 	if(measRecord.I>=Mass)//���������趨ֵ�� ��Ҫ��ֽ��
// 	{
// 			return ;
// 	}
    songkaiMoto();
    OSMutexPend(OSUart0UsedMutex,0,&err);
    Rotary(30); //һȦ200����
    MotorCtrl(0x02|DIR_RIGHT,100000);//�ú������ڿ��Ʋ��������������������ֱ����� ����Ĳ����޹ؽ�Ҫ��
    OSMutexPost(OSUart0UsedMutex);
    PaperTime++;
    FilterRemain = 100-(100*PaperTime/PAPER_MOVE_TIME);
    if(FilterRemain<0)
    {
        FilterRemain = 0;
    }
    OSMutexPend(OSI2CUsedMutex,0,&err);
    C1025_WriteByte(ConInf,(uint8)(FilterRemain));
    OSMutexPost(OSI2CUsedMutex);
    if(PaperTime>PAPER_MOVE_TIME)
    {
        PaperOverFlg = 1;//  Ԥ��ֽ�����þ���
        PaperTime = PAPER_MOVE_TIME;
    }
    OSTimeDlyHMSM(0, 0, 5, 0);//��ʱ5s������ʱ�䡣��ֹ����ֽʱ�Ĵ��󣬸�Ϊ15s  �Ļ�5�� ��ֹɢ��̫�ࡣ

    OSMutexPend(OSUart0UsedMutex,0,&err);
    Rotary(40); //���ξͼ���Ƿ��ֽ����ֽ��
    OSMutexPost(OSUart0UsedMutex);
    yajingMoto();

    //ʵʱģʽ�´���
    if(RunStyleFlg==SHISHI)//ʵʱģʽ
    {
        for(i = 0; i<16; i++)
        {
            PargRealI[i] = 0;
            PargRealTv2[i] = 0;
            PargRealTv1[i]= 0;//����
        }
        NumOfI=0;
    }
}


float FindMin(float *pt )
{
    float min;
    min =*pt;
    pt++;
    while(*pt != 0 )
    {
        if(*pt<min)
        {
            min = *pt;
        }
        pt++;
    }
    return min;
}


static float FindTheOrderGuy(CNT_RECORD * p,uint8 Order,uint8 Record_Num )
{
    float total_tmp = 0;
    float average_tmp = 0;
    float compare_tmp = 0;
    int i=0,j=0;
    float return_value;

    CNT_RECORD* tmp_p = (CNT_RECORD*)malloc(sizeof(CNT_RECORD)*200);

    if((Record_Num>185)||(Record_Num<175))
    {
        free (tmp_p);
        return 0 ;
    }

    if(Order>Record_Num)
    {
        free (tmp_p);
        return 0 ;
    }

    for(i = 0; i<Record_Num; i++)
    {
        total_tmp  += p[i].I;
    }

    average_tmp = total_tmp / Record_Num;

    for(i = 0; i<Record_Num; i++)
    {
        p[i].abs_offset = abs(p[i].I - average_tmp);
    }

    memcpy(tmp_p,p,Record_Num*sizeof(CNT_RECORD));

    for(i=0; i<Record_Num; i++)
    {
        for(j=i; j<Record_Num; j++)
        {
            if( (tmp_p+i)->abs_offset < (tmp_p+j)->abs_offset )
            {
                compare_tmp = (tmp_p+i)->abs_offset;
                (tmp_p+i)->abs_offset = (tmp_p+j)->abs_offset;
                (tmp_p+j)->abs_offset = compare_tmp;
            }
        }
    }
    return_value = (tmp_p + Order)->abs_offset;
    free (tmp_p);
    //ָ�뱻free����Ȼ���Ա�ʹ�� ��������Υ������ ���� �̲߳���ȫ
    return return_value;
}

//һ���˵�30��

static float ReCalculateAverageCnt(CNT_RECORD * p,float Order,uint8 Record_Num)
{
    float cnt_tmp = 0 , cnt_total_tmp =0 ;
    int i=0;
    float TheOrderOffset = 0;

    TheOrderOffset = FindTheOrderGuy(p,Order,Record_Num );

    for(i =0; i<Record_Num; i++)
    {
        if( (p+i)->abs_offset < TheOrderOffset)
        {
            cnt_total_tmp += (p+i)->I;
            cnt_tmp	++;
        }
    }

    return (cnt_total_tmp / cnt_tmp);
}


int MeasureStep(void)
{
    CPU_INT08U   err,i;

    //����Դ��λ
    OSMutexPend(OSUart0UsedMutex,0,&err);
    TTLOutput (TTL_2,1,TTL_BRAY_Ctrl1);
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl2);
    OSMutexPost(OSUart0UsedMutex);

    OSTimeDlyHMSM(0, 0, C14MotMoveTime,0);

    OSMutexPend(OSUart0UsedMutex,0,&err);
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl1);
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl2);
    OSMutexPost(OSUart0UsedMutex);

    OSMutexPend(OSUart0UsedMutex,0,&err);
    CountCtrl(COUNT_START);
    while((CountCodeSendFlg)&&(counterr<5))
    {   counterr++;
        CountCtrl(COUNT_START);
    }
    OSMutexPost(OSUart0UsedMutex);
    if((5< counterr) )	///������
    {
        counterr = 0;
        RestErr ++;
        OSMutexPend(OSUart0UsedMutex,0,&err);
        Rest();
        ADRangeSel(ADRANGESEL_10);
        TTLOutPut[TTL_YAZHIMOTO] = 0;
        TTLOutPut[TTL_PUMP] = 1;
        TTLOutPut[TTL_JIAREI] = 1;
        for(i=0; i<8; i++)
        {
            Sent_Buffer[i+4] = TTLOutPut[i];
        }
        Sent_Buffer[0] = START;
        Sent_Buffer[1] = TTLCTRLO;
        Sent_Buffer[2] = 9;
        Sent_Buffer[3] = 0;
        Sent_Buffer[12] = STOP;
        UART_Send(LPC_UART0, Sent_Buffer, 13,BLOCKING);
        OSMutexPost(OSUart0UsedMutex);
        ClampMotor = 0;	// ά����������ó�ʼ״̬��λ��
        TimeMotoCount = 18*50;
        while(TimeMotoCount)
        {
            OSTimeDlyHMSM(0, 0, 0,50);	//����ɼ�ʱ��
        }
        MeasureStepFlg = 0;
        return 0;
    }
    MeasureTsS = WenDuT3;

    TimeMeasCount =  360;	  //
    //TimeMeasCount =  60;	  //������ ����ʱ���Щ
    while((StopFlg==0)&&TimeMeasCount&&(counterr<5))
    {
        OSTimeDly(100);//0.5��;
        //�۲������
        OSMutexPend(OSUart0UsedMutex,0,&err);
        CountRead();
        CountAver = CountTotal / CountTimes;
        OSMutexPost(OSUart0UsedMutex);
    }
    MeasureTsE =WenDuT3;

    if((StopFlg == 1) ||(counterr==5))
    {
        return 0;
    }
    CountTotal = 0;
    CountTimes = 0;
    OSMutexPend(OSUart0UsedMutex,0,&err);
    CountRead();
    if(CountReadFlg && (CountReaderr<2))
    {
        CountReaderr++;
        CountRead();
    }
    OSMutexPost(OSUart0UsedMutex);
    //����ƽ��ֵ
    CountAver =  CountTotal / CountTimes;
    if(RunStyleFlg==SHISHI)//ʵʱģʽ
    {
        //3���Ӳ���һ�Σ�15��45���ӣ���һ�ο�ʼ����45���Ӻ����ݲ�������
        PargRealI[NumOfI%15] = CountAver;
        PargRealTv2[NumOfI%15] = TV_BK;
        PargRealTv1[NumOfI%15] = TV_GK;//����

        NumOfI++;
        //����
        if(PargRealI[NumOfI%15] == 0)//�տ�ʼ����45����
        {
            Parg_x = 0;
        }
        else
        {
            Parg_x = log(PargRealI[NumOfI%15]/PargRealI[(NumOfI-1)%15])/Parg_u ;   //��λ�� Mg ÿcm2
        }
        Parg_m = Parg_k*Parg_x*AREA ;  ///��λmg
        Parg_v_GK = ( TV_GK-PargRealTv1[NumOfI%15] )/60.0;//LiuLiang_GK*FUJITIME; //��λL��dm2
        Parg_v_BK = ( TV_BK-PargRealTv2[NumOfI%15] )/60.0;//LiuLiangBIAO*FUJITIME; //��λL��dm2 �������� ����ʲô�ط��ı�׼
        if(abs(Parg_v_BK - 666) > 333)//�쳣
        {
            Parg_v_BK = 45*14.8;//ע��˴��̶�Ϊ45���ӡ������������Ҫ�ġ�
        }
        if(abs(Parg_v_GK - 750) > 333)//�쳣
        {
            Parg_v_GK = 45*16.68;//ע��˴��̶�Ϊ45���ӡ������������Ҫ�ġ�
        }
        Parg_PM25 = 1000000.0*Parg_m/Parg_v_BK;	  //��λmg/L
        Parg_PM25_GK = 1000000.0*Parg_m/Parg_v_GK;	  //��λmg/L

        //���������ֵ
        outputstruct1.CONC = Parg_PM25;
        measRecord.I0 = PargRealI[NumOfI%15];
        measRecord.I = PargRealI[(NumOfI-1)%15];
        measRecord.TV = Parg_v_BK;
// 	measRecord.RH  = HumiditySample;//�˴���ʪ�Ⱥ͸���ʱ��ʪ�Ȳ�ͬ�����Ѿ�ͣ�ˣ�Ӧ���ڸ���ʱ�ɼ�
        measRecord.T1 = WenDuT1;
        measRecord.P= QiYa;
        measRecord.recordResultG = Parg_PM25_GK;
        measRecord.MAS = 0;//Parg_m;//�¶Ȳ�����ֵ
        measRecord.recordResult = Parg_PM25;
        measRecord.recordTimeStart = TimeDate;
        measRecord.recordTimeEnd = TimeDate;
        //����
        FlashDatChange(&DatForRecord[0],measRecord);
        WriteRecordFlash(recordPoint,&DatForRecord[0]);
        OSMutexPend(OSI2CUsedMutex,0,&err);
        recordPoint= recordPoint+1;	 /*��¼������1*/
        WriteData32(RECORDPOINT2,recordPoint);/*��¼��¼����*/
        OSMutexPost(OSI2CUsedMutex);

        CUpdateFlg = 1;

        //�������ݴ洢������
        usRegHoldingBuf[200] = 	recordPoint;
// 						if(measRecord.I<Mass)//����xiaoyu�趨ֵ��Ҫ��ֽ��
// 						{
//
// 						}
    }
    else if(RunStyleFlg==ZHOUQI)
    {
        if(MeasureStepFlg== 1)
        {
            OSSchedLock();
            Parg_I0_Upload = Parg_I0;
            Parg_I0 = CountAver;
//				printf("\nI0 cnt result!\n");
//				for(i=1;i<180;i++)
//				printf("%f\n", Cnt_Record[i].I);
//				Parg_I0_ReCnt = (int)ReCalculateAverageCnt(&Cnt_Record[0],60,CountTimes);
            MeasureTs = 0.5*(MeasureTsS+MeasureTsE);
            OSSchedUnlock();
        }
        else if(MeasureStepFlg== 3)
        {
            OSSchedLock();
            MeasureTe = 0.5*(MeasureTsS+MeasureTsE);
            Parg_I = CountAver;
//				printf("\nI1 cnt result!\n");
//				for(i=1;i<180;i++)
//				printf("%f\n", Cnt_Record[i].I);
//				Parg_I_ReCnt = (int)ReCalculateAverageCnt(&Cnt_Record[0],60,CountTimes);
            //����
            Parg_x = log(Parg_I0/Parg_I)/Parg_u ;   //��λ�� mg/cm3
            Parg_x_Recnt = log(Parg_I0_ReCnt/Parg_I_ReCnt)/Parg_u ;   //��λ�� mg/cm3
            ErrorDat = 1.293*273.15*HIGH*(1/(273.15+MeasureTe)-1/(273.15+MeasureTs));//P0*T0*V/S*(1/T2-1/T1); mg/cm3
            //�¶�����ж� �������0.05 ʵ���²������10�ȡ�������ǳ������ˣ���ʱ�������¶Ȳ�����
            //�¶Ȳ�5�� ErrDat = 0.025
            //�¶Ȳ�10�� ErrDat = 0.05
            //�¶Ȳ�15�� ErrDat = 0.075

            if( TempCompensateFlag && (abs(ErrorDat)<0.05) )
            {
                Parg_x = Parg_x - ErrorDat;
            }

            Parg_m =Parg_k* Parg_x*AREA ;  ///��λmg

            //���ۼӵķ��������������ʵ�����ۼ�����
            Parg_v_GK = TV_GK/60.0;//��λL��dm3
            Parg_v_BK = TV_BK/60.0;//��λL��dm3 �������� ����ʲô�ط��ı�׼

            Parg_PM25 = 1000000.0*Parg_m/Parg_v_BK;	  //��λmg/L
            Parg_PM25 = Parg_PM25 + Parg_b - 20;
            if(Parg_PM25<0.01)
            {
                Parg_PM25 = 6.75;//��ֹ�洢��ֵ��
            }
            Parg_PM25_GK =  Parg_PM25*Parg_v_BK/Parg_v_GK;
            OSSchedUnlock();

            //���������ֵ
            outputstruct1.CONC = Parg_PM25;
            //��ֵ
            measRecord.I0 = Parg_I0;
            measRecord.I = Parg_I;
            measRecord.TV = Parg_v_BK;
// 			measRecord.RH  = HumiditySample;//�˴���ʪ�Ⱥ͸���ʱ��ʪ�Ȳ�ͬ�����Ѿ�ͣ�ˣ�Ӧ���ڸ���ʱ�ɼ�
            measRecord.T1 = WenDuT1;
            //measRecord.T1 = Parg_I0_ReCnt;
            measRecord.P= QiYa;
            //measRecord.P= Parg_I0_ReCnt;
            switch(UseFor)
            {
            case 0:
                measRecord.recordResultG = PM10_GS_H/GS_Cnt	;
                GS_Cnt = 0	;
                PM10_GS_H=    0   ;
                TSP_GS_H = 	0	;
                PM25_GS_H= 	0	;
                break;
            case 1:
                measRecord.recordResultG = PM25_GS_H/GS_Cnt	;
                GS_Cnt = 0	;
                PM10_GS_H=    0   ;
                TSP_GS_H = 	0	;
                PM25_GS_H= 	0	;
                break;
            case 2:
                measRecord.recordResultG = TSP_GS_H/GS_Cnt	;
                GS_Cnt = 0	;
                PM10_GS_H=    0   ;
                TSP_GS_H = 	0	;
                PM25_GS_H= 	0	;
                break;
            default:
                measRecord.recordResultG = 0;
                break;
            }

            //measRecord.recordResultG = 1000000.0*(Parg_k* Parg_x_Recnt*AREA)/Parg_v_BK;
			measRecord.recordResultG = Parg_PM25_GK;
            measRecord.MAS = 10000*ErrorDat;
            measRecord.recordResult = Parg_PM25;
            measRecord.recordTimeStart = MeaserStartTime;
            measRecord.recordTimeEnd = MeaserEndTime;
            //����
            FlashDatChange(&DatForRecord[0],measRecord);
            WriteRecordFlash(recordPoint,&DatForRecord[0]);
            OSMutexPend(OSI2CUsedMutex,0,&err);
            recordPoint= recordPoint+1;	 /*��¼������1*/
            WriteData32(RECORDPOINT2,recordPoint);
            Delay100uS(20);

            cheak =  ReadData32(RECORDPOINT2);
            if(cheak != recordPoint)
            {
                WriteData32(RECORDPOINT2,recordPoint);
            }
            /*��¼��¼����*/
            OSMutexPost(OSI2CUsedMutex);
            CUpdateFlg = 1;

            //�������ݴ洢������
            usRegHoldingBuf[200] = 	recordPoint;
        }
    }
    //����Դ��λ
    OSMutexPend(OSUart0UsedMutex,0,&err);
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl1);
    TTLOutput (TTL_2,1,TTL_BRAY_Ctrl2);
    OSMutexPost(OSUart0UsedMutex);

    //����Ӧ������Ϊ�������õ�ʱ��
    ClampRadio = 1;
    OSTimeDlyHMSM(0, 0, C14MotMoveTime ,0);	//����ʱ��
    OSMutexPend(OSUart0UsedMutex,0,&err);
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl1);
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl2);
    OSMutexPost(OSUart0UsedMutex);
    return 0;
}

void FujiStep(void)
{
    CPU_INT08U   err;
    MeaserStartTime = TimeDate;
    //������
    OSMutexPend(OSUart0UsedMutex,0,&err);
    PumpFlg = 0;
    TTLOutput (TTL_0,PumpFlg,TTL_PUMP);
    //�ظ�ѹ
    TTLOutput (TTL_0,GAOYA_CLOSE,TTL_GAOYA);
    //�趨����
    FlowOfSamp_B_Set =  CaliParF1*(QiYa*(FlowOfSamp) * (273.15) )/((273.15+TDAIDIN)*101.325);
    DAMCSOutPutSetOne(DA_PUMP,(5.0*FlowOfSamp_B_Set/20.0));
    DAMCSOutPutAll();

    OSMutexPost(OSUart0UsedMutex);

    TV_GK =0 ;
    TV_BK =0 ;
    TimeMeasCount =30 ;	  //���ȶ�
    while((StopFlg==0)&&TimeMeasCount)
    {
        OSTimeDly(100);//0.5��;
    }
    if(StopFlg == 1)
        return ;
    if(TimeType)
    {
        //TimeMeasCount = 3000;   //50���ӣ�����8���Ӳ��� �հ׺���Ʒ��10����ĵ���������͸����ȶ��ڡ�
        TimeMeasCount =(int)CycleOfSampSet*60 - 400;//57����
    }
    else
    {
        TimeMeasCount =(int)CycleOfSampSet*60 - 400;
    }
    FujiStartFlg =1;
    while((StopFlg==0)&&TimeMeasCount)
    {
        //��ʾ����
        OSTimeDlyHMSM(0, 0, 10,0);	//10��;
        FlowOfSamp_B_Set =  CaliParF1*(QiYa*FlowOfSamp * (273.15) )/((273.15+TDAIDIN)*101.325);
        OSMutexPend(OSUart0UsedMutex,0,&err);
        DAMCSOutPutSetOne(DA_PUMP,(5.0*FlowOfSamp_B_Set/20.0));
        DAMCSOutPutAll();
        OSMutexPost(OSUart0UsedMutex);

        measRecord.RH  = HumiditySample;
        outputstruct1.RH = 	HumiditySample;
        outputstruct1.flow1 = LiuLiang_GK;
        outputstruct1.flow2 = LiuLiang_BK;
        outputstruct1.temperature1 = WenDuT1;
        outputstruct1.temperature2 = WenDuT2;
        outputstruct1.temperature3 = WenDuT3;
        //�� �����ʱ������ѹ����ǰ1���ӣ�ע������С12���ӽ���ʱ�Ĵ���
        if((TimeMeasCount<60)&&(TimeMeasCount>0))
        {
            OSMutexPend(OSUart0UsedMutex,0,&err);
            TTLOutput (TTL_0,GAOYA_OPEN,TTL_GAOYA);
            OSMutexPost(OSUart0UsedMutex);
        }

    }
    FujiStartFlg =0;

    HeatFlg = 1;//�رհ��ȴ�����
    OSMutexPend(OSUart0UsedMutex,0,&err);
    PumpFlg = 1;
    TTLOutput (TTL_0,PumpFlg,TTL_PUMP);
    TTLOutput (TTL_0,HeatFlg,TTL_JIAREI);
    //����ѹ
    TTLOutput (TTL_0,GAOYA_OPEN,TTL_GAOYA);
    OSMutexPost(OSUart0UsedMutex);
    MeaserEndTime = TimeDate;//��¼����ʱʱ��㡣
    if(StopFlg == 1)
    {
        return ;
    }
    OSTimeDlyHMSM(0, 0, 15,0);	//����ʱ���ñ���ȫͣ��
}

void songkaiMoto(void)
{
    CPU_INT08U   err;
    if(ClampMotor==0)
    {
        OSMutexPend(OSUart0UsedMutex,0,&err);
        TTLOutput (TTL_0,0,TTL_YAZHIMOTO);	 //����
        OSMutexPost(OSUart0UsedMutex);
        ClampMotor = 1;	 //״̬��Ϊ�ɿ���
        TimeMotoCount = MotorTime*50;  //7s
        while(TimeMotoCount)
        {
            OSTimeDly(50);//��;
        }
        OSMutexPend(OSUart0UsedMutex,0,&err);
        TTLOutput (TTL_0,1,TTL_YAZHIMOTO);	 //ֹͣ
        OSMutexPost(OSUart0UsedMutex);
    }
}

void yajingMoto(void)
{   CPU_INT08U   err;

    if(ClampMotor==1)
    {
        OSMutexPend(OSUart0UsedMutex,0,&err);
        TTLOutput (TTL_0,0,TTL_YAZHIMOTO);	 //����
        OSMutexPost(OSUart0UsedMutex);
        ClampMotor = 0;	 //״̬��Ϊѹ����
        TimeMotoCount = 8*50; //û��20ms 1sҪ50��    һȦ9������         10s
        while(TimeMotoCount)
        {
            OSTimeDly(50);//0.25��;
        }
        OSMutexPend(OSUart0UsedMutex,0,&err);
        TTLOutput (TTL_0,1,TTL_YAZHIMOTO);	 //��ֹͣ
        OSMutexPost(OSUart0UsedMutex);
    }

}





