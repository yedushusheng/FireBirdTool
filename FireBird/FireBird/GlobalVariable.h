#pragma once
class GlobalVariable
{
public:
	GlobalVariable(void);
	~GlobalVariable(void);
	int static peer_port; //p2p接收连接监听端口;
	int static peer_max;  //p2p接收连接最大数;
	int static server_port; //服务器监听端口;
	int static chat_port; //聊天室通信端口;
	int static chat_port_client;//聊天室客户端使用端口;
	int static AutoLoginFlag;//自动登录标志;
	CString static server_ip; //服务器地址;
	CString static strDir_up; //上传文件夹路径;
	CString static strDir_down; //下载文件夹路径;
	CString static strDir_userdata;  //用户记录文件路径;
	CString static strDir_system;  //主程序文件路径;
	CString static strIni;  //配置文件路径;
	CString static DownTempLog;//未完成下载任务记录文件;
};

