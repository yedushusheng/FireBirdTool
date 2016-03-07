// ListenSocket.cpp : ʵ���ļ�
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


// CListenSocket ��Ա����
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
		m_dlg->m_ListWords.AddString(_T("����һ���ͻ��˵���������"));
	}
	else
	{
		m_dlg->m_ListWords.AddString(_T("���ӿͻ���ʧ�ܣ�"));
		delete RecvSocket;
		RecvSocket=NULL;
	}
	CSocket::OnAccept(nErrorCode);
}
