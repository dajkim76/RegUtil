
#ifndef	__DSINI_H__
#define __DSINI_H__


/**
 *	@param szSecFrom   �̵��� ����
 *  @param szSecTo     szSecFrom�� �̵��ϴ� ����
 *  @param szIniPath   ini ����
 *  @param �̵��ϱ� ����  szDest�� �������� �Ǵ�.
 */
void    dsSectionToSection(LPCTSTR szSecFrom, 
                           LPCTSTR szSecTo, 
                           LPCTSTR szIniPath, 
                           bool bBeforeDeleteAll);

/**
	��� szSection�� NULL�̸� ��� ������ ��´�. �ƴϸ� ��� Ű �̸��� ��´�.
*/
BOOL    dsGetSectionKeys(CStringArray & aKeys, LPCTSTR szSection, LPCTSTR szPath) ;

/**	
	ini���� ���� Ŭ���� ����
*/
class	CDSIni
{
public:
	CString m_sSection, m_sFile;
	//CIni�� �⺻ �����ڰ� ���� ���� ���ϸ�� ���Ǹ��� �����ϱ� ���ؼ�
	CDSIni(LPCTSTR szSection, LPCTSTR szFile)
	{
		m_sSection = szSection;
		m_sFile = szFile;
	}
	void Init(LPCTSTR szSection, LPCTSTR szFile)
	{
		m_sSection = szSection;
		m_sFile = szFile;
	}
	//Integer
	inline int GetInt(LPCTSTR szKey, int nDefault=0)
	{
		return ::GetPrivateProfileInt(m_sSection, szKey, nDefault, m_sFile);
	}
	static int GetInt(LPCTSTR szSection, LPCTSTR szKey, int iDefault, LPCTSTR szFile)
	{
		return ::GetPrivateProfileInt(szSection, szKey, iDefault, szFile);
	}
	inline BOOL WriteInt(LPCTSTR szKey, int nValue)
	{
		CString str; str.Format(_T("%d"), nValue);
		return ::WritePrivateProfileString(m_sSection, szKey, str, m_sFile);
	}
	static BOOL WriteInt(LPCTSTR szSection, LPCTSTR szKey, int iVal, LPCTSTR szFile)
	{
		CString str; str.Format(_T("%d"), iVal);
		return ::WritePrivateProfileString(szSection, szKey, str, szFile);
	}


	//String
	inline CString GetStr(LPCTSTR szKey, LPCTSTR szDefault=_T(""))
	{
		TCHAR	szBuffer[1024]={0,};
		DWORD dw = ::GetPrivateProfileString(m_sSection, szKey, szDefault, szBuffer, 1024, m_sFile);
		if(dw == 1024-2)
		{
			TCHAR	szBuffer[10240]={0,};
			::GetPrivateProfileString(m_sSection, szKey, szDefault, szBuffer, 10240, m_sFile);
		}
		return szBuffer;
	}
	static CString GetStr(LPCTSTR szSection, LPCTSTR szKey,LPCTSTR szDefault, LPCTSTR szFile)
	{
		TCHAR	szBuffer[1024]={0,};
		DWORD dw = ::GetPrivateProfileString(szSection, szKey, szDefault, szBuffer, 1024, szFile);
		if(dw == 1024-2)
		{
			TCHAR	szBuffer[10240]={0,};
			::GetPrivateProfileString(szSection, szKey, szDefault, szBuffer, 10240, szFile);
		}
		return szBuffer;
	}
	inline BOOL WriteStr(LPCTSTR szKey, LPCTSTR szValue)
	{
		return ::WritePrivateProfileString(m_sSection, szKey, szValue, m_sFile);
	}
	static BOOL WriteStr(LPCTSTR szSection, LPCTSTR szKey,LPCTSTR szValue, LPCTSTR szFile)
	{
		return ::WritePrivateProfileString(szSection, szKey, szValue, szFile);
	}

	//Struct
	inline BOOL WriteStruct(LPCTSTR lpszKey, LPVOID lpStruct, UINT uSizeStruct)
	{
		return  WritePrivateProfileStruct(m_sSection, lpszKey, lpStruct, uSizeStruct, m_sFile);
	}
	inline BOOL GetStruct(  LPCTSTR lpszKey,  LPVOID lpStruct,  UINT uSizeStruct)
	{
		return  GetPrivateProfileStruct(m_sSection, lpszKey, lpStruct,uSizeStruct, m_sFile);
	}
	
	//Clear
	inline void ClearSection(LPCTSTR szSection)
	{
		::WritePrivateProfileString(m_sSection, NULL, NULL, m_sFile);		
	}
	inline void ClearIniFile()
	{
		::WritePrivateProfileString(NULL, NULL, NULL, m_sFile);		
	}
    inline void SectionToSection(LPCTSTR szSecFrom,  LPCTSTR szSecTo, bool bBeforeDeleteAll)
    {
        dsSectionToSection(szSecFrom, szSecTo, m_sFile, bBeforeDeleteAll);
    }
};


#endif