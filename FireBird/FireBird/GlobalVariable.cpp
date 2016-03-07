#include "StdAfx.h"
#include "GlobalVariable.h"


GlobalVariable::GlobalVariable(void)
{
}


GlobalVariable::~GlobalVariable(void)
{
}

int GlobalVariable::peer_port=7000; //p2p接收连接监听端口;
int GlobalVariable::peer_max=5; //p2p接收连接最大数;
int GlobalVariable::chat_port_client=6001; //聊天室客户端使用端口;
int GlobalVariable::chat_port=6000; //聊天室通信端口;
int GlobalVariable::server_port=5000;//服务器监听端口;
int GlobalVariable::AutoLoginFlag=0;//自动登录标志;
CString GlobalVariable::server_ip=_T("192.168.1.100"); //服务器地址;
CString GlobalVariable::strDir_up=_T("D:\\FireBirdUpFile\\");//上传文件夹路径;
CString GlobalVariable::strDir_down=_T("D:\\FireBirdDownloads\\");//下载文件夹路径;
CString GlobalVariable::strDir_system=_T("C:\\FireBird\\");//主程序文件路径;
CString GlobalVariable::strDir_userdata=GlobalVariable::strDir_system+_T("User\\");//用户记录文件路径;
CString GlobalVariable::strIni=GlobalVariable::strDir_system+_T("FireBird.ini");//配置文件路径;
CString GlobalVariable::DownTempLog=_T("DownTemp.log");//未完成下载任务记录文件;