// StaticEx.cpp : implementation file
//

#include "stdafx.h"
#include "EasyRegistry.h"
#include "StaticEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticEx

CStaticEx::CStaticEx()
{
    b = false;
}

CStaticEx::~CStaticEx()
{
}


BEGIN_MESSAGE_MAP(CStaticEx, CStatic)
	//{{AFX_MSG_MAP(CStaticEx)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticEx message handlers

void CStaticEx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	

	CRect rc;
    GetClientRect(&rc);
    if(b)
        dc.FillSolidRect(&rc, col);
    else
        dc.DrawText(CString("»ö¾øÀ½"),&rc, DT_VCENTER );
	
	// Do not call CStatic::OnPaint() for painting messages
}
