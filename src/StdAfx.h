// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__9D958C69_1EDE_458B_817B_B30CC584A7E7__INCLUDED_)
#define AFX_STDAFX_H__9D958C69_1EDE_458B_817B_B30CC584A7E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include <dslib.h>
#include "dsIni.h"
#include "dsCommon.h"
#include "dsTray.h"
template<class T>
inline CString Int2Str(T  i )
{
    CString s; s.Format("%d", i); return s;
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__9D958C69_1EDE_458B_817B_B30CC584A7E7__INCLUDED_)
