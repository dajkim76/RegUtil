// TCmdBarDlg.h : header file
//

#if !defined(AFX_TCMDBARDLG_H__5C661487_5A2C_4D8D_9DE6_1EE225A47F33__INCLUDED_)
#define AFX_TCMDBARDLG_H__5C661487_5A2C_4D8D_9DE6_1EE225A47F33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FolderToolbar.h"
#include "dsTray.h"
/////////////////////////////////////////////////////////////////////////////
// CTCmdBarDlg dialog
class CMyTray : public CDSTray
{
public:
    virtual LRESULT OnCallback(WPARAM wp, LPARAM lp) ;
};
class CTCmdBarDlg : public CDialog
{
// Construction
public:
	bool ShowSubfolder(int iIndex, bool bRightButton);
	bool AddSubMenu(int iIndex, CString sPath, CRect rc);
	void OnCancel();
	void RecalSize(bool bLoadPos);
    void WriteGluePosition();
	CTCmdBarDlg(CWnd* pParent = NULL);	// standard constructor
    LRESULT CTCmdBarDlg::OnUserOnTray(WPARAM wp, LPARAM lp);

// Dialog Data
	//{{AFX_DATA(CTCmdBarDlg)
	enum { IDD = IDD_TCMDBAR_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
    CFolderToolbar    m_bar;
    void    OnButtonCommandRange(UINT ids);
    void    _Hide();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTCmdBarDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	HICON       m_hIcon;
    CMyTray     m_tray;

	// Generated message map functions
	//{{AFX_MSG(CTCmdBarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg LRESULT OnUserRButtonDown(WPARAM wp, LPARAM lp);
    afx_msg LRESULT OnUserDropFiles(WPARAM wp, LPARAM lp);
    afx_msg LRESULT OnUserTrayCreated(WPARAM wp, LPARAM lp);
    afx_msg void    OnAbout();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCMDBARDLG_H__5C661487_5A2C_4D8D_9DE6_1EE225A47F33__INCLUDED_)
