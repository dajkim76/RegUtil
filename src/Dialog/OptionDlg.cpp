// OptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EasyRegistry.h"
#include "OptionDlg.h"
#include <shlwapi.h>


// COptionDlg dialog

IMPLEMENT_DYNAMIC(COptionDlg, CDialog)

COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent)
{

}

COptionDlg::~COptionDlg()
{
}

void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
END_MESSAGE_MAP()


// COptionDlg message handlers

BOOL COptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CheckDlgButton(IDC_CHECK_GLUE, _GetInt(L"bGlue", 1) == 1 ? BST_CHECKED : BST_UNCHECKED);

	CString launcher = _GetString(L"launcher");
	CheckDlgButton(IDC_CHECK_LAUNCHER, PathFileExists(launcher) ? BST_CHECKED : BST_UNCHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void COptionDlg::OnOK()
{
	_WriteInt(L"bGlue", IsDlgButtonChecked(IDC_CHECK_GLUE) == BST_CHECKED);
	if ( IsDlgButtonChecked(IDC_CHECK_LAUNCHER) == BST_CHECKED )
	{
		TCHAR path[MAX_PATH] = { NULL };
		GetWindowsDirectory(path, _countof(path));
		PathAppend(path, L"RegEdit.exe");
		_WriteString(L"launcher", path);
	}
	else
	{
		_WriteString(L"launcher", L"");
	}

	CDialog::OnOK();
}
