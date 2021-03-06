/****************************************Copyright (c)****************************************************
**                            		skyray-instrument Co.,LTD.
**
**                                 http://www.skyray-instrument.com
**
*********************************************************************************************************/

/*--------------------------------------------
display area:

         0
           1
           12
         13
------------------
14
19
------------------19
20
  21
  32
33
-------------------34
35
  36
  47
48
-------------------49
50
  51
  62
63
-------------------
/////////////////////////////////
First display
        0
          1   data time
          12
        13
          14

              ph
          48
        49-------------------------
        50
           51
           62 Temperature
        63
--------------------------------------------*/

#include "menu.h"
#include <includes.h>
int MainMeauFlg;
uint32 MenuId=0x00000000;
uint32 MenuIdBak=0;
uint32 MenuHistory=0x01000000;                             //menu id
uint8  language=1;//=1;
uint8  language1=0;//=1;
uint8  showFlg = 1;
uint8  DisUnits=1;
uint32 UserPassword=0;
uint8 RunStuation, WarnStuation,
      EeorT,EeorH,EeorL,EeorP,EeorMear,EeorMenmory ;
uint16 EEORStuation1;

uint8 KeyUpRun=0,TimeOverFlg;
uint8 KeyDownRun=0;
uint8 KeyLeftRun=0;
uint8 KeyRightRun=0;
uint8 KeyMenuRun=0;
uint8 KeyEnterRun=0;
uint8 KeyDRRun=0,CountReadTime;
uint8 area[4];

//get num
uint8  GetNumSign=1;
uint32 GetInNum=0;
uint32 GetNumMenu=0;
uint8 const areaMenu[][4] = {
    {0,0,239,17},
    {12,40,68,96},
    {92,40,148,96},
    {172,40,228,96},
    {12,99,68,155},
    {92,99,148,155},
    {172,99,228,155},
};


uint8 const prompt_inf[][40] =
{
    {"ESC:返回                  () : 确 定"},	 //	0
    {"ESC:返回 #$ / }&: +  -    () : 确 定"},
    {"                          () : 确 定"},
    {"ESC:返回                  () :开始/暂停"},	// 3
    {"         #$ / }&: 上 下   () : 详 细"},
    {"         #$ / }&: 上 下   () : 确 定"},
    {"ESC:返回                            "},	 //6
//{"X显示比例:        Y显示比例:        "},
    {"    I1:                  I2:           "},


    {"ESC:Back                  () : Ensure"},
    {"ESC:Back #$ / }&:Add/Subt () : Ensure"},
    {"                          () : Ensure"},
    {"ESC:Back                  () :Star/Stop"},
    {"         #$ / }&: Up/Down () : Details"},
    {"         #$ / }&: Up/Down () : Ensure"},
    {"ESC:Back                             "},
    {"    I1:                  I2:           "},

};

uint8 const NextPrompt[][30] =
{
//中文
    {"档位"},					   //0-	5 Inint
    {"正在自检........"},
    {"正在自检..EEPROM OK!   "},
    {"正在自检..EEPROM Error!"},
    {"正在读取参数..........."},
    {"正在复位..........."},

    {"恢复出厂设置?"},				 //6--10
    {"颗粒物质量浓度"},
    {"运行模式:"},
    {"时间段:"},
    {"运行阶段:"},

    {"均值浓度/0.5H："},			 //11 -15
    {"均值浓度/1H："},
    {"均值浓度/3H："},
    {"均值浓度/24H："},
    {"DD"},

    {"打开"},		//16--20
    {"关闭"},
    {"英文"},
    {"中文"},
    {"周期"},

    {"实时"},	 //21--	25
    {"压紧"},
    {"松开"},
    {"正在松开电机,请稍后.."},
    {"请取下滤纸或标片"},

    {"请换上标片,并按确定"},		  //26--30
    {"测量   s,请稍后.."},
    {"正在压紧电机,请稍后.."},
    {"完成"},
    {"正在压紧电机,请稍后.."},

    {"已经测量   秒"},		  //31--35
    {"无记录!"},
    {"退出"},
    {"完成"},
    {"请稍后..."},

    {"/数据内存"},		  //36--40
    {"/采样测量"},
    {"/压    力"},
    {"/流    量"},
    {"/加热系统"},

    {"/温    度"},		  //41--45
    {"本次值:"},
    {"平均值:"},
    {"阶段:初始化  "},
    {"阶段:空白计数"},//{"阶段:走纸    "},

    {"阶段:富集    "},//46--50
    {"阶段:测量计算"},
    {"阶段:走纸    "},
    {"仪器暂停     "},
    {"运行状态:"},

    {"警告状态:"},//51--55
    {"错误状态:"},
    {"通信状态:"},
    {"正常"},
    {"颗粒物浓度"},

    {"运行模式:"},//56--60
    {"实时模式"},
    {"均值浓度: 单位 ug/m3"},
    {"周期模式"},
    {"富集时间:            "},

    {"提示:富集将开始于   :00"},//61---65
    {"阶段:实时测量"},
    {"按确定开始或结束"},
    {"进入将停止测量流程"},
    {"  请先解开键盘锁  "},

    {"正在运行..."},//66---70
    {"即将恢复默认校准值"},
    {"(PM10)"},
    {"(PM2.5)"},
    {"(TSP)"},

    {"网口"},//71---75
    {"串口"},
    {""},
    {""},
    {""},

//英文						   //
    {"Gear"},
    {"Self-Checking...."},
    {"Self-Checking..EEPROM OK!   "},
    {"Self-Checking..EEPROM Error!"},
    {"Reading Parameters..........."},
    {"Resetting..................."},

    {"Restore default?"},			//37-41
    {"PM CONC:"},
    {"Run mode :"},
    {"Time intervals:"},
    {"Run stage:"},

    {"CONC/0.5H:"},//42--46
    {"CONC/1H:"},
    {"CONC/3H:"},		  //?
    {"CONC/24H:"},
    {"DD"},

    {"Open"},//47--51
    {"Close"},
    {"English"},
    {"Chinese"},
    {"Cycle"},

    {" RT  "},		 //52--56
    {"Compact"},
    {"Release"},
    {"Please Wait"},
    {"Remove the filter paper"},

    {"Replace a standard film"},					   //57-61
    {"Test   s,please wait"},
    {"motor working,plese wait"},
    {"DONE"},
    {"motor working,plese wait"},

    {"Time :      s"},					   //57-61
    {"No Record!"},
    {"Exit"},
    {"DONE"},
    {"plese wait"},

    {"/memory  "},		  //36--40
    {"/measure "},
    {"/Pressure"},
    {"/Flow    "},
    {"/heating "},

    {"/temperature"},		  //41--45
    {"Count :"},
    {"Average"},
    {":initializatio"},
    {": paper feed  "},

    {":blank        "},
    {":accumulation "},//46--50
    {":measure&calcu"},
    {":Suspend      "},
    {"work    :"},

    {"warning :"},
    {"error   :"},//51--55
    {"Communication:"},
    {"ok"},
    {"PM CON :"},

    {"Run mode:"},
    {"real time:"},//56--60
    {"CON: unit(ug/m3)"},
    {"cycle"},
    {"Time:            "},

    {"measurement starts at  :00"},
    {":real-time    "},//61---65
    {"Click to start or end"},
    {"Process  will stop"},
    {"Unlock the  keypad"},


    {"Running.. "},
    {"The default"},//66-70
    {"(PM10)"},
    {"(PM2.5)"},
    {"(TSP)"},

    {"Internet"},
    {"UART"},//71-75
    {""},
    {""},
    {""},

};
uint8 const LocPrompt[][10] =
{
    {"/计数器  "},
    {"/滤纸断裂"},
    {"/大气压  "},
    {"/流    量"},
    {"/加热系统"},//0-4

    {"/湿    度"},
    {"/温  度T1"},
    {"/温  度T2"},
    {"/温  度T3"},
    {"/内    存"},//5-9

//EN
    {"/counter "},
    {"/Fi Paper"},
    {"/Pressure"},
    {"/Flow    "},
    {"/Heating "},//10-14

    {"/HR      "},
    {"/T1      "},
    {"/T2      "},
    {"/T3      "},
    {"/Memory  "},		  //15--19
};
uint8 areaMiddleEE[4] = {90,139,150,159};//中间确定键处框
uint8 areaMiddle[4] = {85,116,145,139};	// 中间确定键处框

MenuStruct const MenuFormat= {
    1,
    1,
    20,
    1,
    {0,0,239,17},
    3,
    {   {128,26,208,41},                 //设置子菜单1区域(for 反白)
        {128,48,208,63},                 //设置子菜单2区域(for 反白)
        {128,70,208,85},                 //设置子菜单3区域(for 反白)
        {128,92,208,107},                //设置子菜单4区域(for 反白)
        {128,114,208,129},               //设置子菜单5区域(for 反白)
        {128,136,208,151},               //设置子菜单6区域(for 反白)
        {0,0,0,0},                     //设置子菜单7区域(for 反白)
        {0,0,0,0},                     //设置子菜单8区域(for 反白)
    },

    {
        {4,26},
        {4,48},
        {4,70},
        {4,92},
        {4,114},
        {4,136},
        {0,0},
        {0,0},
    },

    {   {0,25,217,42},                 //子菜单1区域(for 反白)
        {0,47,217,64},                 //子菜单2区域(for 反白)
        {0,69,217,86},                 //子菜单3区域(for 反白)
        {0,91,217,108},                //子菜单4区域(for 反白)
        {0,113,217,130},               //子菜单5区域(for 反白)
        {0,135,217,152},               //子菜单6区域(for 反白)
        {0,0,0,0},                     //子菜单7区域(for 反白)
        {0,0,0,0},                     //子菜单8区域(for 反白)
    },

    22,
    36,
    {5,31,0,0},
    24,
    28,
    {0,0,0,0},


};

const DISDATSTR  DisDat2[NUM_PARAM]=
{   //参数类型，				字体大小，	长度	小数位数	是否显示正负
    {PARAM_NOMAL,   			2,       	3,   	0,			0},
    {PARAM_SHIDUMENFA,			2,			3,		0,			0},
    {PARAM_MAXHEAT,				2,			4,		1,			0},
    {PARAM_MINHEATPOWER,		2,			3,		0,			1},
    {PARAM_LIULIANG,			2,			4,		2,			0},
    {PARAM_CYCLEOFSAMP,			2,			3,		0,			0},
    {PARAM_HIGHV,				2,			4,		0,			0},
    {PARAM_BIAOPIAN_JIAOZHUN,	2,			4,		0,			0},
    {PARAM_T1_JIAOZHUN,			2,			4,		1,			0},
    {PARAM_RH_JIAOZHUN,			2,			3,		0,			0},
    {PARAM_QIYA_JIAOZHUN,		2,			5,		2,			0},
    {PARAM_LIULIANG_JIAOZHUN,	2,			4,		2,			0},
    {PARAM_PUMP_JIAOZHUN,		2,			3,		0,			0},
    {PARAM_DUIBIDU,				2,			1,		0,			0},
    {PARAM_MONITONGDAO,			2,			2,		0,			0},
    {PARAM_MONIZHI,				2,			3,		1,			0},
    {PARAM_T3_JIAOZHUN,			2,			4,		1,			0},
    {PARAM_MASS,				2,			4,		0,			0},
    {PARAM_MOTO,				2,			3,		1,			0},
    {PARAM_ERR,   				2,			1,		0,			0},
    {PARAM_B,   				2,			2,		0,			1},
    {PARAM_K,   				2,			3,		2,			0},
    {PARAM_TESTTIMES,   		2,			1,		0,			0},
    {PARAM_GUANGSAN_CYCLE,		2,			4,		0,			0},
    {PARAM_GUANGSAN_PM25_K,		2,			4,		2,			0},
    {PARAM_GUANGSAN_PM10_K,		2,			4,		2,			0},
    {PARAM_GUANGSAN_TSP_K,		2,			4,		2,			0},
    {PARAM_SENSOR,				2,			1,		0,			0},
} ;
/*********************************************************************************************************
* Function name: prompt
* Descriptions: 提示信息
* input parameters:   type 提示信息种类 0-5
						0 esc         enter
						1 esc  updown enter
						2			  enter
						3 esc  updown enter
						4 esc  enter
						5 esc  updown enter
* output parameters:无
* Returned value: 无
*********************************************************************************************************/

void prompt(uint8 type)
{
    uint8 area_enter0[4] = {154,147,181,159};
    uint8 area_enter1[4] = {181,147,181,159};

    uint8 area_ESC0[4] = {2,147,26,159};
    uint8 area_ESC1[4] = {26,147,26,159};

    uint8 area_UPDOWN0[4] = {56,147,103,159};
    uint8 area_UPDOWN1[4] = {103,147,103,159};
    uint8 loc_offset;

    loc_offset = (language)?(8):(0);

    switch(type)
    {
    case 0:
        WriteCode1212(2,147,prompt_inf[0+loc_offset]);	//
        ReverseVideo(area_enter0);
        ReverseVideo(area_enter1);
        ReverseVideo(area_ESC0);
        ReverseVideo(area_ESC1);
        break;
    case 1:
        WriteCode1212(2,147,prompt_inf[1+loc_offset]);	//
        ReverseVideo(area_enter0);
        ReverseVideo(area_enter1);
        ReverseVideo(area_UPDOWN0);
        ReverseVideo(area_UPDOWN1);
        ReverseVideo(area_ESC0);
        ReverseVideo(area_ESC1);
        break;
    case 2:
        WriteCode1212(2,147,prompt_inf[2+loc_offset]);	//
        ReverseVideo(area_enter0);
        ReverseVideo(area_enter1);
        break;
    case 3:
        WriteCode1212(2,147,prompt_inf[3+loc_offset]);	//
        ReverseVideo(area_ESC0);
        ReverseVideo(area_ESC1);
        ReverseVideo(area_enter0);
        ReverseVideo(area_enter1);
        break;
    case 4:
        WriteCode1212(2,147,prompt_inf[4+loc_offset]);	//
        ReverseVideo(area_enter0);
        ReverseVideo(area_enter1);
        ReverseVideo(area_UPDOWN0);
        ReverseVideo(area_UPDOWN1);
        break;

    case 5:
        WriteCode1212(2,147,prompt_inf[5+loc_offset]);	//
        ReverseVideo(area_enter0);
        ReverseVideo(area_enter1);
        ReverseVideo(area_UPDOWN0);
        ReverseVideo(area_UPDOWN1);
        break;
    case 6:
        WriteCode1212(2,147,prompt_inf[6+loc_offset]);	//
        ReverseVideo(area_ESC0);
        ReverseVideo(area_ESC1);
        break;
    case 7:
        WriteCode1212(2,130,prompt_inf[7+loc_offset]);	//
        break;
    default :
        break;
    }
    DarwHLine(0,239,145);
}


/*********************************************************************************************************
* Function name:DisPleaseWait
* Descriptions: 显示please wait ***
* input parameters:   无
* output parameters:无
* Returned value: 无
*********************************************************************************************************/
void DisPleaseWait(void)
{
    uint8 cs[20]= {"Please Wait..."};
    ClrDisBuf();
    DisEList168(8,20,cs);
    LcdUpdata(DisBuf);
}

void MainMenuSen()               //  传感器值显示界面1�
{
    area[0] = 0;
    area[1] = 22;
    area[2] = 239;
    area[3] = 159;
    CleanVideo(area);		/*区域清屏*/

    WriteCode1212(12,25,"环境T1:");
    WriteCode1212(12,50,"检测舱T3:");
    WriteCode1212(12,75,"DHS-T2:");
    WriteCode1212(12,100,"PMT-T4:");
    WriteCode1212(12,125,"标况V1:");

    DisDat.font = 1;
    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 1;
    DisValue2(WenDuT1,12+50,25,&DisDat," ℃");
    DisValue2(WenDuT3,12+60,50,&DisDat," ℃");
    DisValue2(WenDuT2,12+50,75,&DisDat," ℃");
    DisValue2(TCQWenDuT4,12+50,100,&DisDat," ℃");

    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 0;
    DisValue2(TV_GK/60.0,12+50,125,&DisDat," L");

    WriteCode1212(127,25,"环境RH2:");
    WriteCode1212(127,50,"检测舱RH1:");
    WriteCode1212(127,75,"大气压P:");
    WriteCode1212(127,100,"工况 C2:");
    WriteCode1212(127,125,"标况 F1:");

    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 0;
    DisValue2(QiYa,127+50,75,&DisDat,         "Kpa");
    DisDat.decimal = 2;
    DisValue2(LiuLiang_BK,127+50,125,&DisDat,   "L/min");

    DisDat.length = 5;
    DisDat.decimal = 1;
    DisDat.DisSign = 0;
    DisValue2(Parg_PM25_GK,127+50,100,&DisDat,"ug/m3");
    DisValue2(HumiditySample,127+60,50,&DisDat," %");
    DisValue2(CYCShiDu,127+50,25,&DisDat," %");
    prompt(4);
}

void MainMenuSen2()
{
    area[0] = 0;
    area[1] = 22;
    area[2] = 239;
    area[3] = 159;
    CleanVideo(area);		/*区域清屏*/

    WriteCode1212(12,25,"工况V2:");
    WriteCode1212(12,50,"电压v1:");
    WriteCode1212(12,75,"电压V2:");

    DisDat.font = 1;
    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 1;
    DisValue2(PowerV1,12+50,50,&DisDat," V");    //显示5v电压
    DisValue2(PowerV2,12+50,75,&DisDat," V");    //显示12v

    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 0;
    DisValue2(TV_BK/60.0,12+50,25,&DisDat," L");

    WriteCode1212(127,25,"工况F2:");
    WriteCode1212(127,50,"电压v3:");
    WriteCode1212(127,75,"电压V4:");

    DisDat.length = 4;
    DisDat.DisSign = 0;
    DisDat.decimal = 2;
    DisValue2(LiuLiang_GK,127+50,25,&DisDat, "L/min");

    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 1;

    DisValue2(PowerV3,127+50,50,&DisDat," V");    //显示-12电压
    DisValue2(PowerV4,127+50,75,&DisDat," V");    //显示24v

    prompt(4);
}

void OutputStation(void)    //4-20ma输出页面
{
    area[0] = 0;
    area[1] = 20;
    area[2] = 239;
    area[3] = 159;
    CleanVideo(area);		/*区域清屏*/

    WriteCode1212(12,20,       "0 环境   T1:");       //环境嘉露�
    WriteCode1212(12,20+16*1,  "1 伴热管 T2:");       //加热管温度
    WriteCode1212(12,20+16*2,  "2 测试舱 T3:");       //测试仓温度
    WriteCode1212(12,20+16*3,  "3 探测器 T4:");
    WriteCode1212(12,20+16*4,  "4 测试舱RH1:");
    WriteCode1212(12,20+16*5,  "5 环境  RH2:");
    WriteCode1212(12,20+16*6,  "6 剩余滤纸R:");
    WriteCode1212(12,20+16*7,  "7 测试结果C:");

    DisDat.font = 1;
    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 0;
    DisValue2(measRecord.recordResult,85,20+16*7,&DisDat," ug/m3");
    DisDat.font = 1;
    DisDat.length = 3;
    DisDat.decimal = 0;
    DisDat.DisSign = 0;

    DisValue2(FilterRemain,85,20+16*6,&DisDat," %");
    DisValue2(HumiditySample,85,20+16*4,&DisDat," %");
    DisValue2(CYCShiDu,85,20+16*5,&DisDat," %");

    DisDat.font = 1;
    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 1;
    DisValue2(WenDuT1,85,20,&DisDat,  " ℃");
    DisValue2(WenDuT2,85,20+16*1,&DisDat," ℃");
    DisValue2(WenDuT3,85,20+16*2,&DisDat," ℃");
    DisValue2(TCQWenDuT4,85,20+16*3,&DisDat," ℃");

    //电压值

    DisDat.font = 1;
    DisDat.length = 3;
    DisDat.decimal = 1;
    DisDat.DisSign = 0;
    DisValue2(outputDat[0]/1000,180,20,&DisDat,"mA");
    DisValue2(outputDat[1]/1000,180,20+16*1,&DisDat,"mA");
    DisValue2(outputDat[2]/1000,180,20+16*2,&DisDat,"mA");
    DisValue2(outputDat[3]/1000,180,20+16*3,&DisDat,"mA");
    DisValue2(outputDat[4]/1000,180,20+16*4,&DisDat,"mA");
    DisValue2(outputDat[5]/1000,180,20+16*5,&DisDat,"mA");
    DisValue2(outputDat[6]/1000,180,20+16*6,&DisDat,"mA");
    DisValue2(outputDat[7]/1000,180,20+16*7,&DisDat,"mA");

    prompt(4);
    LcdUpdata(DisBuf);

}

void SensorMenu()    //维护操作-传感器页面
{
    area[0] = 0;
    area[1] = 20;
    area[2] = 239;
    area[3] = 159;
    CleanVideo(area);		/*区域清屏*/

    WriteCode1212(12,20,       "环境温度   T1:");            //环境嘉露�
    WriteCode1212(12,20+16*1,  "伴热管温度 T2:");       //加热管温度
    WriteCode1212(12,20+16*2,  "测试舱温度 T3:");       //测试仓温度
    WriteCode1212(12,20+16*3,  "探测器温度 T4:");
    WriteCode1212(12,20+16*4,  "测试舱湿度RH1:");
    WriteCode1212(12,20+16*5,  "环境湿度  RH2:");
    WriteCode1212(12,20+16*6,  "大气压力    P:");
    WriteCode1212(12,20+16*7,  "工况流量   F2:");

    DisDat.font = 1;
    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 1;
    DisValue2(WenDuT1,100,20,&DisDat," ℃");      //环境温度
    DisValue2(WenDuT2,100,20+16*1,&DisDat," ℃"); //加热管温度
    DisValue2(WenDuT3,100,20+16*2,&DisDat," ℃"); //测试仓温度
    DisValue2(TCQWenDuT4,100,20+16*3,&DisDat," ℃"); //测试仓温度

    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 0;
    DisValue2(QiYa,106,20+16*6,&DisDat,"Kpa");
    DisDat.decimal = 2;
    DisValue2(LiuLiang_GK,106,20+16*7,&DisDat,"L/h");

    DisDat.length = 2;
    DisDat.decimal = 0;
    DisDat.DisSign = 0;
    DisValue2(HumiditySample,106,20+16*4,&DisDat,"%");
    DisValue2(CYCShiDu,106,20+16*5,&DisDat,"%");
    //电压值

    DisDat.font = 1;
    DisDat.length = 4;
    DisDat.decimal = 3;
    DisDat.DisSign = 1;
    DisValue2((GetADValue(AD_WENDU1)),180,20,&DisDat,"V");
    DisValue2(GetADValue(AD_WENDU2),180,20+16*1,&DisDat,"V");
    DisValue2(GetADValue(AD_WENDU3),180,20+16*2,&DisDat,"V");
    DisValue2(GetADValue(AD_TCQ_WENDU),180,20+16*3,&DisDat,"V");
    DisValue2(GetADValue(AD_SHIDU),180,20+16*4,&DisDat,"V");
    DisValue2(GetADValue(AD_CYK_SHIDU),180,20+16*5,&DisDat,"V");
    DisValue2(GetADValue(AD_QIYA),180,20+16*6,&DisDat,"V");
    DisValue2(GetADValue(AD_LIULIANG),180,20+16*7,&DisDat,"V");

    prompt(4);
    LcdUpdata(DisBuf);
}

void DisHexDat(uint8 *arr,uint8 dat)
{
    uint8 *p;
    uint8 i;
    if(dat>256)
        return ;
    p = arr;
    i= (dat>>4);	//先取高位
    (i>9)?(*p = i + '0'+7):(*p = i + '0');
    p++;
    i=(dat&0x0F); //再取低位
    (i>9)?(*p = i + '0'+7):(*p = i + '0');
    p++;
    *p = 0;
}

void ShowEeor(uint16 errdat1)
{
    uint8 locOffset;

    int i=0,j=0,k=0;
    locOffset = (language)?(10):(0); 	// 英文	中文

    for(i=0; i<10; i++)
    {
        if((errdat1>>1*i)&0x01)
        {
            WriteCode1212(27+j*107,65+k*20,LocPrompt[i+locOffset]);
            j++;
            if(j>1) {
                j=0,k++;
            }
        }
    }
}

void ErrDatChange(void)
{
    EEORStuationOld = EEORStuation1;
    EEORStuation1 = 0;
    if((EeorMear&0x01) ==  0x01)//
    {
        EEORStuation1 |= 0x0001;
    }
    if((EeorMear&0x20) ==  0x20)
    {
        EEORStuation1 |= 0x0002;
    }
    if((EeorP&0x01) ==  0x01)
    {
        EEORStuation1 |= 0x0004;
    }
    if((EeorL&0x01) ==  0x01)//
    {
        EEORStuation1 |= 0x0008;
    }
    if((EeorH&0x01) ==  0x01)//
    {
        EEORStuation1 |= 0x0010;
    }
    if((EeorH&0x02) ==  0x02)//
    {
        EEORStuation1 |= 0x0020;
    }
    if((EeorT&0x01) ==  0x01)//
    {
        EEORStuation1 |= 0x0040;
    }

    if((EeorT&0x02 )==  0x02)//
    {
        EEORStuation1 |= 0x0080;
    }
    if((EeorT&0x04) ==  0x04)//
    {
        EEORStuation1 |= 0x0100;
    }

    if((EeorMenmory&0x01)==  0x01)//
    {
        EEORStuation1 |= 0x0200;
    }
}

void MainMenuSta()
{
    uint8 DisNumDat[3],i,j;
    area[0] = 0;
    area[1] = 22;
    area[2] = 239;
    area[3] = 159;
    CleanVideo(area);		/*区域清屏*/
    /*
    //所以运行状态包括：
    01 	走纸
    02 	空白计数
    04  富集
    08 	测量计数
    10 手动操作暂 （脱机键盘启动）
    20 模式 （周期和实时）
    40 整点模式
    80 DHS开关
    */

    DisCList16(12 ,25,NextPrompt[50+NePrOffset]);
    DisCList16(120,25,NextPrompt[51+NePrOffset]);
    //运行状态更新
    RunStuation = 0;
    if((MeasureStepFlg<5)&&(MeasureStepFlg>0))
    {
        RunStuation =  (HeatOn<<7)|(TimeType<<6)|(StopFlg<<4)|(RunStyleFlg<<5)|(1<<(MeasureStepFlg-1));
    }
    else
    {
        RunStuation =  (HeatOn<<7)|(TimeType<<6)|(StopFlg<<4)|(RunStyleFlg<<5);

    }
    DisHexDat(DisNumDat,RunStuation);
    DisCList16(12+72,25,DisNumDat);

    /*
    所有警告状态包括：
    01 相对湿度大于（设定值的+10%）
    02 超过质量限，滤待将更换  //实时模式才用

    */

    //警告状态更新
    if(HumiditySample > Humidity*1.1)
    {
        WarnStuation |= 0x01;
    }
    else
    {
        WarnStuation &= 0xfe;
    }
    DisHexDat(DisNumDat,WarnStuation);
    DisCList16(120+72,25,DisNumDat);
    (WarnStuation != 0)?(SET_LED2(1)):(SET_LED2(0));//亮灯。



//	WriteCode1212(40,47,"手工更换滤纸/零点调整/手动操作");
//	DisCList16(12,67,"警告状态:03");
//	WriteCode1212(40,87,"泵最大功率/超过质量限/滤纸将更换");
//	WriteCode1212(40,100,"相对湿度大于设定值");
    /*
    // 	错误状态 有
    // 	 00      00     00    00    00    00   00           00
    // 	未使用	浊度计	温度  加热	流量  压力 采样和测量	数据和程序内存状态
    // 	从最低位说起
    // 	数据内存： 01 EEPROM 错误
    // 	采样和测量系统：01 检测器错误 02 滤带断裂
    // 	压力传感器：01 大气压 超过范围
    // 	流量调节状态总和： 01 调节偏差大于2%
    // 	加热状态： 01 T2超过最大加热温度》10摄氏度  02相对湿度传感器超过范围
    // 	温度：  01 温度T1超过量程  02 t2超过量程  04 温度t3超过量程

    最新 共16位。高6位保留， 内存 T3 T2 T1 湿度 伴热带，流量 气压 滤带 计数

    */

    DisCList16(12,45,NextPrompt[52+NePrOffset]);
    DisHexDat(DisNumDat,EEORStuation1>>8);	//
    DisCList12(12+72,45,DisNumDat);

    DisHexDat(DisNumDat,(uint8)(EEORStuation1));	//
    DisCList12(12+72+12,45,DisNumDat);

    ShowEeor(EEORStuation1);


    (WarnStuation|EEORStuation1)?(SET_LED3(0)):(SET_LED3(1));




//显示通信错误值
    DisCList16(12,123,NextPrompt[53+NePrOffset]);

    for(i = 0,j=0; i<16; i++)
    {
        if(CmErrorState[i]!=0)
        {
            DisHexDat(DisNumDat,CmErrorState[i]);	//	压力
            DisCList12(12+80+j*14,125,DisNumDat);
            j++;
        }

    }
    if(j==0)
    {
        DisCList16(92,123,NextPrompt[54+NePrOffset]);
    }
    DisHexDat(DisNumDat,counterr);	//	通信错误，复位次数。
    DisCList12(142,123,DisNumDat);

    DisHexDat(DisNumDat,FilterErrTimes);	//	计数器读错误
    DisCList12(155,123,DisNumDat);


    prompt(4);

}
void MainMenu()
{
    uint8 aprr;
    area[0] = 0;
    area[1] = 22;
    area[2] = 239;
    area[3] = 159;

    CleanVideo(area);		/*区域清屏*/
    DisCList16(12,36,NextPrompt[55+NePrOffset]);//颗粒物浓度
    DisCList16(12+5*16,36,NextPrompt[68+UseFor+NePrOffset]);//PM2.5 PM10 TSP


    DisDat.font = 3;
    DisDat.length = 5;
    DisDat.decimal = 1;
    DisDat.DisSign = 0;
    if(measRecord.recordResult < 0)
    {
        measRecord.recordResult = 0;
    }

#ifdef YI_TI_JI //不是一体机
    switch(UseFor)
    {
    case 0:
        DisValue(PM10_GS_M,25,54,3,5,1,0);
        DisValue2(measRecord.recordResult,110,54,&DisDat,"ug/m3");
        break;
    case 1:
        DisValue(PM25_GS_M,25,54,3,5,1,0);
        DisValue2(measRecord.recordResult,110,54,&DisDat,"ug/m3");
        break;
    case 2:
        DisValue(TSP_GS_M,25,54,3,5,1,0);
        DisValue2(measRecord.recordResult,110,54,&DisDat,"ug/m3");
        break;
    default:
        break;
    }
#else
    //显示测量结果 注销掉原来只显示一个的贝塔法结果
    DisValue2(measRecord.recordResult,80,54,&DisDat,"ug/m3");
#endif

    WriteCode1212(12,111,NextPrompt[56+NePrOffset]);//运行模式
    switch(RunStyleFlg)
    {
    case SHISHI:
        WriteCode1212(12+9*6,111,NextPrompt[57+NePrOffset]);// 实时模式
        break;
    case ZHOUQI:
        WriteCode1212(12+9*6,111,NextPrompt[59+NePrOffset]);//周期模式
        WriteCode1212(12,91,NextPrompt[60+NePrOffset]);//富集时间
        //显示日期

        TimeDateKey = measRecord.recordTimeStart;
        DisTime(12+9*6,91,TimeDateKey,3 );
        DisCList16(12+9*6+5*6,91,"~");
        TimeDateKey = measRecord.recordTimeEnd;
        DisTime(12+9*6+5*6+8,91,TimeDateKey,3);


        //周期后面加个整点
        if(TimeType == 1)
        {
            WriteCode1212(12+17*6,111," -- 整点");// ***开始于
        }

        if((TimeType == 1)&&(StopFlg == 0)&&(showFlg == 1))
        {
            WriteCode1212(12,131,NextPrompt[61+NePrOffset]);// ***开始于
            TimeDateKey.hour = TimeDate.hour+1;
            aprr = 	(uint8)(TimeDateKey.hour/10+'0') ;
            WriteCode1212(12+8*12+8,131,&aprr);
            aprr = 	 ('0'+TimeDateKey.hour%10);
            WriteCode1212(12+8*12+16,131,&aprr);
        }
        //读值，读时间段。
        //显示
//			DisCList16(32,130,"11.23 12:00~~11.23 13:00");
        break;
    }

//	DisCList16(12,113,"运行阶段:");

//取值





//单位
//	DisCList16(140,36,"45.33ug/m3");


    prompt(4);

}
/*********************************************************************************************************
* Function name:   NewLcdVol
* Descriptions:      刷新显示
* input parameters:   menuid
* output parameters:无
* Returned value: 0
*********************************************************************************************************/
/*********************************************************************************************************
* Function name:   Menm0Updata
* Descriptions://Measure refurbish 主界面显示  时间 电量
* input parameters:   无
* output parameters:无
* Returned value: 无
*********************************************************************************************************/
void Menm0Updata(void)
{
    uint8 areaMenu1[4] = {0,0,239,17};
    CleanVideo(areaMenu1);
    DisDate(10,3,TimeDate,1);
    DisTime(90,3,TimeDate,3);
    if(0==MenuId)
    {
        switch (MainMeauFlg)
        {
        case 0:
            MainMenu();
            break;
        case 1:
            MainMenuSta();
            break;
        case 2:
            MainMenuSen2();
            break;
        case 3:
            MainMenuSen();
            break;
        default:
            break;
        }
    }
    if(0x02610000 == (MenuId&0x0fff0000))//在传感器测试界面
    {
        SensorMenu();
    }
    if(0x02710000 == (MenuId&0x0fff0000))//模拟输出界面
    {
        OutputStation();
    }
    switch(MeasureStepFlg)
    {
    case 0:
        WriteCode1212(156,3,NextPrompt[44+NePrOffset]);	//
        break;
    case 1:
        WriteCode1212(156,3,NextPrompt[45+NePrOffset]);	//
        break;
    case 2:
        if(RunStyleFlg == SHISHI)
        {
            WriteCode1212(156,3,NextPrompt[62+NePrOffset]);	//

        }
        else if(RunStyleFlg == ZHOUQI)
        {
            WriteCode1212(156,3,NextPrompt[46+NePrOffset]);	//
        }

        break;
    case 3:
        WriteCode1212(156,3,NextPrompt[47+NePrOffset]);	//
        break;
    case 4:
        WriteCode1212(156,3,NextPrompt[48+NePrOffset]);	//
        break;
    default :
        break;
    }
    if(StopFlg == 1)
    {
        WriteCode1212(156,3,NextPrompt[49+NePrOffset]);	//
    }
    CReverseVideo(areaMenu[0]);
    LcdUpdata(DisBuf);
}


void showCountDat(void)
{
    uint8 slen,err;
    CountReadTime = 180;
    while(CountReadTime > 0)
    {
        if(keyEnterFlg == 0)
            break ;
        OSMutexPend(OSLcdUsedMutex,1,&err);
        CleanVideo(area);
        DisCList16((96-(4*16)),96 ,NextPrompt[42+NePrOffset]);	//本次值
        DisCList16((96-(4*16)),120,NextPrompt[43+NePrOffset]);	//平均值
        prompt(3);
        DisDat.length = 5;
        DisDat.decimal = 0;
        DisDat.DisSign = 0;
        DisDat.font = 2;
        DisValue2(CountNow,96,96, &DisDat ,"");
        CountAver =  CountTotal / CountTimes;
        DisValue2(CountAver,96,120, &DisDat ,"");
        slen = StringLen(NextPrompt[31+NePrOffset]); //   测量200s	稍后
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[31+NePrOffset]);
        DisValue2((int)(CountTimes),16*4+(DIS_AREA_X-slen*8)/2,70, &DisDat ,"");
        LcdUpdata(DisBuf);
        OSMutexPost(OSLcdUsedMutex);

        OSTimeDly(100);//1秒;
    }
    if(CountReadTime == 0)
    {
        TimeOverFlg = 1;
    }
}


void NewLcdVol(uint32 menuid)
{
    if((MenuId&0xffffff0f)==0x02510000)	//探测器测试显示
    {
        showCountDat();
    }
    return ;
}

/*********************************************************************************************************
* Function name: SetHoldregToE2rom
* Descriptions:         将hold寄存器的值写入 e2prom
* input parameters: type 写入的字节数类型
* output parameters:
* Returned value:
*********************************************************************************************************/
void SetHoldregToE2rom(uint8 type)
{
    uint8 i;
    switch(type)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        for(i=0; i<45; i++)
        {
            ;
        }
        break;
    case 0:
        for(i=0; i<MENUADDSIZE*45; i++)
        {
            ;
        }
        break;
    }

}
/*********************************************************************************************************
* Function name:   GetHoldregFormE2rom
* Descriptions:   从e2prom中读数到 hold寄存器
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
void GetHoldregFormE2rom(void)
{
    uint16 i;
    for(i=0; i<MENUADDSIZE*45; i++)
    {
        ;
    }
}
/*********************************************************************************************************
* Function name:GetMenuFormHoldreg
* Descriptions: 从寄存器中读值 并赋值给相应参数
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
void GetMenuFormHoldreg(void)
{
//	uint8 i,j;
//	;
}
/*********************************************************************************************************
* Function name:SetMenuList
* Descriptions:  测量参数赋值
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
void SetMenuList(void)
{
//	uint8 i,j,ss;
//	uint16 cd;
//	uint8 cc[] = "金属选择";
//	uint8 ee[] = "Select Metal";

    ;
}



/*
菜单结构体
*/
MenuTitel  MenuTab[MENU_SIZE]= { //MENU_SIZE 15*2 + 49
    {   0x00000000,3,1,1,3,  //1//1
        {0,1},

        {""},
        {""},
        {""},
        {""},
    },
    {   0x01000000,6, 6, 1, 4,//2//2
        {0,1},

        {"运行设置"},
        {"主菜单选择项"},
        {"Operation information"},
        {"Main Menu"},

    },

    {   0x02000000,6, 6, 1, 4,//3//3
        {0,2},

        {"维护操作"},
        {"主菜单选择项"},
        {"Maintenance Menu"},
        {"Main Menu"},

    },

    {   0x03000000,6, 6, 1, 4,//4//4
        {0,3},

        {"校准操作"},
        {"主菜单选择项"},
        {"Calibration operation"},
        {"Main Menu"},
    },

    {   0x04000000,6, 6, 1, 4,//5
        {0,4},

        {"历史记录"},
        {"主菜单选择项"},
        {"History Record"},
        {"Main Menu"},
    },

    {   0x05000000,6, 6, 1, 4,//6
        {0,5},

        {"系统设置"},
        {"主菜单选择项"},
        {"Settings"},
        {"Main Menu"},
    },

    {   0x06000000,6, 6, 1, 4,//7
        {0,6},

        {"关    于"},
        {"主菜单选择项"},
        {"About   "},
        {"Main Menu"},
    },

//运行设置
    {   0x01100000,8, 6, 0, 1,//8
        {"1"},

        {"运行模式"},
        {"运行设置"},
        {"Run Style"},
        {"Operation information"},
    },
    {   0x01200000,8, 6, 0, 1,//9
        {"2"},

        {"湿度门坎"},
        {"运行设置"},
        {"RH Gate"},
        {"Operation information"},
    },
    {   0x01300000,8, 6, 0, 1,//10
        {"3"},

        {"最大加热温度"},
        {"运行设置"},
        {"max heat T"},
        {"Operation information"},
    },
    {   0x01400000,8, 6, 0, 1,//11
        {"4"},

        {"最小加热功率"},
        {"运行设置"},
        {"Min Power"},
        {"Operation information"},
    },
    {   0x01500000,8, 6, 0, 1,//12
        {"5"},

        {"采样流量"},
        {"运行设置"},
        {"Flow"},
        {"Operation information"},
    },
    {   0x01600000,8, 6, 0, 1,//13
        {"6"},

        {"DHS加热控制"},
        {"运行设置"},
        {"DHS"},
        {"Operation information"},
    },

    {   0x01700000,8, 6, 1, 1,//14
        {"7"},

        {"模式设置"},
        {"运行设置"},
        {"Run Style Set"},
        {"Operation information"},
    },
//增加 温度补偿开关控制
    {   0x01800000,8,6,0,1,//15
        //菜单ID，同级菜单个数，一屏可显示菜单个数，有没有子菜单，菜单显示方式
        {"8"},
        //菜单前图标位置
        {"温度补偿"},
        {"运行设置"},
        {"temp compensate"},
        {"Maintenance Menu"},
    },

    {   0x01710000,3, 3, 0, 1,//16
        {"1"},

        {"整点模式设置"},
        {"运行设置"},
        {"top of hour"},
        {"Run Style Set"},
    },
    {   0x01720000,3, 3, 0, 1,//17
        {"2"},

        {"采样周期"},
        {"运行设置"},
        {"Cycle"},
        {"Run Style Set"},
    },

    {   0x01730000,3, 3, 0, 1,//18
        {"3"},

        {"质量限设置"},
        {"运行设置"},
        {"Mass"},
        {"Run Style Set"},
    },

//维护操作

    {   0x02100000,8, 6, 0, 1,//19
        {"1"},

        {"泵操作  "},
        {"维护操作"},
        {"Pump"},
        {"Maintenance Menu"},

    },

    {   0x02200000,8, 6, 0, 1,//20
        {"2"},

        {"测量室"},
        {"维护操作"},
        {"Measure room"},
        {"Maintenance Menu"},
    },

    {   0x02300000,8, 6, 0, 1,//21
        {"3"},

        {"移动滤纸带"},
        {"维护操作"},
        {"Move paper"},
        {"Maintenance Menu"},
    },

    {   0x02400000,8, 6, 0, 1,//22
        {"4"},

        {"加热系统"},
        {"维护操作"},
        {"Heat System"},
        {"Maintenance Menu"},
    },

    {   0x02500000,8,6,1,1,//23
        {"5"},

        {"探测器探测"},
        {"维护操作"},
        {"Counter"},
        {"Maintenance Menu"},

    },
    {   0x02600000,8,6,1,1,//24
        {"6"},

        {"传感器"},
        {"维护操作"},
        {"The sensor"},
        {"Maintenance Menu"},
    },

    {   0x02700000,8,6,1,1,//25
        {"7"},

        {"模拟输出测试"},
        {"维护操作"},
        {"Analog output"},
        {"Maintenance Menu"},
    },
//增加 放射源操作
    {   0x02800000,8,6,0,1,//26
        //菜单ID，同级菜单个数，一屏可显示菜单个数，有没有子菜单，菜单显示方式
        {"8"},
        //菜单前图标位置
        {"放射源"},
        {"维护操作"},
        {"Radio source"},
        {"Maintenance Menu"},
    },

    {   0x02510000,1, 1, 0, 1,//27
        {"1"},

        {""},
        {"探测器测试"},
        {""},
        {"Counter"},
    },

    {   0x02610000,1, 1, 0, 1,//28
        {"1"},

        {""},
        {"传感器测试"},
        {""},
        {"The Sensor"},
    },
    {   0x02710000,1, 1, 0, 1,//29
        {"1"},

        {""},
        {"模拟输出测试"},
        {"Channel"},
        {"Analog output"},
    },

//"校准操作"

    {   0x03100000,10, 6, 1, 1,//30
        {"1"},

        {"标片校准"},
        {"校准操作"},
        {"Demarcate"},
        {"Calibration operation"},
    },

    {   0x03200000,10, 6, 0, 1,//31
        {"2"},

        {"T1环境温度"},
        {"校准操作"},
        {"T1"},
        {"Calibration operation"},
    },
    {   0x03300000,10, 6, 0, 1,//32
        {"3"},

        {"T3采样室温度"},
        {"校准操作"},
        {"T3"},
        {"Calibration operation"},
    },

    {   0x03400000,10, 6, 0, 1,//33
        {"4"},

        {"T4探测器温度"},
        {"校准操作"},
        {"T4"},
        {"Calibration operation"},
    },

    {   0x03500000,10, 6, 0, 1,//34
        {"5"},

        {"RH1采样湿度  "},
        {"校准操作"},
        {"RH1"},
        {"Calibration operation"},
    },

    {   0x03600000,10, 6, 0, 1,//35
        {"6"},

        {"RH2环境湿度  "},
        {"校准操作"},
        {"RH2"},
        {"Calibration operation"},
    },

    {   0x03700000,10, 6, 0, 1,//36
        {"7"},

        {"P1大气压"},
        {"校准操作"},
        {"P1"},
        {"Calibration operation"},
    },
    {   0x03800000,10, 6, 1, 1,//37
        {"8"},

        {"校准膜测试"},
        {"校准操作"},
        {"Standard sample test"},
        {"Calibration operation"},
    },
    {   0x03900000,10, 6, 0, 1,//38
        {"9"},

        {"F1标准流量"},
        {"校准操作"},
        {"F2"},
        {"Calibration operation"},
    },
    {   0x03a00000,10, 6, 1, 1,//39
        {"10"},

        {"默认校准"},
        {"校准操作"},
        {"The default"},
        {"Calibration operation"},
    },
    {   0x03110000,1,1,0,0,//40
        {"1"},

        {""},
        {"标片校准"},
        {""},
        {"Standard sample"},
    },
    {   0x03810000,1,1,0,0,//41
        {"1"},

        {""},
        {"标片测试"},
        {""},
        {"Standard sample test"},
    },
    {   0x03a10000,1,1,0,0,//42
        {"1"},

        {""},
        {"默认校准"},
        {""},
        {"The default"},
    },


//
    {   0x04100000,4,4, 1, 1,//43
        {"1"},

        {"数据查看"},
        {"数据记录"},
        {"View"},
        {"Histroy Record"},

    },

    {   0x04200000,4, 4, 1, 1,//44
        {"2"},

        {"校准参数"},
        {"数据记录"},
        {"Calibration Parm"},
        {"Histroy Record"},

    },
    {   0x04300000,4, 4, 1, 1,//45
        {"3"},

        {"系统日志"},
        {"数据记录"},
        {"System log"},
        {"Histroy Record"},

    },
    {   0x04400000,4, 4, 1, 1,//46
        {"4"},

        {"擦除数据"},
        {"数据记录"},
        {"Erase dat"},
        {"Histroy Record"},

    },

    {   0x04110000,1,1,1,0,//47
        {"1"},

        {0},
        {"数据回放"},
        {0},
        {"View"},

    },
    {   0x04111000,1,1,0,0,//48
        {"1"},

        {0},
        {"数据回放"},
        {0},
        {"View"},

    },
    {   0x04210000,1,1,0,0,//49
        {"1"},

        {0},
        {"校准参数"},
        {0},
        {"Calibration Parm"},

    },

// 设置界面
    {   0x05100000,12, 6, 1, 1,//50
        {"1"},

        {"时间设置"},
        {"系统设置"},
        {"Time"},
        {"Setting"},
    },

    {   0x05200000,12, 6, 0, 1,//51
        {"2"},

        {"声音设置"},
        {"系统设置"},
        {"Sounds"},
        {"Setting"},

    },

    {   0x05300000,12, 6, 0, 1,//52
        {"3"},

        {"电机设置"},
        {"系统设置"},
        {"Print"},
        {"Setting"},
    },

    {   0x05400000,12, 6, 0, 1,//53
        {"4"},

        {"键盘锁功能"},
        {"系统设置"},
        {"KeyLock"},
        {"Setting"},
    },

    {   0x05500000,12, 6, 0, 1,//54
        {"5"},

        {"语言设置"},
        {"系统设置"},
        {"Language"},
        {"Setting"},
    },

    {   0x05600000,12, 6, 0, 1,//55
        {"6"},

        {"对比度设置"},
        {"系统设置"},
        {"Contrast"},
        {"Setting"},

    },

    {   0x05700000,12, 6, 0, 1,//56
        {"7"},

        {"滤纸剩余"},
        {"系统设置"},
        {"filter"},
        {"Setting"},

    },

    {   0x05800000,12, 6, 1, 1,//57
        {"8"},

        {"出厂设置"},
        {"系统设置"},
        {"Factory settings"},
        {"Setting"},
    },

    {   0x05900000,12, 6, 1, 1,//58
        {"9"},

        {"版本信息"},
        {"系统设置"},
        {"Version Info"},
        {"Setting"},

    },
    {   0x05a00000,12, 6, 1, 1,//59
        {"10"},

        {"开发人员项"},
        {"系统设置"},
        {"develop"},
        {"Setting"},

    },

    {   0x05a10000,6, 6, 0, 1,//60
        {"1"},

        {"滤纸容错次数"},
        {"开发人员项"},
        {"err times"},
        {"Setting"},

    },
    {   0x05a20000,6, 6, 0, 1,//61
        {"2"},

        {"仪器用途"},
        {"开发人员项"},
        {"use for"},
        {"Setting"},

    },
    {   0x05a50000,6, 6, 1, 1,//62
        {"5"},

        {"IP设置"},
        {"开发人员项"},
        {"IP set"},
        {"Setting"},
    },
    {   0x05a51000,1, 1, 0, 0,//63
        {"1"},

        {0},
        {"IP设置"},
        {0},
        {"IP set"},
    },
    {   0x05a30000,6, 6, 0, 1,//64
        {"3"},

        {"零点校正"},
        {"开发人员项"},
        {"zero set"},
        {"Setting"},
    },
    {   0x05a40000,6, 6, 0, 1,//65
        {"4"},

        {"C14设置"},
        {"开发人员项"},
        {"C14 set"},
        {"Setting"},
    },
    {   0x05a60000,6, 6, 0, 1,//66
        {"6"},

        {"通信方式"},
        {"开发人员项"},
        {"CommniStye"},
        {"Setting"},
    },

    {   0x05110000,1,1,0,0,//67
        {"1"},

        {0},
        {"时间设置"},
        {0},
        {"Time set"},
    },

    {   0x05810000,1,1,0,0,//68
        {"1"},

        {0},
        {"出厂设置"},
        {0},
        {"Factroy Settings"},
    },

    {   0x05910000,1,1,0,0,//69
        {"1"},

        {0},
        {"版本信息"},
        {0},
        {"Version Info"},
    },

    {   0x06100000,1,1,0,0,//70
        {"0"},

        {0},
        {0},
    },

    {   0x06110000,1,1,0,0,//71
        {"0"},

        {0},
        {0},
    },

    {   0x05b00000,12, 6, 1, 1,//72
        {"11"},

        {"光散设置"},
        {"系统设置"},
        {"scatter"},
        {"Setting"},
    },


    {   0x05b10000,4, 4, 0, 1,//73
        {"1"},

        {"计数周期"},
        {"光散设置"},
        {"period"},
        {"scatter"},
    },

    {   0x05b20000,4, 4, 0, 1,//74
        {"2"},

        {"PM25系数"},
        {"光散设置"},
        {"PM25 K Value"},
        {"scatter"},
    },

    {   0x05b30000,4, 4, 0, 1,//75
        {"3"},

        {"PM10系数"},
        {"光散设置"},
        {"PM10 K Value"},
        {"scatter"},
    },


    {   0x05b40000,4, 4, 0, 1,//76
        {"4"},

        {"TSP系数"},
        {"光散设置"},
        {"TSP K Value"},
        {"scatter"},
    },

    {   0x05c00000,12, 6, 0, 1,//77
        {"12"},

        {"传感器设置"},
        {"系统设置"},
        {"Sensor"},
        {"Setting"},
    },


};
