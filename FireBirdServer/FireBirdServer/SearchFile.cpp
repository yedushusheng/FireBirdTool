#include "StdAfx.h"
#include "SearchFile.h"
#include "sharedfile.h"



CSearchFile::CSearchFile(void)
{
}


CSearchFile::~CSearchFile(void)
{
}

void CSearchFile::dosearch(TCHAR RecvTemp[])
{
	/***********��������ؼ���****************/
	int i=1;   
	searchwords="";
	do{
		searchwords=searchwords+RecvTemp[i];      
		i++;
	}
	while(RecvTemp[i]!='\0');

	/***********�����ݿ������ļ�****************/
	Csharedfile *m_set=new Csharedfile;
	m_set->Open();
	m_set->MoveFirst();

	nCount=0;

	CString temp;
	while(!m_set->IsEOF())
	{
		temp.Format(_T("%s"),m_set->m_filename);
		CSearchAlgorithm aa;
		if(aa.matching(temp,searchwords))   //�����㷨�����ƶȱȽ�,LCS�㷨;
		{
			FileList[nCount].filename=m_set->m_filename;
			FileList[nCount].filetype=m_set->m_filetype;
			FileList[nCount].filesize=m_set->m_filesize;
			FileList[nCount].fileuser=m_set->m_username;
			nCount++;
		}
		m_set->MoveNext();
	}
	m_set->Close();
	delete m_set;
	m_set=NULL;
}

