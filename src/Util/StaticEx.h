#if !defined(AFX_STATICEX_H__CE40B493_C707_497A_9A6E_230F2000C062__INCLUDED_)
#define AFX_STATICEX_H__CE40B493_C707_497A_9A6E_230F2000C062__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticEx window

class CStaticEx : public CStatic
{
// Construction
public:
	CStaticEx();

// Attributes
public:
    COLORREF    col;
    bool        b;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStaticEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticEx)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICEX_H__CE40B493_C707_497A_9A6E_230F2000C062__INCLUDED_)
