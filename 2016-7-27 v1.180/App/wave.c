///****************************************Copyright (c)****************************************************
//**                            		skyray-instrument Co.,LTD.
//**
//**                                 http://www.skyray-instrument.com
//**
//**--------------File Info---------------------------------------------------------------------------------
//** File Name:           wave.c
//** Last modified Date:  2011-02-24
//** Last Version:        V1.0
//** Descriptions:
//**
//**--------------------------------------------------------------------------------------------------------
//** Created By:          方卫龙
//** Created date:        2011-02-24
//** Version:             V1.0
//** Descriptions:        First version
//**
//**--------------------------------------------------------------------------------------------------------
//** Modified by:         方卫龙
//** Modified date:       2011-02-24
//** Version:             V1.X
//** Descriptions:
//**
//*********************************************************************************************************/
//#include "wave.h"
//
//WAVE_PARAMETER waveParameter = {-900.0,-500.0,500.0,1.0,0.0,0.5,0.0,16};
//WAVE_PARAMETER waveParameterForTest = {-1000.0,1000.0,500.0,1.0,0.0,0.5,0.0,16};
//
//WAVE_RUN_PARAMETER waveRunParameter;
//
//ELECTRODE_CONDITION electrodeCondition;		//CV调节
//
//ELECTRODE_ACTIVATION electrodeActivation;	//方波调节
//
//FILM_PARAMETER filmParameter;
//
//MEASURE_PARAMETER measureParameter;
//
//MEAS_RECORD measRecord;
//
//MENU_PARAMETER menuParameter[MENUADDSIZE] = {
//{"汞(Hg)",0,{{10,10,0},{100,10,0},{1000,10,0},},-1000.0,5,-900.0,-500.0,5,-900.0,-500.0,500.0,1.0,0,0,0,16,-500.0,-900.0},
//
//{"铜(Cu)",0,{{10,10,0},{100,10,0},{100,10,0},},-1000.0,5,-900.0,-500.0,5,-900.0,-500.0,500.0,1.0,0,0,0,16,-500.0,-900.0},
//
//{"铅(Pb)",0,{{10,10,0},{100,10,0},{100,10,0},},-1000.0,5,-900.0,-500.0,5,-900.0,-500.0,500.0,1.0,0,0,0,16,-500.0,-900.0},
//
//{"锌(Zn)",0,{{10,10,0},{100,10,0},{100,10,0},},-1000.0,5,-900.0,-500.0,5,-900.0,-500.0,500.0,1.0,0,0,0,16,-500.0,-900.0},
//
//{"砷(As)",0,{{10,10,0},{100,10,0},{100,10,0},},-1000.0,5,-900.0,-500.0,5,-900.0,-500.0,500.0,1.0,0,0,0,16,-500.0,-900.0},
//
//{"锰(Mn)",0,{{10,10,0},{100,10,0},{100,10,0},},-1000.0,5,-900.0,-500.0,5,-900.0,-500.0,500.0,1.0,0,0,0,16,-500.0,-900.0},
//
//{"汞(Hg)",0,{{10,10,0},{100,10,0},{100,10,0},},-1000.0,5,-900.0,-500.0,5,-900.0,-500.0,500.0,1.0,0,0,0,16,-500.0,-900.0},
//
//
////[5].range = {{10,10,0},{100,10,0},{100,10,0},},
//};
//
//MENU_PARAMETER *pmenuParameter = menuParameter ;
//
//MENU_PARAMETER runMenuParameter;
//
//uint16 cleanTime = 5;		//物理清洗5s
//uint16 sweepTime = 0;
//
//uint16 currentGain = 1;
//
////uint16 ADsampleResult[2000]={0};
///*********************************************************************************************************
//* Function name:        StartCV
//* Descriptions:         设置CV扫描参数
//* input parameters:     waveType:	波形类型
//* output parameters:    无
//* Returned value:       无
//*********************************************************************************************************/
//uint8 StartCV(WAVE_RUN_PARAMETER *runParameter,ELECTRODE_CONDITION *electrodeCondition,uint8 waveStart)
//{
//	if(waveStart)
//	{
//		runParameter->time1Value = (uint32)((electrodeCondition->sweepStep/electrodeCondition->sweepSpeed)*10000.0);
//		runParameter->timeCount = ((electrodeCondition->endVoltage - electrodeCondition->startVoltage)/electrodeCondition->sweepStep);
//		runParameter->waveType	= 0x0F;									  //定义F为CV扫描
//
//		iCountTime2 = 0;
//		TIM_ResetCounter(LPC_TIM2);
//		TIM_UpdateMatchValue(LPC_TIM2,2,runParameter->time1Value);		  //更新DA输出定时器
//		TIM_Cmd(LPC_TIM2,ENABLE);										  //启动DA定时器
//	}
//	return 0;
//}
///*********************************************************************************************************
//* Function name:        SetWaveform
//* Descriptions:         设置运行波形参数
//* input parameters:     waveType:	波形类型
//* output parameters:    无
//* Returned value:       无
//*********************************************************************************************************/
//uint8 SetWaveform (WAVE_RUN_PARAMETER *runParameter,WAVE_PARAMETER *parameter,uint8 waveType)
//{
//	uint32 valueT;
//
//	switch (waveType)
//	{
//		case 0:				   //线性扫描
//			runParameter->time1Value = (uint32)((parameter->sweepStep/parameter->sweepSpeed)*10000.0);
//			runParameter->time2Value = 0;
//			runParameter->timeCount = ((parameter->endVoltage - parameter->startVoltage)/parameter->sweepStep);
//			runParameter->waveTimeDelay = (uint32)((parameter->waveDelay) * 10.0);
//			runParameter->waveType = waveType;
//			break;
//		case 1:				  //方波扫描
//			valueT = (uint32)((parameter->sweepStep/parameter->sweepSpeed)*10000.0);
//			runParameter->time2Value = (uint32)(valueT *0.5);
//			runParameter->time1Value = (uint32)(valueT *0.5);
//			runParameter->timeCount = ((parameter->endVoltage - parameter->startVoltage)/parameter->sweepStep);
//			runParameter->waveTimeDelay = (uint32)((parameter->waveDelay) * 10.0);
//			runParameter->waveType = waveType;
//			break;
//		case 2:				  //差分脉冲
//			valueT = (uint32)((parameter->sweepStep/parameter->sweepSpeed)*10000.0);
//			runParameter->time2Value = (uint32)(valueT * (parameter->dutyCycle)/100.0);
//			runParameter->time1Value = valueT - (runParameter->time2Value);
//			runParameter->timeCount = ((parameter->endVoltage - parameter->startVoltage)/parameter->sweepStep);
//			runParameter->waveTimeDelay = (uint32)((parameter->waveDelay) * 10.0);
//			runParameter->waveType = waveType;
//			break;
//		default:
//		    break;
//	}
//	return 0;
//}
//
///*********************************************************************************************************
//* Function name:        StartWaveform
//* Descriptions:         启动波形产生函数
//* input parameters:     waveStart:	波形类型
//* output parameters:    无
//* Returned value:       无
//*********************************************************************************************************/
//uint8 StartWaveform(WAVE_RUN_PARAMETER *runParameter,uint8 waveStart)
//{
//	if(waveStart)
//	{
//		switch (runParameter->waveType)
//		{
//			case 0:
//				iCountTime2 = 0;
//				iCountTime2AD = 0;
//				TIM_ResetCounter(LPC_TIM2);
//				TIM_ResetCounter(LPC_TIM1);
//				TIM_UpdateMatchValue(LPC_TIM2,2,runParameter->time1Value);		  //更新DA输出定时器
//				TIM_UpdateMatchValue(LPC_TIM1,1,runParameter->time1Value);		  //更新AD采样定时器
//
//				TIM_Cmd(LPC_TIM2,ENABLE);										  //启动DA定时器
//				Delay100uS(runParameter->waveTimeDelay);						  //延时采样
//				TIM_Cmd(LPC_TIM1,ENABLE); 										  //启动AD定时器
//				break;
//			case 1:
//			case 2:
//				iCountTime2 = 0;
//				iCountTime2AD = 0;
//				iCountTime2Flag = 0;
//				iCountTime1Flag = 0;
//				TIM_ResetCounter(LPC_TIM2);
//				TIM_ResetCounter(LPC_TIM1);
//				TIM_UpdateMatchValue(LPC_TIM2,2,runParameter->time1Value);		  //脉冲周期计数
//				TIM_UpdateMatchValue(LPC_TIM1,1,runParameter->time1Value);		  //脉冲宽度计数
//				TIM_Cmd(LPC_TIM1,ENABLE);
//				Delay100uS(runParameter->waveTimeDelay);
//				TIM_Cmd(LPC_TIM2,ENABLE);
//				break;
//			default:
//				break;
//		}
//	}
//	return 0;
//}
//
///*********************************************************************************************************
//* Function name:        GetSweepTime
//* Descriptions:         驱动反扫时间
//* input parameters:     *parameter:	波形设置参数
//* output parameters:    无
//* Returned value:       无
//*********************************************************************************************************/
//uint16 GetSweepTime(WAVE_PARAMETER *parameter)
//{
//	uint16 datr;
//
//	datr = ((parameter->endVoltage) - (parameter->startVoltage))*1000/(parameter->sweepSpeed);
//	return datr;
//}
//
///*********************************************************************************************************
//* Function name:  		GetXData
//* Descriptions:   		对xdata 赋值
//* input parameters:   	*parameter 测量参数信息
//* output parameters:  	*padt  xdata 每个xdata被赋值
//* Returned value:     	icount  测量点数
//*********************************************************************************************************/
//uint16 GetXData(MENU_PARAMETER *parameter,int16 *pdat)
//{
//	uint16 iCount,i;
//
//	iCount = ((parameter->measureStop) - (parameter->measureStart))/(parameter->sweepStep);
//	for(i=0;i<iCount;i++)
//	{
//		*(pdat++) = (int16)((parameter->measureStart) + i * (parameter->sweepStep));
//	}
//
//	return iCount;
//}
///*********************************************************************************************************
//* Function name:   		GetYDataStart
//* Descriptions:      	ydata 的大小
//* input parameters:   	*parameter  	测量参数
//* output parameters:  	*YAddLength   ydata 大小
//* Returned value:      	0
//*********************************************************************************************************/
//
//int8 GetYDataStart(MENU_PARAMETER *parameter,uint16 *YAddLength)
//{
//	*YAddLength = ((parameter->measureStart) - (parameter->sweepStart))/(parameter->sweepStep);
//
//	return 0;
//}
//
///*********************************************************************************************************
//* Function name:   		GetYDataStop
//* Descriptions:      	ydata 的大小
//* input parameters:   	*parameter  	测量参数
//* output parameters:  	*YPluseLength   ydata 大小
//* Returned value:      	0
//*********************************************************************************************************/
//
//int8 GetYDataStop(MENU_PARAMETER *parameter,uint16 *YPluseLength)
//{
//	*YPluseLength = ((parameter->sweepStop) - (parameter->measureStop))/(parameter->sweepStep);
//
//	return 0;
//}
///*********************************************************************************************************
//* Function name: 		WriteRecord
//* Descriptions:  		e2rom 存数 ：  名称      时间     结果
//* input parameters:   	recordP  存入的排位点    *record 	存入的结构体
//* output parameters:    无
//* Returned value:       无
//*********************************************************************************************************/
//void WriteRecord(uint16 recordP,MEAS_RECORD *record)
//{
//	uint8 i;
//
//	recordP = recordP*21+RECORDSTAR;
//	for(i = 0;i<10;i++)
//	{
//		C1025_WriteByte(recordP+i,record->menuName[0+i]);
//	}
//	WriteData16(recordP+10,record->recordTime.year);
//	C1025_WriteByte(recordP+12,record->recordTime.month);
//	C1025_WriteByte(recordP+13,record->recordTime.date);
//	C1025_WriteByte(recordP+14,record->recordTime.hour);
//	C1025_WriteByte(recordP+15,record->recordTime.minute);
//	C1025_WriteByte(recordP+16,record->recordTime.second);
//	WriteFp32(recordP+17,record->recordResult);
//}
//
///*********************************************************************************************************
//* Function name:       ReadRecord
//* Descriptions:        对出该排位点的数据： 名称 时间 结果
//* input parameters:    uint16 recordP  存入的排位点,
//* output parameters:   MEAS_RECORD *record 读出的结构体
//* Returned value:      无
//*********************************************************************************************************/
//void ReadRecord(uint16 recordP,MEAS_RECORD *record)
//{
//	uint8 i;
//	recordP = recordP*21+RECORDSTAR;
//	for(i = 0;i<10;i++)
//	{
//		record->menuName[0+i]=C1025_ReadByte(recordP+i);
//	}
//	record->recordTime.year=ReadData16(recordP+10);
//	record->recordTime.month=C1025_ReadByte(recordP+12);
//	record->recordTime.date=C1025_ReadByte(recordP+13);
//	record->recordTime.hour=C1025_ReadByte(recordP+14);
//	record->recordTime.minute=C1025_ReadByte(recordP+15);
//	record->recordTime.second=C1025_ReadByte(recordP+16);
//	record->recordResult= ReadFp32(recordP+17);
//}
//
//
//
//
///*********************************************************************************************************
//* Function name:        Clean
//* Descriptions:         物理清洗，不带电压
//* input parameters:     无
//* output parameters:    无
//* Returned value:       无
//*********************************************************************************************************/
//void Clean(void)
//{	 /*
//	持续清洗时间的清洗
//
//	清洗命令复位
//	蜂鸣叫 表示完成
//	*/
//	SET_MOTOR(1);
//	TimeMeasCount = cleanTime;
//	while(TimeMeasCount);
//	SET_MOTOR(0);
//
//	usRegHoldingBuf[2] &= (~0x0001);
//	BuzzerCtrl(2,5,5);
//}
//
///*********************************************************************************************************
//* Function name:        AnodicStrippingVoltammetry
//* Descriptions:         阳极溶出法
//* input parameters:     无
//* output parameters:    无
//* Returned value:       无
//*********************************************************************************************************/
//void AnodicStrippingVoltammetry(void)
//{
//	SetSWI1(1);					  /*选通电解池*/
//	SetSWI2(1);					  /*此处不要电流增益过大*/
//	/*搅拌*/
//	/*
//	给搅拌电位
//	选通对应的开关 和增益
//	电机转动 到搅拌时间
//	*/
//
//	if(0!= measureParameter.mixTime)
//	{
//		AnalogOutput(measureParameter.mixVoltage);
//		SET_MOTOR(1);
//		TimeMeasCount = measureParameter.mixTime;
//		while(TimeMeasCount);
//	}
//
//
//	/*富集*/
//	/*
//	给富集电位
//	延时 到 富集时间到
//	*/
//	if(0!= measureParameter.accumulationTime)
//	{
//		AnalogOutput(measureParameter.accumulationVoltage);
//		SET_MOTOR(1);
//		TimeMeasCount = measureParameter.accumulationTime;
//		while(TimeMeasCount);
//	}
//	SET_MOTOR(0);
//
//	/*静置*/
//	/*
//	停止电机
//	给静置电位到静置时间到
//	*/
//	if(0!= measureParameter.staticTime)
//	{
//		AnalogOutput(measureParameter.staticVoltage);
//		TimeMeasCount = measureParameter.staticTime;
//		while(TimeMeasCount);
//	}
//
//
//	/*反扫操作*/
//	/*
//	开启电流增益
//	延时1s
//	反扫参数赋值
//	采用一次，但不取值
//	*/
//	SetSWI2(currentGain);
//	OSTimeDly((int32)(OS_TICKS_PER_SEC));
//	SetWaveform(&waveRunParameter,&waveParameter,measureParameter.MeasureType);
//	GetADValue();
//	/*
//	开始反扫 （定时器赋值，使能定时器）
//	计算扫描时间
//	持续到 （扫描时间+采样延时时间 ）结束
//	*/
//	StartWaveform(&waveRunParameter,1);
//	sweepTime = GetSweepTime(&waveParameter);
//	TimeMeasCount = sweepTime/1000+2;//waveRunParameter.waveTimeDelay/10;
//	if(TimeMeasCount<1)
//	{
//		TimeMeasCount = 2;
//	}
//	while(TimeMeasCount);
//	/*
//	更改增益
//	两次移动平均处理
//	*/
//	SetSWI2(0);					  /*此处不要电流增益过大*/
//	MovingAverage((int16 *)usRegInputBuf,waveRunParameter.timeCount,8);
//	MovingAverage((int16 *)usRegInputBuf,waveRunParameter.timeCount,8);
//	//电流判定，如果超过320uA则电流*100，反之电流*10，同时usRegInputBuf[98]=100或者10
//	JudgeCurrent((int16 *)usRegInputBuf,waveRunParameter.timeCount);
//
//
//	/*清洗2*/
//	/*
//	电机转动
//	给溶出清洗电压使得富集的离子全部溶解出
//	持续到清洗时间到
//	关电机
//	*/
//	if(filmParameter.filmTime)
//	{
//		SET_MOTOR(1);
//		AnalogOutput(filmParameter.filmVoltage);
//		TimeMeasCount = filmParameter.filmTime;
//		while(TimeMeasCount);
//		SET_MOTOR(0);
//	}
//
//	/*
//	断开电解池
//	增益选择
//	*/
//	SetSWI1(0);
//	SetSWI2(0);
//	/*
//	操作标志清零
//	蜂鸣叫 表示结束
//	*/
//	usRegHoldingBuf[2] &= (~0x0002);
//
//	if(NoBuzzry == 0)
//	{
//		BuzzerCtrl(2,5,5);
//	}
//
//	OSSemPost(OSMeasSem);
//}
//
//
//
///*********************************************************************************************************
//* Function name:        clean2
//* Descriptions:         待溶出电位的清洗
//* input parameters:     无
//* output parameters:    无
//* Returned value:       无
//*********************************************************************************************************/
//void Clean2(void)
//{
//	/*
//	给溶出清洗电压
//	联通电解池
//	选增益
//	开电机
//	持续到 清洗电位
//	*/
//	AnalogOutput(filmParameter.filmVoltage);
//	SetSWI1(1);
//	SetSWI2(0);					  /*此处不要电流增益过大*/
//	SET_MOTOR(1);
////	OSTimeDly((int32)(OS_TICKS_PER_SEC*filmParameter.filmTime));
//	TimeMeasCount = filmParameter.filmTime;
//	while(TimeMeasCount);
//	/*
//	断开电解池
//	关电机
//	清零操作标志
//	蜂鸣叫
//	*/
//	SET_MOTOR(0);
//	SetSWI1(0);
//	usRegHoldingBuf[2] &= (~0x0004);
//	BuzzerCtrl(2,5,5);
//
//}
//
///*********************************************************************************************************
//* Function name:        CoatingFilm
//* Descriptions:         镀膜
//* input parameters:     无
//* output parameters:    无
//* Returned value:       无
//*********************************************************************************************************/
//void CoatingFilm(void)
//{
//	/*
//	给镀膜电位
//	选通电解池
//	选择增益
//	开电机
//	持续到 镀膜 时间到
//	*/
//	AnalogOutput(filmParameter.filmVoltage);
//	SetSWI1(1);
//	SetSWI2(0);					  /*此处不要电流增益过大*/
//	SET_MOTOR(1);
////	OSTimeDly((int32)(OS_TICKS_PER_SEC*filmParameter.filmTime));
//	TimeMeasCount = filmParameter.filmTime;
//	while(TimeMeasCount);
//
//	/*
//	关电机
//	关电解池
//	清零操作标志
//	蜂鸣叫
//	*/
//	SET_MOTOR(0);
//	SetSWI1(0);
//	usRegHoldingBuf[2] &= (~0x2000);
//	BuzzerCtrl(2,5,5);
//}
//
///*********************************************************************************************************
//* Function name:        defilm
//* Descriptions:         去膜
//* input parameters:     无
//* output parameters:    无
//* Returned value:       无
//*********************************************************************************************************/
//void defilm(void)
//{
//	/*
//	给镀膜电位
//	选通电解池
//	选择增益
//	开电机
//	持续到 去膜 时间到
//	*/
//	AnalogOutput(filmParameter.filmVoltage);
//	SetSWI1(1);
//	SetSWI2(0);					  /*此处不要电流增益过大*/
//	SET_MOTOR(1);
////	OSTimeDly((int32)(OS_TICKS_PER_SEC*filmParameter.filmTime));
//	TimeMeasCount = filmParameter.filmTime;
//	while(TimeMeasCount);
//
//	/*
//	关电机
//	关电解池
//	清零操作标志
//	蜂鸣叫
//	*/
//	SET_MOTOR(0);
//	SetSWI1(0);
//	usRegHoldingBuf[2] &= (~0x4000);
//	BuzzerCtrl(2,5,5);
//
//}
//
///*********************************************************************************************************
//* Function name:        Regulate
//* Descriptions:         调节
//* input parameters:     无
//* output parameters:    无
//* Returned value:       无
//*********************************************************************************************************/
//void Regulate(void)
//{
//	/*
//	先输出一个第一电位
//	连接电解池
//	增益选择
//	开始CV扫描
//	持续到 时间完成
//	*/
//	AnalogOutput(electrodeCondition.startVoltage);
//	SetSWI1(1);
//	SetSWI2(0);					  /*此处不要电流增益过大*/
//	StartCV(&waveRunParameter,&electrodeCondition,1);
//	TimeMeasCount = (int32)   ( electrodeCondition.Count \
//			  *2*(electrodeCondition.endVoltage - electrodeCondition.startVoltage)\
//	          /electrodeCondition.sweepSpeed) ;
//	while(TimeMeasCount);
//	/*
//	关断电解池
//	清零操作标志
//	蜂鸣叫
//	*/
//	SetSWI1(0);
//	usRegHoldingBuf[2] &= (~0x8000);
//	BuzzerCtrl(2,5,5);
//}
//
///*********************************************************************************************************
//* Function name:        ReadHistory
//* Descriptions:         读历史数据
//* input parameters:     无
//* output parameters:    无
//* Returned value:       无
//*********************************************************************************************************/
//void ReadHistory(void)
//{
//	MEAS_RECORD rRecord;
//	/*读取对应点的历史记录*/
//	ReadRecord(recordPoint - usRegHoldingBuf[981] -1,&rRecord);
//	/*赋值历史数据*/
//	usRegHoldingBuf[980] = 	recordPoint;
//	/*名称*/
//	usRegHoldingBuf[982] = 	rRecord.menuName[1]+(rRecord.menuName[0]<<8);
//	usRegHoldingBuf[983] = 	rRecord.menuName[3]+(rRecord.menuName[2]<<8);
//	usRegHoldingBuf[984] = 	rRecord.menuName[5]+(rRecord.menuName[4]<<8);
//	usRegHoldingBuf[985] = 	rRecord.menuName[7]+(rRecord.menuName[6]<<8);
//	usRegHoldingBuf[986] = 	rRecord.menuName[9]+(rRecord.menuName[8]<<8);
//	/*年月日*/
//	usRegHoldingBuf[987] = 	 rRecord.recordTime.year;
//	usRegHoldingBuf[988] = 	(rRecord.recordTime.month) & 0x00ff;
//	usRegHoldingBuf[989] = 	(rRecord.recordTime.date) & 0x00ff;
//	/*时分秒*/
//	usRegHoldingBuf[990] = 	(rRecord.recordTime.hour) & 0x00ff;
//	usRegHoldingBuf[991] = 	(rRecord.recordTime.minute) & 0x00ff;
//	usRegHoldingBuf[992] = 	(rRecord.recordTime.second) & 0x00ff;
//	/*结果*/
////	FloatToChArray(rRecord.recordResult,cc);
////	usRegHoldingBuf[993] = 	(cc[1]<<8)+cc[0];
////	usRegHoldingBuf[994] = 	(cc[3]<<8)+cc[2];
//	if((rRecord.recordResult<0.0)||(GetNumLen(rRecord.recordResult)==127)) //数据无效
//	{
//		usRegHoldingBuf[993] = 	0xffff;
//		usRegHoldingBuf[994] = 	0xffff;
//	}
//	else
//	{
//		usRegHoldingBuf[993] = 	((unsigned long)(rRecord.recordResult*10000.0))>>16;
//		usRegHoldingBuf[994] = 	(uint16)(rRecord.recordResult*10000.0);
//	}
//
//}
//
//
//
//
