#if !defined(AFX_EDITSUBMENUDLG_H__00C22C11_B6F5_4F43_8BF9_CFB8BD63DD83__INCLUDED_)
#define AFX_EDITSUBMENUDLG_H__00C22C11_B6F5_4F43_8BF9_CFB8BD63DD83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSort.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSort dialog

class EditSubmenuDlg : public CDialog
{
// Construction
public:
	int getIndex();
	EditSubmenuDlg(CString sSection, CWnd* pParent = NULL);   // standard constructor

    CString _sSection;
// Dialog Data
	//{{AFX_DATA(CDlgSort)
	enum { IDD = IDD_EDIT_SUBMENU };
	CListCtrl	_list;
    bool    m_bChanged;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSort)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSort)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnUP();
	afx_msg void OnDown();
	afx_msg void OnDelete();
	afx_msg void OnRename();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITSUBMENUDLG_H__00C22C11_B6F5_4F43_8BF9_CFB8BD63DD83__INCLUDED_)