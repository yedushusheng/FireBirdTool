#pragma once
#include "SearchAlgorithm.h"
class CSearchFile
{	
public:
	CString searchwords;
	struct fileinfo{
		CString filename;
		CString filetype;
		CString filesize;
		CString fileuser;
	};
	fileinfo FileList[200];
	int nCount;
	CSearchAlgorithm m_SearchAlgorithm;
	void dosearch(TCHAR RecvTemp[]);
	CSearchFile(void);
	~CSearchFile(void);
};

