// ChatRoom.cpp : 实现文件
//

#include "stdafx.h"
#include "FireBird.h"
#include "ChatRoom.h"
#include "afxdialogex.h"
#include "FireBirdDlg.h"
#include "GlobalVariable.h"



CEdit m_MessageList;
CListCtrl m_OnlineList;
CComboBox m_talkperson;

DWORD WINAPI RecvProc(LPVOID lpParameter);

struct RECVPARAM2    //多线程套接字参数结构;
{
	SOCKET sock;
	HWND hwnd;
};

struct CHATMESSAGE
{
	DWORD nType;
	TCHAR Name[30];
	TCHAR NameTo[30];
	TCHAR Content[100];
};
// CChatRoom 对话框

IMPLEMENT_DYNAMIC(CChatRoom, CDialogEx)

	CChatRoom::CChatRoom(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatRoom::IDD, pParent)
	, m_SendWords(_T(""))
{

}

CChatRoom::~CChatRoom()
{
}

void CChatRoom::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_Send,m_send);
	DDX_Control(pDX, IDC_COMBO1, m_talkperson);
	DDX_Control(pDX, IDC_COMBO2, m_talksel);
	DDX_Control(pDX, IDC_EDIT2, m_MessageList);
	DDX_Text(pDX, IDC_EDIT1, m_SendWords);
	DDX_Control(pDX, IDC_LIST3, m_OnlineList);
	DDX_Control(pDX, IDC_LIST2, m_ListNotice);
}


BEGIN_MESSAGE_MAP(CChatRoom, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_Send, &CChatRoom::OnBnClickedSend)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CChatRoom::OnSelchangeCombo1)
	ON_WM_CREATE()
	ON_WM_HOTKEY()
END_MESSAGE_MAP()


// CChatRoom 消息处理程序


BOOL CChatRoom::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect   temprect(0,0,630,509);
	CWnd::SetWindowPos(NULL,0,0,temprect.Width(),temprect.Height(),SWP_NOZORDER|SWP_NOMOVE);

	m_send.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Send)));
	m_send.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_SendHot)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Send)));

	m_talkperson.SetCurSel(0);
	m_talksel.SetCurSel(0);

	//聊天室通信初始化代码;
	m_sendsocket=socket(AF_INET,SOCK_DGRAM,0);
	InitSocket();
	RECVPARAM2 *pRecvParam=new RECVPARAM2;
	pRecvParam->sock=m_socket;
	pRecvParam->hwnd=this->m_hWnd;
	HANDLE hThread=CreateThread(NULL,0,RecvProc,(LPVOID)pRecvParam,0,NULL);
	CloseHandle(hThread);

	//聊天记录字体;
	m_Font = new CFont;
	m_Font->CreateFont(-15,0,0,0,100,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,_T("宋体"));
	CEdit *m_Edit=(CEdit *)GetDlgItem(IDC_EDIT2);
	m_Edit->SetFont(m_Font,FALSE);

	//发送消息字体;
	CEdit *m_Edit1=(CEdit *)GetDlgItem(IDC_EDIT1);
	m_Edit1->SetFont(m_Font,FALSE);

	//在线列表;
	LONG lStyle;
	lStyle = GetWindowLong(m_OnlineList.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_OnlineList.m_hWnd, GWL_STYLE, lStyle);//设置style
	DWORD dwStyle = m_OnlineList.GetExtendedStyle();
	m_OnlineList.SetExtendedStyle(dwStyle); //设置扩展风格

	m_OnlineList.InsertColumn( 0, _T("在线用户"), LVCFMT_LEFT, 150 );//插入列

	m_ImageOnlineList.Create(16,16,ILC_COLOR32|ILC_MASK,2,2);
	HICON hIcon0 = ::LoadIcon (AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	m_ImageOnlineList.Add(hIcon0);
	m_OnlineList.SetImageList(&m_ImageOnlineList,LVSIL_SMALL);//在线列表框添加图标

	//公聊、私聊;
	m_talkperson.AddString(_T("大家"));
	m_talkperson.SetCurSel(0);
	m_talksel.EnableWindow(FALSE);
	m_SendType=2;

	m_ListNotice.AddString(_T("欢迎使用火鸟聊天室V1.0"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CChatRoom::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpBackground;
	bmpBackground.LoadBitmap(IDB_ChatBG);
	BITMAP bitmap;
	bmpBackground.GetBitmap(&bitmap);
	CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground);
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}

BOOL CChatRoom::InitSocket()
{
	m_socket=socket(AF_INET,SOCK_DGRAM,0);
	if (INVALID_SOCKET==m_socket)
	{
		MessageBox(_T("套接字创建失败！"));
		return FALSE;
	}
	SOCKADDR_IN addrSocket;
	addrSocket.sin_family=AF_INET;
	addrSocket.sin_port=htons(GlobalVariable::chat_port_client);
	addrSocket.sin_addr.S_un.S_addr=htonl(INADDR_ANY);

	int retval;
	//绑定套接字;
	retval=bind(m_socket,(SOCKADDR*)&addrSocket,sizeof(SOCKADDR));
	if (SOCKET_ERROR==retval)
	{
		closesocket(m_socket);
		MessageBox(_T("没能连上聊天室！"));
		return FALSE;
	}


	addrTo.sin_family=AF_INET;
	addrTo.sin_port=htons(GlobalVariable::chat_port);
	addrTo.sin_addr.S_un.S_addr= inet_addr("192.168.1.100"); 

	CFireBirdDlg *m_dlg=(CFireBirdDlg*) AfxGetApp()->m_pMainWnd;
	ChatName=m_dlg->username;

	CHATMESSAGE MessageSend;
	memset(&MessageSend, 0, sizeof(MessageSend));
	MessageSend.nType=0;
	memcpy(MessageSend.Name, ChatName.GetBuffer(), sizeof(MessageSend.Name)); 


	sendto(m_sendsocket,(char*)&MessageSend,sizeof(MessageSend),0,(SOCKADDR*)&addrTo,sizeof(SOCKADDR));  

	ChatName.ReleaseBuffer();

	return TRUE;
}

DWORD WINAPI RecvProc(LPVOID lpParameter)
{
	SOCKET sock=((RECVPARAM2*)lpParameter)->sock;
	HWND hwnd=((RECVPARAM2*)lpParameter)->hwnd;
	delete lpParameter;

	SOCKADDR_IN addrFrom;
	int len=sizeof(SOCKADDR);

	TCHAR recvBuf[200];    //接收信息辅助数组;
	CString Content;  
	CString Name;
	CTime time;
	CString strtm;
	CString temp;

	while(TRUE)
	{
		//读取来自客户端的信息,并做类型转换;
		memset(recvBuf,0,200);
		recvfrom(sock,(char*)recvBuf,200,0,(SOCKADDR*)&addrFrom,&len);
		CHATMESSAGE *szChat_Recv=(CHATMESSAGE*) recvBuf;	
		time=CTime::GetCurrentTime();  //获得当前时间;
		strtm=time.Format("%H:%M:%S");  
		switch(szChat_Recv->nType)
		{
		case 0:
			Name.Format(_T("%s 进入了聊天室 %s"),szChat_Recv->Name,strtm);
			m_MessageList.ReplaceSel(Name+"\r\n");
			m_OnlineList.InsertItem(m_OnlineList.GetItemCount(),szChat_Recv->Name);
			m_talkperson.InsertString(m_talkperson.GetCount(),szChat_Recv->Name);
			break;

		case 1:
			Name.Format(_T("%s"),szChat_Recv->Name);
			for (int i=0;i<=m_OnlineList.GetItemCount();i++)
			{
				if (Name==m_OnlineList.GetItemText(i,0))
				{
					m_OnlineList.DeleteItem(i);
					break;
				}
			}
			for (int j=0;j<m_talkperson.GetCount();j++)
			{
				m_talkperson.GetLBText(j,temp);
				if (Name==temp)
				{
					m_talkperson.DeleteString(j);
					break;
				}
			}
			Name.Format(_T("%s 退出了聊天室 %s"),szChat_Recv->Name,strtm);
			m_MessageList.ReplaceSel(Name+"\r\n");
			break;

		case 2:
			Name.Format(_T("%s %s 说:"),szChat_Recv->Name,strtm);
			Content.Format(_T("  %s"),szChat_Recv->Content);
			m_MessageList.ReplaceSel(Name+"\r\n");
			m_MessageList.ReplaceSel(Content+"\r\n");
			break;

		case 3:
			Name.Format(_T("%s %s对你说:"),szChat_Recv->Name,strtm);
			Content.Format(_T("  %s"),szChat_Recv->Content);
			m_MessageList.ReplaceSel(Name+"\r\n");
			m_MessageList.ReplaceSel(Content+"\r\n");
			break;
		case -1:
			m_OnlineList.InsertItem(m_OnlineList.GetItemCount(),szChat_Recv->Name);
			m_talkperson.InsertString(m_talkperson.GetCount(),szChat_Recv->Name);
		default:
			break;
		}
	}
	return 0;
}



void CChatRoom::OnBnClickedSend()
{
	UpdateData(TRUE);
	CHATMESSAGE MessageSend;
	memset(&MessageSend, 0, sizeof(MessageSend));
	MessageSend.nType=m_SendType;
	if (MessageSend.nType==3)
	{
		CString NameTo;
		m_talkperson.GetLBText(m_talkperson.GetCurSel(),NameTo);
		memcpy(MessageSend.NameTo, NameTo.GetBuffer(), sizeof(MessageSend.NameTo)); 
		NameTo.ReleaseBuffer();
	}
	memcpy(MessageSend.Name, ChatName.GetBuffer(), sizeof(MessageSend.Name)); 
	memcpy(MessageSend.Content, m_SendWords.GetBuffer(), sizeof(MessageSend.Content));
	sendto(m_sendsocket,(char*)&MessageSend,sizeof(MessageSend),0,(SOCKADDR*)&addrTo,sizeof(SOCKADDR));  
	ChatName.ReleaseBuffer();
	m_SendWords.ReleaseBuffer();
	if (MessageSend.nType==3)
	{
		CString tt;
		tt.Format(_T("你对%s说："),ChatName);
		m_MessageList.ReplaceSel(tt+"\r\n");
		m_MessageList.ReplaceSel(m_SendWords+"\r\n");
	}
	
}


void CChatRoom::OnClose()
{
	CHATMESSAGE MessageSend;
	memset(&MessageSend, 0, sizeof(MessageSend));
	MessageSend.nType=1;
	memcpy(MessageSend.Name, ChatName.GetBuffer(), sizeof(MessageSend.Name)); 
	sendto(m_sendsocket,(char*)&MessageSend,sizeof(MessageSend),0,(SOCKADDR*)&addrTo,sizeof(SOCKADDR));  
	ChatName.ReleaseBuffer();
	closesocket(m_socket);
	CDialogEx::OnClose();
}


void CChatRoom::OnSelchangeCombo1()
{
	m_talksel.EnableWindow(TRUE);
	if (m_talkperson.GetCurSel()!=0)	
	{
		m_talksel.SetCurSel(1);
		m_SendType=3;
	}
	else
	{
		m_talksel.SetCurSel(0);
		m_SendType=2;
	}	
	m_talksel.EnableWindow(FALSE);
}


int CChatRoom::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CFireBirdDlg *m_dlg=(CFireBirdDlg*) AfxGetApp()->m_pMainWnd;
	if (m_dlg->TabPage4.m_ChatSendKey1.GetCurSel()==0)
	{
		CString str;
		m_dlg->TabPage4.m_ChatSendKey2.GetWindowText(str);
		RegisterHotKey(m_hWnd,1002,MOD_ALT,str[0]); 
	}
	if (m_dlg->TabPage4.m_ChatSendKey1.GetCurSel()==1)
	{
		CString str;
		m_dlg->TabPage4.m_ChatSendKey2.GetWindowText(str);
		RegisterHotKey(m_hWnd,1002,MOD_CONTROL,str[0]); 
	}
	if (m_dlg->TabPage4.m_ChatSendKey1.GetCurSel()==2)
	{
		CString str;
		m_dlg->TabPage4.m_ChatSendKey2.GetWindowText(str);
		RegisterHotKey(m_hWnd,1002,MOD_SHIFT,str[0]); 
	}     
	return 0;
}


void CChatRoom::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFireBirdDlg *m_dlg=(CFireBirdDlg*) AfxGetApp()->m_pMainWnd;
	if(nHotKeyId==1002&&(m_dlg->TabPage4.m_Check_BossKey.GetCheck()==1))
	{
		OnBnClickedSend();
	}           
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}
