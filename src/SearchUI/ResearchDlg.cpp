// ResearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EasyRegistry.h"
#include "ResearchDlg.h"


// CResearchDlg dialog

IMPLEMENT_DYNAMIC(CResearchDlg, CDialog)

CResearchDlg::CResearchDlg(SearchOption& option, CWnd* pParent /*=NULL*/)
	: CDialog(CResearchDlg::IDD, pParent), option_(option)
{

}

CResearchDlg::~CResearchDlg()
{
}

void CResearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CResearchDlg, CDialog)
END_MESSAGE_MAP()


BOOL CResearchDlg::OnInitDialog()
{
#define __C(id,val) CheckDlgButton(id, val ? BST_CHECKED : BST_UNCHECKED )

	__C(IDC_CHECK_KEY, option_.keyCheck_);
	__C(IDC_CHECK_VALUE, option_.nameCheck_);
	__C(IDC_CHECK_DATA, option_.valueCheck_);
	__C(IDC_CHECK_CASE, option_.caseSenstive_);

	return TRUE;
}

void CResearchDlg::OnOK()
{
	CString str;
	GetDlgItemText(IDC_EDIT1, str);
	if( !str.GetLength())
	{
		return;
	}

	option_.keyword_ = str;


#undef __C
#define __C(id,val) val = (IsDlgButtonChecked(id) == BST_CHECKED )

	__C(IDC_CHECK_KEY, option_.keyCheck_);
	__C(IDC_CHECK_VALUE, option_.nameCheck_);
	__C(IDC_CHECK_DATA, option_.valueCheck_);
	__C(IDC_CHECK_CASE, option_.caseSenstive_);

	CDialog::OnOK();
}

// CResearchDlg message handlers
