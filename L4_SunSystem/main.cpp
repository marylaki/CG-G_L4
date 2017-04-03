#include "CMatrix.h"
#include "CSunSystem.h"
#include <afxwin.h>
#include <windows.h>

#define ID_S 2002
#define pi 3.14159
#define m_timer 1

class CMainWin : public CFrameWnd
{
public:
	CMainWin()
	{
		Create(NULL, L"KG&G Laba 4 SunSystem", WS_SYSMENU | WS_MINIMIZEBOX,SunSystem.GetRW()+CRect(0,0,30,65));
	}
	CMenu *menu;
	int IsData=0;
	int dt_Timer=100;
	CRect RW;
	CRectD RS;
	CSunSystem SunSystem;
	afx_msg int OnCreate(LPCREATESTRUCT);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT flags, CPoint Loc);
	afx_msg void OnRButtonDblClk(UINT flags, CPoint Loc);
	afx_msg void OnTimer(UINT_PTR nInEvent);
	void F();
	DECLARE_MESSAGE_MAP();
};
class CMyApp : public CWinApp
{
public:
	CMyApp(){};
	virtual BOOL InitInstance()
	{
		m_pMainWnd = new CMainWin();
		m_pMainWnd->ShowWindow(SW_SHOW);
		return TRUE;
	}
};
BEGIN_MESSAGE_MAP(CMainWin, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_COMMAND(ID_S, F)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_TIMER()
END_MESSAGE_MAP()

afx_msg void CMainWin::OnPaint()
{
	CPaintDC dc(this);
	if (IsData == 1)
	{
		SunSystem.GetRS(RS);
		RW = SunSystem.GetRW();
		SetMyMode(dc, RS, RW);
		SunSystem.Draw(dc);
		dc.SetMapMode(MM_TEXT);
	}
}

afx_msg int CMainWin::OnCreate(LPCREATESTRUCT)
{
	menu = new CMenu();
	menu->CreateMenu();
	CMenu *subMenu = new CMenu();
	subMenu->CreatePopupMenu();
	subMenu->AppendMenu(MF_STRING, ID_S, _T("Planets"));
	this->menu->AppendMenu(MF_POPUP | MF_STRING, (UINT)subMenu->m_hMenu, _T("LabPlanets"));
	SetMenu(menu);
	return 0;
}

void CMainWin::F()
{
	SunSystem.SetDT(0);
	SunSystem.SetNewCoords();
	SunSystem.SetDT(0.2);
	dt_Timer = 100;
	IsData = 1;
	Invalidate();
}

afx_msg void CMainWin::OnTimer(UINT_PTR nInEvent)
{
	SunSystem.SetNewCoords();
	Invalidate();
	CWnd::OnTimer(nInEvent);
}

afx_msg void CMainWin::OnLButtonDblClk(UINT flags, CPoint Loc)
{
	SetTimer(m_timer, dt_Timer, NULL);
	CWnd::OnLButtonDblClk(flags, Loc);
}

afx_msg void CMainWin::OnRButtonDblClk(UINT flags, CPoint Loc)
{
	KillTimer(m_timer);
	CWnd::OnRButtonDblClk(flags, Loc);
}

CMyApp theApp;