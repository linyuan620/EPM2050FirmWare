/****************************************Copyright (c)**************************************************
**                               Guangzou ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.zlgmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			SST25VF016BDrv.c
** Descriptions:		SPI下的SST25VF016B操作函数库 
**
**------------------------------------------------------------------------------------------------------
** Created by:			Litiantian
** Created date:		2007-04-16
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
//#include 	"config.h"
#include   "SST25VF016BDrv.h"
/* 移植本软件包时需要修改以下的函数或宏 */
#define		CE_Low()		(FIO0CLR |=  F016B_CS)
#define     CE_High()		(FIO0SET |=  F016B_CS)			

/* SPI初始化 */
/*******************************************************************************************
** 函数名称：SPIInit															
** 功能说明：初始化SPI。				
** 入口参数：无																
** 出口参数：无																
*******************************************************************************************/
void  SPIInit(void)
{  
	uint8_t tmpchar[2] = {0, 0};
	PINSEL_CFG_Type PinCfg;
	__IO FlagStatus exitflag;
	/*
	 * Initialize SPI pin connect
	 * P0.15 - SCK;
	 * P0.16 - SSEL - used as GPIO
	 * P0.17 - MISO
	 * P0.18 - MOSI
	 */
	
	PinCfg.Funcnum = 3;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 15;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 17;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 18;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 16;
	PinCfg.Funcnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	
	
	
	
	
	
	
	
	
	
	
	
//	/* 设置SST25VF016B的控制引脚 */
////	PINSEL0 	=	0;		
////   	FIO2DIR 	|=  	F016B_CS;
//	/* 设置硬件SPI的通讯脚	*/ 
////   	PINSEL0	|= 	0xc0000000;				// 设置P0.15脚为SCK脚
////   	PINSEL1	=	(0x03 << 2) | (0x03 << 4);	// 设置P0.17、P0.18引脚为SPI引脚
//			               
//   	S0SPCCR =	0x40;		       		// 设置SPI时钟分频,可按需求配置			
// 	/* 设置SPI的工作方式 */ 
// 	S0SPCR  = 	(0 << 2) |				// SPI控制器每次传输发送和接收8位数据。
// 				(0 << 3) |				// CPHA = 0, 数据在SCK 的第一个时钟沿采样
// 				(0 << 4) |				// CPOL = 0, SCK 为低有效
// 				(1 << 5) |				// MSTR = 1, SPI 处于主模式
// 				(0 << 6) |				// LSBF = 0, SPI 数据传输MSB (位7)在先
// 				(0 << 7);				// SPIE = 0, SPI 中断被禁止
}

/************************************************************************
** 函数名称: Send_Byte													
** 函数功能：通过硬件SPI发送一个字节到SST25VF016B					
** 入口参数:data															
** 出口参数:无																
************************************************************************/
void Send_Byte(uint8 data)
{
    S0SPDR = data;
    while ( 0 == (S0SPSR & 0x80));			// 等待SPIF置位，即等待数据发送完毕
	data = S0SPSR;								// 清除SPIF标志
}

/************************************************************************
** 函数名称:Get_Byte														
** 函数功能:通过硬件SPI接口接收一个字节到处理器						
** 入口参数:无																
** 出口参数:无																
************************************************************************/
uint8 Get_Byte(void)
{
    uint8 temp;
	S0SPDR = 0xff;							// 发送该数据用以产生时钟,数据本身没有用处
    while ( 0 == (S0SPSR & 0x80));			// 等待SPIF置位，即等待数据发送完毕
    temp = S0SPSR;								// 清除SPIF标志
    temp=temp;
    return (uint8)(S0SPDR);					// 返回接收到的数据
}
 
/* 以下函数在移植时无需修改 */
/************************************************************************
** 函数名称:SSTF016B_RD																										
** 函数功能:SST25VF016B的读函数,可选择读ID和读数据操作				
** 入口参数:
**			uint32 Dst：目标地址,范围 0x0 - MAX_ADDR（MAX_ADDR = 0x1FFFFF）				
**      	uint32 NByte:	要读取的数据字节数
**			uint8* RcvBufPt:接收缓存的指针			
** 出口参数:操作成功则返回OK,失败则返回ERROR		
** 注	 意:若某功能下,某一入口参数无效,可在该入口参数处填Invalid，该参数将被忽略
************************************************************************/
uint8 SSTF016B_RD(uint32 Dst, uint32 NByte,uint8* RcvBufPt)
{
	uint32 i = 0;
	if ((Dst+NByte > MAX_ADDR)||(NByte == 0))	return (ERROR);	 //	检查入口参数
	
	CE_Low();			
	Send_Byte(0x0B); 						// 发送读命令
	Send_Byte(((Dst & 0xFFFFFF) >> 16));	// 发送地址信息:该地址由3个字节组成
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	Send_Byte(0xFF);						// 发送一个哑字节以读取数据
	for (i = 0; i < NByte; i++)		
	{
		RcvBufPt[i] = Get_Byte();		
	}
	CE_High();			
	return (OK);
}

/************************************************************************
** 函数名称:SSTF016B_RdID																										
** 函数功能:SST25VF016B的读ID函数,可选择读ID和读数据操作				
** 入口参数:
**			idtype IDType:ID类型。用户可在Jedec_ID,Dev_ID,Manu_ID三者里选择
**			uint32* RcvbufPt:存储ID变量的指针
** 出口参数:操作成功则返回OK,失败则返回ERROR		
** 注	 意:若填入的参数不符合要求，则返回ERROR
************************************************************************/
uint8 SSTF016B_RdID(idtype IDType, uint32* RcvbufPt)
{
	uint32 temp = 0;
	if (IDType == Jedec_ID)
	{
		CE_Low();			
		Send_Byte(0x9F);		 		// 发送读JEDEC ID命令(9Fh)
    	temp = (temp | Get_Byte()) << 8;// 接收数据 
		temp = (temp | Get_Byte()) << 8;	
		temp = (temp | Get_Byte()); 	// 在本例中,temp的值应为0xBF2541
		CE_High();			
		*RcvbufPt = temp;
		return (OK);
	}
	
	if ((IDType == Manu_ID) || (IDType == Dev_ID) )
	{
		CE_Low();			
		Send_Byte(0x90);				// 发送读ID命令 (90h or ABh)
    	Send_Byte(0x00);				// 发送地址
		Send_Byte(0x00);				// 发送地址
		Send_Byte(IDType);				// 发送地址 - 不是00H就是01H
		temp = Get_Byte();				// 接收获取的数据字节
		CE_High();			
		*RcvbufPt = temp;
		return (OK);
	}
	else
	{
		return (ERROR);	
	}
}

/************************************************************************
** 函数名称:SSTF016B_WR											
** 函数功能:SST25VF016B的写函数，可写1个和多个数据到指定地址									
** 入口参数:
**			uint32 Dst：目标地址,范围 0x0 - MAX_ADDR（MAX_ADDR = 0x1FFFFF）				
**			uint8* SndbufPt:发送缓存区指针
**      	uint32 NByte:要写的数据字节数
** 出口参数:操作成功则返回OK,失败则返回ERROR		
** 注	 意:若某功能下,某一入口参数无效,可在该入口参数处填Invalid，该参数将被忽略
************************************************************************/
uint8 SSTF016B_WR(uint32 Dst, uint8* SndbufPt, uint32 NByte)
{
	uint8 temp = 0,i = 0,StatRgVal = 0;
	if (( (Dst+NByte-1 > MAX_ADDR)||(NByte == 0) ))
	{
		return (ERROR);	 //	检查入口参数
	}
   	CE_Low();			 
	Send_Byte(0x05);							// 发送读状态寄存器命令
	temp = Get_Byte();							// 保存读得的状态寄存器值
	CE_High();								

	CE_Low();			
	Send_Byte(0x50);							// 使状态寄存器可写
	CE_High();			
	CE_Low();			
	Send_Byte(0x01);							// 发送写状态寄存器指令
	Send_Byte(0);								// 清0BPx位，使Flash芯片全区可写 
	CE_High();			

		
	for(i = 0; i < NByte; i++)
	{
		CE_Low();			
		Send_Byte(0x06);						// 发送写使能命令
		CE_High();			

		CE_Low();			
		Send_Byte(0x02); 						// 发送字节数据烧写命令
		Send_Byte((((Dst+i) & 0xFFFFFF) >> 16));// 发送3个字节的地址信息 
		Send_Byte((((Dst+i) & 0xFFFF) >> 8));
		Send_Byte((Dst+i) & 0xFF);
		Send_Byte(SndbufPt[i]);					// 发送被烧写的数据
		CE_High();			

		do
		{
		  	CE_Low();			 
			Send_Byte(0x05);					// 发送读状态寄存器命令
			StatRgVal = Get_Byte();				// 保存读得的状态寄存器值
			CE_High();								
  		}
		while (StatRgVal == 0x03);				// 一直等待，直到芯片空闲
	}

	CE_Low();			
	Send_Byte(0x06);							// 发送写使能命令
	CE_High();			

	CE_Low();			
	Send_Byte(0x50);							// 使状态寄存器可写
	CE_High();			
	CE_Low();			
	Send_Byte(0x01);							// 发送写状态寄存器指令
	Send_Byte(temp);							// 恢复状态寄存器设置信息 
	CE_High();

	return (OK);		
}

/************************************************************************
** 函数名称:SSTF016B_Erase												
** 函数功能:根据指定的扇区号选取最高效的算法擦除								
** 入口参数:
**			uint32 sec1：起始扇区号,范围(0~499)
**			uint32 sec2：终止扇区号,范围(0~499)
** 出口参数:操作成功则返回OK,失败则返回ERROR		
************************************************************************/
uint8 SSTF016B_Erase(uint32 sec1, uint32 sec2)
{
	uint8  temp1 = 0,temp2 = 0,StatRgVal = 0;
    uint32 SecnHdAddr = 0;	  			
	uint32 no_SecsToEr = 0;				   			// 要擦除的扇区数目
	uint32 CurSecToEr = 0;	  						// 当前要擦除的扇区号
	
	/*  检查入口参数 */
	if ((sec1 > SEC_MAX)||(sec2 > SEC_MAX))	
	{
		return (ERROR);	
	}
   	
   	CE_Low();			 
	Send_Byte(0x05);								// 发送读状态寄存器命令
	temp1 = Get_Byte();								// 保存读得的状态寄存器值
	CE_High();								

	CE_Low();			
	Send_Byte(0x50);								// 使状态寄存器可写
	CE_High();			
	CE_Low();								  	
	Send_Byte(0x01);								// 发送写状态寄存器指令
	Send_Byte(0);									// 清0BPx位，使Flash芯片全区可写 
	CE_High();
	
	CE_Low();			
	Send_Byte(0x06);								// 发送写使能命令
	CE_High();			

	/* 如果用户输入的起始扇区号大于终止扇区号，则在内部作出调整 */
	if (sec1 > sec2)
	{
	   temp2 = sec1;
	   sec1  = sec2;
	   sec2  = temp2;
	} 
	/* 若起止扇区号相等则擦除单个扇区 */
	if (sec1 == sec2)	
	{
	    SecnHdAddr = SEC_SIZE * sec1;				// 计算扇区的起始地址
	    CE_Low();	
    	Send_Byte(0x20);							// 发送扇区擦除指令
	    Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // 发送3个字节的地址信息
   		Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
   		Send_Byte(SecnHdAddr & 0xFF);
  		CE_High();			
		do
		{
		  	CE_Low();			 
			Send_Byte(0x05);						// 发送读状态寄存器命令
			StatRgVal = Get_Byte();					// 保存读得的状态寄存器值
			CE_High();								
  		}
		while (StatRgVal == 0x03);					// 一直等待，直到芯片空闲
		return (OK);			
	}
	
/* 		根据起始扇区和终止扇区间距调用最快速的擦除功能 							*/	
	
	if (sec2 - sec1 == SEC_MAX)	
	{
		CE_Low();			
		Send_Byte(0x60);							// 发送芯片擦除指令(60h or C7h)
		CE_High();			
		do
		{
		  	CE_Low();			 
			Send_Byte(0x05);						// 发送读状态寄存器命令
			StatRgVal = Get_Byte();					// 保存读得的状态寄存器值
			CE_High();								
  		}
		while (StatRgVal == 0x03);					// 一直等待，直到芯片空闲
   		return (OK);
	}
	
	no_SecsToEr = sec2 - sec1 +1;					// 获取要擦除的扇区数目
	CurSecToEr  = sec1;								// 从起始扇区开始擦除
	
	/* 若两个扇区之间的间隔够大，则采取16扇区擦除算法 */
	while (no_SecsToEr >= 16)
	{
	    SecnHdAddr = SEC_SIZE * CurSecToEr;			// 计算扇区的起始地址
	    CE_Low();	
	    Send_Byte(0xD8);							// 发送64KB块擦除指令
	    Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // 发送3个字节的地址信息
   		Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
   		Send_Byte(SecnHdAddr & 0xFF);
  		CE_High();			
		do
		{
		  	CE_Low();			 
			Send_Byte(0x05);						// 发送读状态寄存器命令
			StatRgVal = Get_Byte();					// 保存读得的状态寄存器值
			CE_High();								
  		}
		while (StatRgVal == 0x03);					// 一直等待，直到芯片空闲
		CurSecToEr  += 16;							// 计算擦除了16个扇区后,和擦除区域相邻的待擦除扇区号
		no_SecsToEr -=  16;							// 对需擦除的扇区总数作出调整
	}
	/* 若两个扇区之间的间隔够大，则采取8扇区擦除算法 */
	while (no_SecsToEr >= 8)
	{
	    SecnHdAddr = SEC_SIZE * CurSecToEr;			// 计算扇区的起始地址
	    CE_Low();	
	    Send_Byte(0x52);							// 发送32KB擦除指令
	    Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // 发送3个字节的地址信息
   		Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
   		Send_Byte(SecnHdAddr & 0xFF);
  		CE_High();			
		do
		{
		  	CE_Low();			 
			Send_Byte(0x05);						// 发送读状态寄存器命令
			StatRgVal = Get_Byte();					// 保存读得的状态寄存器值
			CE_High();								
  		}
		while (StatRgVal == 0x03);					// 一直等待，直到芯片空闲	
		CurSecToEr  += 8;
		no_SecsToEr -=  8;
	}
	/* 采用扇区擦除算法擦除剩余的扇区 */
	while (no_SecsToEr >= 1)
	{
	    SecnHdAddr = SEC_SIZE * CurSecToEr;			// 计算扇区的起始地址
	    CE_Low();	
    	Send_Byte(0x20);							// 发送扇区擦除指令
	    Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // 发送3个字节的地址信息
   		Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
   		Send_Byte(SecnHdAddr & 0xFF);
  		CE_High();			
		do
		{
		  	CE_Low();			 
			Send_Byte(0x05);						// 发送读状态寄存器命令
			StatRgVal = Get_Byte();					// 保存读得的状态寄存器值
			CE_High();								
  		}
		while (StatRgVal == 0x03);					// 一直等待，直到芯片空闲
		CurSecToEr  += 1;
		no_SecsToEr -=  1;
	}
/*				擦除结束,恢复状态寄存器信息							*/
	CE_Low();			
	Send_Byte(0x06);								// 发送写使能命令
	CE_High();			

	CE_Low();			
	Send_Byte(0x50);								// 使状态寄存器可写
	CE_High();			
	CE_Low();			
	Send_Byte(0x01);								// 发送写状态寄存器指令
	Send_Byte(temp1);								// 恢复状态寄存器设置信息 
	CE_High();    
	return (OK);
}



























