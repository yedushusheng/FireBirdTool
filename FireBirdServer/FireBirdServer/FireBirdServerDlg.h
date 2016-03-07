
// FireBirdServerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "ListenSocket.h"
#include "RecvSocket.h"
#include "UserMsg.h"
#include "GlobalVariable.h"
#include "SendMail.h"


// CFireBirdServerDlg �Ի���
class CFireBirdServerDlg : public CDialogEx
{
// ����
public:
	CFireBirdServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FIREBIRDSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��

protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

public:  //���������Լ�����;
	SOCKET m_socket;
	
	BOOL InitSocket();
	struct RECVPARAM
	{
		SOCKET sock;
		HWND hwnd;
	};
	static DWORD WINAPI RecvProc(LPVOID lpParameter);

	struct CHATMESSAGE   //������Ϣ�ṹ��;
	{
		DWORD nType;
		TCHAR Name[30];
		TCHAR NameTo[30];
		TCHAR Content[100];  //����;
	}; 

	typedef struct ChatAddr    //�����������û�����;
	{
		SOCKADDR_IN addr;    //��ַ;
		CString username;   //�û���;
	    ChatAddr *next;   
	}ChatAddr;

	

	struct ReturnMsg{
		TCHAR flag[4];
		TCHAR msg[100];
	};
	
};
