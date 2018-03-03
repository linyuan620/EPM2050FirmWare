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
    {"ESC:·µ»Ø                  () : È· ¶¨"},	 //	0
    {"ESC:·µ»Ø #$ / }&: +  -    () : È· ¶¨"},
    {"                          () : È· ¶¨"},
    {"ESC:·µ»Ø                  () :¿ªÊ¼/ÔÝÍ£"},	// 3
    {"         #$ / }&: ÉÏ ÏÂ   () : Ïê Ï¸"},
    {"         #$ / }&: ÉÏ ÏÂ   () : È· ¶¨"},
    {"ESC:·µ»Ø                            "},	 //6
//{"XÏÔÊ¾±ÈÀý:        YÏÔÊ¾±ÈÀý:        "},
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
//ÖÐÎÄ
    {"µµÎ»"},					   //0-	5 Inint
    {"ÕýÔÚ×Ô¼ì........"},
    {"ÕýÔÚ×Ô¼ì..EEPROM OK!   "},
    {"ÕýÔÚ×Ô¼ì..EEPROM Error!"},
    {"ÕýÔÚ¶ÁÈ¡²ÎÊý..........."},
    {"ÕýÔÚ¸´Î»..........."},

    {"»Ö¸´³ö³§ÉèÖÃ?"},				 //6--10
    {"¿ÅÁ£ÎïÖÊÁ¿Å¨¶È"},
    {"ÔËÐÐÄ£Ê½:"},
    {"Ê±¼ä¶Î:"},
    {"ÔËÐÐ½×¶Î:"},

    {"¾ùÖµÅ¨¶È/0.5H£º"},			 //11 -15
    {"¾ùÖµÅ¨¶È/1H£º"},
    {"¾ùÖµÅ¨¶È/3H£º"},
    {"¾ùÖµÅ¨¶È/24H£º"},
    {"DD"},

    {"´ò¿ª"},		//16--20
    {"¹Ø±Õ"},
    {"Ó¢ÎÄ"},
    {"ÖÐÎÄ"},
    {"ÖÜÆÚ"},

    {"ÊµÊ±"},	 //21--	25
    {"Ñ¹½ô"},
    {"ËÉ¿ª"},
    {"ÕýÔÚËÉ¿ªµç»ú,ÇëÉÔºó.."},
    {"ÇëÈ¡ÏÂÂËÖ½»ò±êÆ¬"},

    {"Çë»»ÉÏ±êÆ¬,²¢°´È·¶¨"},		  //26--30
    {"²âÁ¿   s,ÇëÉÔºó.."},
    {"ÕýÔÚÑ¹½ôµç»ú,ÇëÉÔºó.."},
    {"Íê³É"},
    {"ÕýÔÚÑ¹½ôµç»ú,ÇëÉÔºó.."},

    {"ÒÑ¾­²âÁ¿   Ãë"},		  //31--35
    {"ÎÞ¼ÇÂ¼!"},
    {"ÍË³ö"},
    {"Íê³É"},
    {"ÇëÉÔºó..."},

    {"/Êý¾ÝÄÚ´æ"},		  //36--40
    {"/²ÉÑù²âÁ¿"},
    {"/Ñ¹    Á¦"},
    {"/Á÷    Á¿"},
    {"/¼ÓÈÈÏµÍ³"},

    {"/ÎÂ    ¶È"},		  //41--45
    {"±¾´ÎÖµ:"},
    {"Æ½¾ùÖµ:"},
    {"½×¶Î:³õÊ¼»¯  "},
    {"½×¶Î:¿Õ°×¼ÆÊý"},//{"½×¶Î:×ßÖ½    "},

    {"½×¶Î:¸»¼¯    "},//46--50
    {"½×¶Î:²âÁ¿¼ÆËã"},
    {"½×¶Î:×ßÖ½    "},
    {"ÒÇÆ÷ÔÝÍ£     "},
    {"ÔËÐÐ×´Ì¬:"},

    {"¾¯¸æ×´Ì¬:"},//51--55
    {"´íÎó×´Ì¬:"},
    {"Í¨ÐÅ×´Ì¬:"},
    {"Õý³£"},
    {"¿ÅÁ£ÎïÅ¨¶È"},

    {"ÔËÐÐÄ£Ê½:"},//56--60
    {"ÊµÊ±Ä£Ê½"},
    {"¾ùÖµÅ¨¶È: µ¥Î» ug/m3"},
    {"ÖÜÆÚÄ£Ê½"},
    {"¸»¼¯Ê±¼ä:            "},

    {"ÌáÊ¾:¸»¼¯½«¿ªÊ¼ÓÚ   :00"},//61---65
    {"½×¶Î:ÊµÊ±²âÁ¿"},
    {"°´È·¶¨¿ªÊ¼»ò½áÊø"},
    {"½øÈë½«Í£Ö¹²âÁ¿Á÷³Ì"},
    {"  ÇëÏÈ½â¿ª¼üÅÌËø  "},

    {"ÕýÔÚÔËÐÐ..."},//66---70
    {"¼´½«»Ö¸´Ä¬ÈÏÐ£×¼Öµ"},
    {"(PM10)"},
    {"(PM2.5)"},
    {"(TSP)"},

    {"Íø¿Ú"},//71---75
    {"´®¿Ú"},
    {""},
    {""},
    {""},

//Ó¢ÎÄ						   //
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
    {"/¼ÆÊýÆ÷  "},
    {"/ÂËÖ½¶ÏÁÑ"},
    {"/´óÆøÑ¹  "},
    {"/Á÷    Á¿"},
    {"/¼ÓÈÈÏµÍ³"},//0-4

    {"/Êª    ¶È"},
    {"/ÎÂ  ¶ÈT1"},
    {"/ÎÂ  ¶ÈT2"},
    {"/ÎÂ  ¶ÈT3"},
    {"/ÄÚ    ´æ"},//5-9

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
uint8 areaMiddleEE[4] = {90,139,150,159};//ÖÐ¼äÈ·¶¨¼ü´¦¿ò
uint8 areaMiddle[4] = {85,116,145,139};	// ÖÐ¼äÈ·¶¨¼ü´¦¿ò

MenuStruct const MenuFormat= {
    1,
    1,
    20,
    1,
    {0,0,239,17},
    3,
    {   {128,26,208,41},                 //ÉèÖÃ×Ó²Ëµ¥1ÇøÓò(for ·´°×)
        {128,48,208,63},                 //ÉèÖÃ×Ó²Ëµ¥2ÇøÓò(for ·´°×)
        {128,70,208,85},                 //ÉèÖÃ×Ó²Ëµ¥3ÇøÓò(for ·´°×)
        {128,92,208,107},                //ÉèÖÃ×Ó²Ëµ¥4ÇøÓò(for ·´°×)
        {128,114,208,129},               //ÉèÖÃ×Ó²Ëµ¥5ÇøÓò(for ·´°×)
        {128,136,208,151},               //ÉèÖÃ×Ó²Ëµ¥6ÇøÓò(for ·´°×)
        {0,0,0,0},                     //ÉèÖÃ×Ó²Ëµ¥7ÇøÓò(for ·´°×)
        {0,0,0,0},                     //ÉèÖÃ×Ó²Ëµ¥8ÇøÓò(for ·´°×)
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

    {   {0,25,217,42},                 //×Ó²Ëµ¥1ÇøÓò(for ·´°×)
        {0,47,217,64},                 //×Ó²Ëµ¥2ÇøÓò(for ·´°×)
        {0,69,217,86},                 //×Ó²Ëµ¥3ÇøÓò(for ·´°×)
        {0,91,217,108},                //×Ó²Ëµ¥4ÇøÓò(for ·´°×)
        {0,113,217,130},               //×Ó²Ëµ¥5ÇøÓò(for ·´°×)
        {0,135,217,152},               //×Ó²Ëµ¥6ÇøÓò(for ·´°×)
        {0,0,0,0},                     //×Ó²Ëµ¥7ÇøÓò(for ·´°×)
        {0,0,0,0},                     //×Ó²Ëµ¥8ÇøÓò(for ·´°×)
    },

    22,
    36,
    {5,31,0,0},
    24,
    28,
    {0,0,0,0},


};

const DISDATSTR  DisDat2[NUM_PARAM]=
{   //²ÎÊýÀàÐÍ£¬				×ÖÌå´óÐ¡£¬	³¤¶È	Ð¡ÊýÎ»Êý	ÊÇ·ñÏÔÊ¾Õý¸º
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
* Descriptions: ÌáÊ¾ÐÅÏ¢
* input parameters:   type ÌáÊ¾ÐÅÏ¢ÖÖÀà 0-5
						0 esc         enter
						1 esc  updown enter
						2			  enter
						3 esc  updown enter
						4 esc  enter
						5 esc  updown enter
* output parameters:ÎÞ
* Returned value: ÎÞ
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
* Descriptions: ÏÔÊ¾please wait ***
* input parameters:   ÎÞ
* output parameters:ÎÞ
* Returned value: ÎÞ
*********************************************************************************************************/
void DisPleaseWait(void)
{
    uint8 cs[20]= {"Please Wait..."};
    ClrDisBuf();
    DisEList168(8,20,cs);
    LcdUpdata(DisBuf);
}

void MainMenuSen()               //  ´«¸ÐÆ÷ÖµÏÔÊ¾½çÃæ1÷
{
    area[0] = 0;
    area[1] = 22;
    area[2] = 239;
    area[3] = 159;
    CleanVideo(area);		/*ÇøÓòÇåÆÁ*/

    WriteCode1212(12,25,"»·¾³T1:");
    WriteCode1212(12,50,"¼ì²â²ÕT3:");
    WriteCode1212(12,75,"DHS-T2:");
    WriteCode1212(12,100,"PMT-T4:");
    WriteCode1212(12,125,"±ê¿öV1:");

    DisDat.font = 1;
    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 1;
    DisValue2(WenDuT1,12+50,25,&DisDat," ¡æ");
    DisValue2(WenDuT3,12+60,50,&DisDat," ¡æ");
    DisValue2(WenDuT2,12+50,75,&DisDat," ¡æ");
    DisValue2(TCQWenDuT4,12+50,100,&DisDat," ¡æ");

    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 0;
    DisValue2(TV_GK/60.0,12+50,125,&DisDat," L");

    WriteCode1212(127,25,"»·¾³RH2:");
    WriteCode1212(127,50,"¼ì²â²ÕRH1:");
    WriteCode1212(127,75,"´óÆøÑ¹P:");
    WriteCode1212(127,100,"¹¤¿ö C2:");
    WriteCode1212(127,125,"±ê¿ö F1:");

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
    CleanVideo(area);		/*ÇøÓòÇåÆÁ*/

    WriteCode1212(12,25,"¹¤¿öV2:");
    WriteCode1212(12,50,"µçÑ¹v1:");
    WriteCode1212(12,75,"µçÑ¹V2:");

    DisDat.font = 1;
    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 1;
    DisValue2(PowerV1,12+50,50,&DisDat," V");    //ÏÔÊ¾5vµçÑ¹
    DisValue2(PowerV2,12+50,75,&DisDat," V");    //ÏÔÊ¾12v

    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 0;
    DisValue2(TV_BK/60.0,12+50,25,&DisDat," L");

    WriteCode1212(127,25,"¹¤¿öF2:");
    WriteCode1212(127,50,"µçÑ¹v3:");
    WriteCode1212(127,75,"µçÑ¹V4:");

    DisDat.length = 4;
    DisDat.DisSign = 0;
    DisDat.decimal = 2;
    DisValue2(LiuLiang_GK,127+50,25,&DisDat, "L/min");

    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 1;

    DisValue2(PowerV3,127+50,50,&DisDat," V");    //ÏÔÊ¾-12µçÑ¹
    DisValue2(PowerV4,127+50,75,&DisDat," V");    //ÏÔÊ¾24v

    prompt(4);
}

void OutputStation(void)    //4-20maÊä³öÒ³Ãæ
{
    area[0] = 0;
    area[1] = 20;
    area[2] = 239;
    area[3] = 159;
    CleanVideo(area);		/*ÇøÓòÇåÆÁ*/

    WriteCode1212(12,20,       "0 »·¾³   T1:");       //»·¾³¼ÎÂ¶È
    WriteCode1212(12,20+16*1,  "1 °éÈÈ¹Ü T2:");       //¼ÓÈÈ¹ÜÎÂ¶È
    WriteCode1212(12,20+16*2,  "2 ²âÊÔ²Õ T3:");       //²âÊÔ²ÖÎÂ¶È
    WriteCode1212(12,20+16*3,  "3 Ì½²âÆ÷ T4:");
    WriteCode1212(12,20+16*4,  "4 ²âÊÔ²ÕRH1:");
    WriteCode1212(12,20+16*5,  "5 »·¾³  RH2:");
    WriteCode1212(12,20+16*6,  "6 Ê£ÓàÂËÖ½R:");
    WriteCode1212(12,20+16*7,  "7 ²âÊÔ½á¹ûC:");

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
    DisValue2(WenDuT1,85,20,&DisDat,  " ¡æ");
    DisValue2(WenDuT2,85,20+16*1,&DisDat," ¡æ");
    DisValue2(WenDuT3,85,20+16*2,&DisDat," ¡æ");
    DisValue2(TCQWenDuT4,85,20+16*3,&DisDat," ¡æ");

    //µçÑ¹Öµ

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

void SensorMenu()    //Î¬»¤²Ù×÷-´«¸ÐÆ÷Ò³Ãæ
{
    area[0] = 0;
    area[1] = 20;
    area[2] = 239;
    area[3] = 159;
    CleanVideo(area);		/*ÇøÓòÇåÆÁ*/

    WriteCode1212(12,20,       "»·¾³ÎÂ¶È   T1:");            //»·¾³¼ÎÂ¶È
    WriteCode1212(12,20+16*1,  "°éÈÈ¹ÜÎÂ¶È T2:");       //¼ÓÈÈ¹ÜÎÂ¶È
    WriteCode1212(12,20+16*2,  "²âÊÔ²ÕÎÂ¶È T3:");       //²âÊÔ²ÖÎÂ¶È
    WriteCode1212(12,20+16*3,  "Ì½²âÆ÷ÎÂ¶È T4:");
    WriteCode1212(12,20+16*4,  "²âÊÔ²ÕÊª¶ÈRH1:");
    WriteCode1212(12,20+16*5,  "»·¾³Êª¶È  RH2:");
    WriteCode1212(12,20+16*6,  "´óÆøÑ¹Á¦    P:");
    WriteCode1212(12,20+16*7,  "¹¤¿öÁ÷Á¿   F2:");

    DisDat.font = 1;
    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 1;
    DisValue2(WenDuT1,100,20,&DisDat," ¡æ");      //»·¾³ÎÂ¶È
    DisValue2(WenDuT2,100,20+16*1,&DisDat," ¡æ"); //¼ÓÈÈ¹ÜÎÂ¶È
    DisValue2(WenDuT3,100,20+16*2,&DisDat," ¡æ"); //²âÊÔ²ÖÎÂ¶È
    DisValue2(TCQWenDuT4,100,20+16*3,&DisDat," ¡æ"); //²âÊÔ²ÖÎÂ¶È

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
    //µçÑ¹Öµ

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
    i= (dat>>4);	//ÏÈÈ¡¸ßÎ»
    (i>9)?(*p = i + '0'+7):(*p = i + '0');
    p++;
    i=(dat&0x0F); //ÔÙÈ¡µÍÎ»
    (i>9)?(*p = i + '0'+7):(*p = i + '0');
    p++;
    *p = 0;
}

void ShowEeor(uint16 errdat1)
{
    uint8 locOffset;

    int i=0,j=0,k=0;
    locOffset = (language)?(10):(0); 	// Ó¢ÎÄ	ÖÐÎÄ

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
    CleanVideo(area);		/*ÇøÓòÇåÆÁ*/
    /*
    //ËùÒÔÔËÐÐ×´Ì¬°üÀ¨£º
    01 	×ßÖ½
    02 	¿Õ°×¼ÆÊý
    04  ¸»¼¯
    08 	²âÁ¿¼ÆÊý
    10 ÊÖ¶¯²Ù×÷ÔÝ £¨ÍÑ»ú¼üÅÌÆô¶¯£©
    20 Ä£Ê½ £¨ÖÜÆÚºÍÊµÊ±£©
    40 ÕûµãÄ£Ê½
    80 DHS¿ª¹Ø
    */

    DisCList16(12 ,25,NextPrompt[50+NePrOffset]);
    DisCList16(120,25,NextPrompt[51+NePrOffset]);
    //ÔËÐÐ×´Ì¬¸üÐÂ
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
    ËùÓÐ¾¯¸æ×´Ì¬°üÀ¨£º
    01 Ïà¶ÔÊª¶È´óÓÚ£¨Éè¶¨ÖµµÄ+10%£©
    02 ³¬¹ýÖÊÁ¿ÏÞ£¬ÂË´ý½«¸ü»»  //ÊµÊ±Ä£Ê½²ÅÓÃ

    */

    //¾¯¸æ×´Ì¬¸üÐÂ
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
    (WarnStuation != 0)?(SET_LED2(1)):(SET_LED2(0));//ÁÁµÆ¡£



//	WriteCode1212(40,47,"ÊÖ¹¤¸ü»»ÂËÖ½/Áãµãµ÷Õû/ÊÖ¶¯²Ù×÷");
//	DisCList16(12,67,"¾¯¸æ×´Ì¬:03");
//	WriteCode1212(40,87,"±Ã×î´ó¹¦ÂÊ/³¬¹ýÖÊÁ¿ÏÞ/ÂËÖ½½«¸ü»»");
//	WriteCode1212(40,100,"Ïà¶ÔÊª¶È´óÓÚÉè¶¨Öµ");
    /*
    // 	´íÎó×´Ì¬ ÓÐ
    // 	 00      00     00    00    00    00   00           00
    // 	Î´Ê¹ÓÃ	×Ç¶È¼Æ	ÎÂ¶È  ¼ÓÈÈ	Á÷Á¿  Ñ¹Á¦ ²ÉÑùºÍ²âÁ¿	Êý¾ÝºÍ³ÌÐòÄÚ´æ×´Ì¬
    // 	´Ó×îµÍÎ»ËµÆð
    // 	Êý¾ÝÄÚ´æ£º 01 EEPROM ´íÎó
    // 	²ÉÑùºÍ²âÁ¿ÏµÍ³£º01 ¼ì²âÆ÷´íÎó 02 ÂË´ø¶ÏÁÑ
    // 	Ñ¹Á¦´«¸ÐÆ÷£º01 ´óÆøÑ¹ ³¬¹ý·¶Î§
    // 	Á÷Á¿µ÷½Ú×´Ì¬×ÜºÍ£º 01 µ÷½ÚÆ«²î´óÓÚ2%
    // 	¼ÓÈÈ×´Ì¬£º 01 T2³¬¹ý×î´ó¼ÓÈÈÎÂ¶È¡·10ÉãÊÏ¶È  02Ïà¶ÔÊª¶È´«¸ÐÆ÷³¬¹ý·¶Î§
    // 	ÎÂ¶È£º  01 ÎÂ¶ÈT1³¬¹ýÁ¿³Ì  02 t2³¬¹ýÁ¿³Ì  04 ÎÂ¶Èt3³¬¹ýÁ¿³Ì

    ×îÐÂ ¹²16Î»¡£¸ß6Î»±£Áô£¬ ÄÚ´æ T3 T2 T1 Êª¶È °éÈÈ´ø£¬Á÷Á¿ ÆøÑ¹ ÂË´ø ¼ÆÊý

    */

    DisCList16(12,45,NextPrompt[52+NePrOffset]);
    DisHexDat(DisNumDat,EEORStuation1>>8);	//
    DisCList12(12+72,45,DisNumDat);

    DisHexDat(DisNumDat,(uint8)(EEORStuation1));	//
    DisCList12(12+72+12,45,DisNumDat);

    ShowEeor(EEORStuation1);


    (WarnStuation|EEORStuation1)?(SET_LED3(0)):(SET_LED3(1));




//ÏÔÊ¾Í¨ÐÅ´íÎóÖµ
    DisCList16(12,123,NextPrompt[53+NePrOffset]);

    for(i = 0,j=0; i<16; i++)
    {
        if(CmErrorState[i]!=0)
        {
            DisHexDat(DisNumDat,CmErrorState[i]);	//	Ñ¹Á¦
            DisCList12(12+80+j*14,125,DisNumDat);
            j++;
        }

    }
    if(j==0)
    {
        DisCList16(92,123,NextPrompt[54+NePrOffset]);
    }
    DisHexDat(DisNumDat,counterr);	//	Í¨ÐÅ´íÎó£¬¸´Î»´ÎÊý¡£
    DisCList12(142,123,DisNumDat);

    DisHexDat(DisNumDat,FilterErrTimes);	//	¼ÆÊýÆ÷¶Á´íÎó
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

    CleanVideo(area);		/*ÇøÓòÇåÆÁ*/
    DisCList16(12,36,NextPrompt[55+NePrOffset]);//¿ÅÁ£ÎïÅ¨¶È
    DisCList16(12+5*16,36,NextPrompt[68+UseFor+NePrOffset]);//PM2.5 PM10 TSP


    DisDat.font = 3;
    DisDat.length = 5;
    DisDat.decimal = 1;
    DisDat.DisSign = 0;
    if(measRecord.recordResult < 0)
    {
        measRecord.recordResult = 0;
    }

#ifdef YI_TI_JI //²»ÊÇÒ»Ìå»ú
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
    //ÏÔÊ¾²âÁ¿½á¹û ×¢ÏúµôÔ­À´Ö»ÏÔÊ¾Ò»¸öµÄ±´Ëþ·¨½á¹û
    DisValue2(measRecord.recordResult,80,54,&DisDat,"ug/m3");
#endif

    WriteCode1212(12,111,NextPrompt[56+NePrOffset]);//ÔËÐÐÄ£Ê½
    switch(RunStyleFlg)
    {
    case SHISHI:
        WriteCode1212(12+9*6,111,NextPrompt[57+NePrOffset]);// ÊµÊ±Ä£Ê½
        break;
    case ZHOUQI:
        WriteCode1212(12+9*6,111,NextPrompt[59+NePrOffset]);//ÖÜÆÚÄ£Ê½
        WriteCode1212(12,91,NextPrompt[60+NePrOffset]);//¸»¼¯Ê±¼ä
        //ÏÔÊ¾ÈÕÆÚ

        TimeDateKey = measRecord.recordTimeStart;
        DisTime(12+9*6,91,TimeDateKey,3 );
        DisCList16(12+9*6+5*6,91,"~");
        TimeDateKey = measRecord.recordTimeEnd;
        DisTime(12+9*6+5*6+8,91,TimeDateKey,3);


        //ÖÜÆÚºóÃæ¼Ó¸öÕûµã
        if(TimeType == 1)
        {
            WriteCode1212(12+17*6,111," -- Õûµã");// ***¿ªÊ¼ÓÚ
        }

        if((TimeType == 1)&&(StopFlg == 0)&&(showFlg == 1))
        {
            WriteCode1212(12,131,NextPrompt[61+NePrOffset]);// ***¿ªÊ¼ÓÚ
            TimeDateKey.hour = TimeDate.hour+1;
            aprr = 	(uint8)(TimeDateKey.hour/10+'0') ;
            WriteCode1212(12+8*12+8,131,&aprr);
            aprr = 	 ('0'+TimeDateKey.hour%10);
            WriteCode1212(12+8*12+16,131,&aprr);
        }
        //¶ÁÖµ£¬¶ÁÊ±¼ä¶Î¡£
        //ÏÔÊ¾
//			DisCList16(32,130,"11.23 12:00~~11.23 13:00");
        break;
    }

//	DisCList16(12,113,"ÔËÐÐ½×¶Î:");

//È¡Öµ





//µ¥Î»
//	DisCList16(140,36,"45.33ug/m3");


    prompt(4);

}
/*********************************************************************************************************
* Function name:   NewLcdVol
* Descriptions:      Ë¢ÐÂÏÔÊ¾
* input parameters:   menuid
* output parameters:ÎÞ
* Returned value: 0
*********************************************************************************************************/
/*********************************************************************************************************
* Function name:   Menm0Updata
* Descriptions://Measure refurbish Ö÷½çÃæÏÔÊ¾  Ê±¼ä µçÁ¿
* input parameters:   ÎÞ
* output parameters:ÎÞ
* Returned value: ÎÞ
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
    if(0x02610000 == (MenuId&0x0fff0000))//ÔÚ´«¸ÐÆ÷²âÊÔ½çÃæ
    {
        SensorMenu();
    }
    if(0x02710000 == (MenuId&0x0fff0000))//Ä£ÄâÊä³ö½çÃæ
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
        DisCList16((96-(4*16)),96 ,NextPrompt[42+NePrOffset]);	//±¾´ÎÖµ
        DisCList16((96-(4*16)),120,NextPrompt[43+NePrOffset]);	//Æ½¾ùÖµ
        prompt(3);
        DisDat.length = 5;
        DisDat.decimal = 0;
        DisDat.DisSign = 0;
        DisDat.font = 2;
        DisValue2(CountNow,96,96, &DisDat ,"");
        CountAver =  CountTotal / CountTimes;
        DisValue2(CountAver,96,120, &DisDat ,"");
        slen = StringLen(NextPrompt[31+NePrOffset]); //   ²âÁ¿200s	ÉÔºó
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[31+NePrOffset]);
        DisValue2((int)(CountTimes),16*4+(DIS_AREA_X-slen*8)/2,70, &DisDat ,"");
        LcdUpdata(DisBuf);
        OSMutexPost(OSLcdUsedMutex);

        OSTimeDly(100);//1Ãë;
    }
    if(CountReadTime == 0)
    {
        TimeOverFlg = 1;
    }
}


void NewLcdVol(uint32 menuid)
{
    if((MenuId&0xffffff0f)==0x02510000)	//Ì½²âÆ÷²âÊÔÏÔÊ¾
    {
        showCountDat();
    }
    return ;
}

/*********************************************************************************************************
* Function name: SetHoldregToE2rom
* Descriptions:         ½«hold¼Ä´æÆ÷µÄÖµÐ´Èë e2prom
* input parameters: type Ð´ÈëµÄ×Ö½ÚÊýÀàÐÍ
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
* Descriptions:   ´Óe2promÖÐ¶ÁÊýµ½ hold¼Ä´æÆ÷
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
* Descriptions: ´Ó¼Ä´æÆ÷ÖÐ¶ÁÖµ ²¢¸³Öµ¸øÏàÓ¦²ÎÊý
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
* Descriptions:  ²âÁ¿²ÎÊý¸³Öµ
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
void SetMenuList(void)
{
//	uint8 i,j,ss;
//	uint16 cd;
//	uint8 cc[] = "½ðÊôÑ¡Ôñ";
//	uint8 ee[] = "Select Metal";

    ;
}



/*
²Ëµ¥½á¹¹Ìå
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

        {"ÔËÐÐÉèÖÃ"},
        {"Ö÷²Ëµ¥Ñ¡ÔñÏî"},
        {"Operation information"},
        {"Main Menu"},

    },

    {   0x02000000,6, 6, 1, 4,//3//3
        {0,2},

        {"Î¬»¤²Ù×÷"},
        {"Ö÷²Ëµ¥Ñ¡ÔñÏî"},
        {"Maintenance Menu"},
        {"Main Menu"},

    },

    {   0x03000000,6, 6, 1, 4,//4//4
        {0,3},

        {"Ð£×¼²Ù×÷"},
        {"Ö÷²Ëµ¥Ñ¡ÔñÏî"},
        {"Calibration operation"},
        {"Main Menu"},
    },

    {   0x04000000,6, 6, 1, 4,//5
        {0,4},

        {"ÀúÊ·¼ÇÂ¼"},
        {"Ö÷²Ëµ¥Ñ¡ÔñÏî"},
        {"History Record"},
        {"Main Menu"},
    },

    {   0x05000000,6, 6, 1, 4,//6
        {0,5},

        {"ÏµÍ³ÉèÖÃ"},
        {"Ö÷²Ëµ¥Ñ¡ÔñÏî"},
        {"Settings"},
        {"Main Menu"},
    },

    {   0x06000000,6, 6, 1, 4,//7
        {0,6},

        {"¹Ø    ÓÚ"},
        {"Ö÷²Ëµ¥Ñ¡ÔñÏî"},
        {"About   "},
        {"Main Menu"},
    },

//ÔËÐÐÉèÖÃ
    {   0x01100000,8, 6, 0, 1,//8
        {"1"},

        {"ÔËÐÐÄ£Ê½"},
        {"ÔËÐÐÉèÖÃ"},
        {"Run Style"},
        {"Operation information"},
    },
    {   0x01200000,8, 6, 0, 1,//9
        {"2"},

        {"Êª¶ÈÃÅ¿²"},
        {"ÔËÐÐÉèÖÃ"},
        {"RH Gate"},
        {"Operation information"},
    },
    {   0x01300000,8, 6, 0, 1,//10
        {"3"},

        {"×î´ó¼ÓÈÈÎÂ¶È"},
        {"ÔËÐÐÉèÖÃ"},
        {"max heat T"},
        {"Operation information"},
    },
    {   0x01400000,8, 6, 0, 1,//11
        {"4"},

        {"×îÐ¡¼ÓÈÈ¹¦ÂÊ"},
        {"ÔËÐÐÉèÖÃ"},
        {"Min Power"},
        {"Operation information"},
    },
    {   0x01500000,8, 6, 0, 1,//12
        {"5"},

        {"²ÉÑùÁ÷Á¿"},
        {"ÔËÐÐÉèÖÃ"},
        {"Flow"},
        {"Operation information"},
    },
    {   0x01600000,8, 6, 0, 1,//13
        {"6"},

        {"DHS¼ÓÈÈ¿ØÖÆ"},
        {"ÔËÐÐÉèÖÃ"},
        {"DHS"},
        {"Operation information"},
    },

    {   0x01700000,8, 6, 1, 1,//14
        {"7"},

        {"Ä£Ê½ÉèÖÃ"},
        {"ÔËÐÐÉèÖÃ"},
        {"Run Style Set"},
        {"Operation information"},
    },
//Ôö¼Ó ÎÂ¶È²¹³¥¿ª¹Ø¿ØÖÆ
    {   0x01800000,8,6,0,1,//15
        //²Ëµ¥ID£¬Í¬¼¶²Ëµ¥¸öÊý£¬Ò»ÆÁ¿ÉÏÔÊ¾²Ëµ¥¸öÊý£¬ÓÐÃ»ÓÐ×Ó²Ëµ¥£¬²Ëµ¥ÏÔÊ¾·½Ê½
        {"8"},
        //²Ëµ¥Ç°Í¼±êÎ»ÖÃ
        {"ÎÂ¶È²¹³¥"},
        {"ÔËÐÐÉèÖÃ"},
        {"temp compensate"},
        {"Maintenance Menu"},
    },

    {   0x01710000,3, 3, 0, 1,//16
        {"1"},

        {"ÕûµãÄ£Ê½ÉèÖÃ"},
        {"ÔËÐÐÉèÖÃ"},
        {"top of hour"},
        {"Run Style Set"},
    },
    {   0x01720000,3, 3, 0, 1,//17
        {"2"},

        {"²ÉÑùÖÜÆÚ"},
        {"ÔËÐÐÉèÖÃ"},
        {"Cycle"},
        {"Run Style Set"},
    },

    {   0x01730000,3, 3, 0, 1,//18
        {"3"},

        {"ÖÊÁ¿ÏÞÉèÖÃ"},
        {"ÔËÐÐÉèÖÃ"},
        {"Mass"},
        {"Run Style Set"},
    },

//Î¬»¤²Ù×÷

    {   0x02100000,8, 6, 0, 1,//19
        {"1"},

        {"±Ã²Ù×÷  "},
        {"Î¬»¤²Ù×÷"},
        {"Pump"},
        {"Maintenance Menu"},

    },

    {   0x02200000,8, 6, 0, 1,//20
        {"2"},

        {"²âÁ¿ÊÒ"},
        {"Î¬»¤²Ù×÷"},
        {"Measure room"},
        {"Maintenance Menu"},
    },

    {   0x02300000,8, 6, 0, 1,//21
        {"3"},

        {"ÒÆ¶¯ÂËÖ½´ø"},
        {"Î¬»¤²Ù×÷"},
        {"Move paper"},
        {"Maintenance Menu"},
    },

    {   0x02400000,8, 6, 0, 1,//22
        {"4"},

        {"¼ÓÈÈÏµÍ³"},
        {"Î¬»¤²Ù×÷"},
        {"Heat System"},
        {"Maintenance Menu"},
    },

    {   0x02500000,8,6,1,1,//23
        {"5"},

        {"Ì½²âÆ÷Ì½²â"},
        {"Î¬»¤²Ù×÷"},
        {"Counter"},
        {"Maintenance Menu"},

    },
    {   0x02600000,8,6,1,1,//24
        {"6"},

        {"´«¸ÐÆ÷"},
        {"Î¬»¤²Ù×÷"},
        {"The sensor"},
        {"Maintenance Menu"},
    },

    {   0x02700000,8,6,1,1,//25
        {"7"},

        {"Ä£ÄâÊä³ö²âÊÔ"},
        {"Î¬»¤²Ù×÷"},
        {"Analog output"},
        {"Maintenance Menu"},
    },
//Ôö¼Ó ·ÅÉäÔ´²Ù×÷
    {   0x02800000,8,6,0,1,//26
        //²Ëµ¥ID£¬Í¬¼¶²Ëµ¥¸öÊý£¬Ò»ÆÁ¿ÉÏÔÊ¾²Ëµ¥¸öÊý£¬ÓÐÃ»ÓÐ×Ó²Ëµ¥£¬²Ëµ¥ÏÔÊ¾·½Ê½
        {"8"},
        //²Ëµ¥Ç°Í¼±êÎ»ÖÃ
        {"·ÅÉäÔ´"},
        {"Î¬»¤²Ù×÷"},
        {"Radio source"},
        {"Maintenance Menu"},
    },

    {   0x02510000,1, 1, 0, 1,//27
        {"1"},

        {""},
        {"Ì½²âÆ÷²âÊÔ"},
        {""},
        {"Counter"},
    },

    {   0x02610000,1, 1, 0, 1,//28
        {"1"},

        {""},
        {"´«¸ÐÆ÷²âÊÔ"},
        {""},
        {"The Sensor"},
    },
    {   0x02710000,1, 1, 0, 1,//29
        {"1"},

        {""},
        {"Ä£ÄâÊä³ö²âÊÔ"},
        {"Channel"},
        {"Analog output"},
    },

//"Ð£×¼²Ù×÷"

    {   0x03100000,10, 6, 1, 1,//30
        {"1"},

        {"±êÆ¬Ð£×¼"},
        {"Ð£×¼²Ù×÷"},
        {"Demarcate"},
        {"Calibration operation"},
    },

    {   0x03200000,10, 6, 0, 1,//31
        {"2"},

        {"T1»·¾³ÎÂ¶È"},
        {"Ð£×¼²Ù×÷"},
        {"T1"},
        {"Calibration operation"},
    },
    {   0x03300000,10, 6, 0, 1,//32
        {"3"},

        {"T3²ÉÑùÊÒÎÂ¶È"},
        {"Ð£×¼²Ù×÷"},
        {"T3"},
        {"Calibration operation"},
    },

    {   0x03400000,10, 6, 0, 1,//33
        {"4"},

        {"T4Ì½²âÆ÷ÎÂ¶È"},
        {"Ð£×¼²Ù×÷"},
        {"T4"},
        {"Calibration operation"},
    },

    {   0x03500000,10, 6, 0, 1,//34
        {"5"},

        {"RH1²ÉÑùÊª¶È  "},
        {"Ð£×¼²Ù×÷"},
        {"RH1"},
        {"Calibration operation"},
    },

    {   0x03600000,10, 6, 0, 1,//35
        {"6"},

        {"RH2»·¾³Êª¶È  "},
        {"Ð£×¼²Ù×÷"},
        {"RH2"},
        {"Calibration operation"},
    },

    {   0x03700000,10, 6, 0, 1,//36
        {"7"},

        {"P1´óÆøÑ¹"},
        {"Ð£×¼²Ù×÷"},
        {"P1"},
        {"Calibration operation"},
    },
    {   0x03800000,10, 6, 1, 1,//37
        {"8"},

        {"Ð£×¼Ä¤²âÊÔ"},
        {"Ð£×¼²Ù×÷"},
        {"Standard sample test"},
        {"Calibration operation"},
    },
    {   0x03900000,10, 6, 0, 1,//38
        {"9"},

        {"F1±ê×¼Á÷Á¿"},
        {"Ð£×¼²Ù×÷"},
        {"F2"},
        {"Calibration operation"},
    },
    {   0x03a00000,10, 6, 1, 1,//39
        {"10"},

        {"Ä¬ÈÏÐ£×¼"},
        {"Ð£×¼²Ù×÷"},
        {"The default"},
        {"Calibration operation"},
    },
    {   0x03110000,1,1,0,0,//40
        {"1"},

        {""},
        {"±êÆ¬Ð£×¼"},
        {""},
        {"Standard sample"},
    },
    {   0x03810000,1,1,0,0,//41
        {"1"},

        {""},
        {"±êÆ¬²âÊÔ"},
        {""},
        {"Standard sample test"},
    },
    {   0x03a10000,1,1,0,0,//42
        {"1"},

        {""},
        {"Ä¬ÈÏÐ£×¼"},
        {""},
        {"The default"},
    },


//
    {   0x04100000,4,4, 1, 1,//43
        {"1"},

        {"Êý¾Ý²é¿´"},
        {"Êý¾Ý¼ÇÂ¼"},
        {"View"},
        {"Histroy Record"},

    },

    {   0x04200000,4, 4, 1, 1,//44
        {"2"},

        {"Ð£×¼²ÎÊý"},
        {"Êý¾Ý¼ÇÂ¼"},
        {"Calibration Parm"},
        {"Histroy Record"},

    },
    {   0x04300000,4, 4, 1, 1,//45
        {"3"},

        {"ÏµÍ³ÈÕÖ¾"},
        {"Êý¾Ý¼ÇÂ¼"},
        {"System log"},
        {"Histroy Record"},

    },
    {   0x04400000,4, 4, 1, 1,//46
        {"4"},

        {"²Á³ýÊý¾Ý"},
        {"Êý¾Ý¼ÇÂ¼"},
        {"Erase dat"},
        {"Histroy Record"},

    },

    {   0x04110000,1,1,1,0,//47
        {"1"},

        {0},
        {"Êý¾Ý»Ø·Å"},
        {0},
        {"View"},

    },
    {   0x04111000,1,1,0,0,//48
        {"1"},

        {0},
        {"Êý¾Ý»Ø·Å"},
        {0},
        {"View"},

    },
    {   0x04210000,1,1,0,0,//49
        {"1"},

        {0},
        {"Ð£×¼²ÎÊý"},
        {0},
        {"Calibration Parm"},

    },

// ÉèÖÃ½çÃæ
    {   0x05100000,12, 6, 1, 1,//50
        {"1"},

        {"Ê±¼äÉèÖÃ"},
        {"ÏµÍ³ÉèÖÃ"},
        {"Time"},
        {"Setting"},
    },

    {   0x05200000,12, 6, 0, 1,//51
        {"2"},

        {"ÉùÒôÉèÖÃ"},
        {"ÏµÍ³ÉèÖÃ"},
        {"Sounds"},
        {"Setting"},

    },

    {   0x05300000,12, 6, 0, 1,//52
        {"3"},

        {"µç»úÉèÖÃ"},
        {"ÏµÍ³ÉèÖÃ"},
        {"Print"},
        {"Setting"},
    },

    {   0x05400000,12, 6, 0, 1,//53
        {"4"},

        {"¼üÅÌËø¹¦ÄÜ"},
        {"ÏµÍ³ÉèÖÃ"},
        {"KeyLock"},
        {"Setting"},
    },

    {   0x05500000,12, 6, 0, 1,//54
        {"5"},

        {"ÓïÑÔÉèÖÃ"},
        {"ÏµÍ³ÉèÖÃ"},
        {"Language"},
        {"Setting"},
    },

    {   0x05600000,12, 6, 0, 1,//55
        {"6"},

        {"¶Ô±È¶ÈÉèÖÃ"},
        {"ÏµÍ³ÉèÖÃ"},
        {"Contrast"},
        {"Setting"},

    },

    {   0x05700000,12, 6, 0, 1,//56
        {"7"},

        {"ÂËÖ½Ê£Óà"},
        {"ÏµÍ³ÉèÖÃ"},
        {"filter"},
        {"Setting"},

    },

    {   0x05800000,12, 6, 1, 1,//57
        {"8"},

        {"³ö³§ÉèÖÃ"},
        {"ÏµÍ³ÉèÖÃ"},
        {"Factory settings"},
        {"Setting"},
    },

    {   0x05900000,12, 6, 1, 1,//58
        {"9"},

        {"°æ±¾ÐÅÏ¢"},
        {"ÏµÍ³ÉèÖÃ"},
        {"Version Info"},
        {"Setting"},

    },
    {   0x05a00000,12, 6, 1, 1,//59
        {"10"},

        {"¿ª·¢ÈËÔ±Ïî"},
        {"ÏµÍ³ÉèÖÃ"},
        {"develop"},
        {"Setting"},

    },

    {   0x05a10000,6, 6, 0, 1,//60
        {"1"},

        {"ÂËÖ½ÈÝ´í´ÎÊý"},
        {"¿ª·¢ÈËÔ±Ïî"},
        {"err times"},
        {"Setting"},

    },
    {   0x05a20000,6, 6, 0, 1,//61
        {"2"},

        {"ÒÇÆ÷ÓÃÍ¾"},
        {"¿ª·¢ÈËÔ±Ïî"},
        {"use for"},
        {"Setting"},

    },
    {   0x05a50000,6, 6, 1, 1,//62
        {"5"},

        {"IPÉèÖÃ"},
        {"¿ª·¢ÈËÔ±Ïî"},
        {"IP set"},
        {"Setting"},
    },
    {   0x05a51000,1, 1, 0, 0,//63
        {"1"},

        {0},
        {"IPÉèÖÃ"},
        {0},
        {"IP set"},
    },
    {   0x05a30000,6, 6, 0, 1,//64
        {"3"},

        {"ÁãµãÐ£Õý"},
        {"¿ª·¢ÈËÔ±Ïî"},
        {"zero set"},
        {"Setting"},
    },
    {   0x05a40000,6, 6, 0, 1,//65
        {"4"},

        {"C14ÉèÖÃ"},
        {"¿ª·¢ÈËÔ±Ïî"},
        {"C14 set"},
        {"Setting"},
    },
    {   0x05a60000,6, 6, 0, 1,//66
        {"6"},

        {"Í¨ÐÅ·½Ê½"},
        {"¿ª·¢ÈËÔ±Ïî"},
        {"CommniStye"},
        {"Setting"},
    },

    {   0x05110000,1,1,0,0,//67
        {"1"},

        {0},
        {"Ê±¼äÉèÖÃ"},
        {0},
        {"Time set"},
    },

    {   0x05810000,1,1,0,0,//68
        {"1"},

        {0},
        {"³ö³§ÉèÖÃ"},
        {0},
        {"Factroy Settings"},
    },

    {   0x05910000,1,1,0,0,//69
        {"1"},

        {0},
        {"°æ±¾ÐÅÏ¢"},
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

        {"¹âÉ¢ÉèÖÃ"},
        {"ÏµÍ³ÉèÖÃ"},
        {"scatter"},
        {"Setting"},
    },


    {   0x05b10000,4, 4, 0, 1,//73
        {"1"},

        {"¼ÆÊýÖÜÆÚ"},
        {"¹âÉ¢ÉèÖÃ"},
        {"period"},
        {"scatter"},
    },

    {   0x05b20000,4, 4, 0, 1,//74
        {"2"},

        {"PM25ÏµÊý"},
        {"¹âÉ¢ÉèÖÃ"},
        {"PM25 K Value"},
        {"scatter"},
    },

    {   0x05b30000,4, 4, 0, 1,//75
        {"3"},

        {"PM10ÏµÊý"},
        {"¹âÉ¢ÉèÖÃ"},
        {"PM10 K Value"},
        {"scatter"},
    },


    {   0x05b40000,4, 4, 0, 1,//76
        {"4"},

        {"TSPÏµÊý"},
        {"¹âÉ¢ÉèÖÃ"},
        {"TSP K Value"},
        {"scatter"},
    },

    {   0x05c00000,12, 6, 0, 1,//77
        {"12"},

        {"´«¸ÐÆ÷ÉèÖÃ"},
        {"ÏµÍ³ÉèÖÃ"},
        {"Sensor"},
        {"Setting"},
    },


};
