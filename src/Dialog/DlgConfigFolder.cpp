// DlgConfigFolder.cpp : implementation file
//

#include "stdafx.h"
#include "EasyRegistry.h"
#include "DlgConfigFolder.h"
#include "toolbar/MenuData.h"
#include "UTIL/ini.h"
#include "RegWorks/RegWorks.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
#include "RegWorks/RegWorks.h"
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgConfigFolder dialog


CDlgConfigFolder::CDlgConfigFolder(int iIndex, CRect rc, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConfigFolder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgConfigFolder)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    _iIndex = iIndex;
    _rc = rc;
}


void CDlgConfigFolder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConfigFolder)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConfigFolder, CDialog)
	//{{AFX_MSG_MAP(CDlgConfigFolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConfigFolder message handlers

BOOL CDlgConfigFolder::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CDSIni ini(Int2Str(_iIndex), PROFILE_INI);
	CString text = ini.GetStr(L"text", L"");
	CString path = ini.GetStr(L"path", L"");
	if ( text.GetLength() == 0 && path.GetLength() == 0)
	{
		CString clipText;
		if ( GetClipboardText(m_hWnd, clipText) )
		{
			if( RegWorks::IsValidPath(clipText) )
			{
				RegWorks::Validate(clipText);
				int pos = clipText.ReverseFind(L'\\');
				if( pos > 0 )
				{
					text = clipText.Mid(pos + 1);
					path = clipText;					
				}
			}
		}
	}
    SetDlgItemText(CEDIT_TEXT, text );	
    SetDlgItemText(CEDIT_PATH, path );
	SetDlgItemText(CEDIT_NAME,  ini.GetStr(L"name", L""));

	GetDlgItem(IDC_STATIC_3)->ShowWindow(SW_HIDE);

    CWindowRect rcWin(this);
    CScreenRect rcScr;
    if( (_rc.left + rcWin.Width()) > rcScr.Width())
        _rc.left = rcScr.Width() - rcWin.Width() - 10;

    MoveWindow(_rc.left, _rc.bottom, rcWin.Width(), rcWin.Height());

    GetDlgItem(CCHK_USE)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_HIDE);

    CRect rc;
    GetDlgItem(CCHK_USE)->GetWindowRect(&rc);
    ScreenToClient(&rc);
    
    CRect rc1;
    GetDlgItem(IDOK)->GetWindowRect(&rc1);
    ScreenToClient(&rc1);
    int h = rc1.Height();
    rc1.top = rc.top;
    rc1.bottom = rc1.top + h;
    GetDlgItem(IDOK)->MoveWindow(&rc1);

    GetDlgItem(IDCANCEL)->GetWindowRect(&rc1);
    ScreenToClient(&rc1);
    h = rc1.Height();
    rc1.top = rc.top;
    rc1.bottom = rc1.top + h;
    GetDlgItem(IDCANCEL)->MoveWindow(&rc1);

    GetWindowRect(&rcWin);
    SetWindowPos(NULL, 0, 0, rcWin.Width(), rc1.bottom + 32, SWP_NOMOVE);
    
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgConfigFolder::OnOK() 
{		
    CDSIni ini(Int2Str(_iIndex), PROFILE_INI);
    CString s;
    
    GetDlgItemText(CEDIT_TEXT, s);
    ini.WriteStr(L"text", s);
    
    GetDlgItemText(CEDIT_NAME, s);
    ini.WriteStr(L"name", s);

    GetDlgItemText(CEDIT_PATH,  s);
    ini.WriteStr(L"path", s);

	CDialog::OnOK();
}

/************************************************************************
//
//  
//
*************************************************************************/



CDlgConfigFolder_Sub::CDlgConfigFolder_Sub(CRect rc, int iSubIndex, CString sFolderKey, CString sTargetPath, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConfigFolder_Sub::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgConfigFolder_Sub)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    _iSubIndex = iSubIndex;
    _sFolderKey = sFolderKey;
    _sTargetPath = sTargetPath;
    _rc = rc;
}


void CDlgConfigFolder_Sub::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConfigFolder_Sub)
		// NOTE: the ClassWizard will add DDX and DDV calls here
			DDX_Control(pDX, IDC_STATIC_COL1, st1);
            DDX_Control(pDX, IDC_STATIC_COL2, st2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConfigFolder_Sub, CDialog)
	//{{AFX_MSG_MAP(CDlgConfigFolder_Sub)
    ON_BN_CLICKED(IDC_BUTTON_COL1, OnCol1)
    ON_BN_CLICKED(IDC_BUTTON_COL2, OnCol2)
    ON_BN_CLICKED(CCHK_USE, OnCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConfigFolder_Sub message handlers

BOOL CDlgConfigFolder_Sub::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CDSIni ini(_sFolderKey, SUBMENU_INI);
	CString sData = ini.GetStr(Int2Str(_iSubIndex), L"");
	if(sData.GetLength())
	{
		int pos = sData.Find(L"|");
		SetDlgItemText(CEDIT_TEXT, sData.Left(pos) );
		
		sData = sData.Mid(pos+1);
		pos = sData.Find(L"|");
		if (pos > 0)
		{
			SetDlgItemText(CEDIT_PATH, sData.Left(pos) );
			SetDlgItemText(CEDIT_NAME,  sData.Mid(pos + 1) );
		}
		else
		{
			SetDlgItemText(CEDIT_PATH, sData );
		}
	}
	else
	{
		if ( _sTargetPath.GetLength() == 0)
		{
			CString text;
			if ( GetClipboardText(m_hWnd, text) )
			{
				if( RegWorks::IsValidPath(text) )
				{
					RegWorks::Validate(text);
					_sTargetPath = text;
				}
			}
		}

		CString s = _sTargetPath.Mid( _sTargetPath.ReverseFind(L'\\')+1);
		SetDlgItemText(CEDIT_TEXT, s );
		SetDlgItemText(CEDIT_PATH, _sTargetPath);
	}	

    CWindowRect rcWin(this);
    CScreenRect rcScr;
    if( (_rc.left + rcWin.Width()) > rcScr.Width())
        _rc.left = rcScr.Width() - rcWin.Width()-10;

    MoveWindow(_rc.left, _rc.bottom, rcWin.Width(), rcWin.Height());

    MyEn(FALSE);

	CString sDefault;
	CDSIni ini2(_sFolderKey + L"E", SUBMENU_INI);
	CString sDisplay = ini2.GetStr(Int2Str(_iSubIndex), L"-1");
	if(sDisplay == L"-1")
	{
		CDSIni cfg(L"main", PROFILE_INI);
		sDefault = cfg.GetStr(L"default");
	}
	else
	{
		sDefault = sDisplay;
	}

	if(sDefault.GetLength() > 0)
	{
		MenuData md;
		TMenuData_Parse(sDefault, md);
		CheckDlgButton(CCHK_USE, BST_CHECKED);
		MyEn(TRUE); //내부 콘트롤을 Enable 시킨다.
		if(md._bBold)
		{
			CheckDlgButton(CCHK_BOLD, BST_CHECKED);
		}

		if(md._colBg != -1)
		{
			st1.col = md._colBg;
			st1.b = true;
			st1.Invalidate();
		}
		if(md._colText  != -1)
		{
			st2.col = md._colText;
			st2.b = true;
			st2.Invalidate();
		}
		if(md._iHeight != -1)
		{
			CString s;
			s.Format(_T("%d"), md._iHeight);
			CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COMBO_HEIGHT);
			if(pBox)
			{
				int iIndex = pBox->FindString(0,s);
				if(iIndex>=0) pBox->SetCurSel(iIndex);
			}
		}

		if(md._iSize != -1)
		{
			CString s;
			s.Format(_T("%d"), (int)(md._iSize/10));
			CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COMBO_SIZE);
			if(pBox)
			{
				int iIndex = pBox->FindString(0,s);
				if(iIndex>=0) pBox->SetCurSel(iIndex);
			}
		}
	}

    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




void CDlgConfigFolder_Sub::OnOK() 
{		
    CDSIni ini(_sFolderKey, SUBMENU_INI);
    CString sPath;    
    CString sName;
//    int i;
    GetDlgItemText(CEDIT_TEXT, sName);
	_sTitle = sName;
	if(sName.Find(L",")>=0)
	{
		AfxMessageBox(L"제목으로 , 문자를 사용할 수 없습니다.");
		return;
	}
        
    GetDlgItemText(CEDIT_PATH,  sPath);
	_sTargetPath = sPath;

    GetDlgItemText(CEDIT_NAME,  name_);

    CString sData = sName + L"|" + sPath + L"|" + name_;
    ini.WriteStr(Int2Str(_iSubIndex), NULL);    //먼저 해당것을 지운다..
   
    //  맨 처음 내것을 맨 위에 입력하고..
    ini.WriteStr(Int2Str(_iSubIndex), sData);
   
	BOOL bDefault = ((CButton*)GetDlgItem(CHK_DEFAULT))->GetCheck();
	CDSIni cfg(L"main", PROFILE_INI);

    CDSIni ini2(_sFolderKey + L"E", SUBMENU_INI);    
    CString sExt = Int2Str(_iSubIndex);
    if( ! ((CButton*)GetDlgItem(CCHK_USE))->GetCheck() )
    {
        ini2.WriteStr(sExt, L"");
		if( bDefault )
		{
			cfg.WriteStr(L"default", L"");
		}
    }
	else
    {
        //진하게..
        BOOL bBold = ((CButton*)GetDlgItem(CCHK_BOLD))->GetCheck();
        int  iSize=-1;
        int iCur = ((CComboBox*)GetDlgItem(IDC_COMBO_SIZE))->GetCurSel();
        if(iCur>0)
        {
            CString s;
             ((CComboBox*)GetDlgItem(IDC_COMBO_SIZE))->GetLBText(iCur, s);
            iSize = _ttoi(s);
        }
        int iHeight=-1;
        iCur = ((CComboBox*)GetDlgItem(IDC_COMBO_HEIGHT))->GetCurSel();
        if(iCur>0)
        {
            CString s;
            ((CComboBox*)GetDlgItem(IDC_COMBO_HEIGHT))->GetLBText(iCur, s);
            iHeight = _ttoi(s);
        }
        COLORREF col1 = st1.b ? st1.col : -1;
        COLORREF col2 = st2.b ? st2.col : -1;


        CString sExtVal;
        sExtVal.Format(L"%d|%d|%d|%d|%d", bBold, iSize, iHeight, col1, col2);
        ini2.WriteStr(sExt, sExtVal);
		if(bDefault)
			cfg.WriteStr(L"default",sExtVal);
    }

	CDialog::OnOK();
}

void TMenuData_Parse(CString sExt, MenuData&data)
{
    if(sExt.IsEmpty())
        return;

    CString s[5];
    for(int i=0; i<5; i++)
        AfxExtractSubString(s[i],  sExt, i, '|');
    if(s[0] == L"1") data._bBold = true; else data._bBold = false;
    if(s[1] == L"-1") data._iSize = -1; else data._iSize = _ttoi( s[1]) * 10;
    if(s[2] == L"-1") data._iHeight = -1; else data._iHeight = _ttoi( s[2]);
    if(s[3] == L"-1") data._colBg = -1; else data._colBg = _ttoi( s[3]);
    if(s[4] == L"-1") data._colText = -1; else data._colText = _ttoi( s[4]);
}

void CDlgConfigFolder_Sub::OnCol1()
{
    CColorDialog dlg(0, 0, this);
    if(dlg.DoModal()==IDOK)
    {
        st1.b = true;
        st1.col = dlg.GetColor();
    }else
        st1.b = false;
    Invalidate();
}
void CDlgConfigFolder_Sub::OnCol2()
{
    CColorDialog dlg(0, 0, this);
    if(dlg.DoModal()==IDOK)
    {
        st2.b = true;
        st2.col = dlg.GetColor();
    }else
        st2.b = false;
    Invalidate();
}

void CDlgConfigFolder_Sub::OnCheck()
{
    BOOL b = 0;
    if( ((CButton*)GetDlgItem(CCHK_USE))->GetCheck() )   
    {
        b = 1;
    }  
    MyEn(b);
}

void CDlgConfigFolder_Sub::MyEn(BOOL b)
{

    
    UINT ids[]= {CCHK_BOLD, IDC_COMBO_SIZE, IDC_COMBO_HEIGHT, IDC_BUTTON_COL1, IDC_BUTTON_COL2};
    for(int i=0;i<5;i++)
        GetDlgItem(ids[i])->EnableWindow(b);
}
