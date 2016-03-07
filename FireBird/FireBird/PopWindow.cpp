// PopWindow.cpp : 实现文件
//

#include "stdafx.h"
#include "FireBird.h"
#include "PopWindow.h"
#include "afxdialogex.h"


// CPopWindow 对话框

#define IDC_MY_STATIC    2000

HHOOK        g_hHook;    //鼠标钩子句柄
CPopWindow    *g_PopWindow;


LRESULT CALLBACK  MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(wParam == WM_MOUSEMOVE)
	{
		POINT pt;
		pt = ((MOUSEHOOKSTRUCT *)lParam)->pt;
		CRect Crect;
		g_PopWindow->GetClientRect(&Crect);
		g_PopWindow->ClientToScreen(&Crect);
		if(!Crect.PtInRect(pt))
		{
			g_PopWindow->m_Static.ShowWindow(SW_SHOW);
			g_PopWindow->MoveWindow(&g_PopWindow->m_rcSmall);
		}
		else
		{           
			g_PopWindow->m_Static.ShowWindow(SW_HIDE);
			g_PopWindow->MoveWindow(&g_PopWindow->m_rcBig);
		}
	}
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}


IMPLEMENT_DYNAMIC(CPopWindow, CDialog)

	CPopWindow::CPopWindow(CWnd* pParent /*=NULL*/)
	: CDialog(CPopWindow::IDD, pParent)
{
	g_PopWindow = this;
	g_hHook = ::SetWindowsHookEx(WH_MOUSE, MouseProc, 0, GetCurrentThreadId());
}

CPopWindow::~CPopWindow()
{
	if(g_hHook)
	{
		::UnhookWindowsHookEx(g_hHook);
	}
}

void CPopWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPopWindow, CDialog)
END_MESSAGE_MAP()


void CPopWindow::SetImage(CRect &Crect, LPCTSTR lpszPath)
{
	HBITMAP hBmp=(HBITMAP)::LoadImage(
		0,
		lpszPath,
		IMAGE_BITMAP,
		Crect.Width(),
		Crect.Height(),
		LR_LOADFROMFILE
		);
	m_Static.Create(NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, Crect, this, IDC_MY_STATIC);
	m_Static.SetBitmap(hBmp);
}

void CPopWindow::SetSmallRect(CRect &Crect)
{
	m_rcSmall = Crect;
	MoveWindow(&m_rcSmall);
}

void CPopWindow::SetBigRect(CRect &Crect)
{
	m_rcBig = Crect;
}




// CPopWindow 消息处理程序
