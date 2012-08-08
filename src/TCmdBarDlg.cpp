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
 *	트레이영역이 새로 만들어질 경우에 아이콘을 다시 생기하기위해서 메시지를 받는다.
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
     *  토탈 커맨드 폴더바를 초기화한다..
     */
    /// 태스크바에 안 나타나게 한다.
    //  
    ModifyStyleEx(WS_EX_APPWINDOW, 0);
    SetWindowPos(&wndTopMost, 0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	

    /// 툴바를 생성하고 초기화한다.
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
     *	사용자 마우스 Ctrl과 마우스 왼쪽을 누르면 창을 움직이게 한다.
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
 *	왼쪽 클릭했을 때 
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
     *	Ctrl이 눌려져 있지 않다면, 서브메뉴를 출력하도록 한다.
     */
    if( ! bCtrl && ShowSubfolder(iIndex,  true))
    {
         return 0;
    }


    /**
     *	Ctrl이 눌려져 있거나
     *  서브메뉴가하나도 없다면(보통 버튼) 프로그램 메뉴를 띄운다.
     */
    CMenu menu;
    CPoint point;

    GetCursorPos(&point);
    menu.CreatePopupMenu(); 
    menu.AppendMenu(MF_STRING, 100, L"수정");
	menu.AppendMenu(MF_STRING, 101, L"삭제");
    menu.AppendMenu(MF_STRING, 104, L"토탈커맨드창에 달라붙기");
    menu.AppendMenu(MF_STRING, 103, L"도움말");
	menu.AppendMenu(MF_STRING, 102, L"토탈커맨드 폴더바 종료");

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
    //삭제
    if(101 == iCmd)
    {
        //iIndex가 삭제될 것이다..
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
 * 툴바에서 폴더를 드랍하였을때 날려오는 사용자 정의 메시지 핸들러
 * @param   wp      (int)wp 드랍한 버튼의 Index
 * @param   lp      wp와 동일
 * @return
 */
LRESULT CTCmdBarDlg::OnUserDropFiles(WPARAM wp, LPARAM lp)
{   
    SetForegroundWindow();
    //드랍된 버튼 index
    int iIndex = (int)wp;
    CRect rc;
    m_bar.GetItemRect(iIndex, &rc);
    m_bar.ClientToScreen(&rc);

    int iTotalSize = GetPrivateProfileInt(L"main", L"size",0,INI_FILE);
    /**
     *	적어도 하나의 버튼이 있다면 옆에 넣을지 팝업메뉴에 넣을지를 결정할 수 있다.
     */
    if(iTotalSize>0)
    {    
        CPoint pt;
        GetCursorPos(&pt);
        CMenu mnuPopup;
        mnuPopup.CreatePopupMenu();
        mnuPopup.AppendMenu(MF_STRING, 1, (TCHAR*)L"옆에 버튼으로 추가");
        mnuPopup.AppendMenu(MF_STRING, 2, (TCHAR*)L"아래 메뉴로 추가");
        int iCmd = mnuPopup.TrackPopupMenu(TPM_RETURNCMD, pt.x, pt.y, this, NULL);
        if(0 == iCmd)
            return 0;
    
        if(2 == iCmd)
        {
            /**
             *	하위메뉴에 추가하는 다이얼로그를 띄워서 설정한다.
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
     *	iIndex오른쪽에 새로운 버튼을 추가한다
     * 
     *  iIndex뒤의 폴더정보를 한 칸씩 뒤로 저장한다.
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
    
    //< 새 버튼의 정보를 ini에 쓴다.
    CDSIni ini(Int2Str(iNewIndex), INI_FILE);
    ini.WriteStr(L"sFolderPath", g_sDropFolderPath);
    int rpos = g_sDropFolderPath.ReverseFind(L'\\');
    CString sText = g_sDropFolderPath.Mid(rpos+1);
    ini.WriteStr(L"sButtonText", sText);
    ini.WriteStr(L"subkey", GetSubkey());

    //< 전체 버튼 사이즈를 갱신한다.
    ini.Init(L"main", INI_FILE);
    ini.WriteInt(L"size", iTotalSize+1);
    
    /**
     *  이미 저장한 버튼에 대한 수정창을 띄운다.	
     */
    CDlgConfigFolder dlg(iNewIndex, rc);
    if(dlg.DoModal()== IDOK)
    {
        LoadIni(&m_bar);
        RecalSize(false);
    }else
    {
        // 수정창을 취소했다면 다시 버튼정보를 ini에서 지운다.
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
 *	    태스크바가 소멸됬다 생성되면 다시 보이게 한다.
 */
LRESULT CTCmdBarDlg::OnUserTrayCreated(WPARAM wp, LPARAM  lp)
{
    if(INI_INT(L"bTray", 1) == 1)
        m_tray.ShowIcon();
    return 1;
}

/**
 * 트레이에서 사용자의 메시지가 발생할 경우에 호출된다.
 * @param 
 * @param 
 * @return
 */
LRESULT CMyTray::OnCallback(WPARAM wp, LPARAM lp)
{
    return ((CTCmdBarDlg*)m_pParent)->OnUserOnTray(wp,lp);
}
/**
 *	     트레이에서 마우스 클릭이나 우클릭시에 팝업메뉴를 띄우기
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
        menu.AppendMenu(MF_STRING, IDM_ABOUTBOX, L"정보");   //1 
        menu.AppendMenu(MF_STRING, IDOK, L"토탈커맨드 폴더바 종료");   //1         
        menu.TrackPopupMenu( TPM_LEFTALIGN, point.x, point.y,this, NULL);        
        PostMessage(WM_NULL, 0, 0);
    }
    return 1;
}

/**
 *	    종료시에 위치를 저장합니다..
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
 *	폴더바를 숨긴다.
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
        //  사용자가 이동하고 나서 마우스 Lbuton을 놓으면 토커바의 새 위치를 저장한다.
        //
        s_bWritePositon = 1;
    }else if(s_bWritePositon == 1 && !ISKEYDOWN(VK_LBUTTON))
    {
        WriteGluePosition();
        s_bWritePositon = 0;
    }

	
    /*
     *	50밀리 타이머로 현재 토탈커맨드가 활성화 되어 있는지에 따라서
     *  토커바를 보이고 감추고 한다.
     */
    HWND hWnd = ::GetForegroundWindow();
    TCHAR szClass[MAX_PATH];
    GetClassName(hWnd, szClass, MAX_PATH);
    if(_tcsicmp(szClass, L"ttotal_cmd") == 0)
    {  
        /*
         *	토커바가 토커에 달라붙어 움직여야 한다면 위치를 찾아서 이동시킨다.
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
         *	토커가 회면에 보이고, 최소화가 아니면 그 위에 토커바를 띄운다.
         */
        if( ! ::IsWindowVisible(m_hWnd) && !::IsIconic(hWnd))
        {
            ShowWindow(SW_SHOW);
            SetWindowPos(&wndTopMost, 0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
        }else 
            if(::IsIconic(hWnd))    ///토커바가 최소화되어 있으면 숨긴다.
                _Hide();

    }else{
        /*
         *	활성창이 포커바가 아니거나, 최소화 되어 있으면 바를 숨긴다.
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
 *	하위 팝업메뉴에 폴더를 추가하는 함수
 */
bool CTCmdBarDlg::AddSubMenu(int iIndex, CString sPath, CRect rc)
{
    CString s = Int2Str(iIndex);
    CDSIni  ini(s, INI_FILE);
    CString sSection = ini.GetStr(L"subkey", L"");
    CDSIni subIni(sSection, SUBFOLDERINI_FILE);
    int iSubIndex = -1;
    /**
     *	비어있는 Entry를 찾는다.
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
     *	엔트리를 찾으면 거기에 서브메뉴 추가창을 띄운다.
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
 * 서브 폴더를 전환한다
 * @param iIndex 툴 버튼의 Index
 * @param  bRightButton  마우스 우클릭이면 true
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

    //왼쪽 클릭일 경우에는.. 
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
            //  전에 출력했던 메뉴 데이타를 map에서 삭제한다.
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
                    d2._sText = L"[순서/삭제/제목변경]";
                    TMenuData_Set(1000, d2);
                    mnuPopup.AppendMenu(MF_STRING|MF_OWNERDRAW, 1000, (TCHAR*)NULL);
                }
            }

            /**
             *	TMenuData에서 정보를 가져와서 팝업메뉴를 꾸민다.
             */
            int iRetCmd = mnuPopup.TrackPopupMenu(TPM_RETURNCMD , pt.x, pt.y, this);
            if(iRetCmd != 0)
            {
                if(iRetCmd == 1000)
                {
                    /**
                     *	수정 변경 차을 띄운다.
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
                     *	Ctrl과 같이 선택하면 그 아이템을 삭제한다.
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
 *  토탈커맨드에서 툴바가 얼마나의 상대위치로 있는지를 저장한다.	
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
 * 다이얼로그위에 툴바를 올리다보니 MFC프레임웍에 의해서 툴바가 밀리는 현상이 있는데
 * 이름 보정하기위해서 특정 윈도우 매시지에 매번 설정을 모두 다시 초기화한다.
 * 좀 무식한 방법이기는 하지만 별 문제는 없을 듯.
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
        bNewLoad = TRUE;    ///< 다음에 회면에 보일때 툴바를 다시 초기화한다.
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
///< 팝업메뉴 핸들러 1
void CTCmdBarDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if(nIDCtl == 0)
    {
        TMenuData_OnDrawItem(nIDCtl, lpDrawItemStruct);
	    return;
    }

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
///< 팝업메뉴 핸들러 2
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
