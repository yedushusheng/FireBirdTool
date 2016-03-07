#pragma once

// CListenSocket ÃüÁîÄ¿±ê
#include "RecvSocket.h"
class CFireBirdServerDlg;
class CListenSocket : public CSocket
{
public:
	CFireBirdServerDlg *m_dlg;
	void GetMainDlg(CFireBirdServerDlg *dlg);
	CRecvSocket *RecvSocket;
	CListenSocket();
	virtual ~CListenSocket();
	virtual void OnAccept(int nErrorCode);
};


