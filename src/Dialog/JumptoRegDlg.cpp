// JumptoReg.cpp : implementation file
//

#include "stdafx.h"
#include "EasyRegistry.h"
#include "JumptoRegDlg.h"
#include "Util\Common.h"
#include "RegWorks\RegWorks.h"


// CJumptoReg dialog

IMPLEMENT_DYNAMIC(CJumptoRegDlg, CDialog)

CJumptoRegDlg::CJumptoRegDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJumptoRegDlg::IDD, pParent)
{

}

CJumptoRegDlg::~CJumptoRegDlg()
{
}

void CJumptoRegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CJumptoRegDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CJumptoRegDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CJumptoReg message handlers

void CJumptoRegDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_EDIT1, path_);
	if( path_.GetLength() )
	{
		OnOK();
	}
}

BOOL CJumptoRegDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString text;

	if ( GetClipboardText(m_hWnd, text) )
	{
		if( RegWorks::IsValidPath(text) )
		{
			RegWorks::Validate(text);
			SetDlgItemText(IDC_EDIT1, text);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
