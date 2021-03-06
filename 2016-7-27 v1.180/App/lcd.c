/****************************************Copyright (c)****************************************************
**                            		skyray-instrument Co.,LTD.
**
**                                 http://www.skyray-instrument.com
**
*********************************************************************************************************/
#include "lcd.h"
uint8 	RunStyleFlg	,HeatOn,TempCompensateFlag,PumpFlg=1 ,HeatFlg=1,	ClampMotor,	ClampRadio/*为1是放射源关闭*/,ClampMotorFlg, RunMotor,  MeasureFlg;

fp32 Humidity/*湿度阈值 */,HumiditySample/* 测试舱湿度 */,CycleOfSampSet	,MaxHeatTemp,MinHeatPower,FlowOfSampOut=5.45,FlowOfSampOut2=3.9 ,FlowOfSamp_B,FlowOfSamp_B_Set,FlowOfSamp ,HighV ,CycleOfSamp,
                                                                                                      BiaoPianTest,BiaoPian,WenDuT1/* 环境温度 */,WenDuT3/* 测试仓温度 */,WenDuT2/* 加热管温度 */,ShiDu,QiYa,LiuLiang_GK,LiuLiang_BK, Pump,Mass,PowerV1=5.1,PowerV2=12.3,PowerV3=-12.1,PowerV4=24,
                                                                                                                                                                                                                                                TCQWenDuT4/* 探测器温度 */,CYCShiDu/* 采样口湿度 */, TestTimes = 1/* 标膜回测测试 */;
float MoniTongDao ,ShuzhiTongDao;
fp32  MoniZhi,	ShuzhiZhi;

uint8  DisBuf[21][240];
DISDATSTR  DisDat;
uint8 volatile DatLength;

/*********************************************************************************************************
* Function name:        DelayLCD
* Descriptions:         用于DA的延时函数,内部调用
* input parameters:     延时的数字
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
volatile void DelayLCD(int32_t ulTime)
{
    int32_t i=0;

    while (ulTime--)
    {
        for (i = 0; i < 500; i++);
    }
}

/*********************************************************************************************************
** 函数名称: LCDIOIint
** 函数功能: lcd引脚初始化
** 入口参数: 无
** 返 回 值: 无
** 说    明:
*********************************************************************************************************/
void LCDIOIint(void)
{
    MASK_OFF;				//关屏蔽
    DATA_OUT;				//数据口设为输出
    CMD_OUT;				//命令口设置为输出

    CS_H;					//命令口初始化为高电平。
    WR_H;
    RD_H;
    CD_H;
    //数据口初始化为高电平
    GPIO_SetValue(1, 0xFFFFFFFF);
}

/*********************************************************************************************************
** Function name:       LCDWrite
** Descriptions:        lcd写数据 或命令
** input parameters:    cmd 0 数据 1 命令
						DATA  数据或命令
** output parameters:
** Returned value:      无
*********************************************************************************************************/
void LCDWrite(uint8 cmd,uint8 data)
{
    CS_L;
    if(1==cmd)
    {
        CD_L;
    }
    else
    {
        CD_H;
    }
    WR_L;
    GPIO_ClearValue(1, DATA_BUS);
    GPIO_SetValue(1, data<<24);
    WR_H;
    CD_H;
    CS_H;
}

/*********************************************************************************************************
* Function name:  SetVLCD
* Descriptions:   对比度设置
* input parameters: dat
* output parameters:
* Returned value:
*********************************************************************************************************/
void SetVLCD(uint8 dat)
{
    uint8 cc;

    if(dat > 4)
    {
        dat = 4;
    }
    cc = 25*(dat + 1);

    LCDWrite(1,0x81);   //VLCD=(CV0+Cpm*pm)*(1+(T-25)*CT%)
    LCDWrite(1,cc);
}

/*********************************************************************************************************
** Function name:       Init1698
** Descriptions:        初始lcd
** input parameters:
** output parameters:
** Returned value:      无
*********************************************************************************************************/
void InitLCD1698(void)
{
    CS_L;
    /*reset*/
    RES_L;
    DelayLCD(20);			//power on reset
    RES_H;
    DelayLCD(20);
    LCDWrite(1,0xe2);			//reset by command
    DelayLCD(20);

    LCDWrite(1,0x2b);   //internal power control  (13nF<LCD<=22nF)
    //wr_command(0x25); //set TC=-0.05%
    LCDWrite(1,0xA0);   //set line rate (00b:8.5Klps)
    LCDWrite(1,0xea);   //0XEA//set bias
    LCDWrite(1,0xf1);   //set com end
    LCDWrite(1,0x9f);   //set duty :1/160  159
    LCDWrite(1,0x81);   //set VLCD value
    LCDWrite(1,0x60);   //VLCD=(CV0+Cpm*pm)*(1+(T-25)*CT%)
    LCDWrite(1,0xc6);   //0XC4 //set LCD mapping control  //scan dirction com0~127
    //wr_command(0xc8);
    //wr_command(0x17);
    LCDWrite(1,0x89);   //set RAM address control
    LCDWrite(1,0xd0);   // B_G_R
    LCDWrite(1,0xd5);   //4k color
    LCDWrite(1,0xc8);   //SET COM SCAN FUNCTION
    LCDWrite(1,0x18);
    LCDWrite(1,0xaf);   //0xad//display  on
}

/*********************************************************************************************************
* Function name:  RAM_ADDRESS
* Descriptions:  设置起始终止区域
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
void RAM_ADDRESS()
{
    LCDWrite(1,0x70);  //set row MSB address
    LCDWrite(1,0x60);  //set row LSB address	0 行
    LCDWrite(1,0x11);  //set column MSB address
    LCDWrite(1,0x08);  //set column LSB address	  24 列
    LCDWrite(1,0xf4);  //set oolumn start address
    LCDWrite(1,0x18);  //start address=0			//24列
    LCDWrite(1,0xf6);  //set column end address
    LCDWrite(1,0x67);  //(0x4f+1)*3=240 103			//67	？？？？？？？？？
    LCDWrite(1,0xf5);  //set row start address	//
    LCDWrite(1,0x00);  //start address=0x00		  00
    LCDWrite(1,0xf7);  //set row end address		  9f
    LCDWrite(1,0x9f);  //row end address=9f  159
    LCDWrite(1,0xf8);  //inside mode 	   //在区域内
}

/*********************************************************************************************************
* Function name:        LcdClr
* Descriptions:         LCD清屏
* input parameters:     无
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void LcdClr(void)
{
    uint8 i,j;
    RAM_ADDRESS();
    for(i=0; i<160; i++)
    {
        for(j=0; j<120; j++)
        {
            LCDWrite(0,0x00);
        }
    }
}

/*********************************************************************************************************
* Function name:  ClrDisBuf
* Descriptions:   显示buf清零
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
void ClrDisBuf(void)
{
    uint8 i,j,*p;
    for(i=0; i<20; i++)
    {
        p=DisBuf[i];
        for(j=0; j<240; j++,p++)
        {
            *p=0x00;
            //DisBuf[i][j]=0;
        }
    }
}

/*********************************************************************************************************
* Function name:LcdUpdata
* Descriptions:   更新显示 将buf的值显示出来
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
uint8 LcdUpdata(uint8 dat[20][240])
{
    uint32 i,k,j,m,n,l,dat0,dat1,dat2,dat3,dat4,dat5,dat6;
    uint8 *p;

    p = dat[0];

    for(i=0; i<20; i++) //160行
    {
        n=i*240;
        for(j=0; j<8; j++)
        {
            m=i*8+j;
            LCDWrite(1,0x89);
            LCDWrite(1,0x08);
            LCDWrite(1,0x11);
            LCDWrite(1,0x60|((m)&0x0f));
            LCDWrite(1,0x70|(((m)&0xf0)>>4));
            for(k=0; k<40; k++)		//画   240列
            {
                l=k*6+n;
                dat6=0x01<<j;
                dat0=(*(p+l))&dat6;
                dat0=dat0>>j;
                dat0<<=7;
                dat1=(*(p+l+1))&dat6;
                dat1=dat1>>j;
                dat1<<=3;
                dat2=*(p+2+l)&dat6;
                dat2=dat2>>j;
                dat2<<=7;
                dat3=(*(p+3+l))&dat6;
                dat3=dat3>>j;
                dat3<<=3;
                dat4=(*(p+4+l))&dat6;
                dat4=dat4>>j;
                dat4<<=7;
                dat5=(*(p+5+l))&dat6;
                dat5=dat5>>j;
                dat5<<=3;
                LCDWrite(0,dat0|dat1);
                LCDWrite(0,dat2|dat3);
                LCDWrite(0,dat4|dat5);
            }
            LCDWrite(1,0x88);
        }
    }
    LCDWrite(1,0x89);

    return 0;
}

/*********************************************************************************************************
** Function name:      words56x56
** Descriptions:        56*56显示
** input parameters:    x,y 地址
** output parameters:   *p字模指针
** Returned value:      无
*********************************************************************************************************/
void pic56x56(uint8 x,uint8 y,const uint8 *p)
{

    uint8 i;

    for(i=0; i<56; i++,p++)
    {
        LcdWriteByte(x+i,y,*p);
        LcdWriteByte(x+i,y+8,*(p+56));
        LcdWriteByte(x+i,y+16,*(p+112));
        LcdWriteByte(x+i,y+24,*(p+168));
        LcdWriteByte(x+i,y+32,*(p+224));
        LcdWriteByte(x+i,y+40,*(p+280));
        LcdWriteByte(x+i,y+48,*(p+336));
    }
}
/*********************************************************************************************************
** Function name:      pic40x16
** Descriptions:        40*16显示
** input parameters:    x,y 地址
** output parameters:   *p字模指针
** Returned value:      无
*********************************************************************************************************/
void pic40x16(uint8 x,uint8 y,const uint8 *p)
{

    uint8 i;

    for(i=0; i<40; i++,p++)
    {
        LcdWriteByte(x+i,y,*p);
        LcdWriteByte(x+i,y+8,*(p+40));
    }
}

/*********************************************************************************************************
* Function name:  LcdReadByte
* Descriptions:   读出1byte数
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
uint8 LcdReadByte(uint8 x,uint8 y)
{
    uint8 i,j;//,k,m;//,n=0;
    i=y/8;
    j=y%8;

    return (DisBuf[i][x]>>j)|(DisBuf[i+1][x]<<(8-j));
}


/*********************************************************************************************************
* Function name:   LcdWriteByte
* Descriptions:    写入1byte数
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
void LcdWriteByte(uint8 x,uint8 y,uint8 data)
{
    uint8 i,j,k,m;//k=0,m=0;

    i=y/8;
    j=y%8;
    k=LcdReadByte(x,i*8);
    m=LcdReadByte(x,(i+1)*8);
    DisBuf[i][x]=(k&(0xff>>(8-j)))|(data<<j);
    DisBuf[i+1][x]=(m&(0xff<<j))|(data>>(8-j));
}

/*********************************************************************************************************
** Function name:      DisBuf168
** Descriptions:        16 8显示
** input parameters:    x,y 地址
** output parameters:   *p字模指针
** Returned value:      无
*********************************************************************************************************/
void DisBuf168(uint8 x,uint8 y,uint8 const p[])
{
    uint8 i;

    for(i=0; i<8; i++,p++)
    {
        LcdWriteByte(x+i,y,*p);
        LcdWriteByte(x+i,y+8,*(p+8));
    }
}
/*********************************************************************************************************
** Function name:      DisBuf126
** Descriptions:        12*6显示
** input parameters:    x,y 地址
** output parameters:   *p字模指针
** Returned value:      无
*********************************************************************************************************/
void DisBuf126(uint8 x,uint8 y,uint8 const p[])
{
    uint8 i,d;

    for(i=0; i<6; i++,p++)
    {
        LcdWriteByte(x+i,y,*p);
        d=LcdReadByte(x+i,y+8)&0xf0;
        d |=((*(p+6))&0x0f);
        LcdWriteByte(x+i,y+8,d);
    }
}

/*********************************************************************************************************
** Function name:      	DisBuf1212
** Descriptions:        12*12显示
** input parameters:    x,y 地址
** output parameters:   *p字模指针
** Returned value:      无
*********************************************************************************************************/
void DisBuf1212(uint8 x,uint8 y,uint8 const p[])
{
    uint8 i,d;

    for(i=0; i<12; i++,p++)
    {
        LcdWriteByte(x+i,y,*p);
        d=LcdReadByte(x+i,y+8)&0xf0;
        d |=((*(p+12))&0x0f);
        LcdWriteByte(x+i,y+8,d);
    }
}

/*********************************************************************************************************
** Function name:     	DisBuf1616
** Descriptions:        16 16显示
** input parameters:    x,y 地址
** output parameters:   *p字模指针
** Returned value:      无
*********************************************************************************************************/
void DisBuf1616(uint8 x,uint8 y,uint8 const p[])
{
    uint8 i;

    for(i=0; i<16; i++,p++)
    {
        LcdWriteByte(x+i,y,*p);
        LcdWriteByte(x+i,y+8,*(p+16));
    }
}

/*********************************************************************************************************
** Function name:      DisBuf2412
** Descriptions:        2412显示
** input parameters:    x,y 地址
** output parameters:   *p字模指针
** Returned value:      无
*********************************************************************************************************/
void DisBuf2412(uint8 x,uint8 y,uint8 const p[])
{
    uint8 i;

    for(i=0; i<12; i++,p++)
    {
        LcdWriteByte(x+i,y,*p);
        LcdWriteByte(x+i,y+8,*(p+12));
        LcdWriteByte(x+i,y+16,*(p+24));
    }
}

/*********************************************************************************************************
** Function name:      	DisCList12
** Descriptions:        显示一串字符
** input parameters:    x,y 地址
** output parameters:   dat字符串
** Returned value:      无
*********************************************************************************************************/
void DisCList12(uint8 x,uint8 y,uint8 const dat[21])
{
    uint8 i;
    uint8 const *p;
    uint16 temp;
    p=dat;
    for(i=0; i<53; p++)
    {
        if(0==*p)
        {
            break;
        }
        else if(*p<=128)
        {
            DisBuf126(x+i*6,y,hzk1212k2[*p]);
            i++;
        }
        else
        {
            temp=(*p-0xb0)*94;
            p++;
            temp=temp+(*p-0xa1)+129;
            DisBuf1212(x+i*6,y,hzk1212k2[temp]);
            i++;
            i++;
        }
    }

}
/*********************************************************************************************************
** Function name:      	DisCList16
** Descriptions:        显示一串字符
** input parameters:    x,y 地址
** output parameters:   dat字符串
** Returned value:      无
*********************************************************************************************************/

void DisCList16(uint8 x,uint8 y,uint8 const dat[21])
{
    uint8 i,err;
    uint8 const *p;
    uint16 temp;
    uint8	buffer[32]= {0};
    p=dat;
    for(i=0; i<53; p++)
    {
        if(0==*p)
        {
            break;
        }
        else if(*p<=128)
        {
            DisBuf168(x+i*8,y,hzk168k1[*p]);
            i++;
        }
        else
        {
            temp=(*p-0xb0)*94;
            p++;
            temp=temp+(*p-0xa1);
            OSMutexPend(OSFlashUsedMutex,0,&err);
            SSTF016B_RD( FLASH_ADD_ZK1616+32*(temp), 32,buffer);
            OSMutexPost(OSFlashUsedMutex);
            DisBuf1616(x+i*8,y,buffer);
            i++;
            i++;
        }
    }

}
/*********************************************************************************************************
** Function name:       DisEList126
** Descriptions:        显示 一串字符窜  12 6
** input parameters:    x,y 地址
** output parameters:   字模指针
** Returned value:      无
*********************************************************************************************************/
void DisEList126(uint8 x,uint8 y,uint8 const dat[40])
{
    uint8 i;
    uint8 const *p;
    p=dat;
    for(i=0; i<40; i++,p++)
    {
        if(0==*p)
        {
            break;
        }
        DisBuf126(x+i*6,y,hzk126k1[*p]);
    }
}


/*********************************************************************************************************
** Function name:       DisEList168
** Descriptions:        显示 一串字符窜  168
** input parameters:    x,y 地址
** output parameters:   字模指针
** Returned value:      无
*********************************************************************************************************/
void DisEList168(uint8 x,uint8 y,uint8 dat[21])
{
    uint8 i;
    uint8 const *p;
    p=dat;
    for(i=0; i<21; i++,p++)
    {
        if(0==*p)
        {
            break;
        }
        DisBuf168(x+i*8,y,hzk168k1[*p]);
    }
}

/*********************************************************************************************************
** Function name:       DisStr168
** Descriptions:        显示一行12*8文字
						dat的元素依次为各显示文字在hzk1212k1的偏移量
** input parameters:    x,y 地址
** output parameters:   字模指针
** Returned value:      无
*********************************************************************************************************/
void DisStr168(uint8 x,uint8 y,char *dat)
{
    uint8 i;
    char *p;
    p=dat;
    for(i=0; i<21; i++,p++)
    {
        if(0==*p)
        {
            break;
        }
        DisBuf168(x+i*8,y,hzk168k1[*p]);
    }
}

/*********************************************************************************************************
** Function name:       DisCStr16
** Descriptions:        显示 一串字符窜
** input parameters:    x,y 地址
** output parameters:   字模指针
** Returned value:      无
*********************************************************************************************************/



/*********************************************************************************************************
** Function name:        DisEList2412
** Descriptions:        显示 一串字符窜
** input parameters:    x,y 地址
** output parameters:   字模指针
** Returned value:      无
*********************************************************************************************************/
void DisEList2412(uint8 x,uint8 y,uint8 const dat[36])
{
    uint8 i;
    uint8 const *p;
    p=dat;
    for(i=0; i<21; i++,p++)
    {
        if(0==*p)
        {
            break;
        }
        DisBuf2412(x+i*12,y,hzk2412k1[*p]);
    }
}
/*********************************************************************************************************
** Function name:       ReverseVideo
** Descriptions:        对话框反白
** input parameters:    对话框地址
** output parameters:
** Returned value:      无
*********************************************************************************************************/
void ReverseVideo(uint8 area[])
{
    uint8 i,j,m=0;
    uint8 da,n;
    for(i=0; i<((area[3]-area[1]+1)/8); i++)
    {
        for(j=0; j<=(area[2]-area[0]); j++)
        {
            m=~LcdReadByte(area[0]+j,area[1]+i*8);
            LcdWriteByte(area[0]+j,area[1]+i*8,(uint8)m);
        }
    }
    da = (area[3]-area[1]+1)%8;
    if(da)
    {
        for(j=0; j<=(area[2]-area[0]); j++)
        {
            n=LcdReadByte(area[0]+j,area[1]+i*8);
            m=~n;
            m=m&(0xff>>(8-da));
            n=n&~(0xff>>(8-da));
            m=m|n;
            LcdWriteByte(area[0]+j,area[1]+i*8,(uint8)(m));
        }

    }
}

/*********************************************************************************************************
** Function name:    CleanVideo
** Descriptions:        区域清屏
** input parameters:    清屏的地址范围数组
** output parameters:
** Returned value:      无
*********************************************************************************************************/
void CleanVideo(uint8 area[])
{
    uint8 i,j,m=0;
    uint8 da,n;
    for(i=0; i<((area[3]-area[1]+1)/8); i++)
    {
        for(j=0; j<=(area[2]-area[0]); j++)
        {
//           m=~LcdReadByte(area[0]+j,area[1]+i*8);
            LcdWriteByte(area[0]+j,area[1]+i*8,(uint8)m);
        }

    }
    da = (area[3]-area[1]+1)%8;
    if(da)
    {
        for(j=0; j<=(area[2]-area[0]); j++)
        {
            n=LcdReadByte(area[0]+j,area[1]+i*8);
            m=0;
            m=m&(0xff>>(8-da));
            n=n&~(0xff>>(8-da));
            m=m|n;
            LcdWriteByte(area[0]+j,area[1]+i*8,(uint8)(m));
        }
    }
}

/*********************************************************************************************************
** Function name:    CReverseVideo
** Descriptions:        显示菜单标题
** input parameters:     汉字字符组
** output parameters:
** Returned value:      无
*********************************************************************************************************/
void CReverseVideo(uint8 const area[])
{
    uint8 i,j,m=0;
    uint8 da,n;
    for(i=0; i<((area[3]-area[1]+1)/8); i++)
    {
        for(j=0; j<=(area[2]-area[0]); j++)
        {
            m=~LcdReadByte(area[0]+j,area[1]+i*8);
            LcdWriteByte(area[0]+j,area[1]+i*8,(uint8)m);
        }

    }
    da = (area[3]-area[1]+1)%8;
    if(da)
    {
        for(j=0; j<=(area[2]-area[0]); j++)
        {
            n=LcdReadByte(area[0]+j,area[1]+i*8);
            m=~n;
            m=m&(0xff>>(8-da));
            n=n&~(0xff>>(8-da));
            m=m|n;
            LcdWriteByte(area[0]+j,area[1]+i*8,(uint8)(m));
        }
    }
}


/*********************************************************************************************************
* Function name: DarwHLine
* Descriptions:  画一条横线
* input parameters: x1,x2,y  y坐标，x起始和终止
* output parameters:
* Returned value:
*********************************************************************************************************/
void DarwHLine(uint8 x1,uint8 x2,uint8 y)
{
    uint8 i,d;
    for(i=x1; i<=x2; i++)
    {
        d=LcdReadByte(i,y);
        LcdWriteByte(i,y,d|0x01);
    }
}


/*********************************************************************************************************
* Function name: DarwVLine
* Descriptions:  画一条竖线
* input parameters:  x,y11,y2  x坐标，y起始和终止
* output parameters:
* Returned value:
*********************************************************************************************************/
void DarwVLine(uint8 x,uint8 y11,uint8 y2)
{
    uint8 i,d;
    for(i=y11; i<=y2; i++)
    {
        d=LcdReadByte(x,i);
        LcdWriteByte(x,i,d|0x01);
    }
}

/*********************************************************************************************************
** Function name:       DarwButton
** Descriptions:        显示 纽扣 图标
** input parameters:    x,y 地址
** output parameters:
** Returned value:      无
*********************************************************************************************************/
void DarwButton(uint8 x,uint8 y)
{
    uint8 i,d;

    for(i=0; i<2; i++)
    {
        d=LcdReadByte(x+i,y);
        LcdWriteByte(x+i,y,d|0x01);
    }
    for(i=0; i<111; i++)
    {
        d=LcdReadByte(x+i+2,y);
        LcdWriteByte(x+i+2,y,d|0x05);
    }
    for(i=0; i<2; i++)
    {
        d=LcdReadByte(x+i+113,y);
        LcdWriteByte(x+i+113,y,d|0x01);
    }

    LcdWriteByte(x,y,0xff);
    LcdWriteByte(x,y+8,0xff);
    d=LcdReadByte(x,y+16);
    LcdWriteByte(x,y+16,d|0x3f);

    LcdWriteByte(x+2,y+2,0xff);
    LcdWriteByte(x+2,y+8+2,0xff);
    LcdWriteByte(x+2,y+16+2,0x03);

    LcdWriteByte(x+113,y+2,0xff);
    LcdWriteByte(x+113,y+2+8,0xff);
    LcdWriteByte(x+113,y+16+2,0x03);

    LcdWriteByte(x+115,y,0xff);
    LcdWriteByte(x+115,y+8,0xff);
    d=LcdReadByte(x+115,y+16);
    LcdWriteByte(x+115,y+16,d|0x3f);

    LcdWriteByte(x+116,y+1,0xff);
    LcdWriteByte(x+116,y+8+1,0xff);
    d=LcdReadByte(x+116,y+16+1);
    LcdWriteByte(x+116,y+16+1,d|0x3f);

    LcdWriteByte(x+117,y+2,0xff);
    LcdWriteByte(x+117,y+8+2,0xff);
    d=LcdReadByte(x+117,y+16+2);
    LcdWriteByte(x+117,y+16+2,d|0x3f);

    for(i=0; i<111; i++)
    {
        d=LcdReadByte(x+i+2,y+19);
        LcdWriteByte(x+i+2,y+19,d|0x1d);
    }

    d=LcdReadByte(x+1,y+21);
    LcdWriteByte(x+1,y+21,d|0x03);

    for(i=0; i<5; i++)
    {
        d=LcdReadByte(x+113+i,y+21);
        LcdWriteByte(x+113+i,y+21,d|0x07);
    }
}

/*********************************************************************************************************
** Function name:    DarwScrollBar
** Descriptions:        显示 滚动条
** input parameters:    tdat 当前数	  ndat 总数
** output parameters:
** Returned value:      无
*********************************************************************************************************/
void DarwScrollBar(uint16 tdat,uint16 ndat)
{
    uint8 c,d,i;
    uint8 cdi[4]= {220,25,239,159};
    if(ndat > 134)
    {   c = 3;
        d = 25 + ( 134 - ( 134 * tdat / ndat ) );
    }
    else
    {
        c = 134/ndat;
        d= 25+c*(tdat - 1);
        c = c + (134 % ndat);
    }

    CleanVideo(cdi);
    DarwHLine(220,239,25);
    DarwHLine(220,239,159);
    DarwVLine(220,25,159);
    DarwVLine(239,25,159);
    for(i=0; i<c; i++)
    {
        DarwHLine(220,239,d+i);
    }
}

/*********************************************************************************************************
* Function name:   DrawRectangular
* Descriptions:    画一对话框边
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
void DrawRectangular(uint8 const area[])
{
    DarwHLine(area[0],area[2],area[1]);
    DarwHLine(area[0],area[2],area[3]);
    DarwVLine(area[0],area[1],area[3]);
    DarwVLine(area[2],area[1],area[3]);
}

/*********************************************************************************************************
** Function name: DisTime
** Descriptions:        显示 倒计时
** input parameters:    x,y 地址 font显示大小
** output parameters:
** Returned value:      无
*********************************************************************************************************/
void DisTime(uint16 x,uint8 y,TimeDat dat,uint8 Font)
{
    uint8 i;
    switch(Font)
    {
    case 1:
        i=(dat.hour/10);
        DisBuf126(x+EFONT_12*0,y,hzk126k1[48+i]);
        i=dat.hour%10;
        DisBuf126(x+EFONT_12*1,y,hzk126k1[48+i]);

        DisBuf126(x+EFONT_12*2,y,hzk126k1[':']);

        i=dat.minute/10;
        DisBuf126(x+EFONT_12*3,y,hzk126k1[48+i]);
        i=dat.minute%10;
        DisBuf126(x+EFONT_12*4,y,hzk126k1[48+i]);

        DisBuf126(x+EFONT_12*5,y,hzk126k1[':']);

        i=dat.second/10;
        DisBuf126(x+EFONT_12*6,y,hzk126k1[48+i]);
        i=dat.second%10;
        DisBuf126(x+EFONT_12*7,y,hzk126k1[48+i]);

        break;
    case 3:
        i=(dat.hour/10);
        DisBuf126(x+EFONT_12*0,y,hzk126k1[48+i]);
        i=dat.hour%10;
        DisBuf126(x+EFONT_12*1,y,hzk126k1[48+i]);
        DisBuf126(x+EFONT_12*2,y,hzk126k1[':']);

        i=dat.minute/10;
        DisBuf126(x+EFONT_12*3,y,hzk126k1[48+i]);
        i=dat.minute%10;
        DisBuf126(x+EFONT_12*4,y,hzk126k1[48+i]);
//       DisBuf126(x+EFONT_12*5,y,hzk126k1[':']);

//       i=dat.second/10;
//       DisBuf126(x+EFONT_12*6,y,hzk126k1[48+i]);
//       i=dat.second%10;
//       DisBuf126(x+EFONT_12*7,y,hzk126k1[48+i]);

        break;
    case 2:
        /*
        i=(Time[0]>>4);
        DisBuf168(x+EFONT_16*0,y,hzk1616k1[48+i]);
        i=Time[0]&0x0f;
        DisBuf168(x+EFONT_16*1,y,hzk1616k1[48+i]);

        DisBuf168(x+EFONT_16*2,y,hzk1616k1['|']);

        i=Time[1]>>4;
        DisBuf168(x+EFONT_16*3,y,hzk1616k2[48+i]);
        i=Time[1]&0x0f;
        DisBuf168(x+EFONT_16*4,y,hzk1616k2[48+i]);

        DisBuf168(x+EFONT_16*5,y,hzk1616k2['|']);

        i=Time[2]>>4;
        DisBuf168(x+EFONT_16*6,y,hzk1616k2[48+i]);
        i=Time[2]&0x0f;
        DisBuf168(x+EFONT_16*7,y,hzk1616k2[48+i]);
        */
        break;
    }
}

/*********************************************************************************************************
** Function name:       DisDate
** Descriptions:        显示 日期
** input parameters:    x,y 地址 font 样式
** output parameters:
** Returned value:      无
*********************************************************************************************************/
void DisDate(uint16 x,uint8 y,TimeDat dat,uint8 Font)
{
    uint8 i;
    switch(Font)
    {
    case 1:
//       i=(dat.year/1000);
//       DisBuf126(x+EFONT_12*0,y,hzk126k1[48+i]);
//       i=dat.year%1000/100;
//       DisBuf126(x+EFONT_12*1,y,hzk126k1[48+i]);
        i=(dat.year%100/10);
        DisBuf126(x+EFONT_12*2,y,hzk126k1[48+i]);
        i=(dat.year%10);
        DisBuf126(x+EFONT_12*3,y,hzk126k1[48+i]);

        DisBuf126(x+EFONT_12*4,y,hzk126k1['/']);

        i=dat.month/10;
        DisBuf126(x+EFONT_12*5,y,hzk126k1[48+i]);
        i=dat.month%10;
        DisBuf126(x+EFONT_12*6,y,hzk126k1[48+i]);

        DisBuf126(x+EFONT_12*7,y,hzk126k1['/']);

        i=dat.date/10;
        DisBuf126(x+EFONT_12*8,y,hzk126k1[48+i]);
        i=dat.date%10;
        DisBuf126(x+EFONT_12*9,y,hzk126k1[48+i]);

        break;

    case 2:
//       i=(dat.year/1000);
//       DisBuf168(x+EFONT_16*0,y,hzk168k1[48+i]);
//       i=dat.year%1000/100;
//       DisBuf168(x+EFONT_16*1,y,hzk168k1[48+i]);
        i=(dat.year%100/10);
        DisBuf168(x+EFONT_16*2,y,hzk168k1[48+i]);
        i=(dat.year%10);
        DisBuf168(x+EFONT_16*3,y,hzk168k1[48+i]);

        DisBuf168(x+EFONT_16*4,y,hzk168k1['/']);

        i=dat.month/10;
        DisBuf168(x+EFONT_16*5,y,hzk168k1[48+i]);
        i=dat.month%10;
        DisBuf168(x+EFONT_16*6,y,hzk168k1[48+i]);

        DisBuf168(x+EFONT_16*7,y,hzk168k1['/']);

        i=dat.date/10;
        DisBuf168(x+EFONT_16*8,y,hzk168k1[48+i]);
        i=dat.date%10;
        DisBuf168(x+EFONT_16*9,y,hzk168k1[48+i]);

        /*
        i=(Time[0]>>4);
        DisBuf168(x+EFONT_16*0,y,hzk1616k2[48+i]);
        i=Time[0]&0x0f;
        DisBuf168(x+EFONT_16*1,y,hzk1616k2[48+i]);

        DisBuf168(x+EFONT_16*2,y,hzk1616k2['.']);

        i=Time[1]>>4;
        DisBuf168(x+EFONT_16*3,y,hzk1616k2[48+i]);
        i=Time[1]&0x0f;
        DisBuf168(x+EFONT_16*4,y,hzk1616k2[48+i]);

        DisBuf168(x+EFONT_16*5,y,hzk1616k2['.']);

        i=Time[2]>>4;
        DisBuf168(x+EFONT_16*6,y,hzk1616k2[48+i]);
        i=Time[2]&0x0f;
        DisBuf168(x+EFONT_16*7,y,hzk1616k2[48+i]);
        */
        break;

    case 3:		 //用在记录值处
//	   i=(dat.year%100/10);
//       DisBuf126(x+EFONT_12*2,y,hzk126k1[48+i]);
//       i=(dat.year%10);
//       DisBuf126(x+EFONT_12*3,y,hzk126k1[48+i]);
//
//       DisBuf126(x+EFONT_12*4,y,hzk126k1['/']);

        i=dat.month/10;
        DisBuf126(x+EFONT_12*2,y,hzk126k1[48+i]);
        i=dat.month%10;
        DisBuf126(x+EFONT_12*3,y,hzk126k1[48+i]);

        DisBuf126(x+EFONT_12*4,y,hzk126k1['/']);

        i=dat.date/10;
        DisBuf126(x+EFONT_12*5,y,hzk126k1[48+i]);
        i=dat.date%10;
        DisBuf126(x+EFONT_12*6,y,hzk126k1[48+i]);
        break;
    }
}

/*********************************************************************************************************
* Function name:   GetNumLen
* Descriptions:     取得数据长度
* input parameters:  fValuel 数据
* output parameters:
* Returned value:
*********************************************************************************************************/
int8 GetNumLen(fp32 fValuel)
{
    if(     fValuel<=-1000000000)
    {
        return -10;
    }
    else if(fValuel<=-100000000)
    {
        return -9;
    }
    else if(fValuel<=-10000000)
    {
        return -8;
    }
    else if(fValuel<=-1000000)
    {
        return -7;
    }
    else if(fValuel<=-100000)
    {
        return -6;
    }
    else if(fValuel<=-10000)
    {
        return -5;
    }
    else if(fValuel<=-1000)
    {
        return -4;
    }
    else if(fValuel<=-100)
    {
        return -3;
    }
    else if(fValuel<=-10)
    {
        return -2;
    }
    else if(fValuel<0)
    {
        return -1;
    }
    else if((fValuel>=-0.00001)&&(fValuel<=0.00001))
    {
        return 0;
    }
    else if(fValuel<10)
    {
        return 1;
    }
    else if(fValuel<100)
    {
        return 2;
    }
    else if(fValuel<1000)
    {
        return 3;
    }
    else if(fValuel<10000)
    {
        return 4;
    }
    else if(fValuel<100000)
    {
        return 5;
    }
    else if(fValuel<1000000)
    {
        return 6;
    }
    else if(fValuel<10000000)
    {
        return 7;
    }
    else if(fValuel<100000000)
    {
        return 8;
    }
    else if(fValuel<1000000000)
    {
        return 9;
    }
    else if(fValuel<10e10)
    {
        return 10;
    }
    return 127;
}


/*********************************************************************************************************
* Function name:  Num10n
* Descriptions://10的n次方
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
uint32 Num10n(uint8 n)
{
    uint8 i;
    uint32 d=1;
    for(i=0; i<n; i++)
    {
        d=d*10;
    }
    return d;
}

/*********************************************************************************************************
* Function name: DisValue
* Descriptions:  显示结果
* input parameters:  fp32 fValuel, 值
					uint16 x,uint8 y, 显示地址
					uint8 font,		  显示字体大小档
					uint8 length,	  显示位数
					uint8 decimal,	  小数点后的个数
					uint8 DisSign     是否显示正负。
					uint8 DanWei	  单位
* output parameters:
* Returned value:
*********************************************************************************************************/
void DisValue2(fp32 fValuel,uint16 x,uint8 y, DISDATSTR *DisDat ,uint8 *DanWei)
{
    uint32 ulTemp;
    uint8  aDis[10];
    uint8 i,j;
    uint8 sign=0;                 //=0 +  ,=1 -
    uint8  c[20]= {"O.F."};
    int8 numLen;
    fp32 fd;

    if(fValuel<0)
    {
        fValuel=fabs(fValuel);
        sign=1;
    }

    numLen=GetNumLen(fValuel);
    if((numLen+DisDat->decimal)<=DisDat->length)   //
    {
        fd=fValuel*Num10n(DisDat->decimal);

        ulTemp=(uint32)fd;
        LongIntegerToBit(ulTemp,aDis);

        switch(DisDat->font)
        {
        case 1:
            if(1==sign)
            {
                DisBuf126(x,y,hzk1212k2[45]);
            }
            else
            {
                DisBuf126(x,y,hzk1212k2[' ']);
            }

            if(1==DisDat->DisSign)
            {
                j=1;
            }
            else
            {
                j=0;
            }

            if(0==numLen)
            {
                DisBuf126(x,y,hzk1212k2[48]);
                j++;
            }
            else
            {
                for(i=1; i<=numLen; i++,j++)        //显示整数
                {
                    DisBuf126(x+j*EFONT_12,y,hzk1212k2[48+aDis[9-numLen-DisDat->decimal+i]]);
                }
            }
            if(0< (DisDat->decimal))
            {
                DisBuf126(x+j*EFONT_12,y,hzk1212k2['.']);
                j++;
                for(i=1; i<= (DisDat->decimal); i++,j++)
                {
                    DisBuf126(x+j*EFONT_12,y,hzk1212k2[48+aDis[9-DisDat->decimal+i]]);
                }
            }
            //单位
            if(strcmp(DanWei," ℃")==0)
                WriteMyWord1212(x+j*EFONT_12,y ,"℃");
            else
                WriteCode1212(x+j*EFONT_12,y ,DanWei);
            break;

        case 2:
            if(1==sign)
            {
                DisBuf168(x,y,hzk168k1['-']);
            }
            else
            {
                DisBuf168(x,y,hzk168k1[' ']);
            }

            if(1 == DisDat->DisSign)
            {
                j=1;
            }
            else
            {
                j=0;
            }

            if(0==numLen)
            {
                DisBuf168(x,y,hzk168k1[48]);
                j++;
            }
            else
            {
                for(i=1; i<=numLen; i++,j++)        //显示整数
                {
                    DisBuf168(x+j*EFONT_16,y,hzk168k1[48+aDis[9-numLen-DisDat->decimal+i]]);
                }
            }
            if(0<DisDat->decimal)
            {
                DisBuf168(x+j*EFONT_16,y,hzk168k1['.']);
                j++;
                for(i=1; i<=DisDat->decimal; i++,j++)
                {
                    DisBuf168(x+j*EFONT_16,y,hzk168k1[48+aDis[9-DisDat->decimal+i]]);
                }
            }
            //单位
            if(strcmp(DanWei," ℃")==0)
                WriteMyWord1212(x+j*EFONT_16,y ,"℃");
            else
                DisCList16(x+j*EFONT_16,y ,DanWei);

            break;

        case 3:
            if(1==sign)
            {
                DisBuf2412(x,y,hzk2412k1['-']);
            }
            else
            {
                DisBuf2412(x,y,hzk2412k1[' ']);
            }

            if(1== (DisDat->DisSign))
            {
                j=1;
            }
            else
            {
                j=0;
            }

            if(0==numLen)
            {
                DisBuf2412(x,y,hzk2412k1[48]);
                j++;
            }
            else
            {
                for(i=1; i<= (numLen); i++,j++)        //显示整数
                {
                    DisBuf2412(x+j*EFONT_24,y,hzk2412k1[48+aDis[9-numLen-DisDat->decimal+i]]);
                }
            }
            if(0<(DisDat->decimal))
            {
                DisBuf2412(x+j*EFONT_24,y,hzk2412k1['.']);
                j++;
                for(i=1; i<=(DisDat->decimal); i++,j++)
                {
                    DisBuf2412(x+j*EFONT_24,y,hzk2412k1[48+aDis[9-DisDat->decimal+i]]);
                }
            }
            if(strcmp(DanWei,"ug/m3")==0)
                DisUg_M3_En2412(x+j*EFONT_24,y);
            else
                DisBuf2412(x+j*EFONT_24,y ,DanWei);

            break;

        case 4:

            break;
        }
    }
    else if(numLen<=(DisDat->length))                                        //整数加小数部分小于长度
    {
        fd=fValuel*Num10n(DisDat->length-numLen);
        ulTemp=(uint32)fd;
        LongIntegerToBit(ulTemp,aDis);

        switch(DisDat->font)
        {
        case 1:
            if(1==sign)
            {
                DisBuf126(x,y,hzk126k1['-']);
            }
            else
            {
                DisBuf126(x,y,hzk126k1[' ']);
            }
            if(1==DisDat->DisSign)
            {
                j=1;
            }
            else
            {
                j=0;
            }
            for(i=1; i<=DisDat->length; i++,j++)  //显示整数
            {
                DisBuf126(x+j*EFONT_12,y,hzk1212k2[48+aDis[9-DisDat->length+i]]);
                if((i==numLen)&&(numLen<DisDat->length))
                {
                    j++;
                    DisBuf126(x+j*EFONT_12,y,hzk1212k2['.']);
                }
            }			   //单位
            if(strcmp(DanWei," ℃")==0)
                WriteMyWord1212(x+j*EFONT_12,y ,"℃");
            else
                WriteCode1212(x+j*EFONT_12,y ,DanWei);

            break;

        case 2:
            if(1==sign)
            {
                DisBuf168(x,y,hzk168k1['-']);
            }
            else
            {
                DisBuf168(x,y,hzk168k1[' ']);
            }
            if(1==DisDat->DisSign)
            {
                j=1;
            }
            else
            {
                j=0;
            }
            for(i=1; i<=DisDat->length; i++,j++)  //显示整数
            {
                DisBuf168(x+j*EFONT_16,y,hzk168k1[48+aDis[9-DisDat->length+i]]);
                if((i==numLen)&&(numLen<DisDat->length))
                {
                    j++;
                    DisBuf168(x+j*EFONT_16,y,hzk168k1['.']);
                }
            }		   //单位
            if(strcmp(DanWei," ℃")==0)
                WriteMyWord1212(x+j*EFONT_16,y ,"℃");
            else
                DisCList16(x+j*EFONT_16,y ,DanWei);

            break;

        case 3:
            if(1==sign)
            {
                DisBuf2412(x,y,hzk2412k1['-']);
            }
            else
            {
                DisBuf2412(x,y,hzk2412k1[' ']);
            }
            if(1==DisDat->DisSign)
            {
                j=1;
            }
            else
            {
                j=0;
            }
            for(i=1; i<=DisDat->length; i++,j++)  //显示整数
            {
                DisBuf2412(x+j*EFONT_24,y,hzk2412k1[48+aDis[9-DisDat->length+i]]);
                if((i==numLen)&&(numLen<DisDat->length))
                {
                    j++;
                    DisBuf2412(x+j*EFONT_24,y,hzk2412k1['.']);
                }
            }
            if(strcmp(DanWei,"ug/m3")==0)
                DisUg_M3_En2412(x+j*EFONT_24,y);
            else
                DisBuf2412(x+j*EFONT_24,y ,DanWei);


            break;

        case 4:

            break;


        }
    }
    else
    {
        switch(DisDat->font)
        {
        case 1:
            DisCList12(x,y,"  O.F.");
            break;

        case 2:
            DisCList16(x,y,c);
            break;

        case 3:
            DisEList2412(x,y,c);
            break;

        case 4:

            break;

        case 5:
//           DisCEList64(x,y,c);
            break;
        }
    }
}

void DisValue(fp32 fValuel,uint16 x,uint8 y,uint8 font,uint8 length,uint8 decimal,uint8 DisSign)
{
    uint32 ulTemp;
    uint8  aDis[10];
    uint8 i,j;
    uint8 sign=0;                 //=0 +  ,=1 -
    uint8  c[20]= {"O.F."};
    int8 numLen;
    fp32 fd;

    if(fValuel<0)
    {
        fValuel=fabs(fValuel);
        sign=1;
    }

    numLen=GetNumLen(fValuel);
    if((numLen+decimal)<=length)   //
    {
        fd=fValuel*Num10n(decimal);

        ulTemp=(uint32)fd;
        LongIntegerToBit(ulTemp,aDis);

        switch(font)
        {
        case 1:
            if(1==sign)
            {
                DisBuf126(x,y,hzk1212k2[45]);
            }
            else
            {
                DisBuf126(x,y,hzk1212k2[' ']);
            }

            if(1==DisSign)
            {
                j=1;
            }
            else
            {
                j=0;
            }

            if(0==numLen)
            {
                DisBuf126(x,y,hzk1212k2[48]);
                j++;
            }
            else
            {
                for(i=1; i<=numLen; i++,j++)        //显示整数
                {
                    DisBuf126(x+j*EFONT_12,y,hzk1212k2[48+aDis[9-numLen-decimal+i]]);
                }
            }
            if(0<decimal)
            {
                DisBuf126(x+j*EFONT_12,y,hzk1212k2['.']);
                j++;
                for(i=1; i<=decimal; i++,j++)
                {
                    DisBuf126(x+j*EFONT_12,y,hzk1212k2[48+aDis[9-decimal+i]]);
                }
            }
            break;

        case 2:
            if(1==sign)
            {
                DisBuf168(x,y,hzk168k1['-']);
            }
            else
            {
                DisBuf168(x,y,hzk168k1[' ']);
            }

            if(1==DisSign)
            {
                j=1;
            }
            else
            {
                j=0;
            }

            if(0==numLen)
            {
                DisBuf168(x,y,hzk168k1[48]);
                j++;
            }
            else
            {
                for(i=1; i<=numLen; i++,j++)        //显示整数
                {
                    DisBuf168(x+j*EFONT_16,y,hzk168k1[48+aDis[9-numLen-decimal+i]]);
                }
            }
            if(0<decimal)
            {
                DisBuf168(x+j*EFONT_16,y,hzk168k1['.']);
                j++;
                for(i=1; i<=decimal; i++,j++)
                {
                    DisBuf168(x+j*EFONT_16,y,hzk168k1[48+aDis[9-decimal+i]]);
                }
            }

            break;

        case 3:
            if(1==sign)
            {
                DisBuf2412(x,y,hzk2412k1['-']);
            }
            else
            {
                DisBuf2412(x,y,hzk2412k1[' ']);
            }

            if(1==DisSign)
            {
                j=1;
            }
            else
            {
                j=0;
            }

            if(0==numLen)
            {
                DisBuf2412(x,y,hzk2412k1[48]);
                j++;
            }
            else
            {
                for(i=1; i<=numLen; i++,j++)        //显示整数
                {
                    DisBuf2412(x+j*EFONT_24,y,hzk2412k1[48+aDis[9-numLen-decimal+i]]);
                }
            }
            if(0<decimal)
            {
                DisBuf2412(x+j*EFONT_24,y,hzk2412k1['.']);
                j++;
                for(i=1; i<=decimal; i++,j++)
                {
                    DisBuf2412(x+j*EFONT_24,y,hzk2412k1[48+aDis[9-decimal+i]]);
                }
            }

            break;

        case 4:

            break;
        }
    }
    else if(numLen<=length)                                        //整数加小数部分小于长度
    {
        fd=fValuel*Num10n(length-numLen);
        ulTemp=(uint32)fd;
        LongIntegerToBit(ulTemp,aDis);

        switch(font)
        {
        case 1:
            if(1==sign)
            {
                DisBuf126(x,y,hzk126k1['-']);
            }
            else
            {
                DisBuf126(x,y,hzk126k1[' ']);
            }
            if(1==DisSign)
            {
                j=1;
            }
            else
            {
                j=0;
            }
            for(i=1; i<=length; i++,j++)  //显示整数
            {
                DisBuf126(x+j*EFONT_12,y,hzk1212k2[48+aDis[9-length+i]]);
                if((i==numLen)&&(numLen<length))
                {
                    j++;
                    DisBuf126(x+j*EFONT_12,y,hzk1212k2['.']);
                }
            }
            break;

        case 2:
            if(1==sign)
            {
                DisBuf168(x,y,hzk168k1['-']);
            }
            else
            {
                DisBuf168(x,y,hzk168k1[' ']);
            }
            if(1==DisSign)
            {
                j=1;
            }
            else
            {
                j=0;
            }
            for(i=1; i<=length; i++,j++)  //显示整数
            {
                DisBuf168(x+j*EFONT_16,y,hzk168k1[48+aDis[9-length+i]]);
                if((i==numLen)&&(numLen<length))
                {
                    j++;
                    DisBuf168(x+j*EFONT_16,y,hzk168k1['.']);
                }
            }
            break;

        case 3:
            if(1==sign)
            {
                DisBuf2412(x,y,hzk2412k1['-']);
            }
            else
            {
                DisBuf2412(x,y,hzk2412k1[' ']);
            }
            if(1==DisSign)
            {
                j=1;
            }
            else
            {
                j=0;
            }
            for(i=1; i<=length; i++,j++)  //显示整数
            {
                DisBuf2412(x+j*EFONT_24,y,hzk2412k1[48+aDis[9-length+i]]);
                if((i==numLen)&&(numLen<length))
                {
                    j++;
                    DisBuf2412(x+j*EFONT_24,y,hzk2412k1['.']);
                }
            }
            break;

        case 4:

            break;


        }
    }
    else
    {
        switch(font)
        {
        case 1:
            DisCList12(x,y,"  O.F.");
            break;

        case 2:
            DisCList16(x,y,c);
            break;

        case 3:
            DisEList2412(x,y,c);
            break;

        case 4:

            break;

        case 5:
//           DisCEList64(x,y,c);
            break;
        }
    }
}

/*********************************************************************************************************
** Function name:      write_row_strip
** Descriptions:       画一大图	 320*240
** input parameters:    x:x地址
						y:y地址
** output parameters:
** Returned value:      无
*********************************************************************************************************/
void Write240x160(const uint8 *p)
{
    uint8 i,j;

    for(i=0; i<20; i++)
    {
        for(j=0; j<240; j++)
        {
            LcdWriteByte(j,i*8,*(p+j+i*240));
        }
    }
}

/*********************************************************************************************************
** Function name:       DisConfigState
** Descriptions:       设置状态单个显示
** input parameters:    OnOff 开关量状态 0关 1 开  信号状态时无用
						dat 数据值  开关状态时无用
						CurrId 当前meuID
						showplace当在第一页时应该的显示位置
						place 实际显示的位置
** output parameters:
** Returned value:      无
*********************************************************************************************************/
void DisConfigStateSwitch(uint8 OnOff,uint16 lan_offset,uint8 showplace,uint8 place)
{
    uint8 MenuNum;
    uint16 y,i;
    for(i = 2; i<8; i++)
    {
        MenuNum = ((MenuId&0xffffff00)>>i*4)&0x0f;
        if(MenuNum)
        {
            break;
        }
    }
    y = 26+(showplace-MenuNum+place)*22;
    if( ( (y ) > 25 )&&( (y ) <137 ))	//在显示范围内
    {
        DisCList16(120,(y),"[          ]");
        if(lan_offset == 68)
        {
            DisCList16(120+8+24,(y),NextPrompt[OnOff+lan_offset+NePrOffset]);
        }
        else
        {
            DisCList16(120+8+24,(y),NextPrompt[(0X01^OnOff)+lan_offset+NePrOffset]);
        }
    }
}

void DisConfigStateValue(fp32 dat,uint8 showplace,uint8 place,uint8 DisDatStyle,uint8 *danwei)
{
    uint8 MenuNum,DecimalFlg ,length2;
    uint16 y,i;
    uint16 x = 120;
    length2 = GetNumLen(fabs(dat));
    if(length2==0)
        length2=1;

    for(i=0; i<NUM_PARAM; i++)
    {
        if(DisDat2[i].ParamName == DisDatStyle )
        {
            DisDat.font = DisDat2[i].font;
            DisDat.length = DisDat2[i].length;
            DisDat.decimal = DisDat2[i].decimal;
            DisDat.DisSign = DisDat2[i].DisSign;
            DecimalFlg = (DisDat.decimal>0)?(1):(0);
            break;
        }
    }

    for(i = 2; i<8; i++)
    {
        MenuNum = ((MenuId&0xffffff00)>>i*4)&0x0f;
        if(MenuNum)
        {
            break;
        }
    }
    y=	26+(showplace-MenuNum+place)*22;

    if( ( (y) > 25 )&&( (y) <137 ))	//在显示范围内
    {
        DisCList16(x,(y ),"[          ]");
        (dat>=0)?(DisCList16(x+8,y," ")):(DisCList16(x+8,y,"-"))	;
        DatLength = DisDat.length+DecimalFlg-1+DisDat.DisSign; //正负号，数值长度，小数点

        DisDat.DisSign = 0;
        //	[ +-
        DisValue2(dat,x+8+ 8 +(DisDat.length-length2-DisDat.decimal)*8,(y ),&DisDat,danwei);

    }


}

FanHei(uint8 place,uint8 Flg,uint8 ParamName)
{
    uint8 i;
    if(Flg)
    {
        for(i=0; i<NUM_PARAM; i++)
        {
            if(DisDat2[i].ParamName == ParamName )
            {
                break;
            }
        }
        // +-
        area[0] = 128+(OffsetFlg+1-DisDat2[i].DisSign)*8;
        area[1] = 26+(place-1)*22;
        area[2] = 134+(OffsetFlg+1-DisDat2[i].DisSign)*8;
        area[3] = 41+(place-1)*22;
        CReverseVideo(area);
    }
    else
    {
        CReverseVideo(MenuFormat.SetArea[place-1]);
    }

}

/*********************************************************************************************************
** Function name:       DisConfigState
** Descriptions:       设置状态显示
** input parameters:
** output parameters:
** Returned value:      无
*********************************************************************************************************/

//显示开关左右选择 涂黑显示：   开|关
void DisConfigStateSwitch2(uint8 OnOff,uint8 showplace,uint8 place)
{
    uint8 MenuNum;
    uint16 y,i;
    for(i = 2; i<8; i++)
    {
        MenuNum = ((MenuId&0xffffff00)>>i*4)&0x0f;
        if(MenuNum)
        {
            break;
        }
    }
    y = 26+(showplace-MenuNum+place)*22;
    if( ( (y ) > 25 )&&( (y ) <137 ))	//在显示范围内
    {

        if(language)
        {
            DisCList16(120,(y),     "[Open  Clos]");	//改成
        }
        else
        {
            DisCList16(120,(y),     "[打开  关闭]");	//改成
        }
        area[0] = 128+OnOff*40;
        area[1] = y;//26+(place-1)*22;
        area[2] = 128+5*8+OnOff*40;
        area[3] = y+15;//41+(place-1)*22;
        CReverseVideo(area);

    }

}

FanHei2(uint8 showplace,uint8 place)
{
    uint8 MenuNum;
    uint16 y,i;
    y = 26+22*showplace;
    for(i = 2; i<8; i++)
    {
        MenuNum = ((MenuId&0xffffff00)>>i*4)&0x0f;
        if(MenuNum)
        {
            break;
        }
    }
    y = 26+(showplace-MenuNum+place)*22;
    if( ( (y ) > 25 )&&( (y ) <137 ))	//在显示范围内
    {
        DisCList16(120,(y),     "            ");	//改成
    }

}


void DisConfigState(void)
{
    uint16 place;
    place=(MenuId&0x000000f0)>>4;

    switch(MenuId&0xffffff0f)
    {
    case 0x02100000:
    case 0x02200000:
    case 0x02400000:
    case 0x02300000:
    case 0x02500000:
    case 0x02600000:
    case 0x02700000:
    case 0x02800000:
        DisConfigStateSwitch2(PumpFlg,0,place);
        DisConfigStateSwitch2(!ClampMotor,1,place);
        DisConfigStateSwitch2(HeatFlg,3,place);
        DisConfigStateSwitch2(ClampRadio,7,place);
        FanHei2(2,place);
        break;
    case 0x04400000:
        FanHei2(2,place);
        break;
    }

    if(keyEnterFlg==0)
    {
        switch(MenuId&0xffffff0f)
        {
            //设置界面
        case 0x05100000:
        case 0x05200000:
        case 0x05300000:
        case 0x05400000:
        case 0x05500000:
        case 0x05600000:
        case 0x05700000:
        case 0x05800000:
        case 0x05900000:
        case 0x05a00000:
        case 0x05b00000:
        case 0x05c00000:
            /*
            声音
            对比度
            背光
            语言
            电量 的信息
            */
            DisConfigStateSwitch(keyBuzzerBitFlg,16,1,place);
            DisConfigStateValue(MotorTime,2,place,PARAM_MOTO," S");
            DisConfigStateSwitch(KeyLockFlg,16,3,place);
            DisConfigStateSwitch(language,18,4,place);
            DisConfigStateValue(LCDContrast,5,place,PARAM_DUIBIDU," ");
            DisConfigStateValue(FilterRemain,6,place,PARAM_NOMAL,"%");
            DisConfigStateValue(SensorSel,11,place,PARAM_DUIBIDU," ");
            break;
            /*
            	开发人员选项
            */
        case 0x05a10000:
        case 0x05a20000:
        case 0x05a30000:
        case 0x05a40000:
        case 0x05a50000:
        case 0x05a60000:
            DisConfigStateValue(errtimemax0, 0,place,PARAM_ERR ,    "次");
            DisConfigStateValue((fp32)(int32)Parg_b0, 2,place,PARAM_B ,    "");
            DisConfigStateValue(Parg_k0, 3,place,PARAM_K ,    "");
            DisConfigStateSwitch(UseFor0,68,1,place);
            DisConfigStateSwitch(!ComminiStyle0,71,5,place);
            break;
            /*
            	光散设置
            */
        case 0x05b10000:
        case 0x05b20000:
        case 0x05b30000:
        case 0x05b40000:
            DisConfigStateValue(GS_Cycle,0,place,PARAM_GUANGSAN_CYCLE,"  S");
            DisConfigStateValue(PM25_GS_k,1,place,PARAM_GUANGSAN_PM25_K,    " ");
            DisConfigStateValue(PM10_GS_k,2,place,PARAM_GUANGSAN_PM10_K,    " ");
            DisConfigStateValue(TSP_GS_k,3,place,PARAM_GUANGSAN_TSP_K,    " ");
            break;

            //运行设置里的
        case 0x01100000:
        case 0x01200000:
        case 0x01300000:
        case 0x01400000:
        case 0x01500000:
        case 0x01600000:
        case 0x01700000:
        case 0x01800000:
            /*
            运行模式
            湿度门阀
            最大加热温度
            最小加热功率
            采样流量
            采样周期
            //实时模式设置
            高压设置
            */
            DisConfigStateSwitch(RunStyleFlg,20,0,place);
            DisConfigStateValue(Humidity,1,place,PARAM_SHIDUMENFA,      "   %");
            DisConfigStateValue(MaxHeatTemp,2,place, PARAM_MAXHEAT,     " ℃");
            DisConfigStateValue(MinHeatPower,3,place,PARAM_MINHEATPOWER,"   %");
            DisConfigStateValue(FlowOfSamp, 4,place,PARAM_LIULIANG ,    " L/m");
            DisConfigStateSwitch(HeatOn,16,5,place);
            DisConfigStateSwitch(TempCompensateFlag,16,7,place);
            break;
            //运行设置二级菜单
        case 0x01710000:
        case 0x01720000:
        case 0x01730000:
            DisConfigStateSwitch(TimeType,16,0,place);
            DisConfigStateValue(CycleOfSamp,1,place,PARAM_CYCLEOFSAMP , "   min");
            DisConfigStateValue(Mass, 2,place,PARAM_MASS ,    " ");
            break;

            //校准操作里的。
        case 0x03100000:
        case 0x03200000:
        case 0x03300000:
        case 0x03400000:
        case 0x03500000:
        case 0x03600000:
        case 0x03700000:
        case 0x03800000:
        case 0x03900000:
        case 0x03a00000:
            DisConfigStateValue(BiaoPian,0,place,PARAM_BIAOPIAN_JIAOZHUN, "  ug");
            DisConfigStateValue(CaliSetT1,1,place,PARAM_T1_JIAOZHUN,      " ℃");
            DisConfigStateValue(CaliSetT3,2,place,PARAM_T3_JIAOZHUN,      " ℃");
            DisConfigStateValue(CaliSetT4,3,place,PARAM_T3_JIAOZHUN,      " ℃");	   //add by yxq
            DisConfigStateValue(CaliSetRH,4,place,PARAM_RH_JIAOZHUN,      "   %");
            DisConfigStateValue(CaliSetRH2,5,place,PARAM_RH_JIAOZHUN,      "   %");  //add by yxq
            DisConfigStateValue(CaliSetP,6,place,PARAM_QIYA_JIAOZHUN,    "KPa");
            DisConfigStateValue(TestTimes,7,place,PARAM_TESTTIMES,    "    xS");
            DisConfigStateValue(CaliSetF1,8,place,PARAM_LIULIANG_JIAOZHUN," L/m");
            break;

        default :
            break;
        }
    }

    else
    {
        switch(MenuId&0xffffff0f)
        {
            //设置界面
        case 0x05100000:
        case 0x05200000:
            DisConfigStateSwitch(keyBuzzerBitFlg,16,1,place);
            FanHei(place,0,0);
            break;
        case 0x05300000:
            DisConfigStateValue(MotorTime,2,place,PARAM_MOTO," S");
            FanHei(place,1,PARAM_MOTO);
            break;
        case 0x05400000:
            DisConfigStateSwitch(KeyLockFlg,16,3,place);
            FanHei(place,0,0);
            break;
        case 0x05500000:
            DisConfigStateSwitch(language,18,4,place);
            FanHei(place,0,0);
            break;
        case 0x05600000:
            DisConfigStateValue(LCDContrast,5,place,PARAM_DUIBIDU," ");
            FanHei(place,1,PARAM_DUIBIDU);
            break;
        case 0x05700000:
            DisConfigStateValue(FilterRemain,6,place,PARAM_NOMAL,"%");
            FanHei(place,1,PARAM_NOMAL);
            break;

        case 0x05a10000:
            DisConfigStateValue(errtimemax0, 0,place,PARAM_ERR ,    "次");
            FanHei(place,1,PARAM_ERR);
            break;
        case 0x05a20000:
            DisConfigStateSwitch(UseFor0,68,1,place);
            FanHei(place,0,0);
            break;
        case 0x05a30000:
            DisConfigStateValue((fp32)(int32)Parg_b0, 2,place,PARAM_B ,    "");
            FanHei(place,1,PARAM_B);
            break;
        case 0x05a40000:
            DisConfigStateValue(Parg_k0, 3,place,PARAM_K ,    "");
            FanHei(place,1,PARAM_K);
            break;
        case 0x05a60000:
            DisConfigStateSwitch(!ComminiStyle0,71,5,place);
            FanHei(place,0,0);
            break;
        case 0x05b10000:
            DisConfigStateValue(GS_Cycle,0,place,PARAM_GUANGSAN_CYCLE,"  S");
            FanHei(place,1,PARAM_GUANGSAN_CYCLE);
            break;
        case 0x05b20000:
            DisConfigStateValue(PM25_GS_k,1,place,PARAM_GUANGSAN_PM25_K," ");
            FanHei(place,1,PARAM_GUANGSAN_PM25_K);
            break;
        case 0x05b30000:
            DisConfigStateValue(PM10_GS_k,2,place,PARAM_GUANGSAN_PM10_K," ");
            FanHei(place,1,PARAM_GUANGSAN_PM10_K);
            break;
        case 0x05b40000:
            DisConfigStateValue(TSP_GS_k,3,place,PARAM_GUANGSAN_TSP_K," ");
            FanHei(place,1,PARAM_GUANGSAN_TSP_K);
            break;
        case 0x05c00000:
            DisConfigStateValue(SensorSel,11,place,PARAM_SENSOR," ");
            FanHei(place,1,PARAM_SENSOR);
            break;

        case 0x05800000:
        case 0x05900000:
            /*
            声音
            对比度
            背光
            语言
            电量 的信息
            */
            break;

            //运行设置里的。
        case 0x01100000:
            DisConfigStateSwitch(RunStyleFlg,20,0,place);
            FanHei(place,0,0);
            break;
        case 0x01200000:
            DisConfigStateValue(Humidity,1,place,PARAM_SHIDUMENFA ,     "   %");
            FanHei(place,1,PARAM_SHIDUMENFA);
            break;
        case 0x01300000:
            DisConfigStateValue(MaxHeatTemp,2,place,PARAM_MAXHEAT,      " ℃");
            FanHei(place,1,PARAM_MAXHEAT);
            break;
        case 0x01400000:
            DisConfigStateValue(MinHeatPower,3,place,PARAM_MINHEATPOWER,"   %");
            FanHei(place,1,PARAM_MINHEATPOWER);
            break;
        case 0x01500000:
            DisConfigStateValue(FlowOfSamp, 4,place,PARAM_LIULIANG ,    " L/m");
            FanHei(place,1,PARAM_LIULIANG);
            break;
        case 0x01600000:
            DisConfigStateSwitch(HeatOn,16,5,place);
            FanHei(place,0,0);
            break;
        case 0x01800000:
            DisConfigStateSwitch(TempCompensateFlag,16,7,place);
            FanHei(place,0,0);
            break;
            /*
            运行模式
            湿度门阀
            最大加热温度
            最小加热功率
            采样流量
            采样周期
            //实时模式设置
            高压设置
            */
            //运行设置二级菜单
        case 0x01710000:
            DisConfigStateSwitch(TimeType,16,0,place);
            FanHei(place,0,0);
            break;
        case 0x01720000:
            DisConfigStateValue(CycleOfSamp,1,place,PARAM_CYCLEOFSAMP , "   min");
            FanHei(place,1,PARAM_CYCLEOFSAMP);
            break;
        case 0x01730000:
            DisConfigStateValue(Mass, 2,place,PARAM_MASS ,    " ");
            FanHei(place,1,PARAM_MASS);
            break;

            //校准操作里的。
        case 0x03100000:
            DisConfigStateValue(BiaoPian,0,place,PARAM_BIAOPIAN_JIAOZHUN,"  ug");
            FanHei(place,1,PARAM_BIAOPIAN_JIAOZHUN);
            break;
        case 0x03200000:
            DisConfigStateValue(CaliSetT1,1,place,PARAM_T1_JIAOZHUN," ℃");
            FanHei(place,1,PARAM_T1_JIAOZHUN);
            break;
        case 0x03300000:
            DisConfigStateValue(CaliSetT3,2,place,PARAM_T3_JIAOZHUN," ℃");
            FanHei(place,1,PARAM_T3_JIAOZHUN);
            break;
        case 0x03400000:
            DisConfigStateValue(CaliSetT4,3,place,PARAM_T3_JIAOZHUN," ℃");
            FanHei(place,1,PARAM_T3_JIAOZHUN);
            break;
        case 0x03500000:
            DisConfigStateValue(CaliSetRH,4,place,PARAM_RH_JIAOZHUN,"   %");
            FanHei(place,1,PARAM_RH_JIAOZHUN);
            break;
        case 0x03600000:
            DisConfigStateValue(CaliSetRH2,5,place,PARAM_RH_JIAOZHUN,"   %");
            FanHei(place,1,PARAM_RH_JIAOZHUN);
            break;
        case 0x03700000:
            DisConfigStateValue(CaliSetP,6,place,PARAM_QIYA_JIAOZHUN,"KPa");
            FanHei(place,1,PARAM_QIYA_JIAOZHUN);
            break;
        case 0x03800000:
            DisConfigStateValue(TestTimes,7,place,PARAM_TESTTIMES," xS");
            FanHei(place,1,PARAM_TESTTIMES);
            break;
        case 0x03900000:
            DisConfigStateValue(CaliSetF1,8,place,PARAM_LIULIANG_JIAOZHUN," L/m");
            FanHei(place,1,PARAM_LIULIANG_JIAOZHUN);
            break;
        default :
            break;
        }
    }
}

void WriteMyWord1212(uint8 x,uint8 y,uint8 const dat[40])
{
    uint16 i=0;
    unsigned char c1,c2,j;
    const uint8 black[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                           0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                          };
    for(i=0; i<40;)
    {
        c1=dat[i];      //取字符串的ASCII码
        c2=dat[i+1];

        if(0==c1)
        {
            break;
        }
        else if(c1<=128)
        {
            DisBuf126(x+i*6,y,hzk126k1[c1]);
            i++;
        }
        else
        {
            for(j=0; j<WordNum; j++) //找到汉字
            {
                if(c1 == GB_16[j].Index[0])
                {
                    if(c2==GB_16[j].Index[1])
                    {
                        break;            //汉字找到则跳出
                    }
                }
            }
            if(j<WordNum)   //------如果汉字在子码表内
            {
                DisBuf1212(x+i*6,y,GB_16[j].Msk);
                i++;
                i++;
            }
            else             //-----没找到对应汉字则涂黑
            {
                DisBuf1212(x+i*6,y,black);
                i++;
                i++;
            }
        }
    }
}
void WriteCode1212(uint8 x,uint8 y,uint8 const dat[40])
{
    uint8 i,err;
    uint8 const *p;
    uint16 temp;
    uint8	buffer[32]= {0};
    p=dat;

    for(i=0; i<53; p++)
    {
        if(0==*p)
        {
            break;
        }
        else if(*p<=128)
        {
            DisBuf126(x+i*6,y,hzk126k1[*p]);
            i++;
        }
        else
        {
            temp=(*p-0xb0)*94;
            p++;
            temp=temp+(*p-0xa1);
            OSMutexPend(OSFlashUsedMutex,0,&err);
            SSTF016B_RD(FLASH_ADD_ZK1212+24*temp, 24,&buffer[0]);
            OSMutexPost(OSFlashUsedMutex);
            DisBuf1212(x+i*6,y,buffer);
            i++;
            i++;
        }
    }

}
void ShowCoord(void)
{   uint32 i,d;
    DarwHLine(0,239,128); //x=128的横坐标
    DarwVLine(5,30,128); //y=40的竖线
    for(i=0; i<23; i++)
    {
        d=LcdReadByte(5+i*10,129);
        LcdWriteByte(5+i*10,129,d|0x01);
    }
    for(i=0; i<10; i++)
    {
        d=LcdReadByte(6,(128-i*10));
        LcdWriteByte(6,(128-i*10),d|0x01);
    }
    DisCList12(5,30,"uA");
    DisCList12(220,114,"mv");


}

//void Disppb_En1212(uint8 x,uint8 y)
//{
//	DisBuf1212(x,y,hanzi1212[11]);
//	DisCList12(x+12,y,"g");//μg
//	DisCList12(x+24,y,"/");
//	DisCList12(x+30,y,"L");
////	DisBuf1212(x,y,hanzi1212[7]); //升
//}


void Disppb_En2412(uint8 x,uint8 y)
{
    DisBuf2412(x,y,hanzi2412[0]);  //u
    DisEList2412(x+12,y,"g/L");
//	DisBuf1212(x,y,hanzi1212[7]); //升
}
void DisUg_M3_En2412(uint8 x,uint8 y)
{
    DisBuf2412(x,y,hanzi2424[2]);  //u g
    DisEList2412(x+12,y,"g/");
    DisBuf2412(x+12*3,y,hanzi2424[0]);  //u g
    DisBuf2412(x+12*4,y,hanzi2424[1]);  //u g
}

void Disppb_En1616(uint8 x,uint8 y)
{
    DisBuf1616(x,y,hanzi1616[0]);  //ug
    DisCList16(x+16,y,"/L");
//	DisBuf1212(x,y,hanzi1212[7]); //升
}
//void Disppb_cn1212(uint8 x,uint8 y)
//{
//	DisBuf1212(x,y,hanzi1212[9]);
//	DisBuf1212(x+12,y,hanzi1212[5]);//微克
//	DisCList12(x+24,y,"/");
//	DisBuf1212(x+30,y,hanzi1212[7]); //升
//}
