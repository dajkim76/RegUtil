// TMenuData.cpp: implementation of the TMenuData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EasyRegistry.h"
#include "MenuData.h"
#include <map>
using namespace std;


/**
 *  DrawItem으로 메뉴를 그리기위한 MenuData를 단순히 글로벌 map에 저장해 두는 방법을 사용한다
 *  왜냐면 사용하는 팝업메뉴는 하나뿐이고, 이렇게 하는 것이 구현이 가장 간단하다.
 *  
 *  데이타 크기도 작기때문에 포인터를 사용하지 않고, 객체를 그냥 집어넣어서 관리한다.
 *  동적할당하지 않기 때문에 프로그램 종료와 동시에 메모리도 잘 해제된다.
 */
typedef map<UINT, MenuData> TMenuDataMap;
TMenuDataMap  g_MenuDataMap;
void        TMenuData_Set(UINT nID, MenuData data)
{
    g_MenuDataMap[nID] = data;
}

bool   TMenuData_Get(UINT nID, MenuData& data)
{
    map<UINT, MenuData> ::iterator iter = g_MenuDataMap.find(nID);
    if(iter  == g_MenuDataMap.end())
        return false;
    data = iter->second;
    return true;
}

void   TMenuData_Clear()
{
    g_MenuDataMap.clear();
}


MenuData::~MenuData()
{

}

/************************************************************************
//
//  
//
*************************************************************************/
#define MB_CX_ICON	16
#define MB_CY_ICON	16

/**
 *	bBold를 true이면 두꺼운 폰트가 생성되도록 하는 클래스 확장..
 */
class	CFontEx : public  CFont
{public:
	BOOL CreatePointFont_2(int nPointSize,BOOL bBold, LPCTSTR lpszFaceName, CDC* pDC=NULL)
	{
		ASSERT(AfxIsValidString(lpszFaceName));

		LOGFONT logFont;
		memset(&logFont, 0, sizeof(LOGFONT));
		logFont.lfCharSet = DEFAULT_CHARSET;
		logFont.lfHeight = nPointSize;
		if(bBold)
			logFont.lfWeight = FW_BOLD;
		lstrcpy(logFont.lfFaceName, lpszFaceName);//, _countof(logFont.lfFaceName));

		return CreatePointFontIndirect(&logFont, pDC);
	}
};


void TMenuData_OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	UINT nID = lpMeasureItemStruct->itemID;
	if(nID==0) //분리자이면
	{
		lpMeasureItemStruct->itemHeight = 9; //4라인에 어두운색 5라인에 밝은색
		return ;
	}
    
    MenuData d ;
    if(!TMenuData_Get(nID, d))
        ASSERT(0);

    CString strString = d._sText;
	//크기 계산에 사용될 
	CDC dc;
	dc.CreateCompatibleDC(NULL);
    
    //  폰트를 없다면 일단 생성하지..
    //
    CFontEx MenuFont;
    MenuFont.CreatePointFont_2(d._iSize>0 ? d._iSize : 90, d._bBold?true:false, L"굴림체");
	dc.SelectObject(&MenuFont);

	//메뉴 스트링을 메뉴이름과 단축키로 분리하여 얻어온다.
	CString strName, strKey;
	AfxExtractSubString(strName, strString, 0, '\t');
	AfxExtractSubString(strKey, strString, 1, '\t');

	// measure text size
	CRect rcRect;
	dc.DrawText(strName, &rcRect, DT_SINGLELINE | DT_CALCRECT);

	// both icon and text must fit
	int nHeight = max(MB_CY_ICON + 4, rcRect.Height());
	int nWidth = 2 + MB_CX_ICON + 6 + rcRect.Width() + nHeight/2;
	
	//단축키가 있으면 더 위치를 확보한다.
	if (!strKey.IsEmpty())
	{
		dc.DrawText(strKey, &rcRect, DT_SINGLELINE | DT_CALCRECT);
		nWidth += rcRect.Width() + nHeight/2;
	}

	lpMeasureItemStruct->itemWidth = nWidth;
    lpMeasureItemStruct->itemHeight = d._iHeight >0 ?max(nHeight, d._iHeight) : nHeight;
    
    //CString sDe;
    //sDe.Format("nHeight=%d _iHeight=%d", nHeight, d._iHeight);
    //OutputDebugString(sDe);
}


BOOL	g_bXpMenuStyle = 1;

COLORREF	g_colUnSelectedBack		  = RGB(252,252,252);
COLORREF	g_colSelectedBack	  = RGB(202,212,227);
COLORREF	g_colSelectedRect = RGB(115,152,205);
COLORREF	g_colSep = RGB(168,168,168);
COLORREF	g_colSelectedText = RGB(0,0,0);
COLORREF	g_colUnSelectedText = RGB(0,0,0);
COLORREF	g_colLeft = RGB(239, 237, 222);

const	int	nIconWidth = 21;



void TMenuData_OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	//그리고 여기서 글자를 출력한다..
	UINT nID = lpDrawItemStruct->itemID; //

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	int fState = lpDrawItemStruct->itemState;
	dc.SetBkMode(TRANSPARENT);

	//위치를 얻어온다..
	CRect rcItem = lpDrawItemStruct->rcItem;	

	CBrush brLeft; 	

	brLeft.CreateSolidBrush(g_colLeft);	

	if(nID == 0)	//분리자이면.. ItemHeight = 9
	{
		dc.FillSolidRect(rcItem, g_colUnSelectedBack);
		dc.Draw3dRect(rcItem.left + nIconWidth +2 , rcItem.top+3, rcItem.Width() - (nIconWidth +2), 1, g_colSep, g_colSep );

		//dc.FillRect(&rcItem, &brMenu);
		CRect rc= rcItem;
		rc.right= rc.left + nIconWidth;
		dc.FillRect(&rc, &brLeft);
		dc.Detach();
		return;		
	}

    MenuData d ;
    if(!TMenuData_Get(nID, d))
        ASSERT(0);
	
	//브러쉬를 만든다..
	CBrush brHighlight; 	
	CBrush brMenuHilight; 	
	CBrush brMenu; 	
	
	brHighlight.CreateSolidBrush(g_colSelectedRect);
	brMenuHilight.CreateSolidBrush(g_colSelectedBack);
    if(d._colBg!=-1)
        g_colUnSelectedBack  = d._colBg;
    else
        g_colUnSelectedBack  = RGB(252,252,252);
	brMenu.CreateSolidBrush(g_colUnSelectedBack);


    if(d._colText!=-1)
    {
        g_colUnSelectedText = d._colText;
        g_colSelectedText = d._colText;
    }
    else
    {
        g_colUnSelectedText = RGB(0,0,0);
        g_colSelectedText = RGB(0,0,0);
    }


	if (fState & ODS_SELECTED)
	{
		// 마우스 위면 그리고 Rect를 그린다..
		dc.FillRect(&rcItem, &brMenuHilight);
		dc.FrameRect(&rcItem, &brHighlight);
	}
	else
	{
		dc.FillRect(&rcItem, &brMenu);
		CRect rc= rcItem;
		rc.right= rc.left + nIconWidth;
		dc.FillRect(&rc, &brLeft);
	}

    CString strString = d._sText;
    //OutputDebugString(strString);
	CString strName, strKey;
	AfxExtractSubString(strName, strString, 0, '\t');
	AfxExtractSubString(strKey, strString, 1, '\t');

    //  폰트를 없다면 일단 생성하지..
    //
    CFontEx MenuFont;
    MenuFont.CreatePointFont_2(d._iSize>0 ? d._iSize : 90, d._bBold?true:false, L"굴림체");
	CFont* pFont = dc.SelectObject(&MenuFont);    

	CRect rcText = rcItem;
	rcText.left += 2 + MB_CX_ICON + 6;
	// center icon vertically
	int nOffY = (rcItem.Height() - MB_CY_ICON) / 2;
	UINT fFormat = DT_SINGLELINE | DT_VCENTER ;

	int nColorText = (fState & (ODS_DISABLED | ODS_GRAYED))
		? (fState & ODS_SELECTED) ? COLOR_3DSHADOW : COLOR_GRAYTEXT
		: (fState & ODS_SELECTED) ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT;

	if(g_bXpMenuStyle==0)
		dc.SetTextColor(::GetSysColor(nColorText));
	else
	{
		dc.SetTextColor( (fState & ODS_SELECTED) ? g_colSelectedText : g_colUnSelectedText );
	}

    //CString sDebug;
    //sDebug.Format("%s->%d %d %d %d", strName, rcText.left, rcText.top, rcText.Width(), rcText.Height());
    //OutputDebugString(sDebug);

    
	dc.DrawText(strName, &rcText, fFormat | DT_LEFT);
	if (!strKey.IsEmpty())
		dc.DrawText(strKey, &rcText, fFormat | DT_RIGHT);

    dc.SelectObject(pFont);
	dc.Detach();

}

