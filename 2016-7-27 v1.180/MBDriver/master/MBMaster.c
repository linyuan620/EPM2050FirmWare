/****************************************Copyright (c)**************************************************
**                                  广州致远电子有限公司
**
**                                 http://www.21cm.com.cn
**
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			MBMaster.c
** Last modified Date:  2005-01-11
** Last Version:		1.0
** Descriptions:		
**
**------------------------------------------------------------------------------------------------------
** Created by:			ZhouLishan
** Created date:		2005-01-11
** Version:				1.0
** Descriptions:		
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
********************************************************************************************************/
#include "config.h"

#define ERR_ADRRESS 	0xff		// 设备地址无效
#define ERR_PARAMETER	0xfe		// 输入参数无效出错

#define ERR_TIMEOUT		0xfd		// 没应答超时出错
#define ERR_FRAME		0xfc		// 接收到的帧出错（CRC出错）
#define ERR_NON			0x00		// 正常返回


#define MB_POLL_TIMEOUT	 1//	100
#define MB_POLL_DELAY	1//	5


extern FUNCTION_PACK_PDU FunPDUPackHanlde[MAX_FUNCTION];
extern FUNCTION_DATA_HANDLE FunParRepAndStData[MAX_FUNCTION];

uint16 Modbus_CRC16(uint8 *Buff_addr,uint16 len);
void SendResponse(uint8 *buff,uint16 len);

//*****************************************************************************
// 函数名称：
// 输入参数：
// 输出参数：
// 功能描述：
//*****************************************************************************

// uint8 PDUBuffer[256];//发送？

uint8 ADUBuffer[256];//接收？

extern int g_WaitN10mS;

#ifdef UCOSII
	OS_EVENT *mb_handle_event_ptr;
	OS_EVENT *mb_reply_event_ptr;
	OS_EVENT *mb_idle_event_ptr;
#endif

MASTER_INFORMATION MB_Master;
//********************************************************************************************************
// 函数名称：MBSlaveIni
// 输入参数：无
// 输出参数：无
// 功能描述：MODBUS从机初始化
//********************************************************************************************************
void MBMasterIni(void)
{

	MB_Master.Status = MB_MASTER_IDLE;	// 主机空闲
	ADUData.Address = 0;
	ADUData.ADUBuffPtr = ADUBuffer;
	ADUData.ADULength = 0;

//	ADUData.RedundancyCheck = 0;
	ADUData.EndT15 = FALSE;
	ADUData.FrameOK = FALSE;
//	ADUData.OverTime = FALSE;
	PDUData.PDUBuffPtr = NULL;
//	PDUData.Response.ExceptionCode = 0;
#ifdef UCOSII
	mb_idle_event_ptr   = OSSemCreate(1);			// MODBUS主机空闲信号量
	mb_handle_event_ptr = OSSemCreate(0);			// MODBUS处理信号量
	mb_reply_event_ptr  = OSMboxCreate((void *)0);	// MODBUS处理应答消息
#endif
}


//******************************************************************************************
// 函数名称：ModbusPoll
// 输入参数：NodeID,节点ID（设备地址）
// 			 FunctionCode，功能代码
//			 ParameterPtr，参数入口
// 输出参数：err,指令执行状态
// 功能描述：
//******************************************************************************************

uint8 ModbusPoll(uint8 NodeID,uint8 FunctionCode,void *ParameterPtr)
{
	uint8 i,err;
	ADUData.Address = NodeID;
	

	for(i=0;i<MAX_FUNCTION;i++)							// 查打有效的功能代码			
	{	if(FunPDUPackHanlde[i].Code==FunctionCode)		// 查找功能代码处理函数
		{
		#ifdef UCOSII
			OSSemPend(mb_idle_event_ptr,0,&err);		// 等待可以操作MODBUS主机的一个信号量	
		#else
			if(MB_Master.Status!=(uint8)MB_MASTER_IDLE)
				return (uint8)MB_MASTER_BUSY;			// 无效指令返回
		#endif	
			PDUData.PDUBuffPtr = ADUData.ADUBuffPtr+1;	// 设置PDU起始指针
			FunPDUPackHanlde[i].Function(ParameterPtr);	// 调用功能代码处理函数
			PDUData.FrameOK = TRUE;						// 请求帧准备就绪
			MB_Master.ParPtr = ParameterPtr;			// 请求参数结构指针
			
		#ifdef UCOSII
			OSSemPost(mb_handle_event_ptr);				// 发报信号量,使MODBUS服务任务处理传输
			err = *(uint8 *)OSMboxPend(mb_reply_event_ptr,0,&err);	// 等待执行完成	
		#else
			do
			{
				err = IdleModbus();
			}while(MB_Master.Status==(uint8)MB_MASTER_BUSY);
		#endif		
			return err;	// 无效指令返回

		}
	}
	return (uint8)MB_FUN_NOT_DEF;
}


//******************************************************************************************
// 函数名称：ParseReponseAndStoreData
// 输入参数：无
// 输出参数：无
// 功能描述：解析应答帧，并保存数据
//******************************************************************************************
uint8 ParseReponseAndStoreData(void)
{
	uint8 i,err;
	err = MB_FUN_NOT_DEF;
	for(i=0;i<MAX_FUNCTION;i++)							
		if(FunParRepAndStData[i].Code==PDUData.FunctionCode)		// 查找功能代码处理函数
		{
			if(PDUData.PDUBuffPtr[0]&0x80)	// 异常产生
			{	err = PDUData.PDUBuffPtr[1];}
			else	
			{	err = MB_NO_ERR;
				FunParRepAndStData[i].Function(PDUData.PDUBuffPtr,MB_Master.ParPtr);	// 调用功能代码处理函数
			
			}
			break;
		}
	return err;// 未定义功能码
}

//******************************************************************************************
// 函数名称：CheckModbusMessage
// 输入参数：无
// 输出参数：ADU描述符
// 功能描述：检查是否有需要发送的的帧
//******************************************************************************************
ADU_CONTROL *CheckModbusMessage(void)
{
	uint16 CRC16;
	uint8 *ADUPtr;
	if(PDUData.FrameOK == TRUE)			// 请求帧的PDU是否准备就绪
	{
		PDUData.FrameOK = FALSE;		// 清PDU是否准备就绪标志，当接收到有效的PDU后将被再次置1
		ADUPtr = ADUData.ADUBuffPtr;
		ADUPtr[0] = ADUData.Address;	// 设置ADU地址（节点）
		
		CRC16 = Modbus_CRC16(ADUPtr,PDUData.PDULength+1);	// 求CRC16值
		ADUPtr[PDUData.PDULength+1] = CRC16;				// 设置CRC16值
		ADUPtr[PDUData.PDULength+2] = CRC16>>8;
		ADUData.ADULength=PDUData.PDULength+3;		// ADU长度等于PDU长度加1个地址和2个CRC字符
		ADUData.FrameOK = TRUE;						// 标记ADU请求帧准备就绪
		
		return (ADU_CONTROL *) &ADUData;			
	}
	else
		return NULL;
}








//******************************************************************************************
// 函数名称：IdleModbus
// 输入参数：无
// 输出参数：MODBUS主机状态
// 功能描述：
//******************************************************************************************
uint8 IdleModbus(void)
{
	static uint8 err;
	static uint8  modbus_state=0;
	
#ifdef	UCOSII
	OSSemPend(mb_handle_event_ptr,0,&err);
#endif	
	
	err = MB_MASTER_BUSY;
	switch ( modbus_state )
	{
      	case 0:
         	/******************************************************************/
         	/*       CHECK FOR THE NEXT MESSAGE TO SEND                       */
         	/*       检查需要发送的消息，并发送请求帧						  */
         	/******************************************************************/
        	if ( CheckModbusMessage() )
			{     
				/******************************************************************/
	         	/*       SEND THE POLL                                            */
	         	/*		发送请求帧												  */
	         	/******************************************************************/
	         	SendResponse(ADUData.ADUBuffPtr,ADUData.ADULength);// 发送帧
	        	ADUData.FrameOK = FALSE;
	        	ADUData.ADULength = 0;
   				MB_Master.Status = (uint8)MB_MASTER_BUSY;				// 设置主机忙状态
	        	Waite10mS(MB_POLL_TIMEOUT);			// 命令应答计时 
	            modbus_state++;
 				
            }
            else
            	err = MB_MASTER_IDLE;	// 没有功能代码需传输
         	break;
      	case 1:
         	/******************************************************************/
         	/*       GET THE PACKET AND MAKE SURE IT IS COMPLETE              */
         	/******************************************************************/
         	if ( PDUData.FrameOK )	// 成功接收PDU帧
            {
                /******************************************************************/
         		/*       PARSE THE RESPONSE AND STORE THE DATA                    */
         		/*       解析应等帧，并储存数据									  */
         		/******************************************************************/
         		err = ParseReponseAndStoreData();
         		PDUData.FrameOK = FALSE;
         		Waite10mS(MB_POLL_DELAY);	
    		#ifdef	UCOSII
				OSMboxPost(mb_reply_event_ptr,&err);  		   
			#endif		 
            	modbus_state++;
            	break;
            }

         	/******************************************************************/
         	/*       IF NO RESPONSE RECEIVED TIME OUT AND SEND NEXT POLL      */
         	/******************************************************************/
         	if (g_TimeEnd)
            {
            	modbus_state = 0;
            	MB_Master.Status = MB_MASTER_IDLE;	// 主机空闲
            	err = MB_RESPONSE_TIME_OUT;
 			#ifdef	UCOSII
				OSMboxPost(mb_reply_event_ptr,&err);  		   
			#endif	
            }
         	break;
      	case 2:
         	/******************************************************************/
         	/*       POLL DELAY                                               */
         	/*		轮循延时												  */
         	/******************************************************************/

         	if (g_TimeEnd)
            {
               	modbus_state=0;
          		MB_Master.Status = MB_MASTER_IDLE;	// 主机空闲
            }
      	default:
         	break;
      }
      
#ifdef	UCOSII
	if(MB_Master.Status == MB_MASTER_IDLE)
		OSSemPost(mb_idle_event_ptr);     
#endif
    return err;
}







