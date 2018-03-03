#ifndef __TCPIP_H
#define __TCPIP_H

#include "global.h"
extern unsigned char Rx_Buffer[256];			/* 端口接收数据缓冲区 */
extern unsigned char Tx_Buffer[256];			/* 端口发送数据缓冲区 */
extern uint32 WEBxintiao;
extern unsigned char IP_Addr[4];				/* Loacal IP Address */
extern unsigned char Phy_Addr[6];				/* Loacal IP Address */



#endif

