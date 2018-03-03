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
** Created By:          方卫龙
** Created date:        2011-02-24
** Version:             V1.0
** Descriptions:        电池电量 相关
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
volatile fp32	ResultAll[4] = {0}; //[0]:温度@℃，[1]:电压@V，[2]：电流@mA，[3]：ICA@mAh
static uint8 Battery_flag;
float C_power_max = 2600.0f;

/*********************************************************************************************************
* Function name:        DelayDQDQ
* Descriptions:         用于单总线驱动的延时函数,内部调用 delay(1) =1us
* input parameters:     延时的数字
* output parameters:    无
* Returned value:       无
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
* Descriptions:         用于单总线初始化，主要是Sets ICA, CA, EE, AD Bits active
* input parameters:     无
* output parameters:    无
* Returned value:       无
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
* Descriptions:         用于单总线复位
* input parameters:     无
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void DS2438Rst(void)
{
    uint16 Outwait =900; //60us 延时时间 =60*15
    SET_DIR(1);
    SET_DQ(1);
    SET_DQ(0);
    DelayDQ(500);		//480us~960us
    SET_DQ(1);
    SET_DIR(0);
    DelayDQ(30);		//15us~60us

    while(DQ_READ)
    {
        Outwait --;			  //防止无限等待
        if(0 ==Outwait)
            break;
    }

    DelayDQ(100);		//60us~240us
    SET_DIR(1);
    SET_DQ(1);
    DelayDQ(200);		//wihle开始最小480us
}
/*********************************************************************************************************
* Function name:        DS2438Read
* Descriptions:         用于读函数
* input parameters:     无
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
uint8 DS2438Read(void)
{
    uint8 DS2438Read_DATA=0, Read_bit=0;
    for(; Read_bit<8; Read_bit++)
    {
        SET_DQ(0);
        SET_DIR(0);
        DelayDQ(10);
        DS2438Read_DATA = DS2438Read_DATA >> 1;	       //数据右移，读顺序：先低后高
        if(DQ_READ)
        {
            DS2438Read_DATA |= 0x80;  				 	//如果是高，置1，右移数据
        }
        DelayDQ(60);

        SET_DIR(1);
        SET_DQ(1);
    }

    return 	DS2438Read_DATA;

}
/*********************************************************************************************************
* Function name:        DS2438Write
* Descriptions:         用于写8位数据函数
* input parameters:     WriteData
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void DS2438Write(uint8 WriteData)
{
    uint8  Read_bit=0;
    for(; Read_bit<8; Read_bit++)
    {
        SET_DQ(0);
        DelayDQ(5);
        if(WriteData & 0x01)   	//写数据
        {
            SET_DQ(1);    		//写1
        }
        else
        {
            SET_DQ(0);   		//写0
        }
        WriteData >>= 1;      	//数据右移1位，先写低位

        DelayDQ(80);		 	//60~120

        SET_DQ(1);
    }
}

/*********************************************************************************************************
* Function name:        RecallMem
* Descriptions:         把PageX中的内容复制到SP中，以供read_sp使用
* input parameters:     pagex  =0~7
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void RecallMem(uint8 PageX)
{
    DS2438Rst();									 //复位
    DS2438Write(SKIP_ROM);
    DS2438Write(RECALL_MEM);						 //发送recall 01h内存页命令
    DS2438Write(PageX);
    DelayDQ(80);		 	//60~120
    DS2438Rst();									 //复位
    DelayDQ(80);		 	//60~120
}
/*********************************************************************************************************
* Function name:        ReadRom
* Descriptions:         将某一PageX中的内容读出
* input parameters:     pagex  =0~7，  *StorePage存储数据
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
uint8 ReadOnePageRom(uint8 PageX,uint8 *StorePage)
{
    uint8 i;

    RecallMem(PageX);

    DS2438Rst();
    DS2438Write(SKIP_ROM);
    DS2438Write(READ_SP);							    //读取第01页
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
* Descriptions:         启动电压转换
* input parameters:
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void StartConvVol(void)
{
    DS2438Rst();
    DS2438Write(SKIP_ROM);
    DS2438Write(CONVERT_VOL);								 //电压转换

}
/*********************************************************************************************************
* Function name:        StartConvTemp
* Descriptions:         启动温度转换
* input parameters:
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void StartConvTemp(void)
{
    DS2438Rst();
    DS2438Write(SKIP_ROM);
    DS2438Write(CONVERT_TEMP);								 //温度转换
}

//uint8 WriteonePageRom(uint8 PageX,uint8 *StorePage)	   //没调通
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
* Descriptions:         将温度，电压，电流，ICA转化计算出最终结果，保存与ResultAll数组中
* input parameters:
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void ConvAll(void)
{

    StartConvVol();
    StartConvTemp();
//	WriteonePageRom(1,DS2438_page1_Init);
    ReadOnePageRom(0,DS2438_page0);
    ReadOnePageRom(1,DS2438_page1);

    ResultAll[0] =(int16)DS2438_page0[2]+(((int16)DS2438_page0[1])>>4)*0.0625;					  //温度
    ResultAll[1] =((uint16)DS2438_page0[4]<<8 | DS2438_page0[3])/100.0;							  //电压

    //0.025
    ResultAll[2] =((int16)(DS2438_page0[6]<<8 | DS2438_page0[5]))/0.1024;	 //204.8 =4096*0.05 102.4 = 4096*0.025   //电流
    ResultAll[3] =DS2438_page1[4]/0.512;	 //102.4 =2048*0.05	 51.2 = 2048*0.025								  //ICA
    //0.05
//	ResultAll[2] =((int16)(DS2438_page0[6]<<8 | DS2438_page0[5]))/0.2048;//0.1024;	 //204.8 =4096*0.05 102.4 = 4096*0.025   //电流
//	ResultAll[3] =DS2438_page1[4]/1.024;//0.512;	 //102.4 =2048*0.05	 51.2 = 2048*0.025								  //ICA

}

/*********************************************************************************************************
* Function name:        signed char DisBattery(fp32 *Battery)
* Descriptions:         电池电量LCD显示
* input parameters:     *Battery 电源管理芯片读出的值  应该调用ResultAll数组
						*(Battery+1)   电压
						*(Battery+2)   电流
						*(Battery+3)   电量
* output parameters:    无
* Returned value:       0
						-1			error
*********************************************************************************************************/
signed char DisBattery(volatile fp32 *Battery)
{
    uint8 charge;
    /*数据采集错误
    		电压太大
    		电流太大
    		电量太大
    */
    if( (*(Battery+1) > 20.0)
            || (abs(*(Battery+2)) > 600.0)
            || ( *(Battery+3) > 3500.0 ) )
    {
        return -1;
    }

    /*  充电
    	通过电流的正负来判断充放电
    	充电时的电压比放电是的大0.2v
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

    /*  正常显示  电池电量显示4、3、2、1、0格电量          */
    else
    {
        /*  电池有电，但没测出电量，用电压判断（用于电池第一次用，或更换了电池的情况） */

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


