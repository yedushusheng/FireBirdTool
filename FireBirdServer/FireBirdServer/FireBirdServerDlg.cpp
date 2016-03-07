
// FireBirdServerDlg.cpp : 实现文件
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
CList<online_user,online_user&> UserList; //在线用户列表;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CFireBirdServerDlg 对话框




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


// CFireBirdServerDlg 消息处理程序

BOOL CFireBirdServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	UserCount=0;  //初始化在线人数，在线人数上限设为100;

	
    //聊天室服务器初始化代码;
	InitSocket();
	RECVPARAM *pRecvParam=new RECVPARAM;
	pRecvParam->sock=m_socket;
	pRecvParam->hwnd=m_hWnd;
	HANDLE hThread=CreateThread(NULL,0,RecvProc,(LPVOID)pRecvParam,0,NULL);   //创建聊天室接收信息线程;
	CloseHandle(hThread);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFireBirdServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFireBirdServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFireBirdServerDlg::OnBnClickedButton1()   //消息框内容清空
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

		/*************消息头标志************/
		/********登录消息     L;************/
		/********注册消息     R;***********/
		/********上传消息     U;***********/
		/********下载消息     D;***********/
		/********搜索消息     S;***********/
		/********注销消息     O;***********/
		/********退出消息     Q;***********/
		/********找回密码消息 Y;***********/
		/********修改密码消息 A;***********/
		/***********************************/

		/***********登录消息处理************/
		if (flag[0]=='L')              
		{
			CUserMsg m_UserInfo;
			m_UserInfo.formatting(RecvTemp);
			m_UserInfo.Login_Check();
			m_dlg->m_ListWords.AddString(m_UserInfo.LoginMessage);
			if (m_UserInfo.login_flag)    //如果登录成功，则将当前用户IP记录下;
			{
				online_user tempdata;
				tempdata.username=m_UserInfo.username;
				tempdata.userip=m_UserInfo.userip;
				UserList.AddHead(tempdata);
			}
			RecvSocket.Send(m_UserInfo.LoginMessage,200);
		}
		/***********注册消息处理************/
		if (flag[0]=='R')
		{
			CLoginMsg m_LoginInfo;
			m_LoginInfo.formatting(RecvTemp);
			m_LoginInfo.checkdata();
			RecvSocket.Send(m_LoginInfo.LoginMessage,200);
		}
		/***********上传文件消息处理************/
		if (flag[0]=='U')
		{
			CUpFile m_UpFile;
			m_UpFile.formatting(RecvTemp);
			m_UpFile.savefileinfo();
			m_UpFile.alertintergal();
			RecvSocket.Send(m_UpFile.UpMessage,200);
		}
		/***********下载文件消息处理************/
		if (flag[0]=='D')
		{
			CDown m_DownFile;
			m_DownFile.formatting(RecvTemp);
			m_DownFile.alertintergal();
			RecvSocket.Send(m_DownFile.DownMessage,200);
		}
		/***********搜索文件消息处理************/
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
		/***********退出消息处理************/
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
					UserList.RemoveAt(pos_delete);  //清除该用户在线信息;
					temp.Format(_T("%s %s退出了客户端，关闭连接！"),data.username,data.userip);
					m_dlg->m_ListWords.AddString(temp);
					break;
				}
				pos_delete=pos;
			}
			break;  //跳出循环，结束与客户端通讯线程！
		}
		/***********注销消息处理************/
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
					UserList.RemoveAt(pos_delete);  //清除该用户在线信息;
					temp.Format(_T("%s %s退出了客户端，关闭连接！"),data.username,data.userip);
					m_dlg->m_ListWords.AddString(temp);
					break;
				}
				pos_delete=pos;
			}
		}
		/***********找回密码消息************/
		if (flag[0]=='Y')
		{
			CUserMsg m_UserInfo;
			m_UserInfo.FindPassword(RecvTemp);
			RecvSocket.Send(m_UserInfo.FindPWMessage,200);
		}
		/***********找回密码消息************/
		if (flag[0]=='A')
		{
			CUserMsg m_UserInfo;
			m_UserInfo.AlterPassword(RecvTemp);
			RecvSocket.Send(m_UserInfo.AlterPWMessage,200);
		}
	}
	RecvSocket.Close();   //套接字关闭;
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
			dlg->m_ListWords.AddString(_T("接收了一个客户端的连接请求！"));
		}
	}


}

void CFireBirdServerDlg::OnBnClickedButtonStart()
{
	void *dlg=(void*)this;
	AfxBeginThread(MainListenThread,dlg);
	m_ListWords.AddString(_T("服务器启动监听，等待客户端连接："));
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
}




BOOL CFireBirdServerDlg::InitSocket()    //聊天室套接字初始化;
{
	m_socket=socket(AF_INET,SOCK_DGRAM,0);
	if (INVALID_SOCKET==m_socket)
	{
		MessageBox(_T("套接字创建失败！"));
		return FALSE;
	}
	SOCKADDR_IN addrSocket;
	addrSocket.sin_family=AF_INET;
	addrSocket.sin_port=htons(GlobalVariable::chat_port);     //服务器聊天室使用端口：6000;客户端:6001;
	addrSocket.sin_addr.S_un.S_addr=htonl(INADDR_ANY);

	int retval;
	//绑定套接字;
	retval=bind(m_socket,(SOCKADDR*)&addrSocket,sizeof(SOCKADDR));
	if (SOCKET_ERROR==retval)
	{
		closesocket(m_socket);
		MessageBox(_T("绑定失败！"));
		return FALSE;
	}
	return TRUE;
}

DWORD WINAPI CFireBirdServerDlg::RecvProc(LPVOID lpParameter)
{
	SOCKET sock=((RECVPARAM*)lpParameter)->sock;
	HWND hwnd=((RECVPARAM*)lpParameter)->hwnd;
	delete lpParameter;

	CFireBirdServerDlg *m_dlg=(CFireBirdServerDlg*) AfxGetApp()->m_pMainWnd;  //获得主对话框对针;
	SOCKADDR_IN addrFrom;    //接收地址辅助变量;
	int len=sizeof(SOCKADDR);  //接收地址长度;

	ChatAddr* head=new ChatAddr();//头指针;
	ChatAddr* rear=head;   //尾指针;
	head->next=NULL;
	ChatAddr* q;   //检索指针;
	int nCount=0; //在线人数初始化0;

	TCHAR recvBuf[200];    //接收信息辅助数组;
	CString Content;
	CString Name;

	SOCKET m_sendsocket=socket(AF_INET,SOCK_DGRAM,0);


	while(TRUE)
	{
		//读取来自客户端的信息;
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
					
			nCount++;//在线用户人数增1;

			
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
					if (q->next->next==NULL)  //尾指针处理;
						rear=q;
					q->next=q->next->next;  //删除结点;
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
