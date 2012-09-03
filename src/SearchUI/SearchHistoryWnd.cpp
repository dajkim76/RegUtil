// SearchHistoryWnd.cpp : implementation file
//

#include "stdafx.h"
#include "EasyRegistry.h"
#include "SearchHistoryWnd.h"
#include "SearchDlg.h"
#include "SearchThread.h"
#include "RegWorks\RegWorks.h"


LPCWSTR kCaption = L"레지스트리 검색 결과: ";
// CSearchHistoryWnd

IMPLEMENT_DYNAMIC(CSearchHistoryWnd, CFrameWnd)

CSearchHistoryWnd::CSearchHistoryWnd()
{
	currentView_ = NULL;

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
END_MESSAGE_MAP()



// CSearchHistoryWnd message handlers



int CSearchHistoryWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	menu_.LoadMenu(IDR_MENU2);
	SetMenu(&menu_);

	CRect rc;
//	tab_.Create(WS_CHILD | WS_VISIBLE, rc, this, 101);
//	tab_.InsertItem(0, L"탭1");
//	tab_.InsertItem(1, L"탭2");
//	tab_.SetFont( CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT)) );

	currentView_ = new CListCtrl();
	currentView_->Create(WS_VISIBLE | WS_CHILD | LVS_REPORT, rc, this, 102);
	ASSERT(currentView_->GetSafeHwnd() != NULL);
	currentView_->InsertColumn(0, L"Registry Key", LVCFMT_LEFT, 500 );
	currentView_->InsertColumn(1, L"Name", LVCFMT_LEFT, 100 );
	currentView_->InsertColumn(2, L"Type", LVCFMT_LEFT, 60 );
	currentView_->InsertColumn(3, L"Data", LVCFMT_LEFT, 500 );
	currentView_->InsertColumn(4, L"Key Modified time", LVCFMT_LEFT, 100 );
	currentView_->InsertColumn(5, L"Data Length", LVCFMT_LEFT, 100 );
	currentView_->SetExtendedStyle(currentView_->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return 0;
}

void CSearchHistoryWnd::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	if( currentView_ && nType != SC_MINIMIZE )
	{
		//tab_.MoveWindow(0, 0, cx, 25);
		
		ASSERT(currentView_ && currentView_->GetSafeHwnd() != NULL);
		currentView_->MoveWindow(0, 0, cx, cy);
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
		currentView_->DeleteAllItems();
		currentView_->InsertItem(0, L"검색중...");
		SearchThread* searchThread = new SearchThread(m_hWnd, option);
		searchThread->Start();
	}
}

LRESULT CSearchHistoryWnd::OnSearchItem( WPARAM wParam, LPARAM lParam )
{
	if( currentView_ == NULL )
	{
		return 0;
	}

	CAtlString* key = (CAtlString*) wParam;
	RegItem* item = (RegItem*) lParam;
	
	int index = currentView_->GetItemCount() - 1;
	currentView_->SetItemText(index, 0, *key);
	if( item )
	{
		currentView_->SetItemText(index, 1, item->name_ );
		currentView_->SetItemText(index, 2, RegTypeToString(item->type_) );
		currentView_->SetItemText(index, 3, item->text_ );
		currentView_->SetItemText(index, 5, Int2Str(item->length_) );
	}

	int next = currentView_->GetItemCount();
	currentView_->InsertItem(next, L"검색중...");
	currentView_->EnsureVisible(next, TRUE);
	delete key;
	delete item;
	return 0;
}

LRESULT CSearchHistoryWnd::OnSearchEnd( WPARAM wParam, LPARAM lParam )
{
	int index = currentView_->GetItemCount() - 1;
	int second = (int) wParam;
	CString str;
	str.Format(L"검색 끝: %d 건이 검색되었습니다. %d 초 걸렸습니다.", index, second);
	currentView_->SetItemText(index, 0, str);
	return 0;
}

void CSearchHistoryWnd::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	POSITION pos = currentView_->GetFirstSelectedItemPosition();
	if ( pos )
	{
		int index = currentView_->GetNextSelectedItem(pos);
		if ( index < currentView_->GetItemCount() - 1)
		{
			CString path = currentView_->GetItemText(index, 0);
			CString name = currentView_->GetItemText(index, 1);
			CWaitCursor wait;
			RegWorks::Lookup(path, name);
		}
	}
}
