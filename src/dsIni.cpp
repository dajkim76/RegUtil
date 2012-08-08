#include	"stdafx.h"
#include	"dsIni.h"





/** @ingroup DSUTIL
	dsGetSectionKeys szPath에 szSection있는 모든 키를 aKeys 저장된다.

	aKeys : 키 리스트가 저장되는 컨테이너
	szSection : 섹션 명
	szPath : ini 파일 
	return : 메모리 할당 에러나 기타 오류가 나면 FALSE, 성공적이면 TRUE
*/
BOOL	dsGetSectionKeys(CStringArray & aKeys, LPCTSTR szSection, LPCTSTR szPath)
{
	//파일 존재 확인
	aKeys.RemoveAll();

	//버퍼 할당, 초기버퍼는 2만 바이트로 충분하게
	DWORD dwSize = 20000;
	DWORD dwRetSize =0;
	TCHAR*szBuffer = new TCHAR[dwSize];
	if(NULL == szBuffer)
		return FALSE;
	do
	{
		dwRetSize = GetPrivateProfileString(szSection, NULL, NULL, szBuffer, dwSize, szPath);
		//만약 버퍼의 크기가 모자라면
		if((dwSize-2) == dwRetSize) 
		{			
			delete	[]szBuffer;
			dwSize = dwSize * 2;
			szBuffer = new TCHAR[dwSize];
			//메모리 할당이 실패하면
			if(NULL == szBuffer)
				return FALSE;
		}else
			break;
		
	}while(TRUE);

	//버퍼 할당
	TCHAR *pPos = szBuffer;
	while (NULL != *pPos)
	{
		aKeys.Add(pPos);
		pPos += lstrlen(pPos) + 1;
	}

	delete	[]szBuffer;
	szBuffer = NULL;

	return TRUE;
}


/**
 *	@param szSecFrom   이동될 섹션
 *  @param szSecTo     szSecFrom이 이동하는 섹션
 *  @param szIniPath   ini 파일
 *  @param 이동하기 전에  szDest를 지울지를 판단.
 */
void    dsSectionToSection(LPCTSTR szSecFrom, 
                           LPCTSTR szSecTo, 
                           LPCTSTR szIniPath, 
                           bool bBeforeDeleteAll)
{
    TCHAR   szBuffer[10240];
    CStringArray arKeys, arDatas;
    int i=0;

    dsGetSectionKeys(arKeys, szSecFrom, szIniPath);    
    for(i=0;i<arKeys.GetSize(); i++)
    {
        GetPrivateProfileString(szSecFrom, arKeys[i], "", szBuffer, 10240, szIniPath);
        arDatas.Add(szBuffer);
    }

    if(bBeforeDeleteAll)
    {
        WritePrivateProfileString(szSecTo, NULL, NULL, szIniPath);
    }

    for(i=0;i<arKeys.GetSize(); i++)
    {
        WritePrivateProfileString(szSecTo, arKeys[i], arDatas[i], szIniPath); 
    }
}