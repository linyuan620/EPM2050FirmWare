/****************************************Copyright (c)****************************************************
**                            		skyray-instrument Co.,LTD.
**
**                                 http://www.skyray-instrument.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           DTU.c
** Last modified Date:  2011-02-24
** Last Version:        V1.0
** Descriptions:
*********************************************************************************************************/
#include "DTU.h"
/*********************************************************************************************************
**                       宏定义
*********************************************************************************************************/
#define  RESULT    01
#define  STATE     04

/*********************************************************************************************************
**                       变量申明
*********************************************************************************************************/
uint8 StartWord[2] = {'#','#'};
uint8 MN[20] = {0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30};
uint8 crc[2] = {0xff,0xff};
uint8 EndWord[2] = {'&','&'};

uint8 DTUSendBuff[249];

/*********************************************************************************************************
**                       函数声明
*********************************************************************************************************/

/*********************************************************************************************************
* Function name:        SendResult()
* Descriptions:         用于发送结果数据
* input parameters:     延时的数字
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void SendResult(void)
{
    int i;
    uint8 a[8];
    fp64 TestValue;
    DTUSendBuff[0] = StartWord[0];
    DTUSendBuff[1] = StartWord[1];

    for(i=0; i<20; i++)
    {
        DTUSendBuff[2+i]=MN[i];
    }

    DTUSendBuff[22] = 0xff;	//仪器类别
    DTUSendBuff[23] = 0x01;	//命令码，发送数据结果
    DTUSendBuff[24] = 0x01;	//命令包总数
    DTUSendBuff[25] = 0x01;	//当前命令包号
    DTUSendBuff[26] = 0xb5;	//命令包长度低位	n*18 +1  n=10 项目个数
    DTUSendBuff[27] = 0x00;	//命令包长度高位

    DTUSendBuff[28] = 0x0a;	//测试项目个数

    //项目1 Parg_PM25
    //测试时间
    //TimeDate
    DTUSendBuff[29] = 0x00;	//毫秒
    DTUSendBuff[30] = TimeDate.second;	//秒
    DTUSendBuff[31] = TimeDate.minute;	//分钟
    DTUSendBuff[32] = TimeDate.hour;	//时
    DTUSendBuff[33] = TimeDate.date;	//日
    DTUSendBuff[34] = TimeDate.month;	//月
    DTUSendBuff[35] = (uint8)TimeDate.year;	//年低位
    DTUSendBuff[36] = (uint8)(TimeDate.year >> 8);	//年高位

    DTUSendBuff[37] = 0x23;	//测试项目  TSP
    DTUSendBuff[38] = 0x09;	//测试项目单位
    //测试值，双精度浮点型

    TestValue = (fp64)Parg_PM25;
    Float64ToChArray(TestValue,a);

    DTUSendBuff[39] = a[0];
    DTUSendBuff[40] = a[1];
    DTUSendBuff[41] = a[2];
    DTUSendBuff[42] = a[3];
    DTUSendBuff[43] = a[4];
    DTUSendBuff[44] = a[5];
    DTUSendBuff[45] = a[6];
    DTUSendBuff[46] = a[7];

    //项目2 PM10_GS_M
    //测试时间
    //TimeDate
    DTUSendBuff[47] = 0x00;	//毫秒
    DTUSendBuff[48] = TimeDate.second;	//秒
    DTUSendBuff[49] = TimeDate.minute;	//分钟
    DTUSendBuff[50] = TimeDate.hour;	//时
    DTUSendBuff[51] = TimeDate.date;	//日
    DTUSendBuff[52] = TimeDate.month;	//月
    DTUSendBuff[53] = (uint8)TimeDate.year;	//年低位
    DTUSendBuff[54] = (uint8)(TimeDate.year >> 8);	//年高位

    DTUSendBuff[55] = 0x64;	//测试项目  100 PM10_GS_M
    DTUSendBuff[56] = 0x09;	//测试项目单位
    //测试值，双精度浮点型

    TestValue = (fp64)PM10_GS_M;
    Float64ToChArray(TestValue,a);

    DTUSendBuff[57] = a[0];
    DTUSendBuff[58] = a[1];
    DTUSendBuff[59] = a[2];
    DTUSendBuff[60] = a[3];
    DTUSendBuff[61] = a[4];
    DTUSendBuff[62] = a[5];
    DTUSendBuff[63] = a[6];
    DTUSendBuff[64] = a[7];

    //项目3 PM25_GS_M
    //测试时间
    //TimeDate
    DTUSendBuff[65] = 0x00;	//毫秒
    DTUSendBuff[66] = TimeDate.second;	//秒
    DTUSendBuff[67] = TimeDate.minute;	//分钟
    DTUSendBuff[68] = TimeDate.hour;	//时
    DTUSendBuff[69] = TimeDate.date;	//日
    DTUSendBuff[70] = TimeDate.month;	//月
    DTUSendBuff[71] = (uint8)TimeDate.year;	//年低位
    DTUSendBuff[72] = (uint8)(TimeDate.year >> 8);	//年高位

    DTUSendBuff[73] = 0x65;	//测试项目  101 PM25_GS_M
    DTUSendBuff[74] = 0x09;	//测试项目单位
    //测试值，双精度浮点型

    TestValue = (fp64)PM25_GS_M;
    Float64ToChArray(TestValue,a);

    DTUSendBuff[75] = a[0];
    DTUSendBuff[76] = a[1];
    DTUSendBuff[77] = a[2];
    DTUSendBuff[78] = a[3];
    DTUSendBuff[79] = a[4];
    DTUSendBuff[80] = a[5];
    DTUSendBuff[81] = a[6];
    DTUSendBuff[82] = a[7];

    //项目4 TSP_GS_M
    //测试时间
    //TimeDate
    DTUSendBuff[83] = 0x00;	//毫秒
    DTUSendBuff[84] = TimeDate.second;	//秒
    DTUSendBuff[85] = TimeDate.minute;	//分钟
    DTUSendBuff[86] = TimeDate.hour;	//时
    DTUSendBuff[87] = TimeDate.date;	//日
    DTUSendBuff[88] = TimeDate.month;	//月
    DTUSendBuff[89] = (uint8)TimeDate.year;	//年低位
    DTUSendBuff[90] = (uint8)(TimeDate.year >> 8);	//年高位

    DTUSendBuff[91] = 0x66;	//测试项目  102 TSP_GS_M
    DTUSendBuff[92] = 0x09;	//测试项目单位
    //测试值，双精度浮点型

    TestValue = (fp64)TSP_GS_M;
    Float64ToChArray(TestValue,a);

    DTUSendBuff[93] = a[0];
    DTUSendBuff[94] = a[1];
    DTUSendBuff[95] = a[2];
    DTUSendBuff[96] = a[3];
    DTUSendBuff[97] = a[4];
    DTUSendBuff[98] = a[5];
    DTUSendBuff[99] = a[6];
    DTUSendBuff[100] = a[7];

    //项目5 Parg_I0
    //测试时间
    //TimeDate
    DTUSendBuff[101] = 0x00;	//毫秒
    DTUSendBuff[102] = TimeDate.second;	//秒
    DTUSendBuff[103] = TimeDate.minute;	//分钟
    DTUSendBuff[104] = TimeDate.hour;	//时
    DTUSendBuff[105] = TimeDate.date;	//日
    DTUSendBuff[106] = TimeDate.month;	//月
    DTUSendBuff[107] = (uint8)TimeDate.year;	//年低位
    DTUSendBuff[108] = (uint8)(TimeDate.year >> 8);	//年高位

    DTUSendBuff[109] = 0x67;	//测试项目  103 Parg_I0
    DTUSendBuff[110] = 0x09;	//测试项目单位
    //测试值，双精度浮点型

    TestValue = (fp64)Parg_I0_Upload;
    Float64ToChArray(TestValue,a);

    DTUSendBuff[111] = a[0];
    DTUSendBuff[112] = a[1];
    DTUSendBuff[113] = a[2];
    DTUSendBuff[114] = a[3];
    DTUSendBuff[115] = a[4];
    DTUSendBuff[116] = a[5];
    DTUSendBuff[117] = a[6];
    DTUSendBuff[118] = a[7];

    //项目6 Parg_I
    //测试时间
    //TimeDate
    DTUSendBuff[119] = 0x00;	//毫秒
    DTUSendBuff[120] = TimeDate.second;	//秒
    DTUSendBuff[121] = TimeDate.minute;	//分钟
    DTUSendBuff[122] = TimeDate.hour;	//时
    DTUSendBuff[123] = TimeDate.date;	//日
    DTUSendBuff[124] = TimeDate.month;	//月
    DTUSendBuff[125] = (uint8)TimeDate.year;	//年低位
    DTUSendBuff[126] = (uint8)(TimeDate.year >> 8);	//年高位

    DTUSendBuff[127] = 0x68;	//测试项目  104 Parg_I
    DTUSendBuff[128] = 0x05;	//测试项目单位
    //测试值，双精度浮点型

    TestValue = (fp64)Parg_I;
    Float64ToChArray(TestValue,a);

    DTUSendBuff[129] = a[0];
    DTUSendBuff[130] = a[1];
    DTUSendBuff[131] = a[2];
    DTUSendBuff[132] = a[3];
    DTUSendBuff[133] = a[4];
    DTUSendBuff[134] = a[5];
    DTUSendBuff[135] = a[6];
    DTUSendBuff[136] = a[7];

    //项目7 ErrorDat
    //测试时间
    //TimeDate
    DTUSendBuff[137] = 0x00;	//毫秒
    DTUSendBuff[138] = TimeDate.second;	//秒
    DTUSendBuff[139] = TimeDate.minute;	//分钟
    DTUSendBuff[140] = TimeDate.hour;	//时
    DTUSendBuff[141] = TimeDate.date;	//日
    DTUSendBuff[142] = TimeDate.month;	//月
    DTUSendBuff[143] = (uint8)TimeDate.year;	//年低位
    DTUSendBuff[144] = (uint8)(TimeDate.year >> 8);	//年高位

    DTUSendBuff[145] = 0x69;	//测试项目  105 ErrorDat
    DTUSendBuff[146] = 0x05;	//测试项目单位
    //测试值，双精度浮点型

    TestValue = (fp64)ErrorDat;
    Float64ToChArray(TestValue,a);

    DTUSendBuff[147] = a[0];
    DTUSendBuff[148] = a[1];
    DTUSendBuff[149] = a[2];
    DTUSendBuff[150] = a[3];
    DTUSendBuff[151] = a[4];
    DTUSendBuff[152] = a[5];
    DTUSendBuff[153] = a[6];
    DTUSendBuff[154] = a[7];

    //项目8 MeasureTs
    //测试时间
    //TimeDate
    DTUSendBuff[155] = 0x00;	//毫秒
    DTUSendBuff[156] = TimeDate.second;	//秒
    DTUSendBuff[157] = TimeDate.minute;	//分钟
    DTUSendBuff[158] = TimeDate.hour;	//时
    DTUSendBuff[159] = TimeDate.date;	//日
    DTUSendBuff[160] = TimeDate.month;	//月
    DTUSendBuff[161] = (uint8)TimeDate.year;	//年低位
    DTUSendBuff[162] = (uint8)(TimeDate.year >> 8);	//年高位

    DTUSendBuff[163] = 0x6a;	//测试项目  106 MeasureTs
    DTUSendBuff[164] = 0x03;	//测试项目单位
    //测试值，双精度浮点型

    TestValue = (fp64)MeasureTs;
    Float64ToChArray(TestValue,a);

    DTUSendBuff[165] = a[0];
    DTUSendBuff[166] = a[1];
    DTUSendBuff[167] = a[2];
    DTUSendBuff[168] = a[3];
    DTUSendBuff[169] = a[4];
    DTUSendBuff[170] = a[5];
    DTUSendBuff[171] = a[6];
    DTUSendBuff[172] = a[7];


    //项目9 MeasureTe
    //测试时间
    //TimeDate
    DTUSendBuff[173] = 0x00;	//毫秒
    DTUSendBuff[174] = TimeDate.second; //秒
    DTUSendBuff[175] = TimeDate.minute; //分钟
    DTUSendBuff[176] = TimeDate.hour;	//时
    DTUSendBuff[177] = TimeDate.date;	//日
    DTUSendBuff[178] = TimeDate.month;	//月
    DTUSendBuff[179] = (uint8)TimeDate.year;	//年低位
    DTUSendBuff[180] = (uint8)(TimeDate.year >> 8); //年高位

    DTUSendBuff[181] = 0x6b;	//测试项目	107 MeasureTe
    DTUSendBuff[182] = 0x03;	//测试项目单位
    //测试值，双精度浮点型

    TestValue = (fp64)MeasureTe;
    Float64ToChArray(TestValue,a);

    DTUSendBuff[183] = a[0];
    DTUSendBuff[184] = a[1];
    DTUSendBuff[185] = a[2];
    DTUSendBuff[186] = a[3];
    DTUSendBuff[187] = a[4];
    DTUSendBuff[188] = a[5];
    DTUSendBuff[189] = a[6];
    DTUSendBuff[190] = a[7];

    //项目10 Parg_v_BK
    //测试时间
    //TimeDate
    DTUSendBuff[191] = 0x00;	//毫秒
    DTUSendBuff[192] = TimeDate.second; //秒
    DTUSendBuff[193] = TimeDate.minute; //分钟
    DTUSendBuff[194] = TimeDate.hour;	//时
    DTUSendBuff[195] = TimeDate.date;	//日
    DTUSendBuff[196] = TimeDate.month;	//月
    DTUSendBuff[197] = (uint8)TimeDate.year;	//年低位
    DTUSendBuff[198] = (uint8)(TimeDate.year >> 8); //年高位

    DTUSendBuff[199] = 0x6c;	//测试项目	108 Parg_v_BK
    DTUSendBuff[200] = 0x01;	//测试项目单位
    //测试值，双精度浮点型

    TestValue = (fp64)Parg_v_BK;
    Float64ToChArray(TestValue,a);

    DTUSendBuff[201] = a[0];
    DTUSendBuff[202] = a[1];
    DTUSendBuff[203] = a[2];
    DTUSendBuff[204] = a[3];
    DTUSendBuff[205] = a[4];
    DTUSendBuff[206] = a[5];
    DTUSendBuff[207] = a[6];
    DTUSendBuff[208] = a[7];

    DTUSendBuff[209] = crc[0];	//校验位
    DTUSendBuff[210] = crc[1];	//校验位

    DTUSendBuff[211] = EndWord[0];	//结束符
    DTUSendBuff[212] = EndWord[1];	//结束符

    UART_Send((LPC_UART_TypeDef *)LPC_UART1, DTUSendBuff, 213,BLOCKING);	//发送协议帧
}

/*********************************************************************************************************
* Function name:         SendStates()
* Descriptions:         用于发送状态数据
* input parameters:     延时的数字
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void SendStates(void)
{
    int i;
    uint8 a[8];
    DTUSendBuff[0] = StartWord[0];
    DTUSendBuff[1] = StartWord[1];

    for(i=0; i<20; i++)
    {
        DTUSendBuff[2+i]=MN[i];
    }

    DTUSendBuff[22] = 0x01;	//仪器类别
    DTUSendBuff[23] = 0x04;	//命令码，发送数据结果
    DTUSendBuff[24] = 0x01;	//命令包总数
    DTUSendBuff[25] = 0x01;	//当前命令包号

    DTUSendBuff[26] = 0x65;	//命令包长度低位
    DTUSendBuff[27] = 0x00;	//命令包长度高位

    DTUSendBuff[28] = 0x0a;	//实时状态个数    10个
    //实时状态1环境温度WenDuT1
    DTUSendBuff[29] = 0x01;	//状态相
    DTUSendBuff[30] = 0x03;	//状态单位
    Float64ToChArray((fp64)WenDuT1,a);
    DTUSendBuff[31] = a[0];	//状态结果
    DTUSendBuff[32] = a[1];	//状态结果
    DTUSendBuff[33] = a[2];	//状态结果
    DTUSendBuff[34] = a[3];	//状态结果
    DTUSendBuff[35] = a[4];	//状态结果
    DTUSendBuff[36] = a[5];	//状态结果
    DTUSendBuff[37] = a[6];	//状态结果
    DTUSendBuff[38] = a[7];	//状态结果
    //实时状态2	气体温度HumiditySample
    DTUSendBuff[39] = 0x07;	//状态相
    DTUSendBuff[40] = 0x04;	//状态单位
    Float64ToChArray((fp64)HumiditySample,a);
    DTUSendBuff[41] = a[0];	//状态结果
    DTUSendBuff[42] = a[1];	//状态结果
    DTUSendBuff[43] = a[2];	//状态结果
    DTUSendBuff[44] = a[3];	//状态结果
    DTUSendBuff[45] = a[4];	//状态结果
    DTUSendBuff[46] = a[5];	//状态结果
    DTUSendBuff[47] = a[6];	//状态结果
    DTUSendBuff[48] = a[7];	//状态结果
    //实时状态3大气压力QiYa
    DTUSendBuff[49] = 0x04;	//状态相
    DTUSendBuff[50] = 0x06;	//状态单位
    Float64ToChArray((fp64)QiYa,a);
    DTUSendBuff[51] = a[0];	//状态结果
    DTUSendBuff[52] = a[1];	//状态结果
    DTUSendBuff[53] = a[2];	//状态结果
    DTUSendBuff[54] = a[3];	//状态结果
    DTUSendBuff[55] = a[4];	//状态结果
    DTUSendBuff[56] = a[5];	//状态结果
    DTUSendBuff[57] = a[6];	//状态结果
    DTUSendBuff[58] = a[7];	//状态结果
    //实时状态4	标况流量BiaoKuangL
    DTUSendBuff[59] = 0x02;	//状态相
    DTUSendBuff[60] = 0x02;	//状态单位
    Float64ToChArray((fp64)LiuLiang_BK,a);
    DTUSendBuff[61] = a[0];	//状态结果
    DTUSendBuff[62] = a[1];	//状态结果
    DTUSendBuff[63] = a[2];	//状态结果
    DTUSendBuff[64] = a[3];	//状态结果
    DTUSendBuff[65] = a[4];	//状态结果
    DTUSendBuff[66] = a[5];	//状态结果
    DTUSendBuff[67] = a[6];	//状态结果
    DTUSendBuff[68] = a[7];	//状态结果
    //实时状态5工况流量LiuLiangGong
    DTUSendBuff[69] = 0x03;	//状态相
    DTUSendBuff[70] = 0x02;	//状态单位
    Float64ToChArray((fp64)LiuLiang_GK,a);
    DTUSendBuff[71] = a[0];	//状态结果
    DTUSendBuff[72] = a[1];	//状态结果
    DTUSendBuff[73] = a[2];	//状态结果
    DTUSendBuff[74] = a[3];	//状态结果
    DTUSendBuff[75] = a[4];	//状态结果
    DTUSendBuff[76] = a[5];	//状态结果
    DTUSendBuff[77] = a[6];	//状态结果
    DTUSendBuff[78] = a[7];	//状态结果
    //实时状态6标况体积TV2
    DTUSendBuff[79] = 0x05;	//状态相
    DTUSendBuff[80] = 0x01;	//状态单位
    Float64ToChArray((fp64)TV_BK,a);
    DTUSendBuff[81] = a[0];	//状态结果
    DTUSendBuff[82] = a[1];	//状态结果
    DTUSendBuff[83] = a[2];	//状态结果
    DTUSendBuff[84] = a[3];	//状态结果
    DTUSendBuff[85] = a[4];	//状态结果
    DTUSendBuff[85] = a[5];	//状态结果
    DTUSendBuff[87] = a[6];	//状态结果
    DTUSendBuff[88] = a[7];	//状态结果
    //实时状态7工况体积TV1
    DTUSendBuff[89] = 0x06;	//状态相
    DTUSendBuff[90] = 0x01;	//状态单位
    Float64ToChArray((fp64)TV_GK,a);
    DTUSendBuff[91] = a[0];	//状态结果
    DTUSendBuff[92] = a[1];	//状态结果
    DTUSendBuff[93] = a[2];	//状态结果
    DTUSendBuff[94] = a[3];	//状态结果
    DTUSendBuff[95] = a[4];	//状态结果
    DTUSendBuff[96] = a[5];	//状态结果
    DTUSendBuff[97] = a[6];	//状态结果
    DTUSendBuff[98] = a[7];	//状态结果
    //实时状态8采样管温度WenDuT2
    DTUSendBuff[99] = 0x08;	//状态相
    DTUSendBuff[100] = 0x03;	//状态单位
    Float64ToChArray((fp64)WenDuT2,a);
    DTUSendBuff[101] = a[0];	//状态结果
    DTUSendBuff[102] = a[1];	//状态结果
    DTUSendBuff[103] = a[2];	//状态结果
    DTUSendBuff[104] = a[3];	//状态结果
    DTUSendBuff[105] = a[4];	//状态结果
    DTUSendBuff[106] = a[5];	//状态结果
    DTUSendBuff[107] = a[6];	//状态结果
    DTUSendBuff[108] = a[7];	//状态结果
    //实时状态9泵开关状态PumpFlg
    DTUSendBuff[109] = 0x0c;	//状态相
    DTUSendBuff[110] = 0x00;	//状态单位
    Float64ToChArray((fp32)PumpFlg,a);
    DTUSendBuff[111] = a[0];	//状态结果
    DTUSendBuff[112] = a[1];	//状态结果
    DTUSendBuff[113] = a[2];	//状态结果
    DTUSendBuff[114] = a[3];	//状态结果
    DTUSendBuff[115] = a[4];	//状态结果
    DTUSendBuff[116] = a[5];	//状态结果
    DTUSendBuff[117] = a[6];	//状态结果
    DTUSendBuff[118] = a[7];	//状态结果
    //实时状态10DHS状态HeatFlg
    DTUSendBuff[119] = 0x0d;	//状态相
    DTUSendBuff[120] = 0x00;	//状态单位
    Float64ToChArray((fp32)HeatFlg,a);
    DTUSendBuff[121] = a[0];	//状态结果
    DTUSendBuff[122] = a[1];	//状态结果
    DTUSendBuff[123] = a[2];	//状态结果
    DTUSendBuff[124] = a[3];	//状态结果
    DTUSendBuff[125] = a[4];	//状态结果
    DTUSendBuff[126] = a[5];	//状态结果
    DTUSendBuff[127] = a[6];	//状态结果
    DTUSendBuff[128] = a[7];	//状态结果

    DTUSendBuff[129] = crc[0];	//校验位
    DTUSendBuff[130] = crc[1];	//校验位

    DTUSendBuff[131] = EndWord[0];	//结束符
    DTUSendBuff[132] = EndWord[1];	//结束符

    UART_Send((LPC_UART_TypeDef *)LPC_UART1, DTUSendBuff, 133,BLOCKING);	//发送协议帧
}
