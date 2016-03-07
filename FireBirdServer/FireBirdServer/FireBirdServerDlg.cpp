
// FireBirdServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FireBirdServer.h"
#include "FireBirdServerDlg.h"
#include "afxdialogex.h"
#include "userinfo.h"
#include "LoginMsg.h"
#include "UpFile.h"
#include "SearchFile.h"
#include "Down.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct online_user{
	CString username;
	CString userip;
};
CList<online_user,online_user&> UserList; //�����û��б�;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFireBirdServerDlg �Ի���




CFireBirdServerDlg::CFireBirdServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFireBirdServerDlg::IDD, pParent)
	, m_SendWords(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFireBirdServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListWords);
}

BEGIN_MESSAGE_MAP(CFireBirdServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CFireBirdServerDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON1, &CFireBirdServerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CFireBirdServerDlg ��Ϣ�������

BOOL CFireBirdServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	UserCount=0;  //��ʼ��������������������������Ϊ100;

	
    //�����ҷ�������ʼ������;
	InitSocket();
	RECVPARAM *pRecvParam=new RECVPARAM;
	pRecvParam->sock=m_socket;
	pRecvParam->hwnd=m_hWnd;
	HANDLE hThread=CreateThread(NULL,0,RecvProc,(LPVOID)pRecvParam,0,NULL);   //���������ҽ�����Ϣ�߳�;
	CloseHandle(hThread);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CFireBirdServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFireBirdServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFireBirdServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFireBirdServerDlg::OnBnClickedButton1()   //��Ϣ���������
{
	m_ListWords.ResetContent();
}



UINT CFireBirdServerDlg::MainRecvThread(LPVOID lparam)
{
	CSocket RecvSocket;
	SOCKET hSOCKET=(SOCKET) lparam;
	RecvSocket.Attach(hSOCKET);
	CFireBirdServerDlg *m_dlg=(CFireBirdServerDlg*) AfxGetApp()->m_pMainWnd;

	TCHAR RecvTemp[200],flag[2];

	while(TRUE)
	{
		memset(RecvTemp,0,200);
		DWORD n=RecvSocket.Receive(RecvTemp,200);
		RecvTemp[n/2]='\0';
		flag[0]=RecvTemp[0];
		int nCount=0;
		CString temp;
		temp.Format(_T("%s"),RecvTemp);
		m_dlg->m_ListWords.AddString(temp);

		/*************��Ϣͷ��־************/
		/********��¼��Ϣ     L;************/
		/********ע����Ϣ     R;***********/
		/********�ϴ���Ϣ     U;***********/
		/********������Ϣ     D;***********/
		/********������Ϣ     S;***********/
		/********ע����Ϣ     O;***********/
		/********�˳���Ϣ     Q;***********/
		/********�һ�������Ϣ Y;***********/
		/********�޸�������Ϣ A;***********/
		/***********************************/

		/***********��¼��Ϣ����************/
		if (flag[0]=='L')              
		{
			CUserMsg m_UserInfo;
			m_UserInfo.formatting(RecvTemp);
			m_UserInfo.Login_Check();
			m_dlg->m_ListWords.AddString(m_UserInfo.LoginMessage);
			if (m_UserInfo.login_flag)    //�����¼�ɹ����򽫵�ǰ�û�IP��¼��;
			{
				online_user tempdata;
				tempdata.username=m_UserInfo.username;
				tempdata.userip=m_UserInfo.userip;
				UserList.AddHead(tempdata);
			}
			RecvSocket.Send(m_UserInfo.LoginMessage,200);
		}
		/***********ע����Ϣ����************/
		if (flag[0]=='R')
		{
			CLoginMsg m_LoginInfo;
			m_LoginInfo.formatting(RecvTemp);
			m_LoginInfo.checkdata();
			RecvSocket.Send(m_LoginInfo.LoginMessage,200);
		}
		/***********�ϴ��ļ���Ϣ����************/
		if (flag[0]=='U')
		{
			CUpFile m_UpFile;
			m_UpFile.formatting(RecvTemp);
			m_UpFile.savefileinfo();
			m_UpFile.alertintergal();
			RecvSocket.Send(m_UpFile.UpMessage,200);
		}
		/***********�����ļ���Ϣ����************/
		if (flag[0]=='D')
		{
			CDown m_DownFile;
			m_DownFile.formatting(RecvTemp);
			m_DownFile.alertintergal();
			RecvSocket.Send(m_DownFile.DownMessage,200);
		}
		/***********�����ļ���Ϣ����************/
		if (flag[0]=='S')
		{
			CSearchFile SearchFile;
			SearchFile.dosearch(RecvTemp);
			for (int i=0;i<SearchFile.nCount;i++)
			{
				POSITION  pos = UserList.GetHeadPosition();   
				online_user data;
				while(pos != NULL)
				{
					data = UserList. GetNext(pos);
					if (data.username==SearchFile.FileList[i].fileuser)
					{
						CString Search_Msg;
						Search_Msg.Format(_T("SF%s*%s*%s*%s*%s"),SearchFile.FileList[i].filename,SearchFile.FileList[i].filetype,
							SearchFile.FileList[i].filesize,SearchFile.FileList[i].fileuser,data.userip);
						RecvSocket.Send(Search_Msg,200);
						break;
					}
				}
			}	
			CString EndMsg=_T("SE");
			RecvSocket.Send(EndMsg,200);
		}
		/***********�˳���Ϣ����************/
		if (flag[0]=='Q')
		{
			CString sIP=_T("");
			int i=1;
			do{
				sIP=sIP+RecvTemp[i];
				i++;
			}
			while(RecvTemp[i]!='\0');
			POSITION  pos = UserList.GetHeadPosition();  
			POSITION  pos_delete=pos;
			online_user data;
			while(pos != NULL)
			{
				data = UserList.GetNext(pos);
				if (data.userip==sIP)
				{
					UserList.RemoveAt(pos_delete);  //������û�������Ϣ;
					temp.Format(_T("%s %s�˳��˿ͻ��ˣ��ر����ӣ�"),data.username,data.userip);
					m_dlg->m_ListWords.AddString(temp);
					break;
				}
				pos_delete=pos;
			}
			break;  //����ѭ����������ͻ���ͨѶ�̣߳�
		}
		/***********ע����Ϣ����************/
		if (flag[0]=='O')
		{
			CString sIP=_T("");
			int i=1;
			do{
				sIP=sIP+RecvTemp[i];
				i++;
			}
			while(RecvTemp[i]!='\0');
			POSITION  pos = UserList.GetHeadPosition();  
			POSITION  pos_delete=pos;
			online_user data;
			while(pos != NULL)
			{
				data = UserList.GetNext(pos);
				if (data.userip==sIP)
				{
					UserList.RemoveAt(pos_delete);  //������û�������Ϣ;
					temp.Format(_T("%s %s�˳��˿ͻ��ˣ��ر����ӣ�"),data.username,data.userip);
					m_dlg->m_ListWords.AddString(temp);
					break;
				}
				pos_delete=pos;
			}
		}
		/***********�һ�������Ϣ************/
		if (flag[0]=='Y')
		{
			CUserMsg m_UserInfo;
			m_UserInfo.FindPassword(RecvTemp);
			RecvSocket.Send(m_UserInfo.FindPWMessage,200);
		}
		/***********�һ�������Ϣ************/
		if (flag[0]=='A')
		{
			CUserMsg m_UserInfo;
			m_UserInfo.AlterPassword(RecvTemp);
			RecvSocket.Send(m_UserInfo.AlterPWMessage,200);
		}
	}
	RecvSocket.Close();   //�׽��ֹر�;
	return 0;
}

UINT CFireBirdServerDlg::MainListenThread(LPVOID lparam)
{
	CFireBirdServerDlg *dlg=(CFireBirdServerDlg*)lparam;
	CSocket m_ServerSocket;
	GlobalVariable VariableGlobal;
	m_ServerSocket.Create(VariableGlobal.server_port);
	m_ServerSocket.Listen(VariableGlobal.server_max);
	while(TRUE)
	{
		CSocket m_RecvSocket;
		if (m_ServerSocket.Accept(m_RecvSocket))
		{
			SOCKET hSOCKET=m_RecvSocket.Detach();
			AfxBeginThread(MainRecvThread,(LPVOID)hSOCKET);
			dlg->m_ListWords.AddString(_T("������һ���ͻ��˵���������"));
		}
	}


}

void CFireBirdServerDlg::OnBnClickedButtonStart()
{
	void *dlg=(void*)this;
	AfxBeginThread(MainListenThread,dlg);
	m_ListWords.AddString(_T("�����������������ȴ��ͻ������ӣ�"));
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
}




BOOL CFireBirdServerDlg::InitSocket()    //�������׽��ֳ�ʼ��;
{
	m_socket=socket(AF_INET,SOCK_DGRAM,0);
	if (INVALID_SOCKET==m_socket)
	{
		MessageBox(_T("�׽��ִ���ʧ�ܣ�"));
		return FALSE;
	}
	SOCKADDR_IN addrSocket;
	addrSocket.sin_family=AF_INET;
	addrSocket.sin_port=htons(GlobalVariable::chat_port);     //������������ʹ�ö˿ڣ�6000;�ͻ���:6001;
	addrSocket.sin_addr.S_un.S_addr=htonl(INADDR_ANY);

	int retval;
	//���׽���;
	retval=bind(m_socket,(SOCKADDR*)&addrSocket,sizeof(SOCKADDR));
	if (SOCKET_ERROR==retval)
	{
		closesocket(m_socket);
		MessageBox(_T("��ʧ�ܣ�"));
		return FALSE;
	}
	return TRUE;
}

DWORD WINAPI CFireBirdServerDlg::RecvProc(LPVOID lpParameter)
{
	SOCKET sock=((RECVPARAM*)lpParameter)->sock;
	HWND hwnd=((RECVPARAM*)lpParameter)->hwnd;
	delete lpParameter;

	CFireBirdServerDlg *m_dlg=(CFireBirdServerDlg*) AfxGetApp()->m_pMainWnd;  //������Ի������;
	SOCKADDR_IN addrFrom;    //���յ�ַ��������;
	int len=sizeof(SOCKADDR);  //���յ�ַ����;

	ChatAddr* head=new ChatAddr();//ͷָ��;
	ChatAddr* rear=head;   //βָ��;
	head->next=NULL;
	ChatAddr* q;   //����ָ��;
	int nCount=0; //����������ʼ��0;

	TCHAR recvBuf[200];    //������Ϣ��������;
	CString Content;
	CString Name;

	SOCKET m_sendsocket=socket(AF_INET,SOCK_DGRAM,0);


	while(TRUE)
	{
		//��ȡ���Կͻ��˵���Ϣ;
		memset(recvBuf,0,200);
		recvfrom(sock,(char*)recvBuf,200,0,(SOCKADDR*)&addrFrom,&len);
		
		CHATMESSAGE *szChat_Recv=(CHATMESSAGE*) recvBuf;

		if (szChat_Recv->nType==0)
		{
			CHATMESSAGE *szChat_Send=new CHATMESSAGE;
			szChat_Send->nType=-1;
			q=head->next;
			while(q!=NULL)
			{
				memset(szChat_Send->Name,0,30);
				memcpy(szChat_Send->Name,q->username.GetBuffer(),30);
				q->username.ReleaseBuffer();
				sendto(m_sendsocket,(char*)szChat_Send,sizeof(CHATMESSAGE),0,(SOCKADDR*)&addrFrom,sizeof(SOCKADDR));
				q=q->next;
			} 

			Name.Format(_T("%s"),szChat_Recv->Name);
			rear->next=new ChatAddr();
			rear=rear->next;
			rear->addr.sin_family=AF_INET;
			rear->addr.sin_addr=addrFrom.sin_addr;
			rear->addr.sin_port=htons(GlobalVariable::chat_port_client);
			rear->username=Name;
			rear->next=NULL;
					
			nCount++;//�����û�������1;

			
			q=head->next;
			while(q!=NULL)
			{
				sendto(m_sendsocket,(char*)szChat_Recv,sizeof(CHATMESSAGE),0,(SOCKADDR*)&q->addr,sizeof(SOCKADDR));
				q=q->next;
			} 
			
			szChat_Recv->nType=-1;
			
			
		}
		if (szChat_Recv->nType==1)
		{
			Name.Format(_T("%s"),szChat_Recv->Name);
			
			q=head;
			do 
			{
				if (q->next->username==Name)
				{
					if (q->next->next==NULL)  //βָ�봦��;
						rear=q;
					q->next=q->next->next;  //ɾ�����;
					break;
				}
			} while (q->next==NULL);

			nCount--;

			q=head->next;
			while(q!=NULL)
			{
				sendto(m_sendsocket,(char*)szChat_Recv,sizeof(CHATMESSAGE),0,(SOCKADDR*)&q->addr,sizeof(SOCKADDR));
				q=q->next;
			} 
		}
		if (szChat_Recv->nType==2)
		{
			q=head->next;
			while(q!=NULL)
			{
				sendto(m_sendsocket,(char*)szChat_Recv,sizeof(CHATMESSAGE),0,(SOCKADDR*)&q->addr,sizeof(SOCKADDR));
				q=q->next;
			} 
		}
		if (szChat_Recv->nType==3)
		{
			Name.Format(_T("%s"),szChat_Recv->NameTo);
			q=head->next;
			while(q!=NULL)
			{
				if (Name.CompareNoCase(q->username)==0)
				{
					sendto(m_sendsocket,(char*)szChat_Recv,sizeof(CHATMESSAGE),0,(SOCKADDR*)&q->addr,sizeof(SOCKADDR));
					break;
				}
				q=q->next;
			} 
		}
	  
		
	}
	return 0;
}
