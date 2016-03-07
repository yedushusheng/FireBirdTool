#pragma once
struct sMailInfo
{
	char* m_pcUserName;
	char* m_pcUserPassword;
	char* m_pcSenderName;
	char* m_pcSender;
	char* m_pcReceiver;
	char* m_pcTitle;
	char* m_pcBody;
	char* m_pcIPAddr;
	char* m_pcIPName;
	sMailInfo(){memset(this,0,sizeof(sMailInfo));}
};
class CSendMail  
{
public:
	CSendMail();
	~CSendMail();
public:
	bool SendMail(sMailInfo &smailInfo);
	void AddFilePath(char* pcFilePath);
	void DeleteFilePath(char* pcFilePath);
	void DeleteAllPath(void);
protected:
	void GetFileName(char* fileName,char* filePath);
	void Char2Base64(char* pBuff64,char* pSrcBuff,int iLen);
	bool CreateSocket(SOCKET &sock);
	bool Logon(SOCKET &sock);
	int GetFileData(char* FilePath);
	bool SendHead(SOCKET &sock);
	bool SendTextBody(SOCKET &sock);
	bool SendFileBody(SOCKET &sock);
	bool SendEnd(SOCKET &sock);
protected:
	CList<char*,char*> m_pcFilePathList;
	char m_cSendBuff[4096];
	char m_cReceiveBuff[1024];
	char* m_pcFileBuff;
public:
	sMailInfo m_sMailInfo;

};
