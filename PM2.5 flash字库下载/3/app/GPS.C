

#include"GPS.H"
int8 	 ucGPSCommand[8];//GPS命令标识头
int8	ucGPSDataBuff[82];//GPS数据缓存
uint8 	ucGPSBuffCnt;
uint32 InHeadComm1;
int32  unInBufCntComm1 = 0;
uint32 ucCheckSum[2];


void  GPS_ProcessNMEA(void);


volatile uint8 	 ucGpsCommandStatus = 0;
//////////////////////////////////////////////
typedef struct INFO_STAR_STRUCT{
			uint8 ucNumber;
			uint8	ucSNR;
			uint8 ucElevation;//高度角
			uint16  unAzimuth;//方位角
			}INFO_STAR,*PINFO_STAR;

typedef struct INFO_LON_STRUCT{
         uint8  ucSign;
   		uint8  ucDegree;
   		uint8  ucMinute;
   		uint16   unSecond;
        }INFO_LON,*PINFO_LON;	//经度

typedef struct INFO_LAT_STRUCT{
         uint8  ucSign;
   		uint8  ucDegree;
   		uint8  ucMinute;
   		uint16   unSecond;
        }INFO_LAT,*PINFO_LAT;	//纬度


typedef struct INFO_DATETIME_STRUCT{
		uint8 ucYear;//年的后2位
		uint8 ucMonth;
		uint8 ucDay;
		uint8 ucHour;
		uint8 ucMinute;
		uint8 ucSecond;
	}INFO_DATETIME,*PINFO_DATETIME;	//时间日期
///////////////////////////////////////
typedef struct INFO_GPS_struct {
		  	unsigned char 	SN[8];
		  	unsigned char  UserName[8];
			unsigned char  Reserved[7];
        	INFO_DATETIME 	Info_DateTime;
         INFO_LON			Info_Lon;
			INFO_LAT			Info_Lat;
			signed 	int	iHigh;
			unsigned char  ucDStatue;
			unsigned int 	unDop;
			unsigned char  ucSvSum;
			unsigned char  ucTextLen;
			unsigned char  Text[50+1];		 
}INFO_GPS,*PINFO_GPS;


extern	INFO_GPS			 InfoGPS;
extern	INFO_GPS			 NowInfoGPS;//实时gps 信息
extern 	INFO_STAR		 ucSNR[12];
extern	uint8  GPS_Date[];
INFO_GPS			 InfoGPS;
INFO_STAR		 ucSNR[12];
INFO_GPS			 NowInfoGPS;
uint8  GPS_Date[];
//--------------------------------------------------
//GPS命令异或校验
//--------------------------------------------------
int32 GPS_CheckCommand( void )
{
	uint8  uci;
	uint32   Sum = 0;

	if(ucCheckSum[0]>0x0f)	ucCheckSum[0] -=0x07;
	if(ucCheckSum[1]>0x0f)	ucCheckSum[1] -=0x07;
	Sum = ucCheckSum[0] << 4;
	Sum +=ucCheckSum[1];
	for(uci=0; (ucGPSDataBuff[uci] != '*') && (uci < 80);uci++)
    	Sum ^=  ucGPSDataBuff[uci];

   if (Sum != 0)
		return FALSE;
	else
    	return TRUE;
}

int32 ReadCharComm1(uint8 *ch)
{	
	if(unInBufCntComm1 <= 0)
	{
		unInBufCntComm1 = UART_RING_BUFSIZE - 1;
		return 0; //缓冲空
	
	}

	*ch = rb.rx[InHeadComm1++];
   InHeadComm1 %= UART_RING_BUFSIZE;        //读尽缓冲区，队列头返回0
   unInBufCntComm1--;

   return 1;    			
}

//////////////////////////////////
//得到第ucSum个都号的位置
///////////////////////////////////
uint8	GetNComma(uint8 ucNum)
{
	uint8  i,j;
	
	//*,c1,c2,\r,\n
	for(i = 5,j=0;i<77;i++)
	{
		if(ucGPSDataBuff[i] == ',')
		{
			j++;
			if(j == ucNum)
				return i;
		}		
	}
	return 0;
}



//------------------------------------------------
//	处理GGA命令	
//------------------------------------------------
void GPS_ProcessGGA(void)
{
	uint8   i,sign;
	//latitude
	i=GetNComma(2);
	if(i>0)
		i++;
	else
		return;
	if(ucGPSDataBuff[i]!=',')
	{
		NowInfoGPS.Info_Lat.ucDegree = (ucGPSDataBuff[i] - 0x30)*10+\
								   (ucGPSDataBuff[i+1]-0x30);
		NowInfoGPS.Info_Lat.ucMinute = (ucGPSDataBuff[i+2] -0x30)*10+\
								   (ucGPSDataBuff[i+3]-0x30);
		NowInfoGPS.Info_Lat.unSecond = (uint16)((ucGPSDataBuff[i+5]-0x30)*1000+\
								   (ucGPSDataBuff[i+6]-0x30)*100+\
								   (ucGPSDataBuff[i+7]-0x30)*10+\
								   (ucGPSDataBuff[i+8]-0x30))*6;
		i += 10;
	}
	else
	{
		NowInfoGPS.Info_Lat.ucDegree = 0x00;
		NowInfoGPS.Info_Lat.ucMinute = 0x00;
		NowInfoGPS.Info_Lat.unSecond = 0x00;
		i++;
	}

	if(ucGPSDataBuff[i]!=',')
	{
		if(ucGPSDataBuff[i] == 'S')
			NowInfoGPS.Info_Lat.ucSign = 1;//s
		else
			NowInfoGPS.Info_Lat.ucSign = 0;//n
		i += 2;
	}
	else
	{
		i++;
	}
	//longitude
	if(ucGPSDataBuff[i]!=',')
	{
		NowInfoGPS.Info_Lon.ucDegree = (ucGPSDataBuff[i]-0x30)*100+\
									(ucGPSDataBuff[i+1]-0x30)*10+\
									(ucGPSDataBuff[i+2]-0x30);
		NowInfoGPS.Info_Lon.ucMinute = (ucGPSDataBuff[i+3]-0x30)*10+\
									(ucGPSDataBuff[i+4]-0x30);
		NowInfoGPS.Info_Lon.unSecond = (uint16)((ucGPSDataBuff[i+6]-0x30)*1000+\
									(ucGPSDataBuff[i+7]-0x30)*100+\
									(ucGPSDataBuff[i+8]-0x30)*10+\
									(ucGPSDataBuff[i+9]-0x30))*6;
		i+=11;
	}
	else
	{
		NowInfoGPS.Info_Lon.ucDegree = 0x00;
		NowInfoGPS.Info_Lon.ucMinute = 0x00;
		NowInfoGPS.Info_Lon.unSecond = 0x00;
		i++;
	}

	if(ucGPSDataBuff[i]!=',')
	{
		if(ucGPSDataBuff[i]=='W')
			NowInfoGPS.Info_Lon.ucSign = 1;//w
		else
			NowInfoGPS.Info_Lon.ucSign = 0;//e
		i+=2;
	}
	else
	{
		i++;
	}

	if(ucGPSDataBuff[i]!=',')
	{
		NowInfoGPS.ucDStatue = ucGPSDataBuff[i]-0x30;
		i+=2;
	}
	else
	{
		NowInfoGPS.ucDStatue = 0x00;
		i++;
	}

	if(ucGPSDataBuff[i]!=',')
	{
		NowInfoGPS.ucSvSum = (ucGPSDataBuff[i]-0x30)*10+\
							 (ucGPSDataBuff[i+1]-0x30);
		i+=3;
	}
	else
	{
		NowInfoGPS.ucSvSum = 0x00;
		i++;
	}

	if(ucGPSDataBuff[i]!=',')
	{
		NowInfoGPS.unDop =(uint16)((ucGPSDataBuff[i]-0x30)*10+\
					  (ucGPSDataBuff[i+2]-0x30));
		i+=4;
	}
	else
	{
		NowInfoGPS.unDop = 0x00;
		i++;
	}
	NowInfoGPS.iHigh = 0;
	if(ucGPSDataBuff[i] == '-')
	{
		sign = 1;
		i++;
	}
	for(; (ucGPSDataBuff[i]!='.') && (ucGPSDataBuff[i]!=','); i++)//舍去高度的小数
	{
		NowInfoGPS.iHigh *= 10;
		NowInfoGPS.iHigh += (ucGPSDataBuff[i]-0x30);
	}
	if(sign ==1)
		NowInfoGPS.iHigh *= -1;
	if(NowInfoGPS.iHigh > 90000)
		NowInfoGPS.iHigh = 0;
}

//-------------------------------------------------
//处理GSV命令
//-------------------------------------------------
void GPS_ProcessGSV(void)
{
	uint8  i,frame,j,k,Statellite,cnt;

	frame = 0;
	i=GetNComma(2);
	if(i>0)
		i++;
	else
		return;
	if(ucGPSDataBuff[i]!=',')
	{
		frame = ucGPSDataBuff[i]-0x30;
		i+=2;
	}
	else
	{
		return;
	}

	Statellite = 0;
	for(;ucGPSDataBuff[i]!=',';i++)
	{
		Statellite *= 10;
		Statellite += ucGPSDataBuff[i]-0x30;
	}
	i++;
	if(ucGPSDataBuff[i]!=',')
	{
		k = (frame -1)*4;

		for(j=0;j<4;j++)
		{
			ucSNR[j+k].ucNumber = 0;
			ucSNR[j+k].unAzimuth = 0;
			ucSNR[j+k].ucElevation = 0;
			ucSNR[j+k].ucSNR = 0;
		}
		if(Statellite>=frame*4)
			cnt = 4;
		else
			cnt = Statellite - k;

		for(j=0;j<cnt;j++)
		{
			for(;ucGPSDataBuff[i]!=',';i++)
			{
				ucSNR[j+k].ucNumber *= 10;
				ucSNR[j+k].ucNumber += ucGPSDataBuff[i]-0x30;
			}
			i++;
			for(;ucGPSDataBuff[i]!=',';i++)
			{
				ucSNR[j+k].ucElevation *= 10;
				ucSNR[j+k].ucElevation += ucGPSDataBuff[i]-0x30;
			}
			i++;
			for(;ucGPSDataBuff[i]!=',';i++)
			{
				ucSNR[j+k].unAzimuth *= 10;
				ucSNR[j+k].unAzimuth += ucGPSDataBuff[i]-0x30;
			}
			i++;
			for(;(ucGPSDataBuff[i]!=',')&&(ucGPSDataBuff[i]!='*');i++)
			{
				ucSNR[j+k].ucSNR *=10;
				ucSNR[j+k].ucSNR += ucGPSDataBuff[i]-0x30;
			}
			i++;
		}
	}
}

//-------------------------------------------------
//处理RMC命令
//-------------------------------------------------
void GPS_ProcessRMC(void)
{
	uint8  i;	
	uint32 temp;
	   ;
	i=GetNComma(1);
	if(i>0)
		i++;
	else
		return;
/////////////////hhmmss
	if(ucGPSDataBuff[i]!=',')
	{
		NowInfoGPS.Info_DateTime.ucHour = (ucGPSDataBuff[i]-0x30)*10+(ucGPSDataBuff[i+1]-0x30);	
		//调整为北京时间
		NowInfoGPS.Info_DateTime.ucHour += 8;
		NowInfoGPS.Info_DateTime.ucHour%=24;
		NowInfoGPS.Info_DateTime.ucMinute = (ucGPSDataBuff[i+2]-0x30)*10+(ucGPSDataBuff[i+3]-0x30);
		NowInfoGPS.Info_DateTime.ucSecond = (ucGPSDataBuff[i+4]-0x30)*10+(ucGPSDataBuff[i+5]-0x30);
		i += 11;
	}
	else
	{
		NowInfoGPS.Info_DateTime.ucHour = 0x00;
		NowInfoGPS.Info_DateTime.ucMinute = 0x00;
		NowInfoGPS.Info_DateTime.ucSecond = 0x00;
		i++;
	}
//////////////
	i=GetNComma(9);
	if(i>0)
		i++;
	else
		return;
	if(ucGPSDataBuff[i]!=',')
	{
		NowInfoGPS.Info_DateTime.ucDay = (ucGPSDataBuff[i]-0x30)*10+(ucGPSDataBuff[i+1]-0x30);				
		NowInfoGPS.Info_DateTime.ucMonth = (ucGPSDataBuff[i+2]-0x30)*10+(ucGPSDataBuff[i+3]-0x30);
		NowInfoGPS.Info_DateTime.ucYear = (ucGPSDataBuff[i+4]-0x30)*10+(ucGPSDataBuff[i+5]-0x30);
		//时区调整
		//每天的凌晨0点0分0秒至早上7点59分59秒这段时间需要调整
		if(NowInfoGPS.Info_DateTime.ucHour < 8)
		{
			NowInfoGPS.Info_DateTime.ucDay ++;
			//天数如果大于本月的最大天数，月加1，天数返回1
			if(NowInfoGPS.Info_DateTime.ucDay > GPS_Date[NowInfoGPS.Info_DateTime.ucMonth])
			{
				//闰年的2月的最大天数处理
				if(NowInfoGPS.Info_DateTime.ucMonth == 2)
				{
					temp = 2000+NowInfoGPS.Info_DateTime.ucYear;
					if(((temp%4 == 0) && (temp%100 != 0)) || (temp%400 == 0))//闰年
					{
						//闰年2月29日的处理
						if(NowInfoGPS.Info_DateTime.ucDay > 29)
						{
							NowInfoGPS.Info_DateTime.ucDay = 1;
							NowInfoGPS.Info_DateTime.ucMonth ++;
						}
					}
					else//非闰年
					{
						NowInfoGPS.Info_DateTime.ucDay = 1;
						NowInfoGPS.Info_DateTime.ucMonth ++;
					}
						
				}
				//其它月份的天数处理
				else
				{
					NowInfoGPS.Info_DateTime.ucDay = 1;
					NowInfoGPS.Info_DateTime.ucMonth ++;	
					//12月最大天数，年加1
					if(NowInfoGPS.Info_DateTime.ucMonth > 12)
					{
						NowInfoGPS.Info_DateTime.ucYear ++;
						NowInfoGPS.Info_DateTime.ucMonth = 1;
					}
				}
			}
		}
	}
	else
	{
		NowInfoGPS.Info_DateTime.ucDay = 0x00;
		NowInfoGPS.Info_DateTime.ucMonth = 0x00;
		NowInfoGPS.Info_DateTime.ucYear = 0x00;
	}
}



////////////////////////
void	ProcessGPSCommand(void)
{
	int i;
	if(strcmp(ucGPSCommand,"GGA") == 0)	
	{
		GPS_ProcessGGA();
	}
	else if(strcmp(ucGPSCommand,"GSV") == 0)
	{
		GPS_ProcessGSV();
	}
	else if(strcmp(ucGPSCommand,"RMC") == 0)
	{
		GPS_ProcessRMC();
	}
	//清零		
//			for(i= 0 ;i<81;i++)
//			{
//				ucGPSDataBuff[i] = 0;
//			}
}


////////////////////////////////////
//=================================================
//GPS数据处理主循环
//=================================================
void  GPS_ProcessNMEA(void)
{
	uint8 uch0 = 0;
	while(unInBufCntComm1)
	{
		switch (ucGpsCommandStatus)
		{
			case 0:
         	{
				ReadCharComm1(&uch0);
				if (uch0=='$')
				{
					ucGpsCommandStatus++;
				}
				break;
			}
         case 1:
			{
				ReadCharComm1(&uch0);
				if (uch0=='G')
					ucGpsCommandStatus++;
				else
					ucGpsCommandStatus=0;
				break;
			}

			case 2:
			{
				ReadCharComm1(&uch0);
				if (uch0=='P')
					ucGpsCommandStatus++;
				else
					ucGpsCommandStatus=0;
				break;
			}

			case 3:
			{
				if(unInBufCntComm1 >= 3)
				{
					ReadCharComm1(&ucGPSCommand[0]);
					ReadCharComm1(&ucGPSCommand[1]);
					ReadCharComm1(&ucGPSCommand[2]);
					ucGPSCommand[3] = '\0';
					ucGPSDataBuff[0] = 'G';
					ucGPSDataBuff[1] = 'P';
					strcpy(ucGPSDataBuff+2,ucGPSCommand);
					ucGPSBuffCnt = 5;
					ucGpsCommandStatus++;
				}
				else
					ucGpsCommandStatus=0;
				break;
			}

       	case 4: //process command
			{
				if(unInBufCntComm1 < 2)
				{
					ucGpsCommandStatus=0;
					break;				
				}

				else
				{
					if(ucGPSBuffCnt>80)
					{
						ucGpsCommandStatus = 0;
						break;
					}
					ReadCharComm1(ucGPSDataBuff+ucGPSBuffCnt);
					ReadCharComm1(ucGPSDataBuff+ucGPSBuffCnt+1);
					if((ucGPSDataBuff[ucGPSBuffCnt] =='\r')\
							&&(ucGPSDataBuff[ucGPSBuffCnt+1] =='\n'))
					{
						ucCheckSum[0] =ucGPSDataBuff[ucGPSBuffCnt-2] - 0x30;
						ucCheckSum[1] =ucGPSDataBuff[ucGPSBuffCnt-1] - 0x30;;
						if(GPS_CheckCommand())
						{
							ProcessGPSCommand();
						}
						ucGpsCommandStatus=0;
						break;
					}
					else
					{
						if(ucGPSDataBuff[ucGPSBuffCnt+1] =='\r')
							ucGpsCommandStatus++;
					}
					ucGPSBuffCnt+=2;
				}
				break;
        	}//end of case 4
			case 5:
			{
				if(ucGPSBuffCnt>81)
				{
					ucGpsCommandStatus = 0;
					break;
				}
				ReadCharComm1(ucGPSDataBuff+ucGPSBuffCnt);
				if(ucGPSDataBuff[ucGPSBuffCnt] =='\n');
				{
					ucCheckSum[0] =ucGPSDataBuff[ucGPSBuffCnt-3] - 0x30;
					ucCheckSum[1] =ucGPSDataBuff[ucGPSBuffCnt-2] - 0x30;;
					if(GPS_CheckCommand())
					{
						ProcessGPSCommand();
					}
				}
				ucGpsCommandStatus=0;
				break;
			}
			default :
				ucGpsCommandStatus=0;
		}//end of switch1
	}//end of if
}

