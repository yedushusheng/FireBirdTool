
// FireBirdDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FireBird.h"
#include "FireBirdDlg.h"
#include "afxdialogex.h"
#include "GlobalVariable.h"
#include "FindPassword.h"
#include "AlterPassword.h"
#include "Register.h"
#include "LogOff.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
public:
	//	virtual BOOL OnInitDialog();
protected:
//	afx_msg LRESULT OnCancel(WPARAM wParam, LPARAM lParam);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_MESSAGE(WM_CANCEL, &CAboutDlg::OnCancel)
END_MESSAGE_MAP()


// CFireBirdDlg �Ի���




CFireBirdDlg::CFireBirdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFireBirdDlg::IDD, pParent)
	, m_SendWords(_T(""))
	,LoginOff(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFireBirdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Control(pDX, IDC_MyDown, m_down);
	DDX_Control(pDX, IDC_UpFile, m_upFile);
	DDX_Control(pDX, IDC_Chat, m_chat);
	DDX_Control(pDX, IDC_Shezhi, m_conf);
	DDX_Control(pDX, IDC_Login, m_userlogin);
	DDX_Control(pDX, IDC_Search,m_search);
	DDX_Control(pDX, IDC_BUTTONMIN,m_min);
	DDX_Text(pDX, IDC_EDIT2, m_SendWords);
	DDX_Control(pDX, IDC_EDIT2, m_SearchKeys);
}

BEGIN_MESSAGE_MAP(CFireBirdDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CFireBirdDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_Login, &CFireBirdDlg::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_UpFile, &CFireBirdDlg::OnBnClickedUpfile)
	ON_BN_CLICKED(IDC_Chat, &CFireBirdDlg::OnBnClickedChat)
	ON_BN_CLICKED(IDC_Search, &CFireBirdDlg::OnBnClickedSearch)
	ON_BN_CLICKED(IDC_Shezhi, &CFireBirdDlg::OnBnClickedShezhi)
	ON_BN_CLICKED(IDC_BUTTONMIN, &CFireBirdDlg::OnBnClickedButtonmin)
	ON_MESSAGE(WM_SYSTEMTRAY,OnSystemTray)
	ON_COMMAND(ID_EXIT,OnExit)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_HOTKEY,OnHotKey)  
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CFireBirdDlg ��Ϣ�������

BOOL CFireBirdDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�;
	//  ִ�д˲���;
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��;
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��;

	//������FireBird�������ʼ������

	GUI_Init();//��������ʼ��;
	ConnectServer();  //�������ӷ�����;
	ServerListen(); //�������p2p�ͻ��˵�������������;	
	Config_Init();//���ó�ʼ��;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE;
}

/************************************************************************/
/* function:��Ҫ�������ӷ�����                                        */
/************************************************************************/
void CFireBirdDlg::ConnectServer()   
{
	ClientSocket.GetMainDlg(this);
	if (!ClientSocket.Create())
	{
		AfxMessageBox(_T("������ͨ���׽��ֳ�ʼ��ʧ�ܣ�"));
	}
	if (!ClientSocket.Connect(GlobalVariable::server_ip,GlobalVariable::server_port))
	{
		AfxMessageBox(_T("���ӷ�����ʧ�ܣ�"));
	}
}
/************************************************************************/
/* function:�������p2p�ͻ��˵�������������;                            */
/************************************************************************/
void CFireBirdDlg::ServerListen()
{
	serverSocket=new CSocket;
	if (!serverSocket->Create(GlobalVariable::peer_port))
	{
		AfxMessageBox(_T("p2p�����׽��ִ���ʧ�ܣ�"));
	}
	SOCKET hSOCKET=serverSocket->Detach();
	RECVPARAM *pRecvData=new RECVPARAM;
	pRecvData->serverSocket=serverSocket;
	pRecvData->currentSocket=NULL;
	pRecvData->hwnd=m_hWnd;//��ǰ���ڵľ��
	pRecvData->pSOCKET=hSOCKET;
	AfxBeginThread(ListenThreadProc,pRecvData);//�����µ��߳�����������
}
/************************************************************************/
/* function:��Ҫ������������ͻ��˵���������;  
����lparm ��RECVPARAM��Ķ�����Ҫ�����ڶ��߳��д���CSocket�׽��֣�
�Լ����ھ��                                                        */
/************************************************************************/
UINT CFireBirdDlg::ListenThreadProc(LPVOID lparam)//��ʼ�������ļ���
{
	RECVPARAM *pRecvData=(RECVPARAM *)lparam;
	CSocket *server=pRecvData->serverSocket; 
	SOCKET sock=pRecvData->pSOCKET;
	server->Attach(sock);//��CSocket��SOCKET��
	server->Listen(GlobalVariable::peer_max);
	while(true)
	{
		CSocket *PeerSocket=new CSocket;
		if (server->Accept(*PeerSocket))	//��ÿͻ��˵�����
		{
			//���տͻ��˵�����֮�������µ��߳���������ͻ��˵Ľ���
			pRecvData->currentSocket=PeerSocket;
			SOCKET hSOCKET=PeerSocket->Detach();//��CSocket�з����SOCKET
			pRecvData->pSOCKET=hSOCKET;
			AfxBeginThread(SendRecvProc,pRecvData);//�����µ��߳���������ͻ��˵Ľ���*/
		}
	}
	return 0;
}
/************************************************************************/
/* function:��Ҫ�����������ӵ������Ŀͻ��˴��乲���ļ�;  
����lparm ��RECVPARAM��Ķ�����Ҫ�����ڶ��߳��д���CSocket�׽��֣�
�Լ����ھ��                                                        */
/************************************************************************/
UINT CFireBirdDlg::SendRecvProc(LPVOID lparam)
{	
	RECVPARAM *pRecvData=(RECVPARAM *)lparam;
	CSocket *PeerSocket=pRecvData->currentSocket;
	HWND hwnd=pRecvData->hwnd;
	SOCKET sock=pRecvData->pSOCKET;
	PeerSocket->Attach(sock);//��CSocket��SOCKET��
	CFireBirdDlg *m_dlg=(CFireBirdDlg*) AfxGetApp()->m_pMainWnd;

	/******���նԷ�������Ҫ���ص��ļ���******/
	TCHAR t_Temp[200];
	DWORD dw_n=PeerSocket->Receive(t_Temp,200);
	t_Temp[dw_n/2]='\0';
	CString str_FileName;
	str_FileName.Format(_T("%s"),t_Temp);


	/******��ø��ļ��Ĵ�С����֪�Է�*******/
	CString str_FilePath=_T("");
	str_FilePath+=GlobalVariable::strDir_up+str_FileName;   //�γ��ļ�·��;
	CFileStatus rStatus;                       
	if (!CFile::GetStatus(str_FilePath,rStatus))
	{
		tagFILE szFile_Send; 
		szFile_Send.len=-1;    //���ļ������ڣ����߶Է�;
		szFile_Send.seek=0;
		PeerSocket->Send(&szFile_Send,sizeof(szFile_Send));
		return 0;
	}
	ULONGLONG uLong=rStatus.m_size;
	tagFILE szFile_Send; 
	szFile_Send.len=uLong;    //����ļ���С;
	szFile_Send.seek=0;
	PeerSocket->Send(&szFile_Send,sizeof(szFile_Send));

	/********�յ����ļ������ؽ���**************************/
	TCHAR szfileinfo[1024] = {0}; 
	PeerSocket->Receive((void*)szfileinfo,1024);  //���յ��ļ���;
	tagFILE  *szFile=(tagFILE*) szfileinfo;  //�յ��ϵ㴫�͵���Ϣ�ṹ��;

	ULONGLONG seek,len;
	len=szFile->len;
	seek=szFile->seek;

	/********************�����ļ�********************************/
	ULONGLONG len2,len3,aa,sum;

	CFile myFile;
	myFile.Open(str_FilePath,CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone);
	myFile.Seek(seek,CFile::begin);

	
	char m_buf[SIZE]; 
	ULONGLONG len1;  //len1��ÿ�ζ�ȡ���ļ�������ֵ;
	len2=0;		//���ļ��ж�ȡ���ֽ���;
	len3=len; //len3���ļ�ʣ���С;
	aa=0;	//aaΪÿ�γɹ����͵��ֽ���;
	sum=0;	//��seek���ж�ȡ���ֽ���;

	while(len3>0)  //�ж��ļ��Ƿ������;
	{
		len1=len3>SIZE?SIZE:len3;//len1Ϊ�ļ����С���ļ�ʣ��С��SIZEʱ��Ϊ�ļ���ʣ���С;
		if (aa!=len2) //���ͺͶ�ȡ���ֽ�����һ�£��޸��ļ�ָ��λ��;
		{
			myFile.Seek(seek+sum,CFile::begin);
		}
		
		len2=myFile.Read(m_buf,len1); //���ļ���ȡ����;
		aa=PeerSocket->Send(m_buf,len2);    //���Ͷ�ȡ������;
		if (aa<0)
		{
			break;    //����ʧ�ܣ��˳�ѭ��;
		}
		len3=len3-aa;	//���ͳɹ����ļ�ʣ���С���޸�;
		sum+=aa;  //���ͳɹ����ѷ��͵��ֽ������޸�;
	}
	myFile.Close();
	PeerSocket->Close();

	delete PeerSocket;
	PeerSocket=NULL;

	/***********************************************************/

	return 0;	
}

void CFireBirdDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if(nID == SC_MAXIMIZE) //��� ;
	{ 
		this->ShowWindow(SW_SHOW); 
	} 
	else if(nID == SC_MINIMIZE) //��С����������������; 
	{ 
		this->ShowWindow(SW_HIDE); 
	} 
	else 
	{ 
		CDialog::OnSysCommand(nID, lParam); 
	} 
}
//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFireBirdDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
/*************************************************************************/
/* function:���⺯��������������汳��                                   */
/************************************************************************/
void CFireBirdDlg::OnPaint()
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
		//CDialogEx::OnPaint();
		//�ػ������汳��;
		CPaintDC dc(this);
		CRect rect;
		GetClientRect(&rect);
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP1);
		BITMAP bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
	}
}
/************************************************************************/
/* function:��Ҫ��������������ʼ��                                    */
/************************************************************************/
void CFireBirdDlg::GUI_Init()
{
	//����ϸ�ڴ���
	SetWindowText(_T("��������ϵͳ"));   //����������;
	_fnToTray();						//���̹���;

	//TAB�ؼ���ʼ����ʼ
	tc1.mask = TCIF_TEXT;
	tc1.pszText = _T("�ҵ�����");
	m_tab.InsertItem(0, &tc1);
	tabcount=0; 
	tabsel[tabcount]=_T("�ҵ�����");//��־��ʼ��;
	m_tab.GetClientRect(&rec);//���TAB�ؼ�������
	//������ҳ��;
	TabPage1.Create(IDD_DOWNLOAD,GetDlgItem(IDC_TAB1));
	TabPage2.Create(IDD_UPFILE,GetDlgItem(IDC_TAB1));
	TabPage3.Create(IDD_SEARCHDLG,GetDlgItem(IDC_TAB1));
	//TabPage4.Create(IDD_CONFIG,GetDlgItem(IDC_TAB1));

	//��λѡ�ҳ��λ�ã�������Ը�������Լ�����ƫ����;
	rec.bottom -= 2;
	rec.left += 1;
	rec.top += 27;
	rec.right -= 3;

	//����ҳ���ƶ���ָ����λ��;
	TabPage1.MoveWindow(&rec);
	//��ʾ��ҳ��;
	TabPage1.ShowWindow(SW_SHOW);

	//TAB�ؼ���ʼ������
	IsSearch=FALSE; //������ť��־��ʼ��;

	/*****��ť�����ȵ�ͼ******/
	/*****�ҵ����ذ�ť******/
	m_down.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP14)));
	m_down.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_DownHot)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP14)));
	/*****�ҵ��ϴ���ť******/
	m_upFile.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP15)));
	m_upFile.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_UpFileHot)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP15)));
	/*****�����Ұ�ť******/
	m_chat.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP13)));
	m_chat.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_ChatHot)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP13)));
	/*****�������İ�ť******/
	m_conf.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP16)));
	m_conf.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_ConfigHot)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP16)));
	/*****��¼��ť******/
	m_userlogin.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP2)));
	m_userlogin.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_UserHot)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP2)));
	/*****������ť******/
	m_search.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Search)));
	m_search.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_SearchHot)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Search)));
	/*****��С����ť******/
	m_min.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_min)));
	m_min.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_minhot)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_min)));

	/*****���������С*****/
	CFont m_Font;
	m_Font.CreateFont(45,32,0,0,300,FALSE,FALSE,0,   
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS,   
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY,   
		DEFAULT_PITCH|FF_SWISS, 
		_T( "���� ") 
		); 
	GetDlgItem(IDC_Static_UserName)->SetFont(&m_Font);
	GetDlgItem(IDC_Static_Intergal)->SetFont(&m_Font);
}
/*************************************************************************/
/* function:�������ó�ʼ��                                              */
/************************************************************************/
void CFireBirdDlg::Config_Init()
{
	TabPage4.m_Combo_UpPath.GetLBText(TabPage4.m_Combo_UpPath.GetCurSel(),GlobalVariable::strDir_up);
	TabPage4.m_Combo_DownPath.GetLBText(TabPage4.m_Combo_DownPath.GetCurSel(),GlobalVariable::strDir_down);
	CreateDirectory(GlobalVariable::strDir_up,NULL);//�����ϴ�Ŀ¼;
	CreateDirectory(GlobalVariable::strDir_down,NULL);//��������Ŀ¼;
	AutoLogin();//�Զ���¼;
	switch(TabPage4.m_RADIO_RunToOpen)//�������ҳ��;
	{
	case 1:
		OnBnClickedUpfile();
		break;
	case 2:
		OnBnClickedSearch();
		break;
	case 3:
		OnBnClickedChat();
		break;
	default:
		break;
	}
}
/*************************************************************************/
/* function:�����Զ���¼����                                             */
/************************************************************************/
void CFireBirdDlg::AutoLogin()
{
	if (TabPage4.m_Check_AutoLogin.GetCheck()==1)
	{
		char name[255];
		CString sIP;
		PHOSTENT hostinfo;
		if(gethostname ( name, sizeof(name)) == 0)
		{
			//����ܹ���ȡ�����������Ϣ�Ļ������ȡ����IP��ַ
			if ((hostinfo = gethostbyname(name)) != NULL)
			{
				//��ȡ����IP��ַ
				sIP=inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list);
			}
		}
		CString temp;
		temp.Format(_T("L%s*%s*%s"),TabPage4.m_EDIT_UserName,TabPage4.m_EDIT_PassWord,sIP);
		ClientSocket.Send(temp,temp.GetLength()*sizeof(TCHAR));
	}	
}
/*************************************************************************/
/* function:����������TABLE�ؼ�����ҳ�л�                               */
/************************************************************************/
void CFireBirdDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int n=m_tab.GetCurSel();
	CString str=tabsel[n];
	if(str==_T("�ҵ�����"))
	{
		m_tab.SetCurSel(0);
		TabPage1.ShowWindow(SW_SHOW);
		TabPage2.ShowWindow(SW_HIDE);
		TabPage3.ShowWindow(SW_HIDE);
		TabPage4.ShowWindow(SW_HIDE);
	}
	else if (str==_T("�ҵ��ϴ�"))
	{
		m_tab.SetCurSel(n);
		TabPage1.ShowWindow(SW_HIDE);
		TabPage2.ShowWindow(SW_SHOW);
		TabPage3.ShowWindow(SW_HIDE);
		TabPage4.ShowWindow(SW_HIDE);
	}
	else if(str==_T("�ҵ�����"))
	{
		m_tab.SetCurSel(n);
		TabPage1.ShowWindow(SW_HIDE);
		TabPage2.ShowWindow(SW_HIDE);
		TabPage3.ShowWindow(SW_SHOW);
		TabPage4.ShowWindow(SW_HIDE);
	}
	else if(str==_T("��������"))
	{
		m_tab.SetCurSel(n);
		TabPage1.ShowWindow(SW_HIDE);
		TabPage2.ShowWindow(SW_HIDE);
		TabPage3.ShowWindow(SW_HIDE);
		TabPage4.ShowWindow(SW_SHOW);
	}
	*pResult = 0;
}
/*************************************************************************/
/* function:�����ļ��Ի��򵯳�                                           */
/************************************************************************/
void CFireBirdDlg::OnBnClickedSearch()
{
	if (!LoginOff)
	{
		AfxMessageBox(_T("�������Ͻ�ͷ���¼��"));
		return;
	}
	TabPage3.m_ListFlag=FALSE;   //������ʼ����־��FALSE;
	TabPage3.m_SearchList.DeleteAllItems();
	if(!IsSearch)  
	{    //��������������������ҳ��;
		tc3.mask = TCIF_TEXT;
		tc3.pszText = _T("�ҵ�����");
		tabcount++;
		tabsel[tabcount]=_T("�ҵ�����");
		m_tab.InsertItem(tabcount, &tc3);
		m_tab.GetClientRect(&rec);
		rec.bottom -= 2;
		rec.left += 1;
		rec.top += 27;
		rec.right -= 3;
		TabPage3.MoveWindow(&rec);
		m_tab.SetCurSel(tabcount);
		TabPage1.ShowWindow(SW_HIDE);
		TabPage2.ShowWindow(SW_HIDE);
		TabPage3.ShowWindow(SW_SHOW);
		TabPage4.ShowWindow(SW_HIDE);
		IsSearch=TRUE;
	}
	CString temp,Send_Msg;
	m_SearchKeys.GetWindowText(temp);
	TabPage3.GetDlgItem(IDC_STATIC_SearchWords)->SetWindowText(temp);
	Send_Msg='S'+temp;
	ClientSocket.Send(Send_Msg,Send_Msg.GetLength()*sizeof(TCHAR));
}
/*************************************************************************/
/* function:�ϴ��ļ��Ի�����ʾ                                           */
/************************************************************************/
void CFireBirdDlg::OnBnClickedUpfile()
{
	if (!LoginOff)
	{
		AfxMessageBox(_T("�������Ͻ�ͷ���¼��"));
		return;
	}
	tc2.mask = TCIF_TEXT;
	tc2.pszText = _T("�ҵ��ϴ�");
	tabcount++;
	tabsel[tabcount]=_T("�ҵ��ϴ�");
	m_tab.InsertItem(tabcount, &tc2);
	m_tab.GetClientRect(&rec);
	rec.bottom -= 2;
	rec.left += 1;
	rec.top += 27;
	rec.right -= 3;
	TabPage2.MoveWindow(&rec);
	m_tab.SetCurSel(tabcount);
	TabPage1.ShowWindow(SW_HIDE);
	TabPage2.ShowWindow(SW_SHOW);
	TabPage3.ShowWindow(SW_HIDE);
	TabPage4.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_UpFile)->EnableWindow(FALSE);
}
/*************************************************************************/
/* function:��¼�Ի��򵯳�                                            */
/************************************************************************/
void CFireBirdDlg::OnBnClickedLogin()
{
	if (!LoginOff)
	{
		CLoginDlg m_LoginDlg;
		m_LoginDlg.DoModal();
	}
	else
	{
		CLogOff m_LogOffDlg;
		m_LogOffDlg.DoModal();
	}
}
/*************************************************************************/
/* function:�����ҶԻ��򵯳�                                            */
/************************************************************************/
void CFireBirdDlg::OnBnClickedChat()
{
	if (!LoginOff)
	{
		AfxMessageBox(_T("�������Ͻ�ͷ���¼��"));
		return;
	}
	CChatRoom *pCR=new CChatRoom;
	pCR->Create(IDD_CHATROOM);
	pCR->ShowWindow(SW_SHOWNORMAL);
}
/*************************************************************************/
/* function:�������öԻ�����ʾ                                          */
/************************************************************************/
void CFireBirdDlg::OnBnClickedShezhi()
{
	tc4.mask = TCIF_TEXT;
	tc4.pszText = _T("��������");
	tabcount++;
	tabsel[tabcount]=_T("��������");
	m_tab.InsertItem(tabcount, &tc4);
	m_tab.GetClientRect(&rec);
	rec.bottom -= -150;
	rec.left += 164;
	rec.top += 185;
	rec.right -= -165;
	TabPage4.MoveWindow(&rec);
	m_tab.SetCurSel(tabcount);
	TabPage1.ShowWindow(SW_HIDE);
	TabPage2.ShowWindow(SW_HIDE);
	TabPage3.ShowWindow(SW_HIDE);
	TabPage4.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_Shezhi)->EnableWindow(FALSE);
}
/************************************************************************/
/* function:�������Ͻ��û����ͻ���STATIC�ؼ��ػ�                        */
/************************************************************************/
HBRUSH CFireBirdDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  �ڴ˸��� DC ���κ�����
	if(pWnd->GetDlgCtrlID() == IDC_Static_UserName)  // �û���static�ؼ�;
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));//�˴������������ɫ
		return  HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	if(pWnd->GetDlgCtrlID() == IDC_Static_Intergal)  // ����static�ؼ�;
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));//�˴������������ɫ
		return  HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
/*************************************************************************/
/* function:��������С����ť��Ϣ����                                    */
/************************************************************************/
void CFireBirdDlg::OnBnClickedButtonmin()
{
	
	if (TabPage4.m_Check_MinToClose.GetCheck()==0)
	{
		PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
	}
	if (TabPage4.m_Check_MinToClose.GetCheck()==1)
	{
		OnClose();
		SendMessage(WM_CLOSE);
	}
}
/*************************************************************************/
/* function:������ʾ                                                     */
/************************************************************************/
void CFireBirdDlg::_fnToTray() 
{ 
	NOTIFYICONDATA nid; 
	nid.cbSize = sizeof( NOTIFYICONDATA ); 
	nid.hWnd = m_hWnd; 
	nid.uID = IDR_MAINFRAME; 
	nid.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP; 
	nid.uCallbackMessage = WM_SYSTEMTRAY; 
	nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME); 
	wcscpy_s(nid.szTip, _T("FireBird")); 
	::Shell_NotifyIcon(NIM_ADD,&nid ); 
}
/************************************************************************/
/* function:������Ϣ����                                              */
/************************************************************************/
LRESULT CFireBirdDlg::OnSystemTray(WPARAM wParam, LPARAM lParam) 
{ 
	if ( wParam = IDR_MAINFRAME ) 
	{ 
		switch( lParam ) 
		{ 
		case WM_LBUTTONDOWN:            //����������ͼ����ʾ���� ;
			this->ShowWindow(SW_NORMAL); 
			break; 
		case WM_RBUTTONDOWN:           //�Ҽ��������ͼ����ʾ�˵� ;
			CMenu menu; 
			menu.LoadMenu(IDR_MENU1); 
			POINT pt; 
			::GetCursorPos(&pt); 
			menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTALIGN, pt.x, pt.y, this); 
			break; 
		} 
	} 
	return 1; 
}
/************************************************************************/
/* function:�����˳�                                                   */
/************************************************************************/
void CFireBirdDlg::OnExit() 
{ 
	HWND hWnd=::FindWindow(NULL,_T("�û���¼"));      //�õ��Ի���ľ��;
	CLoginDlg* pWnd= (CLoginDlg *)FromHandle(hWnd); //�ɾ���õ��Ի���Ķ���ָ��;
	if (pWnd)		//�����¼�Ի���ģ̬�Ի��򣩵�ǰ��δ�رգ�Ӧ�ȹرոô���;
	{
		pWnd->OnCancel();
	}
	CFireBirdDlg::OnClose();
	this->PostMessageW(WM_QUIT); 
}
/************************************************************************/
/* function:�رճ������������ͼ��                                       */
/************************************************************************/
void CFireBirdDlg::OnDestroy() 
{ 
	UnregisterHotKey(m_hWnd,1001);  //�ͷ��ȼ�;
	CDialog::OnDestroy();
	NOTIFYICONDATA nid; 
	nid.cbSize = sizeof( NOTIFYICONDATA ); 
	nid.hWnd = m_hWnd; 
	nid.uID = IDR_MAINFRAME; 
	nid.uFlags = 0; 
	::Shell_NotifyIcon( NIM_DELETE,&nid );       
	         
}
/************************************************************************/
/* function:��ȡ���Է���������Ϣ                                        */
/************************************************************************/
void CFireBirdDlg::ServerRecv()
{
	TCHAR sTemp[200];
	DWORD n=ClientSocket.Receive(sTemp,200);
	sTemp[n/2]='\0';
	/*******��¼������Ϣ,��ϢͷL*****/
	if(sTemp[0]=='L')              
	{
		HWND hWnd=::FindWindow(NULL,_T("�û���¼"));      //�õ��Ի���ľ��;
		CLoginDlg* pWnd= (CLoginDlg *)FromHandle(hWnd); //�ɾ���õ��Ի���Ķ���ָ��;
		if (sTemp[1]=='Y')
		{
			int i=2;
			int image_id;
			CString temp=_T("");
			CString integral_temp=_T("");
			do{
				integral_temp+=sTemp[i];
				i++;
			}while (sTemp[i]!='*');
			i++;
			do{
				temp+=sTemp[i];
				i++;
			}while (sTemp[i]!='\0');
			image_id=_ttoi(temp);
			m_intergral=_ttoi(integral_temp);
			if (GlobalVariable::AutoLoginFlag>0)
			{
				username=TabPage4.m_EDIT_UserName;
				GlobalVariable::AutoLoginFlag--;
			}
			else
			{
				pWnd->OnCancel();								//�رյ�¼�Ի���;
				username=pWnd->m_username;
			}
			GetDlgItem(IDC_Static_UserName)->SetWindowText(_T("�û�����")+username);
			GetDlgItem(IDC_Static_Intergal)->SetWindowText(_T("���֣�")+integral_temp);
			CRect rtlbl,rtlbl1; 
			GetDlgItem(IDC_Static_Intergal)->GetWindowRect(&rtlbl);
			ScreenToClient(&rtlbl); //ת���ͻ��˽���
			InvalidateRect(&rtlbl);//���ˢ�¶Ի��򱳾� 
			UpdateWindow();
			GetDlgItem(IDC_Static_UserName)->GetWindowRect(&rtlbl1); 
			ScreenToClient(&rtlbl1); //ת���ͻ��˽���
			InvalidateRect(&rtlbl1);//���ˢ�¶Ի��򱳾� 
			UpdateWindow();
			LoginOff=TRUE;//�ѵ�¼;
			UpdateImage(image_id);	//�����û�ͷ��;	
			TabPage1.ReadTempFile();//���������б�;
		}
		else 
		{
			AfxMessageBox(_T("��¼ʧ�ܣ��û��������벻��ȷ��"));
			if (GlobalVariable::AutoLoginFlag<=0)
			{
				pWnd->GetDlgItem(IDC_Login)->EnableWindow(TRUE);
			}
		}
	}
	/*******ע�᷵����Ϣ,��ϢͷL*****/
	if (sTemp[0]=='R')    
	{
		CRegister m_Register;
		if(sTemp[1]=='U')
			m_Register.ReturnError1();
		if(sTemp[1]=='E')
			m_Register.ReturnError2();
		if(sTemp[1]=='Y')
			m_Register.ReturnSuccess();
	}
	/*******����������Ϣ,��ϢͷS*****/
	if (sTemp[0]=='S')    
	{
		TabPage3.doSearch(sTemp);
	}
	/*******�޸����뷵����Ϣ,��ϢͷA*****/
	if (sTemp[0]=='A')    
	{
		HWND hWnd=::FindWindow(NULL,_T("�޸�����"));      //�õ��Ի���ľ��;
		CAlterPassword* pWnd= (CAlterPassword* )FromHandle(hWnd); //�ɾ���õ��Ի���Ķ���ָ��;
		if (sTemp[1]=='Y')
			pWnd->ReturnSuccess();
		if(sTemp[1]=='N')
			pWnd->ReturnError();
	}
	/*******�һ����뷵����Ϣ,��ϢͷY*****/
	if (sTemp[0]=='Y')    
	{
		HWND hWnd=::FindWindow(NULL,_T("�һ�����"));      //�õ��Ի���ľ��;
		CFindPassword* pWnd= (CFindPassword* )FromHandle(hWnd); //�ɾ���õ��Ի���Ķ���ָ��;
		if (sTemp[1]=='Y')
			pWnd->ReturnSuccess();
		if(sTemp[1]=='N')
			pWnd->ReturnError();
	}
	/*******���ַ�����Ϣ,��ϢͷZ*****/
	if (sTemp[0]=='Z')    
	{
		int i=1;
		CString temp_intergral;
		do{
			temp_intergral+=sTemp[i];
			i++;
		}while (sTemp[i]!='\0');
		GetDlgItem(IDC_Static_Intergal)->SetWindowText(_T("���֣�")+temp_intergral);
		CRect rtlbl; 
		GetDlgItem(IDC_Static_Intergal)->GetWindowRect(&rtlbl); 
		ScreenToClient(&rtlbl); //ת���ͻ��˽���
		InvalidateRect(&rtlbl);//���ˢ�¶Ի��򱳾� 
		UpdateWindow();
	}
}
/************************************************************************/
/* function:��¼�ɹ�������û�ͷ��  
����image_id��0-7�������ɷ����������ص��û�ע��ʱѡ���ͷ��          */
/************************************************************************/
void CFireBirdDlg::UpdateImage(int image_id)	
{
	switch(image_id)
	{
	case 0:
		m_userlogin.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login001)));
		m_userlogin.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login001)),
			LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login001)));
		break;
	case 1:
		m_userlogin.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login002)));
		m_userlogin.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login002)),
			LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login002)));
		break;
	case 2:
		m_userlogin.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login003)));
		m_userlogin.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login003)),
			LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login003)));
		break;
	case 3:
		m_userlogin.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login004)));
		m_userlogin.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login004)),
			LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login004)));
		break;
	case 4:
		m_userlogin.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login005)));
		m_userlogin.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login005)),
			LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login005)));
		break;
	case 5:
		m_userlogin.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login006)));
		m_userlogin.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login006)),
			LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login006)));
		break;
	case 6:
		m_userlogin.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login007)));
		m_userlogin.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login007)),
			LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login007)));
		break;
	case 7:
		m_userlogin.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login008)));
		m_userlogin.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login008)),
			LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Login008)));
		break;
	default:
		break;
	}
}  
/************************************************************************/
/* function:�������˳�����������������˳���Ϣ���ر�����              */
/************************************************************************/
void CFireBirdDlg::BeforeExitDo()
{
	char name[255];
	CString sIP;
	PHOSTENT hostinfo;
	if(gethostname ( name, sizeof(name)) == 0)
	{
		//����ܹ���ȡ�����������Ϣ�Ļ������ȡ����IP��ַ
		if ((hostinfo = gethostbyname(name)) != NULL)
		{
			//��ȡ����IP��ַ
			sIP=inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list);
		}
	}
	CString ExitMessage;
	ExitMessage.Format(_T("Q%s"),sIP);
	ClientSocket.Send(ExitMessage,200);
	LoginOff=FALSE;//��ע��;
	TabPage1.SaveTempFile();//��δ����������񱣴�;
}
/************************************************************************/
/* function:�����ڹر�                                                  */
/************************************************************************/
void CFireBirdDlg::OnClose()
{
	BeforeExitDo();
	CDialogEx::OnClose();
}
/************************************************************************/
/* function:��������С����ݼ�����                                      */
/************************************************************************/
int CFireBirdDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	TabPage4.Create(IDD_CONFIG,GetDlgItem(IDC_TAB1));
	if (TabPage4.m_BossKey1.GetCurSel()==0)
	{
		CString str;
		TabPage4.m_BossKey2.GetWindowText(str);
		RegisterHotKey(m_hWnd,1001,MOD_ALT,str[0]); 
	}
	if (TabPage4.m_BossKey1.GetCurSel()==1)
	{
		CString str;
		TabPage4.m_BossKey2.GetWindowText(str);
		RegisterHotKey(m_hWnd,1001,MOD_CONTROL,str[0]); 
	}
	if (TabPage4.m_BossKey1.GetCurSel()==2)
	{
		CString str;
		TabPage4.m_BossKey2.GetWindowText(str);
		RegisterHotKey(m_hWnd,1001,MOD_SHIFT,str[0]); 
	}     
	return 0;
}
/************************************************************************/
/* function:��������С����ݼ�����                                      */
/************************************************************************/
LRESULT CFireBirdDlg::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	if(wParam==1001&&(TabPage4.m_Check_BossKey.GetCheck()==1))
	{
		PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
	}           
	return 0;
}




//afx_msg LRESULT CAboutDlg::OnCancel(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}
