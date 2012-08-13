// DlgRename.cpp : implementation file
//

#include "stdafx.h"
#include "EasyRegistry.h"
#include "DlgRename.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRename dialog


CDlgRename::CDlgRename(CString s,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRename::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRename)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    _s = s;
}


void CDlgRename::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRename)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRename, CDialog)
	//{{AFX_MSG_MAP(CDlgRename)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRename message handlers

BOOL CDlgRename::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDlgItemText(IDC_EDIT1, _s);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRename::OnOK() 
{
	GetDlgItemText(IDC_EDIT1, _s);
    if(_s.IsEmpty())
    {
        AfxMessageBox(L"비어있습니다.");
        return;
    }
	
	CDialog::OnOK();
}
