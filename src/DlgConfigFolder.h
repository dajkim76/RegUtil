#if !defined(AFX_DLGCONFIGFOLDER_H__DC17E6BE_E674_4726_A957_4B7FC0DF38CA__INCLUDED_)
#define AFX_DLGCONFIGFOLDER_H__DC17E6BE_E674_4726_A957_4B7FC0DF38CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConfigFolder.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgConfigFolder dialog
#include "StaticEx.h"
/**
 버튼 수정 창
*/
class CDlgConfigFolder : public CDialog
{
// Construction
public:
	CDlgConfigFolder(int iIndex, CRect rc, CWnd* pParent = NULL);   // standard constructor
    int _iIndex;
    CRect _rc;

// Dialog Data
	//{{AFX_DATA(CDlgConfigFolder)
	enum { IDD = IDLG_CONFIG_FOLDER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA    


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgConfigFolder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgConfigFolder)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/**
 하위 메뉴 추가 및 수정 창
*/
class CDlgConfigFolder_Sub : public CDialog
{
// Construction
public:
	CDlgConfigFolder_Sub(CRect rc, int iSubIndex, CString sFolderKey, CString sTargetPath, CWnd* pParent = NULL);   // standard constructor

    CStaticEx     st1, st2;
    void    OnCol1();
    void    OnCol2();
    void    OnCheck();
    void    MyEn(BOOL b);
    int _iSubIndex;
    CString _sFolderKey;
    CString _sTargetPath;
	CString _sTitle;
    CRect _rc;

// Dialog Data
	//{{AFX_DATA(CDlgConfigFolder)
	enum { IDD = IDLG_CONFIG_FOLDER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgConfigFolder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgConfigFolder)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONFIGFOLDER_H__DC17E6BE_E674_4726_A957_4B7FC0DF38CA__INCLUDED_)
