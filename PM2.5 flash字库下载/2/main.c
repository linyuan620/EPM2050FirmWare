/***********************************************************************//**
 * @file		uart_interrupt_test.c
 * @purpose		This example describes how to using UART in interrupt mode
 * @version		2.0
 * @date		21. May. 2010
 * @author		NXP MCU SW Application Team
 *---------------------------------------------------------------------
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 **********************************************************************/
//#include "lpc17xx_uart.h"
//#include "lpc17xx_libcfg.h"
//#include "lpc17xx_pinsel.h"
#include"uart.h"

#include"Globle.h"


#define		BUZZER			(1 << 28)					/*P0.6 蜂鸣器*/
#define		SET_BUZZER(x)	((x) ? GPIO_SetValue(0, BUZZER): GPIO_ClearValue(0, BUZZER))


void BuzzerIOInit(uint8 OnOff)
{
	if(OnOff)
	{
		GPIO_SetDir(0,(BUZZER ),1);
	}
	else
	{
		GPIO_SetDir(0,(BUZZER ),0);
	}
}

void SetBuzzer(uint8 OnOff)
{
	switch ( OnOff )
    {
	    case 1:
			SET_BUZZER(1);
			break;
	    case 0:
		default:
			SET_BUZZER(0);
	        break;
    }
}






/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] = "Hello NXP Semiconductors \n\r";
uint8_t menu2[] =
"UART interrupt mode demo using ring buffer \n\r\t "
"MCU LPC17xx - ARM Cortex-M3 \n\r\t "
"UART0 - 9600bps \n\r";
uint8_t menu3[] = "UART demo terminated!\n";
uint8_t menu4[] = "heshunjie wahaha！\n";
uint8_t buf[50];
void print_menu(void);


/*********************************************************************//**
 * @brief	Print Welcome Screen Menu subroutine
 * @param	None
 * @return	None
 **********************************************************************/
void print_menu(void)
{
	uint32_t tmp, tmp2;
	uint8_t *pDat;

	tmp = sizeof(menu1);
	tmp2 = 0;
	pDat = (uint8_t *)&menu1[0];
	while(tmp) {
		tmp2 = UARTSend((LPC_UART_TypeDef *)LPC_UART0, pDat, tmp);
		pDat += tmp2;
		tmp -= tmp2;
	}

	tmp = sizeof(menu2);
	tmp2 = 0;
	pDat = (uint8_t *)&menu2[0];
	while(tmp) {
		tmp2 = UARTSend((LPC_UART_TypeDef *)LPC_UART0, pDat, tmp);
		pDat += tmp2;
		tmp -= tmp2;
	}
}



/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		c_entry: Main UART program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry(void)
{
    uint32 GulChipID = 0,i;
  
    uint8 len,buffer[34],idx,a;
//	uint8 dat[15]={1,2,3,4,5,6,};
//	uint8 dat1[15]={7,6,5,4,3,2,1,1};
		BuzzerIOInit(1);		 			//蜂鸣
	SetBuzzer(0);		  				//关蜂鸣

	SPIInit();
// 	flash_chip_erase();
//    MX25L1602_RdID(1, &GulChipID);                               /* 单步运行到此处时, 查看ChipID */
    /* Read some data from the buffer */
// 		SSTF016B_RD(0, 15,buffer);
//
//		SSTF016B_WR( 0, dat1,15);
//
// 		SSTF016B_RD(0, 15,buffer);

//写 16x16的字库
//	for(i = 0;i<6767;i++)
//	{
//		SSTF016B_WR( i*32, hzk1616k1[i],32);
//	}
//写12*12的字库
	for(i = 0;i<6767;i++)
	{
		SSTF016B_WR(216640+i*24,hzk1212[i],24);
	}



//	for(i = 0;i<2;i++)
//	{
// 		SSTF016B_RD(i*32, 32,buffer);
//
//	}
//	for(i = 0;i<10;i++)
//	{
//		SSTF016B_RD(216640+24*(6757+i),  24,buffer);
//
//	}
				
    while (1);
	return 0;
}

/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
int main(void)
{
    return c_entry();
}
