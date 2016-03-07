// MySocket.cpp : 实现文件
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

// MySocket 成员函数
/************************************************************************/
/* function:获得主对话框指针                                            */
/************************************************************************/
void MySocket::GetMainDlg(CFireBirdDlg *dlg)
{
	m_dlg=dlg;
}
/************************************************************************/
/* function:接收服务器发送的消息                                        */
/************************************************************************/
void MySocket::OnReceive(int nErrorCode)
{
	m_dlg->ServerRecv();
	CSocket::OnReceive(nErrorCode);
}
