#include "StdAfx.h"
#include "UserMsg.h"
#include "userinfo.h"
CUserMsg::CUserMsg(void)
{
}


CUserMsg::~CUserMsg(void)
{
}

void CUserMsg::formatting(TCHAR RecvTemp[])
{
	int i=1;
	username="";
	password="";
	userip="";
	do{
		username=username+RecvTemp[i];
		i++;
	}
	while(RecvTemp[i]!='*');
	i++;
	do{
		password=password+RecvTemp[i];
		i++;
	}
	while(RecvTemp[i]!='*');
	i++;
	do{
		userip=userip+RecvTemp[i];
		i++;
	}
	while(RecvTemp[i]!='\0');

}

void CUserMsg::Login_Check()
{
	Cuserinfo *m_set=new Cuserinfo;
	login_flag=FALSE;
	m_set->Open();
	m_set->MoveFirst();
	while (!m_set->IsEOF())
	{
		if((m_set->m_username==username)&&(m_set->m_password==password)) 
		{
			LoginMessage.Format(_T("LY%d*%d"),m_set->m_integral,m_set->m_image_id);
			login_flag=TRUE;
			return;
		}
		m_set->MoveNext();
	}
	LoginMessage.Format(_T("LN"));
	m_set->Close();
	delete m_set;
	m_set=NULL;
}

void CUserMsg::FindPassword(TCHAR RecvTemp[])
{
	CString email=_T("");
	int i=1;
	do{
		email=email+RecvTemp[i];
		i++;
	}
	while(RecvTemp[i]!='\0');
	Cuserinfo m_set;
	m_set.Open();
	m_set.MoveFirst();
	while (!m_set.IsEOF())
	{
		if(m_set.m_email==email) 
		{
			FindPWMessage.Format(_T("YY"));
			m_set.Edit();
			m_set.m_password=_T("123456");
			m_set.Update();
			username=m_set.m_username;
			m_email=m_set.m_email;
			EmailMessage();
			m_set.Close();
			return;
		}
		m_set.MoveNext();
	}
	FindPWMessage.Format(_T("YN"));
	m_set.Close();
}

void CUserMsg::EmailMessage()
{
	CStringA	m_UserName;
	CStringA	m_UserPassword;
	CStringA	m_SendName;
	CStringA	m_Sender;
	CStringA	m_Receiver;
	CStringA	m_Title;
	CStringA	m_Body;

	m_UserName=_T("FireBirdDown@163.com");
	m_UserPassword=_T("liuziliang");
	m_SendName=_T("FireBird_Admin");
	m_Sender=_T("FireBirdDown@163.com");
	m_Receiver=CStringA(m_email);
	m_Title=_T("FireBird下载系统找回密码");
	CString temp;
	temp.Format(_T("您的用户名:%s,新密码：123456"),username);
	m_Body=CStringA(temp);
	sendMail.m_sMailInfo.m_pcBody=(char*)(const char*)(m_Body);
	sendMail.m_sMailInfo.m_pcIPAddr="202.106.186.34";
	sendMail.m_sMailInfo.m_pcIPName="smtp.163.com";
	sendMail.m_sMailInfo.m_pcReceiver=(char*)(const char*)(m_Receiver);
	sendMail.m_sMailInfo.m_pcSender=(char*)(const char*)(m_Sender);
	sendMail.m_sMailInfo.m_pcSenderName=(char*)(const char*)(m_SendName);
	sendMail.m_sMailInfo.m_pcTitle=(char*)(const char*)(m_Title);
	sendMail.m_sMailInfo.m_pcUserName=(char*)(const char*)(m_UserName);
	sendMail.m_sMailInfo.m_pcUserPassword=(char*)(const char*)(m_UserPassword);
	sendMail.SendMail(sendMail.m_sMailInfo);
}

void CUserMsg::AlterPassword(TCHAR RecvTemp[])
{
	CString newpassword;
	username="";
	password="";
	newpassword="";
	int i=1;
	do{
		username=username+RecvTemp[i];
		i++;
	}
	while(RecvTemp[i]!='*');
	i++;
	do{
		password=password+RecvTemp[i];
		i++;
	}
	while(RecvTemp[i]!='*');
	i++;
	do{
		newpassword=newpassword+RecvTemp[i];
		i++;
	}
	while(RecvTemp[i]!='\0');
	Cuserinfo m_set;
	m_set.Open();
	m_set.MoveFirst();
	while (!m_set.IsEOF())
	{
		if((m_set.m_username==username)&&(m_set.m_password==password)) 
		{
			m_set.Edit();
			m_set.m_password=newpassword;
			m_set.Update();
			m_set.Close();
			AlterPWMessage.Format(_T("AY"));
			return;
		}
		m_set.MoveNext();
	}
	AlterPWMessage.Format(_T("AN"));
	m_set.Close();
}