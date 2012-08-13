// DlgSort.cpp : implementation file
//

#include "stdafx.h"
#include "EasyRegistry.h"
#include "DlgSort.h"
#include "DlgRename.h"
#include "DlgConfigFolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSort dialog


CDlgSort::CDlgSort(CString sSection, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSort::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSort)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    _sSection = sSection;
    m_bChanged = false;
}


void CDlgSort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSort)
	DDX_Control(pDX, IDC_LIST1, _list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSort, CDialog)
	//{{AFX_MSG_MAP(CDlgSort)
	ON_BN_CLICKED(IDC_BUTTON1, OnUP)
	ON_BN_CLICKED(IDC_BUTTON2, OnDown)
	ON_BN_CLICKED(IDC_BUTTON3, OnDelete)
	ON_BN_CLICKED(IDC_BUTTON4, OnRename)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSort message handlers


BOOL CDlgSort::OnInitDialog() 
{
	CDialog::OnInitDialog();

    CenterWindow(GetDesktopWindow());

    _list.InsertColumn(0, L"������", LVCFMT_LEFT, 250);
    _list.InsertColumn(1, L"", LVCFMT_LEFT, 0);
	
    CStringArray arKeys;
    dsGetSectionKeys(arKeys,  _sSection, SUBMENU_INI);
    CDSIni ini(_sSection, SUBMENU_INI);
    int iListIndex = 0;
    for(int i=0;i<arKeys.GetSize(); i++)
    {
        CString sData = ini.GetStr(arKeys[i]);
        if(sData.IsEmpty()) continue;
        int pos = sData.Find(L"|");
        CString sTitle = sData.Left(pos);
        _list.InsertItem(iListIndex, sTitle);
        _list.SetItemText(iListIndex, 1, arKeys[i] + L"," + sData);
        iListIndex++;
    }

	_list.SetExtendedStyle( _list.GetExtendedStyle() | LVS_EX_FULLROWSELECT );
    
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CDlgSort::getIndex()
{
    POSITION pos = _list.GetFirstSelectedItemPosition();
    if( ! pos)
    {
        AfxMessageBox(L"���� �����ϼ���..");
        return -1;
    }
    return _list.GetNextSelectedItem(pos);
}

void CDlgSort::OnUP() 
{
	int index  = getIndex();
    if(index<0) return;

    if(index == 0) return;
    CString s0,s1;
    s0 = _list.GetItemText(index, 0);
    s1 = _list.GetItemText(index, 1);
    _list.DeleteItem(index);
    index--;
    _list.InsertItem(index, s0);
    _list.SetItemText(index, 1, s1);
    _list.SetItemState(index, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
    _list.SetFocus();
	
}

void CDlgSort::OnDown() 
{
	int index  = getIndex();
    if(index<0) return;	

    if(index == _list.GetItemCount()-1) return;

    CString s0,s1;
    s0 = _list.GetItemText(index, 0);
    s1 = _list.GetItemText(index, 1);
    _list.DeleteItem(index);
    index++;
    _list.InsertItem(index, s0);
    _list.SetItemText(index, 1, s1);
    _list.SetItemState(index, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
    _list.SetFocus();
	
}


void CDlgSort::OnOK() 
{
	CDSIni ini(_sSection, SUBMENU_INI);
	ini.ClearSection(_sSection);
    for(int i=0;i<_list.GetItemCount();i++)
    {
        CString s = _list.GetItemText(i, 1);
        CString s0 = s.Left( s.Find(L",") );
        CString s1 = s.Mid( s.Find(L",") + 1);

        CString d0,d1;
        d0 = s1.Left( s1.Find(L"|"));
        d1 = s1.Mid( s1.Find(L"|")+1);

        CString t1 = _list.GetItemText(i, 0);
        ini.WriteStr(s0, t1 + L"|" + d1);
    }
	CDialog::OnOK();
}

void CDlgSort::OnDelete() 
{
	int index  = getIndex();
    if(index<0) return;
    _list.DeleteItem(index);
    index--;
    _list.SetItemState(index, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
    _list.SetFocus();	
    m_bChanged = true;
}

void CDlgSort::OnRename() 
{
	int index  = getIndex();
    if(index<0) return;

	CString sKey = _list.GetItemText(index, 1);
	if(sKey.Find(L",") > 0)
	{
		sKey = sKey.Left( sKey.Find(L",") );
	}
	CRect rc;
	GetWindowRect(&rc);
	int iHeight = rc.Height();
	rc.top -= iHeight;
	rc.bottom -= iHeight;
	CDlgConfigFolder_Sub dlg(rc, _ttoi(sKey), _sSection, L"", this );
	if(dlg.DoModal() == IDOK)
	{
		_list.SetItemText(index, 0, dlg._sTitle);
		CString sData;
		sData.Format(L"%d,%s|%s|%s", _ttoi(sKey), dlg._sTitle, dlg._sTargetPath, dlg.name_);
		_list.SetItemText(index, 1, sData);
	}

}
