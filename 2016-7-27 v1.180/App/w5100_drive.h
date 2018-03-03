//ESA中用到的命令有1234 9 11 12 13 14 16  1z  21  23 24  29

#ifndef __w5100__h
#define __w5100__h

#include "global.h"

#define W5100_BUFSIZE 50
#define W5100START  0X02
#define W5100END  0X03

#define SYNCHRONIZATION  1
#define CONFIREAD  2
#define QUARTEROFHOUR  3
#define INSTANTANEOUS  4
#define MEASURE  9
#define MUX  11
#define SOFTWARE  12
#define DAYSINMEMORY  13
#define STAND_BY  14
#define INSTANTANEOUS_FLOAT  16
// #define RESTART  1Z
#define PARTSYNCHRONIZATION  21
#define RECOVERYDATA  23
#define INSTANTANEOUS4MODE  24
#define IDENTIFICATION  29



extern unsigned char Rx_Buffer[256];			/* 端口接收数据缓冲区 */
extern unsigned char Tx_Buffer[256];			/* 端口发送数据缓冲区 */
extern unsigned char S_tx_process(uint8 s, unsigned int size);


#endif

