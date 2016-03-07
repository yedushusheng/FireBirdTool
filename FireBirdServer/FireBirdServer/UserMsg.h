#pragma once
#include "SendMail.h"
class CUserMsg
{	
public:
	CString username;
	CString password;
	CString userip;
	CString LoginMessage;
	CString FindPWMessage;
	CString AlterPWMessage;
	CString m_email;
	BOOL login_flag;
	CSendMail sendMail;
	void formatting(TCHAR RecvTemp[]);
	void Login_Check();
	void FindPassword(TCHAR RecvTemp[]);
	void EmailMessage();
	void AlterPassword(TCHAR RecvTemp[]);
	CUserMsg(void);
	~CUserMsg(void);
};

