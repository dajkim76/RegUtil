#if !defined(AFX_DLGRENAME_H__E504C8EB_3179_4030_BF45_DC79F9CAD1A8__INCLUDED_)
#define AFX_DLGRENAME_H__E504C8EB_3179_4030_BF45_DC79F9CAD1A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRename.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRename dialog

class CDlgRename : public CDialog
{
// Construction
public:
	CDlgRename(CString s,CWnd* pParent = NULL);   // standard constructor
    CString _s;
// Dialog Data
	//{{AFX_DATA(CDlgRename)
	enum { IDD = IDLG_RENAME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRename)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRename)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRENAME_H__E504C8EB_3179_4030_BF45_DC79F9CAD1A8__INCLUDED_)
