// TCmdBar.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "EasyRegistry.h"
#include "Dialog/ToolbarDlg.h"
//#include <dsUtil.h>
#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEasyRegistryApp

BEGIN_MESSAGE_MAP(CEasyRegistryApp, CWinApp)
	//{{AFX_MSG_MAP(CEasyRegistryApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEasyRegistryApp construction

CEasyRegistryApp::CEasyRegistryApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CEasyRegistryApp object

CEasyRegistryApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEasyRegistryApp initialization

BOOL CEasyRegistryApp::InitInstance()
{	
	BOOL isLauncher = _GetInt(L"launcher", 1);
	if ( isLauncher )
	{
		TCHAR path[MAX_PATH] = { NULL };
		GetWindowsDirectory(path, _countof(path));
		PathAppend(path, L"RegEdit.exe");
		ShellExecute(NULL, L"open", path, NULL, NULL, SW_NORMAL);
	}


    if( !CheckMutextInstance(L"regutil_mutex") )
    {
		if ( ! isLauncher )
		{
			AfxMessageBox(L"EasyRegistry 프로그램이 이미 실행중입니다.", MB_ICONWARNING);
		}
        return FALSE;
    }

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

     //이 부분을 추가
    CWnd m_wndInvisible;
    if (!::IsWindow(m_wndInvisible.m_hWnd))
    {
        LPCTSTR pstrOwnerClass = AfxRegisterWndClass(0);
        if (!m_wndInvisible.CreateEx(0,pstrOwnerClass , _T(""), WS_POPUP ,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, 0))
            return FALSE;
    }

    HICON hIco = LoadIcon(MAKEINTRESOURCE(IDR_MAINFRAME));
    m_wndInvisible.SetIcon(hIco, TRUE);
    m_wndInvisible.SetIcon(hIco, FALSE);


	CToolbarDlg dlg(&m_wndInvisible);
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
