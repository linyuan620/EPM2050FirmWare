

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
	if ((Dst+NByte > MAX_ADDR)||(NByte == 0))	return (0);	 //	�����ڲ���
	
	SET_CE(0);
	Send_Byte(0x0B); 						// ���Ͷ�����
	Send_Byte(((Dst & 0xFFFFFF) >> 16));	// ���͵�ַ��Ϣ:�õ�ַ��3���ֽ����
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	Send_Byte(0xFF);						// ����һ�����ֽ��Զ�ȡ����
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
		return (ERROR);	 //	�����ڲ���
	}
   	SET_CE(0);			 
	Send_Byte(0x05);							// ���Ͷ�״̬�Ĵ�������
	temp = Get_Byte();							// ������õ�״̬�Ĵ���ֵ
	SET_CE(1);								

	SET_CE(0);			
	Send_Byte(0x50);							// ʹ״̬�Ĵ�����д
	SET_CE(1);			
	SET_CE(0);			
	Send_Byte(0x01);							// ����д״̬�Ĵ���ָ��
	Send_Byte(0);								// ��0BPxλ��ʹFlashоƬȫ����д 
	SET_CE(1);			


		
	for(i = 0; i < NByte; i++)
	{
  		SET_CE(0);			
		Send_Byte(0x06);						// ����дʹ������
		SET_CE(1);			


		SET_CE(0);			
		Send_Byte(0x02); 						// �����ֽ�������д����

		Send_Byte((((Dst+i) & 0xFFFFFF) >> 16));// ����3���ֽڵĵ�ַ��Ϣ 
		Send_Byte((((Dst+i) & 0xFFFF) >> 8));
		Send_Byte((Dst+i) & 0xFF);

		Send_Byte(SndbufPt[i]);					// ���ͱ���д������
		SET_CE(1);			
		do
		{
		  	SET_CE(0);			 
			Send_Byte(0x05);					// ���Ͷ�״̬�Ĵ�������
			StatRgVal = Get_Byte();				// ������õ�״̬�Ĵ���ֵ
			SET_CE(1);								
  		}
		while (StatRgVal == 0x03);				// һֱ�ȴ���ֱ��оƬ����
	
	}


	SET_CE(0);			
	Send_Byte(0x06);							// ����дʹ������
	SET_CE(1);			

	SET_CE(0);			
	Send_Byte(0x50);							// ʹ״̬�Ĵ�����д
	SET_CE(1);			
	SET_CE(0);			
	Send_Byte(0x01);							// ����д״̬�Ĵ���ָ��
	Send_Byte(temp);							// �ָ�״̬�Ĵ���������Ϣ 
	SET_CE(1);

	return (1);		
}
char flash_chip_erase(void)  
{  
	uint8  temp1 = 0,temp2 = 0,StatRgVal = 0;
    uint32 SecnHdAddr = 0;	  			
	uint32 no_SecsToEr = 0;				   			// Ҫ������������Ŀ
	uint32 CurSecToEr = 0;	  						// ��ǰҪ������������
  SET_CE(0);			 
	Send_Byte(0x05);								// ���Ͷ�״̬�Ĵ�������
	temp1 = Get_Byte();								// ������õ�״̬�Ĵ���ֵ
	SET_CE(1);								

	SET_CE(0);			
	Send_Byte(0x50);								// ʹ״̬�Ĵ�����д
	SET_CE(1);			
	SET_CE(0);								  	
	Send_Byte(0x01);								// ����д״̬�Ĵ���ָ��
	Send_Byte(0);									// ��0BPxλ��ʹFlashоƬȫ����д 
	SET_CE(1);
	
	SET_CE(0);			
	Send_Byte(0x06);								// ����дʹ������
	SET_CE(1);			
	
	
		SET_CE(0);			
		Send_Byte(0x60);							// ����оƬ����ָ��(60h or C7h)
		SET_CE(1);			
		do
		{
		  	SET_CE(0);			 
			Send_Byte(0x05);						// ���Ͷ�״̬�Ĵ�������
			StatRgVal = Get_Byte();					// ������õ�״̬�Ĵ���ֵ
			SET_CE(1);								
  		}
		while (StatRgVal == 0x03);					// һֱ�ȴ���ֱ��оƬ����
   		return (1);
}

 uint8 MX25L1602_RdID(uint8 IDType, uint32* RcvbufPt)
{
	uint32 temp = 0;
	if (IDType == 1)
	{
		  	SET_CE(0);			 
		Send_Byte(0x9F);		 		// ���Ͷ�JEDEC ID����(9Fh)
    	temp = (temp | Get_Byte()) << 8;// �������� 
		temp = (temp | Get_Byte()) << 8;	
		temp = (temp | Get_Byte()); 	// �ڱ�����,temp��ֵӦΪ0xBF2541
			SET_CE(1);								
		*RcvbufPt = temp;
		return (1);
	}
	
	if ((IDType == 0) || (IDType == 2) )
	{
		  	SET_CE(0);			 
		Send_Byte(0x90);				// ���Ͷ�ID���� (90h or ABh)
    	Send_Byte(0x00);				// ���͵�ַ
		Send_Byte(0x00);				// ���͵�ַ
		Send_Byte(IDType);				// ���͵�ַ - ����00H����01H
		temp = Get_Byte();				// ���ջ�ȡ�������ֽ�
			SET_CE(1);								
		*RcvbufPt = temp;
		return (1);
	}
	else
	{
		return (0);	
	}
}


