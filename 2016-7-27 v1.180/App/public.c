/****************************************Copyright (c)****************************************************
**                            		skyray-instrument Co.,LTD.
**
**                                 http://www.skyray-instrument.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           public.c
** Last modified Date:  2011-02-24
** Last Version:        V1.0
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Created By:          方卫龙
** Created date:        2011-02-24
** Version:             V1.0
** Descriptions:        First version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         方卫龙
** Modified date:       2011-02-24
** Version:             V1.X
** Descriptions:
**
*********************************************************************************************************/
#include "public.h"
volatile uint16 V_for_I_max;
/*********************************************************************************************************
* Function name:        Delay10uS
* Descriptions:         小延时函数(只有5)
* input parameters:     dly
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
volatile void Delay100uS(uint32 dly)
{
    uint32 i;

    if (0 == dly)
    {
        return ;
    }

    i = SystemCoreClock/100000*dly;
    while (i--);
}

/*********************************************************************************************************
* Function name:        DelayuS
* Descriptions:         小延时函数
* input parameters:     dly
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
volatile void DelayuS(uint32 dly)
{
    uint32 i;

    i = SystemCoreClock/1000000*dly;
    while (i--);
}

/*********************************************************************************************************
* Function name:        Delay
* Descriptions:         小延时函数
* input parameters:     dly
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
volatile void Delay(uint32 dly)
{
    uint32 i;

    i = 0;
    while (dly--) {
        for (i = 0; i < 5000; i++);
    }
}

/*********************************************************************************************************
* Function name:        Uint16ToMod
* Descriptions:         modbus用，字变字节
* input parameters:     *arr		字节指针
*						dat			要变化的字
* output parameters:    无
* Returned value:       无
*********************************************************************************************************/
void Uint16ToMod(uint8 *arr,uint16 dat)
{
    *(arr++)=(uint8)(dat>>8);
    *(arr++)=(uint8)dat;
}

/*********************************************************************************************************
* Function name:        ModToUint16
* Descriptions:         modbus用，字节变字
* input parameters:     *arr		字节指针
* output parameters:    无
* Returned value:       字
*********************************************************************************************************/
uint16 ModToUint16(uint8 *arr)
{
    uint16 rDat;
    rDat = *(arr+1)+*(arr )<<8;
    return rDat;
}


/*********************************************************************************************************
* Function name:        LongIntegerToBit
* Descriptions:         长字变字节
* input parameters:     ulSource	长字
* output parameters:    *pTarget	字节数组
* Returned value:       0
*********************************************************************************************************/
uint8 LongIntegerToBit(uint32 ulSource,uint8 *pTarget)
{
    uint8  i;
    uint32 j=1000000000;

    //[0]~[8];
    for(i=0; i<=8; i++)
    {
        *pTarget=ulSource/j;
        ulSource %=j;
        pTarget++;
        j /=10;
    }
    //[9];
    *pTarget=ulSource%10;
    return 0;
}

/*********************************************************************************************************
* Function name:  IntegerToBitAscii
* Descriptions:    整形变为ascii值
* input parameters: uint16 dat,uint8 *pArr  整形  对应的ascii
* output parameters:     无
* Returned value:          无
*********************************************************************************************************/
uint8 IntegerToBitAscii(uint32 dat,uint8 *pArr)
{
    if(dat<10)
    {
        *pArr = (uint8)dat+'0';
        return 1;
    }
    else if(dat<100)
    {
        *pArr = dat/10+'0';
        pArr ++;
        *pArr = dat%10+'0';
        return 2;
    }
    else if(dat<1000)
    {
        *pArr = dat/100+'0';
        pArr ++;
        *pArr = dat%100/10+'0';
        pArr ++;
        *pArr = dat%10+'0';
        return 3;
    }
    else if(dat<10000)
    {
        *pArr = dat/1000+'0';
        pArr ++;
        *pArr = dat/100%10+'0';
        pArr ++;
        *pArr = dat/10%10+'0';
        pArr ++;
        *pArr = dat%10+'0';
        return 4;
    }
    return 0;
}
/*********************************************************************************************************
* Function name:   disSeondTime
* Descriptions:    将时间分开放入数组
* input parameters: uint16 dat,uint8 *pArr  时间  数组
* output parameters:     无
* Returned value:          无
*********************************************************************************************************/
void disSeondTime(uint16 dat,uint8 *pArr)
{
    uint8 i,j;

    if(dat>3600)
    {
        dat = 3600;
    }

    i = dat/60;
    j = dat%60;
    *pArr = i/10+'0';
    pArr ++;
    *pArr = i%10+'0';
    pArr ++;
    *pArr = ':';
    pArr ++;
    *pArr = j/10+'0';
    pArr ++;
    *pArr = j%10+'0';
    pArr ++;
    *pArr = 0;
}

/*********************************************************************************************************
* Function name:        FloatToChArray
* Descriptions:         浮点变字节
* input parameters:     Source	浮点数
* output parameters:    *Target	字节数组
* Returned value:       无
*********************************************************************************************************/
void FloatToChArray(fp32 Source,uint8 *Target)
{
    uint8 *p;
    union FloatChar
    {
        fp32 ft;
        uint8 ta[4];
    } fc;
    fc.ft=Source;
    p=&(fc.ta[0]);
    *Target=*p;
    Target++;
    p++;
    *Target=*p;
    Target++;
    p++;
    *Target=*p;
    Target++;
    p++;
    *Target=*p;
    //p++;
}
/*********************************************************************************************************
* Function name:        FloatToChArray
* Descriptions:         浮点变字节
* input parameters:     Source	浮点数
* output parameters:    *Target	字节数组
* Returned value:       无
*********************************************************************************************************/
void Float64ToChArray(fp64 Source,uint8 *Target)
{
    uint8 *p;
    union FloatChar64
    {
        fp64 ft;
        uint8 ta[8];
    } fc64;
    fc64.ft=Source;
    p=&(fc64.ta[0]);
    *Target=*p;
    Target++;
    p++;
    *Target=*p;
    Target++;
    p++;
    *Target=*p;
    Target++;
    p++;
    *Target=*p;
    Target++;
    p++;
    *Target=*p;
    Target++;
    p++;
    *Target=*p;
    Target++;
    p++;
    *Target=*p;
    Target++;
    p++;
    *Target=*p;
    //p++;
}





/*********************************************************************************************************
* Function name:        ChArrayToFloat
* Descriptions:         字节变浮点
* input parameters:     *Source	字节数组
* output parameters:    无
* Returned value:       浮点数
*********************************************************************************************************/
fp32 ChArrayToFloat(uint8 *Source)
{
    uint8 *p;
    union FloatChar
    {
        fp32 ft;
        uint8 ta[4];
    } fc;
    p=Source;
    fc.ta[0]=*p++;
    fc.ta[1]=*p++;
    fc.ta[2]=*p++;
    fc.ta[3]=*p;
    return fc.ft;
}

/*********************************************************************************************************
* Function name:        Int16ArrayToFloat
* Descriptions:         字变浮点
* input parameters:     *Source	字数组
* output parameters:    无
* Returned value:       浮点数
*********************************************************************************************************/
fp32 Int16ArrayToFloat(uint16 *Source)
{
    uint16 *p;
    union FloatChar
    {
        fp32 ft;
        uint8 ta[4];
    } fc;
    p=Source;
    fc.ta[3]=*(p+1)>>8;
    fc.ta[2]=*(p+1);
    fc.ta[1]=*p>>8;
    fc.ta[0]=*p;
    return fc.ft;
}

/*********************************************************************************************************
* Function name:        AverageSelectUint16
* Descriptions:         对字数组排序取中间的平均值
* input parameters:     数组指针，数组长度
* output parameters:    无
* Returned value:       平均值
*********************************************************************************************************/
uint32 AverageSelectUint16(uint16 *dat,uint8 length)
{
    uint8 i,j,k;
    uint32 sum=0;
    uint16 t;
    uint16 *p=dat;

    if(0==length)
    {
        return 0;
    }
    else if(1==length)
    {
        return *p;
    }
    else if(2==length)
    {
        return ((*p)+(*(p+1)))/2;
    }

    else
    {
        for(i=0; i<length-1; i++)
        {
            k=i;
            for(j=i+1; j<length; j++)
                if(*(dat+j)>*(dat+k)) k=j;
            if(k!=i)
            {
                t=*(dat+i);
                *(dat+i)=*(dat+k);
                *(dat+k)=t;
            }
        }

        for(i=length/4; i<(length/4+length/2); i++)
        {

            sum +=*(dat+i);
        }
        return sum/(length/2);
    }
}

/*********************************************************************************************************
* Function name:        AverageSelectFp
* Descriptions:         对字数组排序取中间的平均值
* input parameters:     数组指针，数组长度
* output parameters:    无
* Returned value:       平均值
*********************************************************************************************************/
fp32 AverageSelectFp(fp32 *dat,uint8 length)
{
//   uint8 i;
//   fp32 sum=0;
//   fp32 *p=dat;
//
//	for(i=0;i<length;i++,p++)
//	{
//		sum +=*p;
//	}
//	sum/=(fp32)length;
//	return sum;

    uint8 i,j,k;
    fp32 sum=0;
    fp32 t;

    if(0==length)
    {
        return 0;
    }
    else if(1==length)
    {
        return *dat;
    }
    else if(2==length)
    {
        return ((*dat)+(*(dat+1)))/2;
    }

    else
    {
        for(i=0; i<length-1; i++)
        {
            k=i;
            for(j=i+1; j<length; j++)
                if(*(dat+j)>*(dat+k)) k=j;
            if(k!=i)
            {
                t=*(dat+i);
                *(dat+i)=*(dat+k);
                *(dat+k)=t;
            }
        }

        for(i=length/4; i<(length/4+length/2); i++)
        {
            sum +=*(dat+i);
        }
        return sum/(length/2);
    }
}

/**********************************************************	********
* Function name:        MovingAverage
* Descriptions:         移动平均
* input parameters:     数组指针，数组长度，移动数目
* output parameters:    无
* Returned value:       最大值
****************************************************************************/
fp32 MovingAverage(int16 *dat,uint32 length,uint8 num)
{
    uint8  j;
    uint16 i;
    int32 sum_data;

    V_for_I_max =0;                  //fuck!!!!!

    if(num == 0)//除数不为0
        return 0;
    for(i = 0; i < (length-num) ; i++)
    {
        sum_data = 0;
        for(j = 0; j < num; j++)
        {
            sum_data = sum_data + dat[j + i + 100];			 //200为AD数据的起始地址
        }

        dat[i+num/2+100] = (int16)(sum_data / num);
    }
    /******查找最大绝对值********/
    for(i = 0; i < length-num; i++)
    {
        if( abs(dat[i +100]) > V_for_I_max )
        {
            V_for_I_max =abs(dat[i +100]);
        }
    }
    return(V_for_I_max);
}
/*********************************************************************************************************
* Function name:  LzgCStrlen
* Descriptions:   返回数组长度
* input parameters:  s[] 数组
* output parameters:  无
* Returned value:    数组长度
*********************************************************************************************************/
uint16 LzgCStrlen(uint8 const s[])
{
    uint16 i;
    uint8 const *p;
    for(i=0,p=s; i<0xffff; i++,p++)
    {
        if(0==*p)
        {
            return i;
        }
    }
    return i;
}

/*********************************************************************************************************
* Function name:      LzgChineselen
* Descriptions:       返回汉字字节
* input parameters:    s
* output parameters:
* Returned value:    汉字字符字节数
*********************************************************************************************************/
uint16 LzgChineselen(uint8 const s[])
{
    uint16 i;
    uint8 const *p;
    for(i=0,p=s; i<0xffff; i++,p++)
    {
        if(0==*p)
        {
            if(0==(i%2))
            {
                return i/2;
            }
            else
            {
                return (i+1)/2;
            }
        }
    }
    return i;
}
/*********************************************************************************************************
* Function name:      StringLen
* Descriptions:       返回字符串长度
* input parameters:    s
* output parameters:
* Returned value:    汉字字符字节数
*********************************************************************************************************/
uint16 StringLen(uint8 const s[])
{
    uint16 i;
    uint8 const *p;
    for(i=0,p=s; i<0xffff; i++,p++)
    {

        if(0==*p)
        {
            return i;
        }
        if(*p>128)//汉字
        {
            i++;
            p++;
        }
    }
    return i;
}

/*********************************************************************************************************
* Function name: StrCopyStr
* Descriptions:   复值length长的数组
* input parameters:  uint8 *arr1,  被赋值的数组
					uint8 *arr2,   赋值的数组
					uint8 length
* output parameters:      无
* Returned value:       无
*********************************************************************************************************/
void StrCopyStr (uint8 *arr1,const uint8 *arr2,uint8 length)
{
    uint8 i;

    for(i=0; i<length; i++)
    {
        *(arr1++) = *(arr2++);
    }
}
