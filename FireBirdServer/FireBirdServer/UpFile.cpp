#include "StdAfx.h"
#include "UpFile.h"
#include "sharedfile.h"
#include "userinfo.h"

CUpFile::CUpFile(void)
{
}


CUpFile::~CUpFile(void)
{
}

void CUpFile::formatting(TCHAR RecvTemp[])
{
	int i=1;
	filename="";
	username="";
	filetype="";
	filesize="";
	do{
		filename=filename+RecvTemp[i];    
		i++;
	}
	while(RecvTemp[i]!='*');
	i++;
	do{
		username=username+RecvTemp[i];
		i++;
	}
	while(RecvTemp[i]!='*');
	i++;
	do{
		filetype=filetype+RecvTemp[i];
		i++;
	}
	while(RecvTemp[i]!='*');
	i++;
	do{
		filesize=filesize+RecvTemp[i];
		i++;
	}
	while(RecvTemp[i]!='\0');
}

void CUpFile::savefileinfo()
{
	Csharedfile *m_set=new Csharedfile;
	m_set->Open();
	m_set->MoveLast();
	int n=m_set->m_file_id;
	m_set->AddNew();
	m_set->m_file_id=n+1;
	m_set->m_filename=filename;
	m_set->m_username=username;
	m_set->m_filetype=filetype;
	m_set->m_filesize=filesize;
	m_set->Update();
	m_set->Close();
	delete m_set;
	m_set=NULL;
}

void CUpFile::alertintergal()
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
	m_set->m_integral+=5;
	UpMessage.Format(_T("Z%d"),m_set->m_integral);
	m_set->Update();
	m_set->Close();
	delete m_set;
	m_set=NULL;
}