// TCmdBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TCmdBar.h"
#include "TCmdBarDlg.h"
#include "DlgConfigFolder.h"
#include "TMenuData.h"
#include "TCmdUtil.h"
#include "DlgSort.h"

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

CTCmdBarDlg::CTCmdBarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTCmdBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTCmdBarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCmdBarDlg::DoDataExchange(CDataExchange* pDX)
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

BEGIN_MESSAGE_MAP(CTCmdBarDlg, CDialog)
	//{{AFX_MSG_MAP(CTCmdBarDlg)
    ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_MESSAGE(WMUM_RDOWN, OnUserRButtonDown)
    ON_MESSAGE(WMUM_DROPFILES, OnUserDropFiles)
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
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTCmdBarDlg message handlers


BOOL CTCmdBarDlg::OnInitDialog()
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
        m_tray.Create(this, L"RegUtil 0.9", m_hIcon);
    }
    SetTimer(101, 50, NULL);
    ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CTCmdBarDlg::OnAbout()
{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
}
void CTCmdBarDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTCmdBarDlg::OnPaint() 
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
HCURSOR CTCmdBarDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CTCmdBarDlg::PreTranslateMessage(MSG* pMsg) 
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
void CTCmdBarDlg::OnButtonCommandRange(UINT ids)
{
    int iIndex = ids - 100;
    ShowSubfolder(iIndex, false);
}   

LRESULT CTCmdBarDlg::OnUserRButtonDown(WPARAM wp, LPARAM lp)
{
    int iIndex = (INT)wp;
    int bCtrl = (INT)lp;

    /**
     *	Ctrl�� ������ ���� �ʴٸ�, ����޴��� ����ϵ��� �Ѵ�.
     */
    if( ! bCtrl && ShowSubfolder(iIndex,  true))
    {
         return 0;
    }


    /**
     *	Ctrl�� ������ �ְų�
     *  ����޴����ϳ��� ���ٸ�(���� ��ư) ���α׷� �޴��� ����.
     */
    CMenu menu;
    CPoint point;

    GetCursorPos(&point);
    menu.CreatePopupMenu(); 
    menu.AppendMenu(MF_STRING, 100, L"����");
	menu.AppendMenu(MF_STRING, 101, L"����");
    menu.AppendMenu(MF_STRING, 104, L"��ŻĿ�ǵ�â�� �޶�ٱ�");
    menu.AppendMenu(MF_STRING, 103, L"����");
	menu.AppendMenu(MF_STRING, 102, L"��ŻĿ�ǵ� ������ ����");

    int bGlue = GetPrivateProfileInt(L"main", L"bGlue", 1, INI_FILE);
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
    if(104 == iCmd)
    {
      bGlue = !bGlue;
      WritePrivateProfileString(L"main", L"bGlue", Int2Str(bGlue), INI_FILE);
    }    
    if(100 == iCmd)
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
    if(101 == iCmd)
    {
        //iIndex�� ������ ���̴�..
        CString sSubkey = g_arButtonData[iIndex].sSubkey;
        WritePrivateProfileString(sSubkey,NULL,NULL,SUBFOLDERINI_FILE);

        int iTotalSize = INI_INT(L"size", 0);
        if(iTotalSize > 0)
        {        
            for(int i=iIndex+1; i<=iTotalSize-1; i++ )
            {
                dsSectionToSection(Int2Str(i), Int2Str(i-1), INI_FILE, false );
            }
            WritePrivateProfileString(L"main",L"size",Int2Str(iTotalSize-1), INI_FILE);
        }
        LoadIni(&m_bar);
        RecalSize(false);
    }
    if(103 == iCmd)
    {
        ShellExecute(m_hWnd, _T("open"), dsRunningPath(L"TCmdBar.txt"), NULL, NULL, SW_MAXIMIZE);
    }
    return 1;
}

void CTCmdBarDlg::RecalSize(bool bLoadPos)
{
    /// Resize Dialog & Toolbar
    //  
    CSize sizeToolbar = m_bar.CalcFixedLayout(false,true);
    CRect rc;
    int iCount = m_bar.GetToolBarCtrl().GetButtonCount();
    m_bar.GetItemRect(iCount-1, &rc);
    m_bar.MoveWindow(-7,-5,rc.right+6, sizeToolbar.cy+7);

    int ix = 0;
    int iy = 0;
    if(bLoadPos)
    {    
        ix = GetPrivateProfileInt(L"main", L"x", 100, INI_FILE);
        iy = GetPrivateProfileInt(L"main", L"y", 100, INI_FILE);
    }
    
    SetWindowPos(&wndTopMost ,ix,
                              iy,
                              rc.right-2, 
                              sizeToolbar.cy-10, 
                              SWP_SHOWWINDOW | (bLoadPos==true ? 0 : SWP_NOMOVE));                              
    
}




/**
 * ���ٿ��� ������ ����Ͽ����� �������� ����� ���� �޽��� �ڵ鷯
 * @param   wp      (int)wp ����� ��ư�� Index
 * @param   lp      wp�� ����
 * @return
 */
LRESULT CTCmdBarDlg::OnUserDropFiles(WPARAM wp, LPARAM lp)
{   
    SetForegroundWindow();
    //����� ��ư index
    int iIndex = (int)wp;
    CRect rc;
    m_bar.GetItemRect(iIndex, &rc);
    m_bar.ClientToScreen(&rc);

    int iTotalSize = GetPrivateProfileInt(L"main", L"size",0,INI_FILE);
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
            dsSectionToSection(Int2Str(i), Int2Str(i+1), INI_FILE, false);
        }
    }
    
    int iNewIndex = iIndex+1;
    if(iTotalSize ==0)
        iNewIndex = 0;
    
    //< �� ��ư�� ������ ini�� ����.
    CDSIni ini(Int2Str(iNewIndex), INI_FILE);
    ini.WriteStr(L"sFolderPath", g_sDropFolderPath);
    int rpos = g_sDropFolderPath.ReverseFind(L'\\');
    CString sText = g_sDropFolderPath.Mid(rpos+1);
    ini.WriteStr(L"sButtonText", sText);
    ini.WriteStr(L"subkey", GetSubkey());

    //< ��ü ��ư ����� �����Ѵ�.
    ini.Init(L"main", INI_FILE);
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
            dsSectionToSection(Int2Str(i), Int2Str(i-1), INI_FILE, false );
        }
        ini.WriteInt(L"size", iTotalSize);
    }
    return 1;
}

/**
 *	    �½�ũ�ٰ� �Ҹ��� �����Ǹ� �ٽ� ���̰� �Ѵ�.
 */
LRESULT CTCmdBarDlg::OnUserTrayCreated(WPARAM wp, LPARAM  lp)
{
    if(INI_INT(L"bTray", 1) == 1)
        m_tray.ShowIcon();
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
    return ((CTCmdBarDlg*)m_pParent)->OnUserOnTray(wp,lp);
}
/**
 *	     Ʈ���̿��� ���콺 Ŭ���̳� ��Ŭ���ÿ� �˾��޴��� ����
 */
LRESULT CTCmdBarDlg::OnUserOnTray(WPARAM wp, LPARAM lp)
{
    if(lp == WM_LBUTTONUP || lp == WM_RBUTTONUP)
    {        
    	CPoint	point;
        GetCursorPos(&point);
        SetForegroundWindow();
        
        CMenu menu;
        menu.CreatePopupMenu(); 
        menu.AppendMenu(MF_STRING, IDM_ABOUTBOX, L"����");   //1 
        menu.AppendMenu(MF_STRING, IDOK, L"��ŻĿ�ǵ� ������ ����");   //1         
        menu.TrackPopupMenu( TPM_LEFTALIGN, point.x, point.y,this, NULL);        
        PostMessage(WM_NULL, 0, 0);
    }
    return 1;
}

/**
 *	    ����ÿ� ��ġ�� �����մϴ�..
 */
void CTCmdBarDlg::OnDestroy() 
{
    CWindowRect rc(this);    
    CDSIni ini(_T("main"), INI_FILE);
    ini.WriteInt(_T("x"), rc.left);
    ini.WriteInt(_T("y"), rc.top);		    
	CDialog::OnDestroy();
}


/**
 *	�����ٸ� �����.
 */
void CTCmdBarDlg::_Hide()
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


void CTCmdBarDlg::OnTimer(UINT nIDEvent) 
{
    static BOOL s_bWritePositon = 0;
    if(ISKEYDOWN(VK_CONTROL) && ISKEYDOWN(VK_LBUTTON))
    {
        //  ����ڰ� �̵��ϰ� ���� ���콺 Lbuton�� ������ ��Ŀ���� �� ��ġ�� �����Ѵ�.
        //
        s_bWritePositon = 1;
    }else if(s_bWritePositon == 1 && !ISKEYDOWN(VK_LBUTTON))
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
    if(_tcsicmp(szClass, L"ttotal_cmd") == 0)
    {  
        /*
         *	��Ŀ�ٰ� ��Ŀ�� �޶�پ� �������� �Ѵٸ� ��ġ�� ã�Ƽ� �̵���Ų��.
         */
        int bGlue = GetPrivateProfileInt(L"main", L"bGlue", 1, INI_FILE);    
        if(bGlue && !::IsIconic(hWnd))
        {           
            int gx = GetPrivateProfileInt(L"main", L"iGlueX", 100, INI_FILE);
            int gy = GetPrivateProfileInt(L"main", L"iGlueY", 100, INI_FILE);

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

    }else{
        /*
         *	Ȱ��â�� ��Ŀ�ٰ� �ƴϰų�, �ּ�ȭ �Ǿ� ������ �ٸ� �����.
         */
        if(hWnd != m_hWnd || ::IsIconic(hWnd))
            _Hide();
    }
    
	CDialog::OnTimer(nIDEvent);
}


void CTCmdBarDlg::OnCancel()
{

}

/**
 *	���� �˾��޴��� ������ �߰��ϴ� �Լ�
 */
bool CTCmdBarDlg::AddSubMenu(int iIndex, CString sPath, CRect rc)
{
    CString s = Int2Str(iIndex);
    CDSIni  ini(s, INI_FILE);
    CString sSection = ini.GetStr(L"subkey", L"");
    CDSIni subIni(sSection, SUBFOLDERINI_FILE);
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
bool CTCmdBarDlg::ShowSubfolder(int iIndex, bool bRightButton)
{
    if(iIndex >= g_arButtonData.GetSize())
        return false;

    CButtonData item = g_arButtonData[iIndex];

    CPoint pt;
    GetCursorPos(&pt);
    CRect rc;
    m_bar.GetItemRect(iIndex, &rc);
    m_bar.ClientToScreen(&rc);
    rc.right =  rc.left + 22;

    //���� Ŭ���� ��쿡��.. 
    if( ! bRightButton )
    {
        if( item.iCommand != 0 )
            TCMD_SendID( item.iCommand);
        else
            TCMD_SendPath( item.sFolderPath );  
    }
    else
    {
        CStringArray ar;
        CStringArray arExt;
        CStringArray arKey;
        GetSubFolders(item.sSubkey,  arKey, ar, arExt);        
        if( ar.GetSize() ==0)
            return false;

        if( ar.GetSize() > 0)
        {
            //  ���� ����ߴ� �޴� ����Ÿ�� map���� �����Ѵ�.
            //
            TMenuData_Clear();

            CMenu mnuPopup;
            mnuPopup.CreatePopupMenu();
            for(int i=0;i<ar.GetSize(); i++)
            {
                CString s = ar[i];
                s = s.Left ( s.Find(L"|") );
                TMenuData data;
                data._sText = s;
                data._iHeight = 20;                
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
                    TMenuData d2;
                    d2._sText = L"[����/����/���񺯰�]";
                    TMenuData_Set(1000, d2);
                    mnuPopup.AppendMenu(MF_STRING|MF_OWNERDRAW, 1000, (TCHAR*)NULL);
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
                    CDlgSort dlg(item.sSubkey);
                    if(dlg.DoModal() == IDOK)
                    {
                        if(dlg.m_bChanged)
                        {
                            LoadIni(&m_bar);
                            RecalSize(false);
                        }
                    }
                }
                else if(ISKEYDOWN(VK_CONTROL) && iRetCmd>0)
                {
                    /**
                     *	Ctrl�� ���� �����ϸ� �� �������� �����Ѵ�.
                     */
                    CDSIni ini (item.sSubkey, SUBFOLDERINI_FILE);
                    int iKey = _ttoi(arKey.GetAt(iRetCmd-1));
                    ini.WriteStr(Int2Str(iKey), L""); //1 2[s] 3[s] 4[s]
                    LoadIni(&m_bar);
                    RecalSize(false);
                }else
                {                
                    CString sData = ar.GetAt(iRetCmd-1);
                    sData = sData.Mid( sData.ReverseFind(L'|') + 1);
                    TCMD_SendPath( sData );    
                }
            }            
        }            
    }

    return true;
}


/**
 *  ��ŻĿ�ǵ忡�� ���ٰ� �󸶳��� �����ġ�� �ִ����� �����Ѵ�.	
 */
void CTCmdBarDlg::WriteGluePosition()
{
    HWND hWndCmd = ::FindWindow(L"TTOTAL_CMD", NULL);
    if(hWndCmd && !::IsIconic(hWndCmd))
    {        
        CRect rc;
        ::GetWindowRect(hWndCmd, &rc);
        CRect rcMy;
        GetWindowRect(&rcMy);
        int gx = rcMy.left - rc.left;
        int gy = rcMy.top - rc.top;

        WritePrivateProfileString(L"main", L"iGlueX", Int2Str(gx), INI_FILE);
        WritePrivateProfileString(L"main", L"iGlueY", Int2Str(gy), INI_FILE);
    }    

}

/**
 * ���̾�α����� ���ٸ� �ø��ٺ��� MFC�����ӿ��� ���ؼ� ���ٰ� �и��� ������ �ִµ�
 * �̸� �����ϱ����ؼ� Ư�� ������ �Ž����� �Ź� ������ ��� �ٽ� �ʱ�ȭ�Ѵ�.
 * �� ������ ����̱�� ������ �� ������ ���� ��.
 */
BOOL bNewLoad = FALSE;
void CTCmdBarDlg::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CDialog::OnSettingChange(uFlags, lpszSection);
	
    if(IsWindowVisible())
    {
        LoadIni(&m_bar);
        RecalSize(false);	        
    }else
        bNewLoad = TRUE;    ///< ������ ȸ�鿡 ���϶� ���ٸ� �ٽ� �ʱ�ȭ�Ѵ�.
}

void CTCmdBarDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);	
}

void CTCmdBarDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
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
void CTCmdBarDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if(nIDCtl == 0)
    {
        TMenuData_OnDrawItem(nIDCtl, lpDrawItemStruct);
	    return;
    }

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
///< �˾��޴� �ڵ鷯 2
void CTCmdBarDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDCtl == 0)
    {
        TMenuData_OnMeasureItem(nIDCtl, lpMeasureItemStruct);
        return;
    }
    
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
