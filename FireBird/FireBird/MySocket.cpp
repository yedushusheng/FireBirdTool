// MySocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FireBird.h"
#include "MySocket.h"
#include "FireBirdDlg.h"

// MySocket

MySocket::MySocket()
{
}

MySocket::~MySocket()
{
}

// MySocket ��Ա����
/************************************************************************/
/* function:������Ի���ָ��                                            */
/************************************************************************/
void MySocket::GetMainDlg(CFireBirdDlg *dlg)
{
	m_dlg=dlg;
}
/************************************************************************/
/* function:���շ��������͵���Ϣ                                        */
/************************************************************************/
void MySocket::OnReceive(int nErrorCode)
{
	m_dlg->ServerRecv();
	CSocket::OnReceive(nErrorCode);
}
