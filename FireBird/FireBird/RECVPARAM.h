#pragma once
class RECVPARAM
{
public:
	RECVPARAM(void);
	~RECVPARAM(void);
	CSocket *currentSocket;//���յ��Ŀͻ��˵��׽���
	CSocket *serverSocket;//�������˸����������׽���
	SOCKET pSOCKET;
	HWND hwnd;//���������ڵľ��
};
