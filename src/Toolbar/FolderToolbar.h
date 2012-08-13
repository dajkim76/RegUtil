#if !defined(AFX_FOLDERTOOLBAR_H__B382297E_F8F3_4571_A96F_6043CE585676__INCLUDED_)
#define AFX_FOLDERTOOLBAR_H__B382297E_F8F3_4571_A96F_6043CE585676__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FolderToolbar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFolderToolbar window

#define WMUM_RDOWN      (WM_USER+100)
#define WMUM_DROPFILES  (WM_USER+200)
extern  CString g_sDropFolderPath;  ///< 폴더나 파일을 드랍했을때의 폴더패스가 저장됨.

class CFolderToolbar : public CToolBar
{
// Construction
public:
	CFolderToolbar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFolderToolbar)
	//}}AFX_VIRTUAL

// Implementation
public:
	CImageList m_il;
	virtual ~CFolderToolbar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFolderToolbar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOLDERTOOLBAR_H__B382297E_F8F3_4571_A96F_6043CE585676__INCLUDED_)
