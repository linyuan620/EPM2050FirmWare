#ifndef SPI_H
#define SPI_H

#include "globle.h"


#define		SCK			(1 << 8)					
#define		SO			(1 << 14)					
#define		SI			(1 << 9)
#define		WP			(1 << 10)					
#define		CE			(1 << 15)
#define		HOLD		(1 << 4)					

#define		SET_SCK(x)	((x) ? GPIO_SetValue(1, SCK): GPIO_ClearValue(1, SCK))
#define		SET_SO(x)	((x) ? GPIO_SetValue(1, SO): GPIO_ClearValue(1, SO))
#define		SET_SI(x)	((x) ? GPIO_SetValue(1, SI): GPIO_ClearValue(1, SI))
#define		SET_WP(x)	((x) ? GPIO_SetValue(1, WP): GPIO_ClearValue(1, WP))
#define		SET_CE(x)	((x) ? GPIO_SetValue(1, CE): GPIO_ClearValue(1, CE))
#define		SET_HOLD(x)	((x) ? GPIO_SetValue(1, HOLD): GPIO_ClearValue(1, HOLD))


//#define		SCK			(1 << 16)					
//#define		SO			(1 << 9)					
//#define		SI			(1 << 15)
//#define		WP			(1 << 8)					
//#define		CE			(1 << 8)
//#define		HOLD		(1 << 9)					
//
//#define		SET_SCK(x)	((x) ? GPIO_SetValue(0, SCK): GPIO_ClearValue(0, SCK))
//#define		SET_SO(x)	((x) ? GPIO_SetValue(0, SO): GPIO_ClearValue(0, SO))
//#define		SET_SI(x)	((x) ? GPIO_SetValue(0, SI): GPIO_ClearValue(0, SI))
//#define		SET_WP(x)	((x) ? GPIO_SetValue(0, WP): GPIO_ClearValue(0, WP))
//#define		SET_CE(x)	((x) ? GPIO_SetValue(2, CE): GPIO_ClearValue(2, CE))
//#define		SET_HOLD(x)	((x) ? GPIO_SetValue(2, HOLD): GPIO_ClearValue(2, HOLD))



#define MAX_ADDR		0x1fFFFFF	// 定义芯片内部最大地址 
#define	SEC_MAX     	512			// 定义最大扇区号
#define SEC_SIZE		0x1000      // 扇区大小
extern void SPIInit();
extern uint8 SSTF016B_RD(uint32 Dst, uint32 NByte,uint8* RcvBufPt)	;
extern uint8 SSTF016B_WR(uint32 Dst, uint8 const SndbufPt[], uint32 NByte);
extern char flash_chip_erase(void);  
extern  uint8 MX25L1602_RdID(uint8 IDType, uint32* RcvbufPt) ;
extern uint8 const hzk1616k1[][32];

#endif