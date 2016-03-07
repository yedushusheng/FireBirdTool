#pragma once
#include "BmpButton.h"
#include "afxwin.h"
#include "afxcmn.h"

// CChatRoom 对话框
#define WM_RECVDATA WM_USER+100

class CChatRoom : public CDialogEx
{
	DECLARE_DYNAMIC(CChatRoom)

public:
	CChatRoom(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChatRoom();

// 对话框数据
	enum { IDD = IDD_CHATROOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CBmpButton m_send;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	
	CComboBox m_talksel;

	SOCKET m_socket;
	SOCKET m_sendsocket;
	BOOL InitSocket();
	
	//DWORD WINAPI RecvProc(LPVOID lpParameter);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedSend();
	
	CString ChatName;
	CString m_SendWords;
	int m_SendType;

	SOCKADDR_IN addrTo;

	CFont *m_Font;

	CImageList m_ImageOnlineList;
	
	afx_msg void OnClose();
	afx_msg void OnSelchangeCombo1();
	CListBox m_ListNotice;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
};
