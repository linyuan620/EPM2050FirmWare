/****************************************Copyright (c)****************************************************
**                            skyray-instrument Co.,LTD.
**
**                                 http://www.skyray-instrument.com
**
*********************************************************************************************************/

#include "init.h"
//Watchodog time out in 5 seconds
#define WDT_TIMEOUT 	7000000
// #define TestV 100.0f
fp32  measResult=10.0;
volatile uint32 recordPoint = 0;  //��¼��
volatile uint32 SysrecordPoint = 0;  //��¼��
uint32 recordOutput;
uint8 FujiStartFlg,keyBuzzerBitFlg = 1;
uint8 LCDContrast = 3;
uint8 SensorSel = 0;// 0 ���غ��� 1 ����
float MotorTime=11.0;
//����ʱ�䵹��ʱ
int TimeMeasCount=0;
int TimeCount1s;
//����ʱ�䵹��ʱ
int TimeKeyCount=0;
int TimeMotoCount ;
TimeDat MeaserStartTime;
TimeDat MeaserEndTime;

TimeDat TimeDate= {
    6,
    9,
    1,
    8,
    0x30,
    1
};


/*********************************************************************************************************
* Function name: MachineFristRun
* Descriptions:  ��ʼ��Щ��������

* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
void MachineFristRun(uint8 run_type)
{
    int i;
    if((C1025_ReadByte(INIT_FLAG)!=0x4f)||(run_type == 2))
    {
        WriteData32(RECORDPOINT2,0);
        Delay100uS(20);
        WriteData32(SYSRECORDPOINT2,0);
        Delay100uS(20);

        //�������flash������
        SSTF016B_Erase(((RECORDFLASHSTAR)/SEC_SIZE), ((0xb2000)/SEC_SIZE));//������ʷ��¼��ϵͳ��־�Ĳ��ֲ�����

        //��������
        C1025_WriteByte(ConInf,0); //��ֽʣ��
        C1025_WriteByte(ConInf+2,1); //��������
        C1025_WriteByte(ConInf+3,0); //������
        C1025_WriteByte(ConInf+4,0); //����
        C1025_WriteByte(ConInf+5,0); //yontu

        //��������ֵ
        C1025_WriteByte(RUNSETADD8,1);	//����ģʽ  ����/ʵʱ
        C1025_WriteByte(RUNSETADD8+1,0); //	���ȿ���
        C1025_WriteByte(RUNSETADD8+2,1); //����ģʽ
        C1025_WriteByte(RUNSETADD8+3,1); //�¶Ȳ���
        C1025_WriteByte(SENSOR_SEL_8ADDR,0); //������ʪ��ѡ��

        WriteFp32(RUNSETADD32,35.0); //ʪ���ŷ�
        WriteFp32(RUNSETADD32+4,60.0);//������ �¶�
        WriteFp32(RUNSETADD32+8,0.0);	// ��С�����¶�
        WriteFp32(RUNSETADD32+12,16.67);// ��������
        WriteFp32(RUNSETADD32+16,53.0);	// ��������
        WriteFp32(RUNSETADD32+20,5.0);	//�������ʱ��
        WriteFp32(RUNSETADD32+24,4000.0);	//��������
        WriteFp32(PM25_GS_32ADDR,4.1);	//��ɢpm2.5ϵ��
        WriteFp32(PM10_GS_32ADDR,9.2);	//��ɢpm10ϵ��
        WriteFp32(TSP_GS_32ADDR,14.3);	//��ɢtspϵ��

        //�����A���� 8λ����
        C1025_WriteByte(RESERVED_8,0);	// Parg_b

        //�����A���� 32λ����

        WriteFp32(RESERVED_32,1.25);	//�������ʱ��Parg_k

        //У׼��ز���
        WriteFp32(JIAOZHUN_T1,1);
        WriteFp32(JIAOZHUN_T3,1);
        WriteFp32(JIAOZHUN_RH,1);
        WriteFp32(JIAOZHUN_P,1);
        WriteFp32(JIAOZHUN_F1,1.05);
        WriteFp32(JIAOZHUN_BIAOPIAN,0.303); //u
        WriteFp32(JIAOZHUN_T4,1);
        WriteFp32(JIAOZHUN_RH2,1);
        language = 0;
        NePrOffset = (language)?(71):(0); 	// Ӣ��	����
        keyBuzzerBitFlg=C1025_ReadByte(ConInf+2)&0x01;
        KeyLockFlg=C1025_ReadByte(ConInf+3)&0x01;
        FilterRemain = C1025_ReadByte(ConInf);
        SetVLCD(3);
        SetBuzzer(keyBuzzerBitFlg);
        SET_LCD_BL(1);

        //�������ò���
        //8wei
        RunStyleFlg	 =C1025_ReadByte(RUNSETADD8)&0x01;
        HeatOn =  C1025_ReadByte(RUNSETADD8+1)&0x01;
        TimeType = C1025_ReadByte(RUNSETADD8+2)&0x01;
        TempCompensateFlag = C1025_ReadByte(RUNSETADD8+3)&0x01;
        SensorSel = C1025_ReadByte(SENSOR_SEL_8ADDR)&0x01;

        Humidity = ReadFp32(RUNSETADD32);
        MaxHeatTemp = ReadFp32(RUNSETADD32+4);
        MinHeatPower = ReadFp32(RUNSETADD32+8);
        FlowOfSamp = ReadFp32(RUNSETADD32+12);
        CycleOfSampSet =  ReadFp32(RUNSETADD32+16);
        CycleOfSamp = CycleOfSampSet;

        PM25_GS_k =  ReadFp32(PM25_GS_32ADDR);
        PM10_GS_k =  ReadFp32(PM10_GS_32ADDR);
        TSP_GS_k =  ReadFp32(TSP_GS_32ADDR);

        C1025_WriteByte(INIT_FLAG,0x4f);
    }
}


/*********************************************************************************************************
* Function name:     ComeBackConfig
* Descriptions:      ��ʱ�ǰѴ洢����������
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
void ComeBackConfig(uint8 type)
{
    uint8 *p,i;
    uint32 cheak;
    uint8 wr[6] = {0x55,0x55,0x55,0xaa,0xaa,0xaa};
    uint8 wr1[6] = {1,2,3,4,5,6};

    uint8 rd[6] = {0,0,0,0,0,0};
    p = IP_Addr;

    if((1==type)||(2==type)||(3==type))
    {
        SysrecordPoint = ReadData32(SYSRECORDPOINT2);
        Delay100uS(20);
        recordPoint = ReadData32(RECORDPOINT2);
        if(recordPoint>50000)//�쳣ֵ ����7��������ˡ�
        {
            for(i = 0; (recordPoint>50000&&i<2); i++)
            {
                recordPoint = ReadData32(RECORDPOINT2);
                Delay100uS(20);
            }
            if(recordPoint>50000)//�쳣ֵ ����7��������ˡ�
            {
                recordPoint = 0;
                //�������flash������
                SSTF016B_Erase(((RECORDFLASHSTAR)/SEC_SIZE), ((0xb2000)/SEC_SIZE));//������ʷ��¼��ϵͳ��־�Ĳ��ֲ�����
                //���� 0x5d000---0xb0000�Ŀռ䡣
                SysrecordPoint = 0;
            }
        }

        /*�����������*/
        FilterRemain = C1025_ReadByte(ConInf);
        keyBuzzerBitFlg=C1025_ReadByte(ConInf+2)&0x01;
        KeyLockFlg=C1025_ReadByte(ConInf+3)&0x01;
        UseFor=C1025_ReadByte(ConInf+5)&0x03;
        if(UseFor>2)
        {
            UseFor =1;
        }

        //ipֵ
        ip.first = C1025_ReadByte(ConInf+6);
        ip.second = C1025_ReadByte(ConInf+7);
        ip.third = C1025_ReadByte(ConInf+8);
        ip.end = C1025_ReadByte(ConInf+9);

        *p = ip.first;
        p++;
        *p = ip.second;
        p++;
        *p = ip.third;
        p++;
        *p = ip.end;
        //Ӳ����ַҪ���Ÿģ�������ͬ ��ͬһ������
        Phy_Addr[5] = ip.end;

        LCDContrast =3;

        SetVLCD(LCDContrast);
        SetBuzzer(keyBuzzerBitFlg);
        SET_LCD_BL(1);

        //�������ò���

        //8wei
        RunStyleFlg	 =C1025_ReadByte(RUNSETADD8)&0x01;
        HeatOn =  C1025_ReadByte(RUNSETADD8+1)&0x01;
        TimeType = C1025_ReadByte(RUNSETADD8+2)&0x01;
        TempCompensateFlag = C1025_ReadByte(RUNSETADD8+3)&0x01;
        SensorSel = C1025_ReadByte(SENSOR_SEL_8ADDR)&0x01;

        //32wei
        Humidity = ReadFp32(RUNSETADD32);
        MaxHeatTemp = ReadFp32(RUNSETADD32+4);
        MinHeatPower = ReadFp32(RUNSETADD32+8);
        FlowOfSamp = ReadFp32(RUNSETADD32+12);
        CycleOfSampSet =  ReadFp32(RUNSETADD32+16);
        MotorTime =  ReadFp32(RUNSETADD32+20);//�������ʱ��
        Mass =  ReadFp32(RUNSETADD32+24);

        PM25_GS_k = ReadFp32(PM25_GS_32ADDR);
        PM10_GS_k = ReadFp32(PM10_GS_32ADDR);
        TSP_GS_k = ReadFp32(TSP_GS_32ADDR);
        CycleOfSamp = CycleOfSampSet;

        Parg_b = C1025_ReadByte(RESERVED_8)&0xff;;
        Parg_k = ReadFp32(RESERVED_32);
        ComminiStyle=C1025_ReadByte(RESERVED_8+1)&0x01;//����������������Ǹ�ff

        if((Parg_k>2)||(Parg_k<0.5))
        {
            Parg_k = 1.25;
        }

        //У׼����
        if(JIAOZHUN_ON)
        {
            CaliParT1 	=  	ReadFp32(JIAOZHUN_T1);
            CaliParT3 	=	ReadFp32(JIAOZHUN_T3);
            CaliParRH 	=	ReadFp32(JIAOZHUN_RH);
            CaliParP  	=	ReadFp32(JIAOZHUN_P);
            CaliParF1 	=	ReadFp32(JIAOZHUN_F1);
            Parg_u    	=  	ReadFp32(JIAOZHUN_BIAOPIAN);
            CaliParT4 	=  	ReadFp32(JIAOZHUN_T4);
            CaliParRH2	=  	ReadFp32(JIAOZHUN_RH2);
            //���غ��������ǹ���һ��У׼�ռ�
            SetCaliDat(CGQ_WENDU1_KL,CaliParT1);
            SetCaliDat(CGQ_WENDU1_XY,CaliParT1);

            SetCaliDat(CGQ_WENDU3,CaliParT3);
            SetCaliDat(CGQ_TCQ_WENDU,CaliParT4);
            SetCaliDat(CGQ_SHIDU,CaliParRH) ;

            SetCaliDat(CGQ_CYK_SHIDU_KL,CaliParRH2) ;
            SetCaliDat(CGQ_CYK_SHIDU_XY,CaliParRH2) ;

            SetCaliDat(CGQ_QIYA,CaliParP) ;
            SetCaliDat(CGQ_LIULIANG,(1/CaliParF1));
        }
        else
        {
            SetCaliDat(CGQ_WENDU1_KL,1);
            SetCaliDat(CGQ_WENDU1_XY,1);

            SetCaliDat(CGQ_WENDU2,1);
            SetCaliDat(CGQ_SHIDU,1) ;
            SetCaliDat(CGQ_QIYA,1) ;
            SetCaliDat(CGQ_LIULIANG,1);
            TimeDate.year = 2013;
            TimeDate.month = 3;
            TimeDate.date = 23;

            WriteCaliParTime(JIAOZHUN_T1_TIME,&TimeDate);
            WriteCaliParTime(JIAOZHUN_T3_TIME,&TimeDate);
            WriteCaliParTime(JIAOZHUN_RH_TIME,&TimeDate);
            WriteCaliParTime(JIAOZHUN_P_TIME,&TimeDate);
            WriteCaliParTime(JIAOZHUN_F1_TIME,&TimeDate);
            WriteCaliParTime(JIAOZHUN_F2_TIME,&TimeDate);
            WriteCaliParTime(JIAOZHUN_BIAOPIAN_TIME,&TimeDate);
        }
    }
}

/*********************************************************************************************************
* Function name:        IInitDevice1
* Descriptions:         ��һҪ��ʼ���Ķ���
* input parameters:     ��
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void InitDevice1(void)
{
    uint8 i;
		uint32_t tmp;
    KeyIOInit();   						//����
    LEDIOInit();						//LED
    I2C1Init();			 				//I2C
    BuzzerIOInit(1);		 			//����
    SetBuzzer(0);		  				//�ط���
    SET_LCD_BL(1);		   				//����
    LCDIOIint();			   			//LCD�˿�
    Time0Init();			   			//TIME 0 1 2
    MBMasterIni();						//modubs2������ʼ��
    Time2Init();						//10ms	Time2Init(0);//0.5���ַ���ʱ MODBUS
    RTCInit();				   			//RTC
    InitLCD1698();						//LCD
    LcdClr();					 		//����
    ClrDisBuf();
    SPIInit();

    TimeKeyCount = 1*50;
    while(TimeKeyCount);
    Uart0Init(9600,UART_PARITY_NONE,UART_STOPBIT_1);  //FPGA��

    //DTU ��SHELL ����
    //Uart1Init(115200,UART_PARITY_NONE,UART_STOPBIT_1);  //DTU


    //����Ϊ����ͨ��
    //CommunicationSet(UART_STYLE);
    Sent_Buffer[0] = START;
    Sent_Buffer[1] = BUSSEL;
    Sent_Buffer[2] = 1;		 //length
    Sent_Buffer[3] = UART_STYLE;
    Sent_Buffer[4] = STOP;
    UART_Send(LPC_UART0, Sent_Buffer, 5,BLOCKING);
    Delay100uS(20);	//�����ӳٻ�ʹϵͳ���𲻳���һ֡����һ֡
    //��λ
    Sent_Buffer[0] = START;
    Sent_Buffer[1] = FPGA_RESET;
    Sent_Buffer[2] = 0;
    Sent_Buffer[3] = STOP;
    UART_Send(LPC_UART0, Sent_Buffer, 4,BLOCKING);

    Uart3Init(9600,UART_PARITY_NONE,UART_STOPBIT_1);  //MODBUS ����

    language=C1025_ReadByte(ConInf+4);

    NePrOffset = (language)?(71):(0); 	// Ӣ��	����
    if(language == 1)
    {
        Write240x160(pic_240x160_en);			//��ͼ240*160
    }
    else
    {
        Write240x160(pic_240x160);			//��ͼ240*160
    }


    LcdUpdata(DisBuf);

//	ADRangeSel(ADRANGESEL_10);	//AD���Է�Χ
    Sent_Buffer[0] = START;
    Sent_Buffer[1] = ADRANGESEL;
    Sent_Buffer[2] = 1;
    Sent_Buffer[3] = ADRANGESEL_10;
    Sent_Buffer[4] = STOP;
    UART_Send(LPC_UART0, Sent_Buffer, 5,BLOCKING);

    MachineFristRun(1);					//��һ����		�ָ�����
    ComeBackConfig(1);					// ��ȡ�˵�����
    //WriteCode1212(5,147,NextPrompt[5+NePrOffset]); //���ڸ�λ���   �˴��õ���flsh �ں���Դ���� ����δ����ʱʹ����bug
    LcdUpdata(DisBuf);

    if(!ComminiStyle)
    {
        SPIInit_w5100();															/* ��ʼ��SPI�ӿ�*/
        W5100_Init();
        Socket_Init(0);
        Socket_Listen(0);
    }

    //�������
    TTLOutPut[TTL_YAZHIMOTO] = 0; //����ѹֽ���
    TTLOutPut[TTL_PUMP] = 1;      //�ر�����
    TTLOutPut[TTL_JIAREI] = 1;    //�رռ��ȴ�
    //������������ѹ
    TTLOutPut[TTL_GAOYA] = GAOYA_OPEN; //

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

    WriteData32(TESTADD,0xaa);
    Delay100uS(20);
		tmp = ReadData32(TESTADD);
    if(ReadData32(TESTADD) != 0xaa)
    {
        EeorMenmory  |= 0x01;
    }

    ClampMotor = 0;	// ά����������ó�ʼ״̬��λ��
    TimeKeyCount = 18*50;
    while(TimeKeyCount);
    ClrDisBuf();
    LcdUpdata(DisBuf);

    __BUF_RESET(rb.rx_head);
    __BUF_RESET(rb.rx_tail);
    __BUF_RESET(rb.tx_head);
    __BUF_RESET(rb.tx_tail);

}

/*********************************************************************************************************
* Function name:        IInitDevice2
* Descriptions:         �ڶ�Ҫ��ʼ���Ķ���
* input parameters:     ��
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void InitDevice2(void)
{
    uint8 err;

    //�ϵ翪��д��������־
    goSysRecor.recordTime = TimeDate;
    goSysRecor.Event = EVENTMEASURE;
    goSysRecor.ContentH =0;//(uint8)((EEORStuation1|0X0800)>>8);
    goSysRecor.ContentL = 1;//(uint8)(EEORStuation1|0X0800);
    FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
    WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
    OSMutexPend(OSI2CUsedMutex,0,&err);
    SysrecordPoint +=1;
    SysrecordReadFlg++;
    WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
    OSMutexPost(OSI2CUsedMutex);
}


