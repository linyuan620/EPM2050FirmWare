/****************************************Copyright (c)**************************************************
**                                  广州致远电子有限公司
**
**                                 http://www.21cm.com.cn
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: syshal.c
**创   建   人: 周立山
**最后修改日期: 2006年2月23日
**描        述: Modbus基于MagicARM2410低层驱动移植文件。
**                
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 
** 版  本: 
** 日　期: 
** 描　述: 
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include "config.h"
int8_t 	T35Flag=7;//3.5T标志位 
int8_t 	T15Flag=3;//1.5T标志位
int8_t 	T10msFlag=40;//10ms标志位

uint8_t T15CountEn=0;
uint8_t T35CountEn=0;
uint8_t T10msCountEn=0;
 
///**********************************
//** RS485 收发初始化
//***********************************/
//#define RS_485_S_R		13							// 485发送与接收控制引脚,初始化GPIO
//#define RS_485_INI()	rGPECON = (rGPECON & (~(0x03<<26))) | (0x01<<26);
//						
//		
//#define RS_485_S()		rGPEDAT |= (1<<RS_485_S_R);		// 485发送使能
//#define RS_485_R()		rGPEDAT &= ~(1<<RS_485_S_R);	// 485接收使能
//
/*********************************************************************************************************
** Function name: SendResponse
** Descriptions:  发送帧函数为底层字符发送处理函数，ZLG/MODBUS RTU协议栈中使用该函数发送帧数据。
** Input: buff，字符串指针，类型：unsigned char
**        len，字符长度，类型：unsigned short     
** Output:        无
** Created by:    周立山
** Created Date:  2006-02-21
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified Date: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void SendResponse(uint8 *buff,uint16 len)
{

	UART_Send((LPC_UART_TypeDef *)LPC_UART3, buff, len,BLOCKING);
//	uint16 i,k;
//	RS_485_S();						// RS485发送使能
//
//	for(k=0;k<len;k++)
//	{
//	  while(!(rUTRSTAT1 & 0x02));	// 等待发送器THR为空
//   	  for(i=0; i<10; i++);
//   	  rUTXH1 = buff[k];				// 发送数据
//   	  while(!(rUTRSTAT1 & 0x02));	// 等待发送器THR为空
//	}
//	while(!(rUTRSTAT1 & 0x04));		// 等待发送器THR为空
//	RS_485_R();						// RS485接收使能
}

//#define TIMERS_UP_MAK  (~0x222202)

/*********************************************************************************************************
** Function name: StartCountT15
** Descriptions:  使能T1.5计数。T1.5的时间值为UART传输1.5个字符的时间。
**			      注意：该函数只是使能计时，不允许在函数中产生任何延时。
**			            如果T1.5时间结束执行T15EndHandle函数。
** Input:无       
** Output:        无
** Created by:    周立山
** Created Date:  2006-02-21
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified Date: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void StartCountT15(void)
{
	T15Flag = 3;
	T15CountEn = 1;
	
	TIM_ClearIntPending(LPC_TIM2, TIM_MR2_INT);
	TIM_ResetCounter(LPC_TIM2);
	TIM_Cmd(LPC_TIM2,ENABLE);	//启动定时器。0.5个字符延时

//	uint32 temp = rTCON& TIMERS_UP_MAK;
//	rTCNTB2 = 75;				// 定时750 us
//
//	rTCON = temp|(1<<13);		// 更新定时器数据		
//	rTCON = temp|(1<<12);		// 启动定时器
//
}


/*********************************************************************************************************
** Function name: StartCountT35
** Descriptions:  使能T3.5计数。T3.5的时间值为UART传输3.5个字符的时间。
**			      注意：该函数只是使能计时，不允许在函数中产生任何延时。
**			             如果T3.5时间结束执行T35EndHandle函数。
** Input:无       
** Output:        无
** Created by:    周立山
** Created Date:  2006-02-21
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified Date: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void StartCountT35(void)
{
	T35Flag = 7;
	T35CountEn = 1;
	
	TIM_ClearIntPending(LPC_TIM2, TIM_MR2_INT);
	TIM_ResetCounter(LPC_TIM2);	
	TIM_Cmd(LPC_TIM2,ENABLE);	//启动定时器。0.5个字符延时

//	uint32 temp = rTCON & TIMERS_UP_MAK;
//	rTCNTB3 = 750;				// 定时1750 us
//	
//	rTCON = temp|(1<<17);		// 更新定时器数据		
//	rTCON = temp|(1<<16);		// 启动定时器
	
}


/*********************************************************************************************************
** Function name: Start10mS
** Descriptions:  10mS开始计数,只在Modbus主机使用，从机不使用该中断。该定时器需要自动加载。
** Input:无       
** Output:        无
** Created by:    周立山
** Created Date:  2006-02-21
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified Date: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void Start10mS(void)
{
	T10msFlag = 40;
	T10msCountEn = 1;
	
	TIM_ClearIntPending(LPC_TIM2, TIM_MR2_INT);
	TIM_ResetCounter(LPC_TIM2);	
	TIM_Cmd(LPC_TIM2,ENABLE);	//启动定时器。0.5个字符延时



// 	TIM_Cmd(LPC_TIM1,ENABLE);	//启动定时器

//	uint32 temp = rTCON& TIMERS_UP_MAK;
//	rTCNTB4 = 1000;						// 定时10000 us
//	
//	rTCON = temp|(1<<21);				// 更新定时器数据		
//	rTCON = temp|(1<<20)|(1<<22);		// 启动定时器

}


/*********************************************************************************************************
** Function name: T15_Exception
** Descriptions:  T15定时中断处理
** Input:无       
** Output:        无
** Created by:    周立山
** Created Date:  2006-02-21
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified Date: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
 void T15_Exception(void)
{


//    rSRCPND = BIT_TIMER2;       //Clear pending bit
//    rINTPND = BIT_TIMER2;
//    rINTPND;                    //Prevent an double interrupt pending
//	
//	T15EndHandle();
//	rTCON &= ~(1<<12);		// 关定时器

	T15CountEn = 0;
	T15Flag=3;//1.5T标志位
	T15EndHandle();
// 	TIM_Cmd(LPC_TIM2,DISABLE);	//关定时器2中断
}

/*********************************************************************************************************
** Function name: T35_Exception
** Descriptions:  T35定时中断处理
** Input:无       
** Output:        无
** Created by:    周立山
** Created Date:  2006-02-21
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified Date: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
 void T35_Exception(void)
{
//    rSRCPND = BIT_TIMER3;       //Clear pending bit
//    rINTPND = BIT_TIMER3;
//    rINTPND;                    //Prevent an double interrupt pending
//
//	T35EndHandle();
//	rTCON &= ~(1<<16);		// 关定时器
	T35CountEn = 0;
	T35Flag=7;//3.5T标志位 
	T35EndHandle();
// 	TIM_Cmd(LPC_TIM2,DISABLE);	//关定时器2中断

}

/*********************************************************************************************************
** Function name: T10ms_Exception
** Descriptions:  T10ms定时中断处理
** Input:无       
** Output:        无
** Created by:    周立山
** Created Date:  2006-02-21
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified Date: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
 
//单独用timer1实现。 
void T10ms_Exception(void)
{
       
////    rSRCPND = BIT_TIMER4;       //Clear pending bit
////    rINTPND = BIT_TIMER4;
////    rINTPND;                    //Prevent an double interrupt pending
////
		T10msCountEn = 0;
		T10msFlag=40;//10ms标志位	
		Time10mSHandle();
//		TIM_Cmd(LPC_TIM2,DISABLE);	//关定时器2中断
//
}


/*********************************************************************************************************
** Function name: TimersInit
** Descriptions:  定时器初始化函数,系统启动时调用该函数初始化定时器。
** Input:无       
** Output:        无
** Created by:    周立山
** Created Date:  2006-02-21
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified Date: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void TimersInit(void)
{ 

	Time2Init();						//		Time2Init(0);//0.5个字符延时 MODBUS	  19200 对应350us 为0.5个字符时间  28个及10ms

	// Fclk=200MHz，Pclk=50MHz。
	// 定时器的记数单位为1微秒
//	rTCFG0 &=~(0xff<<8);
//	rTCFG0 |= 250<<8;			// 预分频器0设置为250，取得200KHz
//	rTCFG1 &= ~(0xf<<8);		// TIMER2再取1/2分频，取得100KHz
//	rTCFG1 &= ~(0xf<<12);		// TIMER3再取1/2分频，取得100KHz
//	rTCFG1 &= ~(0xf<<16);		// TIMER4再取1/2分频，取得100KHz
//
//	rTCON &= ~(0x7ff<<12);		// 初始化定时器2、3、4的控制位，全为零
}



/*********************************************************************************************************
** Function name: UART1_Exception
** Descriptions:  串口中断处理函数
** Input:无       
** Output:        无
** Created by:    周立山
** Created Date:  2006-02-21
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified Date: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
//直接在串口中断那边实现
//void  UART1_Exception(void)
//{


//	rINTSUBMSK|=(BIT_SUB_RXD1|BIT_SUB_ERR1);
//
//	if(rUTRSTAT1 & 0x01)			
//	{
//
//		ReceOneChar(rURXH1);		// 调用MODBUS接收字符函数，;
//	}
//
// 	ClearPending(BIT_UART1);
//   rSUBSRCPND =(BIT_SUB_RXD1|BIT_SUB_ERR1);
//   rINTSUBMSK &=~(BIT_SUB_RXD1|BIT_SUB_ERR1);
    
//}



/*********************************************************************************************************
** Function name: IniUART1
** Descriptions:  串口初始化。系统起动时调用该函数初始化串口
** Input:	bps ,串口波特率    
** Output:        无
** Created by:    周立山
** Created Date:  2006-02-21
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified Date: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void IniUART1(uint32 bps)
{  	
 	Uart3Init(bps,UART_PARITY_NONE,UART_STOPBIT_1);  //MODBUS


//	RS_485_INI();
//	// IO口设置 (GPH5,GPH4)
//	rGPHUP = rGPHUP | (0x03<<4);
//	rGPHCON = (rGPHCON & (~0x00000F00)) | (0x00000A00);	
//	
//	// 串口模式设置
//	rUFCON1 = 0x00;   	// 禁止FIFO功能
//	rUMCON1 = 0x00;   	// AFC(流控制)禁能
//	rULCON1 = 0x07; 	// 禁止IRDA，无奇偶校验，2位停止位，8位数据位
//	rUCON1  = 0x105; 	// 使用PCLK来生成波特率，发送中断为电平触发模式，接收中断为边沿触发模式，
//	                    // 禁止接收超时中断，使能接收错误中断，正常工作模式，中断或查询方式(非DMA)	
//	// 串口波特率设置
//	rUBRDIV1=(int)(PCLK/16.0/bps + 0.5) -1; 


} 
