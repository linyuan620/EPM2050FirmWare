/*
  * FreeModbus Libary: LPC214X Port
  * Copyright (C) 2007 Tiago Prado Lone <tiago@maxwellbohr.com.br>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2007/04/24 23:15:18 wolti Exp $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
/* ----------------------- Start implementation -----------------------------*/
void vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
//	if(xRxEnable)
//	{
//		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//	}
//	else
//	{
//		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);	
//	}
//
//	if(xTxEnable)
//	{
//		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
//	}
//	else
//	{
//	   USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
//	}

	if(xRxEnable)
	{
		UART_IntConfig((LPC_UART_TypeDef *)LPC_UART2, UART_INTCFG_RBR, ENABLE);
	}
	else
	{
		UART_IntConfig((LPC_UART_TypeDef *)LPC_UART2, UART_INTCFG_RBR, DISABLE);
	}

	if(xTxEnable)
	{
//		UART_IntConfig((LPC_UART_TypeDef *)LPC_UART2, UART_INTCFG_THRE, ENABLE);
		prvvUARTTxReadyISR();
	}
	else
	{
//	   UART_IntConfig((LPC_UART_TypeDef *)LPC_UART2, UART_INTCFG_THRE, DISABLE);
	}

}

void
vMBPortClose( void )
{
//	USART_ITConfig(USART1, USART_IT_TXE|USART_IT_RXNE, DISABLE);
//	USART_Cmd(USART1, DISABLE);
	
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART2, UART_INTCFG_THRE, DISABLE);
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART2, UART_INTCFG_RBR, DISABLE);
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART2, DISABLE);
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{	
	BOOL            bInitialized = TRUE;
//	USART_InitTypeDef USART_InitStructure;
//	USART_ClockInitTypeDef  USART_ClockInitStructure;
//
//	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
//	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
//	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
//	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
//
//	USART_InitStructure.USART_BaudRate = ulBaudRate;
//    switch ( eParity )
//    {
//    case MB_PAR_NONE:
//        USART_InitStructure.USART_Parity = USART_Parity_No ;
//				USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//				break;
//    case MB_PAR_ODD:
//        USART_InitStructure.USART_Parity = USART_Parity_Odd ;
//				USART_InitStructure.USART_WordLength = USART_WordLength_9b;
//        break;
//    case MB_PAR_EVEN:
//        USART_InitStructure.USART_Parity = USART_Parity_Even ;
//				USART_InitStructure.USART_WordLength = USART_WordLength_9b;
//        break;
//    }
//    if( bInitialized )
//    {
//        ENTER_CRITICAL_SECTION(  );
//
//				USART_InitStructure.USART_StopBits = USART_StopBits_1;
//				USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	
//				USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//				switch(ucPORT)
//				{
//		    case 1:
//		        {
//								USART_ClockInit(USART1, &USART_ClockInitStructure);
//								USART_Init(USART1, &USART_InitStructure);
//								USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//								USART_Cmd(USART1, ENABLE);
//						}
//						break;
//		    case 2:
//		        {
//								USART_ClockInit(USART2, &USART_ClockInitStructure);
//								USART_Init(USART2, &USART_InitStructure);
//								USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//								USART_Cmd(USART2, ENABLE);
//						}
//		        break;
//		    case 3:
//		        {
//								USART_ClockInit(USART3, &USART_ClockInitStructure);
//								USART_Init(USART3, &USART_InitStructure);
//								USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//								USART_Cmd(USART3, ENABLE);
//						}
//		        break;
//	  	  }
//
//        EXIT_CRITICAL_SECTION(  );
//    }
	UART_CFG_Type UARTConfigStruct;

	UARTConfigStruct.Baud_rate = ulBaudRate;

    switch ( eParity )
    {
    case MB_PAR_NONE:
		UARTConfigStruct.Parity=UART_PARITY_NONE;
		break;
    case MB_PAR_ODD:
		UARTConfigStruct.Parity=UART_PARITY_ODD;
        break;
    case MB_PAR_EVEN:
		UARTConfigStruct.Parity=UART_PARITY_EVEN;
        break;
    }

	UARTConfigStruct.Stopbits=UART_STOPBIT_1;

	Uart2Init(UARTConfigStruct.Baud_rate,UARTConfigStruct.Parity,UARTConfigStruct.Stopbits);
    return bInitialized;
}



BOOL
xMBPortSerialPutByte( UCHAR *ucByte,USHORT usSndBufferCount )
{
	static USHORT i;

	GPIO_SetValue(0, 1<<4);
    while( usSndBufferCount != 0 )
    {
		UART_SendByte((LPC_UART_TypeDef *)LPC_UART2, *ucByte);
        ucByte++;  /* next byte in sendbuffer. */
        usSndBufferCount--;
	}
//	USART_SendData(USART1, ucByte);
//	UART3Send((LPC_UART_TypeDef *)LPC_UART3, (uint8_t *)&ucByte, 1);
//	GPIO_SetValue(0, 1<<4);
//	UART_SendByte((LPC_UART_TypeDef *)LPC_UART2, ucByte);
	for(i=0;i<10000;i++)
	{
		;
	}
	GPIO_ClearValue(0, 1<<4);
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
//	*pucByte = USART_ReceiveData(USART1);
//	UART3Receive((LPC_UART_TypeDef *)LPC_UART3,(uint8_t *)pucByte,1);
	*pucByte = UART_ReceiveByte((LPC_UART_TypeDef *)LPC_UART2);
    return TRUE;
}

/* 
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
void prvvUARTTxReadyISR(void)
{
    pxMBFrameCBTransmitterEmpty();
}

/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR(void)
{
    pxMBFrameCBByteReceived();
}

