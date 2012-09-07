// TCmdBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EasyRegistry.h"
#include "Dialog/ToolbarDlg.h"
#include "Dialog/RegistryPathDlg.h"
#include "Toolbar/MenuData.h"
#include "Toolbar/ToolbarUtil.h"
#include "Dialog/EditSubmenuDlg.h"
#include "RegWorks\RegWorks.h"
#include "JumptoRegDlg.h"
#include "OptionDlg.h"
#include "SearchUI/SearchHistoryWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTCmdBarDlg dialog

CToolbarDlg::CToolbarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolbarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTCmdBarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CToolbarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTCmdBarDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}



/*
 *	Ʈ���̿����� ���� ������� ��쿡 �������� �ٽ� �����ϱ����ؼ� �޽����� �޴´�.
 */
const UINT WM_TRAYCREATED = RegisterWindowMessage(_T("TaskbarCreated"));

BEGIN_MESSAGE_MAP(CToolbarDlg, CDialog)
	//{{AFX_MSG_MAP(CTCmdBarDlg)
    ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_MESSAGE(WMUM_RDOWN, OnUserRButtonDown)
//    ON_MESSAGE(WMUM_DROPFILES, OnUserDropFiles)
    ON_MESSAGE(WM_TRAYCREATED, OnUserTrayCreated)
    ON_COMMAND(IDM_ABOUTBOX, OnAbout)
	ON_WM_DESTROY()
	ON_WM_TIMER()    
	ON_WM_SETTINGCHANGE()
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
    ON_COMMAND_RANGE(100, 200, OnButtonCommandRange)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_JUMPTOREG, &CToolbarDlg::OnJumptoreg)
	ON_COMMAND(ID_OPTION, &CToolbarDlg::OnOption)
	ON_COMMAND(ID_HELP, &CToolbarDlg::OnHelp)
	ON_COMMAND(ID_RUN_EDITOR, &CToolbarDlg::OnRunEditor)
	ON_COMMAND(ID_MNU_SEARCH, &CToolbarDlg::OnMnuSearch)
	ON_COMMAND(ID_TRAY_DEVPAGE, &CToolbarDlg::OnTrayDevpage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTCmdBarDlg message handlers


BOOL CToolbarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    /*
     *  ��Ż Ŀ�ǵ� �����ٸ� �ʱ�ȭ�Ѵ�..
     */
    /// �½�ũ�ٿ� �� ��Ÿ���� �Ѵ�.
    //  
    ModifyStyleEx(WS_EX_APPWINDOW, 0);
    SetWindowPos(&wndTopMost, 0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	

    /// ���ٸ� �����ϰ� �ʱ�ȭ�Ѵ�.
    //  
	m_bar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP | CCS_VERT | CCS_NODIVIDER);
    m_bar.ModifyStyle(0, TBSTYLE_LIST);

    /// Set Buttons
    //  
    LoadIni(&m_bar);
    RecalSize(true);   
    //if(INI_INT("bTray", 1) == 1)
    {    
        m_tray.Create(this, L"EasyRegistry 1.0", m_hIcon);
    }

    autoClose_ = _GetInt(L"autoClose", 1);

	SetTimer(101, 50, NULL);
	SetTimer(102, 1000, NULL);
    ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CToolbarDlg::OnAbout()
{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
}
void CToolbarDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CToolbarDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CToolbarDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CToolbarDlg::PreTranslateMessage(MSG* pMsg) 
{
    /*
     *	����� ���콺 Ctrl�� ���콺 ������ ������ â�� �����̰� �Ѵ�.
     */
	if(pMsg->hwnd == m_bar.m_hWnd &&
        pMsg->message == WM_LBUTTONDOWN &&
        ISKEYDOWN(VK_CONTROL))
    {
        CPoint point = pMsg->pt;
        PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));        
        return 1;
    }

	return CDialog::PreTranslateMessage(pMsg);
}


/**
 *	���� Ŭ������ �� 
 */
void CToolbarDlg::OnButtonCommandRange(UINT ids)
{
    int iIndex = ids - 100;
    ShowSubfolder(iIndex, false);
}   

LRESULT CToolbarDlg::OnUserRButtonDown(WPARAM wp, LPARAM lp)
{
    int iIndex = (INT)wp;
    int bCtrl = (INT)lp;

    /**
     *	Ctrl�� ������ ���� �ʴٸ�, ����޴��� ����ϵ��� �Ѵ�.
     */
    if( ShowSubfolder(iIndex,  true))
    {
         return 0;
    }

#if 0
    /**
     *	Ctrl�� ������ �ְų�
     *  ����޴����ϳ��� ���ٸ�(���� ��ư) ���α׷� �޴��� ����.
     */
    CMenu menu;
    CPoint point;

    GetCursorPos(&point);
    menu.CreatePopupMenu(); 
	menu.AppendMenu(MF_STRING, 105, L"�����ʿ� �߰�");
	menu.AppendMenu(MF_STRING, 106, L"�Ʒ��� �߰�");
	menu.AppendMenu(MF_STRING, 107, L"�ּҷ� �ٷ� �̵��ϱ�");
    menu.AppendMenu(MF_STRING, 100, L"����");
	menu.AppendMenu(MF_STRING, 101, L"����");
    menu.AppendMenu(MF_STRING, 104, L"RegEditâ�� �޶�ٱ�");
    menu.AppendMenu(MF_STRING, 103, L"����");
	menu.AppendMenu(MF_STRING, 102, L"EasyRegistry ����");

    int bGlue = _GetInt( L"bGlue", 1);
    if(bGlue) 
        menu.CheckMenuItem(104, MF_BYCOMMAND | MF_CHECKED);
    int iCmd = menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RETURNCMD, point.x, point.y,this, NULL);        
    if(iCmd <=0)
        return 1;

    if(102 == iCmd)
    {
        PostQuitMessage(1);
        return 1;
    }
    else if(104 == iCmd)
    {
      bGlue = !bGlue;
      _WriteInt( L"bGlue", bGlue);
    }    
    else if(100 == iCmd)
    {    
        CRect rc;
        m_bar.GetItemRect(iIndex, &rc);
        m_bar.ClientToScreen(&rc);
        CDlgConfigFolder dlg(iIndex, rc);
        if(dlg.DoModal() == IDOK)
        {
            LoadIni(&m_bar);
            RecalSize(false);
        }
    }
    //����
    else if(101 == iCmd)
    {
        //iIndex�� ������ ���̴�..
        CString sSubkey = g_arButtonData[iIndex].submenu_;
        WritePrivateProfileString(sSubkey,NULL,NULL,SUBMENU_INI);

        int iTotalSize = _GetInt(L"size", 0);
        if(iTotalSize > 0)
        {        
            for(int i=iIndex+1; i<=iTotalSize-1; i++ )
            {
                dsSectionToSection(Int2Str(i), Int2Str(i-1), PROFILE_INI, false );
            }
            WritePrivateProfileString(L"main",L"size",Int2Str(iTotalSize-1), PROFILE_INI);
        }
        LoadIni(&m_bar);
        RecalSize(false);
    }
    else if(103 == iCmd)
    {
        ShellExecute(m_hWnd, _T("open"), _ModulePath(L"EasyRegistry.txt"), NULL, NULL, SW_NORMAL);
    }
	else if( 105 == iCmd )
	{
		CPoint pt;
		GetCursorPos(&pt);
		CRect rc(pt.x, pt.y, pt.x, pt.y);

		int iTotalSize = _GetInt( L"size", 0);

		/**
		 *	iIndex�����ʿ� ���ο� ��ư�� �߰��Ѵ�
		 * 
		 *  iIndex���� ���������� �� ĭ�� �ڷ� �����Ѵ�.
		 */    
		if(iIndex >=0 && iTotalSize>0)
		{        
			for(int i=iTotalSize-1 ; i>=iIndex+1; i--)
			{
				dsSectionToSection(Int2Str(i), Int2Str(i+1), PROFILE_INI, false);
			}
		}
	    
		int iNewIndex = iIndex+1;
		if(iTotalSize ==0)
			iNewIndex = 0;
	    
		//< �� ��ư�� ������ ini�� ����.
		CDSIni ini(Int2Str(iNewIndex), PROFILE_INI);
		ini.WriteStr(L"path", g_sDropFolderPath);
		int rpos = g_sDropFolderPath.ReverseFind(L'\\');
		CString sText = g_sDropFolderPath.Mid(rpos+1);
		ini.WriteStr(L"text", sText);
		ini.WriteStr(L"subkey", CreateSubkey());

		//< ��ü ��ư ����� �����Ѵ�.
		ini.Init(L"main", PROFILE_INI);
		ini.WriteInt(L"size", iTotalSize+1);
	    
		/**
		 *  �̹� ������ ��ư�� ���� ����â�� ����.	
		 */
		CDlgConfigFolder dlg(iNewIndex, rc);
		if(dlg.DoModal()== IDOK)
		{
			LoadIni(&m_bar);
			RecalSize(false);
		}else
		{
			// ����â�� ����ߴٸ� �ٽ� ��ư������ ini���� �����.
			//
			for(int i=iNewIndex+1; i<=iTotalSize+1; i++ )
			{
				dsSectionToSection(Int2Str(i), Int2Str(i-1), PROFILE_INI, false );
			}
			ini.WriteInt(L"size", iTotalSize);
		}
	}
	else if ( 106 == iCmd )
	{
		CPoint pt;
		GetCursorPos(&pt);
		CRect rc(pt.x, pt.y, pt.x, pt.y);
		if(AddSubMenu(iIndex, L"", rc))
        {
            LoadIni(&m_bar);
            RecalSize(false);
        }
	}
	else if( 107 == iCmd )
	{
		CJumptoRegDlg dlg;
		if( dlg.DoModal() == IDOK )
		{
			JumptoReg(dlg.path_);
		}
	}
#endif
    return 1;
}

void CToolbarDlg::RecalSize(bool bLoadPos)
{
    /// Resize Dialog & Toolbar
    //  
    CSize sizeToolbar = m_bar.CalcFixedLayout(false,true);
    CRect rc;
    int iCount = m_bar.GetToolBarCtrl().GetButtonCount();
    m_bar.GetItemRect(iCount-1, &rc);
    m_bar.MoveWindow(-7,-5,rc.right + 11, sizeToolbar.cy+7);

    int ix = 0;
    int iy = 0;
    if(bLoadPos)
    {    
        ix = _GetInt(L"x", 100);
        iy = _GetInt(L"y", 100);
    }
    
    SetWindowPos(&wndTopMost ,ix,
                              iy,
                              rc.right + 1, 
                              sizeToolbar.cy-10, 
                              SWP_SHOWWINDOW | (bLoadPos==true ? 0 : SWP_NOMOVE));                              
    
}



#if 0
/**
 * ���ٿ��� ������ ����Ͽ����� �������� ����� ���� �޽��� �ڵ鷯
 * @param   wp      (int)wp ����� ��ư�� Index
 * @param   lp      wp�� ����
 * @return
 */
LRESULT CToolbarDlg::OnUserDropFiles(WPARAM wp, LPARAM lp)
{   
    SetForegroundWindow();
    //����� ��ư index
    int iIndex = (int)wp;
    CRect rc;
    m_bar.GetItemRect(iIndex, &rc);
    m_bar.ClientToScreen(&rc);

    int iTotalSize = _GetInt(L"size", 0);
    /**
     *	��� �ϳ��� ��ư�� �ִٸ� ���� ������ �˾��޴��� �������� ������ �� �ִ�.
     */
    if(iTotalSize>0)
    {    
        CPoint pt;
        GetCursorPos(&pt);
        CMenu mnuPopup;
        mnuPopup.CreatePopupMenu();
        mnuPopup.AppendMenu(MF_STRING, 1, (TCHAR*)L"���� ��ư���� �߰�");
        mnuPopup.AppendMenu(MF_STRING, 2, (TCHAR*)L"�Ʒ� �޴��� �߰�");
        int iCmd = mnuPopup.TrackPopupMenu(TPM_RETURNCMD, pt.x, pt.y, this, NULL);
        if(0 == iCmd)
            return 0;
    
        if(2 == iCmd)
        {
            /**
             *	�����޴��� �߰��ϴ� ���̾�α׸� ����� �����Ѵ�.
             */
            if(AddSubMenu(iIndex, g_sDropFolderPath, rc))
            {
                LoadIni(&m_bar);
                RecalSize(false);
            }
            return 1;
        }
    }


    /**
     *	iIndex�����ʿ� ���ο� ��ư�� �߰��Ѵ�
     * 
     *  iIndex���� ���������� �� ĭ�� �ڷ� �����Ѵ�.
     */    
    if(iIndex >=0 && iTotalSize>0)
    {        
        for(int i=iTotalSize-1 ; i>=iIndex+1; i--)
        {
            dsSectionToSection(Int2Str(i), Int2Str(i+1), PROFILE_INI, false);
        }
    }
    
    int iNewIndex = iIndex+1;
    if(iTotalSize ==0)
        iNewIndex = 0;
    
    //< �� ��ư�� ������ ini�� ����.
    CDSIni ini(Int2Str(iNewIndex), PROFILE_INI);
    ini.WriteStr(L"path", g_sDropFolderPath);
    int rpos = g_sDropFolderPath.ReverseFind(L'\\');
    CString sText = g_sDropFolderPath.Mid(rpos+1);
    ini.WriteStr(L"text", sText);
    ini.WriteStr(L"subkey", CreateSubkey());

    //< ��ü ��ư ����� �����Ѵ�.
    ini.Init(L"main", PROFILE_INI);
    ini.WriteInt(L"size", iTotalSize+1);
    
    /**
     *  �̹� ������ ��ư�� ���� ����â�� ����.	
     */
    CDlgConfigFolder dlg(iNewIndex, rc);
    if(dlg.DoModal()== IDOK)
    {
        LoadIni(&m_bar);
        RecalSize(false);
    }else
    {
        // ����â�� ����ߴٸ� �ٽ� ��ư������ ini���� �����.
        //
        for(int i=iNewIndex+1; i<=iTotalSize+1; i++ )
        {
            dsSectionToSection(Int2Str(i), Int2Str(i-1), PROFILE_INI, false );
        }
        ini.WriteInt(L"size", iTotalSize);
    }
    return 1;
}
#endif

/**
 *	    �½�ũ�ٰ� �Ҹ��� �����Ǹ� �ٽ� ���̰� �Ѵ�.
 */
LRESULT CToolbarDlg::OnUserTrayCreated(WPARAM wp, LPARAM  lp)
{
    if(_GetInt(L"bTray", 1) == 1)
	{
		m_tray.ShowIcon();
	}
    return 1;
}

/**
 * Ʈ���̿��� ������� �޽����� �߻��� ��쿡 ȣ��ȴ�.
 * @param 
 * @param 
 * @return
 */
LRESULT CMyTray::OnCallback(WPARAM wp, LPARAM lp)
{
    return ((CToolbarDlg*)m_pParent)->OnUserOnTray(wp,lp);
}
/**
 *	     Ʈ���̿��� ���콺 Ŭ���̳� ��Ŭ���ÿ� �˾��޴��� ����
 */
LRESULT CToolbarDlg::OnUserOnTray(WPARAM wp, LPARAM lp)
{
    if(lp == WM_LBUTTONUP || lp == WM_RBUTTONUP)
    {        
    	CPoint	point;
        GetCursorPos(&point);
        SetForegroundWindow();
        
        CMenu menu;
        menu.LoadMenu(IDR_MENU_TRAY);
		CMenu* trayMenu = menu.GetSubMenu(0);
        trayMenu->TrackPopupMenu( TPM_LEFTALIGN, point.x, point.y,this, NULL);        
        PostMessage(WM_NULL, 0, 0);
    }
    return 1;
}

/**
 *	    ����ÿ� ��ġ�� �����մϴ�..
 */
void CToolbarDlg::OnDestroy() 
{
    CWindowRect rc(this);    
    _WriteInt(_T("x"), rc.left);
    _WriteInt(_T("y"), rc.top);		    
	CDialog::OnDestroy();
}


/**
 *	�����ٸ� �����.
 */
void CToolbarDlg::_Hide()
{
    if( ::IsWindowVisible(m_hWnd))
    {
        HWND hWnd = ::GetForegroundWindow();
        DWORD TID,PID;
	    TID = GetWindowThreadProcessId (hWnd, &PID);

        DWORD TID2,PID2;
        TID2 = GetWindowThreadProcessId (m_hWnd, &PID2);

        if(TID != TID2)
	        ShowWindow(SW_HIDE);
    }
}


void CToolbarDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 101)
	{
		static BOOL s_bWritePositon = 0;
		if(ISKEYDOWN(VK_CONTROL) && ISKEYDOWN(VK_LBUTTON))
		{
			//  ����ڰ� �̵��ϰ� ���� ���콺 Lbuton�� ������ ��Ŀ���� �� ��ġ�� �����Ѵ�.
			//
			s_bWritePositon = 1;
		}
		else if(s_bWritePositon == 1 && !ISKEYDOWN(VK_LBUTTON))
		{
			WriteGluePosition();
			s_bWritePositon = 0;
		}

		
		/*
		 *	50�и� Ÿ�̸ӷ� ���� ��ŻĿ�ǵ尡 Ȱ��ȭ �Ǿ� �ִ����� ����
		 *  ��Ŀ�ٸ� ���̰� ���߰� �Ѵ�.
		 */
		HWND hWnd = ::GetForegroundWindow();
		TCHAR szClass[MAX_PATH];
		GetClassName(hWnd, szClass, MAX_PATH);
		if(_tcsicmp(szClass, REGEDIT_CLASSNAME) == 0)
		{  
			/*
			 *	��Ŀ�ٰ� ��Ŀ�� �޶�پ� �������� �Ѵٸ� ��ġ�� ã�Ƽ� �̵���Ų��.
			 */
			int bGlue = _GetInt( L"bGlue", 1);
			if(bGlue && !::IsIconic(hWnd))
			{           
				int gx = _GetInt(L"iGlueX", 253);
				int gy = _GetInt(L"iGlueY", 21);

				CRect rc;
				::GetWindowRect(hWnd, &rc);
				CRect rcMy;
				GetWindowRect(&rcMy);
				if( rcMy.left - rc.left != gx || 
					rcMy.top - rc.top != gy)
				{            
					rcMy.left = rc.left+ gx;
					rcMy.top = rc.top + gy;
					SetWindowPos(NULL, rcMy.left, rcMy.top,0,0,SWP_NOSIZE);
				}
			}

			/*
			 *	��Ŀ�� ȸ�鿡 ���̰�, �ּ�ȭ�� �ƴϸ� �� ���� ��Ŀ�ٸ� ����.
			 */
			if( ! ::IsWindowVisible(m_hWnd) && !::IsIconic(hWnd))
			{
				ShowWindow(SW_SHOW);
				SetWindowPos(&wndTopMost, 0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
			}else 
				if(::IsIconic(hWnd))    ///��Ŀ�ٰ� �ּ�ȭ�Ǿ� ������ �����.
					_Hide();

		}else
		{
			/*
			 *	Ȱ��â�� ��Ŀ�ٰ� �ƴϰų�, �ּ�ȭ �Ǿ� ������ �ٸ� �����.
			 */
			if(hWnd != m_hWnd || ::IsIconic(hWnd))
			{
				_Hide();
			}
			else if ( RegWorks::FindRegEdit() == NULL )
			{
				ShowWindow(SW_HIDE);			
			}

		}
	}

	else if( nIDEvent == 102 )
	{
		if ( autoClose_ &&  RegWorks::FindRegEdit() == NULL )
		{
			if ( _GetInt(L"launcher") == 1)
			{
				PostQuitMessage(1);
			}
		}
	}
    
	CDialog::OnTimer(nIDEvent);
}


void CToolbarDlg::OnCancel()
{

}

/**
 *	���� �˾��޴��� ������ �߰��ϴ� �Լ�
 */
bool CToolbarDlg::AddSubMenu(int iIndex, CString sPath, CRect rc)
{
    CString s = Int2Str(iIndex);
    CDSIni  ini(s, PROFILE_INI);
    CString sSection = ini.GetStr(L"subkey", L"");
    CDSIni subIni(sSection, SUBMENU_INI);
    int iSubIndex = -1;
    /**
     *	����ִ� Entry�� ã�´�.
     */
    for(int i=0;i<20;i++)
    {
        CString sFirst = subIni.GetStr(Int2Str(i), L"")   ;
        if(sFirst.IsEmpty()) 
        {
            iSubIndex = i;
            break;
        }
    }

    /**
     *	��Ʈ���� ã���� �ű⿡ ����޴� �߰�â�� ����.
     */
    if(iSubIndex >=0)
    {
        CDlgConfigFolder_Sub dlg(rc, iSubIndex, sSection, sPath, this );
        if(IDOK == dlg.DoModal())
            return true;
    }
    return false;
}

/**
 * ���� ������ ��ȯ�Ѵ�
 * @param iIndex �� ��ư�� Index
 * @param  bRightButton  ���콺 ��Ŭ���̸� true
 * @return
 */
bool CToolbarDlg::ShowSubfolder(int iIndex, bool bRightButton)
{
    if(iIndex >= g_arButtonData.GetSize())        
	{
		return false;
	}

    ButtonData item = g_arButtonData[iIndex];

    //���� Ŭ���� ��쿡��.. 
    if( ! bRightButton )
    {
		if ( item.path_.GetLength() )
		{
			JumptoReg( item.path_, item.name_ );  
		}
    }
    else
    {
		CPoint pt;
		GetCursorPos(&pt);
		CRect rc;
		m_bar.GetItemRect(iIndex, &rc);
		m_bar.ClientToScreen(&rc);
		rc.right =  rc.left + 22;


        CStringArray ar;
        CStringArray arExt;
        CStringArray arKey;
        GetSubFolders(item.submenu_,  arKey, ar, arExt);        
        //if( ar.GetSize() ==0)
        //    return false;
        //if( ar.GetSize() > 0)
        {
            //  ���� ����ߴ� �޴� ����Ÿ�� map���� �����Ѵ�.
            //
            TMenuData_Clear();

			const int kHKLM = 4;
			const int kHKCU = 2;

			const int kIdToogle = 109;

			_KeyRoot key = KeyRoot::toType(item.path_);
			int toggleImage = -1;
			CString togglePrefix;
			if ( key == KeyRoot::HKLM )
			{
				toggleImage = kHKCU;
				togglePrefix = L"User > ";
			}
			else if ( key == KeyRoot::HKCU )
			{
				toggleImage = kHKLM;
				togglePrefix = L"Machine > ";
			}

            CMenu mnuPopup;
            mnuPopup.CreatePopupMenu();

			if ( toggleImage > 0 )
			{
				MenuData data;
				data._sText = togglePrefix + item.text_;
				data._iHeight = 20;
				data._iconIndex = toggleImage;
				TMenuData_Set(kIdToogle, data);
				mnuPopup.AppendMenu(MF_STRING|MF_OWNERDRAW, kIdToogle, (TCHAR*)NULL);
			}

            for(int i = 0; i < ar.GetSize(); i++)
            {
                CString item = ar[i];
				int pos = item.Find(L"|");
                CString s = item.Left ( pos );
				CString path = item.Mid(pos + 1);

                MenuData data;
                data._sText = s;
                data._iHeight = 20;


				if( KeyRoot::toType(path) == KeyRoot::HKLM )
				{
					data._iconIndex = kHKLM;
				}
				else if( KeyRoot::toType(path) == KeyRoot::HKCU )
				{
					data._iconIndex = kHKCU;
				}

                CString sExt = arExt[i];
				OutputDebugString(sExt);
				TMenuData_Parse(sExt, data);
                
                TMenuData_Set(i+1, data);
				if(s == L"-")
					mnuPopup.AppendMenu(MF_SEPARATOR|MF_OWNERDRAW, 0, (TCHAR*)NULL);
				else
					mnuPopup.AppendMenu(MF_STRING|MF_OWNERDRAW, i+1, (TCHAR*)NULL);
                if(i == ar.GetSize()-1)
                {
                    MenuData d2;
                    d2._sText = L"���� �޴� �����ϱ�...";
                    TMenuData_Set(1000, d2);
                    mnuPopup.AppendMenu(MF_STRING|MF_OWNERDRAW, 1000, (TCHAR*)NULL);
					mnuPopup.AppendMenu(MF_SEPARATOR|MF_OWNERDRAW, 0, (TCHAR*)NULL);
                }
            }

			int iTotalSize = _GetInt( L"size", 0);
			CDSIni ini0(Int2Str(0), PROFILE_INI);
			int dummy = 0;
			if( iTotalSize == 1 )
			{
				dummy = ini0.GetInt(L"dummy", 0);
			}

			{
				MenuData data;

				#define  _AppendMenu(id, text)	\
				data._sText = text;	\
				TMenuData_Set(id, data);	\
				mnuPopup.AppendMenu(MF_STRING|MF_OWNERDRAW, id, (TCHAR*)NULL);
				
				_AppendMenu( 107, L"�ּҷ� �ٷ� �̵��ϱ�");								
				_AppendMenu( 105, L"�����ʿ� �߰�");
				if ( dummy == 0)
				{
					_AppendMenu( 106, L"�Ʒ��� �߰�");
				}
				mnuPopup.AppendMenu(MF_SEPARATOR|MF_OWNERDRAW, 0, (TCHAR*)NULL);
				_AppendMenu( 108, L"�˻�...");
				_AppendMenu( 100, L"����");
				if ( dummy == 0)
				{
					_AppendMenu( 101, L"����");
				}
				_AppendMenu( 102, L"EasyRegistry ����");
				
				
				//_AppendMenu( 104, L"RegEditâ�� �޶�ٱ�");
				//_AppendMenu( 103, L"����");
				//_AppendMenu( 102, L"EasyRegistry ����");

				int bGlue = _GetInt( L"bGlue", 1);
				if(bGlue) 
				{
					mnuPopup.CheckMenuItem(104, MF_BYCOMMAND | MF_CHECKED);
				}
			}

            /**
             *	TMenuData���� ������ �����ͼ� �˾��޴��� �ٹδ�.
             */			
            int iRetCmd = mnuPopup.TrackPopupMenu(TPM_RETURNCMD , pt.x, pt.y, this);
            if(iRetCmd != 0)
            {
                if(iRetCmd == 1000)
                {
                    /**
                     *	���� ���� ���� ����.
                     */
                    EditSubmenuDlg dlg(item.submenu_);
                    if(dlg.DoModal() == IDOK)
                    {
                        if(dlg.m_bChanged)
                        {
                            LoadIni(&m_bar);
                            RecalSize(false);
                        }
                    }
                }
				else if(ISKEYDOWN(VK_CONTROL) && iRetCmd < 100)
                {
                    /**
                     *	VK_CONTROL�� ���� �����ϸ� �� �������� �����Ѵ�.
                     */
                    CDSIni ini (item.submenu_, SUBMENU_INI);
                    int iKey = _ttoi(arKey.GetAt(iRetCmd-1));
                    ini.WriteStr(Int2Str(iKey), L""); //1 2[s] 3[s] 4[s]
                    LoadIni(&m_bar);
                    RecalSize(false);
                }
                else if(ISKEYDOWN(VK_SHIFT) && iRetCmd < 100)
                {
					CString sData = ar.GetAt(iRetCmd-1);
					int pos = sData.Find(L"|");
					sData = sData.Mid(pos + 1);
					pos = sData.ReverseFind(L'|');
					CString path, name;
					if (pos > 0)
					{
						path = sData.Left(pos);
						name = sData.Mid( pos + 1);
					}
					else
					{
						path = sData;
					}

					key = KeyRoot::toType(path);
					if( key == KeyRoot::HKLM )
					{
						pos = path.Find(L"\\");
						JumptoReg( KeyRoot::toText(KeyRoot::HKCU) + L"\\" + path.Mid(pos + 1), name);
					}
					else if( key == KeyRoot::HKCU )
					{
						pos = path.Find(L"\\");
						JumptoReg( KeyRoot::toText(KeyRoot::HKLM) + L"\\" + path.Mid(pos + 1), name);
					}
					else
					{
						JumptoReg(path, name);
					}
                }
				else if ( iRetCmd < 100 )
                {                
                    CString sData = ar.GetAt(iRetCmd-1);
					int pos = sData.Find(L"|");
					sData = sData.Mid(pos + 1);

					pos = sData.ReverseFind(L'|');
					if (pos > 0)
					{
						JumptoReg( sData.Left(pos), sData.Mid( pos + 1));    
					}
					else
					{
						JumptoReg(sData);
					}
                }
				else
				{
						int iCmd = iRetCmd;
					// ���..
						if(102 == iCmd)
						{
							PostQuitMessage(1);
							return 1;
						}
						else if(104 == iCmd)
						{
							int bGlue = _GetInt( L"bGlue", 1);
							bGlue = !bGlue;
							_WriteInt( L"bGlue", bGlue);
						}    
						else if(100 == iCmd)
						{    
							CRect rc;
							m_bar.GetItemRect(iIndex, &rc);
							m_bar.ClientToScreen(&rc);
							RegistryPathDlg dlg(iIndex, rc);
							if(dlg.DoModal() == IDOK)
							{
								LoadIni(&m_bar);
								RecalSize(false);
								if ( dummy )
								{
									ini0.WriteInt(L"dummy", 0);
								}
							}
						}
						//����
						else if(101 == iCmd)
						{
							// ����Ű �����
							CString sSubkey = g_arButtonData[iIndex].submenu_;
							WritePrivateProfileString(sSubkey,NULL,NULL,SUBMENU_INI);
							WritePrivateProfileString(sSubkey + L"E",NULL,NULL,SUBMENU_INI);

							// �����
							CDSIni ini(Int2Str(iIndex), PROFILE_INI);
							ini.WriteStr(L"path", NULL);
							ini.WriteStr(L"name", NULL);
							ini.WriteStr(L"text", NULL);

							// �ű��
							int iTotalSize = _GetInt(L"size", 0);
							if(iTotalSize > 0)
							{        
								for(int i=iIndex+1; i<=iTotalSize-1; i++ )
								{
									dsSectionToSection(Int2Str(i), Int2Str(i-1), PROFILE_INI, false );
								}
								WritePrivateProfileString(L"main",L"size",Int2Str(iTotalSize-1), PROFILE_INI);
							}
							LoadIni(&m_bar);
							RecalSize(false);
						}
						else if(103 == iCmd)
						{
							ShellExecute(m_hWnd, _T("open"), _ModulePath(L"EasyRegistry.txt"), NULL, NULL, SW_NORMAL);
						}
						else if( 105 == iCmd )
						{
							CPoint pt;
							GetCursorPos(&pt);
							CRect rc(pt.x, pt.y, pt.x, pt.y);
							
							if( dummy )
							{
								iTotalSize --;
							}

							/**
							 *	iIndex�����ʿ� ���ο� ��ư�� �߰��Ѵ�
							 * 
							 *  iIndex���� ���������� �� ĭ�� �ڷ� �����Ѵ�.
							 */    
							if(iIndex >=0 && iTotalSize>0)
							{        
								for(int i=iTotalSize-1 ; i>=iIndex+1; i--)
								{
									dsSectionToSection(Int2Str(i), Int2Str(i+1), PROFILE_INI, false);
								}
							}
						    
							int iNewIndex = iIndex+1;
							if(iTotalSize ==0)
								iNewIndex = 0;
						    
							//< �� ��ư�� ������ ini�� ����.
							CDSIni ini(Int2Str(iNewIndex), PROFILE_INI);
							ini.WriteStr(L"path", g_sDropFolderPath);
							int rpos = g_sDropFolderPath.ReverseFind(L'\\');
							CString sText = g_sDropFolderPath.Mid(rpos+1);
							ini.WriteStr(L"text", sText);
							ini.WriteStr(L"subkey", CreateSubkey());

							//< ��ü ��ư ����� �����Ѵ�.
							ini.Init(L"main", PROFILE_INI);
							ini.WriteInt(L"size", iTotalSize+1);
						    
							/**
							 *  �̹� ������ ��ư�� ���� ����â�� ����.	
							 */
							RegistryPathDlg dlg(iNewIndex, rc);
							if(dlg.DoModal()== IDOK)
							{
								LoadIni(&m_bar);
								RecalSize(false);
								if( dummy )
								{
									ini0.WriteInt(L"dummy", 0);
								}
							}
							else
							{
								// ����â�� ����ߴٸ� �ٽ� ��ư������ ini���� �����.
								//
								for(int i=iNewIndex+1; i<=iTotalSize+1; i++ )
								{
									dsSectionToSection(Int2Str(i), Int2Str(i-1), PROFILE_INI, false );
								}
								ini.WriteInt(L"size", iTotalSize);
							}
						}
						else if ( 106 == iCmd )
						{
							CPoint pt;
							GetCursorPos(&pt);
							CRect rc(pt.x, pt.y, pt.x, pt.y);
							if(AddSubMenu(iIndex, L"", rc))
							{
								LoadIni(&m_bar);
								RecalSize(false);
							}
						}
						else if( 107 == iCmd )
						{
							CJumptoRegDlg dlg;
							if( dlg.DoModal() == IDOK )
							{
								JumptoReg(dlg.path_);
							}
						}
						else if( 108 == iCmd )
						{
							PostMessage(WM_COMMAND, ID_MNU_SEARCH, 0);
						}
						else if( iCmd == kIdToogle)
						{
							int pos = item.path_.Find(L"\\");
							CString path = item.path_.Mid(pos + 1);
							if( key == KeyRoot::HKLM )
							{
								JumptoReg( KeyRoot::toText(KeyRoot::HKCU) + L"\\" + path, item.name_);
							}
							else if( key == KeyRoot::HKCU )
							{
								JumptoReg( KeyRoot::toText(KeyRoot::HKLM) + L"\\" + path, item.name_);
							}
						}
						//���
				}
            }            
        }            
    }

    return true;
}


/**
 *  ��ŻĿ�ǵ忡�� ���ٰ� �󸶳��� �����ġ�� �ִ����� �����Ѵ�.	
 */
void CToolbarDlg::WriteGluePosition()
{
    HWND hWndCmd = ::FindWindow(REGEDIT_CLASSNAME, NULL);
    if(hWndCmd && !::IsIconic(hWndCmd))
    {        
        CRect rc;
        ::GetWindowRect(hWndCmd, &rc);
        CRect rcMy;
        GetWindowRect(&rcMy);
        int gx = rcMy.left - rc.left;
        int gy = rcMy.top - rc.top;

        _WriteInt( L"iGlueX", gx);
        _WriteInt( L"iGlueY", gy);
    }    

}

/**
 * ���̾�α����� ���ٸ� �ø��ٺ��� MFC�����ӿ��� ���ؼ� ���ٰ� �и��� ������ �ִµ�
 * �̸� �����ϱ����ؼ� Ư�� ������ �Ž����� �Ź� ������ ��� �ٽ� �ʱ�ȭ�Ѵ�.
 * �� ������ ����̱�� ������ �� ������ ���� ��.
 */
BOOL bNewLoad = FALSE;
void CToolbarDlg::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CDialog::OnSettingChange(uFlags, lpszSection);
	
    if(IsWindowVisible())
    {
        LoadIni(&m_bar);
        RecalSize(false);	        
    }else
        bNewLoad = TRUE;    ///< ������ ȸ�鿡 ���϶� ���ٸ� �ٽ� �ʱ�ȭ�Ѵ�.
}

void CToolbarDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);	
}

void CToolbarDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
    if(m_bar.m_hWnd && (lpwndpos->flags & SWP_SHOWWINDOW) && bNewLoad)
    {
        bNewLoad = FALSE;
        LoadIni(&m_bar);
        RecalSize(false);	
    }	
}
///< �˾��޴� �ڵ鷯 1
void CToolbarDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if(nIDCtl == 0)
    {
		CImageList& imageList = m_bar.m_il;
        TMenuData_OnDrawItem(nIDCtl, lpDrawItemStruct, imageList);
	    return;
    }

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
///< �˾��޴� �ڵ鷯 2
void CToolbarDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDCtl == 0)
    {
        TMenuData_OnMeasureItem(nIDCtl, lpMeasureItemStruct);
        return;
    }
    
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CToolbarDlg::OnJumptoreg()
{
	CJumptoRegDlg dlg;
	if( dlg.DoModal() == IDOK)
	{
		JumptoReg(dlg.path_);
	}
}

void CToolbarDlg::OnOption()
{
	COptionDlg dlg;
	if( dlg.DoModal() == IDOK)
	{
		autoClose_ = _GetInt(L"autoClose", 1);
	}
}

void CToolbarDlg::OnHelp()
{
	ShellExecute(m_hWnd, _T("open"), _ModulePath(L"EasyRegistry.txt"), NULL, NULL, SW_NORMAL);	
}

void CToolbarDlg::OnRunEditor()
{
	ShellExecute(m_hWnd, _T("open"), L"regedit.exe", NULL, NULL, SW_NORMAL);
}

void CToolbarDlg::OnMnuSearch()
{
	CSearchHistoryWnd * wnd = new CSearchHistoryWnd();
	wnd->PostMessage(WM_COMMAND, ID_NEW_SEARCH, 0);
}

void CToolbarDlg::OnTrayDevpage()
{
	LPCWSTR url = L"http://www.mdiwebma.com/webma2/board/?id=14";
	ShellExecute(m_hWnd, L"open", url, NULL, NULL, SW_SHOWNORMAL);
}
