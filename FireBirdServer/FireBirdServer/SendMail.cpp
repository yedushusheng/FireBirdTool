#include "StdAfx.h"
#include "SendMail.h"


CSendMail::CSendMail(void)
{
	m_pcFileBuff=NULL;
	memset(m_cSendBuff,0,sizeof(m_cSendBuff));
	memset(m_cReceiveBuff,0,sizeof(m_cReceiveBuff));
}

CSendMail::~CSendMail()
{
	DeleteAllPath();

}

void CSendMail::Char2Base64(char* pBuff64,char* pSrcBuff,int iLen)
{
	static char Base64Encode[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int point;
	point = 2;
	int i;
	int iIndex;
	char n=0;
	for(i=0;i<iLen;i++)
	{
		if(point==2)
		{
			iIndex=((*pSrcBuff)>>point)&0x3f;
		}
		else if(point==4)
		{
			iIndex=((*pSrcBuff)>>point)&0xf;
		}
		else if(point==6)
		{
			iIndex=((*pSrcBuff)>>point)&0x3;
		}
		iIndex += n;
		*pBuff64++ = Base64Encode[iIndex];
		n=((*pSrcBuff)<<(6-point));
		n=n&0x3f;
		point+=2;
		if(point==8)
		{
			iIndex=(*pSrcBuff)&0x3f;
			*pBuff64++=Base64Encode[iIndex];
			n=0;
			point=2;
		}
		pSrcBuff++;
	}
	if(n!=0)
	{
		*pBuff64++=Base64Encode[n];
	}
	if(iLen%3==2)
	{
		*pBuff64='=';
	}
	else if(iLen%3==1)
	{
		*pBuff64++='=';
		*pBuff64='=';
	}
}

void CSendMail::AddFilePath(char* pcFilePath)
{
	if(pcFilePath==NULL)
	{
		return;
	}
	int i;
	char* temp;
	for(i=0;i<m_pcFilePathList.GetCount();i++)
	{
		temp=m_pcFilePathList.GetAt(m_pcFilePathList.FindIndex(i));
		if(strcmp(pcFilePath,temp)==0)
		{
			return;
		}
	}
	m_pcFilePathList.AddTail(pcFilePath);
	delete[] temp;
}

void CSendMail::DeleteFilePath(char* pcFilePath)
{
	int i;
	char* temp;
	for(i=0;i<m_pcFilePathList.GetCount();i++)
	{
		temp=m_pcFilePathList.GetAt(m_pcFilePathList.FindIndex(i));
		if(strcmp(temp,pcFilePath)==0)
		{
			m_pcFilePathList.RemoveAt(m_pcFilePathList.FindIndex(i));
			delete[] temp;
			return;
		}
	}
}

void CSendMail::DeleteAllPath(void)
{
	m_pcFilePathList.RemoveAll();
}

int CSendMail::GetFileData(char* FilePath)
{
	m_pcFileBuff=NULL;
	if(FilePath==NULL)
	{
		return 0;
	}
	CFile f;
	int len;
	//	USES_CONVERSION; A2W(FilePath)
	//if(!f.Open(FilePath,CFile::modeRead|CFile::modeNoTruncate|CFile::typeBinary))
	//{
		//return 0;
	//}
	len=(int)f.GetLength();
	m_pcFileBuff=new char[len+1];
	memset(m_pcFileBuff,0,len+1);
	f.Read(m_pcFileBuff,len);
	f.Close();
	return len;
}

void CSendMail::GetFileName(char* fileName,char* filePath)
{
	if(filePath==NULL||fileName==NULL)
	{
		return;
	}
	int i;
	for(i=0;i<(int)strlen(filePath);i++)
	{
		if(filePath[strlen(filePath)-1-i]=='\\')
		{
			memcpy(fileName,&filePath[strlen(filePath)-i],i);
			return;
		}
	}
}

bool CSendMail::CreateSocket(SOCKET &sock)
{
	sock=socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
	if(sock == INVALID_SOCKET)
	{
		return false;
	}
	sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(sockaddr_in));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(25);
	if(m_sMailInfo.m_pcIPName=="")
	{
		servaddr.sin_addr.S_un.S_addr=inet_addr(m_sMailInfo.m_pcIPAddr);
	}
	else
	{
		struct hostent *hp=gethostbyname(m_sMailInfo.m_pcIPName);
		servaddr.sin_addr.S_un.S_addr=*(int*)(*hp->h_addr_list);
	}

	int ret=connect(sock,(sockaddr*)&servaddr,sizeof(servaddr));
	if(ret == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

bool CSendMail::Logon(SOCKET &sock)
{
	recv(sock,m_cReceiveBuff,1024,0);
	memset(m_cSendBuff,0,sizeof(m_cSendBuff));
	//	sprintf_s(m_cSendBuff,"HELO []\r\n");
	sprintf(m_cSendBuff,"HELO []\r\n");
	//	_snprintf(m_cSendBuff,"HELO []\r\n");
	send(sock,m_cSendBuff,strlen(m_cSendBuff),0);
	recv(sock,m_cReceiveBuff,1024,0);
	if(m_cReceiveBuff[0]!='2'||m_cReceiveBuff[1]!='5'||m_cReceiveBuff[2]!='0')
	{
		return false;
	}
	memset(m_cSendBuff,0,sizeof(m_cSendBuff));
	sprintf(m_cSendBuff,"AUTH LOGIN\r\n");
	send(sock,m_cSendBuff,strlen(m_cSendBuff),0);
	recv(sock,m_cReceiveBuff,1024,0);
	if(m_cReceiveBuff[0]!='3'||m_cReceiveBuff[1]!='3'||m_cReceiveBuff[2]!='4')
	{
		return false;
	}
	memset(m_cSendBuff,0,sizeof(m_cSendBuff));
	Char2Base64(m_cSendBuff,m_sMailInfo.m_pcUserName,strlen(m_sMailInfo.m_pcUserName));
	m_cSendBuff[strlen(m_cSendBuff)]='\r';
	m_cSendBuff[strlen(m_cSendBuff)]='\n';
	send(sock,m_cSendBuff,strlen(m_cSendBuff),0);
	recv(sock,m_cReceiveBuff,1024,0);
	if(m_cReceiveBuff[0]!='3'||m_cReceiveBuff[1]!='3'||m_cReceiveBuff[2]!='4')
	{
		return false;
	}
	memset(m_cSendBuff,0,sizeof(m_cSendBuff));
	Char2Base64(m_cSendBuff,m_sMailInfo.m_pcUserPassword,strlen(m_sMailInfo.m_pcUserPassword));
	m_cSendBuff[strlen(m_cSendBuff)]='\r';
	m_cSendBuff[strlen(m_cSendBuff)]='\n';
	send(sock,m_cSendBuff,strlen(m_cSendBuff),0);
	recv(sock,m_cReceiveBuff,1024,0);
	if(m_cReceiveBuff[0]!='2'||m_cReceiveBuff[1]!='3'||m_cReceiveBuff[2]!='5')
	{
		return false;
	}
	return true;
}

bool CSendMail::SendHead(SOCKET &sock)
{
	int rt;
	memset(m_cSendBuff,0,sizeof(m_cSendBuff));
	sprintf(m_cSendBuff,"MAIL FROM:<%s>\r\n",m_sMailInfo.m_pcSender);
	rt=send(sock,m_cSendBuff,strlen(m_cSendBuff),0);
	if(rt != (int)strlen(m_cSendBuff))
	{
		return false;
	}
	recv(sock,m_cReceiveBuff,1024,0);
	memset(m_cSendBuff,0,sizeof(m_cSendBuff));
	sprintf(m_cSendBuff,"RCPT TO:<%s>\r\n",m_sMailInfo.m_pcReceiver);
	rt=send(sock,m_cSendBuff,strlen(m_cSendBuff),0);
	if(rt!=(int)strlen(m_cSendBuff))
	{
		return false;
	}
	recv(sock,m_cReceiveBuff,1024,0);
	memset(m_cSendBuff,0,sizeof(m_cSendBuff));
	memcpy(m_cSendBuff,"DATA\r\n",strlen("DATA\r\n"));
	rt=send(sock,m_cSendBuff,strlen(m_cSendBuff),0);
	if(rt!=(int)strlen(m_cSendBuff))
	{
		return false;
	}
	recv(sock,m_cReceiveBuff,1024,0);
	memset(m_cSendBuff,0,sizeof(m_cSendBuff));
	sprintf(m_cSendBuff,"From:\"%s\"<%s>\r\n",m_sMailInfo.m_pcSenderName,m_sMailInfo.m_pcSender);
	//	sprintf_s(&m_cSendBuff[strlen(m_cSendBuff)],150,"TO:\"INVT.COM.CN\"<%s>\r\n",m_sMailInfo.m_pcReceiver);
	//	sprintf_s(&m_cSendBuff[strlen(m_cSendBuff)],150,"Subject:%s\r\nMime-Version:1.0\r\nContent=Type:multipart/mixed;boundary=\"INVT\"\r\n\r\n",m_sMailInfo.m_pcTitle);
	sprintf(&m_cSendBuff[strlen(m_cSendBuff)],"TO:\"INVT.COM.CN\"<%s>\r\n",m_sMailInfo.m_pcReceiver);
	sprintf(&m_cSendBuff[strlen(m_cSendBuff)],"Subject:%s\r\nMime-Version:1.0\r\nContent=Type:multipart/mixed;boundary=\"INVT\"\r\n\r\n",m_sMailInfo.m_pcTitle);
	rt=send(sock,m_cSendBuff,strlen(m_cSendBuff),0);
	if(rt!=(int)strlen(m_cSendBuff))
	{
		return false;
	}
	return true;
}

bool CSendMail::SendTextBody(SOCKET &sock)
{
	int rt;
	memset(m_cSendBuff,0,sizeof(m_cSendBuff));
	sprintf(m_cSendBuff,
		"--INVT\r\n--取回密码说明\r\n尊敬的用户你好，这封信是由FireBird下载系统发送的。\r\n您收到这封邮件，是由于这个邮箱地址在FireBird下载系统被登记为用户邮箱,且用户请求找回密码功能所致\r\n\r\n------------------------------------\r\n%s\r\n------------------------------------\r\n\r\n此密码为默认找回密码，建议您立即登录，“FireBird下载系统”用户中心，修改密码\r\n感谢您使用FireBird下载系统！\r\n\r\n",
		m_sMailInfo.m_pcBody);
	rt=send(sock,m_cSendBuff,strlen(m_cSendBuff),0);
	if(rt!=(int)strlen(m_cSendBuff))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CSendMail::SendFileBody(SOCKET &sock)
{
	int i;
	char* filePath;
	int rt;
	int len;
	int pt=0;
	char fileName[128];
	for(i=0;i<m_pcFilePathList.GetCount();i++)
	{
		pt=0;
		memset(fileName,0,128);
		filePath=m_pcFilePathList.GetAt(m_pcFilePathList.FindIndex(i));
		len=GetFileData(filePath);
		GetFileName(fileName,filePath);

		sprintf(m_cSendBuff,"--INVT\r\nContent-Type:application/octet-stream;\r\n name=\"%s\"\r\nContent-Transfer-Encoding:base64\r\nContent-Disposition: attachment;\r\nfilename=\"%s\"\r\n\r\n",fileName,fileName);
		send(sock,m_cSendBuff,strlen(m_cSendBuff),0);
		while(pt<len)
		{
			memset(m_cSendBuff,0,sizeof(m_cSendBuff));
			Char2Base64(m_cSendBuff,&m_pcFileBuff[pt],min(len-pt,3000));
			m_cSendBuff[strlen(m_cSendBuff)]='\r';
			m_cSendBuff[strlen(m_cSendBuff)]='\n';
			rt=send(sock,m_cSendBuff,strlen(m_cSendBuff),0);
			pt+=min(len-pt,3000);
			if(rt!=(int)strlen(m_cSendBuff))
			{
				return false;
			}
		}
		if(len!=0)
		{
			delete [] m_pcFileBuff;
		}
	}
	return true;
}

bool CSendMail::SendEnd(SOCKET &sock)
{
	sprintf(m_cSendBuff,"--INVT--\r\n.\r\n");
	send(sock,m_cSendBuff,strlen(m_cSendBuff),0);
	sprintf(m_cSendBuff,"QUIT\r\n");
	send(sock,m_cSendBuff,strlen(m_cSendBuff),0);
	closesocket(sock);
	WSACleanup();
	return true;
}

bool CSendMail::SendMail(sMailInfo &smailInfo)
{
	memcpy(&m_sMailInfo,&smailInfo,sizeof(smailInfo));
	if(m_sMailInfo.m_pcBody==NULL||m_sMailInfo.m_pcIPAddr==NULL||m_sMailInfo.m_pcIPName==NULL||m_sMailInfo.m_pcReceiver==NULL||m_sMailInfo.m_pcSender==NULL||m_sMailInfo.m_pcSenderName==NULL||m_sMailInfo.m_pcTitle==NULL||m_sMailInfo.m_pcUserName==NULL||m_sMailInfo.m_pcUserPassword==NULL)
	{
		AfxMessageBox(_T("Please input every information!"));
		return false;
	}
	SOCKET sock;
	if(!CreateSocket(sock))
	{
		AfxMessageBox(_T("Create socket error!"));
		return false;
	}
	if(!Logon(sock))
	{
		AfxMessageBox(_T("logon the mail server error!"));
		return false;
	}
	if(!SendHead(sock))
	{
		AfxMessageBox(_T("Send head error!"));
		return false;
	}
	if(!SendTextBody(sock))
	{
		AfxMessageBox(_T("Send Text-Body error!"));
		return false;
	}
	if(!SendFileBody(sock))
	{
		AfxMessageBox(_T("Send file error!"));
		return false;
	}
	if(!SendEnd(sock))
	{
		AfxMessageBox(_T("Send end error!"));
		return false;
	}
	return true;
}