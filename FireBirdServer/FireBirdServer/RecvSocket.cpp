// RecvSocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FireBirdServer.h"
#include "RecvSocket.h"
#include "FireBirdServerDlg.h"


// CRecvSocket

CRecvSocket::CRecvSocket()
{
}

CRecvSocket::~CRecvSocket()
{
}


// CRecvSocket ��Ա����
void CRecvSocket::GetMainDlg(CFireBirdServerDlg *dlg)
{
	m_dlg=dlg;
}




void CRecvSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	CSocket::OnReceive(nErrorCode);
}
