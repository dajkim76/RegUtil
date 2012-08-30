#pragma once

struct KeyRoot
{
	enum type
	{
		HKCR = 0,
		HKLM, 
		HKCU,
		HKUSERS,
		HKCC,
		UNKNOWN,
	};

	static type toType(CString path);
	static HKEY GetKey(type key);
	static CString toText(type key);
};

typedef KeyRoot::type _KeyRoot;

class RegWorks
{
public:
	RegWorks(void);
	~RegWorks(void);

	static HWND FindRegEdit();
	static void LaunchRegEditor(bool waitIdle);
	static HWND RequireRegEditorHanlde();
	static bool Lookup(_KeyRoot root, CString path, CString name);
	static bool Lookup(CString fullpath, CString name);
	static bool IsValidPath(CString path);
	static void Validate(CString& path);
};
