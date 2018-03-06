#include"FPGA_Contorl.h"
#ifdef CH320
#define AREA	  1.02
#else
#define AREA	  1.539
#endif
#define HIGH 1.3
int8 errtimemax 	= 5;
int8 errtimemax0 	= 5;
int8 UseFor 	= 	1;//仪器用途(中文) 0  PM10 1  PM2.5  2 TSP
int8 UseFor0 	= 	1;
uint32 ComminiStyle,ComminiStyle0;//0 网口，1串口
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
float TSP_GS_k=14.3,PM10_GS_k=9.2,PM25_GS_k=4.1;//光散矫正系数
fp32 GS_Cycle;

char Parg_b=0,Parg_b0 = 0;
uint8 kaijiTime;
uint8 FilterErrTimes;
static uint8_t Send_Bytes=7;
uint8_t Sent_Buffer[50]= {'a','b','c','d'}; //发送缓冲区
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
float PargRealTv1[16];//工况
float MeasureTs,MeasureTe,MeasureTsS,MeasureTsE;
int16  ADDat2;
float addat[16];
uint32 NumOfI;
volatile uint8 TTLOutPut[8]= {0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03};
uint8 CmErrorState[16];//={1,2,3,4,5};
uint8  TTLDat;
uint8 StopFlg,StartKeyFlg,ADBufeerCleanFlg;
uint8  StatuErrorFlg;
uint8 TimeType,MeasureStepFlg;	 //整点模式flg
float Vcc_5;
int  CountReadFlg,CountReaderr,CountStartFlg,CountTimer;
uint32 CountNow,CountAver	,CountTotal ,CountTimes  ;
uint8 ADRangesel =1;
#define CODE_R  1.8
#define PAPER_MOVE_TIME (2*5000)/(3.14*CODE_R)       //60米 保守50米	
uint32 PaperTime;
uint8 PaperOverFlg;

#define LEN  4   //移动算术平均的个数+2=SHIFT<<2+2
#define SHIFT 2  //^SHIFT
//uint8 pmax,pmin;//记录数据表中最大值和最小值的位置,
//在一般的数据采集系统中,数据的长度>=8,
//因此用指针记录而不是直接记录最大值和最小值
uint16 AvrT[LEN] = {25,25,25,25};
uint16 AvrP[LEN] = {101,101,101,101};
uint16 AvrL[LEN];
uint16 AvrHR[LEN] = {30,30,30,30};
uint8 pdata_T;    //移动指针
uint8 pdata_P;    //移动指针
uint8 pdata_L;    //移动指针
uint8 CountCodeSendFlg,pdata_HR;    //移动指针
uint16  EEORStuationOld;

float TV_GK,TV_BK;
uint8 CheackDat(volatile uint8 *dat)
{
    uint8 tmpc = 0,step = 0;
    int32 i,j;
    int32 ADall=0 ;
    //延伸很重要，如果太近那么就会一起发过来。

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
        case 1:   //命令解析
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
            case ERR1: //通道号无效
                break;

            case ERR2: // 命令不完整
                break;
            case ERR3: //奇偶校验位不准确
                break;
            case ERR4: //控制字未定义
                break;
            case ERR5: //数据或命令长度不正确
                break;
            case ERR6: //缓存未就位
                break;
            case ERR7: // 所请求的数据超长
                break;
            case ERR8: // 无法响应新命令 通道0--7忙
                break;
            case ERR9: // 8--15 忙
//						ADStatBusyFlg = 1;
                break;
            case ERR10:// 0--15忙
//						ADStatBusyFlg = 1;
                break;
            case ERR11:// dac忙
                break;
            case ERR12://  SPI忙
                break;
            case ERR13:// 滤纸断了或没了
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
                //其他错误处理
                break;
            }
            if(tmpc <0x11)
                CmErrorState[tmpc] = tmpc;
            step = 3;
            break;
            //帧结束
        case 3 :
            if(tmpc == STOP )
            {
                step = 0;//帧分析结束。
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
            //AD数据采集
        case 5:
            for(j=0; j<8; j++)
            {
                ADDat_1[j] =(int16)(dat[i]*256+dat[i+1]);
                i=i+2;
            }
            return 0;
//				break;
            //TTL数据采集
        case 6:
            TTLDat = dat[i];
            step = 0;
            break;
            //AD数据多个采集
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
            //光管计数值
        case 8:
            if(tmpc == 0x0b)//数据长度正确
            {
                if(CountReadFlg == 1)
                {
                    CountReadFlg = 0;
                }
                CountNow = (dat[i+1]<<24)|(dat[i+2]<<16)|(dat[i+3]<<8)|(dat[i+4]);
                CountTotal =(dat[i+4+1]<<24)|(dat[i+4+2]<<16)|(dat[i+4+3]<<8)|(dat[i+4+4]);
                //计数是从1次开始的
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
            //AD数据采集
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

//控制命令
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

//应答帧
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

//结束帧
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

//转义函数
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

//旋转编码器
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

//MOIndex 的最高位时控制方向的。
void MotorCtrl(uint8 MOIndex,uint32 Distance)//该函数用于控制步进电机，我们用来控制直流电机 后面的参数无关紧要。
{   //CPU_INT08U   err;
    uint8 Dat;
    uint8 length=0;	//startVelocity = EA60	200HZ	maxVelocity  2EE0 1K
    // 0X0010 16US    31步加速 加减速时间为16*31US
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

//AD多通道单点采集
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

////AD多通道多点采集
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

//DA 多通道单点设置
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

//da 多通道单点输出 全部输出。
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

//DA单通道多点输出
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

//DA单通道连续输出，
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


//DA与AD同步设置
//void DAC_ADC_Synchronize ();


//TTL输入
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
function:AD buffer状态查询
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
function: AD缓存读
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
    /* 在调用此子程序前必须对 */
    /* pdata,datas[]数组,  */
    /* pmax,pmin进行初始化  */
    /****************************/
    uint8 i;
    uint16 average=0;  //清零，用来计算平均值
    pdata_T=(pdata_T+1)%LEN; //指针下标在0到LEN-1上滑动
    AvrT[pdata_T]=_data;  //采样所得数据存入数据表中
    for(i=0; i<LEN; i++)
        average+=AvrT[i]; //求所有数据总和
    return (average>>SHIFT);    //求算术平均值
}

uint16 AverP(float _data)
{
    /****************************/
    /* 在调用此子程序前必须对 */
    /* pdata,datas[]数组,  */
    /* pmax,pmin进行初始化  */
    /****************************/
    uint8 i;
    uint16 average=0;  //清零，用来计算平均值
    pdata_P=(pdata_P+1)%LEN; //指针下标在0到LEN-1上滑动
    AvrP[pdata_P]=_data;  //采样所得数据存入数据表中
    for(i=0; i<LEN; i++)
        average+=AvrP[i]; //求所有数据总和
    return (average>>2);    //求算术平均值
}

uint16 AverL(float _data)
{
    /****************************/
    /* 在调用此子程序前必须对 */
    /* pdata,datas[]数组,  */
    /* pmax,pmin进行初始化  */
    /****************************/
    uint8 i;
    uint16 average=0;  //清零，用来计算平均值
    pdata_L=(pdata_L+1)%LEN; //指针下标在0到LEN-1上滑动
    AvrL[pdata_L]=_data;  //采样所得数据存入数据表中
    for(i=0; i<LEN; i++)
        average+=AvrL[i]; //求所有数据总和
    return (average>>2);    //求算术平均值
}

uint16 AverHR(float _data)
{
    /****************************/
    /* 在调用此子程序前必须对 */
    /* pdata,datas[]数组,  */
    /* pmax,pmin进行初始化  */
    /****************************/
    uint8 i;
    uint16 average=0;  //清零，用来计算平均值
    pdata_HR=(pdata_HR+1)%LEN; //指针下标在0到LEN-1上滑动
    AvrHR[pdata_HR]=_data;  //采样所得数据存入数据表中
    for(i=0; i<LEN; i++)
        average+=AvrHR[i]; //求所有数据总和
    return (average>>2);    //求算术平均值
}

void ADget(void)
{
    CPU_INT08U   err;
    OSMutexPend(OSUart0UsedMutex,0,&err);
    if(ADBufferStart2 != 0x03)
    {
        ADMCSSampling(0x00ff,0x01,2); //AD多通道单点采集
    }
    ADBufferStart2 = 0;
    ADBufferStatus(0x00ff);
    if(0!=ADBufferStart2)
    {   ADRead1 = 1;
        ADBufferDatReadFlg = 1;
        ADBufferRead(0x00ff,1,1);	// ad2 ad5  ad6  ad 3要乘以2倍  ad1 ad7	 ad8 不变  ad4要乘以 4.92倍
        ADBufferRLS(0x02);	//清除buffer
        while(ADBufeerCleanFlg&&(ADreadErr<5))
        {
            ADreadErr++;
            ADBufferRLS(0x02);	//清除buffer
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
        ADBufferRead(0xFF00,1,1);	// ad2 ad5  ad6  ad 3要乘以2倍  ad1 ad7	 ad8 不变  ad4要乘以 4.92倍
        ADBufferRLS(0x02);	//清除buffer
        while(ADBufeerCleanFlg&&(ADreadErr<5))
        {
            ADreadErr++;
            ADBufferRLS(0x02);	//清除buffer
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
        //移动平均
        //ADDat[AD_LIULIANG] = AverL(ADDat[AD_LIULIANG]);
//		ADDat[AD_SHIDU] = AverHR(ADDat[AD_SHIDU]);
//		ADDat[AD_QIYA] = AverP(ADDat[AD_QIYA]);
//		ADDat[AD_WENDU1] = AverT(ADDat[AD_WENDU1]);
        //计算采样电压值
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
//		LoadK(CGQ_QIYA,(1/(Vcc_5*0.009)));//计算气压
//		//电源电压
//		PowerV1=GetADValue(VCC_5);             //   +5v
//		PowerV2=GetADValue(VCC_12);             //   +12v
//		PowerV3=GetADValue(VSS_12);             //   -12v
//		PowerV4=GetADValue(VCC_24);             //   +24v

//		TCQWenDuT4 		= 	GetMeasureValue(CGQ_TCQ_WENDU,AD_TCQ_WENDU);    //新增探测器温度传感器 未做滑动平均

//		switch(SensorSel)
//		{
//			case 1:
//				CYCShiDu		=	GetMeasureValue(CGQ_CYK_SHIDU_XY,AD_CYK_SHIDU); //新增采样口湿度传感器 未做滑动平均
//				WenDuT1 		=	GetMeasureValue(CGQ_WENDU1_XY,AD_WENDU1);		//采样口温度值
//				break;
//			default:
//				CYCShiDu		=	GetMeasureValue(CGQ_CYK_SHIDU_KL,AD_CYK_SHIDU); //新增采样口湿度传感器 未做滑动平均
//				WenDuT1 		=	GetMeasureValue(CGQ_WENDU1_KL,AD_WENDU1);		//采样口温度值
//			break;
//		}

//		WenDuT2 		=  	GetMeasureValue(CGQ_WENDU2,AD_WENDU2);          //DHS温度值
//		WenDuT3 		=  	GetMeasureValue(CGQ_WENDU3,AD_WENDU3);		    //测试舱温度
//		HumiditySample 	=  	GetMeasureValue(CGQ_SHIDU,AD_SHIDU);            //测试舱湿度
//		QiYa 			=	GetMeasureValue(CGQ_QIYA,AD_QIYA);              //气压
//		LiuLiang_BK		=	GetMeasureValue(CGQ_LIULIANG,AD_LIULIANG);      //流量
        LiuLiang_BK		=	GetMeasureValue(CGQ_LIULIANG_HONEYWELL,AD_LIULIANG);      //流量计流量
        dat = addat[AD_LIULIANG]*ADStructTable[AD_LIULIANG].Gain;//求电压值
        printf("LiuLiang_BK = %f\t",LiuLiang_BK);
        printf("LiuLiang_ad = %f\r\n",dat);
// 		if(LiuLiang_BK<0.3)
// 		{
// 			LiuLiang_BK = 0;
// 		}
        //工况流量=标况流量*标况压力/（273+标况温度(℃))*（273+工况温度(℃))/工况压力
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
        //移动平均
        ADDat[AD_LIULIANG] = AverL(ADDat[AD_LIULIANG]);
        ADDat[AD_SHIDU] = AverHR(ADDat[AD_SHIDU]);
        ADDat[AD_QIYA] = AverP(ADDat[AD_QIYA]);
        ADDat[AD_WENDU1] = AverT(ADDat[AD_WENDU1]);
        //计算采样电压值
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

        //LoadK(CGQ_QIYA,(1/(Vcc_5*0.009)));//计算气压
        //电源电压
        PowerV1=GetADValue(VCC_5);             //   +5v
        PowerV2=GetADValue(VCC_12);             //   +12v
        PowerV3=GetADValue(VSS_12);             //   -12v
        PowerV4=GetADValue(VCC_24);             //   +24v

        TCQWenDuT4 		= 	GetMeasureValue(CGQ_TCQ_WENDU,AD_TCQ_WENDU);    //新增探测器温度传感器 未做滑动平均

        switch(SensorSel)
        {
        case 1:
            CYCShiDu		=	GetMeasureValue(CGQ_CYK_SHIDU_XY,AD_CYK_SHIDU); //新增采样口湿度传感器 未做滑动平均
            WenDuT1 		=	GetMeasureValue(CGQ_WENDU1_XY,AD_WENDU1);		//采样口温度值
            break;
        default:
            CYCShiDu		=	GetMeasureValue(CGQ_CYK_SHIDU_KL,AD_CYK_SHIDU); //新增采样口湿度传感器 未做滑动平均
            WenDuT1 		=	GetMeasureValue(CGQ_WENDU1_KL,AD_WENDU1);		//采样口温度值
            break;
        }

        WenDuT2 		=  	GetMeasureValue(CGQ_WENDU2,AD_WENDU2);          //DHS温度值
        WenDuT3 		=  	GetMeasureValue(CGQ_WENDU3,AD_WENDU3);		    //测试舱温度
        HumiditySample 	=  	GetMeasureValue(CGQ_SHIDU,AD_SHIDU);            //测试舱湿度
        QiYa 			=	GetMeasureValue(CGQ_QIYA,AD_QIYA);              //气压
        LiuLiang_BK		=	GetMeasureValue(CGQ_LIULIANG,AD_LIULIANG);      //流量
// 		if(LiuLiang_BK<0.3)
// 		{
// 			LiuLiang_BK = 0;
// 		}
        //工况流量=标况流量*标况压力/（273+标况温度(℃))*（273+工况温度(℃))/工况压力
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
            Rest();   //FPGA复位
            ADRangeSel(ADRANGESEL_10);     //设定ADC输入范围
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
            ClampMotor = 0;	// 维护里面的设置初始状态归位。
            OSMutexPost(OSUart0UsedMutex);

            TimeMotoCount = 18*50;
            while(TimeMotoCount)
            {
                OSTimeDlyHMSM(0, 0, 0,50);	//给点采集时间
            }
            MeasureStepFlg = 0;
            return ;
        }
    }

    //伴热带控制
    if(HeatOn)
    {//测试仓湿度大于阈值
        if((HumiditySample > Humidity ) &&(PumpFlg==0)/*泵打开*/ /*&& (abs(CYCShiDu - WenDuT3)<5)*/ && (WenDuT2<MaxHeatTemp))	 // 湿度大于阈值且采样口与检测舱温差小于5℃且DHS温度小于60℃
        //if((CYCShiDu > Humidity ) &&(PumpFlg==0)/*泵打开*/ /*&& (abs(CYCShiDu - WenDuT3)<5)*/ && (WenDuT2<60))	 // 湿度大于阈值且采样口与检测舱温差小于5℃且DHS温度小于60℃
        {
           // if(HeatFlg==1) { //伴热带关闭
                HeatFlg = 0;//打开伴热带
                OSMutexPend(OSUart0UsedMutex,0,&err);
                TTLOutput (TTL_0,HeatFlg,TTL_JIAREI);
                OSMutexPost(OSUart0UsedMutex);
           // }
        }
        else
        {
            //if(HeatFlg ==0) {
                HeatFlg = 1;//关闭伴热带加热
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
        EeorT |= 0x01;	//错误处理
    }
    if(WenDuT3<-20 || WenDuT3>100)
    {
        EeorT |= 0x04;	//错误处理
    }
    if((WenDuT2<0) || (WenDuT2>(MaxHeatTemp+10)))
    {
        EeorT |= 0x02;	//错误处理
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
    if(EEORStuation1)	  //有错
    {

        SET_LED1(1);
        if( EEORStuation1!= EEORStuationOld)//新的错误记录
        {
            //系统日志 cuowu
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
                    WriteData32(SYSRECORDPOINT2,SysrecordPoint );/*记录记录点数*/
                    OSMutexPost(OSI2CUsedMutex);
                    OSTimeDlyHMSM(0, 0, 0,300);
                }
            }
        }
    }
    else  //恢复正常。
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
    (WarnStuation != 0)?(SET_LED2(1)):(SET_LED2(0));//亮灯。
    (WarnStuation|EEORStuation1)?(SET_LED3(0)):(SET_LED3(1));
}

//自检
void 	SelfCheak(void)
{
//uint8 err;
//检测测回的数据是否合理  测电源电压确定压力传感器系数。


//	OSMutexPend(OSUart0UsedMutex,0,&err);
//	Rest();
//	TTLOutput (TTL_0,0,TTL_YAZHIMOTO);	 //。 启动
//	OSMutexPost(OSUart0UsedMutex);
//	TimeMeasCount = 8;
//	while(TimeMeasCount)
//	{
//		OSTimeDly(50);//0.25秒;
//	}
}

void  RunPaperStep(void)
{
    CPU_INT08U   err,i;
// 	if(measRecord.I>=Mass)//计数大于设定值不 需要走纸。
// 	{
// 			return ;
// 	}
    songkaiMoto();
    OSMutexPend(OSUart0UsedMutex,0,&err);
    Rotary(30); //一圈200脉冲
    MotorCtrl(0x02|DIR_RIGHT,100000);//该函数用于控制步进电机，我们用来控制直流电机 后面的参数无关紧要。
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
        PaperOverFlg = 1;//  预估纸带快用尽。
        PaperTime = PAPER_MOVE_TIME;
    }
    OSTimeDlyHMSM(0, 0, 5, 0);//延时5s估摸的时间。防止无滤纸时的错误，改为15s  改回5秒 防止散热太多。

    OSMutexPend(OSUart0UsedMutex,0,&err);
    Rotary(40); //当次就检测是否断纸或无纸。
    OSMutexPost(OSUart0UsedMutex);
    yajingMoto();

    //实时模式下处理
    if(RunStyleFlg==SHISHI)//实时模式
    {
        for(i = 0; i<16; i++)
        {
            PargRealI[i] = 0;
            PargRealTv2[i] = 0;
            PargRealTv1[i]= 0;//工况
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
    //指针被free后仍然可以被使用 但是属于违法操作 慎用 线程不安全
    return return_value;
}

//一般滤掉30个

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

    //放射源就位
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
    if((5< counterr) )	///出错了
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
        ClampMotor = 0;	// 维护里面的设置初始状态归位。
        TimeMotoCount = 18*50;
        while(TimeMotoCount)
        {
            OSTimeDlyHMSM(0, 0, 0,50);	//给点采集时间
        }
        MeasureStepFlg = 0;
        return 0;
    }
    MeasureTsS = WenDuT3;

    TimeMeasCount =  360;	  //
    //TimeMeasCount =  60;	  //测试用 计数时间短些
    while((StopFlg==0)&&TimeMeasCount&&(counterr<5))
    {
        OSTimeDly(100);//0.5秒;
        //观察调试用
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
    //计算平均值
    CountAver =  CountTotal / CountTimes;
    if(RunStyleFlg==SHISHI)//实时模式
    {
        //3分钟测量一次，15次45分钟，第一次开始测量45分钟后数据才正常。
        PargRealI[NumOfI%15] = CountAver;
        PargRealTv2[NumOfI%15] = TV_BK;
        PargRealTv1[NumOfI%15] = TV_GK;//工况

        NumOfI++;
        //计算
        if(PargRealI[NumOfI%15] == 0)//刚开始的那45分钟
        {
            Parg_x = 0;
        }
        else
        {
            Parg_x = log(PargRealI[NumOfI%15]/PargRealI[(NumOfI-1)%15])/Parg_u ;   //单位是 Mg 每cm2
        }
        Parg_m = Parg_k*Parg_x*AREA ;  ///单位mg
        Parg_v_GK = ( TV_GK-PargRealTv1[NumOfI%15] )/60.0;//LiuLiang_GK*FUJITIME; //单位L即dm2
        Parg_v_BK = ( TV_BK-PargRealTv2[NumOfI%15] )/60.0;//LiuLiangBIAO*FUJITIME; //单位L即dm2 标况的体积 看是什么地方的标准
        if(abs(Parg_v_BK - 666) > 333)//异常
        {
            Parg_v_BK = 45*14.8;//注意此处固定为45分钟。后面次数换后要改。
        }
        if(abs(Parg_v_GK - 750) > 333)//异常
        {
            Parg_v_GK = 45*16.68;//注意此处固定为45分钟。后面次数换后要改。
        }
        Parg_PM25 = 1000000.0*Parg_m/Parg_v_BK;	  //单位mg/L
        Parg_PM25_GK = 1000000.0*Parg_m/Parg_v_GK;	  //单位mg/L

        //对外输出赋值
        outputstruct1.CONC = Parg_PM25;
        measRecord.I0 = PargRealI[NumOfI%15];
        measRecord.I = PargRealI[(NumOfI-1)%15];
        measRecord.TV = Parg_v_BK;
// 	measRecord.RH  = HumiditySample;//此处的湿度和富集时的湿度不同，泵已经停了，应该在富集时采集
        measRecord.T1 = WenDuT1;
        measRecord.P= QiYa;
        measRecord.recordResultG = Parg_PM25_GK;
        measRecord.MAS = 0;//Parg_m;//温度补偿的值
        measRecord.recordResult = Parg_PM25;
        measRecord.recordTimeStart = TimeDate;
        measRecord.recordTimeEnd = TimeDate;
        //存入
        FlashDatChange(&DatForRecord[0],measRecord);
        WriteRecordFlash(recordPoint,&DatForRecord[0]);
        OSMutexPend(OSI2CUsedMutex,0,&err);
        recordPoint= recordPoint+1;	 /*记录点数加1*/
        WriteData32(RECORDPOINT2,recordPoint);/*记录记录点数*/
        OSMutexPost(OSI2CUsedMutex);

        CUpdateFlg = 1;

        //更新数据存储个数。
        usRegHoldingBuf[200] = 	recordPoint;
// 						if(measRecord.I<Mass)//计数xiaoyu设定值需要走纸。
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
            //计算
            Parg_x = log(Parg_I0/Parg_I)/Parg_u ;   //单位是 mg/cm3
            Parg_x_Recnt = log(Parg_I0_ReCnt/Parg_I_ReCnt)/Parg_u ;   //单位是 mg/cm3
            ErrorDat = 1.293*273.15*HIGH*(1/(273.15+MeasureTe)-1/(273.15+MeasureTs));//P0*T0*V/S*(1/T2-1/T1); mg/cm3
            //温度误差判断 如果大于0.05 实际温差可能有10度。则可能是出问题了，此时不加入温度补偿。
            //温度差5度 ErrDat = 0.025
            //温度差10度 ErrDat = 0.05
            //温度差15度 ErrDat = 0.075

            if( TempCompensateFlag && (abs(ErrorDat)<0.05) )
            {
                Parg_x = Parg_x - ErrorDat;
            }

            Parg_m =Parg_k* Parg_x*AREA ;  ///单位mg

            //秒累加的分钟流量换算成真实分钟累加流量
            Parg_v_GK = TV_GK/60.0;//单位L即dm3
            Parg_v_BK = TV_BK/60.0;//单位L即dm3 标况的体积 看是什么地方的标准

            Parg_PM25 = 1000000.0*Parg_m/Parg_v_BK;	  //单位mg/L
            Parg_PM25 = Parg_PM25 + Parg_b - 20;
            if(Parg_PM25<0.01)
            {
                Parg_PM25 = 6.75;//防止存储负值。
            }
            Parg_PM25_GK =  Parg_PM25*Parg_v_BK/Parg_v_GK;
            OSSchedUnlock();

            //对外输出赋值
            outputstruct1.CONC = Parg_PM25;
            //存值
            measRecord.I0 = Parg_I0;
            measRecord.I = Parg_I;
            measRecord.TV = Parg_v_BK;
// 			measRecord.RH  = HumiditySample;//此处的湿度和富集时的湿度不同，泵已经停了，应该在富集时采集
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
            //存入
            FlashDatChange(&DatForRecord[0],measRecord);
            WriteRecordFlash(recordPoint,&DatForRecord[0]);
            OSMutexPend(OSI2CUsedMutex,0,&err);
            recordPoint= recordPoint+1;	 /*记录点数加1*/
            WriteData32(RECORDPOINT2,recordPoint);
            Delay100uS(20);

            cheak =  ReadData32(RECORDPOINT2);
            if(cheak != recordPoint)
            {
                WriteData32(RECORDPOINT2,recordPoint);
            }
            /*记录记录点数*/
            OSMutexPost(OSI2CUsedMutex);
            CUpdateFlg = 1;

            //更新数据存储个数。
            usRegHoldingBuf[200] = 	recordPoint;
        }
    }
    //放射源复位
    OSMutexPend(OSUart0UsedMutex,0,&err);
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl1);
    TTLOutput (TTL_2,1,TTL_BRAY_Ctrl2);
    OSMutexPost(OSUart0UsedMutex);

    //这里应该设置为可以配置的时间
    ClampRadio = 1;
    OSTimeDlyHMSM(0, 0, C14MotMoveTime ,0);	//运行时间
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
    //开气泵
    OSMutexPend(OSUart0UsedMutex,0,&err);
    PumpFlg = 0;
    TTLOutput (TTL_0,PumpFlg,TTL_PUMP);
    //关高压
    TTLOutput (TTL_0,GAOYA_CLOSE,TTL_GAOYA);
    //设定流量
    FlowOfSamp_B_Set =  CaliParF1*(QiYa*(FlowOfSamp) * (273.15) )/((273.15+TDAIDIN)*101.325);
    DAMCSOutPutSetOne(DA_PUMP,(5.0*FlowOfSamp_B_Set/20.0));
    DAMCSOutPutAll();

    OSMutexPost(OSUart0UsedMutex);

    TV_GK =0 ;
    TV_BK =0 ;
    TimeMeasCount =30 ;	  //等稳定
    while((StopFlg==0)&&TimeMeasCount)
    {
        OSTimeDly(100);//0.5秒;
    }
    if(StopFlg == 1)
        return ;
    if(TimeType)
    {
        //TimeMeasCount = 3000;   //50分钟，加上8分钟测量 空白和样品，10多秒的点击操作，和富集稳定期。
        TimeMeasCount =(int)CycleOfSampSet*60 - 400;//57分钟
    }
    else
    {
        TimeMeasCount =(int)CycleOfSampSet*60 - 400;
    }
    FujiStartFlg =1;
    while((StopFlg==0)&&TimeMeasCount)
    {
        //显示测试
        OSTimeDlyHMSM(0, 0, 10,0);	//10秒;
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
        //在 快结束时开启高压，提前1分钟，注意在最小12分钟进程时的处理
        if((TimeMeasCount<60)&&(TimeMeasCount>0))
        {
            OSMutexPend(OSUart0UsedMutex,0,&err);
            TTLOutput (TTL_0,GAOYA_OPEN,TTL_GAOYA);
            OSMutexPost(OSUart0UsedMutex);
        }

    }
    FujiStartFlg =0;

    HeatFlg = 1;//关闭伴热带加热
    OSMutexPend(OSUart0UsedMutex,0,&err);
    PumpFlg = 1;
    TTLOutput (TTL_0,PumpFlg,TTL_PUMP);
    TTLOutput (TTL_0,HeatFlg,TTL_JIAREI);
    //开高压
    TTLOutput (TTL_0,GAOYA_OPEN,TTL_GAOYA);
    OSMutexPost(OSUart0UsedMutex);
    MeaserEndTime = TimeDate;//记录测试时时间点。
    if(StopFlg == 1)
    {
        return ;
    }
    OSTimeDlyHMSM(0, 0, 15,0);	//给点时间让泵完全停下
}

void songkaiMoto(void)
{
    CPU_INT08U   err;
    if(ClampMotor==0)
    {
        OSMutexPend(OSUart0UsedMutex,0,&err);
        TTLOutput (TTL_0,0,TTL_YAZHIMOTO);	 //启动
        OSMutexPost(OSUart0UsedMutex);
        ClampMotor = 1;	 //状态改为松开。
        TimeMotoCount = MotorTime*50;  //7s
        while(TimeMotoCount)
        {
            OSTimeDly(50);//秒;
        }
        OSMutexPend(OSUart0UsedMutex,0,&err);
        TTLOutput (TTL_0,1,TTL_YAZHIMOTO);	 //停止
        OSMutexPost(OSUart0UsedMutex);
    }
}

void yajingMoto(void)
{   CPU_INT08U   err;

    if(ClampMotor==1)
    {
        OSMutexPend(OSUart0UsedMutex,0,&err);
        TTLOutput (TTL_0,0,TTL_YAZHIMOTO);	 //启动
        OSMutexPost(OSUart0UsedMutex);
        ClampMotor = 0;	 //状态改为压紧。
        TimeMotoCount = 8*50; //没个20ms 1s要50个    一圈9秒左右         10s
        while(TimeMotoCount)
        {
            OSTimeDly(50);//0.25秒;
        }
        OSMutexPend(OSUart0UsedMutex,0,&err);
        TTLOutput (TTL_0,1,TTL_YAZHIMOTO);	 //。停止
        OSMutexPost(OSUart0UsedMutex);
    }

}





