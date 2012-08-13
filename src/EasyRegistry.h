// TCmdBar.h : main header file for the TCMDBAR application
//

#if !defined(AFX_TCMDBAR_H__F2AD7BA3_7E47_4FB9_A9A7_393FE77E0B38__INCLUDED_)
#define AFX_TCMDBAR_H__F2AD7BA3_7E47_4FB9_A9A7_393FE77E0B38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTCmdBarApp:
// See TCmdBar.cpp for the implementation of this class
//

class CEasyRegistryApp : public CWinApp
{
public:
	CEasyRegistryApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTCmdBarApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTCmdBarApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCMDBAR_H__F2AD7BA3_7E47_4FB9_A9A7_393FE77E0B38__INCLUDED_)
