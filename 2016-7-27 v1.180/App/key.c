/****************************************Copyright (c)****************************************************
**                            		skyray-instrument Co.,LTD.
**
**                                 http://www.skyray-instrument.com
**
*********************************************************************************************************/

#include "global.h"

#define TIME_COEF	1
#define areaZeroError 10.0f
#define areaCalError 0.5f
uint8 volatile NePrOffset; //������ʾƫ��
uint8 menui = 0;	 //ͼ�� ѡ��
uint8  areaBar[4] = {221,26,238,158};
volatile uint8 OnlineFlg =1 ;
uint8 button;
uint8 NoBuzzry = 1;

uint8 SysRecordFlg;
uint8 SysRecordWHflg;
uint8 firstlook = 0;  //��һ�ζ�e2prom ��ʷ����
uint8 counterr,RestErr;
uint8 keyBuzzerBitFlg1 = 1;
uint8 KeyLockFlg = 1;
uint8 KeyLockFlg1=1;
signed char LCDContrast1=3;
int16 FilterRemain;
int16 rRecordFlg ;
int16 rRecordShowFlg;
int32 recordShowFlg = 0;
uint8 recordShowBlg = 0;
uint8 measAgainFlg = 0;	  //�Ƿ��ٴ� ������
uint8 blankMeasFlg = 0;	  //�Ƿ� �հ��� ������
uint8 standardMeasFlg = 0;	//�Ƿ������׼��
uint8 rangeFlg = 0;//��ǰѡ�е�����
uint8 rangeNum = 0;//�ܵ����̸���
uint8 rangeBlg = 0;//ͬһ��ҳ�������������־
uint8 keyFlg = 0;
uint8 keyEnterFlg = 0;
int8  OffsetFlg = 0;
uint8 ConfChange;
uint8 strKey[30];
uint8 DatForRecord[64];
uint8 DatForSysRecord[11];
volatile uint8 CheckCount = 0;   //��Ƭ������
TimeDat CaliTimeDate;
TimeDat TimeDateKey;
MEAS_RECORD rRecord[8];
uint8 OldAction;
volatile uint32 StandardMeasFlg = 0;
float		CaliParT1,CaliParT3,CaliParRH, CaliParP,  CaliParF1,	 CaliParF2,	CaliParPump,CaliParT4,CaliParRH2;
float		CaliSetT1,CaliSetT3,CaliSetRH, CaliSetP,  CaliSetF1,	 CaliSetF2,	CaliSetPump, CaliSetT4, CaliSetRH2;
MEAS_RECORD measRecord;

uint16 KeyTimer=0x0;
uint8  KeyUpRepeat=1;
uint8  KeyDownRepeat=1;
uint8  KeyLeftRepeat=1;
uint8  KeyRightRepeat=1;
uint8  KeyDRRepeat=1;
uint8  KeyDRWait=0;
uint8 MenuOffset;

int8 sign=1,SensorDatS[10];
fp32 SensorDat;
uint16 ReturnMenu0Time=RETURN_MENU0_TIME;



/*********************************************************************************************************
* Function name:    FindUpId
* Descriptions:      �ϼ� ��  �¸��ṹ�� ID
   					UP������ַ���һ������λ�������1��ô�ͼ�ȥ1�������1��ô�ͱ�ɸü��˵�����ͬ�˵�����
					//��1��0x01400000 UP�� 0x01300000
					//��2: ����������������ͬ�˵�����5��0x03100000 UP�� 0x03500000��
* input parameters:    CurrId  ��ǰID
 					   same	   ������ ��ͬ�˵�����
* output parameters:
* Returned value:       �ϼ� ���ID
*********************************************************************************************************/
uint32 FindUpId(uint32 CurrId,uint8 same)
{
    uint32 UpId,TmpId;
    uint8  i,j;
    uint32 k32,m32;

    UpId=CurrId;
    TmpId=CurrId>>8;
    k32=0x00000100;
    m32=0xfffff000;

    for(i=0,j=0; i<4; i++)
    {
        j=TmpId&0x0000000f;
        if(0!=j)
        {
            if(1==j)
            {
                UpId=(UpId&m32)|(same<<(8+i*4));
            }
            else
            {
                UpId=UpId-k32;
            }
            return UpId;
        }
        TmpId=TmpId>>4;
        k32=k32<<4;
        m32=m32<<4;
    }

    TmpId=CurrId&0xff000000;
    m32=(int32)same;
    if(0!=TmpId)
    {
        if(0x01000000==TmpId)
        {
            UpId=((int32)same)<<24;
        }
        else
        {
            UpId=UpId-0x01000000;
        }
        return UpId;
    }

    return 0;
}



/*********************************************************************************************************
* Function name:    FindDownId
* Descriptions:     �¼� ��  ��һ���ṹ�� ID
   					down������ַ���һ������λ�������1��ô�ͼ�ȥ1�������1��ô�ͱ�ɸü��˵�����ͬ�˵�����
					//��1��0x01300000 UP�� 0x01400000
					//��2: ����������������ͬ�˵�����5��0x03500000 down�� 0x03100000��
* input parameters:    CurrId  ��ǰID
 					   same	   ������ ��ͬ�˵�����
* output parameters:
* Returned value:       �¼� ���ID
*********************************************************************************************************/
uint32 FindDownId(uint32 CurrId,uint8 same)
{
    uint32 DownId,TmpId;
    uint8  i,j;
    uint32 k32,m32;

    DownId=CurrId;
    TmpId=CurrId>>8;
    k32=0x00000100;
    m32=0xfffff000;


    for(i=0,j=0; i<4; i++)
    {
        j=TmpId&0x0000000f;
        if(0!=j)
        {
            if(same==j)
            {
                DownId=(DownId&m32)|k32;
            }
            else
            {
                DownId=DownId+k32;
            }

            return DownId;
        }
        TmpId=TmpId>>4;
        k32=k32<<4;
        m32=m32<<4;
    }

    TmpId=CurrId&0xff000000;
    m32=(int32)same;
    if(0!=TmpId)
    {
        if(same==(TmpId>>24))
        {
            DownId=0x01000000;
        }
        else
        {
            DownId=DownId+0x01000000;
        }
        return DownId;
    }

    return 0;
}



/*********************************************************************************************************
* Function name: FindEnterId
* Descriptions:  ENTER������ַ���һ������λ�����ĺ���Ϊ��1������0x01000000 ENTER��0x01100000��
* input parameters:   currid ��ǰid
* output parameters:    ��
* Returned value:      ChildId enter���id
*********************************************************************************************************/
uint32 FindEnterId(uint32 CurrId)
{
    uint8  i,j;
    uint32 ChildId,TmpId,k32;

    if(0==CurrId)
        return 0x01000010;

    TmpId=CurrId>>12;
    k32=0x00000100;
    for(i=0,j=0; i<3; i++)
    {   /*
        �����һ�����㼶��
            �����һ�� ���㼶��������1
        */
        j=TmpId&0x0000000f;
        if(0!=j)
        {
            ChildId=(CurrId&0xffffff0f)|k32|0x00000010;
            return ChildId;
        }
        TmpId=TmpId>>4;
        k32=k32<<4;
    }
    ChildId=CurrId&0xff00000f|0x00100010;
    return  ChildId;
}

/*********************************************************************************************************
* Function name:    SaveHistoryMenu(uint32 CurrId,uint32 MHistory)
* Descriptions:
* input parameters:		currid
						MHistory
* output parameters:
* Returned value:       history
*********************************************************************************************************/
uint32 SaveHistoryMenu(uint32 CurrId,uint32 MHistory)
{
    uint32 History,TmpId;
    uint8  i,j;
    uint32 k32,m32;

    TmpId=CurrId>>8;
    k32=0x000000f0&CurrId;
    m32=0xfffff000;
    History=MHistory;

    for(i=0,j=0; i<4; i++)
    {
        j=TmpId&0x0000000f;
        if(0!=j)  //�����һ��
        {
            History =(History&m32)|k32<<(4*(i+1));
            return History;
        }
        TmpId=TmpId>>4;
        m32=m32<<4;
    }
    /*���˵�*/
    TmpId=CurrId&0xff000000;
    if(0!=TmpId)
    {
        History =k32<<20;
        return History;
    }
    return 0;

}
/*********************************************************************************************************
* Function name:  ReadHistoryMenu(uint32 CurrId)
* Descriptions:      ��ȡ ��ʷ ��ַ ���ڷ��ص��ϴΰ��� enter�ĵط�
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/

uint8 ReadHistoryMenu(uint32 CurrId)
{
    uint32 TmpId;
    uint8  i,j,place;

    TmpId=CurrId>>8;

    for(i=0,j=0; i<4; i++)
    {
        j=TmpId&0x0000000f;
        if(0!=j)
        {
            place=(MenuHistory>>(12+i*4))&0x0f;
            return place;
        }
        TmpId=TmpId>>4;
    }
    return 0;
}

/*********************************************************************************************************
* Function name:  FindReturnId(uint32 CurrId)
* Descriptions:      ���ؼ� ��ID �Ĳ���   	  ��ǰID�����һ��������һ��
* input parameters:
* output parameters:
* Returned value:
*********************************************************************************************************/
uint32 FindReturnId(uint32 CurrId)
{
    uint32 TmpId,ReId,k32;
    uint8  i,j,place;
    /*������ ʧЧ*/
    if(CurrId&0x0fffff0f==0)
    {
        return 0;
    }

    TmpId=CurrId>>8;
    k32=0xfffff000;

    for(i=0,j=0; i<4; i++)
    {   /*
        �����һ��
        	   ��ȡ�ϼ��˵� ��enterʱ���ֵģ�
           �������һ��������һ��
        */
        j=TmpId&0x0000000f;
        if(0!=j)
        {
            ReId=CurrId&(k32<<(i*4));
            place=ReadHistoryMenu(CurrId);
            ReId |=(place<<4);
            return ReId;
        }
        TmpId=TmpId>>4;
    }
    return 0;
}

/*********************************************************************************************************
* Function name:   FindLab
* Descriptions:    ���� ����Ľṹ����λ
* input parameters:   inId��ǰID
* output parameters:     ��
* Returned value:     ��ǰID��Ӧ�Ľṹ�� ��λ
*********************************************************************************************************/
uint8 FindLab(uint32 inId)
{
    uint8 inLab;
    //uint8 const err[20]={"Lab no find!"};
    uint32 tab_id;
    tab_id=inId&0xffffff00;
    for(inLab=0; inLab<MENU_SIZE; inLab++)
    {
        //if((inId&0xffffff00)==MenuTab[inLab].Id)
        if(tab_id==MenuTab[inLab].Id)
        {
            //Delay(1);
            return inLab;
        }
    }
    return MENU_SIZE;
}

/*********************************************************************************************************
* Function name:        UpKeyAutoPorc
* Descriptions:    		//�ϼ����º�  MENUID �仯  �� ģ�黯����ʾ����
						//UP������ַ���һ������λ�������1��ô�ͼ�ȥ1�������1��ô�ͱ�ɸü��˵�����ͬ�˵�����
						//��1��0x01400000 UP�� 0x01300000
						//��2: ����������������ͬ�˵�����5��0x03100000 UP�� 0x03500000��
* input parameters:     allID:	��ǰID
* output parameters:    ��
* Returned value:       �ɹ� 0
						���ɹ� 	MENU_SIZE
*********************************************************************************************************/
uint32 UpKeyAutoPorc(uint32 allId)
{
    uint8  i,
           currLab,
           nextLab,
           downLab[10],
           upLab[10],
           area[4];
    uint8  place;
    uint32 nextId,
           downId[10],upId[10];
    uint16 slen;
    uint8 mp;
    /*
    �ҳ� ID��Ӧ�Ľṹ����λ
    	δ�ҵ�
    */
    currLab=FindLab(MenuId);
    if(MENU_SIZE==currLab)
    {
        return currLab;
    }
    /*
    ��һ���ṹ��ID
    ��һ���ṹ��
    */
    nextId=FindUpId(MenuId,MenuTab[currLab].Same);
    nextLab=FindLab(nextId);
    /*˵ѡ������ĸ��˵�*/
    place=(MenuId&0x000000f0)>>4;
    if(1==MenuTab[nextLab].Sel)/*��ʽ���Ʒ�  1���б���*/
    {   /*���һ������λ��ֵ*/
        for(i = 2; i<8; i++)
        {
            mp = ((nextId&0xffffff00)>>i*4)&0x0f;
            if(mp)
            {
                break;
            }
        }
        /*ѡ�в˵�λ�ò��ڶ���  ��ֱ�������ƶ�һ����ʾ*/
        if(place>=2)
        {   /*��ʾѡ�еĲ˵�*/
            CReverseVideo(MenuFormat.ChArea[place-1]);
            place--;
            CReverseVideo(MenuFormat.ChArea[place-1]);
            /*������*/
            CleanVideo(areaBar);
            DarwScrollBar(mp,MenuTab[currLab].Same);
        }
        /*ѡ�в˵�ʱ��ǰ����ĵ�һ����ʾλ��*/
        else
        {   /*��ʾ����*/
            ClrDisBuf();
            if(1==MenuFormat.Title)
            {   /*
                ���㺺��������ֽ���
                ��ʾ���⺺��
                ���򷴰�
                */
                if(language==1)
                {
                    slen=LzgChineselen(MenuTab[nextLab].TEn);
                    DisCList16((DIS_AREA_X-slen*16)/2,MenuFormat.TitleY1,MenuTab[nextLab].TEn);
                }
                else
                {
                    slen=LzgChineselen(MenuTab[nextLab].TCh);
                    DisCList16((DIS_AREA_X-slen*16)/2,MenuFormat.TitleY1,MenuTab[nextLab].TCh);
                }
                CReverseVideo(MenuFormat.TiArea1);

            }
            /*��ǰ�˵��������еĵ�һ��*/
            if(nextId<MenuId)
            {   /*��ʾ�Լ��Ĳ˵���Ϣ
                  ������λ
                  ���⺺��
                */
                DisCList16(MenuFormat.ChildXY[0][0],MenuFormat.ChildXY[0][1],
                           MenuTab[nextLab].place);

                if(language==1)
                {
                    DisCList16(MenuFormat.ChildXY[0][0]+20,MenuFormat.ChildXY[0][1],
                               MenuTab[nextLab].CEn);
                }
                else
                {
                    DisCList16(MenuFormat.ChildXY[0][0]+20,MenuFormat.ChildXY[0][1],
                               MenuTab[nextLab].CCh);
                }
                CReverseVideo(MenuFormat.ChArea[0]);
                /*��ʾ����ļ����˵�
                ���µ�ID
                ���µĽṹ��
                ��ʾ���µ� Id��Ӧ�� �˵���Ϣ
                */
                downId[2]=FindDownId(nextId,MenuTab[nextLab].Same);
                downLab[2]=FindLab(downId[2]);
                for(i=2; i<=MenuTab[nextLab].Show; i++)
                {
                    DisCList16(MenuFormat.ChildXY[i-1][0],MenuFormat.ChildXY[i-1][1],
                               MenuTab[downLab[i]].place);

                    if(language==1)
                    {
                        DisCList16(MenuFormat.ChildXY[i-1][0]+20,MenuFormat.ChildXY[i-1][1],
                                   MenuTab[downLab[i]].CEn);
                    }
                    else
                    {
                        DisCList16(MenuFormat.ChildXY[i-1][0]+20,MenuFormat.ChildXY[i-1][1],
                                   MenuTab[downLab[i]].CCh);
                    }

                    downId[i+1]=FindDownId(downId[i],MenuTab[nextLab].Same);
                    downLab[i+1]=FindLab(downId[i+1]);
                }
                /*������*/
                DarwScrollBar(mp,MenuTab[currLab].Same);
            }
            /*
            �����в˵��ĵ�һ��
            */
            else
            {   /*��ʾ�Լ�����Ϣ
                ����ʾλ���������һ��λ�ã�
                ��ʾ�˵���λ
                ��ʾ�˵�����
                ��ʾ��������
                */
                place=MenuTab[nextLab].Show;
                DisCList16(MenuFormat.ChildXY[MenuTab[nextLab].Show-1][0],
                           MenuFormat.ChildXY[MenuTab[nextLab].Show-1][1],
                           MenuTab[nextLab].place);
                if(language==1)
                {
                    DisCList16(MenuFormat.ChildXY[MenuTab[nextLab].Show-1][0]+20,
                               MenuFormat.ChildXY[MenuTab[nextLab].Show-1][1],
                               MenuTab[nextLab].CEn);
                }
                else
                {
                    DisCList16(MenuFormat.ChildXY[MenuTab[nextLab].Show-1][0]+20,
                               MenuFormat.ChildXY[MenuTab[nextLab].Show-1][1],
                               MenuTab[nextLab].CCh);
                }
                slen=MenuTab[nextLab].Show-1;
                CReverseVideo(MenuFormat.ChArea[slen]);
                /*��ʾ���漸�� ID��Ӧ����ʾ����*/
                upId[MenuTab[nextLab].Show-1]=FindUpId(nextId,MenuTab[nextLab].Same);
                upLab[MenuTab[nextLab].Show-1]=FindLab(upId[MenuTab[nextLab].Show-1]);
                for(i=MenuTab[nextLab].Show-1; i>=1; i--)
                {
                    DisCList16(MenuFormat.ChildXY[i-1][0],MenuFormat.ChildXY[i-1][1],
                               MenuTab[upLab[i]].place);
                    if(language==1)
                    {
                        DisCList16(MenuFormat.ChildXY[i-1][0]+20,MenuFormat.ChildXY[i-1][1],
                                   MenuTab[upLab[i]].CEn);
                    }
                    else
                    {
                        DisCList16(MenuFormat.ChildXY[i-1][0]+20,MenuFormat.ChildXY[i-1][1],
                                   MenuTab[upLab[i]].CCh);
                    }
                    upId[i-1] =FindUpId(upId[i],MenuTab[nextLab].Same);
                    upLab[i-1]=FindLab(upId[i-1]);
                }
                /*������*/
                DarwScrollBar(MenuTab[currLab].Same,MenuTab[currLab].Same);
            }
        }
    }
    /*�˵���ʽ ���Ʒ� 2��û���õ�*/
//	else if(2==MenuTab[nextLab].Sel)
//	{
//		DisCList16(MenuFormat.ChildXY[1][0]+20,MenuFormat.ChildXY[1][1],
//		MenuTab[nextLab].CCh);
//		DarwButton(5,31);
//	}
    /*�˵���ʽ ���Ʒ� 4��ͼ��ʽ ������*/
    else if(4==MenuTab[nextLab].Sel)
    {   /*���������*/
        mp = (MenuId&0x0f000000)>>24;

        area[0] = 0;
        area[1] = 22;
        area[2] = 239;
        area[3] = 159;
        CleanVideo(area);		/*��������*/
        /*��6��ͼ��*/
        pic56x56(12,40,pic[0]);
        pic56x56(92,40,pic[1]);
        pic56x56(172,40,pic[2]);
        pic56x56(12,99,pic[3]);
        pic56x56(92,99,pic[4]);
        pic56x56(172,99,pic[5]);
        /*����2���˵�
        	��ʾ��ǰ
        	�����ǰ��һ����ʾЧ��
        */
        if(mp>=2)
        {
            if(language==1)
            {
                slen=LzgChineselen(MenuTab[nextLab].CEn);
                DisCList16((DIS_AREA_X-slen*16)/2,20,MenuTab[nextLab].CEn);
            }
            else
            {
                slen=LzgChineselen(MenuTab[nextLab].CCh);
                DisCList16((DIS_AREA_X-slen*16)/2,20,MenuTab[nextLab].CCh);
            }
            CReverseVideo(areaMenu[mp-1]);
        }
        /*ֻ��һ����2���˵�
        	��ʾ��ǰ
        	�������ȫ��
        */
        else
        {
            if(language==1)
            {
                slen=LzgChineselen(MenuTab[nextLab].CEn);
                DisCList16((DIS_AREA_X-slen*16)/2,20,MenuTab[nextLab].CEn);
            }
            else
            {
                slen=LzgChineselen(MenuTab[nextLab].CCh);
                DisCList16((DIS_AREA_X-slen*16)/2,20,MenuTab[nextLab].CCh);
            }
            CReverseVideo(areaMenu[6]);
        }
        /*����ʾλ�ø�ֵ*/
        MenuId=(nextId&0xffffff0f)|(place<<4);
        return 0;
    }
    if(3==MenuTab[nextLab].Sel)
    {

        MainMeauFlg++;
        if(MainMeauFlg>3)
        {
            MainMeauFlg = 0;
        }
        switch (MainMeauFlg)
        {
        case 0:
            MainMenu();
            break;
        case 1:
            MainMenuSta();
            break;
        case 2:
            MainMenuSen2();
            break;
        case 3:
            MainMenuSen();
            break;
        default:
            break;
        }
    }
    MenuId=(nextId&0xffffff0f)|(place<<4);
    return 0;
}

/*********************************************************************************************************
* Function name:     	DownKeyAutoPorc(uint32 allId)
* Descriptions:      	DOWN������ַ���һ������λ��������ڸý�����ͬ�˵�����ô�ͼ�1���������ô�ͱ��1��
						����0x01400000 DOWN�� 0x01500000
						��2: ����������������ͬ�˵�����5��0x03500000 DOWN�� 0x03100000��
* input parameters:    	allId  ��ǰID
* output parameters:   	��
* Returned value: 		�ɹ� 0
						���ɹ� MENU_SIZE
*********************************************************************************************************/
uint32 DownKeyAutoPorc(uint32 allId)
{
    uint8  i,
           currLab,
           nextLab,
           upLab[10],
           downLab[10];
    uint8  place;
    uint32 nextId,
           upId[10],
           downId[10];
    uint16 slen;
    uint8 mp;
    /*
    �ҳ� ID��Ӧ�Ľṹ����λ
    	δ�ҵ�
    */

    currLab=FindLab(MenuId);
    if(MENU_SIZE==currLab)
    {
        return currLab;
    }
    /*
    ����һ���ṹ��ID
    ��һ���ṹ����λֵ
    ��ǰѡ��Ĳ˵�
    */
    nextId=FindDownId(MenuId,MenuTab[currLab].Same);
    nextLab=FindLab(nextId);
    place=(MenuId&0x000000f0)>>4;

    if(1==MenuTab[nextLab].Sel)  /*�˵���ʽ���Ʒ�  �б�ʽ*/
    {   /*���һ������4λ��ֵ*/
        for(i = 2; i<8; i++)
        {
            mp = ((nextId&0xffffff00)>>i*4)&0x0f;
            if(mp)
            {
                break;
            }
        }
        /*��ǰ�˵��������һ���˵�
        	��ʾ��ǰ��Ϣ
        */
        if(place<MenuTab[nextLab].Show)
        {
            CReverseVideo(MenuFormat.ChArea[place-1]);
            place++;
            CReverseVideo(MenuFormat.ChArea[place-1]);
            /*������*/
            CleanVideo(areaBar);
            DarwScrollBar(mp,MenuTab[currLab].Same);

        }
        /*��ǰ�����������һ���˵�*/
        else
        {   /*��ʾ����*/
            ClrDisBuf();
            if(1==MenuFormat.Title)
            {
                if(language==1)
                {
                    slen=LzgChineselen(MenuTab[nextLab].TEn);
                    DisCList16((DIS_AREA_X-slen*16)/2,MenuFormat.TitleY1,MenuTab[nextLab].TEn);
                }
                else
                {
                    slen=LzgChineselen(MenuTab[nextLab].TCh);
                    DisCList16((DIS_AREA_X-slen*16)/2,MenuFormat.TitleY1,MenuTab[nextLab].TCh);
                }
                CReverseVideo(MenuFormat.TiArea1);
            }
            /*�������в˵������һ��*/
            if(nextId>MenuId)
            {   /*��ʾ��ǰ�˵���Ϣ
                �˵���λ
                �˵�����
                ��������
                */
                place=MenuTab[nextLab].Show;
                DisCList16(MenuFormat.ChildXY[MenuTab[nextLab].Show-1][0],
                           MenuFormat.ChildXY[MenuTab[nextLab].Show-1][1],
                           MenuTab[nextLab].place);
                if(language==1)
                {
                    DisCList16(MenuFormat.ChildXY[MenuTab[nextLab].Show-1][0]+20,
                               MenuFormat.ChildXY[MenuTab[nextLab].Show-1][1],
                               MenuTab[nextLab].CEn);
                }
                else
                {
                    DisCList16(MenuFormat.ChildXY[MenuTab[nextLab].Show-1][0]+20,
                               MenuFormat.ChildXY[MenuTab[nextLab].Show-1][1],
                               MenuTab[nextLab].CCh);
                }
                slen=MenuTab[nextLab].Show-1;
                CReverseVideo(MenuFormat.ChArea[slen]);
                /*��ʾ����ļ����˵���Ϣ*/
                upId[MenuTab[nextLab].Show-1]=FindUpId(nextId,MenuTab[nextLab].Same);
                upLab[MenuTab[nextLab].Show-1]=FindLab(upId[MenuTab[nextLab].Show-1]);
                for(i=MenuTab[nextLab].Show-1; i>=1; i--)
                {
                    DisCList16(MenuFormat.ChildXY[i-1][0],MenuFormat.ChildXY[i-1][1],
                               MenuTab[upLab[i]].place);
                    if(language==1)
                    {
                        DisCList16(MenuFormat.ChildXY[i-1][0]+20,MenuFormat.ChildXY[i-1][1],
                                   MenuTab[upLab[i]].CEn);
                    }
                    else
                    {
                        DisCList16(MenuFormat.ChildXY[i-1][0]+20,MenuFormat.ChildXY[i-1][1],
                                   MenuTab[upLab[i]].CCh);
                    }
                    upId[i-1] =FindUpId(upId[i],MenuTab[nextLab].Same);
                    upLab[i-1]=FindLab(upId[i-1]);
                }
                DarwScrollBar(mp,MenuTab[currLab].Same);
            }
            /*�����в˵������һ��*/
            else
            {   /*�������һ����ʾλ��
                ��ʾ��ǰ�˵���Ϣ
                �˵���λ
                �˵�����
                �˵���������
                */
                place=1;
                DisCList16(MenuFormat.ChildXY[0][0],MenuFormat.ChildXY[0][1],
                           MenuTab[nextLab].place);
                if(language==1)
                {
                    DisCList16(MenuFormat.ChildXY[0][0]+20,MenuFormat.ChildXY[0][1],
                               MenuTab[nextLab].CEn);
                }
                else
                {
                    DisCList16(MenuFormat.ChildXY[0][0]+20,MenuFormat.ChildXY[0][1],
                               MenuTab[nextLab].CCh);
                }
                CReverseVideo(MenuFormat.ChArea[0]);
                /*��ʾ����ļ���ID��Ӧ�Ĳ˵���Ϣ*/
                downId[2]=FindDownId(nextId,MenuTab[nextLab].Same);
                downLab[2]=FindLab(downId[2]);
                for(i=2; i<=MenuTab[nextLab].Show; i++)
                {
                    DisCList16(MenuFormat.ChildXY[i-1][0],MenuFormat.ChildXY[i-1][1],
                               MenuTab[downLab[i]].place);

                    if(language==1)
                    {
                        DisCList16(MenuFormat.ChildXY[i-1][0]+20,MenuFormat.ChildXY[i-1][1],
                                   MenuTab[downLab[i]].CEn);
                    }
                    else
                    {
                        DisCList16(MenuFormat.ChildXY[i-1][0]+20,MenuFormat.ChildXY[i-1][1],
                                   MenuTab[downLab[i]].CCh);
                    }

                    downId[i+1]=FindDownId(downId[i],MenuTab[nextLab].Same);
                    downLab[i+1]=FindLab(downId[i+1]);
                }
                DarwScrollBar(1,MenuTab[currLab].Same);
            }
        }
    }
    else if(4==MenuTab[nextLab].Sel)  /*�˵���ʽ4  ͼ��ʽ ������*/
    {
        /*������ ��6��ͼ��*/
        mp = (MenuId&0x0f000000)>>24;
        area[0] = 0;
        area[1] = 22;
        area[2] = 239;
        area[3] = 159;
        CleanVideo(area);		/*��������*/
        pic56x56(12,40,pic[0]);
        pic56x56(92,40,pic[1]);
        pic56x56(172,40,pic[2]);
        pic56x56(12,99,pic[3]);
        pic56x56(92,99,pic[4]);
        pic56x56(172,99,pic[5]);
        /*�������һ��
        	λ������һ��λ����
        */
        if(mp<6)
        {
            if(language==1)
            {
                slen=LzgChineselen(MenuTab[nextLab].CEn);
                DisCList16((DIS_AREA_X-slen*16)/2,20,MenuTab[nextLab].CEn);
            }
            else
            {
                slen=LzgChineselen(MenuTab[nextLab].CCh);
                DisCList16((DIS_AREA_X-slen*16)/2,20,MenuTab[nextLab].CCh);
            }
            CReverseVideo(areaMenu[mp+1]);
        }
        /*�����һ��
        	������һ����ʾλ��
        */
        else
        {
            if(language==1)
            {
                slen=LzgChineselen(MenuTab[nextLab].CEn);
                DisCList16((DIS_AREA_X-slen*16)/2,20,MenuTab[nextLab].CEn);
            }
            else
            {
                slen=LzgChineselen(MenuTab[nextLab].CCh);
                DisCList16((DIS_AREA_X-slen*16)/2,20,MenuTab[nextLab].CCh);
            }
            CReverseVideo(areaMenu[1]);
        }
        MenuId=(nextId&0xffffff0f)|(place<<4);
        return 0;
    }

    if(3==MenuTab[nextLab].Sel)
    {

        MainMeauFlg--;
        if(MainMeauFlg<0)
        {
            MainMeauFlg = 3;
        }
        switch (MainMeauFlg)
        {
        case 0:
            MainMenu();
            break;
        case 1:
            MainMenuSta();
            break;
        case 2:
            MainMenuSen2();
            break;
        case 3:
            MainMenuSen();
            break;
        default:
            break;
        }

    }

    /*��ʾλ�ø���*/
    MenuId=(nextId&0xffffff0f)|(place<<4);
    return 0;
}


/*********************************************************************************************************
* Function name:   EnterKeyAutoPorc
* Descriptions:    ȷ���� ��MENUID �仯
* input parameters:  allid ��ǰid
* output parameters: ��
* Returned value:    	0  �ɹ�
						MenuId	�ɹ�
						currLab   	ʧ��
*********************************************************************************************************/
uint32 EnterKeyAutoPorc(uint32 allId)
{
    uint8  i,
           currLab,
           nextLab,
           downLab[10],
           area[4];
    uint32 nextId,
           downId[10];
    uint16 slen;
    /*
    �ҳ� ID��Ӧ�Ľṹ����λ
    	δ�ҵ�
    	���Ӳ˵�
    */
    currLab=FindLab(MenuId);
    if(MENU_SIZE==currLab)
    {
        return currLab;
    }
    if(0==MenuTab[currLab].Lower)
    {
        return MenuId;
    }
    /*
    enter���ID
    ENTER ��ID��Ӧ�Ľṹͼ��λ
    	ʧ��
    */
    nextId=FindEnterId(MenuId);
    nextLab=FindLab(nextId);
    if(MENU_SIZE==nextLab)
    {
        return MENU_SIZE;
    }

    ClrDisBuf();
    if(0==MenuTab[nextLab].Sel) /*��ʽ��  0 */
    {   /*����*/
        if(1==MenuFormat.Title)
        {
            if(language==1)
            {
                slen=LzgChineselen(MenuTab[nextLab].TEn);
                DisCList16((DIS_AREA_X-slen*16)/2,MenuFormat.TitleY1,MenuTab[nextLab].TEn);
            }
            else
            {
                slen=LzgChineselen(MenuTab[nextLab].TCh);
                DisCList16((DIS_AREA_X-slen*16)/2,MenuFormat.TitleY1,MenuTab[nextLab].TCh);
            }
            CReverseVideo(MenuFormat.TiArea1);
        }
    }

    else if(1==MenuTab[nextLab].Sel)              // ��ʽ����1 �б�ʽ  ѡ�и�ʽ�з���
    {   /*����*/
        if(1==MenuFormat.Title)
        {
            if(language==1)
            {
                slen=LzgChineselen(MenuTab[nextLab].TEn);
                DisCList16((DIS_AREA_X-slen*16)/2,MenuFormat.TitleY1,MenuTab[nextLab].TEn);
            }
            else
            {
                slen=LzgChineselen(MenuTab[nextLab].TCh);
                DisCList16((DIS_AREA_X-slen*16)/2,MenuFormat.TitleY1,MenuTab[nextLab].TCh);
            }
            CReverseVideo(MenuFormat.TiArea1);
        }
        /*��ʾ��ǰ�Ĳ˵���Ϣ*/
        DisCList16(MenuFormat.ChildXY[0][0],MenuFormat.ChildXY[0][1],
                   MenuTab[nextLab].place);
        if(language==1)
        {
            DisCList16(MenuFormat.ChildXY[0][0]+20,MenuFormat.ChildXY[0][1],
                       MenuTab[nextLab].CEn);
        }
        else
        {
            DisCList16(MenuFormat.ChildXY[0][0]+20,MenuFormat.ChildXY[0][1],
                       MenuTab[nextLab].CCh);
        }
        CReverseVideo(MenuFormat.ChArea[0]);
        /*��ʾ��ǰ�˵�����ļ����˵���Ϣ*/
        downId[2]=FindDownId(nextId,MenuTab[nextLab].Same);
        downLab[2]=FindLab(downId[2]);
        for(i=2; i<=MenuTab[nextLab].Show; i++)
        {
            DisCList16(MenuFormat.ChildXY[i-1][0],MenuFormat.ChildXY[i-1][1],
                       MenuTab[downLab[i]].place);
            if(language==1)
            {
                DisCList16(MenuFormat.ChildXY[i-1][0]+20,MenuFormat.ChildXY[i-1][1],
                           MenuTab[downLab[i]].CEn);
            }
            else
            {
                DisCList16(MenuFormat.ChildXY[i-1][0]+20,MenuFormat.ChildXY[i-1][1],
                           MenuTab[downLab[i]].CCh);
            }
            downId[i+1]=FindDownId(downId[i],MenuTab[nextLab].Same);
            downLab[i+1]=FindLab(downId[i+1]);
        }
        DarwScrollBar(1,MenuTab[nextLab].Same);
    }
    else if(2==MenuTab[nextLab].Sel)        /* //ѡ�и�ʽ��ť	   ����	*/
    {
        DisCList16(MenuFormat.ChildXY[1][0]+20,MenuFormat.ChildXY[1][1],
                   MenuTab[nextLab].CCh);
        DarwButton(5,31);
    }
    else if(4==MenuTab[nextLab].Sel) /*ͼ��ʽ*/
    {   /*��6ͼ��*/
        area[0] = 0;
        area[1] = 25;
        area[2] = 239;
        area[3] = 159;
        CleanVideo(area);		/*��������*/

        pic56x56(12,40,pic[0]);
        pic56x56(92,40,pic[1]);
        pic56x56(172,40,pic[2]);
        pic56x56(12,99,pic[3]);
        pic56x56(92,99,pic[4]);
        pic56x56(172,99,pic[5]);
        /*����*/
        if(language==1)
        {
            slen=LzgChineselen(MenuTab[nextLab].CEn);
            DisCList16((DIS_AREA_X-slen*16)/2,20,MenuTab[nextLab].CEn);
        }
        else
        {
            slen=LzgChineselen(MenuTab[nextLab].CCh);
            DisCList16((DIS_AREA_X-slen*16)/2,20,MenuTab[nextLab].CCh);
        }
        CReverseVideo(MenuFormat.TiArea1);
        CReverseVideo(areaMenu[1]);
        /*�����ϼ��˵� ���㷵��*/
        MenuHistory=SaveHistoryMenu(MenuId,MenuHistory);
        MenuId=nextId;
        return 0;
    }
    else if(3==MenuTab[nextLab].Sel)
    {
        switch (MainMeauFlg)
        {
        case 0:
            MainMenu();
            break;
        case 1:
            MainMenuSta();
            break;
        case 2:
            MainMenuSen2();
            break;
        case 3:
            MainMenuSen();
            break;
        default:
            break;
        }
        CReverseVideo(MenuFormat.TiArea1);
    }

    /*�����ϼ��˵� ���㷵��*/
    //LcdUpdata(DisBuf);
    MenuHistory=SaveHistoryMenu(MenuId,MenuHistory);
    MenuId=nextId;
    return 0;
}


/*********************************************************************************************************
* Function name:  EscKeyAutoPorc(uint32 allId)
* Descriptions:   ���ؼ� ����
* input parameters:    allID��ǰID
* output parameters:    ��
* Returned value:       0
*********************************************************************************************************/
uint32 EscKeyAutoPorc(uint32 allId)
{
    uint8  i,
           place,
           nextLab,
           TmpLab,
           downLab[10];

    uint32 nextId,
           TmpId,
           downId[10];
    uint16 slen;
    uint8 mp;
    /*
    �ҳ� ID��Ӧ�Ľṹ����λ
    	δ�ҵ�
    �ҳ���Ӧ��λ�Ľṹͼ
    */
    nextId=FindReturnId(allId);
    nextLab=FindLab(nextId);
    ClrDisBuf();
    /*
    �˵��ṹ�� ͼ��ʽ  Ϊ������
    */
    if(3==MenuTab[nextLab].Sel)
    {
        switch (MainMeauFlg)
        {
        case 0:
            MainMenu();
            break;
        case 1:
            MainMenuSta();
            break;
        case 2:
            MainMenuSen2();
            break;
        case 3:
            MainMenuSen();
            break;
        default:
            break;
        }
        CReverseVideo(MenuFormat.TiArea1);
    }

    if(4==MenuTab[nextLab].Sel)
    {   /*��6ͼ��*/
        mp = (nextId&0x0f000000)>>24;

        pic56x56(12,40,pic[0]);
        pic56x56(92,40,pic[1]);
        pic56x56(172,40,pic[2]);
        pic56x56(12,99,pic[3]);
        pic56x56(92,99,pic[4]);
        pic56x56(172,99,pic[5]);
        if(language==1)
        {
            slen=LzgChineselen(MenuTab[nextLab].CEn);
            DisCList16((DIS_AREA_X-slen*16)/2,20,MenuTab[nextLab].CEn);
        }
        else
        {
            slen=LzgChineselen(MenuTab[nextLab].CCh);
            DisCList16((DIS_AREA_X-slen*16)/2,20,MenuTab[nextLab].CCh);
        }
        CReverseVideo(areaMenu[mp]);
        CReverseVideo(MenuFormat.TiArea1);

        MenuId=nextId;
        return 0;
    }
    /*��ͼ��ʽ  */
    else if(1==MenuTab[nextLab].Sel)
    {   /*����*/
        if(1==MenuFormat.Title)
        {
            if(language==1)
            {
                slen=LzgChineselen(MenuTab[nextLab].TEn);
                DisCList16((DIS_AREA_X-slen*16)/2,MenuFormat.TitleY1,MenuTab[nextLab].TEn);
            }
            else
            {
                slen=LzgChineselen(MenuTab[nextLab].TCh);
                DisCList16((DIS_AREA_X-slen*16)/2,MenuFormat.TitleY1,MenuTab[nextLab].TCh);
            }
            CReverseVideo(MenuFormat.TiArea1);
        }
        /*��ǰѡ��Ĳ˵�λ��*/
        place=(nextId&0x000000f0)>>4;

        TmpId=nextId;
        TmpLab=nextLab;
        /*����Ĳ˵��ṹ����Ϣ*/
        for(i=place; i>=1; i--)
        {
            downId[i]=TmpId;
            downLab[i]=TmpLab;
            TmpId=FindUpId(TmpId,MenuTab[TmpLab].Same);
            TmpLab=FindLab(TmpId);
        }
        /*����Ľṹ����Ϣ*/
        for(i=2; i<=MenuTab[nextLab].Show; i++)
        {
            downId[i]=FindDownId(downId[i-1],MenuTab[downLab[i-1]].Same);
            downLab[i]=FindLab(downId[i]);
        }
        /*��ҳ��ʾ*/
        for(i=1; i<=MenuTab[nextLab].Show; i++)
        {
            DisCList16(MenuFormat.ChildXY[i-1][0],MenuFormat.ChildXY[i-1][1],
                       MenuTab[downLab[i]].place);

            if(language==1)
            {
                DisCList16(MenuFormat.ChildXY[i-1][0]+20,MenuFormat.ChildXY[i-1][1],
                           MenuTab[downLab[i]].CEn);
            }
            else
            {
                DisCList16(MenuFormat.ChildXY[i-1][0]+20,MenuFormat.ChildXY[i-1][1],
                           MenuTab[downLab[i]].CCh);
            }

            if(i==place)
            {
                if(1==MenuTab[nextLab].Sel)
                {
                    CReverseVideo(MenuFormat.ChArea[i-1]);
                }
                else if(2==MenuTab[nextLab].Sel)  /*û�õ�*/
                {
                    DarwButton(5,31);
                }
            }
        }

        for(i = 2; i<8; i++)
        {   /*�����һ�����㼶��ֵ*/
            mp = ((nextId&0xffffff00)>>i*4)&0x0f;
            if(mp)
            {
                break;
            }
        }
        DarwScrollBar(mp,MenuTab[nextLab].Same);
    }
    button=0;
    MenuId=nextId;
    return 0;
}



/*********************************************************************************************************
* Function name:   uint32 PorcUpKey(uint32 KeyVol)
* Descriptions:    �ϼ����º�Ĳ���
* input parameters:  KeyVol�� NO  USE
* output parameters:  NO
* Returned value:     0
*********************************************************************************************************/
uint32 PorcUpKey(uint32 KeyVol)
{
    MenuOffset = (((MenuId&0x00f00000)>>20) - ((MenuId&0x000000f0)>>4));	//�б��˵����22
    /*��ǰMENUID��ֵ*/
    if((button==1)&&(StopFlg == 0))
    {
        return 0;
    }
    switch(MenuId&0xffffff0f)
    {   /*
        �������ý���
        */
    case 0x01100000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            RunStyleFlg	= 1^RunStyleFlg;
            return 0;
        }
        break;
    case 0x01200000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            Humidity = LoadSensorDat(Humidity,PARAM_SHIDUMENFA,1);
            return 0;
        }
        break;
    case 0x01300000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            MaxHeatTemp = LoadSensorDat(MaxHeatTemp,PARAM_MAXHEAT,1);
            return 0;
        }
        break;

    case 0x01400000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            MinHeatPower = LoadSensorDat(MinHeatPower,PARAM_MINHEATPOWER,1);
            return 0;
        }
        break;
    case 0x01500000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            FlowOfSamp = LoadSensorDat(FlowOfSamp,PARAM_LIULIANG,1);
            return 0;
        }
        break;
    case 0x01720000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            CycleOfSamp = LoadSensorDat(CycleOfSamp,PARAM_CYCLEOFSAMP,1);
            return 0;
        }
        break;

    case 0x01730000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            Mass = LoadSensorDat(Mass,PARAM_MASS,1);
            return 0;
        }
        break;

    case 0x01710000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            TimeType	= 1^TimeType;
            return 0;
        }
        break;
    case 0x01600000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            HeatOn	= 1^HeatOn;
            return 0;
        }
        break;

    case 0x01800000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            TempCompensateFlag	= 1^TempCompensateFlag;
            return 0;
        }
        break;
        //ά������
    case 0x02510000:
    case 0x02610000:
        return 0;
    case 0x02710000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            MoniTongDao = LoadSensorDat(MoniTongDao,PARAM_MONITONGDAO,1);
            return 0;
        }
        break;

    case 0x02720000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            MoniZhi = LoadSensorDat(MoniZhi,PARAM_MONIZHI,1);
            return 0;
        }
        break;
    case 0x02810000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            ShuzhiTongDao = LoadSensorDat(ShuzhiTongDao,PARAM_MONITONGDAO,1);
            return 0;
        }
        break;

    case 0x02820000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            ShuzhiZhi = LoadSensorDat(ShuzhiZhi,PARAM_MONIZHI,1);
            return 0;
        }
        break;

        //У׼����
    case 0x03100000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            BiaoPian = LoadSensorDat(BiaoPian,PARAM_BIAOPIAN_JIAOZHUN,1);
            return 0;
        }
        break;

    case 0x03200000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            CaliSetT1 = LoadSensorDat(CaliSetT1,PARAM_T1_JIAOZHUN,1);
            return 0;
        }
        break;

    case 0x03300000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            CaliSetT3 = LoadSensorDat(CaliSetT3,PARAM_T3_JIAOZHUN,1);
            return 0;
        }
        break;

    case 0x03400000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            CaliSetT4 = LoadSensorDat(CaliSetT4,PARAM_T3_JIAOZHUN,1);
            return 0;
        }
        break;

    case 0x03500000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            CaliSetRH = LoadSensorDat(CaliSetRH,PARAM_RH_JIAOZHUN,1);
            return 0;
        }
        break;

    case 0x03600000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            CaliSetRH2 = LoadSensorDat(CaliSetRH2,PARAM_RH_JIAOZHUN,1);
            return 0;
        }
        break;
    case 0x03700000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            CaliSetP = LoadSensorDat(CaliSetP,PARAM_QIYA_JIAOZHUN,1);
            return 0;
        }
        break;
    case 0x03800000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            TestTimes = LoadSensorDat(TestTimes,PARAM_TESTTIMES,1);
            if(TestTimes<1)
            {
                TestTimes = 1;
            }
            if(TestTimes>10)
            {
                TestTimes = 10;
            }
            return 0;
        }
        break;
    case 0x03900000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            CaliSetF1 = LoadSensorDat(CaliSetF1,PARAM_LIULIANG_JIAOZHUN,1);
            return 0;
        }
        break;
        /*���ݴ�������*/
    case 0x04110000:
        if(recordPoint>6)
        {
            if(0 == recordShowBlg) //������
            {
                area[0] = 0;
                area[1] = 25;
                area[2] = 239;
                area[3] = 159;
                CleanVideo(area);/*��������*/

                if(recordShowFlg >= recordPoint - 1)//�����������ˣ������Ͼ����ʼ��������
                {
                    recordShowFlg = 0;
                    ShowRecord2(recordShowFlg,1);
                    CReverseVideo(MenuFormat.ChArea[0]);
                    recordShowBlg = 0;
                }
                else
                {
                    recordShowFlg ++;
                    ShowRecord2(recordShowFlg,1);
                    CReverseVideo(MenuFormat.ChArea[0]);
                    recordShowBlg = 0;
                }

            }
            else
            {
                CReverseVideo(MenuFormat.ChArea[recordShowBlg]);
                recordShowBlg -- ;
                CReverseVideo(MenuFormat.ChArea[recordShowBlg]);
                recordShowFlg ++;
            }
        }
        else
        {
            if((recordPoint-1)<recordShowFlg)
            {
                break;
            }
            if(recordPoint == recordShowFlg+1)
            {
                CReverseVideo(MenuFormat.ChArea[0]);
                recordShowFlg = 0;
                CReverseVideo(MenuFormat.ChArea[recordPoint-1-recordShowFlg]);
            }
            else
            {
                CReverseVideo(MenuFormat.ChArea[recordPoint-1-recordShowFlg]);
                recordShowFlg ++;
                CReverseVideo(MenuFormat.ChArea[recordPoint-1-recordShowFlg]);
            }
        }
        DarwScrollBar(recordPoint-recordShowFlg,recordPoint);
        return 0;
    case 0x04111000:
        if(recordShowFlg >= recordPoint - 1)
        {
            recordShowFlg = 0;
        }
        else
        {
            recordShowFlg ++;
        }
        ShowRecordDetail(recordShowFlg);
        return 0;
    case 0x04210000:
        return 0;
    case 0x04300000:
        if(keyEnterFlg)
            return 0;
        break;

        /*��������*/
    case 0x05110000:
        /*
        û��ȷ��  ����ʱ����ֵ���Ӽ����� ֻ��ѡ��Ч���ı�
        */
        SetUpTime();
        return 0;
        /*��������*/
    case 0x05200000:
        /*������ ����ʾ*/
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            keyBuzzerBitFlg1 = keyBuzzerBitFlg;
            if(0 == keyBuzzerBitFlg1)
            {
                keyBuzzerBitFlg1 = 1;
            }
            /*������ ����ʾ*/
            else
            {
                keyBuzzerBitFlg1 = 0;
            }
            keyBuzzerBitFlg = keyBuzzerBitFlg1;
            return 0;
        }
        break;
        /*����*/
    case 0x05500000:
        /*Ӣ�� ����ʾ*/
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            language1 = language;
            if(0 == language1)
            {
                language1 = 1;
            }
            /*���� ����ʾ*/
            else
            {
                language1 = 0;
            }
            language = language1;
            return 0;
        }
        break;
        /*�Աȶ�����*/
    case 0x05600000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            LCDContrast += 1;
            if(LCDContrast>4)
            {
                LCDContrast = 1;
            }
            /*�ֳ��ı䣬�����û��жϺû�*/
            SetVLCD(LCDContrast);
            return 0;
        }
        break;
    case 0x05c00000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            SensorSel+= 1;
            if(SensorSel>1)
            {
                SensorSel = 0;
            }
            return 0;
        }
        break;

        /*�ָ�����*/
    case 0x05810000:
        return 0;

    case 0x05300000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            MotorTime = LoadSensorDat(MotorTime,PARAM_MOTO,1);
            return 0;
        }
        break;
        //�ݴ�����
    case 0x05a10000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            errtimemax0 += 1;
            if(errtimemax0>9)
                errtimemax0 = 9;
            return 0;
        }
        break;
        //��;
    case 0x05a20000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            UseFor0 += 1;
            if(UseFor0>2)
                UseFor0 = 0;
            return 0;
        }
        break;
        //Bֵ
    case 0x05a30000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            Parg_b0 = (char)(int32)LoadSensorDat(Parg_b0,PARAM_B,1);
            return 0;
        }
        break;
        //kֵ
    case 0x05a40000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            Parg_k0 = LoadSensorDat(Parg_k0,PARAM_K,1);
            return 0;
        }
        break;

    case 0x05a51000:
        SetUpIp();
        break;

    case 0x05a60000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            ComminiStyle0	= 1^ComminiStyle0;
            return 0;
        }
        break;

    case 0x05b10000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            GS_Cycle = LoadSensorDat(GS_Cycle,PARAM_GUANGSAN_CYCLE,1);
            return 0;
        }
        break;
    case 0x05b20000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            PM25_GS_k= LoadSensorDat(PM25_GS_k,PARAM_GUANGSAN_PM25_K,1);
            //д��PM25����ϵ��
            //���ܸ���ģ���ڲ�ϵ�� ��Ϊ�������ĺʹ洢ϵ��
            //OSWriteSingleReg(1,0x9c62,PM25_GS_k);
            return 0;
        }
        break;
    case 0x05b30000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            PM10_GS_k= LoadSensorDat(PM10_GS_k,PARAM_GUANGSAN_PM10_K,1);
            //OSWriteSingleReg(1,0x9c64,PM10_GS_k);
            return 0;
        }
        break;
    case 0x05b40000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            TSP_GS_k= LoadSensorDat(TSP_GS_k,PARAM_GUANGSAN_TSP_K,1);
            //OSWriteSingleReg(1,0x9c63,TSP_GS_k);
            return 0;
        }
        break;
        /*����*/
    case 0x05400000:
        /*������ ����ʾ*/
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            KeyLockFlg1=KeyLockFlg;
            if(0 == KeyLockFlg1)
            {
                KeyLockFlg1 = 1;
            }
            /*�ر��� ����ʾ*/
            else
            {
                KeyLockFlg1 = 0;
            }

            KeyLockFlg=KeyLockFlg1;
            return 0;
        }
        break;
    default:
        break;
    }
    button=0;

    /*�ϼ����º�  ID �仯 ���� ȷ����ʾ ������ֵ��*/
    UpKeyAutoPorc(MenuId);
    //DisConfigState();
    return 0;
}

/*********************************************************************************************************
* Function name:   PorcDownKey
* Descriptions:    �� �� ����
* input parameters: Keyvol  û���õ�
* output parameters:��
* Returned value:  0
*********************************************************************************************************/
uint32 PorcDownKey(uint32 KeyVol)
{
    MenuOffset = (((MenuId&0x00f00000)>>20) - ((MenuId&0x000000f0)>>4));	//�б��˵����22
    if((button==1)&&(StopFlg == 0))
    {
        return 0;
    }

    switch(MenuId&0xffffff0f)
    {
    case 0:	/*������*/
        break;
    case 0x01100000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            RunStyleFlg	= 1^RunStyleFlg;
            return 0;
        }
        break;
    case 0x01200000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            Humidity = LoadSensorDat(Humidity,PARAM_SHIDUMENFA,0);
            return 0;
        }
        break;
    case 0x01300000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            MaxHeatTemp = LoadSensorDat(MaxHeatTemp,PARAM_MAXHEAT,0);
            return 0;
        }
        break;
    case 0x01400000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            MinHeatPower = LoadSensorDat(MinHeatPower,PARAM_MINHEATPOWER,0);
            return 0;
        }
        break;
    case 0x01500000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            FlowOfSamp = LoadSensorDat(FlowOfSamp,PARAM_LIULIANG,0);
            return 0;
        }
        break;
    case 0x01720000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            CycleOfSamp = LoadSensorDat(CycleOfSamp,PARAM_CYCLEOFSAMP,0);
            return 0;
        }
        break;

    case 0x01730000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            Mass = LoadSensorDat(Mass,PARAM_MASS,0);
            return 0;
        }
        break;
    case 0x01710000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            TimeType	= 1^TimeType;
            return 0;
        }
        break;
    case 0x01600000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            HeatOn	= 1^HeatOn;
            return 0;
        }
        break;
    case 0x01800000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            TempCompensateFlag	= 1^TempCompensateFlag;
            return 0;
        }
        break;
        //ά������
    case 0x02510000:
    case 0x02610000:
        return 0;
    case 0x02710000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            MoniTongDao = LoadSensorDat(MoniTongDao,PARAM_MONITONGDAO,0);
            return 0;
        }
        break;
    case 0x02720000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            MoniZhi = LoadSensorDat(MoniZhi,PARAM_MONIZHI,0);
            return 0;
        }
        break;
    case 0x02810000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            ShuzhiTongDao = LoadSensorDat(ShuzhiTongDao,PARAM_MONITONGDAO,0);
            return 0;
        }
        break;
    case 0x02820000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            ShuzhiZhi = LoadSensorDat(ShuzhiZhi,PARAM_MONIZHI,0);
            return 0;
        }
        break;
        //У׼����
    case 0x03100000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            BiaoPian = LoadSensorDat(BiaoPian,PARAM_BIAOPIAN_JIAOZHUN,0);
            return 0;
        }
        break;
    case 0x03200000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            CaliSetT1 = LoadSensorDat(CaliSetT1,PARAM_T1_JIAOZHUN,0);
            return 0;
        }
        break;
    case 0x03300000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            CaliSetT3 = LoadSensorDat(CaliSetT3,PARAM_T3_JIAOZHUN,0);
            return 0;
        }
        break;

    case 0x03400000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            CaliSetT4 = LoadSensorDat(CaliSetT4,PARAM_T3_JIAOZHUN,0);
            return 0;
        }
        break;
    case 0x03500000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            CaliSetRH = LoadSensorDat(CaliSetRH,PARAM_RH_JIAOZHUN,0);
            return 0;
        }
        break;
    case 0x03600000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            CaliSetRH2 = LoadSensorDat(CaliSetRH2,PARAM_RH_JIAOZHUN,0);
            return 0;
        }
        break;
    case 0x03700000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            CaliSetP = LoadSensorDat(CaliSetP,PARAM_QIYA_JIAOZHUN,0);
            return 0;
        }
        break;
    case 0x03800000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            TestTimes = LoadSensorDat(TestTimes,PARAM_TESTTIMES,0);
            if(TestTimes <1)
            {
                TestTimes = 1;
            }
            if(TestTimes >10)
            {
                TestTimes = 10;
            }
            return 0;
        }
        break;
    case 0x03900000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            CaliSetF1 = LoadSensorDat(CaliSetF1,PARAM_LIULIANG_JIAOZHUN,0);
            return 0;
        }
        break;
        /*s���ݲ���*/
    case 0x04110000:
        if(recordPoint>6)
        {
            if(5 == recordShowBlg)// ������
            {
                area[0] = 0;
                area[1] = 25;
                area[2] = 239;
                area[3] = 159;
                CleanVideo(area);
                if(0 == recordShowFlg)//�����ʼ
                {
                    recordShowFlg = recordPoint-1;
                    ShowRecord2(recordShowFlg,2);
                    CReverseVideo(MenuFormat.ChArea[5]);
                    recordShowBlg = 5;
                }
                else
                {
                    recordShowFlg --;
                    if(recordShowFlg<0)
                    {
                        recordShowFlg = recordPoint-1;
                    }
                    ShowRecord2(recordShowFlg+5,2);
                    CReverseVideo(MenuFormat.ChArea[5]);
                    recordShowBlg = 5;
                }
            }
            else
            {
                recordShowFlg --;
                if(recordShowFlg<0)
                {
                    recordShowFlg = recordPoint-1;
                }
                CReverseVideo(MenuFormat.ChArea[recordShowBlg]);
                recordShowBlg ++ ;
                CReverseVideo(MenuFormat.ChArea[recordShowBlg]);
            }
        }
        else
        {
            if((recordPoint-1)<recordShowFlg)
            {
                break;
            }
            if(0 == recordShowFlg)
            {
                CReverseVideo(MenuFormat.ChArea[recordPoint-1-recordShowFlg]);
                recordShowFlg = recordPoint-1;
                CReverseVideo(MenuFormat.ChArea[0]);
            }
            else
            {
                CReverseVideo(MenuFormat.ChArea[recordPoint-1-recordShowFlg]);
                recordShowFlg --;
                if(recordShowFlg<0)
                {
                    recordShowFlg = recordPoint-1;
                }
                CReverseVideo(MenuFormat.ChArea[recordPoint-1-recordShowFlg]);
            }
        }
        DarwScrollBar(recordPoint-recordShowFlg,recordPoint);
        return 0;

    case 0x04111000:
        recordShowFlg --;
        if(recordShowFlg<0)
        {
            recordShowFlg = recordPoint-1;
        }
        ShowRecordDetail(recordShowFlg);
        return 0;

        /*��ʷ����ɾ��  �޲��� */
    case 0x04210000:
        return 0;
        /*ʱ�� ����*/
    case 0x04300000:
        if(keyEnterFlg)
        {
            return 0;
        }
        break;
    case 0x05110000:
        /*û���� ȷ�� ������ֻ����ʾλ������*/
        SetDownTime();
        return 0;
        /*����*/
    case 0x05200000:
        /*������ ����ʾ*/
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            keyBuzzerBitFlg1 =keyBuzzerBitFlg;
            if(0 == keyBuzzerBitFlg1)
            {
                keyBuzzerBitFlg1 = 1;
            }
            /*������ ����ʾ*/
            else
            {
                keyBuzzerBitFlg1 = 0;
            }
            keyBuzzerBitFlg=keyBuzzerBitFlg1;
            return 0;
        }
        break;
        /*����*/
    case 0x05500000:
        /*Ӣ�� ����ʾ*/
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            language1 =language;
            if(0 == language1)
            {
                language1 = 1;
            }
            /*���� ����ʾ*/
            else
            {
                language1 = 0;
            }
            language = language1;
            return 0;
        }
        break;

        /*�Աȶ�����*/
    case 0x05600000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            if( LCDContrast ==1)
            {
                LCDContrast = 4;
            } else
            {
                LCDContrast -=1;
            }
            /*�ֳ��ı䣬�����û��жϺû�*/
            SetVLCD(LCDContrast);
            return 0;
        }
        break;
    case 0x05c00000://������ѡ��
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;

            if( SensorSel == 0)
            {
                SensorSel = 1;
            } else
            {
                SensorSel -= 1;
            }
            return 0;
        }
        break;

        /*�ָ�����*/
    case 0x05810000:
        return 0;

        /*/���ô�ӡ*/
    case 0x05300000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            MotorTime = LoadSensorDat(MotorTime,PARAM_MOTO,0);
            return 0;
        }
        break;

    case 0x05320000:
        break;

    case 0x05a10000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            errtimemax0-=1;
            if(errtimemax0<1)
                errtimemax0 = 1;
            return 0;
        }
        break;

        //��;
    case 0x05a20000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            UseFor0-=1;
            if(UseFor0<0)
                UseFor0 = 2;
            return 0;
        }
        break;

        //Bֵ
    case 0x05a30000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            Parg_b0 = LoadSensorDat(Parg_b0,PARAM_B,0);
            return 0;
        }
        break;
        //kֵ
    case 0x05a40000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            Parg_k0 = LoadSensorDat(Parg_k0,PARAM_K,0);
            return 0;
        }
        break;

    case 0x05a51000:
        SetDownIp();
        break;
    case 0x05a60000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            ComminiStyle0	= 1^ComminiStyle0;
            return 0;
        }
        break;
    case 0x05b10000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            GS_Cycle = LoadSensorDat(GS_Cycle,PARAM_GUANGSAN_CYCLE,0);
            return 0;
        }
    case 0x05b20000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            PM25_GS_k= LoadSensorDat(PM25_GS_k,PARAM_GUANGSAN_PM25_K,0);
            //д��PM25����ϵ�� ���ܸ���ģ���ڲ�kֵ ��Ϊ�����ڲ�������洢
            //OSWriteSingleReg(1,0x9c62,PM25_GS_k);
            return 0;
        }
        break;
    case 0x05b30000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            PM10_GS_k= LoadSensorDat(PM10_GS_k,PARAM_GUANGSAN_PM10_K,0);
            //OSWriteSingleReg(1,0x9c64,PM10_GS_k);
            return 0;
        }
        break;
    case 0x05b40000:
        if(keyEnterFlg == 1)
        {
            ConfChange = 1;
            TSP_GS_k= LoadSensorDat(TSP_GS_k,PARAM_GUANGSAN_TSP_K,0);
            //OSWriteSingleReg(1,0x9c63,TSP_GS_k);
            return 0;
        }
        break;
        /*����*/
    case 0x05400000:
        /*������ ����ʾ*/
        if(keyEnterFlg == 1)
        {   ConfChange = 1;
            KeyLockFlg1 = KeyLockFlg;
            if(0 == KeyLockFlg1)
            {
                KeyLockFlg1 = 1;
            }
            /*�ر��� ����ʾ*/
            else
            {
                KeyLockFlg1 = 0;
            }
            KeyLockFlg = KeyLockFlg1;
            return 0;
        }
        break;
    default:
        break;
    }
    button=0;
    DownKeyAutoPorc(MenuId);	  /*�¼� ��MenuID�仯��ģ�黯���ַ���ʾ */
    return 0;
}

/*********************************************************************************************************
* Function name:   	PorcEscKey
* Descriptions:     	���ؼ�
* input parameters:   	keyvol ����
* output parameters:   ��
* Returned value:      0
*********************************************************************************************************/
uint32 PorcEscKey(uint32 KeyVol)
{
    switch(MenuId&0xffffff0f)
    {
    case 0:
        return 0;
    default:
        break;
    }
    /*��������ʱ��������ò����б仯��ô�ʹ���e2prom*/
    if((1 == ConfChange))
    {
        ConfChange = 0;
        switch (MenuId&0x0f000000)
        {
        case 0x01000000:
            C1025_WriteByte(RUNSETADD8,RunStyleFlg);
            C1025_WriteByte(RUNSETADD8+1,HeatOn);
            C1025_WriteByte(RUNSETADD8+2,TimeType);
            C1025_WriteByte(RUNSETADD8+3,TempCompensateFlag);
            WriteFp32(RUNSETADD32,Humidity);
            WriteFp32(RUNSETADD32+4,MaxHeatTemp);
            WriteFp32(RUNSETADD32+8,MinHeatPower);
            WriteFp32(RUNSETADD32+12,FlowOfSamp);
            WriteFp32(RUNSETADD32+16,CycleOfSampSet);
            WriteFp32(RUNSETADD32+24,Mass);

            break;
        case 0x05000000:
            C1025_WriteByte(SENSOR_SEL_8ADDR,SensorSel);
            C1025_WriteByte(ConInf,FilterRemain);
            C1025_WriteByte(ConInf+2,keyBuzzerBitFlg);
            C1025_WriteByte(ConInf+3,KeyLockFlg);
            C1025_WriteByte(ConInf+4,language);
            WriteFp32(RUNSETADD32+20,MotorTime);
            C1025_WriteByte(ConInf+5,UseFor);
            C1025_WriteByte(RESERVED_8,Parg_b);
            WriteFp32(RESERVED_32,Parg_k);
            C1025_WriteByte(RESERVED_8+1,ComminiStyle);

            WriteFp32(PM25_GS_32ADDR,PM25_GS_k);
            WriteFp32(PM10_GS_32ADDR,PM10_GS_k);
            WriteFp32(TSP_GS_32ADDR,TSP_GS_k);
            break;
        default:
            break;
        }
    }
    if((MenuId&0x0ffff000) == 0x04111000)
    {
        EscKeyAutoPorc(MenuId);	 /*���ؼ�����*/
    }
    EscKeyAutoPorc(MenuId);	 /*���ؼ�����*/
    keyEnterFlg = 0;
    CaliFlg = 0;
    CheckCount =0 ;
    keyFlg = 0;
    return 0;
}

void ReadCaliParTime(uint16 add,TimeDat *CaliParTime)
{
    CaliParTime->year=ReadData16(add);
    CaliParTime->month=C1025_ReadByte(add+2);
    CaliParTime->date=C1025_ReadByte(add+3);
}

void WriteCaliParTime(uint16 add,TimeDat *CaliParTime)
{
    WriteData16(add,CaliParTime->year);
    C1025_WriteByte(add+2,CaliParTime->month);
    C1025_WriteByte(add+3,CaliParTime->date);
}


void check()
{
    if(Humidity>100)
    {
        Humidity = 100;
    }
    if(Humidity<20)
        Humidity = 20;
    if((MaxHeatTemp>100))
    {
        MaxHeatTemp = 100;
    }
    if(MaxHeatTemp<25)
    {
        MaxHeatTemp = 25;
    }
    if((MinHeatPower>90))
    {
        MinHeatPower = 90;
    }
    if((MinHeatPower<-90)) MinHeatPower= -90;
    if((FlowOfSamp>1200))
    {
        FlowOfSamp = 1200;
    }
    if(FlowOfSamp<0)FlowOfSamp = 0;
    if((CycleOfSamp>900))
    {
        CycleOfSamp = 900;
    }
    if(CycleOfSamp<0) {
        CycleOfSamp = 0;
    };
    if(HighV>1500)
    {
        HighV = 1500;
    }
    if(HighV<1)	HighV=1;
//	if((BiaoPian>100)||(BiaoPian<0))
//	{
//		BiaoPian = 100;
//	}
//
//
//	if((WenDuT1>100)||(WenDuT1<0))
//	{
//		WenDuT1 = 100;
//	}
//	if((Pump>100)||(Pump<0))
//	{
//		Pump = 100;
//	}
//
//	if((WenDuT2>100)||(WenDuT2<0))
//	{
//		WenDuT2 = 100;
//	}
//	if((ShiDu>100)||(ShiDu<0))
//	{
//		ShiDu = 100;
//	}
//	if((QiYa>100)||(QiYa<0))
//	{
//		QiYa = 100;
//	}
//	if((LiuLiang_GK>100)||(LiuLiang_GK<0))
//	{
//		LiuLiang_GK = 100;
//	}
//	if((LiuLiang_BK>100)||(LiuLiang_BK<0))
//	{
//		LiuLiang_BK = 100;
//	}
}

void SetPro()
{
    uint8 err;
    float LoFlowBSet=0,LoFlowB=0;
    LoFlowBSet = LoFlowBSet;
    LoFlowB = LoFlowB;
//ʹ��ǰ���ж�
    check();
    switch(MenuId&0xffffff0f)
    {
    case 0x05500000:
        keyEnterFlg = 0;
        NePrOffset = (language)?(71):(0); 	// Ӣ��	����
        C1025_WriteByte(ConInf+4,language);
        break;
    case 0x05400000:
        keyEnterFlg = 0;
        break;

    case 0x01100000:

        //��־��¼
        goSysRecor.recordTime = TimeDate;
        goSysRecor.Event = EVENTRUNPARM;
        goSysRecor.ContentH =0;
        goSysRecor.ContentL = (YXMS);
        FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
        WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
        SysrecordPoint +=1;
        SysrecordReadFlg++;
        WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
        //��־end
        break;

    case 0x01200000:
        //��־��¼
        goSysRecor.recordTime = TimeDate;
        goSysRecor.Event = EVENTRUNPARM;
        goSysRecor.ContentH =0;
        goSysRecor.ContentL = (SDMF);
        FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
        WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
        SysrecordPoint +=1;
        SysrecordReadFlg++;
        WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
        //��־end
        break;

    case 0x01300000:
        //��־��¼
        goSysRecor.recordTime = TimeDate;
        goSysRecor.Event = EVENTRUNPARM;
        goSysRecor.ContentH =0;
        goSysRecor.ContentL = (ZDJRWD);
        FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
        WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
        SysrecordPoint +=1;
        SysrecordReadFlg++;
        WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
        //��־end

        break;

    case 0x01500000:
        FlowOfSamp_B =  CaliParF1*(QiYa*FlowOfSamp * (273.15) )/((273.15+TDAIDIN)*101.325);
        OSMutexPend(OSUart0UsedMutex,0,&err);
        DAMCSOutPutSetOne(DA_PUMP,(5.0*FlowOfSamp_B/20.0));
        DAMCSOutPutAll();
        OSMutexPost(OSUart0UsedMutex);
        //��־��¼
        goSysRecor.recordTime = TimeDate;
        goSysRecor.Event = EVENTRUNPARM;
        goSysRecor.ContentH =0;
        goSysRecor.ContentL = (CYLL);
        FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
        WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
        SysrecordPoint +=1;
        SysrecordReadFlg++;
        WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
        //��־end

        break;

    case 0x01600000:

        //��־��¼
        goSysRecor.recordTime = TimeDate;
        goSysRecor.Event = EVENTRUNPARM;
        goSysRecor.ContentH =0;
        goSysRecor.ContentL = (DHSSN);
        FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
        WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
        SysrecordPoint +=1;
        SysrecordReadFlg++;
        WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
        //��־end
        break;

    case 0x01720000:
        if(CycleOfSamp<7)
        {
            CycleOfSamp = 7;
        }
        if((CycleOfSamp>900))
        {
            CycleOfSamp = 900;
        }
        CycleOfSampSet = CycleOfSamp;

        //��־��¼
        goSysRecor.recordTime = TimeDate;
        goSysRecor.Event = EVENTRUNPARM;
        goSysRecor.ContentH =0;
        goSysRecor.ContentL = (CYZQ);
        FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
        WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
        SysrecordPoint +=1;
        SysrecordReadFlg++;
        WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
        //��־end
        break;
    case 0x01710000:
        //��־��¼
        goSysRecor.recordTime = TimeDate;
        goSysRecor.Event = EVENTRUNPARM;
        goSysRecor.ContentH =0;
        goSysRecor.ContentL = (ZDMS);
        FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
        WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
        SysrecordPoint +=1;
        SysrecordReadFlg++;
        WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
        //��־end
        break;

        //У׼�������
    case 0x03200000:
        //���غ������ǹ���һ��У׼����
        switch(SensorSel)
        {
        case 1:
            CaliParT1 = CaliSetT1/FindOriginalDat(CGQ_WENDU1_XY) ;
            break;

        default:
            CaliParT1 = CaliSetT1/FindOriginalDat(CGQ_WENDU1_KL) ;
            break;
        }


        if(CaliParT1>0.8&&CaliParT1<1.2)
        {
            SetCaliDat(CGQ_WENDU1_KL,CaliParT1);
            SetCaliDat(CGQ_WENDU1_XY,CaliParT1);
            //	����EEPROM
            WriteFp32(JIAOZHUN_T1,CaliParT1);
            //��ʱ��
            WriteCaliParTime(JIAOZHUN_T1_TIME,&TimeDate);
        }

        //��־��¼
        goSysRecor.recordTime = TimeDate;
        goSysRecor.Event = EVENTCALIBRATION;
        goSysRecor.ContentH =0;
        goSysRecor.ContentL = (T1__);
        FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
        WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
        SysrecordPoint +=1;
        SysrecordReadFlg++;
        WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
        //��־end
        break;
    case 0x03300000:
        CaliParT3 = CaliSetT3/FindOriginalDat(CGQ_WENDU3) ;
        if(CaliParT3>0.8&&CaliParT3<1.2)
        {
            SetCaliDat(CGQ_WENDU3,CaliParT3);
            //	����EEPROM
            WriteFp32(JIAOZHUN_T3,CaliParT3);
            //��ʱ��
            WriteCaliParTime(JIAOZHUN_T3_TIME,&TimeDate);
        }
        //��־��¼
        goSysRecor.recordTime = TimeDate;
        goSysRecor.Event = EVENTCALIBRATION;
        goSysRecor.ContentH =0;
        goSysRecor.ContentL = (T3__);
        FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
        WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
        SysrecordPoint +=1;
        SysrecordReadFlg++;
        WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
        //��־end
        break;
    case 0x03400000:
        CaliParT4 = CaliSetT4/FindOriginalDat(CGQ_TCQ_WENDU) ;
        if(CaliParT4>0.8&&CaliParT4<1.2)
        {
            SetCaliDat(CGQ_TCQ_WENDU,CaliParT4);
            //	����EEPROM
            WriteFp32(JIAOZHUN_T4,CaliParT4);
            //��ʱ��
            WriteCaliParTime(JIAOZHUN_T4_TIME,&TimeDate);
        }
        //��־��¼
        goSysRecor.recordTime = TimeDate;
        goSysRecor.Event = EVENTCALIBRATION;
        goSysRecor.ContentH =0;
        goSysRecor.ContentL = (T4__);
        FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
        WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
        SysrecordPoint +=1;
        SysrecordReadFlg++;
        WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
        //��־end
        break;

    case 0x03500000:

        CaliParRH = CaliSetRH/FindOriginalDat(CGQ_SHIDU) ;
        if(CaliParRH>0.8&&CaliParRH<1.2)
        {
            SetCaliDat(CGQ_SHIDU,CaliParRH) ;
            //	����EEPROM
            WriteFp32(JIAOZHUN_RH,CaliParRH);
            WriteCaliParTime(JIAOZHUN_RH_TIME,&TimeDate);
        }
        //��־��¼
        goSysRecor.recordTime = TimeDate;
        goSysRecor.Event = EVENTCALIBRATION;
        goSysRecor.ContentH =0;
        goSysRecor.ContentL = (RH__);
        FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
        WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
        SysrecordPoint +=1;
        SysrecordReadFlg++;
        WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
        //��־end
        break;

    case 0x03600000:
        switch(SensorSel)
        {
        case 1:
            CaliParRH2 = CaliSetRH2/FindOriginalDat(CGQ_CYK_SHIDU_XY) ;
            break;
        default:
            CaliParRH2 = CaliSetRH2/FindOriginalDat(CGQ_CYK_SHIDU_KL) ;
            break;
        }

        if(CaliParRH2>0.8&&CaliParRH2<1.2)
        {
            SetCaliDat(CGQ_CYK_SHIDU_KL,CaliParRH2) ;
            SetCaliDat(CGQ_CYK_SHIDU_XY,CaliParRH2) ;
            //	����EEPROM
            WriteFp32(JIAOZHUN_RH2,CaliParRH2);
            WriteCaliParTime(JIAOZHUN_RH2_TIME,&TimeDate);
        }
        //��־��¼
        goSysRecor.recordTime = TimeDate;
        goSysRecor.Event = EVENTCALIBRATION;
        goSysRecor.ContentH =0;
        goSysRecor.ContentL = (RH2__);
        FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
        WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
        SysrecordPoint +=1;
        SysrecordReadFlg++;
        WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
        //��־end
        break;

    case 0x03700000:
        CaliParP = CaliSetP/FindOriginalDat(CGQ_QIYA)  ;
        if(CaliParP>0.8&&CaliParP<1.2)
        {
            SetCaliDat(CGQ_QIYA,CaliParP) ;
            //	����EEPROM
            WriteFp32(JIAOZHUN_P,CaliParP);
            WriteCaliParTime(JIAOZHUN_P_TIME,&TimeDate);
        }
        //��־��¼
        goSysRecor.recordTime = TimeDate;
        goSysRecor.Event = EVENTCALIBRATION;
        goSysRecor.ContentH =0;
        goSysRecor.ContentL = (P1__);
        FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
        WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
        SysrecordPoint +=1;
        SysrecordReadFlg++;
        WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
        //��־end
        break;

    case 0x03900000:
        //���õĹ���������������ı����Ӧ������
        CaliParF1 = CaliParF1*(FlowOfSamp/CaliSetF1) ;
        SetCaliDat(CGQ_LIULIANG,(1/CaliParF1));
        //	����EEPROM
        WriteFp32(JIAOZHUN_F1,CaliParF1);
        WriteCaliParTime(JIAOZHUN_F1_TIME,&TimeDate);
        FlowOfSamp_B =  CaliParF1*(QiYa*FlowOfSamp * (273.15) )/((273.15+TDAIDIN)*101.325);
        OSMutexPend(OSUart0UsedMutex,0,&err);
        DAMCSOutPutSetOne(DA_PUMP,(5.0*FlowOfSamp_B/20.0));
        DAMCSOutPutAll();
        OSMutexPost(OSUart0UsedMutex);
        //��־��¼
        goSysRecor.recordTime = TimeDate;
        goSysRecor.Event = EVENTCALIBRATION;
        goSysRecor.ContentH =0;
        goSysRecor.ContentL = (F1__);
        FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
        WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
        SysrecordPoint +=1;
        SysrecordReadFlg++;
        WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
        //��־end
        break;
    case 0x05a10000:
        errtimemax = errtimemax0;
        break;
    case 0x05a20000:
        UseFor = UseFor0;
        break;
    case 0x05a30000:
        Parg_b = Parg_b0;
        break;
    case 0x05a40000:
        Parg_k = Parg_k0;
        break;
    case 0x05a60000:
        ComminiStyle = ComminiStyle0;
        break;
    default:
        break;
    }
}


void WriteSysRecord(uint32 recordP,uint8 record[])
{
    uint8 err;

    recordP = (recordP*10)%0x3000;//
    OSMutexPend(OSFlashUsedMutex,0,&err);
    if( (((recordP)%0x3000)/SEC_SIZE) !=  (((recordP+10)%0x3000)/SEC_SIZE)  )//��һ�齫Ҫռ���µ�һ��������
    {
        //������һ����
        SSTF016B_Erase( (((recordP+10)%0x3000+RECORDSYSLOG)/SEC_SIZE), (((recordP+10)%0x3000+RECORDSYSLOG)/SEC_SIZE));
    }
    recordP = recordP+RECORDSYSLOG;
    SSTF016B_WR( recordP, record, 10);
    OSMutexPost(OSFlashUsedMutex);
}


/*********************************************************************************************************
* Function name:  PorcEnterKey
* Descriptions:   ȷ��������
* input parameters: KEYvol  no use
* output parameters:   ��
* Returned value:   0
*********************************************************************************************************/
uint32 PorcEnterKey(uint32 KeyVol)
{
    uint8 slen,place,err;
    uint8 MenuNum;
    uint16 y,i;

    place=((MenuId&0x000000f0)>>4)-1 ;
    for(i = 2; i<8; i++)
    {
        MenuNum = ((MenuId&0xffffff00)>>i*4)&0x0f;
        if(MenuNum)
        {
            break;
        }
    }
    if(StopFlg==1)
    {
        button=1;
    }

    switch(MenuId&0xffffff0f)
    {
    case 0:
        EnterKeyAutoPorc(MenuId);/*/�� ��  menuid ������ģ�黯��ʾ*/
        return 0;
    case 0x01000000:
    case 0x02000000:
        if(KeyLockFlg == 1)
        {
            area[0] = 0;
            area[1] = 20;
            area[2] = 239;
            area[3] = 159;
            CleanVideo(area);		/*��������*/
            DarwButton(60,70);
            WriteCode1212(65,75,NextPrompt[65+NePrOffset]);
            prompt(0);
            return 0;
        }
        if( button == 0 )
        {
            area[0] = 0;
            area[1] = 20;
            area[2] = 239;
            area[3] = 159;
            CleanVideo(area);		/*��������*/
            DarwButton(60,70);
            WriteCode1212(65,75,NextPrompt[64+NePrOffset]);
            prompt(0);
            button = 1;
            return 0;
        }
        if(button==1)
        {
            StopFlg = 1;
            //ϵͳ��־��¼
            if(SysRecordFlg==0)
            {
                SysRecordFlg = 1;
                goSysRecor.recordTime = TimeDate;
                goSysRecor.Event = EVENTMEASURE;
                goSysRecor.ContentH =0;
                goSysRecor.ContentL = 0;
                FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
                WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
                SysrecordPoint +=1;
                SysrecordReadFlg++;
                WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
            }
            if(!SysRecordWHflg)
            {
                SysRecordWHflg = 1;
                if((MenuId&0xffffff0f)==0x02000000)
                {
                    goSysRecor.recordTime = TimeDate;
                    goSysRecor.Event = EVENTMAINTENANCE;
                    goSysRecor.ContentH =0;
                    goSysRecor.ContentL = 0;
                    FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
                    WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
                    SysrecordPoint +=1;
                    SysrecordReadFlg++;
                    WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
                }
            }

            PumpFlg = 1;
            HeatFlg = 1;
            OSMutexPend(OSUart0UsedMutex,0,&err);
            PumpFlg = 1;
            TTLOutput (TTL_0,PumpFlg,TTL_PUMP);	 //��
            TTLOutput (TTL_0,HeatFlg,TTL_JIAREI);

            OSMutexPost(OSUart0UsedMutex);

            break;
        }
    case 0x03000000:
        if(KeyLockFlg == 1)
        {
            area[0] = 0;
            area[1] = 20;
            area[2] = 239;
            area[3] = 159;
            CleanVideo(area);		/*��������*/
            DarwButton(60,70);
            WriteCode1212(65,75,NextPrompt[65+NePrOffset]);
            prompt(0);
            return 0;
        }
        if( button == 0 )
        {
            area[0] = 0;
            area[1] = 20;
            area[2] = 239;
            area[3] = 159;
            CleanVideo(area);		/*��������*/
            DarwButton(60,70);
            WriteCode1212(65,75,NextPrompt[64+NePrOffset]);
            prompt(0);
            button = 1;
            return 0;
        }
        if(button==1)
        {
            StopFlg = 1;
            if(SysRecordFlg==0)
            {
                SysRecordFlg = 1;
                //ϵͳ��־��¼
                goSysRecor.recordTime = TimeDate;
                goSysRecor.Event = EVENTMEASURE;
                goSysRecor.ContentH =0;//(uint8)((EEORStuation1|0X0400)>>8);
                goSysRecor.ContentL =0;// (uint8)(EEORStuation1|0X0400);
                FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
                WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
                SysrecordPoint +=1;
                SysrecordReadFlg++;
                WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
            }

            CaliSetT1 = WenDuT1;
            CaliSetT3 = WenDuT3;
            CaliSetT4 = TCQWenDuT4;
            CaliSetRH = HumiditySample;
            CaliSetRH2 = CYCShiDu;
            CaliSetP=	QiYa;
            CaliSetF1=	LiuLiang_GK;
            CaliSetF2 = LiuLiang_BK;
            HeatFlg = 1;
            OSMutexPend(OSUart0UsedMutex,0,&err);
            TTLOutput (TTL_0,HeatFlg,TTL_JIAREI);
            OSMutexPost(OSUart0UsedMutex);

            break;
        }
    case 0x02100000:
        if(0==PumpFlg)//���ǹ��ŵľͿ���
        {
            PumpFlg = 1;
            OSMutexPend(OSUart0UsedMutex,0,&err);
            TTLOutput (TTL_0,PumpFlg,TTL_PUMP);	 //��
            OSMutexPost(OSUart0UsedMutex);
        }
        else if(PumpFlg==1  )//	���ǿ��ŵľ͹ر�
        {
            PumpFlg = 0;
            OSMutexPend(OSUart0UsedMutex,0,&err);
            TTLOutput (TTL_0,PumpFlg,TTL_PUMP);	 //��
            if(QiYa<90.0||QiYa>110.0)//��ֹ�����������������á���ѹû��ƽ��������������õ�����ֵ�쳣��
            {
                FlowOfSamp_B_Set =  CaliParF1*(101.3*(FlowOfSamp) * (273.15) )/((273.15+20.0)*101.325);
            }
            else
            {
                FlowOfSamp_B_Set =  CaliParF1*(QiYa*(FlowOfSamp) * (273.15) )/((273.15+TDAIDIN)*101.325);
            }
            DAMCSOutPutSetOne(DA_PUMP,(5.0*FlowOfSamp_B_Set/20.0));
            DAMCSOutPutAll();
            OSMutexPost(OSUart0UsedMutex);
        }

        break;
    case 0x02200000:
//				ClampMotor	= 1^ClampMotor;	  //�ı�ѹֽ���״̬��ʵ�ֲ�����
        //��ʾ����״̬
        y = 26+(2-MenuNum+place)*22;
        if( ( (y ) > 25 )&&( (y ) <137 ))	//����ʾ��Χ��
            DisCList16(120,(y),     NextPrompt[66+NePrOffset]);	//�ĳ�
        LcdUpdata(DisBuf);

        (!ClampMotor)?(songkaiMoto()):(yajingMoto());
        return 0;
    case 0x02300000:  //��ֽ���̿���
        //�ɿ�ѹֽ���
        //����ʾ��ֽ��
        y = 26+(3-MenuNum+place)*22;
        if( ( (y ) > 25 )&&( (y ) <137 ))	//����ʾ��Χ��
            DisCList16(120,(y),     NextPrompt[66+NePrOffset]);	//�ĳ�
        LcdUpdata(DisBuf);

        songkaiMoto();
        //��ֽ
        OSMutexPend(OSUart0UsedMutex,0,&err);
        Rotary(40); //һȦ200����
        MotorCtrl(0x02|DIR_RIGHT,100000);//�ú������ڿ��Ʋ��������������������ֱ����� ����Ĳ����޹ؽ�Ҫ��
        //������ת��������
        OSMutexPost(OSUart0UsedMutex);
        //
        OSTimeDlyHMSM(0, 0, 5, 0);//��ʱ10s������ʱ�䡣
        OSMutexPend(OSUart0UsedMutex,0,&err);
        Rotary(40); //����Ƿ��ֽ
        OSMutexPost(OSUart0UsedMutex);

        DisCList16(120,(y),     "[          ]");	//�ĳ�
        LcdUpdata(DisBuf);
        break;
    case 0x04400000:  //�������ݿ���
        y = 26+(4-MenuNum+place)*22;
        if( ( (y ) > 25 )&&( (y ) <137 ))	//����ʾ��Χ��
            DisCList16(120,(y),NextPrompt[66+NePrOffset]);	//�ĳ�
        LcdUpdata(DisBuf);

        //�������flash������
        SSTF016B_Erase(((RECORDFLASHSTAR)/SEC_SIZE), ((0xb2000)/SEC_SIZE));//������ʷ��¼��ϵͳ��־�Ĳ��ֲ�����

        recordPoint= 0;	 /*��¼������*/
        WriteData32(RECORDPOINT2,recordPoint);

        OSTimeDlyHMSM(0, 0, 1, 0);//��ʱ1s������ʱ�䡣

        if(0 != ReadData32(RECORDPOINT2))
        {
            WriteData32(RECORDPOINT2,recordPoint);
        }
        /*��¼��¼����*/


        DisCList16(120,(y),"[          ]");	//�ĳ�

        break;

    case 0x02400000:
        HeatFlg = 1^HeatFlg;
        OSMutexPend(OSUart0UsedMutex,0,&err);
        TTLOutput (TTL_0,HeatFlg,TTL_JIAREI);
        OSMutexPost(OSUart0UsedMutex);
        break;

    case 0x02800000:
        y = 26+(8-MenuNum+place)*22;
        if( ( (y ) > 25 )&&( (y ) <137 ))	//����ʾ��Χ��
            DisCList16(120,(y),     NextPrompt[66+NePrOffset]);	//�ĳ�
        LcdUpdata(DisBuf);

        if(ClampRadio)
        {
            //��ʱ�ر�  ClampRadio Ϊ1�Ƿ���Դ�ر�
            //����Դ��λ
            OSMutexPend(OSUart0UsedMutex,0,&err);
            TTLOutput (TTL_2,1,TTL_BRAY_Ctrl1);
            TTLOutput (TTL_2,0,TTL_BRAY_Ctrl2);
            OSMutexPost(OSUart0UsedMutex);
            ClampRadio = 0;

            OSTimeDlyHMSM(0, 0, C14MotMoveTime ,0);	//����ʱ��
            OSMutexPend(OSUart0UsedMutex,0,&err);
            TTLOutput (TTL_2,0,TTL_BRAY_Ctrl1);
            TTLOutput (TTL_2,0,TTL_BRAY_Ctrl2);
            OSMutexPost(OSUart0UsedMutex);
        } else
        {
            //����Դ��λ
            OSMutexPend(OSUart0UsedMutex,0,&err);
            TTLOutput (TTL_2,0,TTL_BRAY_Ctrl1);
            TTLOutput (TTL_2,1,TTL_BRAY_Ctrl2);
            OSMutexPost(OSUart0UsedMutex);
            ClampRadio = 1;
            //����Ӧ������Ϊ�������õ�ʱ��
            OSTimeDlyHMSM(0, 0, C14MotMoveTime ,0);	//����ʱ��
            OSMutexPend(OSUart0UsedMutex,0,&err);
            TTLOutput (TTL_2,0,TTL_BRAY_Ctrl1);
            TTLOutput (TTL_2,0,TTL_BRAY_Ctrl2);
            OSMutexPost(OSUart0UsedMutex);
        }
        break;
    case 0x01100000:
    case 0x03100000:
    case 0x01200000:
    case 0x03200000:
    case 0x05200000:
    case 0x01300000:
    case 0x03300000:
    case 0x01400000:
    case 0x03400000:
    case 0x01500000:
    case 0x03500000:
    case 0x03600000:
    case 0x01600000:
    case 0x01800000:

    case 0x05600000:
    case 0x01710000:
    case 0x01720000:
    case 0x01730000:
    case 0x03700000:
    case 0x03800000:
    case 0x03900000:
    case 0x05400000:
    case 0x05500000:
    case 0x05300000:
    case 0x02710000:
    case 0x05a10000:
    case 0x05a20000:
    case 0x05a30000:
    case 0x05a40000:
    case 0x05a60000:
    case 0x05b10000:
    case 0x05b20000:
    case 0x05b30000:
    case 0x05b40000:
    case 0x05c00000:

        keyEnterFlg = 1^keyEnterFlg;
        if(keyEnterFlg == 0)
        {
            SetPro();
            if((MenuId&0xffffff0f) == 0x05500000)
            {
                EscKeyAutoPorc(MenuId);
                return 0;
            }
            if(0x03100000 == (MenuId&0xffffff0f))
            {
                EnterKeyAutoPorc(MenuId);
                Calibration();
                return 0;
            }
            if(0x03800000 == (MenuId&0xffffff0f))
            {
                EnterKeyAutoPorc(MenuId);
                Calibration2();
                return 0;
            }
        }
        CReverseVideo(MenuFormat.ChArea[place]);
        return 0;

    case 0x03a00000://�ָ�Ĭ��У׼����
        EnterKeyAutoPorc(MenuId);
        area[0] = 0;
        area[1] = 20;
        area[2] = 239;
        area[3] = 159;
        CleanVideo(area);		/*��������*/
        DarwButton(60,70);
        WriteCode1212(65,75,NextPrompt[67+NePrOffset]);
        prompt(0);
        return 0;

    case 0x03a10000://Ĭ��У׼
        SetCaliDat(CGQ_WENDU1_KL,1);
        SetCaliDat(CGQ_WENDU1_XY,1);
        WriteFp32(JIAOZHUN_T1,1);

        SetCaliDat(CGQ_WENDU3,1);
        //	����EEPROM
        WriteFp32(JIAOZHUN_T3,1);

        SetCaliDat(CGQ_TCQ_WENDU,1);
        //	����EEPROM
        WriteFp32(JIAOZHUN_T4,1);

        SetCaliDat(CGQ_SHIDU,1) ;
        //	����EEPROM
        WriteFp32(JIAOZHUN_RH,1);

        SetCaliDat(CGQ_CYK_SHIDU_KL,1) ;
        SetCaliDat(CGQ_CYK_SHIDU_XY,1) ;
        //	����EEPROM
        WriteFp32(JIAOZHUN_RH2,1);

        SetCaliDat(CGQ_QIYA,1) ;
        //	����EEPROM
        WriteFp32(JIAOZHUN_P,1);
        SetCaliDat(CGQ_LIULIANG,1.0);
        //	����EEPROM
        WriteFp32(JIAOZHUN_F1,1.0);
        WriteCaliParTime(JIAOZHUN_T1_TIME,&TimeDate);
        WriteCaliParTime(JIAOZHUN_T3_TIME,&TimeDate);
        WriteCaliParTime(JIAOZHUN_T4_TIME,&TimeDate);
        WriteCaliParTime(JIAOZHUN_RH_TIME,&TimeDate);
        WriteCaliParTime(JIAOZHUN_RH2_TIME,&TimeDate);
        WriteCaliParTime(JIAOZHUN_P_TIME,&TimeDate);
        WriteCaliParTime(JIAOZHUN_F1_TIME,&TimeDate);
        WriteCaliParTime(JIAOZHUN_BIAOPIAN_TIME,&TimeDate);

        WriteCaliParTime(JIAOZHUN_T1_TIME,&TimeDate);
        EscKeyAutoPorc(MenuId);

        //ϵͳ��־��¼
        goSysRecor.recordTime = TimeDate;
        goSysRecor.Event = EVENTCALIBRATION;
        goSysRecor.ContentH =0;
        goSysRecor.ContentL = (MRJZ);
        FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
        WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
        SysrecordPoint +=1;
        SysrecordReadFlg++;
        WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
        return 0;

    case 0x02500000:	//̽��������
        EnterKeyAutoPorc(MenuId);/*/�� ��  menuid ������ģ�黯��ʾ*/
        area[0] = 0;
        area[1] = 22;
        area[2] = 239;
        area[3] = 159;
        CleanVideo(area);		/*��������*/
        WriteCode1212(65,75,NextPrompt[63+NePrOffset]);
        return 0;

    case 0x02510000:	//̽��������
        keyEnterFlg = 1^keyEnterFlg;
        if(keyEnterFlg == 1) //��ʼ����
        {
            CountTotal = 0;
            CountTimes = 0;
            CountNow = 0;
            OSMutexPend(OSUart0UsedMutex,0,&err);
            CountCtrl(COUNT_START);
            OSMutexPost(OSUart0UsedMutex);
            CountStartFlg = 1;//��ʼ������
        }
        else
        {
            CountStartFlg = 0;
        }
        return 0;
    case 0x02600000:	//����������
        EnterKeyAutoPorc(MenuId);/*/�� ��  menuid ������ģ�黯��ʾ*/
        SensorMenu();
        return 0;

    case 0x02700000:
        EnterKeyAutoPorc(MenuId);/*/�� ��  menuid ������ģ�黯��ʾ*/
        OutputStation();
        return 0;

    case 0x03110000: //��ƬУ׼
        Calibration();
        if( CaliFlg == 10)
        {
            EscKeyAutoPorc(MenuId);
            CaliFlg = 0;
        }
        return 0;
    case 0x03810000: //��ƬУ׼
        Calibration2();
        if(CaliFlg == 5)    //������ɷ���
        {
            EscKeyAutoPorc(MenuId);    //���ؼ�����
            CaliFlg = 0 ;
            CheckCount =0 ;
        }
        return 0;

        /*���ݴ���	  ��¼�鿴*/
    case 0x04100000:
        EnterKeyAutoPorc(MenuId);
        if(0 == recordPoint)
        {
            slen = StringLen(NextPrompt[32+NePrOffset]);
            DisCList16((DIS_AREA_X-slen*8)/2,60,NextPrompt[32+NePrOffset]);
            DisCList16(99,119,NextPrompt[33+NePrOffset]);
            ReverseVideo(areaMiddle);
            LcdUpdata(DisBuf);
            OSTimeDlyHMSM(0, 0, 1,500);
            EscKeyAutoPorc(MenuId);
        }
        else
        {
            recordShowFlg = recordPoint-1;
            ReadFlashRecordSimple(0,&rRecord[0]);//	��̨��
            for(i=1; i<8; i++)
            {
                ReadFlashRecordSimple(recordShowFlg-i+1,&rRecord[i]);//	��̨��
            }
            rRecordFlg = 1;//��ǰ��ʾ�ĵ�һλ����λ�á�
            rRecordShowFlg = 1;
            OldAction = 0;
            ShowRecord2(recordShowFlg,0);
            recordShowBlg = 0;
            DarwScrollBar(recordShowFlg,recordPoint);
            CReverseVideo(MenuFormat.ChArea[0]);
        }
        return 0;

        /*�ٰ�ȷ���ʹ�ӡ����*/
    case 0x04110000:
        //��ʾ��ϸ��Ϣ
        EnterKeyAutoPorc(MenuId);
        ShowRecordDetail(recordShowFlg);
        return 0;

    case 0x04111000:
        EscKeyAutoPorc(MenuId);
        EscKeyAutoPorc(MenuId);
        return 0;

        /*���ݴ���   ȷ��*/
    case 0x04200000:
        EnterKeyAutoPorc(MenuId);
        area[0] = 0;
        area[1] = 22;
        area[2] = 239;
        area[3] = 159;
        CleanVideo(area);		/*��������*/
        //��EEPROM
        CaliParT1 =  ReadFp32(JIAOZHUN_T1);
        CaliParT3 =	 ReadFp32(JIAOZHUN_T3);
        CaliParT4 =	 ReadFp32(JIAOZHUN_T4);	   //add by yxq
        CaliParRH =	 ReadFp32(JIAOZHUN_RH);
        CaliParRH2=	 ReadFp32(JIAOZHUN_RH2);	//add by yxq
        CaliParP  =	 ReadFp32(JIAOZHUN_P);
        CaliParF1 =	 ReadFp32(JIAOZHUN_F1);
        Parg_u    =  ReadFp32(JIAOZHUN_BIAOPIAN);
        ReadCaliParTime(JIAOZHUN_BIAOPIAN_TIME,&CaliTimeDate);
        DisCList16(6,20,  "u :");
        DisValue(Parg_u,6+55,20,1,5,4,0);
        DisDate(6+125,20,CaliTimeDate,2);

        ReadCaliParTime(JIAOZHUN_T1_TIME,&CaliTimeDate);
        DisCList16(6,20+17,  "T1:");
        DisValue(CaliParT1,6+55,20+17,1,5,4,0);
        DisDate(6+125,20+17,CaliTimeDate,2);

        ReadCaliParTime(JIAOZHUN_T3_TIME,&CaliTimeDate);
        DisCList16(6,20+17*2,  "T3:");
        DisValue(CaliParT3,6+55,20+17*2,1,5,4,0);
        DisDate(6+125,20+17*2,CaliTimeDate,2);

        ReadCaliParTime(JIAOZHUN_T4_TIME,&CaliTimeDate);
        DisCList16(6,20+17*3,  "T4:");
        DisValue(CaliParT4,6+55,20+17*3,1,5,4,0);
        DisDate(6+125,20+17*3,CaliTimeDate,2);

        ReadCaliParTime(JIAOZHUN_P_TIME,&CaliTimeDate);
        DisCList16(6,20+17*4,  "P :");
        DisValue(CaliParP,6+55,20+17*4,1,5,4,0);
        DisDate(6+125,20+17*4,CaliTimeDate,2);

        ReadCaliParTime(JIAOZHUN_F1_TIME,&CaliTimeDate);
        DisCList16(6,20+17*5,  "F1 :");
        DisValue(CaliParF1,6+55,20+17*5,1,5,4,0);
        DisDate(6+125,20+17*5,CaliTimeDate,2);

        ReadCaliParTime(JIAOZHUN_RH_TIME,&CaliTimeDate);
        DisCList16(6,20+17*6,  "RH1:");
        DisValue(CaliParRH,6+55,20+17*6,1,5,4,0);
        DisDate(6+125,20+17*6,CaliTimeDate,2);

        ReadCaliParTime(JIAOZHUN_RH2_TIME,&CaliTimeDate);
        DisCList16(6,20+17*7,  "RH2:");
        DisValue(CaliParRH2,6+55,20+17*7,1,5,4,0);
        DisDate(6+125,20+17*7,CaliTimeDate,2);
        return 0;

        /*�鿴*/
    case 0x04300000:
        keyEnterFlg = 1^keyEnterFlg;
        area[0] = 0;
        area[1] = 22;
        area[2] = 239;
        area[3] = 159;
        CleanVideo(area);		/*��������*/
        ShowSysRecord();
        return 0;

    case 0x04210000:
        return 0;

    case 0x05000000:
        ConfChange = 0;
        break;

        /*���ò���	  ʱ������	 ��ʾ ������ ʱ���� ������ֵ  ��� ȡ��*/
    case 0x05100000:
        EnterKeyAutoPorc(MenuId);
        /*ʱ�����ݸ�ֵ*/
        TimeDateKey.year = TimeDate.year;
        if(TimeDateKey.year>2050)
        {
            TimeDateKey.year = 2016;
        }
        TimeDateKey.month = TimeDate.month;
        TimeDateKey.date = TimeDate.date;
        TimeDateKey.hour = TimeDate.hour;
        TimeDateKey.minute = TimeDate.minute;
        TimeDateKey.second = TimeDate.second;

        DealYMD(&strKey[0],TimeDateKey); /*��������ʾ*/
        DisCList16(71,60,strKey);

        DealHMS(&strKey[0],TimeDateKey); /*ʱ������ʾ*/
        DisCList16(79,80,strKey);

        DisCList16(99,120,NextPrompt[29+NePrOffset]);
        DrawRectangular(areaMiddle);

        /*�괦����*/
        area[0] = 71;
        area[1] = 59;
        area[2] = 118;
        area[3] = 77;
        ReverseVideo(area);
        prompt(5);
        keyFlg = 1;
        keyEnterFlg = 0;
        return 0;
        /*����ֵ���� ����*/
    case 0x05110000:
        SetEnterTime();
        return 0;
        /*����*/
    case 0x05800000:
        EnterKeyAutoPorc(MenuId);
        slen = StringLen(NextPrompt[6+NePrOffset]);
        DisCList16((DIS_AREA_X-slen*8)/2,70,NextPrompt[6+NePrOffset]);
        prompt(0);
        return 0;
    case 0x05810000:
        slen = StringLen(NextPrompt[35+NePrOffset]);
        DisCList16((DIS_AREA_X-slen*8)/2,100,NextPrompt[35+NePrOffset]);
        MachineFristRun(2);//�ָ�����
        GetMenuFormHoldreg();				//�Բ�����Ϣ
        SetMenuList();						//�����������ݸ�ֵ
        LcdUpdata(DisBuf);
        OSTimeDlyHMSM(0, 0, 1,500);
        EscKeyAutoPorc(MenuId);
        BuzzerCtrl(2,5,5);
        return 0;

    case 0x05900000:  /*�汾��Ϣ  ��ȱ*/
        EnterKeyAutoPorc(MenuId);
        //"�� ҳwww.skyray-instrument.com"
        if(language == 1)
        {
            DisEList126(0,22,"   Jiangsu Skyray Instruments Co.,LTD");
            DisEList126(0,52,FI1_EN);
            DisEList126(0,66,FI2_EN);
            DisEList126(0,80,FI3_EN);
            DisEList126(0,116, HW_EN);
            DisCList16(96,143,"Ensure");
        }
        else
        {
            DisCList16(0,22,"   �������������ɷ����޹�˾");
            DisCList16(0,39,"      Skyray Instrument");
            DisCList16(0,66,FI1_CH);
            DisCList16(0,83,FI2_CH);
            DisCList16(0,116,HW_CH);
            DisCList16(104,143,"ȷ��");
        }
        ReverseVideo(areaMiddleEE);
        return 0;

    case 0x05910000:   /*�޲���*/
        EscKeyAutoPorc(MenuId);
        return 0;

    case 0x05a00000:
        UseFor0 = UseFor;
        errtimemax0 = errtimemax;
        Parg_b0 = Parg_b;
        Parg_k0 = Parg_k;
        ComminiStyle0 = ComminiStyle;
        break;

    case 0x05a51000:
        C1025_WriteByte(ConInf+6,ip.first);
        C1025_WriteByte(ConInf+7,ip.second);
        C1025_WriteByte(ConInf+8,ip.third);
        C1025_WriteByte(ConInf+9,ip.end);
        EscKeyAutoPorc(MenuId);
        return 0;

    case 0x05a50000:
        EnterKeyAutoPorc(MenuId);
        area[0] = 0;
        area[1] = 22;
        area[2] = 239;
        area[3] = 159;
        CleanVideo(area);		/*��������*/
        DealIP(&strKey[0],ip); /*ip��ʾ*/
        DisCList16(60,60,strKey);
        /*�괦����*/
        ReverseVideo(IpArea[0]);
        prompt(5);
        keyFlg = 0;
        keyEnterFlg = 0;
        return 0;

    case 0x05700000:	/*������Ϣ*/
        return 0;

    case 0x05320000:
        EscKeyAutoPorc(MenuId);
        return 0;
        /*��������*/
    case 0x06000000:
        EnterKeyAutoPorc(MenuId);
        LcdClr();					 		//����
        ClrDisBuf();
        if(language == 1)
        {
            DisEList126(0,22,"   Jiangsu Skyray Instruments Co.,LTD");
            DisEList126(0,57,"Address:1888,West Zhonghuayuan Rd.,");
            DisEList126(0,69,"        Yushan,Kunshan,Jiangsu Province");
            DisCList16(0,91,  "Hot-line");
            DisCList16(0,113, "Homepage");
            DisCList16(0,135, "E-mail");
        }
        else
        {
            DisCList16(20,22,   "�������������ɷ����޹�˾"    );
            DisCList16(0,69, "��ַ:��ɽ���л�԰��·1888��" );
            DisCList16(0,91, "���ߵ绰");
            DisCList16(0,113,"��˾��ҳ");
            DisCList16(0,135,"��������");
        }
        DisEList126(64,93, ":800-9993-800/400-7102-888"     );
        DisEList126(64,115,":www.skyray-instrument.com"     );
        DisEList126(64,137,":sales@skyray-instrument.com"   );
        return 0;
    case 0x06100000:
        EscKeyAutoPorc(MenuId);
        return 0;
    default:
        break;
    }
    EnterKeyAutoPorc(MenuId);
    return 0;
}


/*********************************************************************************************************
* Function name: KeyScan
* Descriptions:  ��ֵɨ��
* input parameters:     	  ��
* output parameters:    	��
* Returned value:       ��ֵ
*********************************************************************************************************/
uint32 KeyScan(void)
{
    uint32 k;

    k=(GPIO_ReadValue(2)&KEY_FILTER)&KEY_NULL;	 /*��ֵ*/
    return k;
}

/*********************************************************************************************************
* Function name:      PorcKey
* Descriptions:       ��������
* input parameters:     keyctl ����
* output parameters:     ��
* Returned value:        0
*********************************************************************************************************/
uint8 PorcKey(uint8 KeyCtl)
{
    uint8 err,i;
    err = err;
    switch(KeyScan())
    {
    case KEY_UP:
        ReturnMenu0Time=RETURN_MENU0_TIME;
        if((0==KeyUpRun)||((1==KeyUpRepeat)&&(KeyTimer<KEY_REPEAT_TIME)&&(KeyTimer>(KEY_REPEAT_TIME-100))))
        {
            BuzzerCtrl(1,2,2);	   /*����*/
            if(0==KeyTimer)
            {
                KeyTimer=0xffff;
            }
            else
            {
                KeyTimer=KEY_SERIES_TIME;
            }
            PorcUpKey(1);
            DisConfigState();
            LcdUpdata(DisBuf);
            KeyUpRun=1;
        }
        break;
        /*�¼�����*/
    case KEY_DOWN:
        ReturnMenu0Time=RETURN_MENU0_TIME;
        /*���� ���� �����������ٷ�����*/
        if((0==KeyDownRun)||((1==KeyDownRepeat)&&(KeyTimer<KEY_REPEAT_TIME)&&(KeyTimer>(KEY_REPEAT_TIME-100))))
        {
            BuzzerCtrl(1,2,2);
            if(0==KeyTimer)
            {
                KeyTimer=0xffff;
            }
            else
            {
                KeyTimer=KEY_SERIES_TIME;
            }
            PorcDownKey(1);	 /*�¼�����*/
            DisConfigState();
            LcdUpdata(DisBuf);
            KeyDownRun=1;
        }
        break;
        /*���ؼ�*/
    case KEY_ESC:
        ReturnMenu0Time=RETURN_MENU0_TIME;
        if((0==KeyLeftRun)||((1==KeyLeftRepeat)&&(KeyTimer<KEY_REPEAT_TIME)&&(KeyTimer>(KEY_REPEAT_TIME-100))))
        {
            BuzzerCtrl(1,2,2);
            if(0==KeyTimer)
            {
                KeyTimer=0xffff;
            }
            else
            {
                KeyTimer=KEY_SERIES_TIME;
            }
            PorcEscKey(1); /*���ؼ�����*/
            DisConfigState();
            LcdUpdata(DisBuf);
            KeyLeftRun=1;
        }
        break;

        /*ȷ��*/
    case KEY_ENTER://5e18:            //Enter
        ReturnMenu0Time=RETURN_MENU0_TIME;
        if(0==KeyEnterRun)
        {
            BuzzerCtrl(1,2,2);
            PorcEnterKey(1);
            OffsetFlg=0;
            DisConfigState();
            LcdUpdata(DisBuf);
            KeyEnterRun=1;
        }
        break;

    case KEY_RIGHT://�����Ҽ�
        ReturnMenu0Time=RETURN_MENU0_TIME;
        /*���� ���� �����������ٷ�����  �ù���δʹ��*/
        if((0==KeyUpRun)||((1==KeyUpRepeat)&&(KeyTimer<KEY_REPEAT_TIME)&&(KeyTimer>(KEY_REPEAT_TIME-100))))
        {
            BuzzerCtrl(1,2,2);	   /*����*/
            if(0==KeyTimer)
            {
                KeyTimer=0xffff;
            }
            else
            {
                KeyTimer=KEY_SERIES_TIME;
            }
            OffsetFlg++;
            if(OffsetFlg>DatLength)
            {
                OffsetFlg=0;
            }
            DisConfigState();
            if((MenuId&0x0ff00000) == 0x05700000)
            {
                //��λ��ֽ
                FilterRemain = 100;
                PaperTime = 0;

                C1025_WriteByte(ConInf,(uint8)(FilterRemain)); //��ӡ����

            }
            if((MenuId&0x0ffff000) == 0x05a51000)
            {
                SetRightIp();
            }
            LcdUpdata(DisBuf);
        }
        break;
    case KEY_START:  //Start��
        BuzzerCtrl(1,2,2);
        //������λ������
        if(StopFlg == 0)
        {
            return 0;
        }
        StopFlg = 0;

        for(i= 0; i<4; i++)
        {
            PorcEscKey(1); /*���ؼ�����*/
        }
        LcdUpdata(DisBuf);
        showFlg = 1;
        //ϵͳ��־����
        SysRecordFlg = 0;
        SysRecordWHflg= 0;
        goSysRecor.recordTime = TimeDate;
        goSysRecor.Event = EVENTMEASURE;
        goSysRecor.ContentH =0;//(uint8)((EEORStuation1|0X0800)>>8);
        goSysRecor.ContentL = 1;//(uint8)(EEORStuation1|0X0800);
        FlashSysDatChange(&DatForSysRecord[0],goSysRecor);
        WriteSysRecord(SysrecordPoint,&DatForSysRecord[0]);
        SysrecordPoint +=1;
        SysrecordReadFlg++;
        WriteData32(SYSRECORDPOINT2,SysrecordPoint);/*��¼��¼����*/
        yajingMoto();            //ѹ����ֽ��Ϊ��һ�θ�����׼��
        MeasureStepFlg = 0;
        break;

    case KEY_NULL:              /*//�ͷ�ȫ������   */
        if(TimeOverFlg)
        {
            PorcEnterKey(1);
            TimeOverFlg = 0;
            CountReadTime = 180;
        }
        KeyUpRun=0;
        KeyDownRun=0;
        KeyLeftRun=0;
        KeyRightRun=0;
        KeyMenuRun=0;
        KeyEnterRun=0;
        KeyDRRun=0;
        KeyTimer=0;
        break;
    default:
        break;
    }
    return 0;
}