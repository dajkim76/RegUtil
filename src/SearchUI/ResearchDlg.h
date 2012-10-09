#pragma once
#include "RegWorks\RegSearch.h"
using namespace RegSearch;


// CResearchDlg dialog

class CResearchDlg : public CDialog
{
	DECLARE_DYNAMIC(CResearchDlg)

public:
	CResearchDlg(SearchOption& option, CWnd* pParent = NULL);   // standard constructor
	virtual ~CResearchDlg();

// Dialog Data
	enum { IDD = IDD_RESEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	SearchOption& option_;

	virtual void OnOK();
	virtual BOOL OnInitDialog();
};
