
#ifndef	__DSINI_H__
#define __DSINI_H__


/**
 *	@param szSecFrom   이동될 섹션
 *  @param szSecTo     szSecFrom이 이동하는 섹션
 *  @param szIniPath   ini 파일
 *  @param 이동하기 전에  szDest를 지울지를 판단.
 */
void    dsSectionToSection(LPCTSTR szSecFrom, 
                           LPCTSTR szSecTo, 
                           LPCTSTR szIniPath, 
                           bool bBeforeDeleteAll);

/**
	모든 szSection이 NULL이면 모든 섹션을 얻는다. 아니면 모든 키 이름을 얻는다.
*/
BOOL    dsGetSectionKeys(CStringArray & aKeys, LPCTSTR szSection, LPCTSTR szPath) ;

/**	
	ini파일 조작 클래스 랩퍼
*/
class	CDSIni
{
public:
	CString m_sSection, m_sFile;
	//CIni의 기본 생성자가 없는 것은 파일명과 섹션명을 보장하기 위해서
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