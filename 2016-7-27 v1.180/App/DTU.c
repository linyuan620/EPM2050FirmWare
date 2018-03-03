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
**                       �궨��
*********************************************************************************************************/
#define  RESULT    01
#define  STATE     04

/*********************************************************************************************************
**                       ��������
*********************************************************************************************************/
uint8 StartWord[2] = {'#','#'};
uint8 MN[20] = {0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30};
uint8 crc[2] = {0xff,0xff};
uint8 EndWord[2] = {'&','&'};

uint8 DTUSendBuff[249];

/*********************************************************************************************************
**                       ��������
*********************************************************************************************************/

/*********************************************************************************************************
* Function name:        SendResult()
* Descriptions:         ���ڷ��ͽ������
* input parameters:     ��ʱ������
* output parameters:    ��
* Returned value:       ��
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

    DTUSendBuff[22] = 0xff;	//�������
    DTUSendBuff[23] = 0x01;	//�����룬�������ݽ��
    DTUSendBuff[24] = 0x01;	//���������
    DTUSendBuff[25] = 0x01;	//��ǰ�������
    DTUSendBuff[26] = 0xb5;	//��������ȵ�λ	n*18 +1  n=10 ��Ŀ����
    DTUSendBuff[27] = 0x00;	//��������ȸ�λ

    DTUSendBuff[28] = 0x0a;	//������Ŀ����

    //��Ŀ1 Parg_PM25
    //����ʱ��
    //TimeDate
    DTUSendBuff[29] = 0x00;	//����
    DTUSendBuff[30] = TimeDate.second;	//��
    DTUSendBuff[31] = TimeDate.minute;	//����
    DTUSendBuff[32] = TimeDate.hour;	//ʱ
    DTUSendBuff[33] = TimeDate.date;	//��
    DTUSendBuff[34] = TimeDate.month;	//��
    DTUSendBuff[35] = (uint8)TimeDate.year;	//���λ
    DTUSendBuff[36] = (uint8)(TimeDate.year >> 8);	//���λ

    DTUSendBuff[37] = 0x23;	//������Ŀ  TSP
    DTUSendBuff[38] = 0x09;	//������Ŀ��λ
    //����ֵ��˫���ȸ�����

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

    //��Ŀ2 PM10_GS_M
    //����ʱ��
    //TimeDate
    DTUSendBuff[47] = 0x00;	//����
    DTUSendBuff[48] = TimeDate.second;	//��
    DTUSendBuff[49] = TimeDate.minute;	//����
    DTUSendBuff[50] = TimeDate.hour;	//ʱ
    DTUSendBuff[51] = TimeDate.date;	//��
    DTUSendBuff[52] = TimeDate.month;	//��
    DTUSendBuff[53] = (uint8)TimeDate.year;	//���λ
    DTUSendBuff[54] = (uint8)(TimeDate.year >> 8);	//���λ

    DTUSendBuff[55] = 0x64;	//������Ŀ  100 PM10_GS_M
    DTUSendBuff[56] = 0x09;	//������Ŀ��λ
    //����ֵ��˫���ȸ�����

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

    //��Ŀ3 PM25_GS_M
    //����ʱ��
    //TimeDate
    DTUSendBuff[65] = 0x00;	//����
    DTUSendBuff[66] = TimeDate.second;	//��
    DTUSendBuff[67] = TimeDate.minute;	//����
    DTUSendBuff[68] = TimeDate.hour;	//ʱ
    DTUSendBuff[69] = TimeDate.date;	//��
    DTUSendBuff[70] = TimeDate.month;	//��
    DTUSendBuff[71] = (uint8)TimeDate.year;	//���λ
    DTUSendBuff[72] = (uint8)(TimeDate.year >> 8);	//���λ

    DTUSendBuff[73] = 0x65;	//������Ŀ  101 PM25_GS_M
    DTUSendBuff[74] = 0x09;	//������Ŀ��λ
    //����ֵ��˫���ȸ�����

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

    //��Ŀ4 TSP_GS_M
    //����ʱ��
    //TimeDate
    DTUSendBuff[83] = 0x00;	//����
    DTUSendBuff[84] = TimeDate.second;	//��
    DTUSendBuff[85] = TimeDate.minute;	//����
    DTUSendBuff[86] = TimeDate.hour;	//ʱ
    DTUSendBuff[87] = TimeDate.date;	//��
    DTUSendBuff[88] = TimeDate.month;	//��
    DTUSendBuff[89] = (uint8)TimeDate.year;	//���λ
    DTUSendBuff[90] = (uint8)(TimeDate.year >> 8);	//���λ

    DTUSendBuff[91] = 0x66;	//������Ŀ  102 TSP_GS_M
    DTUSendBuff[92] = 0x09;	//������Ŀ��λ
    //����ֵ��˫���ȸ�����

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

    //��Ŀ5 Parg_I0
    //����ʱ��
    //TimeDate
    DTUSendBuff[101] = 0x00;	//����
    DTUSendBuff[102] = TimeDate.second;	//��
    DTUSendBuff[103] = TimeDate.minute;	//����
    DTUSendBuff[104] = TimeDate.hour;	//ʱ
    DTUSendBuff[105] = TimeDate.date;	//��
    DTUSendBuff[106] = TimeDate.month;	//��
    DTUSendBuff[107] = (uint8)TimeDate.year;	//���λ
    DTUSendBuff[108] = (uint8)(TimeDate.year >> 8);	//���λ

    DTUSendBuff[109] = 0x67;	//������Ŀ  103 Parg_I0
    DTUSendBuff[110] = 0x09;	//������Ŀ��λ
    //����ֵ��˫���ȸ�����

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

    //��Ŀ6 Parg_I
    //����ʱ��
    //TimeDate
    DTUSendBuff[119] = 0x00;	//����
    DTUSendBuff[120] = TimeDate.second;	//��
    DTUSendBuff[121] = TimeDate.minute;	//����
    DTUSendBuff[122] = TimeDate.hour;	//ʱ
    DTUSendBuff[123] = TimeDate.date;	//��
    DTUSendBuff[124] = TimeDate.month;	//��
    DTUSendBuff[125] = (uint8)TimeDate.year;	//���λ
    DTUSendBuff[126] = (uint8)(TimeDate.year >> 8);	//���λ

    DTUSendBuff[127] = 0x68;	//������Ŀ  104 Parg_I
    DTUSendBuff[128] = 0x05;	//������Ŀ��λ
    //����ֵ��˫���ȸ�����

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

    //��Ŀ7 ErrorDat
    //����ʱ��
    //TimeDate
    DTUSendBuff[137] = 0x00;	//����
    DTUSendBuff[138] = TimeDate.second;	//��
    DTUSendBuff[139] = TimeDate.minute;	//����
    DTUSendBuff[140] = TimeDate.hour;	//ʱ
    DTUSendBuff[141] = TimeDate.date;	//��
    DTUSendBuff[142] = TimeDate.month;	//��
    DTUSendBuff[143] = (uint8)TimeDate.year;	//���λ
    DTUSendBuff[144] = (uint8)(TimeDate.year >> 8);	//���λ

    DTUSendBuff[145] = 0x69;	//������Ŀ  105 ErrorDat
    DTUSendBuff[146] = 0x05;	//������Ŀ��λ
    //����ֵ��˫���ȸ�����

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

    //��Ŀ8 MeasureTs
    //����ʱ��
    //TimeDate
    DTUSendBuff[155] = 0x00;	//����
    DTUSendBuff[156] = TimeDate.second;	//��
    DTUSendBuff[157] = TimeDate.minute;	//����
    DTUSendBuff[158] = TimeDate.hour;	//ʱ
    DTUSendBuff[159] = TimeDate.date;	//��
    DTUSendBuff[160] = TimeDate.month;	//��
    DTUSendBuff[161] = (uint8)TimeDate.year;	//���λ
    DTUSendBuff[162] = (uint8)(TimeDate.year >> 8);	//���λ

    DTUSendBuff[163] = 0x6a;	//������Ŀ  106 MeasureTs
    DTUSendBuff[164] = 0x03;	//������Ŀ��λ
    //����ֵ��˫���ȸ�����

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


    //��Ŀ9 MeasureTe
    //����ʱ��
    //TimeDate
    DTUSendBuff[173] = 0x00;	//����
    DTUSendBuff[174] = TimeDate.second; //��
    DTUSendBuff[175] = TimeDate.minute; //����
    DTUSendBuff[176] = TimeDate.hour;	//ʱ
    DTUSendBuff[177] = TimeDate.date;	//��
    DTUSendBuff[178] = TimeDate.month;	//��
    DTUSendBuff[179] = (uint8)TimeDate.year;	//���λ
    DTUSendBuff[180] = (uint8)(TimeDate.year >> 8); //���λ

    DTUSendBuff[181] = 0x6b;	//������Ŀ	107 MeasureTe
    DTUSendBuff[182] = 0x03;	//������Ŀ��λ
    //����ֵ��˫���ȸ�����

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

    //��Ŀ10 Parg_v_BK
    //����ʱ��
    //TimeDate
    DTUSendBuff[191] = 0x00;	//����
    DTUSendBuff[192] = TimeDate.second; //��
    DTUSendBuff[193] = TimeDate.minute; //����
    DTUSendBuff[194] = TimeDate.hour;	//ʱ
    DTUSendBuff[195] = TimeDate.date;	//��
    DTUSendBuff[196] = TimeDate.month;	//��
    DTUSendBuff[197] = (uint8)TimeDate.year;	//���λ
    DTUSendBuff[198] = (uint8)(TimeDate.year >> 8); //���λ

    DTUSendBuff[199] = 0x6c;	//������Ŀ	108 Parg_v_BK
    DTUSendBuff[200] = 0x01;	//������Ŀ��λ
    //����ֵ��˫���ȸ�����

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

    DTUSendBuff[209] = crc[0];	//У��λ
    DTUSendBuff[210] = crc[1];	//У��λ

    DTUSendBuff[211] = EndWord[0];	//������
    DTUSendBuff[212] = EndWord[1];	//������

    UART_Send((LPC_UART_TypeDef *)LPC_UART1, DTUSendBuff, 213,BLOCKING);	//����Э��֡
}

/*********************************************************************************************************
* Function name:         SendStates()
* Descriptions:         ���ڷ���״̬����
* input parameters:     ��ʱ������
* output parameters:    ��
* Returned value:       ��
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

    DTUSendBuff[22] = 0x01;	//�������
    DTUSendBuff[23] = 0x04;	//�����룬�������ݽ��
    DTUSendBuff[24] = 0x01;	//���������
    DTUSendBuff[25] = 0x01;	//��ǰ�������

    DTUSendBuff[26] = 0x65;	//��������ȵ�λ
    DTUSendBuff[27] = 0x00;	//��������ȸ�λ

    DTUSendBuff[28] = 0x0a;	//ʵʱ״̬����    10��
    //ʵʱ״̬1�����¶�WenDuT1
    DTUSendBuff[29] = 0x01;	//״̬��
    DTUSendBuff[30] = 0x03;	//״̬��λ
    Float64ToChArray((fp64)WenDuT1,a);
    DTUSendBuff[31] = a[0];	//״̬���
    DTUSendBuff[32] = a[1];	//״̬���
    DTUSendBuff[33] = a[2];	//״̬���
    DTUSendBuff[34] = a[3];	//״̬���
    DTUSendBuff[35] = a[4];	//״̬���
    DTUSendBuff[36] = a[5];	//״̬���
    DTUSendBuff[37] = a[6];	//״̬���
    DTUSendBuff[38] = a[7];	//״̬���
    //ʵʱ״̬2	�����¶�HumiditySample
    DTUSendBuff[39] = 0x07;	//״̬��
    DTUSendBuff[40] = 0x04;	//״̬��λ
    Float64ToChArray((fp64)HumiditySample,a);
    DTUSendBuff[41] = a[0];	//״̬���
    DTUSendBuff[42] = a[1];	//״̬���
    DTUSendBuff[43] = a[2];	//״̬���
    DTUSendBuff[44] = a[3];	//״̬���
    DTUSendBuff[45] = a[4];	//״̬���
    DTUSendBuff[46] = a[5];	//״̬���
    DTUSendBuff[47] = a[6];	//״̬���
    DTUSendBuff[48] = a[7];	//״̬���
    //ʵʱ״̬3����ѹ��QiYa
    DTUSendBuff[49] = 0x04;	//״̬��
    DTUSendBuff[50] = 0x06;	//״̬��λ
    Float64ToChArray((fp64)QiYa,a);
    DTUSendBuff[51] = a[0];	//״̬���
    DTUSendBuff[52] = a[1];	//״̬���
    DTUSendBuff[53] = a[2];	//״̬���
    DTUSendBuff[54] = a[3];	//״̬���
    DTUSendBuff[55] = a[4];	//״̬���
    DTUSendBuff[56] = a[5];	//״̬���
    DTUSendBuff[57] = a[6];	//״̬���
    DTUSendBuff[58] = a[7];	//״̬���
    //ʵʱ״̬4	�������BiaoKuangL
    DTUSendBuff[59] = 0x02;	//״̬��
    DTUSendBuff[60] = 0x02;	//״̬��λ
    Float64ToChArray((fp64)LiuLiang_BK,a);
    DTUSendBuff[61] = a[0];	//״̬���
    DTUSendBuff[62] = a[1];	//״̬���
    DTUSendBuff[63] = a[2];	//״̬���
    DTUSendBuff[64] = a[3];	//״̬���
    DTUSendBuff[65] = a[4];	//״̬���
    DTUSendBuff[66] = a[5];	//״̬���
    DTUSendBuff[67] = a[6];	//״̬���
    DTUSendBuff[68] = a[7];	//״̬���
    //ʵʱ״̬5��������LiuLiangGong
    DTUSendBuff[69] = 0x03;	//״̬��
    DTUSendBuff[70] = 0x02;	//״̬��λ
    Float64ToChArray((fp64)LiuLiang_GK,a);
    DTUSendBuff[71] = a[0];	//״̬���
    DTUSendBuff[72] = a[1];	//״̬���
    DTUSendBuff[73] = a[2];	//״̬���
    DTUSendBuff[74] = a[3];	//״̬���
    DTUSendBuff[75] = a[4];	//״̬���
    DTUSendBuff[76] = a[5];	//״̬���
    DTUSendBuff[77] = a[6];	//״̬���
    DTUSendBuff[78] = a[7];	//״̬���
    //ʵʱ״̬6������TV2
    DTUSendBuff[79] = 0x05;	//״̬��
    DTUSendBuff[80] = 0x01;	//״̬��λ
    Float64ToChArray((fp64)TV_BK,a);
    DTUSendBuff[81] = a[0];	//״̬���
    DTUSendBuff[82] = a[1];	//״̬���
    DTUSendBuff[83] = a[2];	//״̬���
    DTUSendBuff[84] = a[3];	//״̬���
    DTUSendBuff[85] = a[4];	//״̬���
    DTUSendBuff[85] = a[5];	//״̬���
    DTUSendBuff[87] = a[6];	//״̬���
    DTUSendBuff[88] = a[7];	//״̬���
    //ʵʱ״̬7�������TV1
    DTUSendBuff[89] = 0x06;	//״̬��
    DTUSendBuff[90] = 0x01;	//״̬��λ
    Float64ToChArray((fp64)TV_GK,a);
    DTUSendBuff[91] = a[0];	//״̬���
    DTUSendBuff[92] = a[1];	//״̬���
    DTUSendBuff[93] = a[2];	//״̬���
    DTUSendBuff[94] = a[3];	//״̬���
    DTUSendBuff[95] = a[4];	//״̬���
    DTUSendBuff[96] = a[5];	//״̬���
    DTUSendBuff[97] = a[6];	//״̬���
    DTUSendBuff[98] = a[7];	//״̬���
    //ʵʱ״̬8�������¶�WenDuT2
    DTUSendBuff[99] = 0x08;	//״̬��
    DTUSendBuff[100] = 0x03;	//״̬��λ
    Float64ToChArray((fp64)WenDuT2,a);
    DTUSendBuff[101] = a[0];	//״̬���
    DTUSendBuff[102] = a[1];	//״̬���
    DTUSendBuff[103] = a[2];	//״̬���
    DTUSendBuff[104] = a[3];	//״̬���
    DTUSendBuff[105] = a[4];	//״̬���
    DTUSendBuff[106] = a[5];	//״̬���
    DTUSendBuff[107] = a[6];	//״̬���
    DTUSendBuff[108] = a[7];	//״̬���
    //ʵʱ״̬9�ÿ���״̬PumpFlg
    DTUSendBuff[109] = 0x0c;	//״̬��
    DTUSendBuff[110] = 0x00;	//״̬��λ
    Float64ToChArray((fp32)PumpFlg,a);
    DTUSendBuff[111] = a[0];	//״̬���
    DTUSendBuff[112] = a[1];	//״̬���
    DTUSendBuff[113] = a[2];	//״̬���
    DTUSendBuff[114] = a[3];	//״̬���
    DTUSendBuff[115] = a[4];	//״̬���
    DTUSendBuff[116] = a[5];	//״̬���
    DTUSendBuff[117] = a[6];	//״̬���
    DTUSendBuff[118] = a[7];	//״̬���
    //ʵʱ״̬10DHS״̬HeatFlg
    DTUSendBuff[119] = 0x0d;	//״̬��
    DTUSendBuff[120] = 0x00;	//״̬��λ
    Float64ToChArray((fp32)HeatFlg,a);
    DTUSendBuff[121] = a[0];	//״̬���
    DTUSendBuff[122] = a[1];	//״̬���
    DTUSendBuff[123] = a[2];	//״̬���
    DTUSendBuff[124] = a[3];	//״̬���
    DTUSendBuff[125] = a[4];	//״̬���
    DTUSendBuff[126] = a[5];	//״̬���
    DTUSendBuff[127] = a[6];	//״̬���
    DTUSendBuff[128] = a[7];	//״̬���

    DTUSendBuff[129] = crc[0];	//У��λ
    DTUSendBuff[130] = crc[1];	//У��λ

    DTUSendBuff[131] = EndWord[0];	//������
    DTUSendBuff[132] = EndWord[1];	//������

    UART_Send((LPC_UART_TypeDef *)LPC_UART1, DTUSendBuff, 133,BLOCKING);	//����Э��֡
}
