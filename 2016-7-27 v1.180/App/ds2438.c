/****************************************Copyright (c)****************************************************
**                            		skyray-instrument Co.,LTD.
**
**                                 http://www.skyray-instrument.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           ds2438.c
** Last modified Date:  2011-02-24
** Last Version:        V1.0
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Created By:          ������
** Created date:        2011-02-24
** Version:             V1.0
** Descriptions:        ��ص��� ���
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         xiahoonhy
** Modified date:       2011-07-01
** Version:             V1.X
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         heshunjie
** Modified date:       2011-07-01
** Version:             V2.5
** Descriptions:
**
*********************************************************************************************************/
#include "ds2438.h"
static uint8 DS2438_page0[8] = {0},DS2438_page1[8] = {0};
//DS2438_page1_Init[8] = {100,100,100,100,100,100,100,100};
volatile fp32	ResultAll[4] = {0}; //[0]:�¶�@�棬[1]:��ѹ@V��[2]������@mA��[3]��ICA@mAh
static uint8 Battery_flag;
float C_power_max = 2600.0f;

/*********************************************************************************************************
* Function name:        DelayDQDQ
* Descriptions:         ���ڵ�������������ʱ����,�ڲ����� delay(1) =1us
* input parameters:     ��ʱ������
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
volatile void DelayDQ(uint32 ulTime)
{
    uint16 i;
    for(; ulTime >0; ulTime--)
    {
        for(i =0; i< 15; i++);		 //100----7us
    }
}
/*********************************************************************************************************
* Function name:        DS2438Init
* Descriptions:         ���ڵ����߳�ʼ������Ҫ��Sets ICA, CA, EE, AD Bits active
* input parameters:     ��
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void DS2438Init(void)
{
    DS2438Rst();
    DS2438Write(SKIP_ROM);
    DS2438Write(WRITE_SP);						 //Issue Write SP 00h command
    DS2438Write(PAGE(0));
    DS2438Write(0X00);							 //Sets ICA, CA, EE, AD Bits active

}
/*********************************************************************************************************
* Function name:        DS2438Rst
* Descriptions:         ���ڵ����߸�λ
* input parameters:     ��
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void DS2438Rst(void)
{
    uint16 Outwait =900; //60us ��ʱʱ�� =60*15
    SET_DIR(1);
    SET_DQ(1);
    SET_DQ(0);
    DelayDQ(500);		//480us~960us
    SET_DQ(1);
    SET_DIR(0);
    DelayDQ(30);		//15us~60us

    while(DQ_READ)
    {
        Outwait --;			  //��ֹ���޵ȴ�
        if(0 ==Outwait)
            break;
    }

    DelayDQ(100);		//60us~240us
    SET_DIR(1);
    SET_DQ(1);
    DelayDQ(200);		//wihle��ʼ��С480us
}
/*********************************************************************************************************
* Function name:        DS2438Read
* Descriptions:         ���ڶ�����
* input parameters:     ��
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
uint8 DS2438Read(void)
{
    uint8 DS2438Read_DATA=0, Read_bit=0;
    for(; Read_bit<8; Read_bit++)
    {
        SET_DQ(0);
        SET_DIR(0);
        DelayDQ(10);
        DS2438Read_DATA = DS2438Read_DATA >> 1;	       //�������ƣ���˳���ȵͺ��
        if(DQ_READ)
        {
            DS2438Read_DATA |= 0x80;  				 	//����Ǹߣ���1����������
        }
        DelayDQ(60);

        SET_DIR(1);
        SET_DQ(1);
    }

    return 	DS2438Read_DATA;

}
/*********************************************************************************************************
* Function name:        DS2438Write
* Descriptions:         ����д8λ���ݺ���
* input parameters:     WriteData
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void DS2438Write(uint8 WriteData)
{
    uint8  Read_bit=0;
    for(; Read_bit<8; Read_bit++)
    {
        SET_DQ(0);
        DelayDQ(5);
        if(WriteData & 0x01)   	//д����
        {
            SET_DQ(1);    		//д1
        }
        else
        {
            SET_DQ(0);   		//д0
        }
        WriteData >>= 1;      	//��������1λ����д��λ

        DelayDQ(80);		 	//60~120

        SET_DQ(1);
    }
}

/*********************************************************************************************************
* Function name:        RecallMem
* Descriptions:         ��PageX�е����ݸ��Ƶ�SP�У��Թ�read_spʹ��
* input parameters:     pagex  =0~7
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void RecallMem(uint8 PageX)
{
    DS2438Rst();									 //��λ
    DS2438Write(SKIP_ROM);
    DS2438Write(RECALL_MEM);						 //����recall 01h�ڴ�ҳ����
    DS2438Write(PageX);
    DelayDQ(80);		 	//60~120
    DS2438Rst();									 //��λ
    DelayDQ(80);		 	//60~120
}
/*********************************************************************************************************
* Function name:        ReadRom
* Descriptions:         ��ĳһPageX�е����ݶ���
* input parameters:     pagex  =0~7��  *StorePage�洢����
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
uint8 ReadOnePageRom(uint8 PageX,uint8 *StorePage)
{
    uint8 i;

    RecallMem(PageX);

    DS2438Rst();
    DS2438Write(SKIP_ROM);
    DS2438Write(READ_SP);							    //��ȡ��01ҳ
    DS2438Write(PageX);

    for(i=0; i<8; i++)
    {
        *StorePage =DS2438Read();
        *StorePage++;
    }
    DS2438Rst();
    return(0);
}
/*********************************************************************************************************
* Function name:        StartConvVo
* Descriptions:         ������ѹת��
* input parameters:
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void StartConvVol(void)
{
    DS2438Rst();
    DS2438Write(SKIP_ROM);
    DS2438Write(CONVERT_VOL);								 //��ѹת��

}
/*********************************************************************************************************
* Function name:        StartConvTemp
* Descriptions:         �����¶�ת��
* input parameters:
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void StartConvTemp(void)
{
    DS2438Rst();
    DS2438Write(SKIP_ROM);
    DS2438Write(CONVERT_TEMP);								 //�¶�ת��
}

//uint8 WriteonePageRom(uint8 PageX,uint8 *StorePage)	   //û��ͨ
//{
//	uint8 i;
//
////	RecallMem(PageX);
//
//	DS2438Rst();
//	DS2438Write(SKIP_ROM);
//	DS2438Write(WRITE_SP);
//	DS2438Write(PageX);
//
//	for(i=0;i<5;i++)
//	{
//		DS2438Write(*StorePage);
//		*StorePage++;
//	}
//	DS2438Rst();
//	return(0);
//}

/*********************************************************************************************************
* Function name:        ConvAll
* Descriptions:         ���¶ȣ���ѹ��������ICAת����������ս����������ResultAll������
* input parameters:
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void ConvAll(void)
{

    StartConvVol();
    StartConvTemp();
//	WriteonePageRom(1,DS2438_page1_Init);
    ReadOnePageRom(0,DS2438_page0);
    ReadOnePageRom(1,DS2438_page1);

    ResultAll[0] =(int16)DS2438_page0[2]+(((int16)DS2438_page0[1])>>4)*0.0625;					  //�¶�
    ResultAll[1] =((uint16)DS2438_page0[4]<<8 | DS2438_page0[3])/100.0;							  //��ѹ

    //0.025
    ResultAll[2] =((int16)(DS2438_page0[6]<<8 | DS2438_page0[5]))/0.1024;	 //204.8 =4096*0.05 102.4 = 4096*0.025   //����
    ResultAll[3] =DS2438_page1[4]/0.512;	 //102.4 =2048*0.05	 51.2 = 2048*0.025								  //ICA
    //0.05
//	ResultAll[2] =((int16)(DS2438_page0[6]<<8 | DS2438_page0[5]))/0.2048;//0.1024;	 //204.8 =4096*0.05 102.4 = 4096*0.025   //����
//	ResultAll[3] =DS2438_page1[4]/1.024;//0.512;	 //102.4 =2048*0.05	 51.2 = 2048*0.025								  //ICA

}

/*********************************************************************************************************
* Function name:        signed char DisBattery(fp32 *Battery)
* Descriptions:         ��ص���LCD��ʾ
* input parameters:     *Battery ��Դ����оƬ������ֵ  Ӧ�õ���ResultAll����
						*(Battery+1)   ��ѹ
						*(Battery+2)   ����
						*(Battery+3)   ����
* output parameters:    ��
* Returned value:       0
						-1			error
*********************************************************************************************************/
signed char DisBattery(volatile fp32 *Battery)
{
    uint8 charge;
    /*���ݲɼ�����
    		��ѹ̫��
    		����̫��
    		����̫��
    */
    if( (*(Battery+1) > 20.0)
            || (abs(*(Battery+2)) > 600.0)
            || ( *(Battery+3) > 3500.0 ) )
    {
        return -1;
    }

    /*  ���
    	ͨ���������������жϳ�ŵ�
    	���ʱ�ĵ�ѹ�ȷŵ��ǵĴ�0.2v
    */
    charge = ( *(Battery+2) > 300 ) ? 1 : 0;
    if(charge) //V_power_max
    {
        switch (Battery_flag%10)
        {
        case 0:
            pic40x16(190,1,picCell[0]);
            break;
        case 2:
            pic40x16(190,1,picCell[1]);
            break;
        case 4:
            pic40x16(190,1,picCell[2]);
            break;
        case 6:
            pic40x16(190,1,picCell[3]);
            break;
        case 8:
            pic40x16(190,1,picCell[4]);
            break;
        default:
            break;
        }
        Battery_flag++;
        if(Battery_flag == 10)
        {
            Battery_flag = 0;
        }
    }

    /*  ������ʾ  ��ص�����ʾ4��3��2��1��0�����          */
    else
    {
        /*  ����е磬��û����������õ�ѹ�жϣ����ڵ�ص�һ���ã�������˵�ص������ */

        if(*(Battery+1) > V_power_max)
        {
            pic40x16(190,1,picCell[4]);
        }
        else if(*(Battery+1) > V_power_2)
        {
            pic40x16(190,1,picCell[3]);
        }
        else if(*(Battery+1) > V_power_1)
        {
            pic40x16(190,1,picCell[2]);
        }
        else if(*(Battery+1) > V_power_0)
        {
            pic40x16(190,1,picCell[1]);
        }
        else
        {
            pic40x16(190,1,picCell[0]);
        }

    }
    return 0;
}


