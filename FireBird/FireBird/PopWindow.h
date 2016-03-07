#pragma once

#ifndef CPOPWINDOW_H
#define CPOPWINDOW_H

// CPopWindow �Ի���

class CPopWindow : public CDialog
{
	DECLARE_DYNAMIC(CPopWindow)

public:
	CPopWindow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPopWindow();

// �Ի�������
	enum { IDD = IDD_POPWINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	// DDX/DDV ֧��
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
