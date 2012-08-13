// TCmdUtil.h: interface for the TCmdUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCMDUTIL_H__6CD5AF0A_85C7_45B5_8049_E63DBC3858CC__INCLUDED_)
#define AFX_TCMDUTIL_H__6CD5AF0A_85C7_45B5_8049_E63DBC3858CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

void    JumptoReg(CString sPath, CString name = L"")  ; 

CString CreateSubkey();                            ///< GUID를 생성해서 고유의 ini섹션키를 얻는다.

class  ButtonData
{
public:
    CString     text_;      //Button
    CString     path_;      //Folder Path
	CString	    name_;
    CString     submenu_;
};


typedef CArray  <ButtonData, ButtonData&>  ItemDataArray;
extern ItemDataArray g_arButtonData;

void    GetSubFolders(CString sFolderSectionKey, CStringArray& arKey, CStringArray &arData, CStringArray& arExt);
void    LoadIni(CToolBar * pBar);

#endif // !defined(AFX_TCMDUTIL_H__6CD5AF0A_85C7_45B5_8049_E63DBC3858CC__INCLUDED_)
