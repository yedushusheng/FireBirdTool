#include "StdAfx.h"
#include "LoginMsg.h"
#include "userinfo.h"
#include "GlobalVariable.h"

CLoginMsg::CLoginMsg(void)
{
}


CLoginMsg::~CLoginMsg(void)
{
}

void CLoginMsg::formatting(TCHAR RecvTemp[])
{
	int i=1;
	temp="";
	username="";
	password="";
	email="";
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
		email=email+RecvTemp[i];
		i++;
	}while(RecvTemp[i]!='*');
	i++;
	do{
		temp=temp+RecvTemp[i];
		i++;
	}
	while(RecvTemp[i]!='\0');
	image_id = _ttoi(temp);   
}
void CLoginMsg::checkdata()
{
	Cuserinfo m_set;
	m_set.Open();
	m_set.MoveFirst();
	while (!m_set.IsEOF())
	{
		if(m_set.m_username==username) 
		{
			LoginMessage.Format(_T("RU"));
			m_set.Close();
			return;
		}
		if(m_set.m_email==email)
		{
			LoginMessage.Format(_T("RE"));
			m_set.Close();
			return;
		}
		m_set.MoveNext();
	}
	LoginMessage.Format(_T("RY"));
	m_set.Close();
	savedata();
}

void CLoginMsg::savedata()
{
	Cuserinfo *m_set=new Cuserinfo;
	m_set->Open();
	m_set->AddNew();
	m_set->m_username=username;
	m_set->m_password=password;
	m_set->m_email=email;
	m_set->m_integral=GlobalVariable::vip_intergral;
	m_set->m_image_id=image_id;
	m_set->Update();
	m_set->Close();
	delete m_set;
	m_set=NULL;
}
