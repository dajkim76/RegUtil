// TCmdUtil.h: interface for the TCmdUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCMDUTIL_H__6CD5AF0A_85C7_45B5_8049_E63DBC3858CC__INCLUDED_)
#define AFX_TCMDUTIL_H__6CD5AF0A_85C7_45B5_8049_E63DBC3858CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class TCmdUtil  
{
public:
	TCmdUtil();
	virtual ~TCmdUtil();

};

void	TCMD_SendID(UINT uiCmd)    ;            ///< 700부터 시작하는 UserMenuID(토탈커맨드에 정의됨)
void    TCMD_SendPath(CString sPath)  ;         ///< 패스를 토커에 전달해서 패스를 변겨하게 한다.


CString GetSubkey();                            ///< GUID를 생성해서 고유의 ini섹션키를 얻는다.

#define INI_FILE                dsRunningPath(L"TCmdBar.ini")
#define INI_INT(x,d)            GetPrivateProfileInt(L"main", x, d, INI_FILE)
#define SUBFOLDERINI_FILE       dsRunningPath(L"SubFolder2.ini")

class  CButtonData
{
public:
    CString     sButtonText;      //Button
    CString     sFolderPath;      //Folder Path
    int         iCommand;         //Command
    CString     sSubkey;
};


typedef CArray  <CButtonData, CButtonData&>  CItemDataArray;
extern CItemDataArray g_arButtonData;

void    GetSubFolders(CString sFolderSectionKey, CStringArray& arKey, CStringArray &arData, CStringArray& arExt);
void    LoadIni(CToolBar * pBar);

#endif // !defined(AFX_TCMDUTIL_H__6CD5AF0A_85C7_45B5_8049_E63DBC3858CC__INCLUDED_)
