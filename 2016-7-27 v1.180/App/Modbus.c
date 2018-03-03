/************************************************************
  Copyright (C), 2011-8-3 ,  Skyray Instrment Co.,LTD.
  FileName: test.cpp
  Author:  			������      Version :  	v2.5        Date:	 	2011-8-3 11:23:34
  Description:     	modbus��������غ���
  Version:         	v2.5
  Function List:
    				1. eMBRegInputCB( UCHAR * pucRegBuffer,
										USHORT usAddress,
										USHORT usNRegs )  			//����Ĵ���
					2. eMBRegHoldingCB( UCHAR * pucRegBuffer,
										USHORT usAddress, USHORT usNRegs,
										eMBRegisterMode eMode )	   	//���ּĴ���
					3.������ʱû�õ�
  History:
    <author>  <time>   <version >   <desc>
	������    11/5/2     1.0     build this moudle
	��˳��    2011-8-4   2.5	 �淶��ע��
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
/*******************************����Ĵ����ص�����**********************************
//��������: eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
//��    ��������Ĵ�����صĹ��ܣ�������������
//��ڲ�����pucRegBuffer : �ص�������Modbus�Ĵ����ĵ�ǰֵд��Ļ�����
//			usAddress    : �Ĵ�������ʼ��ַ������Ĵ����ĵ�ַ��Χ��1-65535��
//			usNRegs      : �Ĵ�������
//���ڲ�����eMBErrorCode : ������������صĴ�����
//��    ע��Editor��Armink 2010-10-31    Company: BXXJS
**********************************************************************************/
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
    /*usAddress ��С���Ϸ�Χ*/
    if( ( usAddress >= REG_INPUT_START )
            && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {   /*�ӵڼ������ݿ�ʼ*/
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {   /* �ȸ�λ	 ���λ*/
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {   /*����*/
        eStatus = MB_ENOREG;
    }

    return eStatus;
}
/*******************************���ּĴ����ص�����**********************************
//��������: eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
//��    �������ּĴ�����صĹ��ܣ�������������д������д��
//��ڲ�����pucRegBuffer : �����Ҫ�����û��Ĵ�����ֵ���������������ָ���µļĴ�����ֵ��
//                         ���Э��ջ��֪����ǰ����ֵ���ص��������뽫��ǰֵд�����������
//			usAddress    : �Ĵ�������ʼ��ַ��
//			usNRegs      : �Ĵ�������
//          eMode        : ����ò���ΪeMBRegisterMode::MB_REG_WRITE���û���Ӧ����ֵ����pucRegBuffer�еõ����¡�
//                         ����ò���ΪeMBRegisterMode::MB_REG_READ���û���Ҫ����ǰ��Ӧ�����ݴ洢��pucRegBuffer��
//���ڲ�����eMBErrorCode : ������������صĴ�����
//��    ע��Editor��Armink 2010-10-31    Company: BXXJS
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
        /*�������� ������д*/
        switch ( eMode )
        {
            /* Pass current register values to the protocol stack. */
        case MB_REG_READ:
            while( usNRegs > 0 )
            {   /*�ȸ�λ���λ*/
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
            {   /*�ȸ���λ���󸳵�λ*/
                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                /* ��ַ */
                switch(iRegIndex)
                {   /*	��������*/
                case 3:
                    break;
                    /*  ��Ĥʱ��*/
                case 100:
                    break;
                    /*  ��Ĥ��ѹ*/
                case 101:
                    break;
                    /*  ���ڲ���*/
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
	û���õ�
*/
//****************************��Ȧ״̬�Ĵ����ص�����********************************
//��������: eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
//��    ������Ȧ״̬�Ĵ�����صĹ��ܣ�������������д������д��
//��ڲ�����pucRegBuffer : λ���һ���ֽڣ���ʼ�Ĵ�����Ӧ��λ���ڸ��ֽ�pucRegBuffer�����λLSB��
//                         ����ص�����Ҫд�����������û���õ�����Ȧ�����粻��8��һ�����Ȧ״̬����Ӧ��λ����ֵ��������λ0��
//			usAddress    : ��һ����Ȧ��ַ��
//			usNCoils     : �������Ȧ����
//          eMode        ������ò���ΪeMBRegisterMode::MB_REG_WRITE���û���Ӧ����ֵ����pucRegBuffer�еõ����¡�
//                         ����ò���ΪeMBRegisterMode::MB_REG_READ���û���Ҫ����ǰ��Ӧ�����ݴ洢��pucRegBuffer��
//���ڲ�����eMBErrorCode : ������������صĴ�����
//��    ע��Editor��Armink 2010-10-31    Company: BXXJS
//**********************************************************************************

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex , iRegBitIndex , iNReg;
    iNReg =  usNCoils / 8 + 1;        //ռ�üĴ�������
    if( ( usAddress >= COIL_START ) &&
            ( usAddress + usNCoils <= COIL_START + COIL_NCOILS ) )
    {
        iRegIndex    = ( int )( usAddress - usCoilStart ) / 8 ;    //ÿ���Ĵ�����8��
        iRegBitIndex = ( int )( usAddress - usCoilStart ) % 8 ;	   //����ڼĴ����ڲ���λ��ַ
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
            usNCoils = usNCoils % 8;                        //���µ���Ȧ��
            *pucRegBuffer = *pucRegBuffer <<(8 - usNCoils); //��λ����
            *pucRegBuffer = *pucRegBuffer >>(8 - usNCoils);
            break;

            /* Update current coil values with new values from the
             * protocol stack. */
        case MB_REG_WRITE:
            while(iNReg > 1)									 //���������������������
            {
                xMBUtilSetBits(&usCoilBuf[iRegIndex++] , iRegBitIndex  , 8 , *pucRegBuffer++);
                iNReg--;
            }
            usNCoils = usNCoils % 8;                            //���µ���Ȧ��
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
	û���õ�
*/
//****************************��ɢ����Ĵ����ص�����********************************
//��������: eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
//��    ������ɢ����Ĵ�����صĹ��ܣ�������������
//��ڲ�����pucRegBuffer : �õ�ǰ����Ȧ���ݸ�������Ĵ�������ʼ�Ĵ�����Ӧ��λ���ڸ��ֽ�pucRegBuffer�����λLSB��
//                         ����ص�����Ҫд�����������û���õ�����Ȧ�����粻��8��һ�����Ȧ״̬����Ӧ��λ����ֵ��������Ϊ0��
//			usAddress    : ��ɢ�������ʼ��ַ
//			usNDiscrete  : ��ɢ���������
//���ڲ�����eMBErrorCode : ������������صĴ�����
//��    ע��Editor��Armink 2010-10-31    Company: BXXJS
//**********************************************************************************
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex , iRegBitIndex , iNReg;
    iNReg =  usNDiscrete / 8 + 1;        //ռ�üĴ�������
    if( ( usAddress >= DISCRETE_INPUT_START )
            && ( usAddress + usNDiscrete <= DISCRETE_INPUT_START + DISCRETE_INPUT_NDISCRETES ) )
    {
        iRegIndex    = ( int )( usAddress - usDiscreteInputStart ) / 8 ;    //ÿ���Ĵ�����8��
        iRegBitIndex = ( int )( usAddress - usDiscreteInputStart ) % 8 ;	   //����ڼĴ����ڲ���λ��ַ
        while( iNReg > 0 )
        {
            *pucRegBuffer++ = xMBUtilGetBits(&usDiscreteInputBuf[iRegIndex++] , iRegBitIndex , 8);
            iNReg --;
        }
        pucRegBuffer --;
        usNDiscrete = usNDiscrete % 8;                     //���µ���Ȧ��
        *pucRegBuffer = *pucRegBuffer <<(8 - usNDiscrete); //��λ����
        *pucRegBuffer = *pucRegBuffer >>(8 - usNDiscrete);
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}
/*
	û���õ�
*/
//******************************Modbus�Ĵ�����ʼ������**************************************
//��������: void ModbusRegInit(uint8_t Fun)
//��    ������RAM��Modbus�ı��ּĴ�����ֵ�浽Flash����Flash�е���ȡ���ŵ�Modbus RAM�Ĵ���
//��ڲ�����Fun 0 : ��RAM��Modbus�ı��ּĴ�����ֵ�浽Flash��
//              1 : ��Flash�е���ȡ��,�ŵ�Modbus��RAM�Ĵ�����
//���ڲ�������
//��    ע��Editor��Armink 2010-11-3    Company: BXXJS
//**********************************************************************************
void ModbusRegInit(uint8_t Fun)
{
//	uint8_t i ;
//	uint32_t Addr ;
//	if (0 == Fun)
//	{
//		FLASH_Unlock();
//		FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
//		FLASH_ErasePage(FALSH_USER_START_ADDRESS + 0);//������0ҳ
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
