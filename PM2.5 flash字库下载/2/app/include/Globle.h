
#ifndef GLOBLE_H_
#define GLOBLE_H_
typedef  unsigned char uint8 ;
typedef  unsigned short uint16 ;
typedef  unsigned int uint32 ;
typedef  signed char int8	;
typedef  signed short int16	;
typedef  signed int int32  ;

#include "lpc17xx_uart.h"
#include "lpc17xx_libcfg.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"

#include"UART.H"





#include"Dio.H"
#include"GPS.H"
#include"SPI.H"


#include "string.h"





extern const uint8 hzk1212[][24];

extern uint8_t buf[50];
#endif 
