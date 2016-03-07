#include "StdAfx.h"
#include "GlobalVariable.h"


GlobalVariable::GlobalVariable(void)
{
}


GlobalVariable::~GlobalVariable(void)
{
}


int GlobalVariable::server_port=5000; //服务器接收连接监听端口;
int GlobalVariable::server_max=200; //服务器接收连接最大数;
int GlobalVariable::chat_port=6000; //聊天室通讯端口;
int GlobalVariable::chat_port_client=6001; //聊天室客户端使用端口;
int GlobalVariable::vip_intergral=10; //会员初始积分;
