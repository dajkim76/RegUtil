// TMenuData.cpp: implementation of the TMenuData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EasyRegistry.h"
#include "MenuData.h"
#include <map>
using namespace std;


/**
 *  DrawItem���� �޴��� �׸������� MenuData�� �ܼ��� �۷ι� map�� ������ �δ� ����� ����Ѵ�
 *  �ֳĸ� ����ϴ� �˾��޴��� �ϳ����̰�, �̷��� �ϴ� ���� ������ ���� �����ϴ�.
 *  
 *  ����Ÿ ũ�⵵ �۱⶧���� �����͸� ������� �ʰ�, ��ü�� �׳� ����־ �����Ѵ�.
 *  �����Ҵ����� �ʱ� ������ ���α׷� ����� ���ÿ� �޸𸮵� �� �����ȴ�.
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
 *	bBold�� true�̸� �β��� ��Ʈ�� �����ǵ��� �ϴ� Ŭ���� Ȯ��..
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
	if(nID==0) //�и����̸�
	{
		lpMeasureItemStruct->itemHeight = 9; //4���ο� ��ο�� 5���ο� ������
		return ;
	}
    
    MenuData d ;
    if(!TMenuData_Get(nID, d))
        ASSERT(0);

    CString strString = d._sText;
	//ũ�� ��꿡 ���� 
	CDC dc;
	dc.CreateCompatibleDC(NULL);
    
    //  ��Ʈ�� ���ٸ� �ϴ� ��������..
    //
    CFontEx MenuFont;
    MenuFont.CreatePointFont_2(d._iSize>0 ? d._iSize : 90, d._bBold?true:false, L"����ü");
	dc.SelectObject(&MenuFont);

	//�޴� ��Ʈ���� �޴��̸��� ����Ű�� �и��Ͽ� ���´�.
	CString strName, strKey;
	AfxExtractSubString(strName, strString, 0, '\t');
	AfxExtractSubString(strKey, strString, 1, '\t');

	// measure text size
	CRect rcRect;
	dc.DrawText(strName, &rcRect, DT_SINGLELINE | DT_CALCRECT);

	// both icon and text must fit
	int nHeight = max(MB_CY_ICON + 4, rcRect.Height());
	int nWidth = 2 + MB_CX_ICON + 6 + rcRect.Width() + nHeight/2;
	
	//����Ű�� ������ �� ��ġ�� Ȯ���Ѵ�.
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
	//�׸��� ���⼭ ���ڸ� ����Ѵ�..
	UINT nID = lpDrawItemStruct->itemID; //

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	int fState = lpDrawItemStruct->itemState;
	dc.SetBkMode(TRANSPARENT);

	//��ġ�� ���´�..
	CRect rcItem = lpDrawItemStruct->rcItem;	

	CBrush brLeft; 	

	brLeft.CreateSolidBrush(g_colLeft);	

	if(nID == 0)	//�и����̸�.. ItemHeight = 9
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
	
	//�귯���� �����..
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
		// ���콺 ���� �׸��� Rect�� �׸���..
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

    //  ��Ʈ�� ���ٸ� �ϴ� ��������..
    //
    CFontEx MenuFont;
    MenuFont.CreatePointFont_2(d._iSize>0 ? d._iSize : 90, d._bBold?true:false, L"����ü");
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

