#pragma once
class GlobalVariable
{
public:
	GlobalVariable(void);
	~GlobalVariable(void);
	int static peer_port; //p2p�������Ӽ����˿�;
	int static peer_max;  //p2p�������������;
	int static server_port; //�����������˿�;
	int static chat_port; //������ͨ�Ŷ˿�;
	int static chat_port_client;//�����ҿͻ���ʹ�ö˿�;
	int static AutoLoginFlag;//�Զ���¼��־;
	CString static server_ip; //��������ַ;
	CString static strDir_up; //�ϴ��ļ���·��;
	CString static strDir_down; //�����ļ���·��;
	CString static strDir_userdata;  //�û���¼�ļ�·��;
	CString static strDir_system;  //�������ļ�·��;
	CString static strIni;  //�����ļ�·��;
	CString static DownTempLog;//δ������������¼�ļ�;
};

