// TCmdUtil.cpp: implementation of the TCmdUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EasyRegistry.h"
#include "ToolbarUtil.h"
#include "RegWorks\RegWorks.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 *	토커에 패스를 전달하여 폴더를 변경한다.
 */
void JumptoReg(CString sPath, CString name) 
{
	CWaitCursor wait;
	RegWorks::Lookup(sPath, name);
}




#include <rpc.h>
#pragma comment(lib,"Rpcrt4.lib")

CString CreateSubkey()
{
    CString sKey;
    UUID uuid;
	//BYTE szSerial[16];

	int iSizeUUID = sizeof(uuid);
	//int iSizeByte16 = sizeof(szSerial);

    if(RPC_S_OK != UuidCreate(&uuid))
    {
        // 여기서 다시 만든다.
        //
        srand(GetTickCount());
        for(int i=0;i<32;i++)
        {
            int iRand = rand();
            iRand %= 26;
            iRand += L'a';
            CString s;
            s.Format(L"%c", iRand);
            sKey += s;
        }
    }else
	{
		//wchar_t
		//WCHAR
		RPC_WSTR		szBuffer;
		if(RPC_S_OK == UuidToString((struct _GUID*)&uuid, &szBuffer))
		{
            sKey = (TCHAR*)szBuffer;
			RpcStringFree(&szBuffer);
		}
	}	
    return sKey;
}


ItemDataArray g_arButtonData;

/**
 * 서브 폴더를 
 * @param sFolderSectionKey :   서브폴더 정보가 저장된 섹션이름
 * @param arKey     키 정보
 * @param arData    데이타 정보
 * @parma arExt     메뉴 꾸미기 정보
 * @return
 */
void GetSubFolders(CString sFolderSectionKey, CStringArray& arKey, CStringArray &arData, CStringArray& arExt)
{
    CStringArray _arKey;
    dsGetSectionKeys(_arKey, sFolderSectionKey, SUBMENU_INI);
    CDSIni  ini(sFolderSectionKey, SUBMENU_INI);
    for(int i=0;i<_arKey.GetSize();i++)
    {
        CString sData = ini.GetStr(_arKey[i], L"");

        //  데이타가 있어야만 유효하다
        if( sData.GetLength() > 0)
        {             
            arKey.Add( _arKey[i]);
            arData.Add(sData);

            //꾸미기 정보를 로드한다.
            CDSIni  ini2(sFolderSectionKey + L"E", SUBMENU_INI);
            CString sExt = ini2.GetStr(_arKey[i]);
            arExt.Add(sExt);
        }
    }
}

/**
 *	툴바를 다 지우고 ini에서 다시 초기화한다.
 */
void    LoadIni(CToolBar * pBar)
{
    int iSize = _GetInt(L"size", 0);
    if(0 == iSize)
    {
		LPCWSTR defaultText = L"우클릭으로 수정 혹은 추가하세요, Ctrl + 드래그로 바를 이동 시킵니다.";
		_WriteInt(L"size", 1);
		CDSIni  ini(L"0", PROFILE_INI);
		ini.WriteStr(L"text", defaultText);
		ini.WriteInt(L"dummy", 1);
		iSize = 1;
    }

    g_arButtonData.RemoveAll();

    ASSERT(pBar != NULL);
    pBar->SetButtons(NULL, iSize);
    for(int i=0; i<iSize; i++)
    {
        CString s = Int2Str(i);
        CDSIni  ini(s, PROFILE_INI);
        
        ButtonData   ButtonData;
        ButtonData.text_ = ini.GetStr(L"text", L"");
        ButtonData.path_ = ini.GetStr(L"path", L"");
		ButtonData.name_ = ini.GetStr(L"name", L"");
        ButtonData.submenu_ =     ini.GetStr(L"subkey", L"");
        if(ButtonData.submenu_.IsEmpty())
        {
            ButtonData.submenu_ = CreateSubkey();
            ini.WriteStr(L"subkey", ButtonData.submenu_);
        }

        //  서브폴더를 로드해 보고 
        //  데이타가 있으면 변경 이미지를 변경한다.
        CStringArray ar;
        CStringArray ar3;
        CStringArray ar2;
        GetSubFolders(ButtonData.submenu_, ar2, ar, ar3);
        int iImage = 6;
		if( KeyRoot::toType(ButtonData.path_) == KeyRoot::HKCU )
		{
			iImage = 2;
		}
		else if( KeyRoot::toType(ButtonData.path_) == KeyRoot::HKLM )
		{
			iImage = 4;
		}

        if(ar.GetSize() > 0)
            iImage ++;

        pBar->SetButtonText(i, ButtonData.text_);
        pBar->SetButtonInfo(i,i+100, TBBS_AUTOSIZE | TBBS_BUTTON  ,iImage);

        g_arButtonData.Add(ButtonData);
    }
}
