
// FireBirdServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFireBirdServerApp:
// �йش����ʵ�֣������ FireBirdServer.cpp
//

class CFireBirdServerApp : public CWinApp
{
public:
	CFireBirdServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFireBirdServerApp theApp;