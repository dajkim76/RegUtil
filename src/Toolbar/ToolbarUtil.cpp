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
 *	��Ŀ�� �н��� �����Ͽ� ������ �����Ѵ�.
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
        // ���⼭ �ٽ� �����.
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
 * ���� ������ 
 * @param sFolderSectionKey :   �������� ������ ����� �����̸�
 * @param arKey     Ű ����
 * @param arData    ����Ÿ ����
 * @parma arExt     �޴� �ٹ̱� ����
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

        //  ����Ÿ�� �־�߸� ��ȿ�ϴ�
        if( sData.GetLength() > 0)
        {             
            arKey.Add( _arKey[i]);
            arData.Add(sData);

            //�ٹ̱� ������ �ε��Ѵ�.
            CDSIni  ini2(sFolderSectionKey + L"E", SUBMENU_INI);
            CString sExt = ini2.GetStr(_arKey[i]);
            arExt.Add(sExt);
        }
    }
}

/**
 *	���ٸ� �� ����� ini���� �ٽ� �ʱ�ȭ�Ѵ�.
 */
void    LoadIni(CToolBar * pBar)
{
    int iSize = _GetInt(L"size", 0);
    if(0 == iSize)
    {
		LPCWSTR defaultText = L"��Ŭ������ ���� Ȥ�� �߰��ϼ���, Ctrl + �巡�׷� �ٸ� �̵� ��ŵ�ϴ�.";
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

        //  ���������� �ε��� ���� 
        //  ����Ÿ�� ������ ���� �̹����� �����Ѵ�.
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
