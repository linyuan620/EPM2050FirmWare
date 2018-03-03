/*********************************************************************************************************
  Copyright (C), 2011-8-3  Skyray Instrment Co.,LTD.
  File name:      	uarts.h
  Author:   		������    	Version:	V2.5        	Date:	2011-8-3 11:37:43
  Description:    	����0,2,3��ʼ���������жϷ�����
  					����3���� ���� ��ӡ
Others:
Function List:  // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
  	1. extern void UART_IntErr(uint8_t bLSErrType);
	2. extern void Uart0Init(uint32_t Baud_rate,UART_PARITY_Type Parity,UART_STOPBIT_Type Stopbits);
	3. extern void UART0_IntReceive(void);
	4. extern void UART0_IntTransmit(void);
	5. extern uint32_t UART0Send(LPC_UART_TypeDef *UARTPort, uint8_t *txbuf, uint8_t buflen);
	6. extern uint32_t UART0Receive(LPC_UART_TypeDef *UARTPort, uint8_t *rxbuf, uint8_t buflen);
	7. extern void Uart2Init(uint32_t Baud_rate,UART_PARITY_Type Parity,UART_STOPBIT_Type Stopbits);
	8. extern void UART2_IntReceive(void);
	9. extern void UART2_IntTransmit(void);
	10.extern uint32_t UART2Send(LPC_UART_TypeDef *UARTPort, uint8_t *txbuf, uint8_t buflen);
	11.extern uint32_t UART2Receive(LPC_UART_TypeDef *UARTPort, uint8_t *rxbuf, uint8_t buflen);
	12.extern void Uart3Init(uint32_t Baud_rate,UART_PARITY_Type Parity,UART_STOPBIT_Type Stopbits);
	13.extern void UART3_IntReceive(void);
	14.extern void UART3_IntTransmit(void);
	15.extern uint32_t UART3Send(LPC_UART_TypeDef *UARTPort, uint8_t *txbuf, uint8_t buflen);
	16.extern uint32_t UART3Receive(LPC_UART_TypeDef *UARTPort, uint8_t *rxbuf, uint8_t buflen);
	17.extern void SetOutUart(uint16_t baud,uint16_t parity,uint16_t stop);

  History:
	1. Date:  	2011-8-3 12:52:43
	   Author: 	��˳��
	   Modification: 	�淶��ע��
************************************************************************************************************/

#ifndef __UARTS_H
#define __UARTS_H

/*********************************************************************************************************
**               ͷ�ļ�����
*********************************************************************************************************/
#include "global.h"

/*********************************************************************************************************
**               C++�������
*********************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif                                                                  /*  __cplusplus                 */

    /*
    buffer size definition
    Buf mask
    Check buf is full or not
    Check buf will be full in next receiving or not
    Check buf is empty
    Reset buf
    */
#define UART_RING_BUFSIZE 256
#define __BUF_MASK (UART_RING_BUFSIZE-1)
#define __BUF_IS_FULL(head, tail) ((tail&__BUF_MASK)==((head+1)&__BUF_MASK))
#define __BUF_WILL_FULL(head, tail) ((tail&__BUF_MASK)==((head+2)&__BUF_MASK))
#define __BUF_IS_EMPTY(head, tail) ((head&__BUF_MASK)==(tail&__BUF_MASK))
#define __BUF_RESET(bufidx)	(bufidx=0)

#define __BUF_INCR(bufidx)	(bufidx=(bufidx+1)&__BUF_MASK)

    /*���ڻ������ṹ�� */
    typedef struct
    {
        __IO uint32_t tx_head;                /*!< UART Tx ring buffer head index */
        __IO uint32_t tx_tail;                /*!< UART Tx ring buffer tail index */
        __IO uint32_t rx_head;                /*!< UART Rx ring buffer head index */
        __IO uint32_t rx_tail;                /*!< UART Rx ring buffer tail index */
        __IO uint8_t  tx[UART_RING_BUFSIZE];  /*!< UART Tx data ring buffer */
        __IO uint8_t  rx[UART_RING_BUFSIZE];  /*!< UART Rx data ring buffer */
    } UART_RING_BUFFER_T;



    /*
    ���崮��0���ݻ�����
    ���崮��2���ݻ�����
    ���崮��3���ݻ�����
    */
    extern UART_RING_BUFFER_T rb;
    extern UART_RING_BUFFER_T uart1RingBuffer;
    extern UART_RING_BUFFER_T uart2RingBuffer;
    extern UART_RING_BUFFER_T uart3RingBuffer;

    /*********************************************************************************************************
    * Function name:	    UART_IntErr
    * Descriptions:	    	���ڴ�������
    * input parameters:    	bLSErrType	UART Line Status Error Type
    * output parameters:   	��
    * Returned value:      	��
    *********************************************************************************************************/
    extern void UART_IntErr(uint8_t bLSErrType);

    /*********************************************************************************************************
    * Function name:	    Uart0Init
    * Descriptions:	    	���ڳ�ʼ����������żУ�飬������,ֹͣλ
    * input parameters:    	Baud_rate	������
    *						Parity		У��λ
    *						Stopbits	ֹͣλ
    * output parameters:   	��
    * Returned value:      	��
    *********************************************************************************************************/
    extern void Uart0Init(uint32_t Baud_rate,UART_PARITY_Type Parity,UART_STOPBIT_Type Stopbits);

    /*********************************************************************************************************
    * Function name:	    UART0_IntReceive
    * Descriptions:	    	����0�жϽ��ܺ���
    * input parameters:    	��
    * output parameters:   	��
    * Returned value:      	��
    *********************************************************************************************************/
    extern void UART0_IntReceive(void);

    /*********************************************************************************************************
    * Function name:	    UART0_IntReceive
    * Descriptions:	    	����0�жϷ��ͺ���
    * input parameters:    	��
    * output parameters:   	��
    * Returned value:      	��
    *********************************************************************************************************/
    extern void UART0_IntTransmit(void);

    /*********************************************************************************************************
    * Function name:	    UART0Send
    * Descriptions:	    	UART transmit function for interrupt mode (using ring buffers)
    * input parameters:    	buflen Length of Transmit buffer
    * output parameters:   	txbuf Pointer to Transmit buffer
    * Returned value:      	Number of bytes actually sent to the ring buffer
    *********************************************************************************************************/
    extern uint32_t UART0Send(LPC_UART_TypeDef *UARTPort, uint8_t *txbuf, uint8_t buflen);

    /*********************************************************************************************************
    * Function name:	    UART0Receive
    * Descriptions:	    	UART read function for interrupt mode (using ring buffers)
    * input parameters:    	buflen Length of Received buffer
    * output parameters:   	txbuf Pointer to Transmit buffer
    * Returned value:      	Number of bytes actually read from the ring buffer
    *********************************************************************************************************/
    extern uint32_t UART0Receive(LPC_UART_TypeDef *UARTPort, uint8_t *rxbuf, uint8_t buflen);

    /*********************************************************************************************************
    * Function name:	    Uart0Init
    * Descriptions:	    	���ڳ�ʼ����������żУ�飬������,ֹͣλ
    * input parameters:    	Baud_rate	������
    *						Parity		У��λ
    *						Stopbits	ֹͣλ
    * output parameters:   	��
    * Returned value:      	��
    *********************************************************************************************************/
//ad by yxq
    extern void Uart1Init(uint32_t Baud_rate,UART_PARITY_Type Parity,UART_STOPBIT_Type Stopbits);

    /*********************************************************************************************************
    * Function name:	    UART1_IntReceive
    * Descriptions:	    	����2�жϽ��ܺ���
    * input parameters:    	��
    * output parameters:   	��
    * Returned value:      	��
    *********************************************************************************************************/
    extern void UART1_IntReceive(void);

    /*********************************************************************************************************
    * Function name:	    UART1_IntReceive
    * Descriptions:	    	����1�жϷ��ͺ���
    * input parameters:    	��
    * output parameters:   	��
    * Returned value:      	��
    *********************************************************************************************************/
    extern void UART1_IntTransmit(void);

    /*********************************************************************************************************
    * Function name:	    UART1Send
    * Descriptions:	    	UART transmit function for interrupt mode (using ring buffers)
    * input parameters:    	buflen Length of Transmit buffer
    * output parameters:   	txbuf Pointer to Transmit buffer
    * Returned value:      	Number of bytes actually sent to the ring buffer
    *********************************************************************************************************/
    extern uint32_t UART1Send(LPC_UART_TypeDef *UARTPort, uint8_t *txbuf, uint8_t buflen);

    /*********************************************************************************************************
    * Function name:	    UART1Receive
    * Descriptions:	    	UART read function for interrupt mode (using ring buffers)
    * input parameters:    	buflen Length of Received buffer
    * output parameters:   	txbuf Pointer to Transmit buffer
    * Returned value:      	Number of bytes actually read from the ring buffer
    *********************************************************************************************************/
    extern uint32_t UART1Receive(LPC_UART_TypeDef *UARTPort, uint8_t *rxbuf, uint8_t buflen);

    /*********************************************************************************************************
    * Function name:	    Uart0Init
    * Descriptions:	    	���ڳ�ʼ����������żУ�飬������,ֹͣλ
    * input parameters:    	Baud_rate	������
    *						Parity		У��λ
    *						Stopbits	ֹͣλ
    * output parameters:   	��
    * Returned value:      	��
    *********************************************************************************************************/
//ad by yxq
    extern void Uart2Init(uint32_t Baud_rate,UART_PARITY_Type Parity,UART_STOPBIT_Type Stopbits);

    /*********************************************************************************************************
    * Function name:	    UART2_IntReceive
    * Descriptions:	    	����2�жϽ��ܺ���
    * input parameters:    	��
    * output parameters:   	��
    * Returned value:      	��
    *********************************************************************************************************/
    extern void UART2_IntReceive(void);

    /*********************************************************************************************************
    * Function name:	    UART2_IntReceive
    * Descriptions:	    	����2�жϷ��ͺ���
    * input parameters:    	��
    * output parameters:   	��
    * Returned value:      	��
    *********************************************************************************************************/
    extern void UART2_IntTransmit(void);

    /*********************************************************************************************************
    * Function name:	    UART2Send
    * Descriptions:	    	UART transmit function for interrupt mode (using ring buffers)
    * input parameters:    	buflen Length of Transmit buffer
    * output parameters:   	txbuf Pointer to Transmit buffer
    * Returned value:      	Number of bytes actually sent to the ring buffer
    *********************************************************************************************************/
    extern uint32_t UART2Send(LPC_UART_TypeDef *UARTPort, uint8_t *txbuf, uint8_t buflen);

    /*********************************************************************************************************
    * Function name:	    UART2Receive
    * Descriptions:	    	UART read function for interrupt mode (using ring buffers)
    * input parameters:    	buflen Length of Received buffer
    * output parameters:   	txbuf Pointer to Transmit buffer
    * Returned value:      	Number of bytes actually read from the ring buffer
    *********************************************************************************************************/
    extern uint32_t UART2Receive(LPC_UART_TypeDef *UARTPort, uint8_t *rxbuf, uint8_t buflen);

    /*********************************************************************************************************
    * Function name:	    Uart3Init
    * Descriptions:	    	���ڳ�ʼ����������żУ�飬������,ֹͣλ
    * input parameters:    	Baud_rate	������
    *						Parity		У��λ
    *						Stopbits	ֹͣλ
    * output parameters:   	��
    * Returned value:      	��
    *********************************************************************************************************/
    extern void Uart3Init(uint32_t Baud_rate,UART_PARITY_Type Parity,UART_STOPBIT_Type Stopbits);

    /*********************************************************************************************************
    * Function name:	    UART3_IntReceive
    * Descriptions:	    	����3�жϽ��ܺ���
    * input parameters:    	��
    * output parameters:   	��
    * Returned value:      	��
    *********************************************************************************************************/
    extern void UART3_IntReceive(void);

    /*********************************************************************************************************
    * Function name:	    UART3_IntReceive
    * Descriptions:	    	����3�жϷ��ͺ���
    * input parameters:    	��
    * output parameters:   	��
    * Returned value:      	��
    *********************************************************************************************************/
    extern void UART3_IntTransmit(void);

    /*********************************************************************************************************
    * Function name:	    UART3Send
    * Descriptions:	    	UART transmit function for interrupt mode (using ring buffers)
    * input parameters:    	buflen Length of Transmit buffer
    * output parameters:   	txbuf Pointer to Transmit buffer
    * Returned value:      	Number of bytes actually sent to the ring buffer
    *********************************************************************************************************/
    extern uint32_t UART3Send(LPC_UART_TypeDef *UARTPort, uint8_t *txbuf, uint8_t buflen);

    /*********************************************************************************************************
    * Function name:	    UART3Receive
    * Descriptions:	    	UART read function for interrupt mode (using ring buffers)
    * input parameters:    	buflen Length of Received buffer
    * output parameters:   	txbuf Pointer to Transmit buffer
    * Returned value:      	Number of bytes actually read from the ring buffer
    *********************************************************************************************************/
    extern uint32_t UART3Receive(LPC_UART_TypeDef *UARTPort, uint8_t *rxbuf, uint8_t buflen);

    /*********************************************************************************************************
    * Function name:	    SetOutUart
    * Descriptions:	    	UART Set for outside comm
    * input parameters:    	baud	baud
    *						parity
    *						stop
    * output parameters:   	None
    * Returned value:      	None
    *********************************************************************************************************/
    extern void SetOutUart(uint16_t baud,uint16_t parity,uint16_t stop);
//extern uint8_t ModbusReceive;//���ձ�־
    extern OS_EVENT *OSUart0UsedMutex;	//�����ź���
    extern volatile uint8 TTLOutPut[8];

#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */


#endif

/********************************************************************************************************
  End Of File
********************************************************************************************************/

