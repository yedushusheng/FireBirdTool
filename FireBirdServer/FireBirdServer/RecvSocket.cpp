// RecvSocket.cpp : 实现文件
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


// CRecvSocket 成员函数
void CRecvSocket::GetMainDlg(CFireBirdServerDlg *dlg)
{
	m_dlg=dlg;
}




void CRecvSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CSocket::OnReceive(nErrorCode);
}
