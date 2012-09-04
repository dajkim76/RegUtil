// SearchHistoryWnd.cpp : implementation file
//

#include "stdafx.h"
#include "EasyRegistry.h"
#include "SearchHistoryWnd.h"
#include "SearchDlg.h"
#include "SearchThread.h"
#include "RegWorks\RegWorks.h"
#include "ResearchDlg.h"
#include "RegWorks\RegSearch.h"


LPCWSTR kCaption = L"������Ʈ�� �˻� ���: ";
// CSearchHistoryWnd

IMPLEMENT_DYNAMIC(CSearchHistoryWnd, CFrameWnd)

CSearchHistoryWnd::CSearchHistoryWnd()
{
	Create(
		NULL,
		kCaption,
		WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
		CRect(100, 180, 1200, 600),
		NULL,
		NULL,
		0);	
}

CSearchHistoryWnd::~CSearchHistoryWnd()
{	
}


BEGIN_MESSAGE_MAP(CSearchHistoryWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_NEW_SEARCH, &CSearchHistoryWnd::OnNewSearch)
	ON_MESSAGE(UM_SEARCH_ITEM, OnSearchItem)
	ON_MESSAGE(UM_SEARCH_END, OnSearchEnd)
	ON_NOTIFY(NM_DBLCLK, 102, &CSearchHistoryWnd::OnNMDblclkList1)
	ON_COMMAND(ID_MNU_SAVEAS, &CSearchHistoryWnd::OnMnuSaveas)
	ON_COMMAND(ID_MNU_RESEARCH, &CSearchHistoryWnd::OnMnuResearch)
END_MESSAGE_MAP()



// CSearchHistoryWnd message handlers



int CSearchHistoryWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	menu_.LoadMenu(IDR_MENU2);
	SetMenu(&menu_);

	CRect rc;
	currentView_.Create(WS_VISIBLE | WS_CHILD | LVS_REPORT, rc, this, 102);
	ASSERT(currentView_.GetSafeHwnd() != NULL);
	currentView_.InsertColumn(0, L"Registry Ű", LVCFMT_LEFT, 500 );
	currentView_.InsertColumn(1, L"�� �̸�", LVCFMT_LEFT, 100 );
	currentView_.InsertColumn(2, L"Ÿ��", LVCFMT_LEFT, 60 );
	currentView_.InsertColumn(3, L"����Ÿ", LVCFMT_LEFT, 500 );
	currentView_.InsertColumn(4, L"������ ��¥", LVCFMT_LEFT, 100 );
	currentView_.InsertColumn(5, L"����", LVCFMT_LEFT, 100 );
	currentView_.SetExtendedStyle(currentView_.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return 0;
}

void CSearchHistoryWnd::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	if( currentView_ && nType != SC_MINIMIZE )
	{
		//tab_.MoveWindow(0, 0, cx, 25);
		
		ASSERT(currentView_ && currentView_.GetSafeHwnd() != NULL);
		currentView_.MoveWindow(0, 0, cx, cy);
	}
}

void CSearchHistoryWnd::OnNewSearch()
{
	SearchOption option;
	CSearchDlg dlg(option);
	if( dlg.DoModal() == IDOK )
	{
		CAtlString text = kCaption;
		text += dlg.option_.keyword_;
		SetWindowText(text);
		currentView_.DeleteAllItems();
		currentView_.InsertItem(0, L"�˻���...");
		SearchThread* searchThread = new SearchThread(m_hWnd, option);
		searchThread->Start();
	}
}

LRESULT CSearchHistoryWnd::OnSearchItem( WPARAM wParam, LPARAM lParam )
{
	CAtlString* key = (CAtlString*) wParam;
	RegItem* item = (RegItem*) lParam;
	
	int index = currentView_.GetItemCount() - 1;
	currentView_.SetItemText(index, 0, *key);

	if( item )
	{
		currentView_.SetItemText(index, 1, item->name_ );
		currentView_.SetItemText(index, 2, RegTypeToString(item->type_) );
		currentView_.SetItemText(index, 3, item->text_ );
		currentView_.SetItemText(index, 5, Int2Str(item->length_) );
	}

	int next = currentView_.GetItemCount();
	currentView_.InsertItem(next, L"�˻���...");
	currentView_.EnsureVisible(next, TRUE);
	delete key;
	delete item;
	return 0;
}

LRESULT CSearchHistoryWnd::OnSearchEnd( WPARAM wParam, LPARAM lParam )
{
	int index = currentView_.GetItemCount() - 1;
	int second = (int) wParam;
	CString str;
	str.Format(L"�˻� ��: %d ���� �˻��Ǿ����ϴ�. %d �� �ɷȽ��ϴ�.", index, second);
	currentView_.SetItemText(index, 0, str);
	return 0;
}

void CSearchHistoryWnd::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	POSITION pos = currentView_.GetFirstSelectedItemPosition();
	if ( pos )
	{
		int index = currentView_.GetNextSelectedItem(pos);
		if ( index < currentView_.GetItemCount() - 1)
		{
			CString path = currentView_.GetItemText(index, 0);
			CString name = currentView_.GetItemText(index, 1);
			CWaitCursor wait;
			RegWorks::Lookup(path, name);
		}
	}
}

// �����ϱ�
void CSearchHistoryWnd::OnMnuSaveas()
{
	CFileDialog dlg(FALSE);
	if( dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetPathName();
		CFile file;
		if( file.Open(path, CFile::modeCreate | CFile::modeWrite) )
		{
			BYTE bom[2] = {0xFF, 0xFE};
			file.Write(bom, 2);
			const CAtlString tabText = L"\t";
			const CAtlString enterText = L"\n";

			int count = currentView_.GetItemCount();
			for( int i = 0; i < count ; i++ )
			{
				CString s[6];
				for( int j = 0; j < 6 - 2; j++)
				{
					s[j] = currentView_.GetItemText(i, j);
					file.Write( s[j].GetString(), s[j].GetLength() * sizeof(TCHAR));
					file.Write( tabText.GetString(), tabText.GetLength() * sizeof(TCHAR));
				}
			
				file.Write( enterText.GetString(), enterText.GetLength() * sizeof(TCHAR));
			}

			file.Close();
		}
	}
}

void CSearchHistoryWnd::OnMnuResearch()
{
	RegSearch::SearchOption option;
	CResearchDlg dlg(option) ;
	if( dlg.DoModal() == IDOK)
	{
		CSearchHistoryWnd* p = new CSearchHistoryWnd();
		CString caption ;
		GetWindowText(caption.GetBufferSetLength(1024), 1024);;
		caption.ReleaseBuffer();

		caption += L" > ";
		caption += option.keyword_;
		p->SetWindowText(caption);

		CListCtrl& outList = p->GetListCtrl();

		int count = currentView_.GetItemCount();
		for( int i = 0; i < count ; i++ )
		{
			CString s[6];			
			for( int j = 0; j < 6 ; j++)
			{
				s[j] = currentView_.GetItemText(i, j)							;
			}

			bool found = false;
			if( option.keyCheck_ && FindKeyword(s[0], option.keyword_, option.caseSenstive_) )
			{
				found = true;				
			}
			else if( option.nameCheck_ && FindKeyword(s[1], option.keyword_, option.caseSenstive_) )
			{
				found = true;				
			}
			else if( option.valueCheck_ && FindKeyword(s[3], option.keyword_, option.caseSenstive_) )
			{
				found = true;
			}

			if( found )
			{
				int next = outList.GetItemCount();
				outList.InsertItem(next, s[0]);
				for( int k = 1; k < 6; k ++)
				{
					outList.SetItemText(next, k, s[k]);
				}
			}
		}
	}
}