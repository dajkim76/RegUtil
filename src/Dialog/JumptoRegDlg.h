#pragma once


// CJumptoReg dialog

class CJumptoRegDlg : public CDialog
{
	DECLARE_DYNAMIC(CJumptoRegDlg)

public:
	CJumptoRegDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CJumptoRegDlg();

// Dialog Data
	enum { IDD = IDD_JUMPTOREG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	CString path_;
	virtual BOOL OnInitDialog();
};
