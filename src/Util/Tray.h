
#ifndef __DSTRAY_H__
#define __DSTRAY_H__

class CDSTray : public CWnd
{
// Construction/destruction
public:	
	CDSTray();
	CDSTray(CWnd* pWnd, LPCTSTR szTip, HICON icon);
	virtual ~CDSTray();

	//Create the tray icon
	BOOL Create(CWnd* pWnd, LPCTSTR szTip, HICON icon, bool bDefaultShow=true);

	//Change or retrieve the Tooltip text
	BOOL    SetTooltipText(LPCTSTR pszTooltipText);
	BOOL    SetTooltipText(UINT nID);
	CString GetTooltipText() const;

	//Change or retrieve the icon displayed
	BOOL  SetIcon(HICON hIcon);
	BOOL  SetIcon(LPCTSTR lpIconName);
	BOOL  SetIcon(UINT nIDResource);
	BOOL  SetStandardIcon(LPCTSTR lpIconName);
	BOOL  SetStandardIcon(UINT nIDResource);
	HICON GetIcon() const;
	void  RemoveIcon();
	void ShowIcon(void);
	void HideIcon(void);	
	static UINT s_uiTrayID;

	//Change or retrieve the window to send notification messages to
	BOOL  SetNotificationWnd(CWnd* pNotifyWnd);
	CWnd* GetNotificationWnd() const;

// Implementation
	virtual LRESULT OnCallback(WPARAM wp, LPARAM lp);
	virtual void OnLButtonUp();
	virtual void OnRButtonUp();
    virtual void OnBalloonClick();
	void	SetPopupMenu(UINT IDR_Left = 0, UINT IDR_Right = 0)
	{
		m_uiLButtonPopupmenu = IDR_Left;
		m_uiRButtonPopupmenu = IDR_Right;
	}
    bool ShowBalloon(LPCTSTR szTitle, LPCTSTR szText, DWORD dwIcon, DWORD dwTimeOut);
protected:
	UINT    m_uiLButtonPopupmenu;
	UINT    m_uiRButtonPopupmenu;
	NOTIFYICONDATA	m_tnd;
	HICON myicon;
	CWnd* m_pParent;
	DECLARE_MESSAGE_MAP()
};


#endif// __DSTRAY_H__