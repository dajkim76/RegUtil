#include	"stdafx.h"
#include	"dsIni.h"





/** @ingroup DSUTIL
	dsGetSectionKeys szPath�� szSection�ִ� ��� Ű�� aKeys ����ȴ�.

	aKeys : Ű ����Ʈ�� ����Ǵ� �����̳�
	szSection : ���� ��
	szPath : ini ���� 
	return : �޸� �Ҵ� ������ ��Ÿ ������ ���� FALSE, �������̸� TRUE
*/
BOOL	dsGetSectionKeys(CStringArray & aKeys, LPCTSTR szSection, LPCTSTR szPath)
{
	//���� ���� Ȯ��
	aKeys.RemoveAll();

	//���� �Ҵ�, �ʱ���۴� 2�� ����Ʈ�� ����ϰ�
	DWORD dwSize = 20000;
	DWORD dwRetSize =0;
	TCHAR*szBuffer = new TCHAR[dwSize];
	if(NULL == szBuffer)
		return FALSE;
	do
	{
		dwRetSize = GetPrivateProfileString(szSection, NULL, NULL, szBuffer, dwSize, szPath);
		//���� ������ ũ�Ⱑ ���ڶ��
		if((dwSize-2) == dwRetSize) 
		{			
			delete	[]szBuffer;
			dwSize = dwSize * 2;
			szBuffer = new TCHAR[dwSize];
			//�޸� �Ҵ��� �����ϸ�
			if(NULL == szBuffer)
				return FALSE;
		}else
			break;
		
	}while(TRUE);

	//���� �Ҵ�
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
 *	@param szSecFrom   �̵��� ����
 *  @param szSecTo     szSecFrom�� �̵��ϴ� ����
 *  @param szIniPath   ini ����
 *  @param �̵��ϱ� ����  szDest�� �������� �Ǵ�.
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