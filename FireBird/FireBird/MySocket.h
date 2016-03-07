#pragma once

// MySocket ÃüÁîÄ¿±ê
class CFireBirdDlg;
class MySocket : public CSocket
{
public:
	CFireBirdDlg *m_dlg;
	void GetMainDlg(CFireBirdDlg *dlg);
	MySocket();
	virtual ~MySocket();
	virtual void OnReceive(int nErrorCode);
};


