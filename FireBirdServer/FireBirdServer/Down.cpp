#include "StdAfx.h"
#include "Down.h"
#include "userinfo.h"

CDown::CDown(void)
{
}


CDown::~CDown(void)
{
}

void CDown::formatting(TCHAR RecvTemp[])
{
	int i=1;
	username="";
	do{
		username=username+RecvTemp[i];
		i++;
	}
	while(RecvTemp[i]!='\0');
}

void CDown::alertintergal()
{
	Cuserinfo *m_set=new Cuserinfo;
	m_set->Open();
	m_set->MoveFirst();
	while (!m_set->IsEOF())
	{
		if(m_set->m_username==username) 
			break;
		m_set->MoveNext();
	}
	m_set->Edit();
	m_set->m_integral-=2;
	DownMessage.Format(_T("Z%d"),m_set->m_integral);
	m_set->Update();
	m_set->Close();
	delete m_set;
	m_set=NULL;
}
