/****************************************Copyright (c)****************************************************
**                            Skyray Instrment Co.,LTD.
**
**                         http://www.skyray-instrument.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           main.C
** Last modified Date:  2011-8-3 10:52:24
** Last Version:        V2.5
** Descriptions:        主函数，便携式重金属检测仪
**
*********************************************************************************************************/

#include 	"global.h"

float  		Wait_Flg = 0;
OS_EVENT 	*OSLcdUsedMutex		=	NULL;	//互斥信号量
OS_EVENT 	*OSI2CUsedMutex		=	NULL;
OS_EVENT 	*OSUart0UsedMutex	=	NULL;	//互斥信号量
OS_EVENT 	*OSFlashUsedMutex	=	NULL;	//互斥信号量

/*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
										任务堆栈申请
*********************************************************************************************************/
static  OS_STK         AppTask_Start_Stk[APP_TASK_START_STK_SIZE];
static  OS_STK         AppTask_ModbusSlave_Stk[APP_TASK_ModbusSlave_STK_SIZE];
static  OS_STK         AppTask_Meas_Stk[APP_TASK_MEAS_STK_SIZE];
static  OS_STK         AppTask_HistoryMemoryRead_Stk[APP_TASK_HistoryMemoryRead_STK_SIZE];
static  OS_STK         AppTask_DataGather_PineTempCtrl_Stk[APP_TASK_DataGather_PineTempCtrl_STK_SIZE];
static  OS_STK         AppTask_Kbd_Stk[APP_TASK_KBD_STK_SIZE];
static  OS_STK         AppTask_ModbusMaster_Stk[APP_TASK_ModbusMaster_STK_SIZE];
static  OS_STK         AppTask_Refresh_Lcd_AnalogOutput_Stk[APP_TASK_Refresh_Lcd_AnalogOutput_STK_SIZE];
static  OS_STK         AppTask_DTU_Stk[APP_TASK_DTU_STK_SIZE];
static  OS_STK         AppTask_SHELL_Stk[APP_TASK_SHELL_STK_SIZE];

/*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************/

static  void  AppTask_Create       						(void);

static  void  AppTask_Start        						(void        *p_arg);
static  void  AppTask_ModbusSlave      					(void        *p_arg);
static  void  AppTask_Meas         						(void        *p_arg);
static  void  AppTask_HistoryMemoryRead          		(void        *p_arg);
static  void  AppTask_Kbd          						(void        *p_arg);
static  void  AppTask_DataGather_PineTempCtrl 	   		(void 		 *p_arg);
static  void  AppTask_Refresh_Lcd_AnalogOutput 	   		(void 		 *p_arg);
static  void  AppTask_DTU 	   		            		(void 		 *p_arg);
static  void  AppTask_SHELL 	   		            		(void 		 *p_arg);

/*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************/
int main (void)
{

    CPU_INT08U  os_err =   os_err;
    /*
    Disable all the interrupts
    创建空闲任务，和计数任务
    Initialize "uC/OS-II, The Real-Time Kernel".
    */
    CPU_IntDis();
    OSInit();
    /* Create the start task.*/
    os_err = OSTaskCreateExt((void (*)(void *)) AppTask_Start,
                             (void          * ) 0,
                             (OS_STK        * )&AppTask_Start_Stk[APP_TASK_START_STK_SIZE - 1],	 //栈顶
                             (INT8U           ) APP_TASK_START_PRIO,							 //优先级
                             (INT16U          ) APP_TASK_START_PRIO,							 //
                             (OS_STK        * )&AppTask_Start_Stk[0],							 //栈尾
                             (INT32U          ) APP_TASK_START_STK_SIZE,						 //申请的堆栈大小
                             (void          * )0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));	 //堆栈是否清零，是否使能堆栈使用情况检测
    /*开始任务调度*/
    OSStart();               /* Start multitasking (i.e. give control to uC/OS-II).  */

    return (0);
}

/*********************************************************************************************************
*                                          AppTask_Start()
*
* Description : The startup task.  The uC/OS-II ticker should only be initialize once multitasking starts.
*
* Argument(s) : p_arg       Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************/

static  void  AppTask_Start (void *p_arg)
{
    CPU_INT08U   err;
    /*
    防编译出错
    Initialize BSP functions.
    包括USART2 Timer2都在里面初始化了
    modus相关初始化
    */
    (void)p_arg;
    InitDevice1();

    //与pc机通信方式选择
    if (!ComminiStyle)
    {
        eMBTCPInit(0);
    }
    else if (ComminiStyle)
    {
        eMBInit(MB_RTU, 0x01, 1, 19200, MB_PAR_NONE);
    }
    eMBEnable();
    /*
    ucosII节拍定时器初始化，保证1s对应OS_TICKS_PER_SEC个节拍
    其中 SystemCoreClock由 system_LPC17XX.C配置  本系统配置为72M
    2级初始化
    */
    SysTick_Config(SystemCoreClock/OS_TICKS_PER_SEC - 1);

#if (OS_TASK_STAT_EN > 0)
    /* 系统任务初始化 Determine CPU capacity.*/
    OSStatInit();
#endif

#if ((APP_PROBE_COM_EN == DEF_ENABLED) || \
     (APP_OS_PROBE_EN  == DEF_ENABLED))
#endif
    /*
    信号量 LCD更新显示，由按键，rtc促发。
    LCD互斥资源建立并且建后可用
    I2C互斥资源建立并且建后可用
    */
    OSFlashUsedMutex	=	OSMutexCreate(MUTEX_FLASHUSED_PRIO,&err);
    OSUart0UsedMutex	=	OSMutexCreate(MUTEX_UART0USED_PRIO,&err);
    OSLcdUsedMutex		=	OSMutexCreate(MUTEX_LCDUSED_PRIO,&err);
    OSI2CUsedMutex		=	OSMutexCreate(MUTEX_I2CUSED_PRIO,&err);
    /*
    建立个邮箱用于ubd，UserIF通信 程序测试用
    创建其他几个任务
    3级初始化
    蜂鸣 表示初始化完成
    */

    InitDevice2();
    AppTask_Create();
    BuzzerCtrl(2,5,5);

    /*程序运行灯*/
    while (DEF_TRUE)
    {
        OSTimeDlyHMSM(0, 0, 3, 0);
    }
}


/*********************************************************************************************************
*                                            AppTask_Create()
*
* Description : Create the application tasks.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : App_Task_Start().
*
* Note(s)     : none.
*********************************************************************************************************/
static  void  AppTask_Create (void)
{
    CPU_INT08U  os_err = os_err;
    /*ModBus从机任务任务*/    //与上位通信
    os_err = OSTaskCreateExt((void (*)(void *)) AppTask_ModbusSlave,
                             (void          * ) 0,
                             (OS_STK        * )&AppTask_ModbusSlave_Stk[APP_TASK_ModbusSlave_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_ModbusSlave_PRIO,
                             (INT16U          ) APP_TASK_ModbusSlave_PRIO,
                             (OS_STK        * )&AppTask_ModbusSlave_Stk[0],
                             (INT32U          ) APP_TASK_ModbusSlave_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
    /*按键任务*/
    os_err = OSTaskCreateExt((void (*)(void *)) AppTask_Kbd,
                             (void          * ) 0,
                             (OS_STK        * )&AppTask_Kbd_Stk[APP_TASK_KBD_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_KBD_PRIO,
                             (INT16U          ) APP_TASK_KBD_PRIO,
                             (OS_STK        * )&AppTask_Kbd_Stk[0],
                             (INT32U          ) APP_TASK_KBD_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
    /*测量任务*/    //测试流程控制
    os_err = OSTaskCreateExt((void (*)(void *)) AppTask_Meas,
                             (void          * ) 0,
                             (OS_STK        * )&AppTask_Meas_Stk[APP_TASK_MEAS_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_MEAS_PRIO,
                             (INT16U          ) APP_TASK_MEAS_PRIO,
                             (OS_STK        * )&AppTask_Meas_Stk[0],
                             (INT32U          ) APP_TASK_MEAS_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

    /*数据上传响应任务*/     //负责响应上位机读取数据事件
    os_err = OSTaskCreateExt((void (*)(void *)) AppTask_HistoryMemoryRead,
                             (void          * ) 0,
                             (OS_STK        * )&AppTask_HistoryMemoryRead_Stk[APP_TASK_HistoryMemoryRead_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_HistoryMemoryRead_PRIO,
                             (INT16U          ) APP_TASK_HistoryMemoryRead_PRIO,
                             (OS_STK        * )&AppTask_HistoryMemoryRead_Stk[0],
                             (INT32U          ) APP_TASK_HistoryMemoryRead_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

    /*数据采集及伴热带温度控制任务*/      //负责读取计数器值，周期更新ADC采样值及周期温度检测及温度控制
    os_err = OSTaskCreateExt((void (*)(void *)) AppTask_DataGather_PineTempCtrl,
                             (void          * ) 0,
                             (OS_STK        * )&AppTask_DataGather_PineTempCtrl_Stk[APP_TASK_DataGather_PineTempCtrl_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_DataGather_PineTempCtrl_PRIO,
                             (INT16U          ) APP_TASK_DataGather_PineTempCtrl_PRIO,
                             (OS_STK        * )&AppTask_DataGather_PineTempCtrl_Stk[0],
                             (INT32U          ) APP_TASK_DataGather_PineTempCtrl_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
    /*modbus主机任务*/  //modbus主机任务负责和接口板通信
//    os_err = OSTaskCreateExt((void (*)(void *)) OSModbusServe,
//                             (void          * ) 0,
//                             (OS_STK        * )&AppTask_ModbusMaster_Stk[APP_TASK_ModbusMaster_STK_SIZE - 1],
//                             (INT8U           ) APP_TASK_ModbusMaster_PRIO,
//                             (INT16U          ) APP_TASK_ModbusMaster_PRIO,
//                             (OS_STK        * )&AppTask_ModbusMaster_Stk[0],
//                             (INT32U          ) APP_TASK_ModbusMaster_STK_SIZE,
//                             (void          * ) 0,
//                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
    /*LCD首页界面刷新及模拟输出刷新*/
    os_err = OSTaskCreateExt((void (*)(void *)) AppTask_Refresh_Lcd_AnalogOutput,
                             (void          * ) 0,
                             (OS_STK        * )&AppTask_Refresh_Lcd_AnalogOutput_Stk[APP_TASK_Refresh_Lcd_AnalogOutput_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_Refresh_Lcd_AnalogOutput_PRIO,
                             (INT16U          ) APP_TASK_Refresh_Lcd_AnalogOutput_PRIO,
                             (OS_STK        * )&AppTask_Refresh_Lcd_AnalogOutput_Stk[0],
                             (INT32U          ) APP_TASK_Refresh_Lcd_AnalogOutput_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
    /*DTU输数据透传 */
//    os_err = OSTaskCreateExt((void (*)(void *)) AppTask_DTU,
//                             (void          * ) 0,
//                             (OS_STK        * )&AppTask_DTU_Stk[APP_TASK_DTU_STK_SIZE - 1],
//                             (INT8U           ) APP_TASK_DTU_PRIO,
//                             (INT16U          ) APP_TASK_DTU_PRIO,
//                             (OS_STK        * )&AppTask_DTU_Stk[0],
//                             (INT32U          ) APP_TASK_DTU_STK_SIZE,
//                             (void          * ) 0,
//                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
    /*SHELL人机串口交互*/
//    os_err = OSTaskCreateExt((void (*)(void *)) AppTask_SHELL,
//                             (void          * ) 0,
//                             (OS_STK        * )&AppTask_SHELL_Stk[APP_TASK_SHELL_STK_SIZE - 1],
//                             (INT8U           ) APP_TASK_SHELL_PRIO,
//                             (INT16U          ) APP_TASK_SHELL_PRIO,
//                             (OS_STK        * )&AppTask_SHELL_Stk[0],
//                             (INT32U          ) APP_TASK_SHELL_STK_SIZE,
//                             (void          * ) 0,
//                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
}


/*********************************************************************************************************
*                                            AppTask_ModbusSlave()
*
* Description :     //与上位机通信任务
*
* Argument(s) : p_arg
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************/

static  void  AppTask_ModbusSlave (void *p_arg)
{
    (void)p_arg;
    while (DEF_TRUE)
    {
        /*modbus 通信50ms 通讯一次*/
        //与pc机通信方式选择
        if (!ComminiStyle)
        {
            //网口中断查询
            if(WEBxintiao > 60 )//2分钟没传来数据  关闭网络连接
            {
                WEBxintiao = 0;
                Write_W5100(W5100_S0_CR, S_CR_CLOSE);		/* 关闭端口，等待重新打开连接 */
                S0_State=0;
                Close_Flg =1;
            }
            W5100_Interrupt_Process();
            if(Close_Flg == 1)
            {
                Socket_Listen(0);
                Close_Flg=0;
            }
            if(S0_State == S_CONN)   //连接成功标志
            {
                S0_State = 0;
            }
            if((S0_Data&S_RECEIVE)==S_RECEIVE)
            {
                S0_Data = 0;			//清置位标志
                LastResDataLen = S_rx_process(0);
                xMBPortEventPost( EV_FRAME_RECEIVED );
            }
        }
        eMBPoll();
        OSTimeDlyHMSM(0, 0, 0,10);
    }
}


/*********************************************************************************************************
*                                            AppTask_Kbd()  //按键任务
*
* Description : Monitor the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************/
static  void  AppTask_Kbd (void *p_arg)
{
    CPU_INT08U   err;
    (void)p_arg;
    while (DEF_TRUE)
    {
        OSMutexPend(OSLcdUsedMutex,0,&err);
        OSMutexPend(OSI2CUsedMutex,0,&err);
        PorcKey(1);
        OSMutexPost(OSI2CUsedMutex);
        OSMutexPost(OSLcdUsedMutex);
        OSTimeDly(10);
    }
}


/*********************************************************************************************************
*                                            AppTask_Meas()
*
* Description : 	 测量任务
*
* Argument(s) : p_arg
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************/
static  void  AppTask_Meas (void *p_arg)
{
    uint8 err;
    (void)p_arg;
    while (DEF_TRUE)
    {
        switch(MeasureStepFlg)
        {
        case 0:  //自检及初始化
            //整点启动
            if(StopFlg == 0)   //启动后
            {
                //关泵
                OSMutexPend(OSUart0UsedMutex,0,&err);
                PumpFlg = 1;
                TTLOutput (TTL_0,PumpFlg,TTL_PUMP);	 //。
                OSMutexPost(OSUart0UsedMutex);

                //放射源复位
                OSMutexPend(OSUart0UsedMutex,0,&err);
                TTLOutput (TTL_2,0,TTL_BRAY_Ctrl1);	 //。
                TTLOutput (TTL_2,1,TTL_BRAY_Ctrl2);	 //。
                OSMutexPost(OSUart0UsedMutex);
                ClampRadio = 1;
                OSTimeDlyHMSM(0, 0, C14MotMoveTime,0);	//运行时间

                OSMutexPend(OSUart0UsedMutex,0,&err);
                TTLOutput (TTL_2,0,TTL_BRAY_Ctrl1);	 //。
                TTLOutput (TTL_2,0,TTL_BRAY_Ctrl2);	 //。
                OSMutexPost(OSUart0UsedMutex);

                while((TimeType==1 )&&(TimeDate.minute != 0)&&(StopFlg == 0))   //如果是整点模式，则等待系统时钟分钟数为0
                {
                    OSTimeDlyHMSM(0, 0, 10,0);
                }
                //走纸
                if(StopFlg == 0)
                {
                    RunPaperStep();
                }
            }
            (!StopFlg)?(MeasureStepFlg++):(MeasureStepFlg = 10);
            break;
        case 1://测空白 或者重复测试
            showFlg = 0;//正常出值，不用再提示。
            if(RunStyleFlg == SHISHI)
            {
                //开泵运行8秒  测空白
                OSMutexPend(OSUart0UsedMutex,0,&err);
                PumpFlg = 0;
                TTLOutput (TTL_0,PumpFlg,TTL_PUMP);	 //。
                FlowOfSamp_B_Set =  CaliParF1*(QiYa*(FlowOfSamp) * (273.15) )/((273.15+TDAIDIN)*101.325);
                DAMCSOutPutSetOne(DA_PUMP,(5.0*FlowOfSamp_B_Set/20.0));
                DAMCSOutPutAll();
                OSMutexPost(OSUart0UsedMutex);
                TimeMeasCount =8 ;	  //等稳定
                while((StopFlg==0)&&TimeMeasCount)
                {
                    OSTimeDly(100);//0.5秒;
                }
                FujiStartFlg =1;
                MeasureStep();
                (!StopFlg)?(MeasureStepFlg =2):(MeasureStepFlg = 10);
                if(measRecord.I<Mass)//计数小于设定值需要走纸。
                {
                    MeasureStepFlg = 4;
                    OSMutexPend(OSUart0UsedMutex,0,&err);
                    PumpFlg = 1;//关闭泵
                    TTLOutput (TTL_0,PumpFlg,TTL_PUMP);	 //。
                    OSMutexPost(OSUart0UsedMutex);
                    FujiStartFlg =0;
                    TV_GK =0 ;
                    TV_BK =0	;
                }
            }
            else if(RunStyleFlg == ZHOUQI)
            {
                MeasureStep();
                (!StopFlg)?(MeasureStepFlg++):(MeasureStepFlg = 10);
            }
            break;
        case 2 :  //富集
            FujiStep();
            (!StopFlg)?(MeasureStepFlg++):(MeasureStepFlg = 10);
            break;
        case 3:	//测样品
            MeasureStep();
            (!StopFlg)?(MeasureStepFlg++):(MeasureStepFlg = 10);
            break;
        case 4://走纸 ：包括 抬起压制电机， 走纸x毫米，放下压纸电机，
            OSTimeDlyHMSM(0, 0, 15,50);      //  防止在非整点模式时直接测量空白样
            (StopFlg)?(MeasureStepFlg++):(MeasureStepFlg = 10);
            if(MeasureStepFlg>4)
            {
                MeasureStepFlg = 0;
            }
            break;
        default:
            MeasureStepFlg = 10;
            OSTimeDlyHMSM(0, 0, 0,50);
            break;
        }
    }
}


/*********************************************************************************************************
*                                            AppTask_HistoryMemoryRead()
*
* Description : 历史数据读取响应任务
*
* Argument(s) : p_arg       .
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************/

static  void  AppTask_HistoryMemoryRead (void *p_arg)
{
    int16 bufHold;
    CPU_INT08U   err = err;
    (void)p_arg;
    while (DEF_TRUE)
    {
        NewLcdVol(MenuId);
        //此处加上上位机上传flg判断 放这里不是太合适
        if(OnlineFlg == 1)
        {
            bufHold	= usRegHoldingBuf[1];
            if(bufHold)//小时数据上传
            {
                Wait_Flg ++;
                usRegHoldingBuf[0] = 0x55;	  //忙
                ReadHistory();
                usRegHoldingBuf[0] = 0;
                usRegHoldingBuf[1] = 0;
                CUpdateFlg = 0;
                WEBxintiao = 0;//检测到数据 当心跳包用
            }
            bufHold = usRegHoldingBuf[2];	//分钟数据
            if(bufHold)
            {
                Wait_Flg ++;
                usRegHoldingBuf[0] = 0x55;	  //忙
                ReadHistoryM();
                usRegHoldingBuf[0] = 0;
                usRegHoldingBuf[2] = 0;
                WEBxintiao = 0;//检测到数据 当心跳包用
            }
            bufHold = usRegHoldingBuf[3];	//系统日志数据	总体读
            if(bufHold)
            {
                Wait_Flg ++;
                usRegHoldingBuf[0] = 0x55;	  //忙
                ReadHistorySys();
                usRegHoldingBuf[0] = 0;
                usRegHoldingBuf[3] = 0;
                WEBxintiao = 0;//检测到数据 当心跳包用
            }
            bufHold = usRegHoldingBuf[4];	//历史数据
            if(bufHold)
            {
                Wait_Flg ++;
                usRegHoldingBuf[0] = 0x55;	  //忙
                ReadHistoryDat();
                usRegHoldingBuf[0] = 0;
                usRegHoldingBuf[4] = 0;
                WEBxintiao = 0;//检测到数据 当心跳包用
            }
            //系统日志分钟读
            bufHold = usRegHoldingBuf[5];	//系统日志数据	分钟读
            if(bufHold)
            {
                Wait_Flg ++;
                usRegHoldingBuf[0] = 0x55;	  //忙
                ReadHistorySysMin();
                usRegHoldingBuf[0] = 0;
                usRegHoldingBuf[5] = 0;
                SysrecordReadFlg=0;
                WEBxintiao = 0;//检测到数据 当心跳包用
            }
        }
        OSTimeDlyHMSM(0, 0, 0,50);
    }
}

/*
*********************************************************************************************************
*                                            App_TaskBattery()
*
* Description : 	 数据采集及温度控制
*
* Argument(s) : p_arg       .
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************/

static  void  AppTask_DataGather_PineTempCtrl (void *p_arg)
{
    CPU_INT08U   err,i;
    (void)p_arg;
    while (DEF_TRUE)
    {
        if(TimeCount1s)//rtc延时一秒 内部操作多余1秒了 有误。
        {
            TimeCount1s = 0;
            if(CountStartFlg == 1)
            {
                OSMutexPend(OSUart0UsedMutex,0,&err);
                CountRead();
                OSMutexPost(OSUart0UsedMutex);
            }
            i++;
            if(i>2)
            {
                i = 0;
                if((Wait_Flg/2)<0.5)//有数据在上传，暂停FPGA操作保证上传数据正确。
                {
                    NomalMeasure();
                }
                else
                {
                    Wait_Flg = Wait_Flg/2.0;
                }
            }
        }
        OSTimeDlyHMSM(0, 0, 0,50);
    }
}
/*********************************************************************************************************
*                                            AppTask_DTU()  //DTU任务
*
* Description : Monitor the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************/
static  void  AppTask_DTU (void *p_arg)
{
    CPU_INT08U   err = err;
    (void)p_arg;
    while (DEF_TRUE)
    {
        static uint16 datbuf[16];
        OSReadHoldReg(1, 0x9c46, 6,datbuf);		//40006
        TSP_GS_M 	= 	TSP_GS_k * (datbuf[0]<<8 | datbuf[1])/10.0;//40006
        PM25_GS_M 	= 	PM25_GS_k * (datbuf[2]<<8 | datbuf[3])/10.0;//40008
        PM10_GS_M 	= 	PM10_GS_k * (datbuf[4]<<8 | datbuf[5])/10.0;//40010
        TSP_GS_H  	+= 	TSP_GS_M;
        PM25_GS_H 	+= 	PM25_GS_M;
        PM10_GS_H 	+= 	PM10_GS_M;
        GS_Cnt	++;
        OSTimeDlyHMSM(0, 0, 1, 0);

        OSReadHoldReg(1, 0x9c5f, 6,datbuf);		//40031
        GS_Cycle 	= datbuf[0]	;//40031
        //PM25_GS_k	= datbuf[3]	;//40034
        //TSP_GS_k 	= datbuf[4] ;//40035
        //PM10_GS_k 	= datbuf[5] ;//40036
        OSTimeDlyHMSM(0, 0, 1, 0);

        //SendStates();
        OSTimeDlyHMSM(0, 0, 1, 0);

        //SendResult();
        OSTimeDlyHMSM(0, 0, 56, 0);
    }
}

/*********************************************************************************************************
*                                            AppTask_SHELL()  //SHELL任务
*
* Description : Monitor the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************/
static  void  AppTask_SHELL (void *p_arg)
{
    CPU_INT08U   err = err;
    static int i = 0;
    char ubuf[20];
    (void)p_arg;

    while (DEF_TRUE)
    {
        i++;
        if(i%2 == 0)
        {
            SET_LED3(0);
        }
        else
        {
            SET_LED3(1);
        }

        //Send_test();
       // getShishiLiuliang();
		

        /* 输入一个字符串 */
        //为调试通过
        //scanf("%s", ubuf);

        /* 显示输入的字符串 */
        //for(i=0;i<180;i++)
        //printf("%f\n", Cnt_Record[i].I);

        OSTimeDlyHMSM(0, 0, 0, 200);
    }
}
/*********************************************************************************************************
*                                            OSModbusServe()  //周立功Modbus主机服务任务
*
* Description : Monitor the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************/

extern void OSModbusServe	(void *pdata)
{
    (void)pdata;
    while (1)
    {
        IdleModbus();
    }
}
/*********************************************************************************************************
*                        AppTask_Refresh_Lcd_AnalogOutput()  //LCD首页界面刷新及模拟输出刷新
*
* Description : Monitor the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************/


static  void  AppTask_Refresh_Lcd_AnalogOutput(void 		*p_arg) /*LCD首页界面刷新及模拟输出刷新*/
{
    CPU_INT08U   err;
    (void)p_arg;
    while (1)
    {
        OSTimeDlyHMSM(0, 0, 1,0);
        if((Wait_Flg/2)<0.5)//有数据在上传，暂停FPGA操作保证上传数据正确。
        {
            OSMutexPend(OSLcdUsedMutex,1,&err);
            Menm0Updata();
            OSMutexPost(OSLcdUsedMutex);
//			outputstruct1.RH 			= 		HumiditySample;
//			outputstruct1.flow1 		= 		LiuLiang_GK;
//			outputstruct1.flow2 		= 		LiuLiang_BK;
//			outputstruct1.temperature1 	= 		WenDuT1;
//			outputstruct1.temperature2 	= 		WenDuT2;
//			outputstruct1.temperature3 	= 		WenDuT3;
//			OutputCONC(outputstruct1);
            OSTimeDlyHMSM(0, 0, 1,0);
//			DigitalOutput();
        }
    }
}

#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while(1);
}
#endif

/*
 * @}
 */
/*
*********************************************************************************************************
*********************************************************************************************************
*                                          uC/OS-II APP HOOKS
*********************************************************************************************************
*********************************************************************************************************
*/
/*
		以下 UCOSII 接口函数      均为空  没有用到
*/
#if (OS_APP_HOOKS_EN > 0)
/*
*********************************************************************************************************
*                                      TASK CREATION HOOK (APPLICATION)
*
* Description : This function is called when a task is created.
*
* Argument(s) : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void  App_TaskCreateHook (OS_TCB *ptcb)
{
#if ((APP_OS_PROBE_EN   == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN == DEF_ENABLED))
    OSProbe_TaskCreateHook(ptcb);
#endif
}

/*
*********************************************************************************************************
*                                    TASK DELETION HOOK (APPLICATION)
*
* Description : This function is called when a task is deleted.
*
* Argument(s) : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void  App_TaskDelHook (OS_TCB *ptcb)
{
    (void)ptcb;
}

/*
*********************************************************************************************************
*                                      IDLE TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskIdleHook(), which is called by the idle task.  This hook
*               has been added to allow you to do such things as STOP the CPU to conserve power.
*
* Argument(s) : none.
*
* Note(s)     : (1) Interrupts are enabled during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 251
void  App_TaskIdleHook (void)
{
}
#endif


/*
*********************************************************************************************************
*                                            TASK RETURN HOOK
*
* Description: This function is called if a task accidentally returns.  In other words, a task should
*              either be an infinite loop or delete itself when done.
*
* Arguments  : ptcb      is a pointer to the task control block of the task that is returning.
*
* Note(s)    : none
*********************************************************************************************************
*/
#if OS_VERSION >= 289
void  OSTaskReturnHook (OS_TCB  *ptcb)
{
    (void)ptcb;
}
#endif
/*
*********************************************************************************************************
*                                        STATISTIC TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskStatHook(), which is called every second by uC/OS-II's
*               statistics task.  This allows your application to add functionality to the statistics task.
*
* Argument(s) : none.
*********************************************************************************************************
*/

void  App_TaskStatHook (void)
{
}

/*
*********************************************************************************************************
*                                        TASK SWITCH HOOK (APPLICATION)
*
* Description : This function is called when a task switch is performed.  This allows you to perform other
*               operations during a context switch.
*
* Argument(s) : none.
*
* Note(s)     : (1) Interrupts are disabled during this call.
*
*               (2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                   will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                  task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/

#if OS_TASK_SW_HOOK_EN > 0
void  App_TaskSwHook (void)
{
#if ((APP_OS_PROBE_EN   == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN == DEF_ENABLED))
    OSProbe_TaskSwHook();
#endif
}
#endif

/*
*********************************************************************************************************
*                                     OS_TCBInit() HOOK (APPLICATION)
*
* Description : This function is called by OSTCBInitHook(), which is called by OS_TCBInit() after setting
*               up most of the TCB.
*
* Argument(s) : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)     : (1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 204
void  App_TCBInitHook (OS_TCB *ptcb)
{
    (void)ptcb;
}
#endif

/*
*********************************************************************************************************
*                                        TICK HOOK (APPLICATION)
*
* Description : This function is called every tick.
*
* Argument(s) : none.
*
* Note(s)     : (1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_TIME_TICK_HOOK_EN > 0
void  App_TimeTickHook (void)
{
#if ((APP_OS_PROBE_EN   == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN == DEF_ENABLED))
    OSProbe_TickHook();
#endif
}
#endif
#endif
