#include"Measure.h"
volatile int32 CaliFlg;
float CalParam_I1[3],CalParam_I2[3];  //校准光强参数
float CalParam_U[3];                  //校准值
float CalParam_avr_U;                  //校准品均值值
float CalParam_max_U;                  //校准最大值值值
float CalParam_min_U;                  //校准最小值值值
float CalParam_AARD;                   //相对误差
float CheParam_I1[10],CheParam_I2[10];  //标样检查参数
float CheParam_U[10];                   //标样检查值
float CheParam_avr_U;                   //标样检查平均值
float CheParam_AARD;                   //相对误差

#define AREA 6.8358
DAStruct DAStructTable[NUN_DA]=
{
    {
        0,
        2.5/1.002,
    },
    {
        1,
        2.5/1.002,
    },
    {
        2,
        1.22/1.002,
    },
    {
        3,
        2/1.002,
    },

};

ADStruct ADStructTable[NUN_AD]=
{   //通道 ，放大倍数。（采集回来的值要乘以这个放大倍数）
    {
        0,
        1*1.002,
    },
    {
        1,
        2,
    },
    {
        2,
        2,
    },
    {
        3,
        3.35,//4.92,yuan3.43
    },
    {
        4,
        2,
    },
    {
        5,
        2,
    },
    {
        6,
        1*1.002,
    },
    {
        7,
        1,
    },
    {
        8,
        1,
    },
    {
        9,
        1,
    },
    {
        10,
        1,
    },
    {
        11,
        1*1.002,
    },
    {
        12,
        1,
    },
    {
        13,
        1,
    },
    {
        14,
        1,
    },
    {
        15,
        1,
    },


};

static SensorStruct SensorStructTable[NUN_SENSOR]=
{
    {   //昆仑海岸JWSK-6户外温湿度传感器
        CGQ_WENDU1_KL,      //采样口温度传感器4~20ma对应0.8v~4v对应-40℃~120V
        {50,-80},
        1.0,
        0.0,
    },

    {   //星仪CWS18-06户外温湿度传感器
        CGQ_WENDU1_XY,    //采样口温度传感器4~20ma对应0.8v~4v对应-20℃~60V
        //{31.25,-65},//0.8v~4v对应-40℃~60V
        {25,-40},
        1.0,
        0.0,
    },

    {   //昆仑海岸JWSK-6户外温湿度传感器
        CGQ_CYK_SHIDU_KL,//采样口湿度传感器4~20ma对应0.8v~4v对应0%~100%
        {31.25,-25},
        1,
        0.0,
    },
    {   //星仪CWS18户外温湿度传感器
        CGQ_CYK_SHIDU_XY, //采样口湿度传感器4~20ma对应0.8v~4v对应0%~100%
        {31.25,-25},
        1,
        0.0,
    },

    {   //T2
        CGQ_WENDU2,      //加热管温度传感器pt100，1V到5V 对应 -50℃到100℃
        {37.5,-87.5},
        1.0,
        0.0,
    },
    {   //T3
        CGQ_WENDU3,     //测试舱温度传感器pt100，1V到5V 对应 -50℃到100℃
        {37.5,-87.5},
        1.0,
        0.0,
    },
    {   //RH2
        CGQ_SHIDU,      //测试仓湿度传感器HM1500 1V~4V 对应 0%~100% 采用数据手册公式
        {39.1,-42.5},
        1,
        0.0,
    },
    {
        CGQ_LIULIANG,
        {4.0,0},
        1,
        0.0,
    },
    {
        CGQ_LIULIANG_HONEYWELL,
        {5.0,-5},
        1,
        0.0,
    },
//	{
//		CGQ_QIYA,
//		{22.957,10.56},//手册上有问题，公式推出的是-10.56 ，带入值计算的是10.56。 然后电源也是4.84.
//		1,
//		0.0,
//	},
//
    {
        CGQ_QIYA,
        {22.217,10.56},//写死了计算公式，而不是根据vss进行调整
        1,
        0.0,
    },

    {   //T4
        CGQ_TCQ_WENDU,    //探测器传感器pt100，1V到5V 对应 -50℃到100℃
        {37.5,-87.5},
        1,
        0.0,
    }
};



float FindOriginalDat(uint8 SensorName)
{
    int i;
    for(i = 0; i<NUN_SENSOR; i++)
    {
        if(SensorStructTable[i].SensorName == SensorName )
        {
            break;
        }
    }
    return SensorStructTable[i].OriginalDat;
}

void SetCaliDat(uint8 SensorName,float CaliDat)
{
    int i;
    for(i = 0; i<NUN_SENSOR; i++)
    {
        if(SensorStructTable[i].SensorName == SensorName )
        {
            break;
        }
    }
    SensorStructTable[i].CaliDat = 	CaliDat;
}

float LoadK(uint8 SensorName,float k)
{
    int i=0;
    if((SensorName>NUN_SENSOR))
    {
        return -1;
    }
    for(i=0; i<NUN_SENSOR; i++)
    {
        if(SensorName == SensorStructTable[i].SensorName)
        {
            SensorStructTable[i].Modulus[0] = k;
            return 0;
        }
    }
    return -1;
}

float GetADValue(uint8 ADChannel)   //将各通道采样值乘以响应放大倍数
{
    int i;//ADRangesel=2;
    float dat;
    if((ADChannel>NUN_AD))
    {
        return -1;
    }

    for(i=0; i<NUN_AD; i++)
    {
        if(ADChannel == ADStructTable[i].Channel)
        {
            dat = addat[ADChannel]*ADStructTable[i].Gain;
            return dat;
        }
    }
    return -1;
}


float GetMeasureValue(uint8 SensorName,uint8 ADChannel)    //将采样电压值转换为实际传感器采样参数值
{
    int i;//ADRangesel=2;
    float dat;
    if((SensorName>NUN_SENSOR)||(ADChannel>NUN_AD))
    {
        return -1;
    }

    for(i=0; i<NUN_AD; i++) //将各通道采样值乘以响应放大倍数
    {
        if(ADChannel == ADStructTable[i].Channel)
        {
            dat = addat[ADChannel]*ADStructTable[i].Gain;//求电压值
            break;
        }
    }

    for(i=0; i<NUN_SENSOR; i++)
    {
        if(SensorName == SensorStructTable[i].SensorName)
        {
            SensorStructTable[i].OriginalDat = (dat*SensorStructTable[i].Modulus[0])+(SensorStructTable[i].Modulus[1]);
            if(JIAOZHUN_ON)
            {
                dat = SensorStructTable[i].OriginalDat* SensorStructTable[i].CaliDat;
            }
            else
            {
                dat =  SensorStructTable[i].OriginalDat	;
            }
            break;
        }
    }
    return dat;
}


//标片校准
void Calibration(void)
{
    uint32 slen;
    uint8 err;
    uint8 i;
    float ParOldU;
    area[0] = 0;
    area[1] = 25;
    area[2] = 239;
    area[3] = 159;


    //放射源就位
    OSMutexPend(OSUart0UsedMutex,0,&err);
    TTLOutput (TTL_2,1,TTL_BRAY_Ctrl1);	 //。
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl2);	 //。
    OSMutexPost(OSUart0UsedMutex);

    OSTimeDlyHMSM(0, 0, C14MotMoveTime,0);	//运行时间

    OSMutexPend(OSUart0UsedMutex,0,&err);
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl1);	 //。
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl2);	 //。
    OSMutexPost(OSUart0UsedMutex);

    switch (CaliFlg)
    {
        //正在松开压纸电机
    case 0:
        CleanVideo(area);

        DisCList16(80,30,"第一次标定");

        slen = StringLen(NextPrompt[24+NePrOffset]); //
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[24+NePrOffset]);   //松开电机提示
        LcdUpdata(DisBuf);
        songkaiMoto();
        CleanVideo(area);
        //请取下滤纸 插入标片1
        DisCList16(80,30,"第一次标定");
        slen = StringLen(NextPrompt[25+NePrOffset]);
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[25+NePrOffset]);   //取下滤纸提示
        prompt(0);	// 提示 确定
        LcdUpdata(DisBuf);
        CaliFlg = 1;

        break;
    case 1:
        //提示测量200s
        CleanVideo(area);
        DisCList16(80,30,"第一次标定");

        slen = StringLen(NextPrompt[30+NePrOffset]);   //   正在压紧电机请	稍后
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[30+NePrOffset]);
        LcdUpdata(DisBuf);
        yajingMoto();

        CleanVideo(area);
        DisCList16(80,30,"第一次标定");

        slen = StringLen(NextPrompt[27+NePrOffset]); //   测量200s	稍后
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[27+NePrOffset]);
        LcdUpdata(DisBuf);
        OSMutexPend(OSUart0UsedMutex,0,&err);
        CountCtrl(COUNT_START);	//启动计数
        OSMutexPost(OSUart0UsedMutex);
        TimeKeyCount = JIAOZHUN_BIAOPIAN_TEST_TIME*50;
        CountStartFlg =1;
        while(TimeKeyCount)
        {
            CleanVideo(area);
            DisCList16(80,30,"第一次标定");
            DisDat.length = 5;
            DisDat.decimal = 0;
            DisDat.DisSign = 0;
            DisDat.font = 2;
            DisValue2(CountNow,96,96, &DisDat ,"");
            DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[27+NePrOffset]);
            DisValue2((int)(TimeKeyCount/50),16*2+(DIS_AREA_X-slen*8)/2,70, &DisDat ,"");

            LcdUpdata(DisBuf);
            OSTimeDly(100);//0.5秒;
        }
        CleanVideo(area);
        OSMutexPend(OSUart0UsedMutex,0,&err);
        CountRead();
        OSMutexPost(OSUart0UsedMutex);
        CountStartFlg = 0;
        //计算平均值
        EeorMear = (CountTotal<50)?(EeorMear|0x01):(EeorMear&0xfe);		   //探测器错误

        CalParam_I1[0] =  CountTotal / CountTimes;
//			OSMutexPend(OSUart0UsedMutex,0,&err);
//			CountCtrl(COUNT_END);
//			OSMutexPost(OSUart0UsedMutex);
        //正在松开压纸电机
        CleanVideo(area);
        DisCList16(80,30,"第一次标定");
        slen = StringLen(NextPrompt[24+NePrOffset]); //
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[24+NePrOffset]);
        LcdUpdata(DisBuf);
        songkaiMoto();
        CleanVideo(area);
        DisCList16(80,30,"第一次标定");
        slen = StringLen(NextPrompt[26+NePrOffset]); //  放入标2
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[26+NePrOffset]);
        prompt(2);	// 提示 确定
        LcdUpdata(DisBuf);
        CaliFlg = 2;
        break;


    case 2:
        //
        CleanVideo(area);
        DisCList16(80,30,"第一次标定");
        slen = StringLen(NextPrompt[30+NePrOffset]); //   正在压紧电机请	稍后
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[30+NePrOffset]);
        LcdUpdata(DisBuf);
        yajingMoto();
        CleanVideo(area);
        DisCList16(80,30,"第一次标定");
        slen = StringLen(NextPrompt[27+NePrOffset]); //   测量200s
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[27+NePrOffset]);
        LcdUpdata(DisBuf);
        OSMutexPend(OSUart0UsedMutex,0,&err);
        CountCtrl(COUNT_START);	//启动计数
        OSMutexPost(OSUart0UsedMutex);
        TimeKeyCount = JIAOZHUN_BIAOPIAN_TEST_TIME*50;//20ms一个 5s
        CountStartFlg =1;

        while(TimeKeyCount)
        {
            CleanVideo(area);
            DisCList16(80,30,"第一次标定");
            DisDat.length = 5;
            DisDat.decimal = 0;
            DisDat.DisSign = 0;
            DisDat.font = 2;
            DisValue2(CountNow,96,96, &DisDat ,"");
            DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[27+NePrOffset]);
            DisValue2((int)(TimeKeyCount/50),16*2+(DIS_AREA_X-slen*8)/2,70, &DisDat ,"");

            LcdUpdata(DisBuf);
            OSTimeDly(100);//0.5秒;
        }
        CleanVideo(area);
        OSMutexPend(OSUart0UsedMutex,0,&err);
        CountRead();
        OSMutexPost(OSUart0UsedMutex);
        CountStartFlg = 0;
        //计算平均值
        EeorMear = (CountTotal<50)?(EeorMear|0x01):(EeorMear&0xfe);		   //探测器错误

        CalParam_I2[0] =  CountTotal / CountTimes;
//			OSMutexPend(OSUart0UsedMutex,0,&err);
//			CountCtrl(COUNT_END);
//			OSMutexPost(OSUart0UsedMutex);
        CalParam_U[0] = 1000.0*log(CalParam_I1[0]/CalParam_I2[0])*AREA/(BiaoPian*10);//cm2/mg
        DisCList16(80,30,"第一次标定");
        slen = StringLen("第一次标定完成,请按确定键"); //
        DisCList16((DIS_AREA_X-slen*8)/2,80,"第一次标定完成,请按确定键");

        WriteCode1212(43,60,"I1:       I2:       u1: ");
        DisValue(CalParam_I1[0],61,60,1,5,0,1);
        DisValue(CalParam_I2[0],121,60,1,5,0,1);
        DisValue(CalParam_U[0],181,60,1,4,3,1);

        prompt(0);	// 提示 确定

        CaliFlg = 3;

        break;
    case 3:    //第二次标定

        //更新 存值，显示
// 			WriteFp32(JIAOZHUN_BIAOPIAN,Parg_u);
// 			WriteCaliParTime(JIAOZHUN_BIAOPIAN_TIME,&TimeDate);
        CleanVideo(area);

        DisCList16(80,30,"第二次标定");

        slen = StringLen(NextPrompt[24+NePrOffset]); //
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[24+NePrOffset]);   //松开电机提示
        LcdUpdata(DisBuf);
        songkaiMoto();
        CleanVideo(area);
        //请取下滤纸 插入标片1
        DisCList16(80,30,"第二次标定");
        slen = StringLen("请取下标片,并按确定键");
        DisCList16((DIS_AREA_X-slen*8)/2,70,"请取下标片,并按确定键");   //取下滤纸提示
        prompt(0);	// 提示 确定
        LcdUpdata(DisBuf);
        CaliFlg = 4;
        break;
    case 4:
        //提示测量200s
        CleanVideo(area);
        DisCList16(80,30,"第二次标定");

        slen = StringLen(NextPrompt[30+NePrOffset]);   //   正在压紧电机请	稍后
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[30+NePrOffset]);
        LcdUpdata(DisBuf);
        yajingMoto();

        CleanVideo(area);
        DisCList16(80,30,"第二次标定");

        slen = StringLen(NextPrompt[27+NePrOffset]); //   测量200s	稍后
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[27+NePrOffset]);
        LcdUpdata(DisBuf);
        OSMutexPend(OSUart0UsedMutex,0,&err);
        CountCtrl(COUNT_START);	//启动计数
        OSMutexPost(OSUart0UsedMutex);
        TimeKeyCount = JIAOZHUN_BIAOPIAN_TEST_TIME*50;
        CountStartFlg =1;
        while(TimeKeyCount)
        {
            CleanVideo(area);
            DisCList16(80,30,"第二次标定");
            DisDat.length = 5;
            DisDat.decimal = 0;
            DisDat.DisSign = 0;
            DisDat.font = 2;
            DisValue2(CountNow,96,96, &DisDat ,"");
            DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[27+NePrOffset]);
            DisValue2((int)(TimeKeyCount/50),16*2+(DIS_AREA_X-slen*8)/2,70, &DisDat ,"");

            LcdUpdata(DisBuf);
            OSTimeDly(100);//0.5秒;
        }
        CleanVideo(area);
        OSMutexPend(OSUart0UsedMutex,0,&err);
        CountRead();
        OSMutexPost(OSUart0UsedMutex);
        CountStartFlg = 0;
        //计算平均值
        EeorMear = (CountTotal<50)?(EeorMear|0x01):(EeorMear&0xfe);		   //探测器错误

        CalParam_I1[1] =  CountTotal / CountTimes;
//			OSMutexPend(OSUart0UsedMutex,0,&err);
//			CountCtrl(COUNT_END);
//			OSMutexPost(OSUart0UsedMutex);
        //正在松开压纸电机
        CleanVideo(area);
        DisCList16(80,30,"第二次标定");
        slen = StringLen(NextPrompt[24+NePrOffset]); //
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[24+NePrOffset]);
        LcdUpdata(DisBuf);
        songkaiMoto();
        CleanVideo(area);
        DisCList16(80,30,"第二次标定");
        slen = StringLen(NextPrompt[26+NePrOffset]); //  放入标2
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[26+NePrOffset]);
        prompt(2);	// 提示 确定
        LcdUpdata(DisBuf);
        CaliFlg = 5;
        break;
    case 5:
        CleanVideo(area);
        DisCList16(80,30,"第二次标定");
        slen = StringLen(NextPrompt[30+NePrOffset]); //   正在压紧电机请	稍后
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[30+NePrOffset]);
        LcdUpdata(DisBuf);
        yajingMoto();
        CleanVideo(area);
        DisCList16(80,30,"第二次标定");
        slen = StringLen(NextPrompt[27+NePrOffset]); //   测量200s
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[27+NePrOffset]);
        LcdUpdata(DisBuf);
        OSMutexPend(OSUart0UsedMutex,0,&err);
        CountCtrl(COUNT_START);	//启动计数
        OSMutexPost(OSUart0UsedMutex);
        TimeKeyCount = JIAOZHUN_BIAOPIAN_TEST_TIME*50;//20ms一个 5s
        CountStartFlg =1;

        while(TimeKeyCount)
        {
            CleanVideo(area);
            DisCList16(80,30,"第二次标定");
            DisDat.length = 5;
            DisDat.decimal = 0;
            DisDat.DisSign = 0;
            DisDat.font = 2;
            DisValue2(CountNow,96,96, &DisDat ,"");
            DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[27+NePrOffset]);
            DisValue2((int)(TimeKeyCount/50),16*2+(DIS_AREA_X-slen*8)/2,70, &DisDat ,"");

            LcdUpdata(DisBuf);
            OSTimeDly(100);//0.5秒;
        }
        CleanVideo(area);
        OSMutexPend(OSUart0UsedMutex,0,&err);
        CountRead();
        OSMutexPost(OSUart0UsedMutex);
        CountStartFlg = 0;
        //计算平均值
        EeorMear = (CountTotal<50)?(EeorMear|0x01):(EeorMear&0xfe);		   //探测器错误

        CalParam_I2[1] =  CountTotal / CountTimes;
//			OSMutexPend(OSUart0UsedMutex,0,&err);
//			CountCtrl(COUNT_END);
//			OSMutexPost(OSUart0UsedMutex);
        CalParam_U[1] = 1000.0*log(CalParam_I1[1]/CalParam_I2[1])*AREA/(BiaoPian*10);//cm2/mg

        DisCList16(80,30,"第二次标定");
        slen = StringLen("第二次标定完成,请按确定键"); //
        DisCList16((DIS_AREA_X-slen*8)/2,80,"第二次标定完成,请按确定键");

        WriteCode1212(43,60,"I1:       I2:       u2: ");
        DisValue(CalParam_I1[1],61,60,1,5,0,1);
        DisValue(CalParam_I2[1],121,60,1,5,0,1);
        DisValue(CalParam_U[1],181,60,1,4,3,1);

        prompt(0);	// 提示 确定

        CaliFlg = 6;
        break;
    case 6:    //第三次标定

        //更新 存值，显示
// 			WriteFp32(JIAOZHUN_BIAOPIAN,Parg_u);
// 			WriteCaliParTime(JIAOZHUN_BIAOPIAN_TIME,&TimeDate);
        CleanVideo(area);

        DisCList16(80,30,"第三次标定");

        slen = StringLen(NextPrompt[24+NePrOffset]); //
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[24+NePrOffset]);   //松开电机提示
        LcdUpdata(DisBuf);
        songkaiMoto();
        CleanVideo(area);
        //请取下滤纸 插入标片1
        DisCList16(80,30,"第三次标定");
        slen = StringLen("请取下标片,并按确定键");
        DisCList16((DIS_AREA_X-slen*8)/2,70,"请取下标片,并按确定键");   //取下滤纸提示
        prompt(0);	// 提示 确定
        LcdUpdata(DisBuf);
        CaliFlg = 7;
        break;
    case 7:
        //提示测量200s
        CleanVideo(area);
        DisCList16(80,30,"第三次标定");

        slen = StringLen(NextPrompt[30+NePrOffset]);   //   正在压紧电机请	稍后
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[30+NePrOffset]);
        LcdUpdata(DisBuf);
        yajingMoto();

        CleanVideo(area);
        DisCList16(80,30,"第三次标定");

        slen = StringLen(NextPrompt[27+NePrOffset]); //   测量200s	稍后
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[27+NePrOffset]);
        LcdUpdata(DisBuf);
        OSMutexPend(OSUart0UsedMutex,0,&err);
        CountCtrl(COUNT_START);	//启动计数
        OSMutexPost(OSUart0UsedMutex);

        TimeKeyCount = JIAOZHUN_BIAOPIAN_TEST_TIME*50;

        CountStartFlg =1;
        while(TimeKeyCount)
        {
            CleanVideo(area);
            DisCList16(80,30,"第三次标定");
            DisDat.length = 5;
            DisDat.decimal = 0;
            DisDat.DisSign = 0;
            DisDat.font = 2;
            DisValue2(CountNow,96,96, &DisDat ,"");
            DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[27+NePrOffset]);
            DisValue2((int)(TimeKeyCount/50),16*2+(DIS_AREA_X-slen*8)/2,70, &DisDat ,"");

            LcdUpdata(DisBuf);
            OSTimeDly(100);//0.5秒;
        }
        CleanVideo(area);
        OSMutexPend(OSUart0UsedMutex,0,&err);
        CountRead();
        OSMutexPost(OSUart0UsedMutex);
        CountStartFlg = 0;
        //计算平均值
        EeorMear = (CountTotal<50)?(EeorMear|0x01):(EeorMear&0xfe);		   //探测器错误

        CalParam_I1[2] =  CountTotal / CountTimes;
//			OSMutexPend(OSUart0UsedMutex,0,&err);
//			CountCtrl(COUNT_END);
//			OSMutexPost(OSUart0UsedMutex);
        //正在松开压纸电机
        CleanVideo(area);
        DisCList16(80,30,"第三次标定");
        slen = StringLen(NextPrompt[24+NePrOffset]); //
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[24+NePrOffset]);
        LcdUpdata(DisBuf);
        songkaiMoto();
        CleanVideo(area);
        DisCList16(80,30,"第三次标定");
        slen = StringLen(NextPrompt[26+NePrOffset]); //  放入标2
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[26+NePrOffset]);
        prompt(2);	// 提示 确定
        LcdUpdata(DisBuf);
        CaliFlg = 8;
        break;
    case 8:
        CleanVideo(area);
        DisCList16(80,30,"第三次标定");
        slen = StringLen(NextPrompt[30+NePrOffset]); //   正在压紧电机请	稍后
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[30+NePrOffset]);
        LcdUpdata(DisBuf);
        yajingMoto();
        CleanVideo(area);
        DisCList16(80,30,"第三次标定");
        slen = StringLen(NextPrompt[27+NePrOffset]); //   测量200s
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[27+NePrOffset]);
        LcdUpdata(DisBuf);

        OSMutexPend(OSUart0UsedMutex,0,&err);
        CountCtrl(COUNT_START);	//启动计数
        OSMutexPost(OSUart0UsedMutex);
        TimeKeyCount = JIAOZHUN_BIAOPIAN_TEST_TIME*50;//20ms一个 5s

        CountStartFlg =1;

        while(TimeKeyCount)
        {
            CleanVideo(area);
            DisCList16(80,30,"第三次标定");
            DisDat.length = 5;
            DisDat.decimal = 0;
            DisDat.DisSign = 0;
            DisDat.font = 2;
            DisValue2(CountNow,96,96, &DisDat ,"");
            DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[27+NePrOffset]);
            DisValue2((int)(TimeKeyCount/50),16*2+(DIS_AREA_X-slen*8)/2,70, &DisDat ,"");

            LcdUpdata(DisBuf);
            OSTimeDly(100);//0.5秒;
        }

        CleanVideo(area);

        OSMutexPend(OSUart0UsedMutex,0,&err);
        CountRead();
        OSMutexPost(OSUart0UsedMutex);
        CountStartFlg = 0;
        //计算平均值
        EeorMear = (CountTotal<50)?(EeorMear|0x01):(EeorMear&0xfe);		   //探测器错误

        CalParam_I2[2] =  CountTotal / CountTimes;
//			OSMutexPend(OSUart0UsedMutex,0,&err);
//			CountCtrl(COUNT_END);
//			OSMutexPost(OSUart0UsedMutex);
        CalParam_U[2] = 1000.0*log(CalParam_I1[2]/CalParam_I2[2])*AREA/(BiaoPian*10);//cm2/mg

        ParOldU = 	ReadFp32(JIAOZHUN_BIAOPIAN);

        CalParam_avr_U = (CalParam_U[0]+CalParam_U[1]+CalParam_U[2])/3;          //三次品均值

        CalParam_max_U = CalParam_U[0];
        CalParam_min_U = CalParam_U[0];

        for (i=0; i<2; i++)
        {

            if(CalParam_max_U < CalParam_U[i+1])
            {
                CalParam_max_U = CalParam_U[i+1];
            }

            if(CalParam_min_U > CalParam_U[i+1])
            {
                CalParam_min_U = CalParam_U[i+1];
            }
        }

        CalParam_AARD =100*(CalParam_max_U - CalParam_min_U )/CalParam_avr_U;       //相对偏差百分比


        WriteCode1212(10,22,"第一次测试:I1:      I2:      u1: ");
        WriteCode1212(10,42,"第二次测试:I1:      I2:      u2: ");
        WriteCode1212(10,62,"第三次测试:I1:      I2:      u2: ");
        WriteCode1212(10,82,"u平均值:          u相对误差:     %");
        WriteCode1212(10,102,"历史校准记录:     ");

        slen = StringLen("是否更新u值 并保存?");                  //
        DisCList16((DIS_AREA_X-slen*8)/2,122,"是否更新u值?");
        //第一次测试参数显示
        DisValue(CalParam_I1[0],92,22,1,5,0,1);
        DisValue(CalParam_I2[0],146,22,1,5,0,1);
        DisValue(CalParam_U[0],200,22,1,4,3,1);
        //第二次测试参数显示
        DisValue(CalParam_I1[1],92,42,1,5,0,1);
        DisValue(CalParam_I2[1],146,42,1,5,0,1);
        DisValue(CalParam_U[1],200,42,1,4,3,1);
        //第三次测试参数显示
        DisValue(CalParam_I1[2],92,62,1,5,0,1);
        DisValue(CalParam_I2[2],146,62,1,5,0,1);
        DisValue(CalParam_U[2],200,62,1,4,3,1);
        //u品均值显示
        DisValue(CalParam_avr_U,53,82,1,4,3,1);
        //相对误差
        DisValue(CalParam_AARD,180,82,1,3,1,1);
        //历史校准记录
        DisValue(ParOldU,60,102,1,4,3,1);

        prompt(0);	// 提示 确定
        LcdUpdata(DisBuf);
        //存入EEPROM
        //显示

        CaliFlg = 9;
        break;
    case 9:

        //更新 存值，显示
        Parg_u = CalParam_avr_U;
        WriteFp32(JIAOZHUN_BIAOPIAN,Parg_u);
        WriteCaliParTime(JIAOZHUN_BIAOPIAN_TIME,&TimeDate);
        CaliFlg = 10;
        break;
    default:
        break;
    }

    //放射源复位
    OSMutexPend(OSUart0UsedMutex,0,&err);
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl1);	 //
    TTLOutput (TTL_2,1,TTL_BRAY_Ctrl2);	 //
    OSMutexPost(OSUart0UsedMutex);

    OSTimeDlyHMSM(0, 0, C14MotMoveTime,0);	//运行时间

    OSMutexPend(OSUart0UsedMutex,0,&err);
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl1);	 //
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl2);	 //
    OSMutexPost(OSUart0UsedMutex);
    // 请输入标片对应  值。  确定返回  重新测量。 确定后将自动计算校准参数并保存。
}
//校准膜重现性测试
void 	Calibration2(void)
{
    uint32 slen;
    uint8 err;
    uint8 i;
    uint8 j;
    float temp;
    float ParOldU;

    //标片校准测试变量TestTimes   by yxq
    //  CheckCount
    //放射源就位
    OSMutexPend(OSUart0UsedMutex,0,&err);
    TTLOutput (TTL_2,1,TTL_BRAY_Ctrl1);	 //。
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl2);	 //。
    OSMutexPost(OSUart0UsedMutex);

    OSTimeDlyHMSM(0, 0, C14MotMoveTime,0);	//运行时间

    OSMutexPend(OSUart0UsedMutex,0,&err);
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl1);	 //。
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl2);	 //。
    OSMutexPost(OSUart0UsedMutex);

    switch (CaliFlg)
    {
        //正在松开压纸电机 ，
    case 0:
        area[0] = 0;
        area[1] = 25;
        area[2] = 239;
        area[3] = 159;
        CleanVideo(area);      //清除显示区域
        //第一行显示检测序号提示
        DisCList16(80,25,"第  次测试");

        DisDat.length = 1;
        DisDat.decimal = 0;
        DisDat.DisSign = 0;
        DisDat.font = 2;
        DisValue2((CheckCount+1),96,25, &DisDat ,"");

        area[0] = 0;      //
        area[1] = 45;
        area[2] = 239;
        area[3] = 159;


        slen = StringLen(NextPrompt[24+NePrOffset]); //
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[24+NePrOffset]);  //显示松开走纸电机提示
        LcdUpdata(DisBuf);
        songkaiMoto();       //执行松开走纸电机动作
        CleanVideo(area);
        //请取下滤纸 插入标片1
        slen = StringLen(NextPrompt[25+NePrOffset]);
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[25+NePrOffset]);  //显示去下滤纸提示
        prompt(0);	// 提示 确定
        LcdUpdata(DisBuf);
        CaliFlg = 1;
        break;
    case 1:
        //提示测量200s
        CleanVideo(area);

        slen = StringLen(NextPrompt[30+NePrOffset]); //   正在压紧电机请	稍后
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[30+NePrOffset]);
        LcdUpdata(DisBuf);
        yajingMoto();           //压紧电机

        CleanVideo(area);
        slen = StringLen(NextPrompt[27+NePrOffset]); //   测量200s	稍后
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[27+NePrOffset]);  //倒计时数秒
        LcdUpdata(DisBuf);
        OSMutexPend(OSUart0UsedMutex,0,&err);
        CountCtrl(COUNT_START);	             //发送启动计数
        OSMutexPost(OSUart0UsedMutex);
        TimeKeyCount = JIAOZHUN_BIAOPIAN_TEST_TIME*50;
        CountStartFlg =1;
        while(TimeKeyCount)
        {
            CleanVideo(area);
            DisDat.length = 5;
            DisDat.decimal = 0;
            DisDat.DisSign = 0;
            DisDat.font = 2;
            DisValue2(CountNow,96,96, &DisDat ,"");
            DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[27+NePrOffset]);
            DisValue2((int)(TimeKeyCount/50),16*2+(DIS_AREA_X-slen*8)/2,70, &DisDat ,"");

            LcdUpdata(DisBuf);
            OSTimeDly(100);//0.5秒;
        }
        CleanVideo(area);
        OSMutexPend(OSUart0UsedMutex,0,&err);
        CountRead();        //读取计数值
        OSMutexPost(OSUart0UsedMutex);
        CountStartFlg = 0;
        //计算平均值
        EeorMear = (CountTotal<50)?(EeorMear|0x01):(EeorMear&0xfe);		   //探测器错误

        CheParam_I1[CheckCount] =  CountTotal / CountTimes;           //空白计数率
//			OSMutexPend(OSUart0UsedMutex,0,&err);
//			CountCtrl(COUNT_END);
//			OSMutexPost(OSUart0UsedMutex);
        //正在松开压纸电机
        CleanVideo(area);
        slen = StringLen(NextPrompt[24+NePrOffset]); //
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[24+NePrOffset]);  //松开电机提示
        LcdUpdata(DisBuf);
        songkaiMoto();
        CleanVideo(area);
        slen = StringLen(NextPrompt[26+NePrOffset]); //  放入标2
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[26+NePrOffset]);
        prompt(2);	// 提示 确定
        LcdUpdata(DisBuf);
        CaliFlg = 2;
        break;


    case 2:
        //
        CleanVideo(area);

        slen = StringLen(NextPrompt[30+NePrOffset]); //   正在压紧电机请	稍后
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[30+NePrOffset]);
        LcdUpdata(DisBuf);
        yajingMoto();
        CleanVideo(area);
        slen = StringLen(NextPrompt[27+NePrOffset]); //   测量200s
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[27+NePrOffset]);
        LcdUpdata(DisBuf);
        OSMutexPend(OSUart0UsedMutex,0,&err);
        CountCtrl(COUNT_START);	//启动计数
        OSMutexPost(OSUart0UsedMutex);
        TimeKeyCount = JIAOZHUN_BIAOPIAN_TEST_TIME*50;//20ms一个 5s
        CountStartFlg =1;

        while(TimeKeyCount)
        {
            CleanVideo(area);
            DisDat.length = 5;
            DisDat.decimal = 0;
            DisDat.DisSign = 0;
            DisDat.font = 2;
            DisValue2(CountNow,96,96, &DisDat ,"");
            DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[27+NePrOffset]);
            DisValue2((int)(TimeKeyCount/50),16*2+(DIS_AREA_X-slen*8)/2,70, &DisDat ,"");

            LcdUpdata(DisBuf);
            OSTimeDly(100);//0.5秒;
        }
        CleanVideo(area);
        OSMutexPend(OSUart0UsedMutex,0,&err);
        CountRead();
        OSMutexPost(OSUart0UsedMutex);
        CountStartFlg = 0;
        //计算平均值
        EeorMear = (CountTotal<50)?(EeorMear|0x01):(EeorMear&0xfe);		   //探测器错误

        CheParam_I2[CheckCount] =  CountTotal / CountTimes;        //标片计数率
//			OSMutexPend(OSUart0UsedMutex,0,&err);
//			CountCtrl(COUNT_END);
//			OSMutexPost(OSUart0UsedMutex);
// 			Parg_u = 1000.0*log(CalParam_I1/CalParam_I2)*AREA/(BiaoPian*10);//cm2/mg
        CheParam_U[CheckCount] = 100.0*log(CheParam_I1[CheckCount]/CheParam_I2[CheckCount])*AREA/Parg_u;//cm2/mg

        DisCList16(30,60,"本次测量:I1:     I2:");
        DisCList16(30,80,"          U:");

        prompt(0);	// 提示 确定
        DisValue(CheParam_I1[CheckCount],126,60,1,5,0,1);
        DisValue(CheParam_I2[CheckCount],190,60,1,5,0,1);
        DisValue(CheParam_U[CheckCount],126,80,1,4,3,1);
        LcdUpdata(DisBuf);
        //存入EEPROM
        //显示

//			CaliFlg = 3;
        CheckCount++;

        if(CheckCount < TestTimes )
        {
            CaliFlg = 0;
        }
        else
        {
            CaliFlg = 3;    //进入显示流程
        }

        break;

    case 3:       //显示所有测试结果,并计算算相对标准偏差
        //计算品均值
        temp = 0;
        for (i=0; i<TestTimes; i++)
        {
            temp += CheParam_U[i];
        }
        CheParam_avr_U = temp / TestTimes ;
        //计算相对标准偏差
        if(TestTimes ==1)
        {
            CheParam_AARD = 0.0 ;
        }
        else
        {
            temp = 0;
            for (i=0; i<TestTimes; i++)
            {
                temp += ((CheParam_U[i]-CheParam_avr_U)*(CheParam_U[i]-CheParam_avr_U));
            }
            CheParam_AARD = 100*(sqrt((1.0/(TestTimes-1))*temp))/CheParam_avr_U;  //相对标准偏差百分比
        }

        area[0] = 0;
        area[1] = 25;
        area[2] = 239;
        area[3] = 159;
        CleanVideo(area);

        j = 0 ;
        for(i=0; i<TestTimes; i++)
        {
            if((i % 2)) //第二列
            {
                WriteCode1212(140,20+17*j,"u : ");

                DisValue(i,146,20+17*j,1,1,0,0);
                DisValue(CheParam_U[i],164,20+17*j,1,4,3,1);
                j++;
            }
            else   //第一列
            {
                WriteCode1212(40,20+17*j,"u  : ");
                DisValue(i,46,20+17*j,1,1,0,0);
                DisValue(CheParam_U[i],64,20+17*j,1,4,3,1);
            }

        }

        WriteCode1212(20,20+17*5,"平均值:    ");
        DisValue(CheParam_avr_U,62,20+17*5,1,4,3,1);

        ParOldU = 	ReadFp32(JIAOZHUN_BIAOPIAN);
        WriteCode1212(140,20+17*5,"记录值:    ");
        DisValue(ParOldU,185,20+17*5,1,4,3,1);


        WriteCode1212(20,20+17*6,"相对标准偏差:       %");
        DisValue(CheParam_AARD,98,20+17*6,1,3,1,1);


        prompt(0);	// 提示 确定

        CaliFlg = 4;
        break;

    case 4:
        CaliFlg = 5;
        break;

    default:
        break;
    }

    //放射源复位
    OSMutexPend(OSUart0UsedMutex,0,&err);
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl1);
    TTLOutput (TTL_2,1,TTL_BRAY_Ctrl2);
    OSMutexPost(OSUart0UsedMutex);

    OSTimeDlyHMSM(0, 0, C14MotMoveTime,0);	//运行时间

    OSMutexPend(OSUart0UsedMutex,0,&err);
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl1);
    TTLOutput (TTL_2,0,TTL_BRAY_Ctrl2);
    OSMutexPost(OSUart0UsedMutex);
    // 请输入标片对应  值。  确定返回  重新测量。 确定后将自动计算校准参数并保存。
}

