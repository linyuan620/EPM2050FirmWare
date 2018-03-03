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
//** Created By:          ������
//** Created date:        2011-02-24
//** Version:             V1.0
//** Descriptions:        First version
//**
//**--------------------------------------------------------------------------------------------------------
//** Modified by:         ������
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
//ELECTRODE_CONDITION electrodeCondition;		//CV����
//
//ELECTRODE_ACTIVATION electrodeActivation;	//��������
//
//FILM_PARAMETER filmParameter;
//
//MEASURE_PARAMETER measureParameter;
//
//MEAS_RECORD measRecord;
//
//MENU_PARAMETER menuParameter[MENUADDSIZE] = {
//{"��(Hg)",0,{{10,10,0},{100,10,0},{1000,10,0},},-1000.0,5,-900.0,-500.0,5,-900.0,-500.0,500.0,1.0,0,0,0,16,-500.0,-900.0},
//
//{"ͭ(Cu)",0,{{10,10,0},{100,10,0},{100,10,0},},-1000.0,5,-900.0,-500.0,5,-900.0,-500.0,500.0,1.0,0,0,0,16,-500.0,-900.0},
//
//{"Ǧ(Pb)",0,{{10,10,0},{100,10,0},{100,10,0},},-1000.0,5,-900.0,-500.0,5,-900.0,-500.0,500.0,1.0,0,0,0,16,-500.0,-900.0},
//
//{"п(Zn)",0,{{10,10,0},{100,10,0},{100,10,0},},-1000.0,5,-900.0,-500.0,5,-900.0,-500.0,500.0,1.0,0,0,0,16,-500.0,-900.0},
//
//{"��(As)",0,{{10,10,0},{100,10,0},{100,10,0},},-1000.0,5,-900.0,-500.0,5,-900.0,-500.0,500.0,1.0,0,0,0,16,-500.0,-900.0},
//
//{"��(Mn)",0,{{10,10,0},{100,10,0},{100,10,0},},-1000.0,5,-900.0,-500.0,5,-900.0,-500.0,500.0,1.0,0,0,0,16,-500.0,-900.0},
//
//{"��(Hg)",0,{{10,10,0},{100,10,0},{100,10,0},},-1000.0,5,-900.0,-500.0,5,-900.0,-500.0,500.0,1.0,0,0,0,16,-500.0,-900.0},
//
//
////[5].range = {{10,10,0},{100,10,0},{100,10,0},},
//};
//
//MENU_PARAMETER *pmenuParameter = menuParameter ;
//
//MENU_PARAMETER runMenuParameter;
//
//uint16 cleanTime = 5;		//������ϴ5s
//uint16 sweepTime = 0;
//
//uint16 currentGain = 1;
//
////uint16 ADsampleResult[2000]={0};
///*********************************************************************************************************
//* Function name:        StartCV
//* Descriptions:         ����CVɨ�����
//* input parameters:     waveType:	��������
//* output parameters:    ��
//* Returned value:       ��
//*********************************************************************************************************/
//uint8 StartCV(WAVE_RUN_PARAMETER *runParameter,ELECTRODE_CONDITION *electrodeCondition,uint8 waveStart)
//{
//	if(waveStart)
//	{
//		runParameter->time1Value = (uint32)((electrodeCondition->sweepStep/electrodeCondition->sweepSpeed)*10000.0);
//		runParameter->timeCount = ((electrodeCondition->endVoltage - electrodeCondition->startVoltage)/electrodeCondition->sweepStep);
//		runParameter->waveType	= 0x0F;									  //����FΪCVɨ��
//
//		iCountTime2 = 0;
//		TIM_ResetCounter(LPC_TIM2);
//		TIM_UpdateMatchValue(LPC_TIM2,2,runParameter->time1Value);		  //����DA�����ʱ��
//		TIM_Cmd(LPC_TIM2,ENABLE);										  //����DA��ʱ��
//	}
//	return 0;
//}
///*********************************************************************************************************
//* Function name:        SetWaveform
//* Descriptions:         �������в��β���
//* input parameters:     waveType:	��������
//* output parameters:    ��
//* Returned value:       ��
//*********************************************************************************************************/
//uint8 SetWaveform (WAVE_RUN_PARAMETER *runParameter,WAVE_PARAMETER *parameter,uint8 waveType)
//{
//	uint32 valueT;
//
//	switch (waveType)
//	{
//		case 0:				   //����ɨ��
//			runParameter->time1Value = (uint32)((parameter->sweepStep/parameter->sweepSpeed)*10000.0);
//			runParameter->time2Value = 0;
//			runParameter->timeCount = ((parameter->endVoltage - parameter->startVoltage)/parameter->sweepStep);
//			runParameter->waveTimeDelay = (uint32)((parameter->waveDelay) * 10.0);
//			runParameter->waveType = waveType;
//			break;
//		case 1:				  //����ɨ��
//			valueT = (uint32)((parameter->sweepStep/parameter->sweepSpeed)*10000.0);
//			runParameter->time2Value = (uint32)(valueT *0.5);
//			runParameter->time1Value = (uint32)(valueT *0.5);
//			runParameter->timeCount = ((parameter->endVoltage - parameter->startVoltage)/parameter->sweepStep);
//			runParameter->waveTimeDelay = (uint32)((parameter->waveDelay) * 10.0);
//			runParameter->waveType = waveType;
//			break;
//		case 2:				  //�������
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
//* Descriptions:         �������β�������
//* input parameters:     waveStart:	��������
//* output parameters:    ��
//* Returned value:       ��
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
//				TIM_UpdateMatchValue(LPC_TIM2,2,runParameter->time1Value);		  //����DA�����ʱ��
//				TIM_UpdateMatchValue(LPC_TIM1,1,runParameter->time1Value);		  //����AD������ʱ��
//
//				TIM_Cmd(LPC_TIM2,ENABLE);										  //����DA��ʱ��
//				Delay100uS(runParameter->waveTimeDelay);						  //��ʱ����
//				TIM_Cmd(LPC_TIM1,ENABLE); 										  //����AD��ʱ��
//				break;
//			case 1:
//			case 2:
//				iCountTime2 = 0;
//				iCountTime2AD = 0;
//				iCountTime2Flag = 0;
//				iCountTime1Flag = 0;
//				TIM_ResetCounter(LPC_TIM2);
//				TIM_ResetCounter(LPC_TIM1);
//				TIM_UpdateMatchValue(LPC_TIM2,2,runParameter->time1Value);		  //�������ڼ���
//				TIM_UpdateMatchValue(LPC_TIM1,1,runParameter->time1Value);		  //�����ȼ���
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
//* Descriptions:         ������ɨʱ��
//* input parameters:     *parameter:	�������ò���
//* output parameters:    ��
//* Returned value:       ��
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
//* Descriptions:   		��xdata ��ֵ
//* input parameters:   	*parameter ����������Ϣ
//* output parameters:  	*padt  xdata ÿ��xdata����ֵ
//* Returned value:     	icount  ��������
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
//* Descriptions:      	ydata �Ĵ�С
//* input parameters:   	*parameter  	��������
//* output parameters:  	*YAddLength   ydata ��С
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
//* Descriptions:      	ydata �Ĵ�С
//* input parameters:   	*parameter  	��������
//* output parameters:  	*YPluseLength   ydata ��С
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
//* Descriptions:  		e2rom ���� ��  ����      ʱ��     ���
//* input parameters:   	recordP  �������λ��    *record 	����Ľṹ��
//* output parameters:    ��
//* Returned value:       ��
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
//* Descriptions:        �Գ�����λ������ݣ� ���� ʱ�� ���
//* input parameters:    uint16 recordP  �������λ��,
//* output parameters:   MEAS_RECORD *record �����Ľṹ��
//* Returned value:      ��
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
//* Descriptions:         ������ϴ��������ѹ
//* input parameters:     ��
//* output parameters:    ��
//* Returned value:       ��
//*********************************************************************************************************/
//void Clean(void)
//{	 /*
//	������ϴʱ�����ϴ
//
//	��ϴ���λ
//	������ ��ʾ���
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
//* Descriptions:         �����ܳ���
//* input parameters:     ��
//* output parameters:    ��
//* Returned value:       ��
//*********************************************************************************************************/
//void AnodicStrippingVoltammetry(void)
//{
//	SetSWI1(1);					  /*ѡͨ����*/
//	SetSWI2(1);					  /*�˴���Ҫ�����������*/
//	/*����*/
//	/*
//	�������λ
//	ѡͨ��Ӧ�Ŀ��� ������
//	���ת�� ������ʱ��
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
//	/*����*/
//	/*
//	��������λ
//	��ʱ �� ����ʱ�䵽
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
//	/*����*/
//	/*
//	ֹͣ���
//	�����õ�λ������ʱ�䵽
//	*/
//	if(0!= measureParameter.staticTime)
//	{
//		AnalogOutput(measureParameter.staticVoltage);
//		TimeMeasCount = measureParameter.staticTime;
//		while(TimeMeasCount);
//	}
//
//
//	/*��ɨ����*/
//	/*
//	������������
//	��ʱ1s
//	��ɨ������ֵ
//	����һ�Σ�����ȡֵ
//	*/
//	SetSWI2(currentGain);
//	OSTimeDly((int32)(OS_TICKS_PER_SEC));
//	SetWaveform(&waveRunParameter,&waveParameter,measureParameter.MeasureType);
//	GetADValue();
//	/*
//	��ʼ��ɨ ����ʱ����ֵ��ʹ�ܶ�ʱ����
//	����ɨ��ʱ��
//	������ ��ɨ��ʱ��+������ʱʱ�� ������
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
//	��������
//	�����ƶ�ƽ������
//	*/
//	SetSWI2(0);					  /*�˴���Ҫ�����������*/
//	MovingAverage((int16 *)usRegInputBuf,waveRunParameter.timeCount,8);
//	MovingAverage((int16 *)usRegInputBuf,waveRunParameter.timeCount,8);
//	//�����ж����������320uA�����*100����֮����*10��ͬʱusRegInputBuf[98]=100����10
//	JudgeCurrent((int16 *)usRegInputBuf,waveRunParameter.timeCount);
//
//
//	/*��ϴ2*/
//	/*
//	���ת��
//	���ܳ���ϴ��ѹʹ�ø���������ȫ���ܽ��
//	��������ϴʱ�䵽
//	�ص��
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
//	�Ͽ�����
//	����ѡ��
//	*/
//	SetSWI1(0);
//	SetSWI2(0);
//	/*
//	������־����
//	������ ��ʾ����
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
//* Descriptions:         ���ܳ���λ����ϴ
//* input parameters:     ��
//* output parameters:    ��
//* Returned value:       ��
//*********************************************************************************************************/
//void Clean2(void)
//{
//	/*
//	���ܳ���ϴ��ѹ
//	��ͨ����
//	ѡ����
//	�����
//	������ ��ϴ��λ
//	*/
//	AnalogOutput(filmParameter.filmVoltage);
//	SetSWI1(1);
//	SetSWI2(0);					  /*�˴���Ҫ�����������*/
//	SET_MOTOR(1);
////	OSTimeDly((int32)(OS_TICKS_PER_SEC*filmParameter.filmTime));
//	TimeMeasCount = filmParameter.filmTime;
//	while(TimeMeasCount);
//	/*
//	�Ͽ�����
//	�ص��
//	���������־
//	������
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
//* Descriptions:         ��Ĥ
//* input parameters:     ��
//* output parameters:    ��
//* Returned value:       ��
//*********************************************************************************************************/
//void CoatingFilm(void)
//{
//	/*
//	����Ĥ��λ
//	ѡͨ����
//	ѡ������
//	�����
//	������ ��Ĥ ʱ�䵽
//	*/
//	AnalogOutput(filmParameter.filmVoltage);
//	SetSWI1(1);
//	SetSWI2(0);					  /*�˴���Ҫ�����������*/
//	SET_MOTOR(1);
////	OSTimeDly((int32)(OS_TICKS_PER_SEC*filmParameter.filmTime));
//	TimeMeasCount = filmParameter.filmTime;
//	while(TimeMeasCount);
//
//	/*
//	�ص��
//	�ص���
//	���������־
//	������
//	*/
//	SET_MOTOR(0);
//	SetSWI1(0);
//	usRegHoldingBuf[2] &= (~0x2000);
//	BuzzerCtrl(2,5,5);
//}
//
///*********************************************************************************************************
//* Function name:        defilm
//* Descriptions:         ȥĤ
//* input parameters:     ��
//* output parameters:    ��
//* Returned value:       ��
//*********************************************************************************************************/
//void defilm(void)
//{
//	/*
//	����Ĥ��λ
//	ѡͨ����
//	ѡ������
//	�����
//	������ ȥĤ ʱ�䵽
//	*/
//	AnalogOutput(filmParameter.filmVoltage);
//	SetSWI1(1);
//	SetSWI2(0);					  /*�˴���Ҫ�����������*/
//	SET_MOTOR(1);
////	OSTimeDly((int32)(OS_TICKS_PER_SEC*filmParameter.filmTime));
//	TimeMeasCount = filmParameter.filmTime;
//	while(TimeMeasCount);
//
//	/*
//	�ص��
//	�ص���
//	���������־
//	������
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
//* Descriptions:         ����
//* input parameters:     ��
//* output parameters:    ��
//* Returned value:       ��
//*********************************************************************************************************/
//void Regulate(void)
//{
//	/*
//	�����һ����һ��λ
//	���ӵ���
//	����ѡ��
//	��ʼCVɨ��
//	������ ʱ�����
//	*/
//	AnalogOutput(electrodeCondition.startVoltage);
//	SetSWI1(1);
//	SetSWI2(0);					  /*�˴���Ҫ�����������*/
//	StartCV(&waveRunParameter,&electrodeCondition,1);
//	TimeMeasCount = (int32)   ( electrodeCondition.Count \
//			  *2*(electrodeCondition.endVoltage - electrodeCondition.startVoltage)\
//	          /electrodeCondition.sweepSpeed) ;
//	while(TimeMeasCount);
//	/*
//	�ضϵ���
//	���������־
//	������
//	*/
//	SetSWI1(0);
//	usRegHoldingBuf[2] &= (~0x8000);
//	BuzzerCtrl(2,5,5);
//}
//
///*********************************************************************************************************
//* Function name:        ReadHistory
//* Descriptions:         ����ʷ����
//* input parameters:     ��
//* output parameters:    ��
//* Returned value:       ��
//*********************************************************************************************************/
//void ReadHistory(void)
//{
//	MEAS_RECORD rRecord;
//	/*��ȡ��Ӧ�����ʷ��¼*/
//	ReadRecord(recordPoint - usRegHoldingBuf[981] -1,&rRecord);
//	/*��ֵ��ʷ����*/
//	usRegHoldingBuf[980] = 	recordPoint;
//	/*����*/
//	usRegHoldingBuf[982] = 	rRecord.menuName[1]+(rRecord.menuName[0]<<8);
//	usRegHoldingBuf[983] = 	rRecord.menuName[3]+(rRecord.menuName[2]<<8);
//	usRegHoldingBuf[984] = 	rRecord.menuName[5]+(rRecord.menuName[4]<<8);
//	usRegHoldingBuf[985] = 	rRecord.menuName[7]+(rRecord.menuName[6]<<8);
//	usRegHoldingBuf[986] = 	rRecord.menuName[9]+(rRecord.menuName[8]<<8);
//	/*������*/
//	usRegHoldingBuf[987] = 	 rRecord.recordTime.year;
//	usRegHoldingBuf[988] = 	(rRecord.recordTime.month) & 0x00ff;
//	usRegHoldingBuf[989] = 	(rRecord.recordTime.date) & 0x00ff;
//	/*ʱ����*/
//	usRegHoldingBuf[990] = 	(rRecord.recordTime.hour) & 0x00ff;
//	usRegHoldingBuf[991] = 	(rRecord.recordTime.minute) & 0x00ff;
//	usRegHoldingBuf[992] = 	(rRecord.recordTime.second) & 0x00ff;
//	/*���*/
////	FloatToChArray(rRecord.recordResult,cc);
////	usRegHoldingBuf[993] = 	(cc[1]<<8)+cc[0];
////	usRegHoldingBuf[994] = 	(cc[3]<<8)+cc[2];
//	if((rRecord.recordResult<0.0)||(GetNumLen(rRecord.recordResult)==127)) //������Ч
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
