#pragma once
class RECVPARAM
{
public:
	RECVPARAM(void);
	~RECVPARAM(void);
	CSocket *currentSocket;//接收到的客户端的套接字
	CSocket *serverSocket;//服务器端负责侦听的套接字
	SOCKET pSOCKET;
	HWND hwnd;//服务器窗口的句柄
};
