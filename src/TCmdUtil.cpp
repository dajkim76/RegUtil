// TCmdUtil.cpp: implementation of the TCmdUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TCmdBar.h"
#include "TCmdUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TCmdUtil::TCmdUtil()
{

}

TCmdUtil::~TCmdUtil()
{

}


/**
 *	��ŻĿ�ǵ忡 ����� ���� �޽����� �����Ͽ� ������ �����ϰ� �Ѵ�.
 */
void	TCMD_SendID(UINT uiCmd)
{
	HWND hWnd = FindWindow(L"TTOTAL_CMD", NULL);
	if(hWnd != NULL)
	{
		SendMessage(hWnd, 1075/*WM_USER+xx*/,uiCmd, 0);
		::SetForegroundWindow(hWnd);
	}
}



BOOL CALLBACK EnumWindowsProc(          HWND hwnd,
    LPARAM lParam
)
{
    TCHAR szClass[MAX_PATH];
    GetClassName(hwnd, szClass, MAX_PATH);
    if( CString(szClass).CompareNoCase(L"tcombobox") == 0
        || CString(szClass).CompareNoCase(L"TMyComboBox.UnicodeClass") == 0 
        || CString(szClass).CompareNoCase(L"TMyComboBox") == 0 )
    {
        HWND hWndEdit = FindWindowEx(hwnd, NULL, L"EDIT", NULL);
        if(hWndEdit)
        {
            //Edit��Ʈ�ѿ� �ƹ��͵� ���� �͸��� ó���ϵ��� �Ѵ�. 
            //FTp�г��� �޺��� �ɸ��µ� �� �޺����� �׻� ���� ��� �ֱ� ������..
            //�������� �ʵ��� �ϱ� ���ؼ���.. 
           int iLen = GetWindowTextLength(hWndEdit);
           if(iLen == 0)
           {
               HWND* pWnd = (HWND*)(lParam);
               *pWnd = hwnd;
               return FALSE;    //Stop searching
           }
        }

    }
    return TRUE;
}

/**
 *	��Ŀ�� �н��� �����Ͽ� ������ �����Ѵ�.
 */
void TCMD_SendPath(CString sPath) 
{
    HWND hWndCmd = ::FindWindow(L"TTOTAL_CMD", NULL);
    if(!hWndCmd)
        return;

    HWND hWndCombo =0;
    EnumChildWindows(hWndCmd, EnumWindowsProc, (LPARAM)&hWndCombo);
    if(!hWndCombo)
        return;
    
    HWND hWndEdit = FindWindowEx(hWndCombo, NULL, L"EDIT", NULL);
    if(!hWndEdit)
        return;

    
    CEdit *pEdit = (CEdit*)CWnd::FromHandle(hWndEdit);
    if(pEdit)
    {
	    pEdit->SetSel(0, -1);
	    pEdit->ReplaceSel("cd "+sPath);

        pEdit->SendMessage(WM_KEYDOWN, VK_RETURN, 0);
        Sleep(10);
        pEdit->SendMessage(WM_KEYUP, VK_RETURN, 0);

        ::SetForegroundWindow(hWndCmd);
    }	
}




#include <rpc.h>
#pragma comment(lib,"Rpcrt4.lib")

CString GetSubkey()
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


CItemDataArray g_arButtonData;

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
    dsGetSectionKeys(_arKey, sFolderSectionKey, SUBFOLDERINI_FILE);
    CDSIni  ini(sFolderSectionKey, SUBFOLDERINI_FILE);
    for(int i=0;i<_arKey.GetSize();i++)
    {
        CString sData = ini.GetStr(_arKey[i], L"");

        //  ����Ÿ�� �־�߸� ��ȿ�ϴ�
        if( sData.GetLength() > 0)
        {             
            arKey.Add( _arKey[i]);
            arData.Add(sData);

            //�ٹ̱� ������ �ε��Ѵ�.
            CDSIni  ini2(sFolderSectionKey + L"E", SUBFOLDERINI_FILE);
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
    int iSize = GetPrivateProfileInt(L"main", L"size", 0, INI_FILE);
    if(0 == iSize)
    {
        pBar->SetButtons(NULL, 1);
        pBar->SetButtonText(0, L"���⿡ ������ ����Ͻÿ�, Ctrl+�巡�׷� �ٸ� �̵� ��ŵ�ϴ�.");
        pBar->SetButtonInfo(0, 99, TBBS_AUTOSIZE | TBBS_BUTTON  ,1);        
        return;
    }

    g_arButtonData.RemoveAll();

    ASSERT(pBar != NULL);
    pBar->SetButtons(NULL, iSize);
    for(int i=0; i<iSize; i++)
    {
        CString s = Int2Str(i);
        CDSIni  ini(s, INI_FILE);
        
        CButtonData   ButtonData;
        ButtonData.iCommand = ini.GetInt(L"iCommand", 0);
        ButtonData.sButtonText = ini.GetStr(L"sButtonText", L"");
        ButtonData.sFolderPath = ini.GetStr(L"sFolderPath", L"");
        ButtonData.sSubkey =     ini.GetStr(L"subkey", L"");
        if(ButtonData.sSubkey.IsEmpty())
        {
            ButtonData.sSubkey = GetSubkey();
            ini.WriteStr(L"subkey", ButtonData.sSubkey);
        }

        if(ButtonData.iCommand == 0 && ButtonData.sFolderPath.IsEmpty())
        {
            AfxMessageBox(L"ini ���Ͽ� ������ �ֽ��ϴ�.");
            continue;
        }       

        //  ���������� �ε��� ���� 
        //  ����Ÿ�� ������ ���� �̹����� �����Ѵ�.
        CStringArray ar;
        CStringArray ar3;
        CStringArray ar2;
        GetSubFolders(ButtonData.sSubkey, ar2, ar, ar3);
        int iImage = 0;
        if(ar.GetSize() > 0)
            iImage = 1;

        pBar->SetButtonText(i, ButtonData.sButtonText);
        pBar->SetButtonInfo(i,i+100, TBBS_AUTOSIZE | TBBS_BUTTON  ,iImage);

        g_arButtonData.Add(ButtonData);
    }
}
