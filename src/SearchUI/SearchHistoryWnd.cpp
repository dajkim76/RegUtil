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


LPCWSTR kCaption = L"레지스트리 검색 결과: ";
// CSearchHistoryWnd

IMPLEMENT_DYNAMIC(CSearchHistoryWnd, CFrameWnd)

CSearchHistoryWnd::CSearchHistoryWnd()
{
	searchThread_ = NULL;

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
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CSearchHistoryWnd::OnNMDblclkList1)
	ON_COMMAND(ID_MNU_SAVEAS, &CSearchHistoryWnd::OnMnuSaveas)
	ON_COMMAND(ID_MNU_RESEARCH, &CSearchHistoryWnd::OnMnuResearch)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CSearchHistoryWnd::OnNMRClickList1)
END_MESSAGE_MAP()



// CSearchHistoryWnd message handlers



int CSearchHistoryWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	menu_.LoadMenu(IDR_MENU_SEARCH);
	SetMenu(&menu_);

	CRect rc;
	currentView_.Create(WS_VISIBLE | WS_CHILD | LVS_REPORT, rc, this, IDC_LIST1);
	ASSERT(currentView_.GetSafeHwnd() != NULL);
	currentView_.InsertColumn(0, L"Registry 키", LVCFMT_LEFT, 500 );
	currentView_.InsertColumn(1, L"값 이름", LVCFMT_LEFT, 100 );
	currentView_.InsertColumn(2, L"타입", LVCFMT_LEFT, 60 );
	currentView_.InsertColumn(3, L"데이타", LVCFMT_LEFT, 500 );
	currentView_.InsertColumn(4, L"수정된 날짜", LVCFMT_LEFT, 140 );
	currentView_.InsertColumn(5, L"길이", LVCFMT_LEFT, 100 );
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
		currentView_.InsertItem(0, L"검색중...");

		if ( searchThread_ )
		{
			searchThread_->Cancel();
		}
		searchThread_ = new SearchThread(searchThread_, m_hWnd, option);
		searchThread_->Start();
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

		if( item->validFiletime_ )
		{
			//filetime;
			SYSTEMTIME systemtime = { NULL };
			FileTimeToSystemTime(&item->filetime_, &systemtime);
			CString datetime;
			datetime.Format(L"%d-%02d-%02d %02d:%02d:%02d", 
				systemtime.wYear, systemtime.wMonth, systemtime.wDay, 
				systemtime.wHour, systemtime.wMinute, systemtime.wSecond);
			currentView_.SetItemText(index, 4, datetime );
		}
	}

	int next = currentView_.GetItemCount();
	currentView_.InsertItem(next, L"검색중...");
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
	str.Format(L"검색 끝: %d 건이 검색되었습니다. %d 초 걸렸습니다.", index, second);
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

// 저장하기
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

void CSearchHistoryWnd::OnNMRClickList1( NMHDR *pNMHDR, LRESULT *pResult )
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	POSITION pos = currentView_.GetFirstSelectedItemPosition();
	if( pos )
	{
		int index = currentView_.GetNextSelectedItem(pos);
		CMenu menu;
		menu.LoadMenu(IDR_MENU_SEARCHCONTEXT);
		CMenu* pMenu = menu.GetSubMenu(0);
		CPoint pt;
		GetCursorPos(&pt);
		CString str;
		CString name;
		name = currentView_.GetItemText(index, 1);
		bool onlyKey = name.IsEmpty();
		if( onlyKey )
		{
			pMenu->DeleteMenu(ID_SEACHCONTEXT_COPYNAME, MF_BYCOMMAND);
			pMenu->DeleteMenu(ID_SEACHCONTEXT_COPYDATA, MF_BYCOMMAND);
			pMenu->DeleteMenu(ID_SEACHCONTEXT_COPYALL, MF_BYCOMMAND);
		}
		int cmd = pMenu->TrackPopupMenu(TPM_RETURNCMD | TPM_NONOTIFY | TPM_LEFTBUTTON , pt.x, pt.y, this);
		switch(cmd)
		{
		case ID_SEACHCONTEXT_COPYKEY:
			str = currentView_.GetItemText(index, 0);
			SetClipboardText(m_hWnd, str);
			break;

		case ID_SEACHCONTEXT_COPYNAME:
			SetClipboardText(m_hWnd, name);
			break;

		case ID_SEACHCONTEXT_COPYDATA:
			str = currentView_.GetItemText(index, 3);
			SetClipboardText(m_hWnd, str);
			break;

		case ID_SEACHCONTEXT_COPYALL:
			str.Format(L"[%s]\n%s=%s\n"
				, currentView_.GetItemText(index, 0)
				, name
				, currentView_.GetItemText(index, 3)
				);
			SetClipboardText(m_hWnd, str);
			break;

		case ID_SEACHCONTEXT_DELETE:
			{
				CString path = currentView_.GetItemText(index, 0);
				if ( name.GetLength())
				{
					str.Format(L"[%s]\n%s=%s\n\n이 값을"
					, path
					, name
					, currentView_.GetItemText(index, 3)
					);
				}
				else
				{
					str.Format(L"[%s]\n\n이 키와 하위 값을", path);
				}

				int ret = AfxMessageBox(str + L"\n정말로 삭제하겠습니까?", MB_YESNO | MB_ICONQUESTION);
				if( ret == IDYES )
				{
					_KeyRoot key = KeyRoot::toType(path);
					ATLASSERT(key != KeyRoot::UNKNOWN);
					if ( key != KeyRoot::UNKNOWN )
					{
						HKEY hkey = KeyRoot::toRootkey(key);
						path = path.Mid(path.Find(L"\\") + 1);
						if( name.IsEmpty() )
						{
							SHDeleteKey(hkey, path);
						}
						else
						{
							SHDeleteValue(hkey, path, name);
						}
					}
				}
			}
			break;
		}
	}
}
