

#include "SPI.H"

void SPIInit()
{
	unsigned char tmpchar[2] = {0, 0};
	PINSEL_CFG_Type PinCfg;
	__IO FlagStatus exitflag;
//	/*
//	 * Initialize SPI pin connect
//	 * P0.8 - WP;
//	 * P0.9 - SO - used as GPIO
//	 * P0.16 - SCK
//	 * P0.15 - SI
//	 * P2.8 - EC CE
//	 * P2.9 - HOLD
//	 */	


			  
//	PinCfg.Funcnum = 0;
//	PinCfg.OpenDrain = 0;
//	PinCfg.Pinmode = 0;
//	PinCfg.Portnum = 0;
//	PinCfg.Pinnum = 15;
//	PINSEL_ConfigPin(&PinCfg);
//	PinCfg.Pinnum = 9;
//	PINSEL_ConfigPin(&PinCfg);
//	PinCfg.Pinnum = 8;
//	PINSEL_ConfigPin(&PinCfg);
//	PinCfg.Pinnum = 16;
//	PinCfg.Funcnum = 0;
//	PINSEL_ConfigPin(&PinCfg);
//	PinCfg.Pinnum = 8;
//	PinCfg.Portnum = 2;
//	PINSEL_ConfigPin(&PinCfg);
//	PinCfg.Pinnum = 9;
//	PinCfg.Portnum = 2;
//	PINSEL_ConfigPin(&PinCfg);




	PinCfg.Funcnum = 0;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 1;
	PinCfg.Pinnum = 4;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 8;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 9;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 10;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 14;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 15;
	PINSEL_ConfigPin(&PinCfg);

 	GPIO_SetDir(1, SCK|SI|WP|CE|HOLD, 1);
 	GPIO_SetDir(1, SO, 0);
	SET_CE(1);
	SET_HOLD(1);

	SET_WP(1);
	SET_SCK(0);

}
volatile void DelayAD(int32_t ulTime)
{
	while (ulTime--);
}
void Send_Byte(uint8 dat)
{
	char i;
	for(i=0;i<8;i++)
	{
		if(dat&0x80)
		{
		 	SET_SI(1);
		}
		else
		{
			SET_SI(0);
		}

//		DelayAD(50);
		SET_SCK(1);
//		DelayAD(50);

		dat <<=1  ;
		SET_SCK(0);

	}
}


uint8 Get_Byte(void)
{
	uint8 i=0,in=0,temp = 0;
	for(i=0;i<8;i++)
	{
		in = in<<1;
		if(GPIO_ReadValue(1)&SO)
		{
			in |= 0x01;	
		}
//		DelayAD(50);
		SET_SCK(1);
//		DelayAD(50);
		SET_SCK(0);
	}
	return in; 
 
}
uint8 SSTF016B_RD(uint32 Dst, uint32 NByte,uint8* RcvBufPt)
{
	uint32 i = 0;
	if ((Dst+NByte > MAX_ADDR)||(NByte == 0))	return (0);	 //	检查入口参数
	
	SET_CE(0);
	Send_Byte(0x0B); 						// 发送读命令
	Send_Byte(((Dst & 0xFFFFFF) >> 16));	// 发送地址信息:该地址由3个字节组成
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	Send_Byte(0xFF);						// 发送一个哑字节以读取数据
	for (i = 0; i < NByte; i++)		
	{
		RcvBufPt[i] = Get_Byte();		
	}
	SET_CE(1);			
	return (1);
}


uint8 SSTF016B_WR(uint32 Dst,uint8 const SndbufPt[], uint32 NByte)
{
	uint8 temp = 0,i = 0,StatRgVal = 0;
	if (( (Dst+NByte-1 > MAX_ADDR)||(NByte == 0) ))
	{
		return (ERROR);	 //	检查入口参数
	}
   	SET_CE(0);			 
	Send_Byte(0x05);							// 发送读状态寄存器命令
	temp = Get_Byte();							// 保存读得的状态寄存器值
	SET_CE(1);								

	SET_CE(0);			
	Send_Byte(0x50);							// 使状态寄存器可写
	SET_CE(1);			
	SET_CE(0);			
	Send_Byte(0x01);							// 发送写状态寄存器指令
	Send_Byte(0);								// 清0BPx位，使Flash芯片全区可写 
	SET_CE(1);			


		
	for(i = 0; i < NByte; i++)
	{
  		SET_CE(0);			
		Send_Byte(0x06);						// 发送写使能命令
		SET_CE(1);			


		SET_CE(0);			
		Send_Byte(0x02); 						// 发送字节数据烧写命令

		Send_Byte((((Dst+i) & 0xFFFFFF) >> 16));// 发送3个字节的地址信息 
		Send_Byte((((Dst+i) & 0xFFFF) >> 8));
		Send_Byte((Dst+i) & 0xFF);

		Send_Byte(SndbufPt[i]);					// 发送被烧写的数据
		SET_CE(1);			
		do
		{
		  	SET_CE(0);			 
			Send_Byte(0x05);					// 发送读状态寄存器命令
			StatRgVal = Get_Byte();				// 保存读得的状态寄存器值
			SET_CE(1);								
  		}
		while (StatRgVal == 0x03);				// 一直等待，直到芯片空闲
	
	}


	SET_CE(0);			
	Send_Byte(0x06);							// 发送写使能命令
	SET_CE(1);			

	SET_CE(0);			
	Send_Byte(0x50);							// 使状态寄存器可写
	SET_CE(1);			
	SET_CE(0);			
	Send_Byte(0x01);							// 发送写状态寄存器指令
	Send_Byte(temp);							// 恢复状态寄存器设置信息 
	SET_CE(1);

	return (1);		
}
char flash_chip_erase(void)  
{  
	uint8  temp1 = 0,temp2 = 0,StatRgVal = 0;
    uint32 SecnHdAddr = 0;	  			
	uint32 no_SecsToEr = 0;				   			// 要擦除的扇区数目
	uint32 CurSecToEr = 0;	  						// 当前要擦除的扇区号
  SET_CE(0);			 
	Send_Byte(0x05);								// 发送读状态寄存器命令
	temp1 = Get_Byte();								// 保存读得的状态寄存器值
	SET_CE(1);								

	SET_CE(0);			
	Send_Byte(0x50);								// 使状态寄存器可写
	SET_CE(1);			
	SET_CE(0);								  	
	Send_Byte(0x01);								// 发送写状态寄存器指令
	Send_Byte(0);									// 清0BPx位，使Flash芯片全区可写 
	SET_CE(1);
	
	SET_CE(0);			
	Send_Byte(0x06);								// 发送写使能命令
	SET_CE(1);			
	
	
		SET_CE(0);			
		Send_Byte(0x60);							// 发送芯片擦除指令(60h or C7h)
		SET_CE(1);			
		do
		{
		  	SET_CE(0);			 
			Send_Byte(0x05);						// 发送读状态寄存器命令
			StatRgVal = Get_Byte();					// 保存读得的状态寄存器值
			SET_CE(1);								
  		}
		while (StatRgVal == 0x03);					// 一直等待，直到芯片空闲
   		return (1);
}

 uint8 MX25L1602_RdID(uint8 IDType, uint32* RcvbufPt)
{
	uint32 temp = 0;
	if (IDType == 1)
	{
		  	SET_CE(0);			 
		Send_Byte(0x9F);		 		// 发送读JEDEC ID命令(9Fh)
    	temp = (temp | Get_Byte()) << 8;// 接收数据 
		temp = (temp | Get_Byte()) << 8;	
		temp = (temp | Get_Byte()); 	// 在本例中,temp的值应为0xBF2541
			SET_CE(1);								
		*RcvbufPt = temp;
		return (1);
	}
	
	if ((IDType == 0) || (IDType == 2) )
	{
		  	SET_CE(0);			 
		Send_Byte(0x90);				// 发送读ID命令 (90h or ABh)
    	Send_Byte(0x00);				// 发送地址
		Send_Byte(0x00);				// 发送地址
		Send_Byte(IDType);				// 发送地址 - 不是00H就是01H
		temp = Get_Byte();				// 接收获取的数据字节
			SET_CE(1);								
		*RcvbufPt = temp;
		return (1);
	}
	else
	{
		return (0);	
	}
}


