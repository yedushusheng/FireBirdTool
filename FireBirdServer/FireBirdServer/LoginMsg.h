#pragma once
#include "usermsg.h"
class CLoginMsg
{
private:
	CString username;
	CString password;
	CString email;
	CString temp;
	int image_id;
public:
	void formatting(TCHAR RecvTemp[]);
	void savedata();
	void checkdata();
	CString LoginMessage;
	CLoginMsg(void);
	~CLoginMsg(void);
};

