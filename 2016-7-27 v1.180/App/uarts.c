/****************************************Copyright (c)****************************************************
**                            		skyray-instrument Co.,LTD.
**
**                                 http://www.skyray-instrument.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           uarts.c
** Last modified Date:  2011-02-24
** Last Version:        V1.0
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Created By:
** Created date:        2011-02-24
** Version:             V1.0
** Descriptions:        First version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:       2011-02-24
** Version:             V1.X
** Descriptions:
**
*********************************************************************************************************/
#include "uarts.h"
extern void prvvUARTTxReadyISR(void);
extern void prvvUARTRxISR(void);
//uint8_t ModbusReceive=1;//接收标志
extern void ReceOneChar(uint8 ReceCharacter);
extern	OS_EVENT *mb_handle_event_ptr;

/*Flag Status (RESET = 0, SET = !RESET)*/
__IO FlagStatus Tx0IntStat;
/*定义串口0数据缓冲区*/
//UART_RING_BUFFER_T uart0RingBuffer;
UART_RING_BUFFER_T rb;

/*Flag Status (RESET = 0, SET = !RESET)*/
__IO FlagStatus Tx1IntStat;
/*定义串口1数据缓冲区*/
UART_RING_BUFFER_T uart1RingBuffer;                           //add by yxq


/*Flag Status (RESET = 0, SET = !RESET)*/
__IO FlagStatus Tx2IntStat;
/*定义串口2数据缓冲区*/
UART_RING_BUFFER_T uart2RingBuffer;

/*Flag Status (RESET = 0, SET = !RESET)*/
__IO FlagStatus Tx3IntStat;
/*定义串口3数据缓冲区*/
UART_RING_BUFFER_T uart3RingBuffer;


/*********************************************************************************************************
* Function name:	    UART_IntErr
* Descriptions:	    	串口错误处理函数
* input parameters:    	bLSErrType	UART Line Status Error Type
* output parameters:   	无
* Returned value:      	无
*********************************************************************************************************/
void UART_IntErr(uint8_t bLSErrType)
{
    if (bLSErrType & UART_LSR_PE) {
//		UART_Send(LPC_UART0, p_err_menu, sizeof(p_err_menu), BLOCKING);
    }

    if (bLSErrType & UART_LSR_FE) {
//		UART_Send(LPC_UART0, f_err_menu, sizeof(f_err_menu), BLOCKING);
    }
}

/*********************************************************************************************************
* Function name:	    Uart0Init
* Descriptions:	    	串口初始化，设置奇偶校验，波特率,停止位
* input parameters:    	Baud_rate	波特率
*						Parity		校验位
*						Stopbits	停止位
* output parameters:   	无
* Returned value:      	无
*********************************************************************************************************/
void Uart0Init(uint32_t Baud_rate,UART_PARITY_Type Parity,UART_STOPBIT_Type Stopbits)//用于fpga接口板通信
{
    // UART Configuration structure variable
    UART_CFG_Type UARTConfigStruct;//配置里有校验方式，字节数，...
    // UART FIFO configuration Struct variable
    UART_FIFO_CFG_Type UARTFIFOConfigStruct;
    // Pin configuration for UART0
    PINSEL_CFG_Type PinCfg;

    /*
     * Initialize UART0 pin connect
     */
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Pinnum = 2;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 3;
    PINSEL_ConfigPin(&PinCfg);
    /* Initialize UART Configuration parameter structure to default state:
     * Baudrate = 9600bps
     * 8 data bit
     * 1 Stop bit
     * None parity
     */
    UART_ConfigStructInit(&UARTConfigStruct);

    // Initialize UART0 peripheral with given to corresponding parameter
    UART_Init((LPC_UART_TypeDef *)LPC_UART0, &UARTConfigStruct);


    /* Initialize FIFOConfigStruct to default state:
     * 				- FIFO_DMAMode = DISABLE
     * 				- FIFO_Level = UART_FIFO_TRGLEV0
     * 				- FIFO_ResetRxBuf = ENABLE
     * 				- FIFO_ResetTxBuf = ENABLE
     * 				- FIFO_State = ENABLE
     */
    UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

    // Initialize FIFO for UART0 peripheral
    UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART0, &UARTFIFOConfigStruct);


    // Enable UART Transmit
    UART_TxCmd((LPC_UART_TypeDef *)LPC_UART0, ENABLE);

    /* Enable UART Rx interrupt */
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_RBR, ENABLE);
    /* Enable UART line status interrupt */
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_RLS, ENABLE);
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_THRE, DISABLE);
    /*
     * Do not enable transmit interrupt here, since it is handled by
     * UART_Send() function, just to reset Tx Interrupt state for the
     * first time
     */
    Tx0IntStat = RESET;

    // Reset ring buf head and tail idx
    __BUF_RESET(rb.rx_head);
    __BUF_RESET(rb.rx_tail);
    __BUF_RESET(rb.tx_head);
    __BUF_RESET(rb.tx_tail);

    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(UART0_IRQn, ((PRIO_GROUP_UART0<<3)|PRIO_UART0));
    /* Enable Interrupt for UART0 channel */
    NVIC_EnableIRQ(UART0_IRQn);
}

/*********************************************************************************************************
* Function name:	    UART0_IRQHandler
* Descriptions:	    	串口0中断处理函数
* input parameters:    	无
* output parameters:   	无
* Returned value:      	无
*********************************************************************************************************/
void UART0_IRQHandler(void)
{
    uint32_t intsrc, tmp, tmp1;

    /* Determine the interrupt source */
    intsrc = UART_GetIntId(LPC_UART0);//iir的状态读取。
    tmp = intsrc & UART_IIR_INTID_MASK;//thre中断
    // Receive Line Status
    if (tmp == UART_IIR_INTID_RLS) { //rls
        // Check line status
        tmp1 = UART_GetLineStatus(LPC_UART0);
        // Mask out the Receive Ready and Transmit Holding empty status
        tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
                 | UART_LSR_BI | UART_LSR_RXFE);
        // If any error exist
        if (tmp1) {
//				UART_IntErr(tmp1);
        }
    }

    // Receive Data Available or Character time-out
    if ((tmp == UART_IIR_INTID_RDA) )//|| (tmp == UART_IIR_INTID_CTI))
    {
        UART0_IntReceive();
    }

    // Transmit Holding Empty
    if (tmp == UART_IIR_INTID_THRE) {
        UART0_IntTransmit();
    }

}

/*********************************************************************************************************
* Function name:	    UART0_IntReceive
* Descriptions:	    	串口0中断接受函数
* input parameters:    	无
* output parameters:   	无
* Returned value:      	无
*********************************************************************************************************/
void UART0_IntReceive(void)
{
    uint8_t tmpc;
    uint32_t rLen;
//	TIM_Cmd(LPC_TIM2,ENABLE);

    while(1) {
        // Call UART read function in UART driver
        rLen = UART_Receive((LPC_UART_TypeDef *)LPC_UART0, &tmpc, 1, NONE_BLOCKING);
        // If data received
        if (rLen) {
            /* Check if buffer is more space
             * If no more space, remaining character will be trimmed out
             */
            if (  (!__BUF_IS_FULL(rb.rx_head,rb.rx_tail)) ) {
                rb.rx[rb.rx_head] = tmpc;
                __BUF_INCR(rb.rx_head);

            }
        }
        // no more data
        else {

            break;
        }
    }
}

/*********************************************************************************************************
* Function name:	    UART0_IntReceive
* Descriptions:	    	串口0中断发送函数
* input parameters:    	无
* output parameters:   	无
* Returned value:      	无
*********************************************************************************************************/
void UART0_IntTransmit(void)
{
    // Disable THRE interrupt
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_THRE, DISABLE);

    /* Wait for FIFO buffer empty, transfer UART_TX_FIFO_SIZE bytes
     * of data or break whenever ring buffers are empty */
    /* Wait until THR empty */
    while (UART_CheckBusy((LPC_UART_TypeDef *)LPC_UART0) == SET);

    while (!__BUF_IS_EMPTY(rb.tx_head,rb.tx_tail))
    {
        /* Move a piece of data into the transmit FIFO */
        if (UART_Send((LPC_UART_TypeDef *)LPC_UART0, (uint8_t *)&rb.tx[rb.tx_tail], 1, NONE_BLOCKING)) {
            /* Update transmit ring FIFO tail pointer */
            __BUF_INCR(rb.tx_tail);
        } else {
            break;
        }
    }

    /* If there is no more data to send, disable the transmit
       interrupt - else enable it or keep it enabled */
    if (__BUF_IS_EMPTY(rb.tx_head, rb.tx_tail)) {
        UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_THRE, DISABLE);
        // Reset Tx Interrupt state
        Tx0IntStat = RESET;
    }
    else {
        // Set Tx Interrupt state
        Tx0IntStat = SET;
        UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_THRE, ENABLE);
    }
}

/*********************************************************************************************************
* Function name:	    UART0Send
* Descriptions:	    	UART transmit function for interrupt mode (using ring buffers)
* input parameters:    	buflen Length of Transmit buffer
* output parameters:   	txbuf Pointer to Transmit buffer
* Returned value:      	Number of bytes actually sent to the ring buffer
*********************************************************************************************************/
uint32_t UART0Send(LPC_UART_TypeDef *UARTPort, uint8_t *txbuf, uint8_t buflen)
{
    uint8_t *data = (uint8_t *) txbuf;
    uint32_t bytes = 0;

    /* Temporarily lock out UART transmit interrupts during this
       read so the UART transmit interrupt won't cause problems
       with the index values */
    UART_IntConfig(UARTPort, UART_INTCFG_THRE, DISABLE);

    /* Loop until transmit run buffer is full or until n_bytes
       expires */
    while ((buflen > 0) && (!__BUF_IS_FULL(rb.tx_head, rb.tx_tail)))
    {
        /* Write data from buffer into ring buffer */
        rb.tx[rb.tx_head] = *data;
        data++;

        /* Increment head pointer */
        __BUF_INCR(rb.tx_head);

        /* Increment data count and decrement buffer size count */
        bytes++;
        buflen--;
    }

    /*
     * Check if current Tx interrupt enable is reset,
     * that means the Tx interrupt must be re-enabled
     * due to call UART_IntTransmit() function to trigger
     * this interrupt type
     */
    if (Tx0IntStat == RESET) {
        UART0_IntTransmit();
    }
    /*
     * Otherwise, re-enables Tx Interrupt
     */
    else {
        UART_IntConfig(UARTPort, UART_INTCFG_THRE, ENABLE);
    }

    return bytes;
}

/*********************************************************************************************************
* Function name:	    UART0Receive
* Descriptions:	    	UART read function for interrupt mode (using ring buffers)
* input parameters:    	buflen Length of Received buffer
* output parameters:   	txbuf Pointer to Transmit buffer
* Returned value:      	Number of bytes actually read from the ring buffer
*********************************************************************************************************/
uint32_t UART0Receive(LPC_UART_TypeDef *UARTPort, uint8_t *rxbuf, uint8_t buflen)
{
    uint8_t *data = (uint8_t *) rxbuf;
    uint32_t bytes = 0;

    /* Temporarily lock out UART receive interrupts during this
       read so the UART receive interrupt won't cause problems
       with the index values */
    UART_IntConfig(UARTPort, UART_INTCFG_RBR, DISABLE);

    /* Loop until receive buffer ring is empty or
    	until max_bytes expires */
    while ((buflen > 0) && (!(__BUF_IS_EMPTY(rb.rx_head, rb.rx_tail))))
    {
        /* Read data from ring buffer into user buffer */
        *data = rb.rx[rb.rx_tail];
        data++;

        /* Update tail pointer */
        __BUF_INCR(rb.rx_tail);

        /* Increment data count and decrement buffer size count */
        bytes++;
        buflen--;
    }

    /* Re-enable UART interrupts */
    UART_IntConfig(UARTPort, UART_INTCFG_RBR, ENABLE);

    return bytes;
}
//add by yxq

/*********************************************************************************************************
* Function name:	    Uart1Init  用于DTU
* Descriptions:	    	串口初始化，设置奇偶校验，波特率,停止位
* input parameters:    	Baud_rate	波特率
*						Parity		校验位
*						Stopbits	停止位
* output parameters:   	无
* Returned value:      	无
*********************************************************************************************************/
void Uart1Init(uint32_t Baud_rate,UART_PARITY_Type Parity,UART_STOPBIT_Type Stopbits)
{
    /* UART Configuration structure variable*/
    UART_CFG_Type UARTConfigStruct;
    /* UART FIFO configuration Struct variable*/
    UART_FIFO_CFG_Type UARTFIFOConfigStruct;
    /* Pin configuration for UART1*/
    PINSEL_CFG_Type PinCfg;

/////	Tx1IntStat = RESET;

    /* Init UART pin connect*/
    PinCfg.Funcnum = 2;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 2;
    PinCfg.Pinnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 1;
    PINSEL_ConfigPin(&PinCfg);   //p2.0 and p2.1

    /* Initialize UART Configuration parameter structure to default state:
     * Baudrate = 9600bps
     * 8 data bit
     * 1 Stop bit
     * None parity
     */
    UART_ConfigStructInit(&UARTConfigStruct);
    UARTConfigStruct.Baud_rate = Baud_rate;
    UARTConfigStruct.Parity=Parity;
    UARTConfigStruct.Stopbits=Stopbits;
    /* Initialize UART1 peripheral with given to corresponding parameter*/
    UART_Init((LPC_UART_TypeDef *)LPC_UART1, &UARTConfigStruct);

    /* Initialize FIFOConfigStruct to default state:
     * 				- FIFO_DMAMode = DISABLE
     * 				- FIFO_Level = UART_FIFO_TRGLEV0
     * 				- FIFO_ResetRxBuf = ENABLE
     * 				- FIFO_ResetTxBuf = ENABLE
     * 				- FIFO_State = ENABLE
     */
    UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

    /* Initialize FIFO for UART1 peripheral*/
    UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART1, &UARTFIFOConfigStruct);

    UART_TxCmd((LPC_UART_TypeDef *)LPC_UART1, ENABLE);
    /* Enable UART Rx interrupt */
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_RBR, ENABLE);
    /* Enable UART line status interrupt */
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_RLS, ENABLE);

    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_THRE, DISABLE);
    /*
     * Do not enable transmit interrupt here, since it is handled by
     * UART_Send() function, just to reset Tx Interrupt state for the
     * first time
     */
    Tx1IntStat = RESET;

    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(UART1_IRQn, ((0x02<<3)|0x03));
    /* Enable Interrupt for UART1 channel */
    NVIC_EnableIRQ(UART1_IRQn);

    /* Reset ring buf head and tail idx*/
    __BUF_RESET(uart1RingBuffer.rx_head);
    __BUF_RESET(uart1RingBuffer.rx_tail);
    __BUF_RESET(uart1RingBuffer.tx_head);
    __BUF_RESET(uart1RingBuffer.tx_tail);
}

/*********************************************************************************************************
* Function name:	    UART1_IRQHandler
* Descriptions:	    	串口1中断处理函数
* input parameters:    	无
* output parameters:   	无
* Returned value:      	无
*********************************************************************************************************/
void UART1_IRQHandler(void)
{
    uint32_t intsrc, tmp, tmp1;

    /* Determine the interrupt source */
    intsrc = UART_GetIntId((LPC_UART_TypeDef *)LPC_UART1);
    tmp = intsrc & UART_IIR_INTID_MASK;

    /* Receive Line Status*/
    if (tmp == UART_IIR_INTID_RLS) {
        // Check line status
        tmp1 = UART_GetLineStatus((LPC_UART_TypeDef *)LPC_UART1);
        // Mask out the Receive Ready and Transmit Holding empty status
        tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
                 | UART_LSR_BI | UART_LSR_RXFE);
        // If any error exist
        if (tmp1) {
            UART_IntErr(tmp1);
        }
    }

    // Receive Data Available or Character time-out
    if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)) {
        UART1_IntReceive();
    }

    // Transmit Holding Empty
    if (tmp == UART_IIR_INTID_THRE) {
        UART1_IntTransmit();
    }

}

/*********************************************************************************************************
* Function name:	    UART1_IntReceive
* Descriptions:	    	串口1中断接受函数
* input parameters:    	无
* output parameters:   	无
* Returned value:      	无
*********************************************************************************************************/
void UART1_IntReceive(void)
{
    uint8_t tmpc;
    uint32_t rLen;

    while(1) {
        // Call UART read function in UART driver
        rLen = UART_Receive((LPC_UART_TypeDef *)LPC_UART1, &tmpc, 1, NONE_BLOCKING);
        // If data received
        if (rLen) {
            /* Check if buffer is more space
             * If no more space, remaining character will be trimmed out
             */
            if (!__BUF_IS_FULL(uart1RingBuffer.rx_head,uart1RingBuffer.rx_tail)) {
                uart1RingBuffer.rx[uart1RingBuffer.rx_head] = tmpc;
                __BUF_INCR(uart1RingBuffer.rx_head);
            }
            // no more data
            else {
                break;
            }
        }
    }
}

/*********************************************************************************************************
* Function name:	    UART1_IntReceive
* Descriptions:	    	串口1中断发送函数  用于DTU
* input parameters:    	无
* output parameters:   	无
* Returned value:      	无
*********************************************************************************************************/
void UART1_IntTransmit(void)
{
    // Disable THRE interrupt
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_THRE, DISABLE);

    /* Wait for FIFO buffer empty, transfer UART_TX_FIFO_SIZE bytes
     * of data or break whenever ring buffers are empty */
    /* Wait until THR empty */
    while (UART_CheckBusy((LPC_UART_TypeDef *)LPC_UART1) == SET);

//	GPIO_SetValue(2,(1<<13));

    while (!__BUF_IS_EMPTY(uart1RingBuffer.tx_head,uart1RingBuffer.tx_tail))
    {
        /* Move a piece of data into the transmit FIFO */
        if (UART_Send((LPC_UART_TypeDef *)LPC_UART1, (uint8_t *)&uart1RingBuffer.tx[uart1RingBuffer.tx_tail], 1, NONE_BLOCKING)) {
            /* Update transmit ring FIFO tail pointer */
            __BUF_INCR(uart1RingBuffer.tx_tail);
        } else {
            break;
        }
    }

    /* If there is no more data to send, disable the transmit
       interrupt - else enable it or keep it enabled */
    if (__BUF_IS_EMPTY(uart1RingBuffer.tx_head, uart1RingBuffer.tx_tail)) {
        UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_THRE, DISABLE);
        // Reset Tx Interrupt state
        Tx1IntStat = RESET;

//		Delay(3);
//		GPIO_ClearValue(2,(13<<4));
    }
    else {
        // Set Tx Interrupt state
        Tx1IntStat = SET;
        UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_THRE, ENABLE);
    }
}

/*********************************************************************************************************
* Function name:	    UART1Send
* Descriptions:	    	UART transmit function for interrupt mode (using ring buffers)
* input parameters:    	buflen Length of Transmit buffer
* output parameters:   	txbuf Pointer to Transmit buffer
* Returned value:      	Number of bytes actually sent to the ring buffer
*********************************************************************************************************/
uint32_t UART1Send(LPC_UART_TypeDef *UARTPort, uint8_t *txbuf, uint8_t buflen)
{
    uint8_t *data = (uint8_t *) txbuf;
    uint32_t bytes = 0;

    /* Temporarily lock out UART transmit interrupts during this
       read so the UART transmit interrupt won't cause problems
       with the index values */
    UART_IntConfig(UARTPort, UART_INTCFG_THRE, DISABLE);

    /* Loop until transmit run buffer is full or until n_bytes
       expires */
    while ((buflen > 0) && (!__BUF_IS_FULL(uart1RingBuffer.tx_head, uart1RingBuffer.tx_tail)))
    {
        /* Write data from buffer into ring buffer */
        uart1RingBuffer.tx[uart1RingBuffer.tx_head] = *data;
        data++;

        /* Increment head pointer */
        __BUF_INCR(uart1RingBuffer.tx_head);

        /* Increment data count and decrement buffer size count */
        bytes++;
        buflen--;
    }

    /*
     * Check if current Tx interrupt enable is reset,
     * that means the Tx interrupt must be re-enabled
     * due to call UART_IntTransmit() function to trigger
     * this interrupt type
     */
    if (Tx1IntStat == RESET) {
        UART1_IntTransmit();
    }
    /*
     * Otherwise, re-enables Tx Interrupt
     */
    else {
        UART_IntConfig(UARTPort, UART_INTCFG_THRE, ENABLE);
    }

    return bytes;
}

/*********************************************************************************************************
* Function name:	    UART1Receive
* Descriptions:	    	UART read function for interrupt mode (using ring buffers)
* input parameters:    	buflen Length of Received buffer
* output parameters:   	txbuf Pointer to Transmit buffer
* Returned value:      	Number of bytes actually read from the ring buffer
*********************************************************************************************************/
uint32_t UART1Receive(LPC_UART_TypeDef *UARTPort, uint8_t *rxbuf, uint8_t buflen)
{
    uint8_t *data = (uint8_t *) rxbuf;
    uint32_t bytes = 0;

    /* Temporarily lock out UART receive interrupts during this
       read so the UART receive interrupt won't cause problems
       with the index values */
    UART_IntConfig(UARTPort, UART_INTCFG_RBR, DISABLE);

    /* Loop until receive buffer ring is empty or
    	until max_bytes expires */
    while ((buflen > 0) && (!(__BUF_IS_EMPTY(uart1RingBuffer.rx_head, uart1RingBuffer.rx_tail))))
    {
        /* Read data from ring buffer into user buffer */
        *data = uart1RingBuffer.rx[uart1RingBuffer.rx_tail];
        data++;

        /* Update tail pointer */
        __BUF_INCR(uart1RingBuffer.rx_tail);

        /* Increment data count and decrement buffer size count */
        bytes++;
        buflen--;
    }

    /* Re-enable UART interrupts */
    UART_IntConfig(UARTPort, UART_INTCFG_RBR, ENABLE);

    return bytes;
}

//add by yxq





/*********************************************************************************************************
* Function name:	    Uart2Init
* Descriptions:	    	串口初始化，设置奇偶校验，波特率,停止位
* input parameters:    	Baud_rate	波特率
*						Parity		校验位
*						Stopbits	停止位
* output parameters:   	无
* Returned value:      	无
*********************************************************************************************************/
void Uart2Init(uint32_t Baud_rate,UART_PARITY_Type Parity,UART_STOPBIT_Type Stopbits)//freemodubs 接收，用于数据上传。比如说历史记录上传。
{
    /* UART Configuration structure variable*/
    UART_CFG_Type UARTConfigStruct;
    /* UART FIFO configuration Struct variable*/
    UART_FIFO_CFG_Type UARTFIFOConfigStruct;
    /* Pin configuration for UART1*/
    PINSEL_CFG_Type PinCfg;

    Tx2IntStat = RESET;

    /* Init UART pin connect*/
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 10;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 11;
    PINSEL_ConfigPin(&PinCfg);



    /* Init UART pin connect*/
//	PinCfg.Funcnum = 2;
//	PinCfg.OpenDrain = 0;
//	PinCfg.Pinmode = 0;
//	PinCfg.Portnum = 2;
//	PinCfg.Pinnum = 9;
//	PINSEL_ConfigPin(&PinCfg);
//	PinCfg.Pinnum = 8;
//	PINSEL_ConfigPin(&PinCfg);

    /* Initialize UART Configuration parameter structure to default state:
     * Baudrate = 9600bps
     * 8 data bit
     * 1 Stop bit
     * None parity
     */
    UART_ConfigStructInit(&UARTConfigStruct);
    UARTConfigStruct.Baud_rate = Baud_rate;
    UARTConfigStruct.Parity=Parity;
    UARTConfigStruct.Stopbits=Stopbits;
    /* Initialize UART1 peripheral with given to corresponding parameter*/
    UART_Init((LPC_UART_TypeDef *)LPC_UART2, &UARTConfigStruct);

    /* Initialize FIFOConfigStruct to default state:
     * 				- FIFO_DMAMode = DISABLE
     * 				- FIFO_Level = UART_FIFO_TRGLEV0
     * 				- FIFO_ResetRxBuf = ENABLE
     * 				- FIFO_ResetTxBuf = ENABLE
     * 				- FIFO_State = ENABLE
     */
    UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

    /* Initialize FIFO for UART1 peripheral*/
    UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART2, &UARTFIFOConfigStruct);

    UART_TxCmd((LPC_UART_TypeDef *)LPC_UART2, ENABLE);
    /* Enable UART Rx interrupt */
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART2, UART_INTCFG_RBR, ENABLE);
    /* Enable UART line status interrupt */
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART2, UART_INTCFG_THRE, ENABLE);

    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(UART2_IRQn, ((PRIO_GROUP_UART2<<3)|PRIO_UART2));
    /* Enable Interrupt for UART1 channel */
    NVIC_EnableIRQ(UART2_IRQn);

    /* Reset ring buf head and tail idx*/
    __BUF_RESET(uart2RingBuffer.rx_head);
    __BUF_RESET(uart2RingBuffer.rx_tail);
    __BUF_RESET(uart2RingBuffer.tx_head);
    __BUF_RESET(uart2RingBuffer.tx_tail);
}

/*********************************************************************************************************
* Function name:	    UART2_IRQHandler
* Descriptions:	    	串口2中断处理函数
* input parameters:    	无
* output parameters:   	无
* Returned value:      	无
*********************************************************************************************************/
void UART2_IRQHandler(void)
{
    uint32_t intsrc, tmp, tmp1;

    /* Determine the interrupt source */
    intsrc = UART_GetIntId(LPC_UART2);
    tmp = intsrc & UART_IIR_INTID_MASK;

    /* Receive Line Status*/
    if (tmp == UART_IIR_INTID_RLS) {
        // Check line status
        tmp1 = UART_GetLineStatus(LPC_UART2);
        // Mask out the Receive Ready and Transmit Holding empty status
        tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
                 | UART_LSR_BI | UART_LSR_RXFE);
        // If any error exist
        if (tmp1) {
//				UART_IntErr(tmp1);
        }
    }

    // Receive Data Available or Character time-out
    if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)) {
//			UART2_IntReceive();
        prvvUARTRxISR();
    }

    // Transmit Holding Empty
    if (tmp == UART_IIR_INTID_THRE) {
        if (Tx2IntStat == SET) {
            UART2_IntTransmit();
        }

        prvvUARTTxReadyISR();
    }

}

/*********************************************************************************************************
* Function name:	    UART2_IntReceive
* Descriptions:	    	串口2中断接受函数
* input parameters:    	无
* output parameters:   	无
* Returned value:      	无
*********************************************************************************************************/
void UART2_IntReceive(void)
{
    uint8_t tmpc;
    uint32_t rLen;

    while(1)
    {
        // Call UART read function in UART driver
        rLen = UART_Receive((LPC_UART_TypeDef *)LPC_UART2, &tmpc, 1, NONE_BLOCKING);
        // If data received
        if (rLen)
        {
            /* Check if buffer is more space
             * If no more space, remaining character will be trimmed out
             */
            if (!__BUF_IS_FULL(uart2RingBuffer.rx_head,uart2RingBuffer.rx_tail))
            {
                uart2RingBuffer.rx[uart2RingBuffer.rx_head] = tmpc;
                __BUF_INCR(uart2RingBuffer.rx_head);

            }
        }
        else
        {
            break;
        }
    }
}

/*********************************************************************************************************
* Function name:	    UART2_IntReceive
* Descriptions:	    	串口2中断发送函数
* input parameters:    	无
* output parameters:   	无
* Returned value:      	无
*********************************************************************************************************/
void UART2_IntTransmit(void)
{
    // Disable THRE interrupt
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART2, UART_INTCFG_THRE, DISABLE);

    /* Wait for FIFO buffer empty, transfer UART_TX_FIFO_SIZE bytes
     * of data or break whenever ring buffers are empty */
    /* Wait until THR empty */
    while (UART_CheckBusy((LPC_UART_TypeDef *)LPC_UART2) == SET);

    while (!__BUF_IS_EMPTY(uart2RingBuffer.tx_head,uart2RingBuffer.tx_tail))
    {
        /* Move a piece of data into the transmit FIFO */
        if (UART_Send((LPC_UART_TypeDef *)LPC_UART2, (uint8_t *)&uart2RingBuffer.tx[uart2RingBuffer.tx_tail], 1, BLOCKING)) {
            /* Update transmit ring FIFO tail pointer */
            __BUF_INCR(uart2RingBuffer.tx_tail);
        } else {
            break;
        }
    }

    /* If there is no more data to send, disable the transmit
       interrupt - else enable it or keep it enabled */
    if (__BUF_IS_EMPTY(uart2RingBuffer.tx_head, uart2RingBuffer.tx_tail)) {
        UART_IntConfig((LPC_UART_TypeDef *)LPC_UART2, UART_INTCFG_THRE, DISABLE);
        // Reset Tx Interrupt state
        Tx2IntStat = RESET;

    }
    else {
        // Set Tx Interrupt state
        Tx2IntStat = SET;
        UART_IntConfig((LPC_UART_TypeDef *)LPC_UART2, UART_INTCFG_THRE, ENABLE);
    }
}

/*********************************************************************************************************
* Function name:	    UART2Send
* Descriptions:	    	UART transmit function for interrupt mode (using ring buffers)
* input parameters:    	buflen Length of Transmit buffer
* output parameters:   	txbuf Pointer to Transmit buffer
* Returned value:      	Number of bytes actually sent to the ring buffer
*********************************************************************************************************/
uint32_t UART2Send(LPC_UART_TypeDef *UARTPort, uint8_t *txbuf, uint8_t buflen)
{
    uint8_t *data = (uint8_t *) txbuf;
    uint32_t bytes = 0;

    /* Temporarily lock out UART transmit interrupts during this
       read so the UART transmit interrupt won't cause problems
       with the index values */
    UART_IntConfig(UARTPort, UART_INTCFG_THRE, DISABLE);

    /* Loop until transmit run buffer is full or until n_bytes
       expires */
    while ((buflen > 0) && (!__BUF_IS_FULL(uart2RingBuffer.tx_head, uart2RingBuffer.tx_tail)))
    {
        /* Write data from buffer into ring buffer */
        uart2RingBuffer.tx[uart2RingBuffer.tx_head] = *data;
        data++;

        /* Increment head pointer */
        __BUF_INCR(uart2RingBuffer.tx_head);

        /* Increment data count and decrement buffer size count */
        bytes++;
        buflen--;
    }

    /*
     * Check if current Tx interrupt enable is reset,
     * that means the Tx interrupt must be re-enabled
     * due to call UART_IntTransmit() function to trigger
     * this interrupt type
     */
    if (Tx2IntStat == RESET) {
        UART2_IntTransmit();
    }
    /*
     * Otherwise, re-enables Tx Interrupt
     */
    else {
        UART_IntConfig(UARTPort, UART_INTCFG_THRE, ENABLE);
    }

    return bytes;
}

/*********************************************************************************************************
* Function name:	    UART2Receive
* Descriptions:	    	UART read function for interrupt mode (using ring buffers)
* input parameters:    	buflen Length of Received buffer
* output parameters:   	txbuf Pointer to Transmit buffer
* Returned value:      	Number of bytes actually read from the ring buffer
*********************************************************************************************************/
uint32_t UART2Receive(LPC_UART_TypeDef *UARTPort, uint8_t *rxbuf, uint8_t buflen)
{
    uint8_t *data = (uint8_t *) rxbuf;
    uint32_t bytes = 0;

    /* Temporarily lock out UART receive interrupts during this
       read so the UART receive interrupt won't cause problems
       with the index values */
    UART_IntConfig(UARTPort, UART_INTCFG_RBR, DISABLE);

    /* Loop until receive buffer ring is empty or
    	until max_bytes expires */
    while ((buflen > 0) && (!(__BUF_IS_EMPTY(uart2RingBuffer.rx_head, uart2RingBuffer.rx_tail))))
    {
        /* Read data from ring buffer into user buffer */
        *data = uart2RingBuffer.rx[uart2RingBuffer.rx_tail];
        data++;

        /* Update tail pointer */
        __BUF_INCR(uart2RingBuffer.rx_tail);

        /* Increment data count and decrement buffer size count */
        bytes++;
        buflen--;
    }

    /* Re-enable UART interrupts */
    UART_IntConfig(UARTPort, UART_INTCFG_RBR, ENABLE);

    return bytes;
}


/*********************************************************************************************************
* Function name:	    Uart3Init
* Descriptions:	    	串口初始化，设置奇偶校验，波特率,停止位
* input parameters:    	Baud_rate	波特率
*						Parity		校验位
*						Stopbits	停止位
* output parameters:   	无
* Returned value:      	无
*********************************************************************************************************/
void Uart3Init(uint32_t Baud_rate,UART_PARITY_Type Parity,UART_STOPBIT_Type Stopbits)//周立功modbus 用于控制接口板。比如4到20ma输出什么的。
{
    // UART Configuration structure variable
    UART_CFG_Type UARTConfigStruct;
    // UART FIFO configuration Struct variable
    UART_FIFO_CFG_Type UARTFIFOConfigStruct;
    // Pin configuration for UART3
    PINSEL_CFG_Type PinCfg;
    Tx3IntStat = SET;


//	PinCfg.Funcnum = 2;
//	PinCfg.OpenDrain = 0;
//	PinCfg.Pinmode = 0;
//	PinCfg.Portnum = 0;
//	PinCfg.Pinnum = 0;
//	PINSEL_ConfigPin(&PinCfg);
//	PinCfg.Pinnum = 1;
//	PINSEL_ConfigPin(&PinCfg);


    PinCfg.Funcnum = 3;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 25;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 26;
    PINSEL_ConfigPin(&PinCfg);




// 	GPIO_SetDir(0, (1<<1), 1);
// 	GPIO_ClearValue(0,(1<<1));
// //	GPIO_SetValue(0,(1<<20));
// 	PinCfg.Funcnum = 3;
// 	PinCfg.OpenDrain = 0;
// 	PinCfg.Pinmode = 0;
// 	PinCfg.Portnum = 4;
// 	PinCfg.Pinnum = 28;
// 	PINSEL_ConfigPin(&PinCfg);
// 	PinCfg.Pinnum = 29;
// 	PINSEL_ConfigPin(&PinCfg);

    /* Initialize UART Configuration parameter structure to default state:
     * Baudrate = 9600bps
     * 8 data bit
     * 1 Stop bit
     * None parity
     */
    UART_ConfigStructInit(&UARTConfigStruct);
    UARTConfigStruct.Baud_rate = Baud_rate;
    UARTConfigStruct.Parity=Parity;
    UARTConfigStruct.Stopbits=Stopbits;
    // Initialize UART3 peripheral with given to corresponding parameter
    UART_Init((LPC_UART_TypeDef *)LPC_UART3, &UARTConfigStruct);

    /* Initialize FIFOConfigStruct to default state:
     * 				- FIFO_DMAMode = DISABLE
     * 				- FIFO_Level = UART_FIFO_TRGLEV0
     * 				- FIFO_ResetRxBuf = ENABLE
     * 				- FIFO_ResetTxBuf = ENABLE
     * 				- FIFO_State = ENABLE
     */
    UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

    // Initialize FIFO for UART3 peripheral
    UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART3, &UARTFIFOConfigStruct);

    UART_TxCmd((LPC_UART_TypeDef *)LPC_UART3, ENABLE);
    /* DISEnable UART Rx interrupt */
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART3, UART_INTCFG_RBR, ENABLE);
    /* DISEnable UART line status interrupt */
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART3, UART_INTCFG_THRE, DISABLE);
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART3, UART_INTCFG_RLS, ENABLE);

    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(UART3_IRQn, ((PRIO_GROUP_UART3<<3)|PRIO_UART3));
    /* Enable Interrupt for UART3 channel */
    NVIC_EnableIRQ(UART3_IRQn);
//    NVIC_DisableIRQ(UART3_IRQn);
    // Reset ring buf head and tail idx
    __BUF_RESET(uart3RingBuffer.rx_head);
    __BUF_RESET(uart3RingBuffer.rx_tail);
    __BUF_RESET(uart3RingBuffer.tx_head);
    __BUF_RESET(uart3RingBuffer.tx_tail);
}

/*********************************************************************************************************
* Function name:	    UART3_IRQHandler
* Descriptions:	    	串口3中断处理函数
* input parameters:    	无
* output parameters:   	无
* Returned value:      	无
*********************************************************************************************************/
void UART3_IRQHandler(void)
{
    uint32_t intsrc, tmp, tmp1;
//	//modbus处理中 返回
//	if(!ModbusReceive)
//	{
//	 	return ;
//	}	/* Determine the interrupt source */
    intsrc = UART_GetIntId((LPC_UART_TypeDef *)LPC_UART3);
    tmp = intsrc & UART_IIR_INTID_MASK;

    // Receive Line Status
    if (tmp == UART_IIR_INTID_RLS) {
        // Check line status
        tmp1 = UART_GetLineStatus((LPC_UART_TypeDef *)LPC_UART3);
        // Mask out the Receive Ready and Transmit Holding empty status
        tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
                 | UART_LSR_BI | UART_LSR_RXFE);
        // If any error exist
        if (tmp1) {
            UART_IntErr(tmp1);
        }
    }

    // Receive Data Available or Character time-out
    if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)) {
        UART3_IntReceive();
//		prvvUARTRxISR();
    }
    // Transmit Holding Empty
    if (tmp == UART_IIR_INTID_THRE) {
        UART3_IntTransmit();
//		prvvUARTTxReadyISR();
    }

}

/*********************************************************************************************************
* Function name:	    UART3_IntReceive
* Descriptions:	    	串口3中断接受函数
* input parameters:    	无
* output parameters:   	无
* Returned value:      	无
*********************************************************************************************************/
void UART3_IntReceive(void)
{
    uint8_t tmpc;
    uint32_t rLen;

    while(1) {
        // Call UART read function in UART driver
        rLen = UART_Receive((LPC_UART_TypeDef *)LPC_UART3, &tmpc, 1, NONE_BLOCKING);
        // If data received
        if (rLen) {
            /* Check if buffer is more space
             * If no more space, remaining character will be trimmed out
             */

            ReceOneChar(tmpc);		// 调用MODBUS接收字符函数，;
//			if ((!__BUF_IS_FULL(uart3RingBuffer.rx_head,uart3RingBuffer.rx_tail)))
//			{
//				uart3RingBuffer.rx[uart3RingBuffer.rx_head] = tmpc;
//				__BUF_INCR(uart3RingBuffer.rx_head);
//
//
//				TIM_Cmd(LPC_TIM2,ENABLE);
////				T35Flag = 7;
////				T15Flag = 3;
//			}

//			prvvUARTRxISR();

        }
        // no more data
        else {
            break;
        }
    }
}

/*********************************************************************************************************
* Function name:	    UART3_IntReceive
* Descriptions:	    	串口3中断发送函数
* input parameters:    	无
* output parameters:   	无
* Returned value:      	无
*********************************************************************************************************/
void UART3_IntTransmit(void)
{
    // Disable THRE interrupt
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART3, UART_INTCFG_THRE, DISABLE);

    /* Wait for FIFO buffer empty, transfer UART_TX_FIFO_SIZE bytes
     * of data or break whenever ring buffers are empty */
    /* Wait until THR empty */
    while (UART_CheckBusy((LPC_UART_TypeDef *)LPC_UART3) == SET);

    while (!__BUF_IS_EMPTY(uart3RingBuffer.tx_head,uart3RingBuffer.tx_tail))
    {
        /* Move a piece of data into the transmit FIFO */
        if (UART_Send((LPC_UART_TypeDef *)LPC_UART3, (uint8_t *)&uart3RingBuffer.tx[uart3RingBuffer.tx_tail], \
                      1, NONE_BLOCKING)) {
            /* Update transmit ring FIFO tail pointer */
            __BUF_INCR(uart3RingBuffer.tx_tail);
        } else {
            break;
        }

    }

    /* If there is no more data to send, disable the transmit
       interrupt - else enable it or keep it enabled */
    if (__BUF_IS_EMPTY(uart3RingBuffer.tx_head, uart3RingBuffer.tx_tail)) {
        UART_IntConfig((LPC_UART_TypeDef *)LPC_UART3, UART_INTCFG_THRE, DISABLE);
        // Reset Tx Interrupt state
        Tx3IntStat = RESET;
//		prvvUARTTxReadyISR();
    }
    else {
        // Set Tx Interrupt state
        Tx3IntStat = SET;
        UART_IntConfig((LPC_UART_TypeDef *)LPC_UART3, UART_INTCFG_THRE, ENABLE);
//		prvvUARTTxReadyISR();
    }

}

/*********************************************************************************************************
* Function name:	    UART3Send
* Descriptions:	    	UART transmit function for interrupt mode (using ring buffers)
* input parameters:    	buflen Length of Transmit buffer
* output parameters:   	txbuf Pointer to Transmit buffer
* Returned value:      	Number of bytes actually sent to the ring buffer
*********************************************************************************************************/
uint32_t UART3Send(LPC_UART_TypeDef *UARTPort, uint8_t *txbuf, uint8_t buflen)
{
    uint8_t *data = (uint8_t *) txbuf;
    uint32_t bytes = 0;


    /* Temporarily lock out UART transmit interrupts during this
       read so the UART transmit interrupt won't cause problems
       with the index values */
    UART_IntConfig(UARTPort, UART_INTCFG_THRE, DISABLE);

    /* Loop until transmit run buffer is full or until n_bytes
       expires */
    while ((buflen > 0) && (!__BUF_IS_FULL(uart3RingBuffer.tx_head, uart3RingBuffer.tx_tail)))
    {
        /* Write data from buffer into ring buffer */
        uart3RingBuffer.tx[uart3RingBuffer.tx_head] = *data;
        data++;

        /* Increment head pointer */
        __BUF_INCR(uart3RingBuffer.tx_head);

        /* Increment data count and decrement buffer size count */
        bytes++;
        buflen--;
    }

    /*
     * Check if current Tx interrupt enable is reset,
     * that means the Tx interrupt must be re-enabled
     * due to call UART_IntTransmit() function to trigger
     * this interrupt type
     */
    if (Tx3IntStat == RESET) {
        UART3_IntTransmit();
    }
    /*
     * Otherwise, re-enables Tx Interrupt
     */
    else {
        UART_IntConfig(UARTPort, UART_INTCFG_THRE, ENABLE);
    }

    return bytes;
}

/*********************************************************************************************************
* Function name:	    UART3Receive
* Descriptions:	    	UART read function for interrupt mode (using ring buffers)
* input parameters:    	buflen Length of Received buffer
* output parameters:   	txbuf Pointer to Transmit buffer
* Returned value:      	Number of bytes actually read from the ring buffer
*********************************************************************************************************/
uint32_t UART3Receive(LPC_UART_TypeDef *UARTPort, uint8_t *rxbuf, uint8_t buflen)
{
    uint8_t *data = (uint8_t *) rxbuf;
    uint32_t bytes = 0;

    /* Temporarily lock out UART receive interrupts during this
       read so the UART receive interrupt won't cause problems
       with the index values */
    UART_IntConfig(UARTPort, UART_INTCFG_RBR, DISABLE);

    /* Loop until receive buffer ring is empty or
    	until max_bytes expires */
    while ((buflen > 0) && (!(__BUF_IS_EMPTY(uart3RingBuffer.rx_head, uart3RingBuffer.rx_tail))))
    {
        /* Read data from ring buffer into user buffer */
        *data = uart3RingBuffer.rx[uart3RingBuffer.rx_tail];
        data++;

        /* Update tail pointer */
        __BUF_INCR(uart3RingBuffer.rx_tail);

        /* Increment data count and decrement buffer size count */
        bytes++;
        buflen--;
    }

    /* Re-enable UART interrupts */
    UART_IntConfig(UARTPort, UART_INTCFG_RBR, ENABLE);

    return bytes;
}

/*********************************************************************************************************
* Function name:	    SetOutUart
* Descriptions:	    	UART Set for outside comm
* input parameters:    	baud	baud
*						parity
*						stop
* output parameters:   	None
* Returned value:      	None
*********************************************************************************************************/
void SetOutUart(uint16_t baud,uint16_t parity,uint16_t stop)
{
    uint32_t baudDat;
    UART_PARITY_Type parityDat;
    UART_STOPBIT_Type stopDat;

    if(0 == baud)
    {
        baudDat = 9600;
    }
    else
    {
        baudDat = 19200;
    }

    if(1 == parity)
    {
        parityDat = UART_PARITY_ODD;
    }
    else if(2 == parity)
    {
        parityDat = UART_PARITY_EVEN;
    }
    else
    {
        parityDat = UART_PARITY_NONE;
    }

    if(1 == stop)
    {
        stopDat = UART_STOPBIT_2;
    }
    else
    {
        stopDat = UART_STOPBIT_1;
    }

    Uart2Init(baudDat,parityDat,stopDat);
}



