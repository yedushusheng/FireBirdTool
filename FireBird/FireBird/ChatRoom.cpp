// ChatRoom.cpp : ʵ���ļ�
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

struct RECVPARAM2    //���߳��׽��ֲ����ṹ;
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
// CChatRoom �Ի���

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


// CChatRoom ��Ϣ�������


BOOL CChatRoom::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect   temprect(0,0,630,509);
	CWnd::SetWindowPos(NULL,0,0,temprect.Width(),temprect.Height(),SWP_NOZORDER|SWP_NOMOVE);

	m_send.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Send)));
	m_send.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_SendHot)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Send)));

	m_talkperson.SetCurSel(0);
	m_talksel.SetCurSel(0);

	//������ͨ�ų�ʼ������;
	m_sendsocket=socket(AF_INET,SOCK_DGRAM,0);
	InitSocket();
	RECVPARAM2 *pRecvParam=new RECVPARAM2;
	pRecvParam->sock=m_socket;
	pRecvParam->hwnd=this->m_hWnd;
	HANDLE hThread=CreateThread(NULL,0,RecvProc,(LPVOID)pRecvParam,0,NULL);
	CloseHandle(hThread);

	//�����¼����;
	m_Font = new CFont;
	m_Font->CreateFont(-15,0,0,0,100,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,_T("����"));
	CEdit *m_Edit=(CEdit *)GetDlgItem(IDC_EDIT2);
	m_Edit->SetFont(m_Font,FALSE);

	//������Ϣ����;
	CEdit *m_Edit1=(CEdit *)GetDlgItem(IDC_EDIT1);
	m_Edit1->SetFont(m_Font,FALSE);

	//�����б�;
	LONG lStyle;
	lStyle = GetWindowLong(m_OnlineList.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle |= LVS_REPORT; //����style
	SetWindowLong(m_OnlineList.m_hWnd, GWL_STYLE, lStyle);//����style
	DWORD dwStyle = m_OnlineList.GetExtendedStyle();
	m_OnlineList.SetExtendedStyle(dwStyle); //������չ���

	m_OnlineList.InsertColumn( 0, _T("�����û�"), LVCFMT_LEFT, 150 );//������

	m_ImageOnlineList.Create(16,16,ILC_COLOR32|ILC_MASK,2,2);
	HICON hIcon0 = ::LoadIcon (AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	m_ImageOnlineList.Add(hIcon0);
	m_OnlineList.SetImageList(&m_ImageOnlineList,LVSIL_SMALL);//�����б�����ͼ��

	//���ġ�˽��;
	m_talkperson.AddString(_T("���"));
	m_talkperson.SetCurSel(0);
	m_talksel.EnableWindow(FALSE);
	m_SendType=2;

	m_ListNotice.AddString(_T("��ӭʹ�û���������V1.0"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}

BOOL CChatRoom::InitSocket()
{
	m_socket=socket(AF_INET,SOCK_DGRAM,0);
	if (INVALID_SOCKET==m_socket)
	{
		MessageBox(_T("�׽��ִ���ʧ�ܣ�"));
		return FALSE;
	}
	SOCKADDR_IN addrSocket;
	addrSocket.sin_family=AF_INET;
	addrSocket.sin_port=htons(GlobalVariable::chat_port_client);
	addrSocket.sin_addr.S_un.S_addr=htonl(INADDR_ANY);

	int retval;
	//���׽���;
	retval=bind(m_socket,(SOCKADDR*)&addrSocket,sizeof(SOCKADDR));
	if (SOCKET_ERROR==retval)
	{
		closesocket(m_socket);
		MessageBox(_T("û�����������ң�"));
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

	TCHAR recvBuf[200];    //������Ϣ��������;
	CString Content;  
	CString Name;
	CTime time;
	CString strtm;
	CString temp;

	while(TRUE)
	{
		//��ȡ���Կͻ��˵���Ϣ,��������ת��;
		memset(recvBuf,0,200);
		recvfrom(sock,(char*)recvBuf,200,0,(SOCKADDR*)&addrFrom,&len);
		CHATMESSAGE *szChat_Recv=(CHATMESSAGE*) recvBuf;	
		time=CTime::GetCurrentTime();  //��õ�ǰʱ��;
		strtm=time.Format("%H:%M:%S");  
		switch(szChat_Recv->nType)
		{
		case 0:
			Name.Format(_T("%s ������������ %s"),szChat_Recv->Name,strtm);
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
			Name.Format(_T("%s �˳��������� %s"),szChat_Recv->Name,strtm);
			m_MessageList.ReplaceSel(Name+"\r\n");
			break;

		case 2:
			Name.Format(_T("%s %s ˵:"),szChat_Recv->Name,strtm);
			Content.Format(_T("  %s"),szChat_Recv->Content);
			m_MessageList.ReplaceSel(Name+"\r\n");
			m_MessageList.ReplaceSel(Content+"\r\n");
			break;

		case 3:
			Name.Format(_T("%s %s����˵:"),szChat_Recv->Name,strtm);
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
		tt.Format(_T("���%s˵��"),ChatName);
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

	// TODO:  �ڴ������ר�õĴ�������
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CFireBirdDlg *m_dlg=(CFireBirdDlg*) AfxGetApp()->m_pMainWnd;
	if(nHotKeyId==1002&&(m_dlg->TabPage4.m_Check_BossKey.GetCheck()==1))
	{
		OnBnClickedSend();
	}           
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}
