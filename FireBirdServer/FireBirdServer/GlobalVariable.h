#pragma once
class GlobalVariable
{
public:
	GlobalVariable(void);
	~GlobalVariable(void);
	int static server_port; //服务器接收连接监听端口;
	int static server_max;  //服务器接收连接最大数;
	int static chat_port;  //聊天室通讯端口;
	int static chat_port_client;//聊天室客户端使用端口;
	int static vip_intergral; //会员初始积分;
};

