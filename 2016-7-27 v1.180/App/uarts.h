/*********************************************************************************************************
  Copyright (C), 2011-8-3  Skyray Instrment Co.,LTD.
  File name:      	uarts.h
  Author:   		方卫龙    	Version:	V2.5        	Date:	2011-8-3 11:37:43
  Description:    	串口0,2,3初始化，及其中断服务函数
  					串口3用于 蓝牙 打印
Others:
Function List:  // 主要函数列表，每条记录应包括函数名及功能简要说明
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
	   Author: 	何顺杰
	   Modification: 	规范了注释
************************************************************************************************************/

#ifndef __UARTS_H
#define __UARTS_H

/*********************************************************************************************************
**               头文件包含
*********************************************************************************************************/
#include "global.h"

/*********************************************************************************************************
**               C++代码兼容
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

    /*串口缓冲区结构体 */
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
    定义串口0数据缓冲区
    定义串口2数据缓冲区
    定义串口3数据缓冲区
    */
    extern UART_RING_BUFFER_T rb;
    extern UART_RING_BUFFER_T uart1RingBuffer;
    extern UART_RING_BUFFER_T uart2RingBuffer;
    extern UART_RING_BUFFER_T uart3RingBuffer;

    /*********************************************************************************************************
    * Function name:	    UART_IntErr
    * Descriptions:	    	串口错误处理函数
    * input parameters:    	bLSErrType	UART Line Status Error Type
    * output parameters:   	无
    * Returned value:      	无
    *********************************************************************************************************/
    extern void UART_IntErr(uint8_t bLSErrType);

    /*********************************************************************************************************
    * Function name:	    Uart0Init
    * Descriptions:	    	串口初始化，设置奇偶校验，波特率,停止位
    * input parameters:    	Baud_rate	波特率
    *						Parity		校验位
    *						Stopbits	停止位
    * output parameters:   	无
    * Returned value:      	无
    *********************************************************************************************************/
    extern void Uart0Init(uint32_t Baud_rate,UART_PARITY_Type Parity,UART_STOPBIT_Type Stopbits);

    /*********************************************************************************************************
    * Function name:	    UART0_IntReceive
    * Descriptions:	    	串口0中断接受函数
    * input parameters:    	无
    * output parameters:   	无
    * Returned value:      	无
    *********************************************************************************************************/
    extern void UART0_IntReceive(void);

    /*********************************************************************************************************
    * Function name:	    UART0_IntReceive
    * Descriptions:	    	串口0中断发送函数
    * input parameters:    	无
    * output parameters:   	无
    * Returned value:      	无
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
    * Descriptions:	    	串口初始化，设置奇偶校验，波特率,停止位
    * input parameters:    	Baud_rate	波特率
    *						Parity		校验位
    *						Stopbits	停止位
    * output parameters:   	无
    * Returned value:      	无
    *********************************************************************************************************/
//ad by yxq
    extern void Uart1Init(uint32_t Baud_rate,UART_PARITY_Type Parity,UART_STOPBIT_Type Stopbits);

    /*********************************************************************************************************
    * Function name:	    UART1_IntReceive
    * Descriptions:	    	串口2中断接受函数
    * input parameters:    	无
    * output parameters:   	无
    * Returned value:      	无
    *********************************************************************************************************/
    extern void UART1_IntReceive(void);

    /*********************************************************************************************************
    * Function name:	    UART1_IntReceive
    * Descriptions:	    	串口1中断发送函数
    * input parameters:    	无
    * output parameters:   	无
    * Returned value:      	无
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
    * Descriptions:	    	串口初始化，设置奇偶校验，波特率,停止位
    * input parameters:    	Baud_rate	波特率
    *						Parity		校验位
    *						Stopbits	停止位
    * output parameters:   	无
    * Returned value:      	无
    *********************************************************************************************************/
//ad by yxq
    extern void Uart2Init(uint32_t Baud_rate,UART_PARITY_Type Parity,UART_STOPBIT_Type Stopbits);

    /*********************************************************************************************************
    * Function name:	    UART2_IntReceive
    * Descriptions:	    	串口2中断接受函数
    * input parameters:    	无
    * output parameters:   	无
    * Returned value:      	无
    *********************************************************************************************************/
    extern void UART2_IntReceive(void);

    /*********************************************************************************************************
    * Function name:	    UART2_IntReceive
    * Descriptions:	    	串口2中断发送函数
    * input parameters:    	无
    * output parameters:   	无
    * Returned value:      	无
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
    * Descriptions:	    	串口初始化，设置奇偶校验，波特率,停止位
    * input parameters:    	Baud_rate	波特率
    *						Parity		校验位
    *						Stopbits	停止位
    * output parameters:   	无
    * Returned value:      	无
    *********************************************************************************************************/
    extern void Uart3Init(uint32_t Baud_rate,UART_PARITY_Type Parity,UART_STOPBIT_Type Stopbits);

    /*********************************************************************************************************
    * Function name:	    UART3_IntReceive
    * Descriptions:	    	串口3中断接受函数
    * input parameters:    	无
    * output parameters:   	无
    * Returned value:      	无
    *********************************************************************************************************/
    extern void UART3_IntReceive(void);

    /*********************************************************************************************************
    * Function name:	    UART3_IntReceive
    * Descriptions:	    	串口3中断发送函数
    * input parameters:    	无
    * output parameters:   	无
    * Returned value:      	无
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
//extern uint8_t ModbusReceive;//接收标志
    extern OS_EVENT *OSUart0UsedMutex;	//互斥信号量
    extern volatile uint8 TTLOutPut[8];

#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */


#endif

/********************************************************************************************************
  End Of File
********************************************************************************************************/

