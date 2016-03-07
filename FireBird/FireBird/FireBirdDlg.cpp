
// FireBirdDlg.cpp : 实现文件
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


// CFireBirdDlg 对话框




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


// CFireBirdDlg 消息处理程序

BOOL CFireBirdDlg::OnInitDialog()
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动;
	//  执行此操作;
	SetIcon(m_hIcon, TRUE);			// 设置大图标;
	SetIcon(m_hIcon, FALSE);		// 设置小图标;

	//以下是FireBird主程序初始化代码

	GUI_Init();//负责界面初始化;
	ConnectServer();  //负责连接服务器;
	ServerListen(); //负责监听p2p客户端的下载连接请求;	
	Config_Init();//设置初始化;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE;
}

/************************************************************************/
/* function:主要负责连接服务器                                        */
/************************************************************************/
void CFireBirdDlg::ConnectServer()   
{
	ClientSocket.GetMainDlg(this);
	if (!ClientSocket.Create())
	{
		AfxMessageBox(_T("服务器通信套接字初始化失败！"));
	}
	if (!ClientSocket.Connect(GlobalVariable::server_ip,GlobalVariable::server_port))
	{
		AfxMessageBox(_T("连接服务器失败！"));
	}
}
/************************************************************************/
/* function:负责监听p2p客户端的下载连接请求;                            */
/************************************************************************/
void CFireBirdDlg::ServerListen()
{
	serverSocket=new CSocket;
	if (!serverSocket->Create(GlobalVariable::peer_port))
	{
		AfxMessageBox(_T("p2p监听套接字创建失败！"));
	}
	SOCKET hSOCKET=serverSocket->Detach();
	RECVPARAM *pRecvData=new RECVPARAM;
	pRecvData->serverSocket=serverSocket;
	pRecvData->currentSocket=NULL;
	pRecvData->hwnd=m_hWnd;//当前窗口的句柄
	pRecvData->pSOCKET=hSOCKET;
	AfxBeginThread(ListenThreadProc,pRecvData);//启动新的线程来负责侦听
}
/************************************************************************/
/* function:主要负责监听其他客户端的下载请求;  
参数lparm 是RECVPARAM类的对象，主要负责在多线程中传输CSocket套接字，
以及窗口句柄                                                        */
/************************************************************************/
UINT CFireBirdDlg::ListenThreadProc(LPVOID lparam)//开始服务器的监听
{
	RECVPARAM *pRecvData=(RECVPARAM *)lparam;
	CSocket *server=pRecvData->serverSocket; 
	SOCKET sock=pRecvData->pSOCKET;
	server->Attach(sock);//将CSocket与SOCKET绑定
	server->Listen(GlobalVariable::peer_max);
	while(true)
	{
		CSocket *PeerSocket=new CSocket;
		if (server->Accept(*PeerSocket))	//获得客户端的连接
		{
			//接收客户端的连接之后启动新的线程来处理与客户端的交互
			pRecvData->currentSocket=PeerSocket;
			SOCKET hSOCKET=PeerSocket->Detach();//从CSocket中分离出SOCKET
			pRecvData->pSOCKET=hSOCKET;
			AfxBeginThread(SendRecvProc,pRecvData);//启动新的线程来处理与客户端的交互*/
		}
	}
	return 0;
}
/************************************************************************/
/* function:主要负责向已连接到本机的客户端传输共享文件;  
参数lparm 是RECVPARAM类的对象，主要负责在多线程中传输CSocket套接字，
以及窗口句柄                                                        */
/************************************************************************/
UINT CFireBirdDlg::SendRecvProc(LPVOID lparam)
{	
	RECVPARAM *pRecvData=(RECVPARAM *)lparam;
	CSocket *PeerSocket=pRecvData->currentSocket;
	HWND hwnd=pRecvData->hwnd;
	SOCKET sock=pRecvData->pSOCKET;
	PeerSocket->Attach(sock);//将CSocket与SOCKET绑定
	CFireBirdDlg *m_dlg=(CFireBirdDlg*) AfxGetApp()->m_pMainWnd;

	/******接收对方发来的要下载的文件名******/
	TCHAR t_Temp[200];
	DWORD dw_n=PeerSocket->Receive(t_Temp,200);
	t_Temp[dw_n/2]='\0';
	CString str_FileName;
	str_FileName.Format(_T("%s"),t_Temp);


	/******获得该文件的大小并告知对方*******/
	CString str_FilePath=_T("");
	str_FilePath+=GlobalVariable::strDir_up+str_FileName;   //形成文件路径;
	CFileStatus rStatus;                       
	if (!CFile::GetStatus(str_FilePath,rStatus))
	{
		tagFILE szFile_Send; 
		szFile_Send.len=-1;    //该文件不存在，告诉对方;
		szFile_Send.seek=0;
		PeerSocket->Send(&szFile_Send,sizeof(szFile_Send));
		return 0;
	}
	ULONGLONG uLong=rStatus.m_size;
	tagFILE szFile_Send; 
	szFile_Send.len=uLong;    //获得文件大小;
	szFile_Send.seek=0;
	PeerSocket->Send(&szFile_Send,sizeof(szFile_Send));

	/********收到该文件的下载进度**************************/
	TCHAR szfileinfo[1024] = {0}; 
	PeerSocket->Receive((void*)szfileinfo,1024);  //接收到文件名;
	tagFILE  *szFile=(tagFILE*) szfileinfo;  //收到断点传送的消息结构体;

	ULONGLONG seek,len;
	len=szFile->len;
	seek=szFile->seek;

	/********************传输文件********************************/
	ULONGLONG len2,len3,aa,sum;

	CFile myFile;
	myFile.Open(str_FilePath,CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone);
	myFile.Seek(seek,CFile::begin);

	
	char m_buf[SIZE]; 
	ULONGLONG len1;  //len1是每次读取的文件块的最大值;
	len2=0;		//从文件中读取的字节数;
	len3=len; //len3是文件剩余大小;
	aa=0;	//aa为每次成功发送的字节数;
	sum=0;	//从seek处有读取的字节数;

	while(len3>0)  //判读文件是否传输完毕;
	{
		len1=len3>SIZE?SIZE:len3;//len1为文件块大小或当文件剩余小于SIZE时，为文件的剩余大小;
		if (aa!=len2) //发送和读取的字节数不一致，修改文件指针位置;
		{
			myFile.Seek(seek+sum,CFile::begin);
		}
		
		len2=myFile.Read(m_buf,len1); //从文件读取数据;
		aa=PeerSocket->Send(m_buf,len2);    //发送读取的数据;
		if (aa<0)
		{
			break;    //发送失败，退出循环;
		}
		len3=len3-aa;	//发送成功，文件剩余大小的修改;
		sum+=aa;  //发送成功，已发送的字节数的修改;
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
	else if(nID == SC_MAXIMIZE) //最大化 ;
	{ 
		this->ShowWindow(SW_SHOW); 
	} 
	else if(nID == SC_MINIMIZE) //最小化，把他隐藏起来; 
	{ 
		this->ShowWindow(SW_HIDE); 
	} 
	else 
	{ 
		CDialog::OnSysCommand(nID, lParam); 
	} 
}
//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFireBirdDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
/*************************************************************************/
/* function:此外函数负责程序主界面背景                                   */
/************************************************************************/
void CFireBirdDlg::OnPaint()
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
		//CDialogEx::OnPaint();
		//重绘主界面背景;
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
/* function:主要负责程序主界面初始化                                    */
/************************************************************************/
void CFireBirdDlg::GUI_Init()
{
	//界面细节处理
	SetWindowText(_T("火鸟下载系统"));   //任务栏标题;
	_fnToTray();						//托盘功能;

	//TAB控件初始化开始
	tc1.mask = TCIF_TEXT;
	tc1.pszText = _T("我的下载");
	m_tab.InsertItem(0, &tc1);
	tabcount=0; 
	tabsel[tabcount]=_T("我的下载");//标志初始化;
	m_tab.GetClientRect(&rec);//获得TAB控件的坐标
	//创建子页面;
	TabPage1.Create(IDD_DOWNLOAD,GetDlgItem(IDC_TAB1));
	TabPage2.Create(IDD_UPFILE,GetDlgItem(IDC_TAB1));
	TabPage3.Create(IDD_SEARCHDLG,GetDlgItem(IDC_TAB1));
	//TabPage4.Create(IDD_CONFIG,GetDlgItem(IDC_TAB1));

	//定位选项卡页的位置，这里可以根据情况自己调节偏移量;
	rec.bottom -= 2;
	rec.left += 1;
	rec.top += 27;
	rec.right -= 3;

	//将子页面移动到指定的位置;
	TabPage1.MoveWindow(&rec);
	//显示子页面;
	TabPage1.ShowWindow(SW_SHOW);

	//TAB控件初始化结束
	IsSearch=FALSE; //搜索按钮标志初始化;

	/*****按钮加载热点图******/
	/*****我的下载按钮******/
	m_down.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP14)));
	m_down.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_DownHot)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP14)));
	/*****我的上传按钮******/
	m_upFile.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP15)));
	m_upFile.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_UpFileHot)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP15)));
	/*****聊天室按钮******/
	m_chat.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP13)));
	m_chat.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_ChatHot)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP13)));
	/*****设置中心按钮******/
	m_conf.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP16)));
	m_conf.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_ConfigHot)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP16)));
	/*****登录按钮******/
	m_userlogin.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP2)));
	m_userlogin.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_UserHot)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP2)));
	/*****搜索按钮******/
	m_search.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Search)));
	m_search.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_SearchHot)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Search)));
	/*****最小化按钮******/
	m_min.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_min)));
	m_min.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_minhot)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_min)));

	/*****设置字体大小*****/
	CFont m_Font;
	m_Font.CreateFont(45,32,0,0,300,FALSE,FALSE,0,   
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS,   
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY,   
		DEFAULT_PITCH|FF_SWISS, 
		_T( "宋体 ") 
		); 
	GetDlgItem(IDC_Static_UserName)->SetFont(&m_Font);
	GetDlgItem(IDC_Static_Intergal)->SetFont(&m_Font);
}
/*************************************************************************/
/* function:负责设置初始化                                              */
/************************************************************************/
void CFireBirdDlg::Config_Init()
{
	TabPage4.m_Combo_UpPath.GetLBText(TabPage4.m_Combo_UpPath.GetCurSel(),GlobalVariable::strDir_up);
	TabPage4.m_Combo_DownPath.GetLBText(TabPage4.m_Combo_DownPath.GetCurSel(),GlobalVariable::strDir_down);
	CreateDirectory(GlobalVariable::strDir_up,NULL);//生成上传目录;
	CreateDirectory(GlobalVariable::strDir_down,NULL);//生成下载目录;
	AutoLogin();//自动登录;
	switch(TabPage4.m_RADIO_RunToOpen)//负责打开子页面;
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
/* function:负责自动登录功能                                             */
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
			//如果能够获取计算机主机信息的话，则获取本机IP地址
			if ((hostinfo = gethostbyname(name)) != NULL)
			{
				//获取本机IP地址
				sIP=inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list);
			}
		}
		CString temp;
		temp.Format(_T("L%s*%s*%s"),TabPage4.m_EDIT_UserName,TabPage4.m_EDIT_PassWord,sIP);
		ClientSocket.Send(temp,temp.GetLength()*sizeof(TCHAR));
	}	
}
/*************************************************************************/
/* function:负责主界面TABLE控件各子页切换                               */
/************************************************************************/
void CFireBirdDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int n=m_tab.GetCurSel();
	CString str=tabsel[n];
	if(str==_T("我的下载"))
	{
		m_tab.SetCurSel(0);
		TabPage1.ShowWindow(SW_SHOW);
		TabPage2.ShowWindow(SW_HIDE);
		TabPage3.ShowWindow(SW_HIDE);
		TabPage4.ShowWindow(SW_HIDE);
	}
	else if (str==_T("我的上传"))
	{
		m_tab.SetCurSel(n);
		TabPage1.ShowWindow(SW_HIDE);
		TabPage2.ShowWindow(SW_SHOW);
		TabPage3.ShowWindow(SW_HIDE);
		TabPage4.ShowWindow(SW_HIDE);
	}
	else if(str==_T("我的搜索"))
	{
		m_tab.SetCurSel(n);
		TabPage1.ShowWindow(SW_HIDE);
		TabPage2.ShowWindow(SW_HIDE);
		TabPage3.ShowWindow(SW_SHOW);
		TabPage4.ShowWindow(SW_HIDE);
	}
	else if(str==_T("设置中心"))
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
/* function:搜索文件对话框弹出                                           */
/************************************************************************/
void CFireBirdDlg::OnBnClickedSearch()
{
	if (!LoginOff)
	{
		AfxMessageBox(_T("请点击右上角头像登录！"));
		return;
	}
	TabPage3.m_ListFlag=FALSE;   //搜索初始化标志置FALSE;
	TabPage3.m_SearchList.DeleteAllItems();
	if(!IsSearch)  
	{    //初次搜索，生成搜索子页面;
		tc3.mask = TCIF_TEXT;
		tc3.pszText = _T("我的搜索");
		tabcount++;
		tabsel[tabcount]=_T("我的搜索");
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
/* function:上传文件对话框显示                                           */
/************************************************************************/
void CFireBirdDlg::OnBnClickedUpfile()
{
	if (!LoginOff)
	{
		AfxMessageBox(_T("请点击右上角头像登录！"));
		return;
	}
	tc2.mask = TCIF_TEXT;
	tc2.pszText = _T("我的上传");
	tabcount++;
	tabsel[tabcount]=_T("我的上传");
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
/* function:登录对话框弹出                                            */
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
/* function:聊天室对话框弹出                                            */
/************************************************************************/
void CFireBirdDlg::OnBnClickedChat()
{
	if (!LoginOff)
	{
		AfxMessageBox(_T("请点击右上角头像登录！"));
		return;
	}
	CChatRoom *pCR=new CChatRoom;
	pCR->Create(IDD_CHATROOM);
	pCR->ShowWindow(SW_SHOWNORMAL);
}
/*************************************************************************/
/* function:基本设置对话框显示                                          */
/************************************************************************/
void CFireBirdDlg::OnBnClickedShezhi()
{
	tc4.mask = TCIF_TEXT;
	tc4.pszText = _T("设置中心");
	tabcount++;
	tabsel[tabcount]=_T("设置中心");
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
/* function:界面右上角用户名和积分STATIC控件重绘                        */
/************************************************************************/
HBRUSH CFireBirdDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  在此更改 DC 的任何特性
	if(pWnd->GetDlgCtrlID() == IDC_Static_UserName)  // 用户名static控件;
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));//此处设置字体的颜色
		return  HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	if(pWnd->GetDlgCtrlID() == IDC_Static_Intergal)  // 积分static控件;
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));//此处设置字体的颜色
		return  HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
/*************************************************************************/
/* function:主界面最小化按钮消息传递                                    */
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
/* function:托盘显示                                                     */
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
/* function:托盘消息处理                                              */
/************************************************************************/
LRESULT CFireBirdDlg::OnSystemTray(WPARAM wParam, LPARAM lParam) 
{ 
	if ( wParam = IDR_MAINFRAME ) 
	{ 
		switch( lParam ) 
		{ 
		case WM_LBUTTONDOWN:            //左键点击托盘图标显示窗口 ;
			this->ShowWindow(SW_NORMAL); 
			break; 
		case WM_RBUTTONDOWN:           //右键点击托盘图标显示菜单 ;
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
/* function:托盘退出                                                   */
/************************************************************************/
void CFireBirdDlg::OnExit() 
{ 
	HWND hWnd=::FindWindow(NULL,_T("用户登录"));      //得到对话框的句柄;
	CLoginDlg* pWnd= (CLoginDlg *)FromHandle(hWnd); //由句柄得到对话框的对象指针;
	if (pWnd)		//如果登录对话框（模态对话框）当前还未关闭，应先关闭该窗口;
	{
		pWnd->OnCancel();
	}
	CFireBirdDlg::OnClose();
	this->PostMessageW(WM_QUIT); 
}
/************************************************************************/
/* function:关闭程序后销毁托盘图标                                       */
/************************************************************************/
void CFireBirdDlg::OnDestroy() 
{ 
	UnregisterHotKey(m_hWnd,1001);  //释放热键;
	CDialog::OnDestroy();
	NOTIFYICONDATA nid; 
	nid.cbSize = sizeof( NOTIFYICONDATA ); 
	nid.hWnd = m_hWnd; 
	nid.uID = IDR_MAINFRAME; 
	nid.uFlags = 0; 
	::Shell_NotifyIcon( NIM_DELETE,&nid );       
	         
}
/************************************************************************/
/* function:获取来自服务器的消息                                        */
/************************************************************************/
void CFireBirdDlg::ServerRecv()
{
	TCHAR sTemp[200];
	DWORD n=ClientSocket.Receive(sTemp,200);
	sTemp[n/2]='\0';
	/*******登录返回消息,消息头L*****/
	if(sTemp[0]=='L')              
	{
		HWND hWnd=::FindWindow(NULL,_T("用户登录"));      //得到对话框的句柄;
		CLoginDlg* pWnd= (CLoginDlg *)FromHandle(hWnd); //由句柄得到对话框的对象指针;
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
				pWnd->OnCancel();								//关闭登录对话框;
				username=pWnd->m_username;
			}
			GetDlgItem(IDC_Static_UserName)->SetWindowText(_T("用户名：")+username);
			GetDlgItem(IDC_Static_Intergal)->SetWindowText(_T("积分：")+integral_temp);
			CRect rtlbl,rtlbl1; 
			GetDlgItem(IDC_Static_Intergal)->GetWindowRect(&rtlbl);
			ScreenToClient(&rtlbl); //转到客户端界面
			InvalidateRect(&rtlbl);//最后刷新对话框背景 
			UpdateWindow();
			GetDlgItem(IDC_Static_UserName)->GetWindowRect(&rtlbl1); 
			ScreenToClient(&rtlbl1); //转到客户端界面
			InvalidateRect(&rtlbl1);//最后刷新对话框背景 
			UpdateWindow();
			LoginOff=TRUE;//已登录;
			UpdateImage(image_id);	//更新用户头像;	
			TabPage1.ReadTempFile();//更新下载列表;
		}
		else 
		{
			AfxMessageBox(_T("登录失败，用户名或密码不正确！"));
			if (GlobalVariable::AutoLoginFlag<=0)
			{
				pWnd->GetDlgItem(IDC_Login)->EnableWindow(TRUE);
			}
		}
	}
	/*******注册返回消息,消息头L*****/
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
	/*******搜索返回消息,消息头S*****/
	if (sTemp[0]=='S')    
	{
		TabPage3.doSearch(sTemp);
	}
	/*******修改密码返回消息,消息头A*****/
	if (sTemp[0]=='A')    
	{
		HWND hWnd=::FindWindow(NULL,_T("修改密码"));      //得到对话框的句柄;
		CAlterPassword* pWnd= (CAlterPassword* )FromHandle(hWnd); //由句柄得到对话框的对象指针;
		if (sTemp[1]=='Y')
			pWnd->ReturnSuccess();
		if(sTemp[1]=='N')
			pWnd->ReturnError();
	}
	/*******找回密码返回消息,消息头Y*****/
	if (sTemp[0]=='Y')    
	{
		HWND hWnd=::FindWindow(NULL,_T("找回密码"));      //得到对话框的句柄;
		CFindPassword* pWnd= (CFindPassword* )FromHandle(hWnd); //由句柄得到对话框的对象指针;
		if (sTemp[1]=='Y')
			pWnd->ReturnSuccess();
		if(sTemp[1]=='N')
			pWnd->ReturnError();
	}
	/*******积分返回消息,消息头Z*****/
	if (sTemp[0]=='Z')    
	{
		int i=1;
		CString temp_intergral;
		do{
			temp_intergral+=sTemp[i];
			i++;
		}while (sTemp[i]!='\0');
		GetDlgItem(IDC_Static_Intergal)->SetWindowText(_T("积分：")+temp_intergral);
		CRect rtlbl; 
		GetDlgItem(IDC_Static_Intergal)->GetWindowRect(&rtlbl); 
		ScreenToClient(&rtlbl); //转到客户端界面
		InvalidateRect(&rtlbl);//最后刷新对话框背景 
		UpdateWindow();
	}
}
/************************************************************************/
/* function:登录成功后更新用户头像  
参数image_id（0-7），是由服务器器返回的用户注册时选择的头像          */
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
/* function:主程序退出处理，向服务器发送退出消息，关闭连接              */
/************************************************************************/
void CFireBirdDlg::BeforeExitDo()
{
	char name[255];
	CString sIP;
	PHOSTENT hostinfo;
	if(gethostname ( name, sizeof(name)) == 0)
	{
		//如果能够获取计算机主机信息的话，则获取本机IP地址
		if ((hostinfo = gethostbyname(name)) != NULL)
		{
			//获取本机IP地址
			sIP=inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list);
		}
	}
	CString ExitMessage;
	ExitMessage.Format(_T("Q%s"),sIP);
	ClientSocket.Send(ExitMessage,200);
	LoginOff=FALSE;//已注销;
	TabPage1.SaveTempFile();//将未下载完的任务保存;
}
/************************************************************************/
/* function:主窗口关闭                                                  */
/************************************************************************/
void CFireBirdDlg::OnClose()
{
	BeforeExitDo();
	CDialogEx::OnClose();
}
/************************************************************************/
/* function:主窗口最小化快捷键设置                                      */
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
/* function:主窗口最小化快捷键设置                                      */
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
