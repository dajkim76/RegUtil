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

	for( int i = 0; i < MAX_SORT_COLUMN; i ++)
	{
		sortOrder_[i] = true;
		columnType_[i] = CListCtrlSortClass::dtSTRING;
	}

	columnType_[5] = CListCtrlSortClass::dtINT;

	curSortColumn_ = -1;

	Create(
		NULL,
		kCaption,
		WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
		CRect(100, 180, 1200, 600),
		NULL,
		NULL,
		0);	

	hbmUp_ = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_UP), IMAGE_BITMAP, 0, 0, 
		LR_LOADTRANSPARENT|LR_DEFAULTCOLOR); 
	hbmDn_ = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_DN), IMAGE_BITMAP, 0, 0, 
		LR_LOADTRANSPARENT|LR_DEFAULTCOLOR); 

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
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CSearchHistoryWnd::OnColumnclick)
	ON_COMMAND(ID_SEARCH_STOP, &CSearchHistoryWnd::OnSearchStop)
	ON_NOTIFY(NM_CUSTOMDRAW,  IDC_LIST1, OnCustomdrawMyList)
END_MESSAGE_MAP()



// CSearchHistoryWnd message handlers
const int IL_FOLDER = 2;
const int IL_REG_SZ = 7;
const int IL_REG_BIN = 8;
const int IL_REG_DWORD = 11;
const int IL_REG_EXPAND_SZ = 12;
const int IL_REG_MULTI_SZ = 13;
const int IL_REG_QWORD = 14;

int CSearchHistoryWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	menu_.LoadMenu(IDR_MENU_SEARCH);
	SetMenu(&menu_);

	CRect rc;
	currentView_.Create(WS_VISIBLE | WS_CHILD | LVS_REPORT, rc, this, IDC_LIST1);
	ASSERT(currentView_.GetSafeHwnd() != NULL);
	currentView_.InsertColumn(0, L"    Registry 키", LVCFMT_LEFT, 500 );
	currentView_.InsertColumn(1, L"값 이름", LVCFMT_LEFT, 100 );
	currentView_.InsertColumn(2, L"타입", LVCFMT_LEFT, 60 );
	currentView_.InsertColumn(3, L"데이타", LVCFMT_LEFT, 500 );
	currentView_.InsertColumn(4, L"수정된 날짜", LVCFMT_LEFT, 140 );
	currentView_.InsertColumn(5, L"길이", LVCFMT_LEFT, 100 );
	currentView_.SetExtendedStyle(currentView_.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	{
		if (imageList_.Create (16, 16, ILC_COLORDDB| ILC_COLOR32 | ILC_MASK, 2, 2))
		{
			HBITMAP hbm = (HBITMAP)::LoadImage (AfxGetResourceHandle(),
				MAKEINTRESOURCE(IDB_BITMAP_SEARCH),
				IMAGE_BITMAP, 0, 0, 
				LR_LOADMAP3DCOLORS);
			ImageList_AddMasked(imageList_.m_hImageList, hbm, RGB(255,0,255));
			currentView_.SetImageList(&imageList_, LVSIL_SMALL);
			DeleteObject(hbm);
		}
	}
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
		currentView_.InsertItem(0, L"검색중...", -1);
		SetKeyword(dlg.option_.keyword_, option.caseSenstive_ );

		bool preCanceled = false;
		if ( searchThread_ )
		{
			searchThread_->Cancel();
			preCanceled = true;
		}
		searchThread_ = new SearchThread(searchThread_, m_hWnd, option);
		searchThread_->Start();

		// 취소알림
		if ( preCanceled )
		{
			AfxMessageBox(L"이전 검색을 취소하고\n" + option.keyword_ + L" 키워드로 검색을 다시 시작했습니다.");
		}
	}
}

LRESULT CSearchHistoryWnd::OnSearchItem( WPARAM wParam, LPARAM lParam )
{
	CAtlString* key = (CAtlString*) wParam;
	RegItem* item = (RegItem*) lParam;
	
	int index = currentView_.GetItemCount() - 1;
	bool isVisible = currentView_.IsItemVisible(index);
	int iImage = item ? GetImage(item->type_) : IL_FOLDER;
	currentView_.InsertItem(index, *key, iImage);

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

	int next = currentView_.GetItemCount() - 1;
	//currentView_.InsertItem(next, L"검색중...");

	if( isVisible )
	{
		currentView_.EnsureVisible(next, TRUE);
	}

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
		p->SetKeyword(option.keyword_, option.caseSenstive_ );

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
				outList.InsertItem(next, s[0], GetImage(s[2]));
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


void CSearchHistoryWnd::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	*pResult = 0;
	//if( ! _bEnableSorting)
	//	return;

	const int count  = currentView_.GetItemCount();
	if ( count == 0 )
	{
		return ;
	}

	if( pNMListView->iSubItem >= MAX_SORT_COLUMN) 
		return;

	if( columnType_[ pNMListView->iSubItem ] == CListCtrlSortClass::SortDataType::dtNULL ) 
		return;

	if (pNMListView->iSubItem == curSortColumn_)
	{
		sortOrder_[ curSortColumn_ ] = ! sortOrder_[ curSortColumn_ ] ;
	}
	else
	{
		curSortColumn_ = pNMListView->iSubItem ;
		//sortOrder_[ curSortColumn_ ] = ! sortOrder_[ curSortColumn_ ] ;
	}

	CHeaderCtrl* pHeader = currentView_.GetHeaderCtrl();
	for(int i=0; i<pHeader->GetItemCount(); i++)
	{
		HDITEM hItem;		
		hItem.mask = HDI_FORMAT;
		pHeader->GetItem(i, &hItem);

		if(i == curSortColumn_)
		{
			hItem.mask |= HDI_BITMAP|HDI_FORMAT ;
			hItem.fmt |= HDF_BITMAP | HDF_STRING | HDF_BITMAP_ON_RIGHT;
			if(sortOrder_[ curSortColumn_ ])
				hItem.hbm = hbmUp_;
			else
				hItem.hbm = hbmDn_;
			pHeader->SetItem(i, &hItem);
		}
		else
		{
			hItem.fmt &= ~(HDF_BITMAP |  HDF_BITMAP_ON_RIGHT );
			hItem.hbm = NULL;
			pHeader->SetItem(i, &hItem);
		}
	}
	

	int lastIndex = (count - 1);
	CString str = currentView_.GetItemText(lastIndex, 0);
	currentView_.DeleteItem(lastIndex);

	// sc의 소멸자를 보장한다.
	{
		CListCtrlSortClass sc(&currentView_, curSortColumn_) ;
		sc.Sort(sortOrder_[curSortColumn_], columnType_[curSortColumn_]);
	}

	currentView_.InsertItem(lastIndex, str, -1);
}
void CSearchHistoryWnd::OnSearchStop()
{
	if ( searchThread_ )
	{
		searchThread_->Cancel();
		int lastIndex = currentView_.GetItemCount() - 1;
		if( lastIndex >= 0)
		{
			currentView_.SetItemText(lastIndex, 0, L"검색이 취소되었습니다.");
		}
	}
}


void DrawHilightText(CDC& dc, const CString text, const CString keyword, bool caseSensitive, const CRect rc)
{
	const UINT kDrawTextFlags = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	const COLORREF colText = RGB(0, 0, 0);
	const COLORREF colHighlight = RGB(255, 128, 64);
	dc.SetBkMode(TRANSPARENT);

	CString textUpper;
	CString keywordUpper;
	CString tokenKeyword;
	if( !caseSensitive )
	{
		textUpper = text;
		textUpper.MakeUpper();

		keywordUpper = keyword;
		keywordUpper.MakeUpper();
	}

	int start = 0;
	int pos = 0;
	pos = caseSensitive ? text.Find(keyword) : textUpper.Find(keywordUpper);
	int left = 0;
	int width = 0;
	CRect rcCalc;
	CRect rcDraw;
	CRect rcItem;

	while(pos >= 0)
	{
		if( pos != start )
		{
			//키워드 왼쪽
			CString leftText = text.Mid(start, pos - start);
			rcCalc.SetRectEmpty();
			dc.DrawText(leftText, -1, &rcCalc, DT_CALCRECT | DT_LEFT );
			rcDraw.SetRect(rc.left + width, rc.top, rc.left + width + rcCalc.Width(), rc.bottom );

			dc.SetTextColor(colText);
			rcItem.IntersectRect(rcDraw, rc);
			dc.DrawText(leftText, -1, &rcItem, kDrawTextFlags);
			width += rcDraw.Width();
			start += leftText.GetLength();
		}
	
		//실제 데이타 키워드
		tokenKeyword = text.Mid(pos , keyword.GetLength());

		// 키워드
		rcCalc.SetRectEmpty();
		dc.DrawText(tokenKeyword, -1, &rcCalc, DT_CALCRECT | DT_LEFT );
		rcDraw.SetRect(rc.left + width, rc.top, rc.left + width + rcCalc.Width(), rc.bottom );

		dc.SetTextColor(colHighlight);
		rcItem.IntersectRect(rcDraw, rc);
		dc.DrawText(tokenKeyword, -1, &rcItem, kDrawTextFlags);
		width += rcDraw.Width();
		start += keyword.GetLength();

		if( width > rc.Width() )
		{
			break;
		}

		pos = caseSensitive ? text.Find(keyword, start) : textUpper.Find(keywordUpper, start);
	}

	CString lastText = text.Mid(start);
	if( width < rc.Width() && lastText.GetLength())
	{
		rcDraw.SetRect(rc.left + width, rc.top, rc.right, rc.bottom );
		dc.SetTextColor(colText);
		dc.DrawText(lastText, -1, &rcDraw, kDrawTextFlags);
	}
}

void CSearchHistoryWnd::OnCustomdrawMyList( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	*pResult = 0;


	HDC hdc = pLVCD->nmcd.hdc;
	CDC* pDC = NULL;

	// here is the item info
	// note that we don't get the subitem
	// number here, as this may not be
	// valid data except when we are
	// handling a sub item notification
	// so we'll do that separately in
	// the appropriate case statements
	// below.
	int nItem = pLVCD->nmcd.dwItemSpec;
	UINT nState = pLVCD->nmcd.uItemState;
	LPARAM lParam = pLVCD->nmcd.lItemlParam;

	switch( pLVCD->nmcd.dwDrawStage )
	{
		case CDDS_PREPAINT :
		{
			*pResult = CDRF_NOTIFYITEMDRAW ;
		}
		break;
	
		case CDDS_ITEMPREPAINT:
		{
			// This is the notification message for an item.  We'll request
			// notifications before each subitem's prepaint stage.

			*pResult = CDRF_NOTIFYSUBITEMDRAW ;
		}
		break;
		
		case  (CDDS_ITEMPREPAINT | CDDS_SUBITEM):
		{
			pLVCD->clrTextBk = RGB(255,255,255);           // 해당 행, 열 아이템의 배경색을 지정한다.
			pLVCD->clrText = RGB(0, 0, 0);                      // 해당 행, 열 아이템의 글자색을 지정한다.


			int nSubItem = pLVCD->iSubItem;
			if ( keyword_.GetLength() && (nSubItem == 0 || nSubItem == 1 || nSubItem == 3))
			{
				CString text = currentView_.GetItemText(nItem, nSubItem);

				CString textUpper;
				CString keywordUpper;
				if( !caseSensitive_ )
				{
					textUpper = text;
					textUpper.MakeUpper();

					keywordUpper = keyword_;
					keywordUpper.MakeUpper();
				}				

				int pos = caseSensitive_ ? text.Find(keyword_) : textUpper.Find(keywordUpper);
				if( pos >= 0 && hdc )
				{
					pDC = CDC::FromHandle(hdc);
					
					
					//CRect rc(pLVCD->nmcd.rc);

					// winxp에서 rect가 제대로 넘겨오지 않는다.
					CRect rc;
					if(nSubItem>0)
					{
						currentView_.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rc);
						pDC->FillSolidRect(&rc, RGB(240, 240, 240));
					}
					else
					{
						currentView_.GetItemRect(nItem ,&rc, LVIR_BOUNDS);
						pDC->FillSolidRect(&rc, RGB(240, 240, 240));
						rc.left += 4;
					}

					if( nSubItem == 0 )
					{						
						CPoint pt = rc.TopLeft();
						rc.left += 16 + 2;
						LVITEM item;
						ZeroMemory(&item, sizeof(item));
						item.iItem = nItem;
						item.mask = LVIF_IMAGE;
						currentView_.GetItem(&item);
						imageList_.Draw(pDC, item.iImage, pt, ILD_NORMAL);
					}
					DrawHilightText(*pDC, text, keyword_, caseSensitive_, rc);					
					*pResult |= CDRF_SKIPDEFAULT;
				}
			}
		}
	}

	// Take the default processing unless we set this to something else below.
	if ( *pResult == 0 )
	{
		*pResult = CDRF_DODEFAULT;
	}
}

int CSearchHistoryWnd::GetImage( const CString type )
{
#define  _C(x, r)	if( type == L#x) return r
	_C(REG_SZ, IL_REG_SZ);
	_C(REG_EXPAND_SZ, IL_REG_EXPAND_SZ);
	_C(REG_MULTI_SZ, IL_REG_MULTI_SZ);
	_C(REG_DWORD, IL_REG_DWORD);
	_C(REG_QWORD, IL_REG_QWORD);
	_C(REG_BINARY, IL_REG_BIN);
	_C(REG_RESOURCE_LIST, IL_REG_BIN);
	_C(REG_FULL_RESOURCE_DESCRIPTOR, IL_REG_BIN);
	_C(REG_RESOURCE_REQUIREMENTS_LIST, IL_REG_BIN);
	return 4;
}

int CSearchHistoryWnd::GetImage( const DWORD type )
{
#undef _C
#define  _C(x, r)	if( type == x) return r
	_C(REG_SZ, IL_REG_SZ);
	_C(REG_EXPAND_SZ, IL_REG_EXPAND_SZ);
	_C(REG_MULTI_SZ, IL_REG_MULTI_SZ);
	_C(REG_DWORD, IL_REG_DWORD);
	_C(REG_QWORD, IL_REG_QWORD);
	_C(REG_BINARY, IL_REG_BIN);
	_C(REG_RESOURCE_LIST, IL_REG_BIN);
	_C(REG_FULL_RESOURCE_DESCRIPTOR, IL_REG_BIN);
	_C(REG_RESOURCE_REQUIREMENTS_LIST, IL_REG_BIN);
	return 4;
}
