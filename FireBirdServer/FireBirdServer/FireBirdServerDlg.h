
// FireBirdServerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "ListenSocket.h"
#include "RecvSocket.h"
#include "UserMsg.h"
#include "GlobalVariable.h"
#include "SendMail.h"


// CFireBirdServerDlg 对话框
class CFireBirdServerDlg : public CDialogEx
{
// 构造
public:
	CFireBirdServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FIREBIRDSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现

protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListenSocket *ListenSocket;
	CStringArray UserNmae_Online;
	CStringArray IP_Online;
	int UserCount;
	CListBox m_ListWords;
	CString m_SendWords;
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButton1();
	static UINT MainListenThread(LPVOID lparam);
	static UINT MainRecvThread(LPVOID lparam);
	CSendMail sendMail;

public:  //聊天室属性及方法;
	SOCKET m_socket;
	
	BOOL InitSocket();
	struct RECVPARAM
	{
		SOCKET sock;
		HWND hwnd;
	};
	static DWORD WINAPI RecvProc(LPVOID lpParameter);

	struct CHATMESSAGE   //传送消息结构体;
	{
		DWORD nType;
		TCHAR Name[30];
		TCHAR NameTo[30];
		TCHAR Content[100];  //内容;
	}; 

	typedef struct ChatAddr    //聊天室在线用户链表;
	{
		SOCKADDR_IN addr;    //地址;
		CString username;   //用户名;
	    ChatAddr *next;   
	}ChatAddr;

	

	struct ReturnMsg{
		TCHAR flag[4];
		TCHAR msg[100];
	};
	
};
