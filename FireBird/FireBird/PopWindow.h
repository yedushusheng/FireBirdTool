#pragma once

#ifndef CPOPWINDOW_H
#define CPOPWINDOW_H

// CPopWindow 对话框

class CPopWindow : public CDialog
{
	DECLARE_DYNAMIC(CPopWindow)

public:
	CPopWindow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPopWindow();

// 对话框数据
	enum { IDD = IDD_POPWINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	// DDX/DDV 支持
public:
	void    SetSmallRect(CRect &Crect);
	void    SetBigRect(CRect &Crect);
	void    SetImage(CRect &Crect, LPCTSTR lpszPath);

	DECLARE_MESSAGE_MAP()

public:
	CStatic            m_Static;
	CRect            m_rcBig;
	CRect            m_rcSmall;
};

#endif 
