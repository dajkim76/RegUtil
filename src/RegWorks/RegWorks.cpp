#include "stdafx.h"
#include "RegWorks.h"
#include "Misc.h"
#include <shlwapi.h>

namespace
{	
	// copy된 텍스트에서 주변글자를 제거하기 위한
	LPCWSTR kTrimStrings = L" \r\n\t()[]";

	class RegEditLocator
	{
	public:
		static const int BUF_SIZE = 1024;

		RegEditLocator(HWND editor) 
				: editor_(editor)
				, treeview_( NULL)
				, listview_(NULL)
				, process_( NULL)
				, remoteBuffer_(NULL)
		{
			localBuffer_ = localBufferImpl;
			GetWindowThreadProcessId(editor_, &pid_);

			if ( IsIconic(editor_) )
			{
				ShowWindow(editor_, SW_RESTORE);
			}

			treeview_ = FindWindowEx( editor_, NULL, L"SysTreeView32", NULL);
			ATLASSERT(treeview_);
			listview_ = FindWindowEx( editor_, NULL, L"SysListView32", NULL);
			ATLASSERT(listview_);

			// xp에서 NUU을 리턴하는 경우가 있어서 최소 권하으로 연다.
			process_ = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid_); 
			ATLASSERT(process_);
			if( process_ )
			{
				remoteBuffer_ = (BYTE*)VirtualAllocEx( process_, 0, BUF_SIZE, MEM_COMMIT, PAGE_READWRITE );
				ATLASSERT(remoteBuffer_);
			}
		}
		
		~RegEditLocator()
		{
			if ( remoteBuffer_ )
			{
				VirtualFreeEx( process_,(void*)remoteBuffer_, 0, MEM_RELEASE );
			}
			if ( process_ )
			{
				CloseHandle(process_);
			}
		}

		bool IsValid() { return (remoteBuffer_ != NULL) && (treeview_ != NULL) ; }

		void Locate(CString path, CString name)
		{
			if ( OpenKey(path, name.IsEmpty()) )
			{
				if ( name.GetLength() )
				{
					Sleep(30);
					OpenValue(name);
				}
			}
		}

		bool OpenKey(CString fullpath, bool isSelect)
		{
			SendMessage(treeview_, WM_SETFOCUS, 0, 0);

			// 트리를 새로고침한다.
			PostMessage(editor_, WM_KEYDOWN, VK_F5, 0x1f01);
			PostMessage(editor_, WM_KEYUP, VK_F5, 0x1f01);
			Sleep(20);

			HTREEITEM  tvItem = (HTREEITEM )SendMessage(treeview_, TVM_GETNEXTITEM, TVGN_ROOT, NULL);
			ATLASSERT(tvItem);
			if ( tvItem )
			{
				int start = 0;
				CString key = fullpath.Tokenize(L"\\", start);
				while( key.GetLength() )
				{
					tvItem = FindKey(tvItem, key);
					if( tvItem == NULL )
					{
						return false;
					}

					SendMessage(treeview_, TVM_SELECTITEM, TVGN_CARET, (LPARAM)tvItem);
					SendMessage(treeview_, WM_KEYDOWN, VK_RIGHT, 0);
					SendMessage(treeview_, WM_KEYUP, VK_RIGHT, 0);

					key = fullpath.Tokenize(L"\\", start);
				}
			}

			SendMessage(treeview_, TVM_SELECTITEM, TVGN_CARET, (LPARAM)tvItem);

			if ( isSelect ) 
			{
				BringWindowToTop(editor_);
			} 
			else 
			{
				SendTabKey(false);
			}

			return true;
		}

		HTREEITEM FindKey(HTREEITEM itemParent, CString key)
		{
			HTREEITEM itemChild = (HTREEITEM)SendMessage(treeview_, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)itemParent);
			while ( itemChild != NULL ) 
			{
				CString text = GetTVItemTextEx(treeview_, itemChild);
				if (text.CompareNoCase(key) == 0) 
				{
					return itemChild;
				}
				itemChild = (HTREEITEM)SendMessage(treeview_, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)itemChild);
			}
			ATLTRACE(L"key '%s' not found!", key);
			Beep(2000, 200);
			return NULL;
		}

		CString GetTVItemTextEx(HWND treeview, HTREEITEM item)
		{
			const int MAX_TVITEMTEXT = 512;

			TVITEM tvi;
			ZeroMemory(&tvi, sizeof(tvi));

			tvi.mask = TVIF_TEXT;
			tvi.hItem = item;
			tvi.cchTextMax = MAX_TVITEMTEXT;
			tvi.pszText = (LPWSTR)( (BYTE*)remoteBuffer_ + sizeof(TVITEM));

			if ( WriteProcessMemory( process_, remoteBuffer_, &tvi, sizeof tvi, NULL) )
			{
				SendMessage(treeview, TVM_GETITEMW, 0, (LPARAM)remoteBuffer_);
			
				if ( ReadProcessMemory( process_, remoteBuffer_, localBuffer_, BUF_SIZE, NULL) )
				{
					TCHAR* text = (TCHAR*) (localBuffer_ + sizeof(TVITEM));
					return text;
				}
			}

			ATLASSERT(!"exception");
			return L"";
		}

		void SendTabKey(bool shiftPressed)
		{
			if ( ! shiftPressed)
			{
				PostMessage(editor_, WM_KEYDOWN, VK_TAB, 0x1f01);
				PostMessage(editor_, WM_KEYUP, VK_TAB, 0x1f01);
			}
			else
			{
				PostMessage(editor_, WM_KEYDOWN, VK_SHIFT, 0x1f01);
				PostMessage(editor_, WM_KEYDOWN, VK_TAB, 0x1f01);
				PostMessage(editor_, WM_KEYUP, VK_TAB, 0x1f01);
				PostMessage(editor_, WM_KEYUP, VK_SHIFT, 0x1f01);
			}
		}

		void OpenValue(CString name)
		{
			ATLASSERT(name.GetLength());
			ATLASSERT(listview_);
			if( ! listview_ )
			{
				return;
			}
			
			SendMessage(listview_, WM_SETFOCUS, 0, 0);
			if ( name.IsEmpty() )
			{
				SetLVItemState(0);
				return;
			}

			int item = 0;
			for ( ; ; )
			{
				CString itemText = GetLVItemText(item);
				if ( itemText.IsEmpty())
				{
					break;
				}
				if ( itemText.CompareNoCase(name) == 0 ) 
				{
					break;
				}
				item++;
			}

			BringWindowToTop(editor_);
			SendMessage( listview_, WM_SETFOCUS, 0, 0);
			Sleep(30);

			SetLVItemState(item);
			ATLTRACE(L"%s --> %d index\n", name, item);

			SendMessage( listview_, LVM_ENSUREVISIBLE, item, 0);

			SendTabKey(false);
			SendTabKey(true);
		}

		void SetLVItemState(int item)
		{
			LVITEM  lvItem;
			ZeroMemory(&lvItem, sizeof lvItem);
			lvItem.mask = LVIF_STATE;
			lvItem.iItem = item;
			lvItem.iSubItem=0;

			lvItem.state = LVIS_FOCUSED | LVIS_SELECTED;
			lvItem.stateMask = LVIS_FOCUSED | LVIS_SELECTED;

			if ( WriteProcessMemory( process_, remoteBuffer_, &lvItem, sizeof lvItem, NULL ))
			{			
				SendMessage( listview_, LVM_SETITEMSTATE, item, (LPARAM)remoteBuffer_);
			}
			else
			{
				ATLASSERT(0);
			}
		}

		CString GetLVItemText(int item)
		{
			LVITEM  lvItem ;
			ZeroMemory(&lvItem, sizeof(lvItem));

			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = item;
			lvItem.iSubItem=0;
			// set address to remote buffer immediately following the lvItem 
			lvItem.pszText = (LPTSTR) (remoteBuffer_ + sizeof(TVITEM));
			lvItem.cchTextMax = 100;

			// copy local lvItem to remote buffer
			if ( WriteProcessMemory( process_, remoteBuffer_, &lvItem, sizeof lvItem, NULL) )
			{
				SendMessage(listview_, LVM_GETITEM, 0, (LPARAM)remoteBuffer_);

				if ( ReadProcessMemory( process_, remoteBuffer_, localBuffer_, BUF_SIZE, NULL) )
				{
					TCHAR* text = (TCHAR*) (localBuffer_ + sizeof(TVITEM));
					return text;
				}				
				else
				{
					ATLASSERT(!"exception 2");
				}
			}
			else
			{
				ATLASSERT(!"exception 1");
			}
			
			return L"";
		}

	protected:
		HWND editor_;
		DWORD pid_;
		HWND treeview_;
		HWND listview_;
		HANDLE process_;
		BYTE* remoteBuffer_;
		BYTE* localBuffer_;
		BYTE localBufferImpl[BUF_SIZE];
	};
}

CString RegTypeToString( DWORD type )
{
#define  _C(x)	if( type == x) return L#x
	_C(REG_SZ);
	_C(REG_EXPAND_SZ);
	_C(REG_MULTI_SZ);
	_C(REG_DWORD);
	_C(REG_QWORD);
	_C(REG_BINARY);
	_C(REG_RESOURCE_LIST);
	_C(REG_FULL_RESOURCE_DESCRIPTOR);
	_C(REG_RESOURCE_REQUIREMENTS_LIST);
	return L"";
}


KeyRoot::type KeyRoot::toType( CString path )
{
	if ( path.IsEmpty() )
	{
		return KeyRoot::UNKNOWN;
	}
	path.Trim(kTrimStrings);
	path.MakeUpper();
	int pos = path.Find(L"\\");
	if( pos > 0 )
	{
		path = path.Left( pos );
	}

	if ( path == L"HKCR" || path ==  L"HKEY_CLASSES_ROOT" )
	{
		return HKCR;
	}
	else if ( path == L"HKCU" || path == L"HKEY_CURRENT_USER" )
	{
		return HKCU;
	}
	else if ( path == L"HKLM" || path == L"HKEY_LOCAL_MACHINE" )
	{
		return HKLM;
	}
	else if ( path == L"HKUSERS" || path == L"HKEY_USERS" )
	{
		return HKUSERS;
	}
	else if ( path == L"HKCC" || path == L"HKEY_CURRENT_CONFIG" )
	{
		return HKCC;
	}
	else
	{
		_ASSERT_EXPR(0, __S(L"invalid root: %s ", path));
		return UNKNOWN;
	}
}

HKEY KeyRoot::toRootkey( type key )
{
	switch ( key )
	{
	case HKCR: return HKEY_CLASSES_ROOT;
		break;
	case HKCU: return HKEY_CURRENT_USER;
		break;
	case HKLM: return HKEY_LOCAL_MACHINE;
		break;
	case HKUSERS: return HKEY_USERS;
		break;
	case HKCC: return HKEY_CURRENT_CONFIG;
		break;
	default:
		ATLASSERT(!"key error");
		return NULL;
		break;
	}
}

CString KeyRoot::toText( type key )
{
	switch ( key )
	{
	case HKCR: return L"HKEY_CLASSES_ROOT";
		break;
	case HKCU: return L"HKEY_CURRENT_USER";
		break;
	case HKLM: return L"HKEY_LOCAL_MACHINE";
		break;
	case HKUSERS: return L"HKEY_USERS";
		break;
	case HKCC: return L"HKEY_CURRENT_CONFIG";
		break;
	default:
		ATLASSERT(!"key error");
		return L"";
		break;
	}
}

CString KeyRoot::toText( HKEY rootKey )
{
	if( rootKey == HKEY_CURRENT_USER) return L"HKEY_CURRENT_USER";
	if( rootKey == HKEY_LOCAL_MACHINE) return L"HKEY_LOCAL_MACHINE";
	if( rootKey == HKEY_CLASSES_ROOT) return L"HKEY_CLASSES_ROOT";
	if( rootKey == HKEY_USERS) return L"HKEY_USERS";
	if( rootKey == HKEY_CURRENT_CONFIG) return L"HKEY_CURRENT_CONFIG";
	
	ATLASSERT(!"key error");
	return L"";
}

HKEY KeyRoot::toRootkey( CString path )
{
	return toRootkey( toType(path) );
}


RegWorks::RegWorks(void)
{
}

RegWorks::~RegWorks(void)
{
}

HWND RegWorks::FindRegEdit()
{
	return FindWindow(L"RegEdit_RegEdit", NULL);
}

void RegWorks::LaunchRegEditor(bool waitIdle)
{
	if ( FindRegEdit() == NULL )
	{
		TCHAR path[MAX_PATH] = { NULL };
		GetWindowsDirectory(path, _countof(path));
		PathAppend(path, L"RegEdit.exe");

		if ( waitIdle )
		{
			SHELLEXECUTEINFO info;
			Execute(path, SW_NORMAL, NULL, &info);
			WaitForInputIdle(info.hProcess, INFINITE);
		}
		else
		{
			Execute(path, SW_NORMAL, NULL);
		}
	}
}

bool RegWorks::Lookup( _KeyRoot root, CString path, CString name )
{
	ATLASSERT(KeyRoot::toText(root).GetLength());
	ATLASSERT(path.GetLength());
	if( path.IsEmpty() )
	{
		return false;
	}

	CString fullpath = KeyRoot::toText(root);
	if( path.GetAt(0) != L'\\' )
	{
		fullpath += L"\\";
	}
	fullpath += path;

	return Lookup(fullpath, name);
}

bool RegWorks::Lookup( CString fullpath, CString name )
{
	Validate(fullpath);

	HWND hEditor = RequireRegEditorHanlde();
	if( hEditor )
	{
		RegEditLocator locator(hEditor);
		if( locator.IsValid())
		{
			locator.Locate( fullpath, name);
			return true;
		}
	}

	return false;
}

HWND RegWorks::RequireRegEditorHanlde()
{
	HWND hwnd = RegWorks::FindRegEdit();
	if ( hwnd == NULL )
	{
		RegWorks::LaunchRegEditor(true);
		hwnd = RegWorks::FindRegEdit();
		ATLASSERT(hwnd);
	}
	if( hwnd )
	{
		SetForegroundWindow(hwnd);
		Sleep(30);
	}
	return hwnd;
}

void RegWorks::Validate( CString& path )
{
	// hklm\\Sof... ==> HKEY_LOCAL_MACHINE\\Soft...
	path.Trim(kTrimStrings);
	path.Replace(L"\\\\", L"\\");

	int pos = path.Find(L'\\');
	if( pos > 0 )
	{
		CString root = path.Left(pos);
		CString key = path.Mid(pos + 1);
		_KeyRoot keyId = KeyRoot::toType(root);
		if( keyId != KeyRoot::UNKNOWN )
		{
			root = KeyRoot::toText(keyId);
			path = root + L"\\" + key;
		}
	}
}

bool RegWorks::IsValidPath( CString path )
{
	path.Trim(kTrimStrings);
	path.Replace(L"\\\\", L"\\");
	int pos = path.Find(L"\\");
	if( pos > 0)
	{
		path = path.Left( pos );
		return KeyRoot::toType(path) != KeyRoot::UNKNOWN;
	}

	return false;
}
