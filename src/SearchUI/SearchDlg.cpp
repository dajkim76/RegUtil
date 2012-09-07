// SearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EasyRegistry.h"
#include "SearchDlg.h"
#include "RegWorks\RegWorks.h"


// CSearchDlg dialog

IMPLEMENT_DYNAMIC(CSearchDlg, CDialog)

CSearchDlg::CSearchDlg(SearchOption& option, CWnd* pParent /*=NULL*/)
	: CDialog(CSearchDlg::IDD, pParent)
	, option_(option)
{

}

CSearchDlg::~CSearchDlg()
{
}

void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSearchDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_CUSTOMKEY, &CSearchDlg::OnBnClickedCheckCustomkey)
END_MESSAGE_MAP()




// CSearchDlg message handlers

BOOL CSearchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

#define __C(id,val) CheckDlgButton(id, val ? BST_CHECKED : BST_UNCHECKED )
	__C(IDC_CHECK_HKLM2, option_.searchHKLM_ );
	__C(IDC_CHECK_HKCU, option_.searchHKCU_ );
	__C(IDC_CHECK_HKCR, option_.searchHKCR_);
	__C(IDC_CHECK_HKUSERS, option_.searchHKUSERS_);
	__C(IDC_CHECK_HKCC, option_.searchHKCONFIG_);

	__C(IDC_CHECK_KEY, option_.keyCheck_);
	__C(IDC_CHECK_VALUE, option_.nameCheck_);
	__C(IDC_CHECK_DATA, option_.valueCheck_);
	__C(IDC_CHECK_CASE, option_.caseSenstive_);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSearchDlg::OnOK()
{
	CString str;
	GetDlgItemText(IDC_EDIT1, str);
	if( !str.GetLength())
	{
		AfxMessageBox(L"검색어가 없습니다.");
		return;
	}

	if ( str.GetLength() == 1)
	{
		int ret = AfxMessageBox(L"검색어가 한 글자입니다\n시간이 오래걸릴 수 있습니다.\n\n계속하겠습니까?", MB_OKCANCEL);
		if( ret == IDCANCEL )
		{
			return ;
		}
	}

	bool isCustom = (IsDlgButtonChecked(IDC_CHECK_CUSTOMKEY) == BST_CHECKED);
	if (isCustom )
	{
		CString customkey;
		GetDlgItemText(IDC_EDIT_CUSTOMKEY, customkey);
		if( customkey.IsEmpty())
		{
			return ;
		}

		if( KeyRoot::toType(customkey) == KeyRoot::UNKNOWN)
		{
			AfxMessageBox(L"커스텀키가 유효하지 않습니다.\n레지스트리 root키로 시작해야 합니다.\n\n예) HKEY_CURRENT_USER\\Software");
			return ;
		}

		option_.customkey_ = customkey;
	}


	option_.keyword_ = str;
	
#undef __C
#define __C(id,val) val = (IsDlgButtonChecked(id) == BST_CHECKED )
	__C(IDC_CHECK_HKLM2, option_.searchHKLM_ );
	__C(IDC_CHECK_HKCU, option_.searchHKCU_ );
	__C(IDC_CHECK_HKCR, option_.searchHKCR_);
	__C(IDC_CHECK_HKUSERS, option_.searchHKUSERS_);
	__C(IDC_CHECK_HKCC, option_.searchHKCONFIG_);

	__C(IDC_CHECK_KEY, option_.keyCheck_);
	__C(IDC_CHECK_VALUE, option_.nameCheck_);
	__C(IDC_CHECK_DATA, option_.valueCheck_);
	__C(IDC_CHECK_CASE, option_.caseSenstive_);


	CDialog::OnOK();
}
void CSearchDlg::OnBnClickedCheckCustomkey()
{
	bool isCustom = (IsDlgButtonChecked(IDC_CHECK_CUSTOMKEY) == BST_CHECKED);

#undef __C
#define __C(id,val) GetDlgItem(id)->EnableWindow(val)
	__C(IDC_CHECK_HKLM2, !isCustom);
	__C(IDC_CHECK_HKCU, !isCustom);
	__C(IDC_CHECK_HKCR, !isCustom);
	__C(IDC_CHECK_HKUSERS, !isCustom);
	__C(IDC_CHECK_HKCC, !isCustom);

	__C(IDC_EDIT_CUSTOMKEY, isCustom);

	if ( isCustom )
	{
		GetDlgItem(IDC_EDIT_CUSTOMKEY)->SetFocus();
	}
}
