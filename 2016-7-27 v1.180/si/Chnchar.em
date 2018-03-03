/****************************************************************************
 *  Ver:    1.13
 *  Date:   2002.9.18
 *  Author: suqiyuan
 * ================================
 * �����м����������������֧�ֺ���:
 * ʹ����Щ�����ض�Ӧ�ļ��Ϳ�����
 *
 * ���ع�ϵ����:
 * EM_delete:            DELETE
 * EM_backspace:         BACKSPACE
 * EM_CursorUp:          �����Ϸ������
 * EM_CursorDown:        �����·������
 * EM_CursorLeft:        �����������
 * EM_CursorRight:       �����ҷ������
 * EM_SelectWordLeft:    Shift + ��
 * EM_SelectWordRight:   Shift + ��
 * EM_SelectLineUp:      Shift + ��
 * EM_SelectLineUp:      Shift + ��
 ****************************************************************************/
 
 //For keyboard delete
 Macro EM_delete()
 {
    //get current character
    hWnd = GetCurrentWnd()
    if(hWnd == 0)
        stop
    ln      = GetWndSelLnFirst(hWnd)
    lnLast  = GetWndSelLnLast(hWnd)
    lnCnt   = lnLast - ln + 1
    sel     = GetWndSel(hWnd)
    ich     = GetWndSelIchFirst(hWnd)
    ichLast = GetWndSelIchLim(hWnd)
    hBuf    = GetWndBuf(hWnd)
    curLine = GetBufLine(hBuf,ln)

    //Msg("Now Select lines:@lnCnt@,Line @ln@ index @ich@ to line @lnLast@ index @ichLast@")
    if((lnCnt > 1) || ((lnCnt==1)&&(ichLast>ich)))//ѡ����ǿ�
    {
        //Msg("Selection is One BLOCK.")
        curLine = GetBufLine(hBuf,ln)
        if(ich>0)
        {
            index = 0
            while(index < ich)
            {
                ch = curLine[index]
                if(SearchCharInTab(ch))
                    index = index + 1
                else
                    index = index + 2
            }
            //��������ں����м䣬������ǰ����һ���ֽ�
            sel.ichFirst = ich - (index-ich)
        }
        curLine = GetBufLine(hBuf,lnLast)
        len     = GetBufLineLength(hBuf,lnLast)
        index   = 0
        while(index < ichLast && index < len)
        {
            ch = curLine[index]
            if(SearchCharInTab(ch))
                index = index + 1
            else
                index = index + 2
        }
        sel.ichLim = index
        if(ichLast>len)
            sel.ichLim = ichLast
        SetWndSel(hWnd,sel)
        //Msg("See the block selected is adjusted now.")
        Delete_Character
    }
    else//ѡ��Ĳ��ǿ�
    {
        //Msg("Selection NOT block.")
        curChar = curLine[ich]
        //�������ĩ,Ӧ���ܹ�ʹ����һ��������β
        if(ich == strlen(curLine))
        {
            Delete_Character
            stop
        }
        //Msg("Not at the end of line.")
        flag    = SearchCharInTab(curChar)
        //Msg("Current char:@curChar@,Valid flag:@flag@")
        if(flag)
        {
            //Msg("Byte location to delete:@ich@,Current char:@curChar@")
            DelCharOfLine(hWnd,ln,ich,1)
        }
        else
        {
            /*�����ʵ�ַ�����������:�����׿�ʼ��,�����Table�е�,��һ����
             *�������,�Ӷ�����,һֱ����ǰ�ַ�,������ôɾ��
             *�����������ļٶ�,��ǰ��û�а�����ֵ�����
             */
            index = 0
            word  = 0
            byte  = 0
            len   = strlen(curLine)
            while(index < ich)
            {
                ch   = curLine[index]
                flag = SearchCharInTab(ch)
                if(flag)
                {
                    index = index + 1
                    byte  = byte + 1
                }
                else
                {
                    index = index + 2
                    word  = word + 1
                }
            }
            //index = ich + 1,current cursor is in the middle of word
            //                or in the front of byte
            //index = ich,current cursor is NOT in the front of word
            nich = 2*(word-(index-ich)) + byte
            //Msg("Start deleting position:@ich@,word:@word@,byte:@byte@")
            DelCharOfLine(hWnd,ln,nich,2)
            if((index-ich) && !flag && (ich != len-1))//����һ������ĩβ�ĺ����м�
                Cursor_Left
        }
    }
}

//For keyboard backspace <-
Macro EM_backspace()
{
    //get current character
    hWnd = GetCurrentWnd()
    if(hWnd == 0)
        stop
    sel     = GetWndSel(hWnd)
    ln      = sel.lnFirst
    ich     = sel.ichFirst
    if(ich < 0)
        stop
    lnLast  = GetWndSelLnLast(hWnd)
    lnCnt   = lnLast - ln + 1
    ichLast = GetWndSelIchLim(hWnd)

    //Msg("Now Select lines:@lnCnt@,Line @ln@ index @ich@ to line @lnLast@ index @ichLast@")
    if((lnCnt > 1) || ((lnCnt==1)&&(ichLast>ich)))//ѡ����ǿ�,ֱ��ɾ��������Ŀ�
    {
        EM_delete
    }
    else
        {if(ich == 0)
        {
            Backspace
            stop
        }
        hBuf    = GetWndBuf(hWnd)
        curLine = GetBufLine(hBuf,ln)

        index = 0
        flag  = 0  // 1-byte,0-word
        byte = 0
        word = 0
        while(index < ich)
        {
            ch   = curLine[index]
            flag = SearchCharInTab(ch)
            if(flag)
                {
                    byte  = byte + 1
                    index = index + 1
                }
            else
                {
                    word  = word + 1
                    index = index + 2
                }
        }
        if(flag)//char before cursor is in table
        {
            //Msg("char before cursor is in table,byte!")
            Backspace
        }
        else if(!flag && (index-ich))//current cursor is in the middle of word
        {
            //Msg("current cursor is in the middle of word.")
            DelCharOfLine(hWnd,ln,ich-1,2)
            if(!(sel.ichFirst == strlen(curLine)-1))
                Cursor_Left
        }
        else if(!flag && !(index-ich))//Current cursor is after a word
        {
            //Msg("Current cursor is after a word.")
            DelCharOfLine(hWnd,ln,ich-2,2)
            if(sel.ichFirst != strlen(curLine))
            {
                Cursor_Left
                Cursor_Left
            }
        }
    }
}

Macro SearchCharInTab(curChar)
{
     /* Total 97 chars */
    AsciiChar = AsciiFromChar(curChar)
    //Msg("Current char in SearchCharInTab():@curChar@.")
    if(AsciiChar >= 32 && AsciiChar <= 126)
        return 1
    //Msg("Current Char(@curChar@) NOT between space and ~")
    if(AsciiChar == 9)//Tab
        return 1
    //Msg("Current Char(@curChar@) NOT Tab")
    if(AsciiChar == 13)//CR
        return 1
    //Msg("Current Char(@curChar@) Not CR")
    return 0
}

Macro DelCharOfLine(hWnd,ln,ich,count)
{
    if(hWnd == 0)
        stop
    sel     = GetWndSel(hWnd)
    hBuf    = GetWndBuf(hWnd)
    if(hBuf == 0)
        stop
    if(ln > GetBufLineCount(hBuf))
        stop
    szLine = GetBufLine(hBuf,ln)
    len    = strlen(szLine)
    if(ich >  len)
        stop

    NewLine = ""
    if(ich > 0)
    {
        NewLine = NewLine # strmid(szLine,0,ich)
    }
    if(ich+count < len)
    {
        ichLast = len
        NewLine = NewLine # strmid(szLine,ich+count,ichLast)
    }
    /**/
    //Msg("Current line:@szLine@")
    //Msg("Replaced as:@NewLine@")
    /**/
    PutBufLine(hBuf,ln,NewLine)
    SetWndSel(hWnd, sel)
}


//���ƹ��
macro EM_CursorUp()
{
    hWnd = GetCurrentWnd()
    if(hWnd == 0)
        stop

    hbuf = GetCurrentBuf()

    //�ƶ����
    Cursor_Up

    //����ƶ�����Ĺ��λ��
    hwnd = GetWndhandle(hbuf)
    sel = GetWndSel(hwnd)
    str = GetBufline(hbuf, sel.lnFirst)

    flag = StrChinChk(str, sel.ichFirst)
    //���λ�������ַ�֮������ǰ�ƶ�һ���ַ�
    if (flag == True)
    {
        Cursor_Left
    }
}

//���ƹ��
macro EM_CursorDown()
{
    hWnd = GetCurrentWnd()
    if(hWnd == 0)
        stop

    hbuf = GetCurrentBuf()

    //�ƶ����
    Cursor_Down

    //����ƶ�����Ĺ��λ��
    hwnd = GetWndhandle(hbuf)
    sel = GetWndSel(hwnd)
    str = GetBufline(hbuf, sel.lnFirst)

    flag = StrChinChk(str, sel.ichFirst)
    //���λ�������ַ�֮������ǰ�ƶ�һ���ַ�
    if (flag == True)
    {
        Cursor_Right
    }
}


//���ƹ��
macro EM_CursorRight()
{
    hWnd = GetCurrentWnd()
    if(hWnd == 0)
        stop

    hbuf = GetCurrentBuf()

    //�ƶ����
    Cursor_Right

    //����ƶ�����Ĺ��λ��
    hwnd = GetWndhandle(hbuf)
    sel = GetWndSel(hwnd)
    str = GetBufline(hbuf, sel.lnFirst)

    flag = StrChinChk(str, sel.ichFirst)
    //���λ�������ַ�֮������ǰ�ƶ�һ���ַ�(�����ʱ��������ƶ�һ���ַ�)
    if (flag == True)
    {
        Cursor_Right
    }
}

//���ƹ��
macro EM_CursorLeft()
{
    hWnd = GetCurrentWnd()
    if(hWnd == 0)
        stop

    hbuf = GetCurrentBuf()

    //�ƶ����
    Cursor_Left

    //����ƶ�����Ĺ��λ��
    hwnd = GetWndhandle(hbuf)
    sel = GetWndSel(hwnd)
    str = GetBufline(hbuf, sel.lnFirst)

    flag = StrChinChk(str, sel.ichFirst)
    //���λ�������ַ�֮������ǰ�ƶ�һ���ַ�(�����ʱ��������ƶ�һ���ַ�)
    if (flag == True)
    {
        Cursor_Left
    }
}

//����ѡ���ַ�
macro EM_SelectWordLeft()
{
    hWnd = GetCurrentWnd()
    if(hWnd == 0)
        stop
    hbuf = GetCurrentBuf()

    //ִ������
    Select_Char_Left

    hwnd = GetWndhandle(hbuf)
    //selold = GetWndSel(hwnd)
    sel = GetWndSel(hwnd)
    //ln = GetBufLnCur(hbuf)

    /*
    if (selold.ichFirst == sel.ichFirst && sel.lnFirst == selold.lnFirst)
        curinhead = 1
    else
        curinhead = 0
    */
    str = GetBufline(hbuf, sel.lnFirst)
    hdflag = StrChinChk(str, sel.ichFirst)

    str = GetBufline(hbuf, sel.lnLast)
    bkflag = StrChinChk(str, sel.ichLim)

    if (hdflag == TRUE || bkflag == TRUE)
    {
        Select_Char_Left
    }
}

//����ѡ���ַ�
macro EM_SelectWordRight()
{
    hWnd = GetCurrentWnd()
    if(hWnd == 0)
        stop
    hbuf = GetCurrentBuf()

    //ִ������
    Select_Char_Right

    hwnd = GetWndhandle(hbuf)
    //selold = GetWndSel(hwnd)
    sel = GetWndSel(hwnd)
    //ln = GetBufLnCur(hbuf)

    /*
    if (selold.ichFirst == sel.ichFirst && sel.lnFirst == selold.lnFirst)
        curinhead = 1
    else
        curinhead = 0
    */
    str = GetBufline(hbuf, sel.lnFirst)
    hdflag = StrChinChk(str, sel.ichFirst)

    str = GetBufline(hbuf, sel.lnLast)
    bkflag = StrChinChk(str, sel.ichLim)

    if (hdflag == TRUE || bkflag == TRUE)
    {
        Select_Char_Right
    }
}

//����ѡ���ַ�
macro EM_SelectLineUp()
{
    hWnd = GetCurrentWnd()
    if(hWnd == 0)
        stop
    hbuf = GetCurrentBuf()

    //ִ������
    Select_Line_Up

    hwnd = GetWndhandle(hbuf)
    //selold = GetWndSel(hwnd)
    sel = GetWndSel(hwnd)
    //ln = GetBufLnCur(hbuf)

    /*
    if (selold.ichFirst == sel.ichFirst && sel.lnFirst == selold.lnFirst)
        curinhead = 1
    else
        curinhead = 0
    */
    str = GetBufline(hbuf, sel.lnFirst)
    hdflag = StrChinChk(str, sel.ichFirst)

    str = GetBufline(hbuf, sel.lnLast)
    bkflag = StrChinChk(str, sel.ichLim)

    if (hdflag == TRUE || bkflag == TRUE)
    {
        Select_Char_Right
    }
}

//����ѡ���ַ�
macro EM_SelectLineDown()
{
    hWnd = GetCurrentWnd()
    if(hWnd == 0)
        stop
    hbuf = GetCurrentBuf()

    //ִ������
    Select_Line_Down

    hwnd = GetWndhandle(hbuf)
    //selold = GetWndSel(hwnd)
    sel = GetWndSel(hwnd)
    //ln = GetBufLnCur(hbuf)

    /*
    if (selold.ichFirst == sel.ichFirst && sel.lnFirst == selold.lnFirst)
        curinhead = 1
    else
        curinhead = 0
    */
    str = GetBufline(hbuf, sel.lnFirst)
    hdflag = StrChinChk(str, sel.ichFirst)

    str = GetBufline(hbuf, sel.lnLast)
    bkflag = StrChinChk(str, sel.ichLim)

    if (hdflag == TRUE || bkflag == TRUE)
    {
        Select_Char_Right
    }
}

//���ַ���str��lnλ���м��
//�����ż���������ַ��򷵻�FALSE
//����������������ַ��򷵻�TRUE
macro StrChinChk(str, ln)
{
    tm  = 0
    flag = False
    len  = strlen(str)
    while (tm < ln)
    {
        if (str[tm] != "")
            ascstr = asciifromchar(str[tm])
        else
            ascstr = 0

        //�����ַ�ASCII > 128
        if (ascstr > 128)
            flag = !flag

        tm = tm + 1
        if (tm >= len)
            break
    }
    return flag
}

// �ڹ����в��Ұ������,����"macro OpenorNewBuf(szfile)"
macro FindHalfChcharInProj()
{
    hprj = GetCurrentProj()
    if (hprj == 0)
        stop
    ifileMax = GetProjFileCount(hprj)
    
    hOutBuf = OpenorNewBuf("HalfChch.txt")
    if (hOutBuf == hNil)
    {
        Msg("Can't Open file:HalfChchar.txt")
        stop
    }
    AppendBufLine(hOutBuf, ">>��������б�>>")
    
    ifile = 0
    while (ifile < ifileMax)
    {
        filename = GetProjFileName(hprj, ifile)
        hbuf = OpenBuf(filename)
        if (hbuf != 0)
        {
            StartMsg("@filename@ is being processing. . . press ESC to cancel.")
            iTotalLn = GetBufLineCount(hbuf)
            iCurLn = 0
            while (iCurLn < iTotalLn)
            {
                str = GetBufline(hbuf, iCurLn)
                flag = StrChinChk(str, strlen(str))
                if (flag == True)
                {
                    // ���ڰ������,��¼�ļ������к�
                    iOutLn = iCurLn + 1
                    outstr = cat(filename, "(@iOutLn@) : ")
                    outstr = cat(outstr, str)
                    AppendBufLine(hOutBuf, outstr)
                    SetSourceLink(hOutBuf,GetBufLineCount(hOutBuf)-1,filename,iCurLn)
                }
                iCurLn = iCurLn + 1
            }
            EndMsg()
        }
        ifile = ifile + 1
    }
    //SetCurrentBuf(hOutBuf)
    //Go_To_First_Link
}

// ���û��szfileָ�����ļ���,���½�,�����,������BUFF���
macro OpenorNewBuf(szfile)
{
    hout = GetBufHandle(szfile)
    if (hout == hNil)
    {
        hout = OpenBuf(szfile)
        if (hout == hNil)
        {
            hout = NewBuf(szfile)
            NewWnd(hout)
        }
    }
    return hout
}
