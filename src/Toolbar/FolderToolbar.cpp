// FolderToolbar.cpp : implementation file
//

#include "stdafx.h"
#include "EasyRegistry.h"
#include "FolderToolbar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFolderToolbar

CFolderToolbar::CFolderToolbar()
{
}

CFolderToolbar::~CFolderToolbar()
{
}


BEGIN_MESSAGE_MAP(CFolderToolbar, CToolBar  )
	//{{AFX_MSG_MAP(CFolderToolbar)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFolderToolbar message handlers

int CFolderToolbar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    //////////////////////////////////////////////////////////////////////////
    //  이미지 리스트 설정
    if (!m_il.Create (16, 16, ILC_COLORDDB| ILC_COLOR32 | ILC_MASK, 2, 2))
		return -1;
	HBITMAP hbm = (HBITMAP)::LoadImage (AfxGetResourceHandle(),
						MAKEINTRESOURCE(IDB_BITMAP1),
						IMAGE_BITMAP, 0, 0, 
						LR_LOADMAP3DCOLORS);
    ImageList_AddMasked(m_il.m_hImageList, hbm, RGB(255,0,255));
    GetToolBarCtrl().SetImageList(&m_il);
    DeleteObject(hbm);
    
	return 0;
}




void CFolderToolbar::OnRButtonDown(UINT nFlags, CPoint point) 
{
    CToolBarCtrl &ctl = GetToolBarCtrl();
    int iIndex = ctl.HitTest(&point);
    if( iIndex >= 0 )
    {
        if(ISKEYDOWN(VK_CONTROL))
            GetParent()->PostMessage(WMUM_RDOWN, iIndex, 1);
        else
            GetParent()->PostMessage(WMUM_RDOWN, iIndex, 0); //show submenu
    }
	CToolBar  ::OnRButtonDown(nFlags, point);
}

/**
 *	    맨 처음 걸리는 드랍한 파일이나 폴더의 전체경로는 리턴한다.
 */
CString	GetFirstDropFilePath(HDROP hDropInfo)
{
	CString strTemp;
	TCHAR   szFilePath[512];      
    UINT    cFiles;
	UINT    u;

	cFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);   
	for (u = 0; u < cFiles; u++)    
	{        
		::DragQueryFile(hDropInfo, u, szFilePath, sizeof(szFilePath));        
		strTemp = szFilePath ;
        break;
	}      

    return strTemp;
}

/**
 *	파일을 드랍하였을때 
 *  몇번째 버튼에서 드랍했는지 iIndex
 *  어떤 폴더를 드랍했는지      g_sDropFolderPath
 *  를 구해서 다이얼로그 윈도우에 메시지를 전달한다.
 */
CString g_sDropFolderPath;
void CFolderToolbar::OnDropFiles(HDROP hDropInfo) 
{	
    CToolBarCtrl &ctl = GetToolBarCtrl();
    CPoint point;
    GetCursorPos(&point);
    ScreenToClient(&point);
    int iIndex = ctl.HitTest(&point);
    g_sDropFolderPath =  GetFirstDropFilePath(hDropInfo);
    if(g_sDropFolderPath.IsEmpty() == FALSE)
    {
        GetParent()->PostMessage(WMUM_DROPFILES, iIndex, iIndex);
    }
	
	CToolBar  ::OnDropFiles(hDropInfo);
}
