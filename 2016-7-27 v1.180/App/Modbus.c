/************************************************************
  Copyright (C), 2011-8-3 ,  Skyray Instrment Co.,LTD.
  FileName: test.cpp
  Author:  			方卫龙      Version :  	v2.5        Date:	 	2011-8-3 11:23:34
  Description:     	modbus功能码相关函数
  Version:         	v2.5
  Function List:
    				1. eMBRegInputCB( UCHAR * pucRegBuffer,
										USHORT usAddress,
										USHORT usNRegs )  			//输入寄存器
					2. eMBRegHoldingCB( UCHAR * pucRegBuffer,
										USHORT usAddress, USHORT usNRegs,
										eMBRegisterMode eMode )	   	//保持寄存器
					3.其他暂时没用到
  History:
    <author>  <time>   <version >   <desc>
	方卫龙    11/5/2     1.0     build this moudle
	何顺杰    2011-8-4   2.5	 规范了注释
***********************************************************/


#include "Modbus.h"
/* ----------------------- Variables ---------------------------------*/
USHORT   usDiscreteInputStart                             = DISCRETE_INPUT_START;
UCHAR    usDiscreteInputBuf[DISCRETE_INPUT_NDISCRETES/8]  ;
USHORT   usCoilStart                                      = COIL_START;
UCHAR    usCoilBuf[COIL_NCOILS/8]                         ;
USHORT   usRegInputStart                                  = REG_INPUT_START;
USHORT   usRegInputBuf[REG_INPUT_NREGS]                   ;
USHORT   usRegHoldingStart                                = REG_HOLDING_START;
USHORT   usRegHoldingBuf[REG_HOLDING_NREGS]               ;
/*******************************输入寄存器回调函数**********************************
//函数定义: eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
//描    述：输入寄存器相关的功能（读、连续读）
//入口参数：pucRegBuffer : 回调函数将Modbus寄存器的当前值写入的缓冲区
//			usAddress    : 寄存器的起始地址，输入寄存器的地址范围是1-65535。
//			usNRegs      : 寄存器数量
//出口参数：eMBErrorCode : 这个函数将返回的错误码
//备    注：Editor：Armink 2010-10-31    Company: BXXJS
**********************************************************************************/
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
    /*usAddress 大小符合范围*/
    if( ( usAddress >= REG_INPUT_START )
            && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {   /*从第几个数据开始*/
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {   /* 先高位	 后低位*/
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {   /*错误*/
        eStatus = MB_ENOREG;
    }

    return eStatus;
}
/*******************************保持寄存器回调函数**********************************
//函数定义: eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
//描    述：保持寄存器相关的功能（读、连续读、写、连续写）
//入口参数：pucRegBuffer : 如果需要更新用户寄存器数值，这个缓冲区必须指向新的寄存器数值。
//                         如果协议栈想知道当前的数值，回调函数必须将当前值写入这个缓冲区
//			usAddress    : 寄存器的起始地址。
//			usNRegs      : 寄存器数量
//          eMode        : 如果该参数为eMBRegisterMode::MB_REG_WRITE，用户的应用数值将从pucRegBuffer中得到更新。
//                         如果该参数为eMBRegisterMode::MB_REG_READ，用户需要将当前的应用数据存储在pucRegBuffer中
//出口参数：eMBErrorCode : 这个函数将返回的错误码
//备    注：Editor：Armink 2010-10-31    Company: BXXJS
**********************************************************************************/
eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{

    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
    if( ( usAddress >= REG_HOLDING_START ) &&
            ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart );
        /*操作类型 读后者写*/
        switch ( eMode )
        {
            /* Pass current register values to the protocol stack. */
        case MB_REG_READ:
            while( usNRegs > 0 )
            {   /*先高位后低位*/
                *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            break;

            /* Update current register values with new values from the
             * protocol stack. */
        case MB_REG_WRITE:
            while( usNRegs > 0 )
            {   /*先赋高位，后赋低位*/
                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                /* 地址 */
                switch(iRegIndex)
                {   /*	电流增益*/
                case 3:
                    break;
                    /*  镀膜时间*/
                case 100:
                    break;
                    /*  镀膜电压*/
                case 101:
                    break;
                    /*  调节参数*/
                default:
                    break;

                }
                iRegIndex++;
                usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}
/*
	没有用到
*/
//****************************线圈状态寄存器回调函数********************************
//函数定义: eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
//描    述：线圈状态寄存器相关的功能（读、连续读、写、连续写）
//入口参数：pucRegBuffer : 位组成一个字节，起始寄存器对应的位处于该字节pucRegBuffer的最低位LSB。
//                         如果回调函数要写这个缓冲区，没有用到的线圈（例如不是8个一组的线圈状态）对应的位的数值必须设置位0。
//			usAddress    : 第一个线圈地址。
//			usNCoils     : 请求的线圈个数
//          eMode        ；如果该参数为eMBRegisterMode::MB_REG_WRITE，用户的应用数值将从pucRegBuffer中得到更新。
//                         如果该参数为eMBRegisterMode::MB_REG_READ，用户需要将当前的应用数据存储在pucRegBuffer中
//出口参数：eMBErrorCode : 这个函数将返回的错误码
//备    注：Editor：Armink 2010-10-31    Company: BXXJS
//**********************************************************************************

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex , iRegBitIndex , iNReg;
    iNReg =  usNCoils / 8 + 1;        //占用寄存器数量
    if( ( usAddress >= COIL_START ) &&
            ( usAddress + usNCoils <= COIL_START + COIL_NCOILS ) )
    {
        iRegIndex    = ( int )( usAddress - usCoilStart ) / 8 ;    //每个寄存器存8个
        iRegBitIndex = ( int )( usAddress - usCoilStart ) % 8 ;	   //相对于寄存器内部的位地址
        switch ( eMode )
        {
            /* Pass current coil values to the protocol stack. */
        case MB_REG_READ:
            while( iNReg > 0 )
            {
                *pucRegBuffer++ = xMBUtilGetBits(&usCoilBuf[iRegIndex++] , iRegBitIndex , 8);
                iNReg --;
            }
            pucRegBuffer --;
            usNCoils = usNCoils % 8;                        //余下的线圈数
            *pucRegBuffer = *pucRegBuffer <<(8 - usNCoils); //高位补零
            *pucRegBuffer = *pucRegBuffer >>(8 - usNCoils);
            break;

            /* Update current coil values with new values from the
             * protocol stack. */
        case MB_REG_WRITE:
            while(iNReg > 1)									 //最后面余下来的数单独算
            {
                xMBUtilSetBits(&usCoilBuf[iRegIndex++] , iRegBitIndex  , 8 , *pucRegBuffer++);
                iNReg--;
            }
            usNCoils = usNCoils % 8;                            //余下的线圈数
            xMBUtilSetBits(&usCoilBuf[iRegIndex++] , iRegBitIndex  , usNCoils , *pucRegBuffer++);
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}
/*
	没有用到
*/
//****************************离散输入寄存器回调函数********************************
//函数定义: eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
//描    述：离散输入寄存器相关的功能（读、连续读）
//入口参数：pucRegBuffer : 用当前的线圈数据更新这个寄存器，起始寄存器对应的位处于该字节pucRegBuffer的最低位LSB。
//                         如果回调函数要写这个缓冲区，没有用到的线圈（例如不是8个一组的线圈状态）对应的位的数值必须设置为0。
//			usAddress    : 离散输入的起始地址
//			usNDiscrete  : 离散输入点数量
//出口参数：eMBErrorCode : 这个函数将返回的错误码
//备    注：Editor：Armink 2010-10-31    Company: BXXJS
//**********************************************************************************
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex , iRegBitIndex , iNReg;
    iNReg =  usNDiscrete / 8 + 1;        //占用寄存器数量
    if( ( usAddress >= DISCRETE_INPUT_START )
            && ( usAddress + usNDiscrete <= DISCRETE_INPUT_START + DISCRETE_INPUT_NDISCRETES ) )
    {
        iRegIndex    = ( int )( usAddress - usDiscreteInputStart ) / 8 ;    //每个寄存器存8个
        iRegBitIndex = ( int )( usAddress - usDiscreteInputStart ) % 8 ;	   //相对于寄存器内部的位地址
        while( iNReg > 0 )
        {
            *pucRegBuffer++ = xMBUtilGetBits(&usDiscreteInputBuf[iRegIndex++] , iRegBitIndex , 8);
            iNReg --;
        }
        pucRegBuffer --;
        usNDiscrete = usNDiscrete % 8;                     //余下的线圈数
        *pucRegBuffer = *pucRegBuffer <<(8 - usNDiscrete); //高位补零
        *pucRegBuffer = *pucRegBuffer >>(8 - usNDiscrete);
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}
/*
	没有用到
*/
//******************************Modbus寄存器初始化函数**************************************
//函数定义: void ModbusRegInit(uint8_t Fun)
//描    述：把RAM中Modbus的保持寄存器的值存到Flash里，或把Flash中的数取出放到Modbus RAM寄存器
//入口参数：Fun 0 : 把RAM中Modbus的保持寄存器的值存到Flash里
//              1 : 把Flash中的数取出,放到Modbus的RAM寄存器中
//出口参数：无
//备    注：Editor：Armink 2010-11-3    Company: BXXJS
//**********************************************************************************
void ModbusRegInit(uint8_t Fun)
{
//	uint8_t i ;
//	uint32_t Addr ;
//	if (0 == Fun)
//	{
//		FLASH_Unlock();
//		FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
//		FLASH_ErasePage(FALSH_USER_START_ADDRESS + 0);//擦除第0页
//		for (i = 0 ,Addr = FALSH_USER_START_ADDRESS; i < REG_HOLDING_NREGS ; i ++ , Addr += 2)
//		{
//			 FLASH_ProgramHalfWord(Addr , usRegHoldingBuf[i]);    //16bit = 2byte = 1/2 word
//		}
//		FLASH_Lock();
//	}
//	else if (1 == Fun)
//	{
//		for (i = 0 ,Addr = FALSH_USER_START_ADDRESS; i < REG_HOLDING_NREGS ; i ++ , Addr += 2)
//		{
//			 usRegHoldingBuf[i] = (uint16_t)(*(__IO uint16_t*) (Addr));    //16bit = 2byte = 1/2 word
//		}
//	}

}
