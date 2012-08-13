// TMenuData.h: interface for the TMenuData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMENUDATA_H__D40D22CC_382F_42BB_AA99_4ADD4CB63391__INCLUDED_)
#define AFX_TMENUDATA_H__D40D22CC_382F_42BB_AA99_4ADD4CB63391__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MenuData  
{
public:
    int             _iSize;   //높이..
    int             _iHeight;
	CString         _sText;     //글자..
    COLORREF        _colText;   //글자색
    COLORREF        _colBg;     //배경색
    bool            _bBold;     //진하게
	MenuData()
    {
        _bBold = false;
        _iHeight = 18;
        _colText = -1;
        _colBg = -1;
        _iSize = -1;
        _iHeight = -1;
    }
	virtual ~MenuData();
};


void TMenuData_OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)  ;
void TMenuData_OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) ;
void TMenuData_Set(UINT nID, MenuData data) ;
void TMenuData_Clear();
void TMenuData_Parse(CString sExt, MenuData&data);
#endif // !defined(AFX_TMENUDATA_H__D40D22CC_382F_42BB_AA99_4ADD4CB63391__INCLUDED_)
