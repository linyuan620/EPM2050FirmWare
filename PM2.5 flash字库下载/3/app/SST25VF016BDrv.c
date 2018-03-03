/****************************************Copyright (c)**************************************************
**                               Guangzou ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.zlgmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			SST25VF016BDrv.c
** Descriptions:		SPI�µ�SST25VF016B���������� 
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
/* ��ֲ�������ʱ��Ҫ�޸����µĺ������ */
#define		CE_Low()		(FIO0CLR |=  F016B_CS)
#define     CE_High()		(FIO0SET |=  F016B_CS)			

/* SPI��ʼ�� */
/*******************************************************************************************
** �������ƣ�SPIInit															
** ����˵������ʼ��SPI��				
** ��ڲ�������																
** ���ڲ�������																
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
	
	
	
	
	
	
	
	
	
	
	
	
//	/* ����SST25VF016B�Ŀ������� */
////	PINSEL0 	=	0;		
////   	FIO2DIR 	|=  	F016B_CS;
//	/* ����Ӳ��SPI��ͨѶ��	*/ 
////   	PINSEL0	|= 	0xc0000000;				// ����P0.15��ΪSCK��
////   	PINSEL1	=	(0x03 << 2) | (0x03 << 4);	// ����P0.17��P0.18����ΪSPI����
//			               
//   	S0SPCCR =	0x40;		       		// ����SPIʱ�ӷ�Ƶ,�ɰ���������			
// 	/* ����SPI�Ĺ�����ʽ */ 
// 	S0SPCR  = 	(0 << 2) |				// SPI������ÿ�δ��䷢�ͺͽ���8λ���ݡ�
// 				(0 << 3) |				// CPHA = 0, ������SCK �ĵ�һ��ʱ���ز���
// 				(0 << 4) |				// CPOL = 0, SCK Ϊ����Ч
// 				(1 << 5) |				// MSTR = 1, SPI ������ģʽ
// 				(0 << 6) |				// LSBF = 0, SPI ���ݴ���MSB (λ7)����
// 				(0 << 7);				// SPIE = 0, SPI �жϱ���ֹ
}

/************************************************************************
** ��������: Send_Byte													
** �������ܣ�ͨ��Ӳ��SPI����һ���ֽڵ�SST25VF016B					
** ��ڲ���:data															
** ���ڲ���:��																
************************************************************************/
void Send_Byte(uint8 data)
{
    S0SPDR = data;
    while ( 0 == (S0SPSR & 0x80));			// �ȴ�SPIF��λ�����ȴ����ݷ������
	data = S0SPSR;								// ���SPIF��־
}

/************************************************************************
** ��������:Get_Byte														
** ��������:ͨ��Ӳ��SPI�ӿڽ���һ���ֽڵ�������						
** ��ڲ���:��																
** ���ڲ���:��																
************************************************************************/
uint8 Get_Byte(void)
{
    uint8 temp;
	S0SPDR = 0xff;							// ���͸��������Բ���ʱ��,���ݱ���û���ô�
    while ( 0 == (S0SPSR & 0x80));			// �ȴ�SPIF��λ�����ȴ����ݷ������
    temp = S0SPSR;								// ���SPIF��־
    temp=temp;
    return (uint8)(S0SPDR);					// ���ؽ��յ�������
}
 
/* ���º�������ֲʱ�����޸� */
/************************************************************************
** ��������:SSTF016B_RD																										
** ��������:SST25VF016B�Ķ�����,��ѡ���ID�Ͷ����ݲ���				
** ��ڲ���:
**			uint32 Dst��Ŀ���ַ,��Χ 0x0 - MAX_ADDR��MAX_ADDR = 0x1FFFFF��				
**      	uint32 NByte:	Ҫ��ȡ�������ֽ���
**			uint8* RcvBufPt:���ջ����ָ��			
** ���ڲ���:�����ɹ��򷵻�OK,ʧ���򷵻�ERROR		
** ע	 ��:��ĳ������,ĳһ��ڲ�����Ч,���ڸ���ڲ�������Invalid���ò�����������
************************************************************************/
uint8 SSTF016B_RD(uint32 Dst, uint32 NByte,uint8* RcvBufPt)
{
	uint32 i = 0;
	if ((Dst+NByte > MAX_ADDR)||(NByte == 0))	return (ERROR);	 //	�����ڲ���
	
	CE_Low();			
	Send_Byte(0x0B); 						// ���Ͷ�����
	Send_Byte(((Dst & 0xFFFFFF) >> 16));	// ���͵�ַ��Ϣ:�õ�ַ��3���ֽ����
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	Send_Byte(0xFF);						// ����һ�����ֽ��Զ�ȡ����
	for (i = 0; i < NByte; i++)		
	{
		RcvBufPt[i] = Get_Byte();		
	}
	CE_High();			
	return (OK);
}

/************************************************************************
** ��������:SSTF016B_RdID																										
** ��������:SST25VF016B�Ķ�ID����,��ѡ���ID�Ͷ����ݲ���				
** ��ڲ���:
**			idtype IDType:ID���͡��û�����Jedec_ID,Dev_ID,Manu_ID������ѡ��
**			uint32* RcvbufPt:�洢ID������ָ��
** ���ڲ���:�����ɹ��򷵻�OK,ʧ���򷵻�ERROR		
** ע	 ��:������Ĳ���������Ҫ���򷵻�ERROR
************************************************************************/
uint8 SSTF016B_RdID(idtype IDType, uint32* RcvbufPt)
{
	uint32 temp = 0;
	if (IDType == Jedec_ID)
	{
		CE_Low();			
		Send_Byte(0x9F);		 		// ���Ͷ�JEDEC ID����(9Fh)
    	temp = (temp | Get_Byte()) << 8;// �������� 
		temp = (temp | Get_Byte()) << 8;	
		temp = (temp | Get_Byte()); 	// �ڱ�����,temp��ֵӦΪ0xBF2541
		CE_High();			
		*RcvbufPt = temp;
		return (OK);
	}
	
	if ((IDType == Manu_ID) || (IDType == Dev_ID) )
	{
		CE_Low();			
		Send_Byte(0x90);				// ���Ͷ�ID���� (90h or ABh)
    	Send_Byte(0x00);				// ���͵�ַ
		Send_Byte(0x00);				// ���͵�ַ
		Send_Byte(IDType);				// ���͵�ַ - ����00H����01H
		temp = Get_Byte();				// ���ջ�ȡ�������ֽ�
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
** ��������:SSTF016B_WR											
** ��������:SST25VF016B��д��������д1���Ͷ�����ݵ�ָ����ַ									
** ��ڲ���:
**			uint32 Dst��Ŀ���ַ,��Χ 0x0 - MAX_ADDR��MAX_ADDR = 0x1FFFFF��				
**			uint8* SndbufPt:���ͻ�����ָ��
**      	uint32 NByte:Ҫд�������ֽ���
** ���ڲ���:�����ɹ��򷵻�OK,ʧ���򷵻�ERROR		
** ע	 ��:��ĳ������,ĳһ��ڲ�����Ч,���ڸ���ڲ�������Invalid���ò�����������
************************************************************************/
uint8 SSTF016B_WR(uint32 Dst, uint8* SndbufPt, uint32 NByte)
{
	uint8 temp = 0,i = 0,StatRgVal = 0;
	if (( (Dst+NByte-1 > MAX_ADDR)||(NByte == 0) ))
	{
		return (ERROR);	 //	�����ڲ���
	}
   	CE_Low();			 
	Send_Byte(0x05);							// ���Ͷ�״̬�Ĵ�������
	temp = Get_Byte();							// ������õ�״̬�Ĵ���ֵ
	CE_High();								

	CE_Low();			
	Send_Byte(0x50);							// ʹ״̬�Ĵ�����д
	CE_High();			
	CE_Low();			
	Send_Byte(0x01);							// ����д״̬�Ĵ���ָ��
	Send_Byte(0);								// ��0BPxλ��ʹFlashоƬȫ����д 
	CE_High();			

		
	for(i = 0; i < NByte; i++)
	{
		CE_Low();			
		Send_Byte(0x06);						// ����дʹ������
		CE_High();			

		CE_Low();			
		Send_Byte(0x02); 						// �����ֽ�������д����
		Send_Byte((((Dst+i) & 0xFFFFFF) >> 16));// ����3���ֽڵĵ�ַ��Ϣ 
		Send_Byte((((Dst+i) & 0xFFFF) >> 8));
		Send_Byte((Dst+i) & 0xFF);
		Send_Byte(SndbufPt[i]);					// ���ͱ���д������
		CE_High();			

		do
		{
		  	CE_Low();			 
			Send_Byte(0x05);					// ���Ͷ�״̬�Ĵ�������
			StatRgVal = Get_Byte();				// ������õ�״̬�Ĵ���ֵ
			CE_High();								
  		}
		while (StatRgVal == 0x03);				// һֱ�ȴ���ֱ��оƬ����
	}

	CE_Low();			
	Send_Byte(0x06);							// ����дʹ������
	CE_High();			

	CE_Low();			
	Send_Byte(0x50);							// ʹ״̬�Ĵ�����д
	CE_High();			
	CE_Low();			
	Send_Byte(0x01);							// ����д״̬�Ĵ���ָ��
	Send_Byte(temp);							// �ָ�״̬�Ĵ���������Ϣ 
	CE_High();

	return (OK);		
}

/************************************************************************
** ��������:SSTF016B_Erase												
** ��������:����ָ����������ѡȡ���Ч���㷨����								
** ��ڲ���:
**			uint32 sec1����ʼ������,��Χ(0~499)
**			uint32 sec2����ֹ������,��Χ(0~499)
** ���ڲ���:�����ɹ��򷵻�OK,ʧ���򷵻�ERROR		
************************************************************************/
uint8 SSTF016B_Erase(uint32 sec1, uint32 sec2)
{
	uint8  temp1 = 0,temp2 = 0,StatRgVal = 0;
    uint32 SecnHdAddr = 0;	  			
	uint32 no_SecsToEr = 0;				   			// Ҫ������������Ŀ
	uint32 CurSecToEr = 0;	  						// ��ǰҪ������������
	
	/*  �����ڲ��� */
	if ((sec1 > SEC_MAX)||(sec2 > SEC_MAX))	
	{
		return (ERROR);	
	}
   	
   	CE_Low();			 
	Send_Byte(0x05);								// ���Ͷ�״̬�Ĵ�������
	temp1 = Get_Byte();								// ������õ�״̬�Ĵ���ֵ
	CE_High();								

	CE_Low();			
	Send_Byte(0x50);								// ʹ״̬�Ĵ�����д
	CE_High();			
	CE_Low();								  	
	Send_Byte(0x01);								// ����д״̬�Ĵ���ָ��
	Send_Byte(0);									// ��0BPxλ��ʹFlashоƬȫ����д 
	CE_High();
	
	CE_Low();			
	Send_Byte(0x06);								// ����дʹ������
	CE_High();			

	/* ����û��������ʼ�����Ŵ�����ֹ�����ţ������ڲ��������� */
	if (sec1 > sec2)
	{
	   temp2 = sec1;
	   sec1  = sec2;
	   sec2  = temp2;
	} 
	/* ����ֹ���������������������� */
	if (sec1 == sec2)	
	{
	    SecnHdAddr = SEC_SIZE * sec1;				// ������������ʼ��ַ
	    CE_Low();	
    	Send_Byte(0x20);							// ������������ָ��
	    Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // ����3���ֽڵĵ�ַ��Ϣ
   		Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
   		Send_Byte(SecnHdAddr & 0xFF);
  		CE_High();			
		do
		{
		  	CE_Low();			 
			Send_Byte(0x05);						// ���Ͷ�״̬�Ĵ�������
			StatRgVal = Get_Byte();					// ������õ�״̬�Ĵ���ֵ
			CE_High();								
  		}
		while (StatRgVal == 0x03);					// һֱ�ȴ���ֱ��оƬ����
		return (OK);			
	}
	
/* 		������ʼ��������ֹ��������������ٵĲ������� 							*/	
	
	if (sec2 - sec1 == SEC_MAX)	
	{
		CE_Low();			
		Send_Byte(0x60);							// ����оƬ����ָ��(60h or C7h)
		CE_High();			
		do
		{
		  	CE_Low();			 
			Send_Byte(0x05);						// ���Ͷ�״̬�Ĵ�������
			StatRgVal = Get_Byte();					// ������õ�״̬�Ĵ���ֵ
			CE_High();								
  		}
		while (StatRgVal == 0x03);					// һֱ�ȴ���ֱ��оƬ����
   		return (OK);
	}
	
	no_SecsToEr = sec2 - sec1 +1;					// ��ȡҪ������������Ŀ
	CurSecToEr  = sec1;								// ����ʼ������ʼ����
	
	/* ����������֮��ļ���������ȡ16���������㷨 */
	while (no_SecsToEr >= 16)
	{
	    SecnHdAddr = SEC_SIZE * CurSecToEr;			// ������������ʼ��ַ
	    CE_Low();	
	    Send_Byte(0xD8);							// ����64KB�����ָ��
	    Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // ����3���ֽڵĵ�ַ��Ϣ
   		Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
   		Send_Byte(SecnHdAddr & 0xFF);
  		CE_High();			
		do
		{
		  	CE_Low();			 
			Send_Byte(0x05);						// ���Ͷ�״̬�Ĵ�������
			StatRgVal = Get_Byte();					// ������õ�״̬�Ĵ���ֵ
			CE_High();								
  		}
		while (StatRgVal == 0x03);					// һֱ�ȴ���ֱ��оƬ����
		CurSecToEr  += 16;							// ���������16��������,�Ͳ����������ڵĴ�����������
		no_SecsToEr -=  16;							// �������������������������
	}
	/* ����������֮��ļ���������ȡ8���������㷨 */
	while (no_SecsToEr >= 8)
	{
	    SecnHdAddr = SEC_SIZE * CurSecToEr;			// ������������ʼ��ַ
	    CE_Low();	
	    Send_Byte(0x52);							// ����32KB����ָ��
	    Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // ����3���ֽڵĵ�ַ��Ϣ
   		Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
   		Send_Byte(SecnHdAddr & 0xFF);
  		CE_High();			
		do
		{
		  	CE_Low();			 
			Send_Byte(0x05);						// ���Ͷ�״̬�Ĵ�������
			StatRgVal = Get_Byte();					// ������õ�״̬�Ĵ���ֵ
			CE_High();								
  		}
		while (StatRgVal == 0x03);					// һֱ�ȴ���ֱ��оƬ����	
		CurSecToEr  += 8;
		no_SecsToEr -=  8;
	}
	/* �������������㷨����ʣ������� */
	while (no_SecsToEr >= 1)
	{
	    SecnHdAddr = SEC_SIZE * CurSecToEr;			// ������������ʼ��ַ
	    CE_Low();	
    	Send_Byte(0x20);							// ������������ָ��
	    Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // ����3���ֽڵĵ�ַ��Ϣ
   		Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
   		Send_Byte(SecnHdAddr & 0xFF);
  		CE_High();			
		do
		{
		  	CE_Low();			 
			Send_Byte(0x05);						// ���Ͷ�״̬�Ĵ�������
			StatRgVal = Get_Byte();					// ������õ�״̬�Ĵ���ֵ
			CE_High();								
  		}
		while (StatRgVal == 0x03);					// һֱ�ȴ���ֱ��оƬ����
		CurSecToEr  += 1;
		no_SecsToEr -=  1;
	}
/*				��������,�ָ�״̬�Ĵ�����Ϣ							*/
	CE_Low();			
	Send_Byte(0x06);								// ����дʹ������
	CE_High();			

	CE_Low();			
	Send_Byte(0x50);								// ʹ״̬�Ĵ�����д
	CE_High();			
	CE_Low();			
	Send_Byte(0x01);								// ����д״̬�Ĵ���ָ��
	Send_Byte(temp1);								// �ָ�״̬�Ĵ���������Ϣ 
	CE_High();    
	return (OK);
}



























