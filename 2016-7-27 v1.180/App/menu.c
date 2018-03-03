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
    {"ESC:����                  () : ȷ ��"},	 //	0
    {"ESC:���� #$ / }&: +  -    () : ȷ ��"},
    {"                          () : ȷ ��"},
    {"ESC:����                  () :��ʼ/��ͣ"},	// 3
    {"         #$ / }&: �� ��   () : �� ϸ"},
    {"         #$ / }&: �� ��   () : ȷ ��"},
    {"ESC:����                            "},	 //6
//{"X��ʾ����:        Y��ʾ����:        "},
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
//����
    {"��λ"},					   //0-	5 Inint
    {"�����Լ�........"},
    {"�����Լ�..EEPROM OK!   "},
    {"�����Լ�..EEPROM Error!"},
    {"���ڶ�ȡ����..........."},
    {"���ڸ�λ..........."},

    {"�ָ���������?"},				 //6--10
    {"����������Ũ��"},
    {"����ģʽ:"},
    {"ʱ���:"},
    {"���н׶�:"},

    {"��ֵŨ��/0.5H��"},			 //11 -15
    {"��ֵŨ��/1H��"},
    {"��ֵŨ��/3H��"},
    {"��ֵŨ��/24H��"},
    {"DD"},

    {"��"},		//16--20
    {"�ر�"},
    {"Ӣ��"},
    {"����"},
    {"����"},

    {"ʵʱ"},	 //21--	25
    {"ѹ��"},
    {"�ɿ�"},
    {"�����ɿ����,���Ժ�.."},
    {"��ȡ����ֽ���Ƭ"},

    {"�뻻�ϱ�Ƭ,����ȷ��"},		  //26--30
    {"����   s,���Ժ�.."},
    {"����ѹ�����,���Ժ�.."},
    {"���"},
    {"����ѹ�����,���Ժ�.."},

    {"�Ѿ�����   ��"},		  //31--35
    {"�޼�¼!"},
    {"�˳�"},
    {"���"},
    {"���Ժ�..."},

    {"/�����ڴ�"},		  //36--40
    {"/��������"},
    {"/ѹ    ��"},
    {"/��    ��"},
    {"/����ϵͳ"},

    {"/��    ��"},		  //41--45
    {"����ֵ:"},
    {"ƽ��ֵ:"},
    {"�׶�:��ʼ��  "},
    {"�׶�:�հ׼���"},//{"�׶�:��ֽ    "},

    {"�׶�:����    "},//46--50
    {"�׶�:��������"},
    {"�׶�:��ֽ    "},
    {"������ͣ     "},
    {"����״̬:"},

    {"����״̬:"},//51--55
    {"����״̬:"},
    {"ͨ��״̬:"},
    {"����"},
    {"������Ũ��"},

    {"����ģʽ:"},//56--60
    {"ʵʱģʽ"},
    {"��ֵŨ��: ��λ ug/m3"},
    {"����ģʽ"},
    {"����ʱ��:            "},

    {"��ʾ:��������ʼ��   :00"},//61---65
    {"�׶�:ʵʱ����"},
    {"��ȷ����ʼ�����"},
    {"���뽫ֹͣ��������"},
    {"  ���Ƚ⿪������  "},

    {"��������..."},//66---70
    {"�����ָ�Ĭ��У׼ֵ"},
    {"(PM10)"},
    {"(PM2.5)"},
    {"(TSP)"},

    {"����"},//71---75
    {"����"},
    {""},
    {""},
    {""},

//Ӣ��						   //
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
    {"/������  "},
    {"/��ֽ����"},
    {"/����ѹ  "},
    {"/��    ��"},
    {"/����ϵͳ"},//0-4

    {"/ʪ    ��"},
    {"/��  ��T1"},
    {"/��  ��T2"},
    {"/��  ��T3"},
    {"/��    ��"},//5-9

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
uint8 areaMiddleEE[4] = {90,139,150,159};//�м�ȷ��������
uint8 areaMiddle[4] = {85,116,145,139};	// �м�ȷ��������

MenuStruct const MenuFormat= {
    1,
    1,
    20,
    1,
    {0,0,239,17},
    3,
    {   {128,26,208,41},                 //�����Ӳ˵�1����(for ����)
        {128,48,208,63},                 //�����Ӳ˵�2����(for ����)
        {128,70,208,85},                 //�����Ӳ˵�3����(for ����)
        {128,92,208,107},                //�����Ӳ˵�4����(for ����)
        {128,114,208,129},               //�����Ӳ˵�5����(for ����)
        {128,136,208,151},               //�����Ӳ˵�6����(for ����)
        {0,0,0,0},                     //�����Ӳ˵�7����(for ����)
        {0,0,0,0},                     //�����Ӳ˵�8����(for ����)
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

    {   {0,25,217,42},                 //�Ӳ˵�1����(for ����)
        {0,47,217,64},                 //�Ӳ˵�2����(for ����)
        {0,69,217,86},                 //�Ӳ˵�3����(for ����)
        {0,91,217,108},                //�Ӳ˵�4����(for ����)
        {0,113,217,130},               //�Ӳ˵�5����(for ����)
        {0,135,217,152},               //�Ӳ˵�6����(for ����)
        {0,0,0,0},                     //�Ӳ˵�7����(for ����)
        {0,0,0,0},                     //�Ӳ˵�8����(for ����)
    },

    22,
    36,
    {5,31,0,0},
    24,
    28,
    {0,0,0,0},


};

const DISDATSTR  DisDat2[NUM_PARAM]=
{   //�������ͣ�				�����С��	����	С��λ��	�Ƿ���ʾ����
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
* Descriptions: ��ʾ��Ϣ
* input parameters:   type ��ʾ��Ϣ���� 0-5
						0 esc         enter
						1 esc  updown enter
						2			  enter
						3 esc  updown enter
						4 esc  enter
						5 esc  updown enter
* output parameters:��
* Returned value: ��
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
* Descriptions: ��ʾplease wait ***
* input parameters:   ��
* output parameters:��
* Returned value: ��
*********************************************************************************************************/
void DisPleaseWait(void)
{
    uint8 cs[20]= {"Please Wait..."};
    ClrDisBuf();
    DisEList168(8,20,cs);
    LcdUpdata(DisBuf);
}

void MainMenuSen()               //  ������ֵ��ʾ����1�
{
    area[0] = 0;
    area[1] = 22;
    area[2] = 239;
    area[3] = 159;
    CleanVideo(area);		/*��������*/

    WriteCode1212(12,25,"����T1:");
    WriteCode1212(12,50,"����T3:");
    WriteCode1212(12,75,"DHS-T2:");
    WriteCode1212(12,100,"PMT-T4:");
    WriteCode1212(12,125,"���V1:");

    DisDat.font = 1;
    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 1;
    DisValue2(WenDuT1,12+50,25,&DisDat," ��");
    DisValue2(WenDuT3,12+60,50,&DisDat," ��");
    DisValue2(WenDuT2,12+50,75,&DisDat," ��");
    DisValue2(TCQWenDuT4,12+50,100,&DisDat," ��");

    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 0;
    DisValue2(TV_GK/60.0,12+50,125,&DisDat," L");

    WriteCode1212(127,25,"����RH2:");
    WriteCode1212(127,50,"����RH1:");
    WriteCode1212(127,75,"����ѹP:");
    WriteCode1212(127,100,"���� C2:");
    WriteCode1212(127,125,"��� F1:");

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
    CleanVideo(area);		/*��������*/

    WriteCode1212(12,25,"����V2:");
    WriteCode1212(12,50,"��ѹv1:");
    WriteCode1212(12,75,"��ѹV2:");

    DisDat.font = 1;
    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 1;
    DisValue2(PowerV1,12+50,50,&DisDat," V");    //��ʾ5v��ѹ
    DisValue2(PowerV2,12+50,75,&DisDat," V");    //��ʾ12v

    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 0;
    DisValue2(TV_BK/60.0,12+50,25,&DisDat," L");

    WriteCode1212(127,25,"����F2:");
    WriteCode1212(127,50,"��ѹv3:");
    WriteCode1212(127,75,"��ѹV4:");

    DisDat.length = 4;
    DisDat.DisSign = 0;
    DisDat.decimal = 2;
    DisValue2(LiuLiang_GK,127+50,25,&DisDat, "L/min");

    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 1;

    DisValue2(PowerV3,127+50,50,&DisDat," V");    //��ʾ-12��ѹ
    DisValue2(PowerV4,127+50,75,&DisDat," V");    //��ʾ24v

    prompt(4);
}

void OutputStation(void)    //4-20ma���ҳ��
{
    area[0] = 0;
    area[1] = 20;
    area[2] = 239;
    area[3] = 159;
    CleanVideo(area);		/*��������*/

    WriteCode1212(12,20,       "0 ����   T1:");       //������¶�
    WriteCode1212(12,20+16*1,  "1 ���ȹ� T2:");       //���ȹ��¶�
    WriteCode1212(12,20+16*2,  "2 ���Բ� T3:");       //���Բ��¶�
    WriteCode1212(12,20+16*3,  "3 ̽���� T4:");
    WriteCode1212(12,20+16*4,  "4 ���Բ�RH1:");
    WriteCode1212(12,20+16*5,  "5 ����  RH2:");
    WriteCode1212(12,20+16*6,  "6 ʣ����ֽR:");
    WriteCode1212(12,20+16*7,  "7 ���Խ��C:");

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
    DisValue2(WenDuT1,85,20,&DisDat,  " ��");
    DisValue2(WenDuT2,85,20+16*1,&DisDat," ��");
    DisValue2(WenDuT3,85,20+16*2,&DisDat," ��");
    DisValue2(TCQWenDuT4,85,20+16*3,&DisDat," ��");

    //��ѹֵ

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

void SensorMenu()    //ά������-������ҳ��
{
    area[0] = 0;
    area[1] = 20;
    area[2] = 239;
    area[3] = 159;
    CleanVideo(area);		/*��������*/

    WriteCode1212(12,20,       "�����¶�   T1:");            //������¶�
    WriteCode1212(12,20+16*1,  "���ȹ��¶� T2:");       //���ȹ��¶�
    WriteCode1212(12,20+16*2,  "���Բ��¶� T3:");       //���Բ��¶�
    WriteCode1212(12,20+16*3,  "̽�����¶� T4:");
    WriteCode1212(12,20+16*4,  "���Բ�ʪ��RH1:");
    WriteCode1212(12,20+16*5,  "����ʪ��  RH2:");
    WriteCode1212(12,20+16*6,  "����ѹ��    P:");
    WriteCode1212(12,20+16*7,  "��������   F2:");

    DisDat.font = 1;
    DisDat.length = 4;
    DisDat.decimal = 1;
    DisDat.DisSign = 1;
    DisValue2(WenDuT1,100,20,&DisDat," ��");      //�����¶�
    DisValue2(WenDuT2,100,20+16*1,&DisDat," ��"); //���ȹ��¶�
    DisValue2(WenDuT3,100,20+16*2,&DisDat," ��"); //���Բ��¶�
    DisValue2(TCQWenDuT4,100,20+16*3,&DisDat," ��"); //���Բ��¶�

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
    //��ѹֵ

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
    i= (dat>>4);	//��ȡ��λ
    (i>9)?(*p = i + '0'+7):(*p = i + '0');
    p++;
    i=(dat&0x0F); //��ȡ��λ
    (i>9)?(*p = i + '0'+7):(*p = i + '0');
    p++;
    *p = 0;
}

void ShowEeor(uint16 errdat1)
{
    uint8 locOffset;

    int i=0,j=0,k=0;
    locOffset = (language)?(10):(0); 	// Ӣ��	����

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
    CleanVideo(area);		/*��������*/
    /*
    //��������״̬������
    01 	��ֽ
    02 	�հ׼���
    04  ����
    08 	��������
    10 �ֶ������� ���ѻ�����������
    20 ģʽ �����ں�ʵʱ��
    40 ����ģʽ
    80 DHS����
    */

    DisCList16(12 ,25,NextPrompt[50+NePrOffset]);
    DisCList16(120,25,NextPrompt[51+NePrOffset]);
    //����״̬����
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
    ���о���״̬������
    01 ���ʪ�ȴ��ڣ��趨ֵ��+10%��
    02 ���������ޣ��˴�������  //ʵʱģʽ����

    */

    //����״̬����
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
    (WarnStuation != 0)?(SET_LED2(1)):(SET_LED2(0));//���ơ�



//	WriteCode1212(40,47,"�ֹ�������ֽ/������/�ֶ�����");
//	DisCList16(12,67,"����״̬:03");
//	WriteCode1212(40,87,"�������/����������/��ֽ������");
//	WriteCode1212(40,100,"���ʪ�ȴ����趨ֵ");
    /*
    // 	����״̬ ��
    // 	 00      00     00    00    00    00   00           00
    // 	δʹ��	�Ƕȼ�	�¶�  ����	����  ѹ�� �����Ͳ���	���ݺͳ����ڴ�״̬
    // 	�����λ˵��
    // 	�����ڴ棺 01 EEPROM ����
    // 	�����Ͳ���ϵͳ��01 ��������� 02 �˴�����
    // 	ѹ����������01 ����ѹ ������Χ
    // 	��������״̬�ܺͣ� 01 ����ƫ�����2%
    // 	����״̬�� 01 T2�����������¶ȡ�10���϶�  02���ʪ�ȴ�����������Χ
    // 	�¶ȣ�  01 �¶�T1��������  02 t2��������  04 �¶�t3��������

    ���� ��16λ����6λ������ �ڴ� T3 T2 T1 ʪ�� ���ȴ������� ��ѹ �˴� ����

    */

    DisCList16(12,45,NextPrompt[52+NePrOffset]);
    DisHexDat(DisNumDat,EEORStuation1>>8);	//
    DisCList12(12+72,45,DisNumDat);

    DisHexDat(DisNumDat,(uint8)(EEORStuation1));	//
    DisCList12(12+72+12,45,DisNumDat);

    ShowEeor(EEORStuation1);


    (WarnStuation|EEORStuation1)?(SET_LED3(0)):(SET_LED3(1));




//��ʾͨ�Ŵ���ֵ
    DisCList16(12,123,NextPrompt[53+NePrOffset]);

    for(i = 0,j=0; i<16; i++)
    {
        if(CmErrorState[i]!=0)
        {
            DisHexDat(DisNumDat,CmErrorState[i]);	//	ѹ��
            DisCList12(12+80+j*14,125,DisNumDat);
            j++;
        }

    }
    if(j==0)
    {
        DisCList16(92,123,NextPrompt[54+NePrOffset]);
    }
    DisHexDat(DisNumDat,counterr);	//	ͨ�Ŵ��󣬸�λ������
    DisCList12(142,123,DisNumDat);

    DisHexDat(DisNumDat,FilterErrTimes);	//	������������
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

    CleanVideo(area);		/*��������*/
    DisCList16(12,36,NextPrompt[55+NePrOffset]);//������Ũ��
    DisCList16(12+5*16,36,NextPrompt[68+UseFor+NePrOffset]);//PM2.5 PM10 TSP


    DisDat.font = 3;
    DisDat.length = 5;
    DisDat.decimal = 1;
    DisDat.DisSign = 0;
    if(measRecord.recordResult < 0)
    {
        measRecord.recordResult = 0;
    }

#ifdef YI_TI_JI //����һ���
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
    //��ʾ������� ע����ԭ��ֻ��ʾһ���ı��������
    DisValue2(measRecord.recordResult,80,54,&DisDat,"ug/m3");
#endif

    WriteCode1212(12,111,NextPrompt[56+NePrOffset]);//����ģʽ
    switch(RunStyleFlg)
    {
    case SHISHI:
        WriteCode1212(12+9*6,111,NextPrompt[57+NePrOffset]);// ʵʱģʽ
        break;
    case ZHOUQI:
        WriteCode1212(12+9*6,111,NextPrompt[59+NePrOffset]);//����ģʽ
        WriteCode1212(12,91,NextPrompt[60+NePrOffset]);//����ʱ��
        //��ʾ����

        TimeDateKey = measRecord.recordTimeStart;
        DisTime(12+9*6,91,TimeDateKey,3 );
        DisCList16(12+9*6+5*6,91,"~");
        TimeDateKey = measRecord.recordTimeEnd;
        DisTime(12+9*6+5*6+8,91,TimeDateKey,3);


        //���ں���Ӹ�����
        if(TimeType == 1)
        {
            WriteCode1212(12+17*6,111," -- ����");// ***��ʼ��
        }

        if((TimeType == 1)&&(StopFlg == 0)&&(showFlg == 1))
        {
            WriteCode1212(12,131,NextPrompt[61+NePrOffset]);// ***��ʼ��
            TimeDateKey.hour = TimeDate.hour+1;
            aprr = 	(uint8)(TimeDateKey.hour/10+'0') ;
            WriteCode1212(12+8*12+8,131,&aprr);
            aprr = 	 ('0'+TimeDateKey.hour%10);
            WriteCode1212(12+8*12+16,131,&aprr);
        }
        //��ֵ����ʱ��Ρ�
        //��ʾ
//			DisCList16(32,130,"11.23 12:00~~11.23 13:00");
        break;
    }

//	DisCList16(12,113,"���н׶�:");

//ȡֵ





//��λ
//	DisCList16(140,36,"45.33ug/m3");


    prompt(4);

}
/*********************************************************************************************************
* Function name:   NewLcdVol
* Descriptions:      ˢ����ʾ
* input parameters:   menuid
* output parameters:��
* Returned value: 0
*********************************************************************************************************/
/*********************************************************************************************************
* Function name:   Menm0Updata
* Descriptions://Measure refurbish ��������ʾ  ʱ�� ����
* input parameters:   ��
* output parameters:��
* Returned value: ��
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
    if(0x02610000 == (MenuId&0x0fff0000))//�ڴ��������Խ���
    {
        SensorMenu();
    }
    if(0x02710000 == (MenuId&0x0fff0000))//ģ���������
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
        DisCList16((96-(4*16)),96 ,NextPrompt[42+NePrOffset]);	//����ֵ
        DisCList16((96-(4*16)),120,NextPrompt[43+NePrOffset]);	//ƽ��ֵ
        prompt(3);
        DisDat.length = 5;
        DisDat.decimal = 0;
        DisDat.DisSign = 0;
        DisDat.font = 2;
        DisValue2(CountNow,96,96, &DisDat ,"");
        CountAver =  CountTotal / CountTimes;
        DisValue2(CountAver,96,120, &DisDat ,"");
        slen = StringLen(NextPrompt[31+NePrOffset]); //   ����200s	�Ժ�
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[31+NePrOffset]);
        DisValue2((int)(CountTimes),16*4+(DIS_AREA_X-slen*8)/2,70, &DisDat ,"");
        LcdUpdata(DisBuf);
        OSMutexPost(OSLcdUsedMutex);

        OSTimeDly(100);//1��;
    }
    if(CountReadTime == 0)
    {
        TimeOverFlg = 1;
    }
}


void NewLcdVol(uint32 menuid)
{
    if((MenuId&0xffffff0f)==0x02510000)	//̽����������ʾ
    {
        showCountDat();
    }
    return ;
}

/*********************************************************************************************************
* Function name: SetHoldregToE2rom
* Descriptions:         ��hold�Ĵ�����ֵд�� e2prom
* input parameters: type д����ֽ�������
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
* Descriptions:   ��e2prom�ж����� hold�Ĵ���
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
* Descriptions: �ӼĴ����ж�ֵ ����ֵ����Ӧ����
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
* Descriptions:  ����������ֵ
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
void SetMenuList(void)
{
//	uint8 i,j,ss;
//	uint16 cd;
//	uint8 cc[] = "����ѡ��";
//	uint8 ee[] = "Select Metal";

    ;
}



/*
�˵��ṹ��
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

        {"��������"},
        {"���˵�ѡ����"},
        {"Operation information"},
        {"Main Menu"},

    },

    {   0x02000000,6, 6, 1, 4,//3//3
        {0,2},

        {"ά������"},
        {"���˵�ѡ����"},
        {"Maintenance Menu"},
        {"Main Menu"},

    },

    {   0x03000000,6, 6, 1, 4,//4//4
        {0,3},

        {"У׼����"},
        {"���˵�ѡ����"},
        {"Calibration operation"},
        {"Main Menu"},
    },

    {   0x04000000,6, 6, 1, 4,//5
        {0,4},

        {"��ʷ��¼"},
        {"���˵�ѡ����"},
        {"History Record"},
        {"Main Menu"},
    },

    {   0x05000000,6, 6, 1, 4,//6
        {0,5},

        {"ϵͳ����"},
        {"���˵�ѡ����"},
        {"Settings"},
        {"Main Menu"},
    },

    {   0x06000000,6, 6, 1, 4,//7
        {0,6},

        {"��    ��"},
        {"���˵�ѡ����"},
        {"About   "},
        {"Main Menu"},
    },

//��������
    {   0x01100000,8, 6, 0, 1,//8
        {"1"},

        {"����ģʽ"},
        {"��������"},
        {"Run Style"},
        {"Operation information"},
    },
    {   0x01200000,8, 6, 0, 1,//9
        {"2"},

        {"ʪ���ſ�"},
        {"��������"},
        {"RH Gate"},
        {"Operation information"},
    },
    {   0x01300000,8, 6, 0, 1,//10
        {"3"},

        {"�������¶�"},
        {"��������"},
        {"max heat T"},
        {"Operation information"},
    },
    {   0x01400000,8, 6, 0, 1,//11
        {"4"},

        {"��С���ȹ���"},
        {"��������"},
        {"Min Power"},
        {"Operation information"},
    },
    {   0x01500000,8, 6, 0, 1,//12
        {"5"},

        {"��������"},
        {"��������"},
        {"Flow"},
        {"Operation information"},
    },
    {   0x01600000,8, 6, 0, 1,//13
        {"6"},

        {"DHS���ȿ���"},
        {"��������"},
        {"DHS"},
        {"Operation information"},
    },

    {   0x01700000,8, 6, 1, 1,//14
        {"7"},

        {"ģʽ����"},
        {"��������"},
        {"Run Style Set"},
        {"Operation information"},
    },
//���� �¶Ȳ������ؿ���
    {   0x01800000,8,6,0,1,//15
        //�˵�ID��ͬ���˵�������һ������ʾ�˵���������û���Ӳ˵����˵���ʾ��ʽ
        {"8"},
        //�˵�ǰͼ��λ��
        {"�¶Ȳ���"},
        {"��������"},
        {"temp compensate"},
        {"Maintenance Menu"},
    },

    {   0x01710000,3, 3, 0, 1,//16
        {"1"},

        {"����ģʽ����"},
        {"��������"},
        {"top of hour"},
        {"Run Style Set"},
    },
    {   0x01720000,3, 3, 0, 1,//17
        {"2"},

        {"��������"},
        {"��������"},
        {"Cycle"},
        {"Run Style Set"},
    },

    {   0x01730000,3, 3, 0, 1,//18
        {"3"},

        {"����������"},
        {"��������"},
        {"Mass"},
        {"Run Style Set"},
    },

//ά������

    {   0x02100000,8, 6, 0, 1,//19
        {"1"},

        {"�ò���  "},
        {"ά������"},
        {"Pump"},
        {"Maintenance Menu"},

    },

    {   0x02200000,8, 6, 0, 1,//20
        {"2"},

        {"������"},
        {"ά������"},
        {"Measure room"},
        {"Maintenance Menu"},
    },

    {   0x02300000,8, 6, 0, 1,//21
        {"3"},

        {"�ƶ���ֽ��"},
        {"ά������"},
        {"Move paper"},
        {"Maintenance Menu"},
    },

    {   0x02400000,8, 6, 0, 1,//22
        {"4"},

        {"����ϵͳ"},
        {"ά������"},
        {"Heat System"},
        {"Maintenance Menu"},
    },

    {   0x02500000,8,6,1,1,//23
        {"5"},

        {"̽����̽��"},
        {"ά������"},
        {"Counter"},
        {"Maintenance Menu"},

    },
    {   0x02600000,8,6,1,1,//24
        {"6"},

        {"������"},
        {"ά������"},
        {"The sensor"},
        {"Maintenance Menu"},
    },

    {   0x02700000,8,6,1,1,//25
        {"7"},

        {"ģ���������"},
        {"ά������"},
        {"Analog output"},
        {"Maintenance Menu"},
    },
//���� ����Դ����
    {   0x02800000,8,6,0,1,//26
        //�˵�ID��ͬ���˵�������һ������ʾ�˵���������û���Ӳ˵����˵���ʾ��ʽ
        {"8"},
        //�˵�ǰͼ��λ��
        {"����Դ"},
        {"ά������"},
        {"Radio source"},
        {"Maintenance Menu"},
    },

    {   0x02510000,1, 1, 0, 1,//27
        {"1"},

        {""},
        {"̽��������"},
        {""},
        {"Counter"},
    },

    {   0x02610000,1, 1, 0, 1,//28
        {"1"},

        {""},
        {"����������"},
        {""},
        {"The Sensor"},
    },
    {   0x02710000,1, 1, 0, 1,//29
        {"1"},

        {""},
        {"ģ���������"},
        {"Channel"},
        {"Analog output"},
    },

//"У׼����"

    {   0x03100000,10, 6, 1, 1,//30
        {"1"},

        {"��ƬУ׼"},
        {"У׼����"},
        {"Demarcate"},
        {"Calibration operation"},
    },

    {   0x03200000,10, 6, 0, 1,//31
        {"2"},

        {"T1�����¶�"},
        {"У׼����"},
        {"T1"},
        {"Calibration operation"},
    },
    {   0x03300000,10, 6, 0, 1,//32
        {"3"},

        {"T3�������¶�"},
        {"У׼����"},
        {"T3"},
        {"Calibration operation"},
    },

    {   0x03400000,10, 6, 0, 1,//33
        {"4"},

        {"T4̽�����¶�"},
        {"У׼����"},
        {"T4"},
        {"Calibration operation"},
    },

    {   0x03500000,10, 6, 0, 1,//34
        {"5"},

        {"RH1����ʪ��  "},
        {"У׼����"},
        {"RH1"},
        {"Calibration operation"},
    },

    {   0x03600000,10, 6, 0, 1,//35
        {"6"},

        {"RH2����ʪ��  "},
        {"У׼����"},
        {"RH2"},
        {"Calibration operation"},
    },

    {   0x03700000,10, 6, 0, 1,//36
        {"7"},

        {"P1����ѹ"},
        {"У׼����"},
        {"P1"},
        {"Calibration operation"},
    },
    {   0x03800000,10, 6, 1, 1,//37
        {"8"},

        {"У׼Ĥ����"},
        {"У׼����"},
        {"Standard sample test"},
        {"Calibration operation"},
    },
    {   0x03900000,10, 6, 0, 1,//38
        {"9"},

        {"F1��׼����"},
        {"У׼����"},
        {"F2"},
        {"Calibration operation"},
    },
    {   0x03a00000,10, 6, 1, 1,//39
        {"10"},

        {"Ĭ��У׼"},
        {"У׼����"},
        {"The default"},
        {"Calibration operation"},
    },
    {   0x03110000,1,1,0,0,//40
        {"1"},

        {""},
        {"��ƬУ׼"},
        {""},
        {"Standard sample"},
    },
    {   0x03810000,1,1,0,0,//41
        {"1"},

        {""},
        {"��Ƭ����"},
        {""},
        {"Standard sample test"},
    },
    {   0x03a10000,1,1,0,0,//42
        {"1"},

        {""},
        {"Ĭ��У׼"},
        {""},
        {"The default"},
    },


//
    {   0x04100000,4,4, 1, 1,//43
        {"1"},

        {"���ݲ鿴"},
        {"���ݼ�¼"},
        {"View"},
        {"Histroy Record"},

    },

    {   0x04200000,4, 4, 1, 1,//44
        {"2"},

        {"У׼����"},
        {"���ݼ�¼"},
        {"Calibration Parm"},
        {"Histroy Record"},

    },
    {   0x04300000,4, 4, 1, 1,//45
        {"3"},

        {"ϵͳ��־"},
        {"���ݼ�¼"},
        {"System log"},
        {"Histroy Record"},

    },
    {   0x04400000,4, 4, 1, 1,//46
        {"4"},

        {"��������"},
        {"���ݼ�¼"},
        {"Erase dat"},
        {"Histroy Record"},

    },

    {   0x04110000,1,1,1,0,//47
        {"1"},

        {0},
        {"���ݻط�"},
        {0},
        {"View"},

    },
    {   0x04111000,1,1,0,0,//48
        {"1"},

        {0},
        {"���ݻط�"},
        {0},
        {"View"},

    },
    {   0x04210000,1,1,0,0,//49
        {"1"},

        {0},
        {"У׼����"},
        {0},
        {"Calibration Parm"},

    },

// ���ý���
    {   0x05100000,12, 6, 1, 1,//50
        {"1"},

        {"ʱ������"},
        {"ϵͳ����"},
        {"Time"},
        {"Setting"},
    },

    {   0x05200000,12, 6, 0, 1,//51
        {"2"},

        {"��������"},
        {"ϵͳ����"},
        {"Sounds"},
        {"Setting"},

    },

    {   0x05300000,12, 6, 0, 1,//52
        {"3"},

        {"�������"},
        {"ϵͳ����"},
        {"Print"},
        {"Setting"},
    },

    {   0x05400000,12, 6, 0, 1,//53
        {"4"},

        {"����������"},
        {"ϵͳ����"},
        {"KeyLock"},
        {"Setting"},
    },

    {   0x05500000,12, 6, 0, 1,//54
        {"5"},

        {"��������"},
        {"ϵͳ����"},
        {"Language"},
        {"Setting"},
    },

    {   0x05600000,12, 6, 0, 1,//55
        {"6"},

        {"�Աȶ�����"},
        {"ϵͳ����"},
        {"Contrast"},
        {"Setting"},

    },

    {   0x05700000,12, 6, 0, 1,//56
        {"7"},

        {"��ֽʣ��"},
        {"ϵͳ����"},
        {"filter"},
        {"Setting"},

    },

    {   0x05800000,12, 6, 1, 1,//57
        {"8"},

        {"��������"},
        {"ϵͳ����"},
        {"Factory settings"},
        {"Setting"},
    },

    {   0x05900000,12, 6, 1, 1,//58
        {"9"},

        {"�汾��Ϣ"},
        {"ϵͳ����"},
        {"Version Info"},
        {"Setting"},

    },
    {   0x05a00000,12, 6, 1, 1,//59
        {"10"},

        {"������Ա��"},
        {"ϵͳ����"},
        {"develop"},
        {"Setting"},

    },

    {   0x05a10000,6, 6, 0, 1,//60
        {"1"},

        {"��ֽ�ݴ����"},
        {"������Ա��"},
        {"err times"},
        {"Setting"},

    },
    {   0x05a20000,6, 6, 0, 1,//61
        {"2"},

        {"������;"},
        {"������Ա��"},
        {"use for"},
        {"Setting"},

    },
    {   0x05a50000,6, 6, 1, 1,//62
        {"5"},

        {"IP����"},
        {"������Ա��"},
        {"IP set"},
        {"Setting"},
    },
    {   0x05a51000,1, 1, 0, 0,//63
        {"1"},

        {0},
        {"IP����"},
        {0},
        {"IP set"},
    },
    {   0x05a30000,6, 6, 0, 1,//64
        {"3"},

        {"���У��"},
        {"������Ա��"},
        {"zero set"},
        {"Setting"},
    },
    {   0x05a40000,6, 6, 0, 1,//65
        {"4"},

        {"C14����"},
        {"������Ա��"},
        {"C14 set"},
        {"Setting"},
    },
    {   0x05a60000,6, 6, 0, 1,//66
        {"6"},

        {"ͨ�ŷ�ʽ"},
        {"������Ա��"},
        {"CommniStye"},
        {"Setting"},
    },

    {   0x05110000,1,1,0,0,//67
        {"1"},

        {0},
        {"ʱ������"},
        {0},
        {"Time set"},
    },

    {   0x05810000,1,1,0,0,//68
        {"1"},

        {0},
        {"��������"},
        {0},
        {"Factroy Settings"},
    },

    {   0x05910000,1,1,0,0,//69
        {"1"},

        {0},
        {"�汾��Ϣ"},
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

        {"��ɢ����"},
        {"ϵͳ����"},
        {"scatter"},
        {"Setting"},
    },


    {   0x05b10000,4, 4, 0, 1,//73
        {"1"},

        {"��������"},
        {"��ɢ����"},
        {"period"},
        {"scatter"},
    },

    {   0x05b20000,4, 4, 0, 1,//74
        {"2"},

        {"PM25ϵ��"},
        {"��ɢ����"},
        {"PM25 K Value"},
        {"scatter"},
    },

    {   0x05b30000,4, 4, 0, 1,//75
        {"3"},

        {"PM10ϵ��"},
        {"��ɢ����"},
        {"PM10 K Value"},
        {"scatter"},
    },


    {   0x05b40000,4, 4, 0, 1,//76
        {"4"},

        {"TSPϵ��"},
        {"��ɢ����"},
        {"TSP K Value"},
        {"scatter"},
    },

    {   0x05c00000,12, 6, 0, 1,//77
        {"12"},

        {"����������"},
        {"ϵͳ����"},
        {"Sensor"},
        {"Setting"},
    },


};
