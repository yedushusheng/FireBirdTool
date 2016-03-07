#pragma once
class CUpFile
{
private:
	CString filename;
	CString username;
	CString filetype;
	CString filesize;

public:
	void formatting(TCHAR RecvTemp[]);
	void savefileinfo();
	void alertintergal();
	CString UpMessage;
	CUpFile(void);
	~CUpFile(void);
};

