#pragma once
#include "RegWorks\RegSearch.h"
using namespace RegSearch;

// CSearchDlg dialog

class CSearchDlg : public CDialog
{
	DECLARE_DYNAMIC(CSearchDlg)

public:
	CSearchDlg(SearchOption& option, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSearchDlg();

// Dialog Data
	enum { IDD = IDD_SEARCH };

	SearchOption& option_;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckCustomkey();
};
