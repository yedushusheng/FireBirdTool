#include "StdAfx.h"
#include "GlobalVariable.h"


GlobalVariable::GlobalVariable(void)
{
}


GlobalVariable::~GlobalVariable(void)
{
}

int GlobalVariable::peer_port=7000; //p2p�������Ӽ����˿�;
int GlobalVariable::peer_max=5; //p2p�������������;
int GlobalVariable::chat_port_client=6001; //�����ҿͻ���ʹ�ö˿�;
int GlobalVariable::chat_port=6000; //������ͨ�Ŷ˿�;
int GlobalVariable::server_port=5000;//�����������˿�;
int GlobalVariable::AutoLoginFlag=0;//�Զ���¼��־;
CString GlobalVariable::server_ip=_T("192.168.1.100"); //��������ַ;
CString GlobalVariable::strDir_up=_T("D:\\FireBirdUpFile\\");//�ϴ��ļ���·��;
CString GlobalVariable::strDir_down=_T("D:\\FireBirdDownloads\\");//�����ļ���·��;
CString GlobalVariable::strDir_system=_T("C:\\FireBird\\");//�������ļ�·��;
CString GlobalVariable::strDir_userdata=GlobalVariable::strDir_system+_T("User\\");//�û���¼�ļ�·��;
CString GlobalVariable::strIni=GlobalVariable::strDir_system+_T("FireBird.ini");//�����ļ�·��;
CString GlobalVariable::DownTempLog=_T("DownTemp.log");//δ������������¼�ļ�;