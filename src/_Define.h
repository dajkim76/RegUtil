#pragma once

const TCHAR REGEDIT_CLASSNAME[] = L"RegEdit_RegEdit";
CString __S( LPCWSTR pszFormat, ...);

CString _GetString(CString key, CString defaultValue = L"");
void  _WriteString(CString key, CString value);
int _GetInt(CString key, int defaultValue = 0);
void  _WriteInt(CString key, int value);

#define SUBMENU_INI		_ModulePath(L"SubMenu.ini")
#define PROFILE_INI		_ModulePath(L"EasyRegistry.ini")