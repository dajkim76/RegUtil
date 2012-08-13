
#include "Stdafx.h"
#include "Tray.h"
#include "Common.h"


const UINT CALLBACK_MSGID = WM_USER+1024;

BEGIN_MESSAGE_MAP(CDSTray, CWnd)
	ON_MESSAGE(CALLBACK_MSGID, OnCallback)
END_MESSAGE_MAP()
CDSTray::CDSTray()
{
	memset(&m_tnd, 0, sizeof(m_tnd));
	m_uiLButtonPopupmenu = 0;
	m_uiRButtonPopupmenu = 0;
}

LRESULT CDSTray::OnCallback(WPARAM wp, LPARAM lp)
{
	switch(lp)
	{
	case WM_LBUTTONUP:
		OnLButtonUp(); 
		break;

	case WM_RBUTTONUP:
		OnRButtonUp(); 
		break;

     case 1029:
//         AfxMessageBox("balloon click ");
     case 1028:
//         AfxMessageBox("balloon exit ");
         OnBalloonClick();
         break;

	}

	return 1;
}

CDSTray::CDSTray(CWnd* pWnd, LPCTSTR szToolTip, HICON icon)
{	
	Create(pWnd,  szToolTip, icon, true);
}

BOOL CDSTray::Create(CWnd* pWnd, LPCTSTR szToolTip, HICON icon, bool bDefaultShow)
{
	ASSERT(this->m_hWnd == 0);
	m_pParent = pWnd;

  
    PCTSTR pstrOwnerClass = ::AfxRegisterWndClass(0);
    
    BOOL bWndCreated = CWnd::CreateEx(
                      0,
                      pstrOwnerClass, 
                      _T(""), 
                      WS_POPUP,
		              CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT, 
                      CW_USEDEFAULT,
		              NULL,
                      0
                  );
                  
	//CWnd::Create(NULL,"", WS_CHILD,CRect(0,0,0,0), pWnd, 101);

	//Make sure we avoid conflict with other messages
	//ASSERT(uCallbackMessage >= WM_USER);

	//Tray only supports tooltip text up to 64 characters
	ASSERT(_tcslen(szToolTip) <= 64);

	// NOTIFYICONDATA 구조체의 값 설정
	m_tnd.cbSize = sizeof(NOTIFYICONDATA);
	m_tnd.hWnd	 = this->GetSafeHwnd();
	m_tnd.uID	 = s_uiTrayID ++ ;
	m_tnd.hIcon  = icon;
	myicon = icon;
	m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_tnd.uCallbackMessage = CALLBACK_MSGID;//uCallbackMessage;
	lstrcpy (m_tnd.szTip, szToolTip);

	if(bDefaultShow)
		ShowIcon();
	// 트레이 아이콘 추가
	return TRUE;
}

CDSTray::~CDSTray()
{
	RemoveIcon();
}


/////////////////////////////////////////////////////////////////////////////
// CDSTray icon manipulation

void CDSTray::RemoveIcon()
{
	m_tnd.uFlags = 0;
    Shell_NotifyIcon(NIM_DELETE, &m_tnd);
}


BOOL CDSTray::SetIcon(HICON hIcon)
{
	m_tnd.uFlags = NIF_ICON;
	m_tnd.hIcon = hIcon;

	return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

BOOL CDSTray::SetIcon(LPCTSTR lpszIconName)
{
	HICON hIcon = AfxGetApp()->LoadIcon(lpszIconName);

	return SetIcon(hIcon);
}

BOOL CDSTray::SetIcon(UINT nIDResource)
{
	HICON hIcon = AfxGetApp()->LoadIcon(nIDResource);

	return SetIcon(hIcon);
}

BOOL CDSTray::SetStandardIcon(LPCTSTR lpIconName)
{
	HICON hIcon = LoadIcon(NULL, lpIconName);

	return SetIcon(hIcon);
}

BOOL CDSTray::SetStandardIcon(UINT nIDResource)
{
	HICON hIcon = LoadIcon(NULL, MAKEINTRESOURCE(nIDResource));

	return SetIcon(hIcon);
}
 
HICON CDSTray::GetIcon() const
{
	HICON hIcon = NULL;
	hIcon = m_tnd.hIcon;

	return hIcon;
}

/////////////////////////////////////////////////////////////////////////////
// CDSTray tooltip text manipulation

BOOL CDSTray::SetTooltipText(LPCTSTR pszTip)
{
	m_tnd.uFlags = NIF_TIP;
	_tcscpy(m_tnd.szTip, pszTip);

	return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

BOOL CDSTray::SetTooltipText(UINT nID)
{
	CString strText;
	VERIFY(strText.LoadString(nID));

	return SetTooltipText(strText);
}

CString CDSTray::GetTooltipText() const
{
	CString strText;
	strText = m_tnd.szTip;

	return strText;
}

/////////////////////////////////////////////////////////////////////////////
// CDSTray notification window stuff

BOOL CDSTray::SetNotificationWnd(CWnd* pWnd)
{
	//Make sure Notification window is valid
	ASSERT(pWnd && ::IsWindow(pWnd->GetSafeHwnd()));

	m_tnd.hWnd = pWnd->GetSafeHwnd();
	m_tnd.uFlags = 0;

	return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

CWnd* CDSTray::GetNotificationWnd() const
{
	return CWnd::FromHandle(m_tnd.hWnd);
}

void CDSTray::ShowIcon(void)
{
	m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &m_tnd);
}

void CDSTray::HideIcon(void)
{
	RemoveIcon();
}

void CDSTray::OnLButtonUp()
{
	if(m_uiLButtonPopupmenu != 0)
	{
		CMenu Menu;
		Menu.LoadMenu(m_uiLButtonPopupmenu);
		CMenu* pMenu = Menu.GetSubMenu(0);
		
		m_pParent->SetForegroundWindow();
		CPointScreen pt;
		pMenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, m_pParent);		
	}
}
void CDSTray::OnRButtonUp()
{
	if(m_uiRButtonPopupmenu != 0)
	{
		CMenu Menu;
		Menu.LoadMenu(m_uiRButtonPopupmenu);
		CMenu* pMenu = Menu.GetSubMenu(0);
		
		m_pParent->SetForegroundWindow();
		CPointScreen pt;
		pMenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, m_pParent);		
	}
}

void CDSTray::OnBalloonClick()
{

}

UINT CDSTray::s_uiTrayID = 1024;

bool CDSTray::ShowBalloon(LPCTSTR szTitle, LPCTSTR szText, DWORD dwIcon, DWORD dwTimeOut)
{
    OSVERSIONINFO os = { sizeof(os) };
    GetVersionEx(&os);
    bool bWin2khigh = ( VER_PLATFORM_WIN32_NT == os.dwPlatformId && os.dwMajorVersion >= 5 );
    if( ! bWin2khigh)
        return false;

    m_tnd.uFlags = NIF_INFO;
    _tcsncpy(m_tnd.szInfo, szText, 256);
    if (szTitle)
        _tcsncpy(m_tnd.szInfoTitle, szTitle, 64);
    else
        m_tnd.szInfoTitle[0] = _T('\0');
    m_tnd.dwInfoFlags = dwIcon;
    m_tnd.uTimeout = dwTimeOut * 1000;   // convert time to ms

    BOOL bSuccess = Shell_NotifyIcon (NIM_MODIFY, &m_tnd);

    // Zero out the balloon text string so that later operations won't redisplay
    // the balloon.
    m_tnd.szInfo[0] = _T('\0');    

    return true;
}
