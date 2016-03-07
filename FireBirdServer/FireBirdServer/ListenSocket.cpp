// ListenSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "FireBirdServer.h"
#include "ListenSocket.h"
#include "FireBirdServerDlg.h"


// CListenSocket

CListenSocket::CListenSocket()
{
}

CListenSocket::~CListenSocket()
{
}


// CListenSocket 成员函数
void CListenSocket::GetMainDlg(CFireBirdServerDlg *dlg)
{
	m_dlg=dlg;
}

DWORD WINAPI ThreadFunc(LPVOID lpParameter)
{
	CRecvSocket *RecvSocket=(CRecvSocket*) lpParameter;
	return 0;
}

void CListenSocket::OnAccept(int nErrorCode)
{
	HANDLE hThread;
	RecvSocket=new CRecvSocket();
	RecvSocket->GetMainDlg(this->m_dlg);
	if (Accept(*RecvSocket))
	{
		hThread=CreateThread(NULL,0,ThreadFunc,RecvSocket,0,NULL);
		RecvSocket=NULL;
		m_dlg->m_ListWords.AddString(_T("接收一个客户端的连接请求！"));
	}
	else
	{
		m_dlg->m_ListWords.AddString(_T("连接客户端失败！"));
		delete RecvSocket;
		RecvSocket=NULL;
	}
	CSocket::OnAccept(nErrorCode);
}
