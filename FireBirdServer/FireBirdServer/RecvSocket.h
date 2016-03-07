#pragma once

// CRecvSocket ÃüÁîÄ¿±ê
class CFireBirdServerDlg;
class CRecvSocket : public CSocket
{
public:
	CFireBirdServerDlg *m_dlg;
	void GetMainDlg(CFireBirdServerDlg *dlg);
	CRecvSocket();
	virtual ~CRecvSocket();
	virtual void OnReceive(int nErrorCode);
};


