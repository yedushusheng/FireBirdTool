// Config.cpp : 实现文件
//

#include "stdafx.h"
#include "FireBird.h"
#include "Config.h"
#include "afxdialogex.h"
#include "GlobalVariable.h"


// CConfig 对话框

IMPLEMENT_DYNAMIC(CConfig, CDialogEx)
	/************************************************************************/
	/* function:若INI文件不存在，则创建INI文件                              */
	/************************************************************************/
	CConfig::CConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfig::IDD, pParent)
	, m_RADIO_RunToOpen(0)
	, m_EDIT_SoundPath(_T(""))
	, m_EDIT_UserName(_T(""))
	, m_EDIT_PassWord(_T(""))
{

	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
	BOOL ifFind = finder.FindFile(GlobalVariable::strIni);  
	if( !ifFind )  
	{  
		CreateDirectory(GlobalVariable::strDir_system,NULL);
		CreateDirectory(GlobalVariable::strDir_userdata,NULL);
		::WritePrivateProfileStringW(_T("BaseSet"),_T("AutoRun"),_T("0"),GlobalVariable::strIni);
		::WritePrivateProfileStringW(_T("BaseSet"),_T("BossKey"),_T("1"),GlobalVariable::strIni);  
		::WritePrivateProfileStringW(_T("BaseSet"),_T("BossKey1"),_T("0"),GlobalVariable::strIni);  
		::WritePrivateProfileStringW(_T("BaseSet"),_T("BossKey2"),_T("X"),GlobalVariable::strIni);
		::WritePrivateProfileStringW(_T("BaseSet"),_T("ChatSendKey"),_T("1"),GlobalVariable::strIni);  
		::WritePrivateProfileStringW(_T("BaseSet"),_T("ChatSendKey1"),_T("1"),GlobalVariable::strIni);  
		::WritePrivateProfileStringW(_T("BaseSet"),_T("ChatSendKey2"),_T("ENTER"),GlobalVariable::strIni);  
		::WritePrivateProfileStringW(_T("BaseSet"),_T("MinToClose"),_T("0"),GlobalVariable::strIni);  
		::WritePrivateProfileStringW(_T("BaseSet"),_T("RunToOpen"),_T("0"),GlobalVariable::strIni);  
		::WritePrivateProfileStringW(_T("UserSet"),_T("DownPath"),GlobalVariable::strDir_down,GlobalVariable::strIni); 
		::WritePrivateProfileStringW(_T("UserSet"),_T("UpPath"),GlobalVariable::strDir_up,GlobalVariable::strIni);  
		::WritePrivateProfileStringW(_T("UserSet"),_T("TipsSound"),_T("1"),GlobalVariable::strIni); 
		::WritePrivateProfileStringW(_T("UserSet"),_T("UserDefinedSound"),_T(""),GlobalVariable::strIni); 
		::WritePrivateProfileStringW(_T("UserSet"),_T("TipsWindow"),_T("1"),GlobalVariable::strIni); 
		::WritePrivateProfileStringW(_T("UserSet"),_T("AutoLogin"),_T("0"),GlobalVariable::strIni); 
		::WritePrivateProfileStringW(_T("UserSet"),_T("username"),_T(""),GlobalVariable::strIni); 
		::WritePrivateProfileStringW(_T("UserSet"),_T("password"),_T(""),GlobalVariable::strIni); 
		::WritePrivateProfileStringW(_T("SystemSet"),_T("server"),_T("192.168.1.100"),GlobalVariable::strIni); 
	}  
}

CConfig::~CConfig()
{
}

void CConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_AUTORUN, m_Check_AutoRun);
	DDX_Control(pDX, IDC_CHECK_BOSSKEY, m_Check_BossKey);
	DDX_Control(pDX, IDC_CHECK_CHATSENDKEY, m_Check_ChatSendKey);
	DDX_Control(pDX, IDC_CHECK_MINTOCLOSE, m_Check_MinToClose);
	DDX_Control(pDX, IDC_COMBO2, m_BossKey1);
	DDX_Control(pDX, IDC_EDIT1, m_BossKey2);
	DDX_Control(pDX, IDC_COMBO4, m_ChatSendKey1);
	DDX_Control(pDX, IDC_EDIT3, m_ChatSendKey2);
	DDX_Radio(pDX, IDC_RADIO1, m_RADIO_RunToOpen);
	DDX_Control(pDX, IDC_COMBO1, m_Combo_DownPath);
	DDX_Control(pDX, IDC_COMBO3, m_Combo_UpPath);
	DDX_Control(pDX, IDC_CHECK5, m_Check_Sound);
	DDX_Text(pDX, IDC_EDIT8, m_EDIT_SoundPath);
	DDX_Control(pDX, IDC_CHECK4, m_Check_Window);
	DDX_Control(pDX, IDC_CHECK3, m_Check_AutoLogin);
	DDX_Text(pDX, IDC_EDIT5, m_EDIT_UserName);
	DDX_Text(pDX, IDC_EDIT6, m_EDIT_PassWord);
}


BEGIN_MESSAGE_MAP(CConfig, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Default, &CConfig::OnBnClickedButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_Application, &CConfig::OnBnClickedButtonApplication)
	ON_BN_CLICKED(IDC_BUTTON1, &CConfig::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CConfig::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON10, &CConfig::OnBnClickedButton10)
END_MESSAGE_MAP()


BOOL CConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetInitial();
	UpdateData(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// CConfig 消息处理程序
/************************************************************************/
/* function:设置中心初始化，负责向INI文件读入                          */
/************************************************************************/
void CConfig::SetInitial()
{
	CString BossKey2,ChatSendKey2,DownPath,UpPath;
	int AutoRun,BossKey,ChatSendKey,BossKey1,ChatSendKey1,MinToClose,TipsSound,TipsWindow,AutoLogin;

	AutoRun=GetPrivateProfileInt(_T("BaseSet"),_T("AutoRun"),0,GlobalVariable::strIni);//读入开机启动;
	MinToClose=GetPrivateProfileInt(_T("BaseSet"),_T("MinToClose"),0,GlobalVariable::strIni);//读入最小化退出;
	BossKey=GetPrivateProfileInt(_T("BaseSet"),_T("BossKey"),1,GlobalVariable::strIni);//读入老板键1;
	BossKey1=GetPrivateProfileInt(_T("BaseSet"),_T("BossKey1"),1,GlobalVariable::strIni);//读入老板键1;
	ChatSendKey=GetPrivateProfileInt(_T("BaseSet"),_T("ChatSendKey"),1,GlobalVariable::strIni);//读入聊天信息发送键1;
	ChatSendKey1=GetPrivateProfileInt(_T("BaseSet"),_T("ChatSendKey1"),1,GlobalVariable::strIni);//读入聊天信息发送键1;
	m_RADIO_RunToOpen=GetPrivateProfileInt(_T("BaseSet"),_T("RunToOpen"),0,GlobalVariable::strIni);//读入开机打开的页面;
	TipsSound=GetPrivateProfileInt(_T("UserSet"),_T("TipsSound"),1,GlobalVariable::strIni);//读入提示声音;
	TipsWindow=GetPrivateProfileInt(_T("UserSet"),_T("TipsWindow"),1,GlobalVariable::strIni);//读入提示窗口;
	AutoLogin=GetPrivateProfileInt(_T("UserSet"),_T("AutoLogin"),0,GlobalVariable::strIni);//读入自动登录;
	::GetPrivateProfileStringW(_T("BaseSet"),_T("BossKey2"),_T("Not Find Message"),BossKey2.GetBuffer(MAX_PATH),MAX_PATH,GlobalVariable::strIni);//读入老板键2; 
	::GetPrivateProfileStringW(_T("BaseSet"),_T("ChatSendKey2"),_T("Not Find Message"),ChatSendKey2.GetBuffer(MAX_PATH),MAX_PATH,GlobalVariable::strIni);//读入聊天信息发送键2;  
	::GetPrivateProfileStringW(_T("UserSet"),_T("DownPath"),_T("Not Find Message"),DownPath.GetBuffer(MAX_PATH),MAX_PATH,GlobalVariable::strIni); //读入下载目录; 
	::GetPrivateProfileStringW(_T("UserSet"),_T("UpPath"),_T("Not Find Message"),UpPath.GetBuffer(MAX_PATH),MAX_PATH,GlobalVariable::strIni); //读入上传目录; 
	::GetPrivateProfileStringW(_T("UserSet"),_T("UserDefinedSound"),_T("Not Find Message"),m_EDIT_SoundPath.GetBuffer(MAX_PATH),MAX_PATH,GlobalVariable::strIni); //读入声音文件路径; 
	::GetPrivateProfileStringW(_T("UserSet"),_T("username"),_T("Not Find Message"),m_EDIT_UserName.GetBuffer(MAX_PATH),MAX_PATH,GlobalVariable::strIni); //读入用户名; 
	::GetPrivateProfileStringW(_T("UserSet"),_T("password"),_T("Not Find Message"),m_EDIT_PassWord.GetBuffer(MAX_PATH),MAX_PATH,GlobalVariable::strIni); //读入密码; 
	::GetPrivateProfileStringW(_T("SystemSet"),_T("server"),_T("192.168.1.100"),GlobalVariable::server_ip.GetBuffer(MAX_PATH),MAX_PATH,GlobalVariable::strIni); //读入服务器地址; 
	BossKey2.ReleaseBuffer();
	ChatSendKey2.ReleaseBuffer();
	DownPath.ReleaseBuffer();
	UpPath.ReleaseBuffer();
	m_EDIT_SoundPath.ReleaseBuffer();
	m_EDIT_UserName.ReleaseBuffer();
	m_EDIT_PassWord.ReleaseBuffer();
	GlobalVariable::server_ip.ReleaseBuffer();	
	GlobalVariable::AutoLoginFlag=AutoLogin;
	m_Check_AutoRun.SetCheck(AutoRun);//设置开机启动; 
	m_Check_BossKey.SetCheck(BossKey);//设置老板键; 
	m_BossKey1.SetCurSel(BossKey1);//设置老板键; 
	m_BossKey2.SetWindowTextW(BossKey2);//设置老板键; 
	m_Check_ChatSendKey.SetCheck(ChatSendKey);//设置聊天信息发送键; 
	m_ChatSendKey1.SetCurSel(ChatSendKey1);//设置聊天信息发送键; 
	m_ChatSendKey2.SetWindowTextW(ChatSendKey2);//设置聊天信息发送键; 
	m_Check_MinToClose.SetCheck(MinToClose);//设置最小化退出; 
	m_Check_Sound.SetCheck(TipsSound);//设置提示声音; 
	m_Check_Window.SetCheck(TipsWindow);//设置提示窗口; 
	m_Check_AutoLogin.SetCheck(AutoLogin);//设置自动登录; 
	int nSel_Down=m_Combo_DownPath.AddString(DownPath);//设置下载目录;
	m_Combo_DownPath.SetCurSel(nSel_Down);
	int nSel_Up=m_Combo_UpPath.AddString(UpPath);//设置上传目录;
	m_Combo_UpPath.SetCurSel(nSel_Up);
	GetDlgItem(IDC_EDIT8)->SetWindowText(m_EDIT_SoundPath);
	GetDlgItem(IDC_EDIT5)->SetWindowText(m_EDIT_UserName);
	GetDlgItem(IDC_EDIT6)->SetWindowText(m_EDIT_PassWord);
	switch(m_RADIO_RunToOpen)
	{
	case 0:
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
		break;
	case 1:
		((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		break;
	case 2:
		((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		break;
	case 3:
		((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		break;
	default:
		break;
	}
}
/************************************************************************/
/* function:恢复默认设置                                               */
/************************************************************************/
void CConfig::OnBnClickedButtonDefault()
{
	m_Check_AutoRun.SetCheck(0);//设置开机启动; 
	m_Check_BossKey.SetCheck(1);//设置老板键; 
	m_BossKey1.SetCurSel(0);//设置老板键; 
	m_BossKey2.SetWindowTextW(_T("X"));//设置老板键; 
	m_Check_ChatSendKey.SetCheck(1);//设置聊天信息发送键; 
	m_ChatSendKey1.SetCurSel(1);//设置聊天信息发送键; 
	m_ChatSendKey2.SetWindowTextW(_T("ENTER"));//设置聊天信息发送键; 
	m_Check_MinToClose.SetCheck(0);//设置最小化退出; 
	m_Check_Sound.SetCheck(1);//设置提示声音; 
	m_Check_Window.SetCheck(1);//设置提示窗口; 
	m_Check_AutoLogin.SetCheck(0);//设置自动登录; 
	m_RADIO_RunToOpen=0;//设置打开页面;
	m_EDIT_SoundPath=_T("");//设置声音文件路径;
	m_EDIT_UserName=_T("");//设置用户名;
	m_EDIT_PassWord=_T("");//设置密码;
	GetDlgItem(IDC_EDIT8)->SetWindowTextW(m_EDIT_SoundPath);
	GetDlgItem(IDC_EDIT5)->SetWindowTextW(m_EDIT_UserName);
	GetDlgItem(IDC_EDIT6)->SetWindowTextW(m_EDIT_PassWord);
	GlobalVariable::strDir_down=_T("D:\\FireBirdDownloads\\");
	GlobalVariable::strDir_up=_T("D:\\FireBirdUpFile\\");
	int nSel_Down=m_Combo_DownPath.AddString(GlobalVariable::strDir_down);//设置下载目录;
	m_Combo_DownPath.SetCurSel(nSel_Down);
	int nSel_Up=m_Combo_UpPath.AddString(GlobalVariable::strDir_up);//设置上传目录;
	m_Combo_UpPath.SetCurSel(nSel_Up);
	UpdateData(TRUE);
}
/************************************************************************/
/* function:应用设置，负责向INI文件写入                                */
/************************************************************************/
void CConfig::OnBnClickedButtonApplication()
{
	CString AutoRun,BossKey,BossKey1,BossKey2,ChatSendKey,ChatSendKey1,ChatSendKey2,MinToClose;
	CString RunToOpen,DownPath,UpPath,TipsSound,TipsWindow,AutoLogin;
	UpdateData(TRUE);
	m_BossKey2.GetWindowText(BossKey2);
	m_ChatSendKey2.GetWindowText(ChatSendKey2);
	m_Combo_UpPath.GetLBText(m_Combo_UpPath.GetCurSel(),UpPath);
	m_Combo_DownPath.GetLBText(m_Combo_DownPath.GetCurSel(),DownPath);
	RunToOpen.Format(_T("%d"),m_RADIO_RunToOpen);
	BossKey1.Format(_T("%d"),m_BossKey1.GetCurSel());
	AutoRun.Format(_T("%d"),m_Check_AutoRun.GetCheck());
	BossKey.Format(_T("%d"),m_Check_BossKey.GetCheck());
	TipsSound.Format(_T("%d"),m_Check_Sound.GetCheck());
	TipsWindow.Format(_T("%d"),m_Check_Window.GetCheck());
	AutoLogin.Format(_T("%d"),m_Check_AutoLogin.GetCheck());
	ChatSendKey1.Format(_T("%d"),m_ChatSendKey1.GetCurSel());
	MinToClose.Format(_T("%d"),m_Check_MinToClose.GetCheck());
	ChatSendKey.Format(_T("%d"),m_Check_ChatSendKey.GetCheck());
	::WritePrivateProfileStringW(_T("BaseSet"),_T("AutoRun"),AutoRun,GlobalVariable::strIni);  
	::WritePrivateProfileStringW(_T("BaseSet"),_T("BossKey"),BossKey,GlobalVariable::strIni);  
	::WritePrivateProfileStringW(_T("BaseSet"),_T("BossKey1"),BossKey1,GlobalVariable::strIni);  
	::WritePrivateProfileStringW(_T("BaseSet"),_T("BossKey2"),BossKey2,GlobalVariable::strIni); 
	::WritePrivateProfileStringW(_T("BaseSet"),_T("ChatSendKey"),ChatSendKey,GlobalVariable::strIni);  
	::WritePrivateProfileStringW(_T("BaseSet"),_T("ChatSendKey1"),ChatSendKey1,GlobalVariable::strIni);  
	::WritePrivateProfileStringW(_T("BaseSet"),_T("ChatSendKey2"),ChatSendKey2,GlobalVariable::strIni);  
	::WritePrivateProfileStringW(_T("BaseSet"),_T("MinToClose"),MinToClose,GlobalVariable::strIni);  
	::WritePrivateProfileStringW(_T("BaseSet"),_T("RunToOpen"),RunToOpen,GlobalVariable::strIni);  
	::WritePrivateProfileStringW(_T("UserSet"),_T("DownPath"),DownPath,GlobalVariable::strIni); 
	::WritePrivateProfileStringW(_T("UserSet"),_T("UpPath"),UpPath,GlobalVariable::strIni);  
	::WritePrivateProfileStringW(_T("UserSet"),_T("TipsSound"),TipsSound,GlobalVariable::strIni); 
	::WritePrivateProfileStringW(_T("UserSet"),_T("UserDefinedSound"),m_EDIT_SoundPath,GlobalVariable::strIni); 
	::WritePrivateProfileStringW(_T("UserSet"),_T("TipsWindow"),TipsWindow,GlobalVariable::strIni); 
	::WritePrivateProfileStringW(_T("UserSet"),_T("AutoLogin"),AutoLogin,GlobalVariable::strIni); 
	::WritePrivateProfileStringW(_T("UserSet"),_T("username"),m_EDIT_UserName,GlobalVariable::strIni); 
	::WritePrivateProfileStringW(_T("UserSet"),_T("password"),m_EDIT_PassWord,GlobalVariable::strIni); 
	AutoRun_RegKey(AutoRun);
	MessageBox(_T("设置应用成功！\n重新运行程序设置生效。"),_T("设置中心"),MB_OK);
}
/************************************************************************/
/* function:负责向注册表写入或删除开机启动项                            */
/************************************************************************/
void CConfig::AutoRun_RegKey(CString AutoRun)
{
	if (AutoRun==_T("1"))
	{
		TCHAR path[MAX_PATH];
		CString keyStr;
		CString fileName;
		HKEY hRegKey;
		GetModuleFileName(NULL,path,sizeof(path));
		CString strPath = path;
		keyStr =_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
		if(RegOpenKey(HKEY_LOCAL_MACHINE,keyStr,&hRegKey) !=  ERROR_SUCCESS)
			return ;
		else
		{
			_wsplitpath(strPath.GetBuffer(0),NULL,NULL,fileName.GetBufferSetLength(MAX_PATH+1),NULL);
			if(!::RegSetValueEx(hRegKey,
				fileName,
				0,
				REG_SZ,
				(CONST BYTE *)strPath.GetBuffer(0),
				strPath.GetLength()*2)!= ERROR_SUCCESS)
				return;
			strPath.ReleaseBuffer();
		}
	}
	else
	{
		TCHAR path[MAX_PATH];
		CString str; 
		HKEY hRegKey; 
		BOOL bResult; 
		GetModuleFileName(NULL,path,sizeof(path));
		CString strPath = path;
		str=_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"); 
		if(RegOpenKey(HKEY_LOCAL_MACHINE, str, &hRegKey) != ERROR_SUCCESS) 
			bResult=FALSE; 
		else 
		{ 
			_wsplitpath(strPath.GetBuffer(0),NULL,NULL,str.GetBufferSetLength(MAX_PATH+1),NULL); 
			strPath.ReleaseBuffer(); 
			str.ReleaseBuffer(); 
			if(::RegDeleteValue(hRegKey, str) != ERROR_SUCCESS) 
				bResult=FALSE; 
			else 
				bResult=TRUE; 
			strPath.ReleaseBuffer(); 
		}
	}
}
/************************************************************************/
/* function:选择自定义声音文件                                         */
/************************************************************************/
void CConfig::OnBnClickedButton2()
{
	CString strFilter=_T("wave音乐(*.wav)|*.wav|mid音乐(*.mid)|*.mid||");   //选择声音文件下拉框类型;
	CFileDialog fileDlg(TRUE,NULL,_T(""),OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT|OFN_ENABLESIZING,strFilter,NULL);
	if (fileDlg.DoModal()==IDOK)      
	{
		m_EDIT_SoundPath=fileDlg.GetPathName();//获取声音文件全路径（..\..\*.*）;
		GetDlgItem(IDC_EDIT8)->SetWindowText(m_EDIT_SoundPath);
	}	
}
/************************************************************************/
/* function:选择自定义下载目录                                          */
/************************************************************************/
void CConfig::OnBnClickedButton1()
{
	TCHAR szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;
	ZeroMemory(szPath, sizeof(szPath));   
	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = _T("请选择需要打开的目录：");   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   
	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		str.Format(_T("%s\\"),szPath);
		int nSel=m_Combo_DownPath.AddString(str);
		m_Combo_DownPath.SetCurSel(nSel);
	}
	else   
		AfxMessageBox(_T("无效的目录，请重新选择"));   
}
/************************************************************************/
/* function:选择自定义上传目录                                          */
/************************************************************************/
void CConfig::OnBnClickedButton10()
{
	TCHAR szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;
	ZeroMemory(szPath, sizeof(szPath));   
	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = _T("请选择需要打开的目录：");   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   
	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		str.Format(_T("%s\\"),szPath);
		int nSel=m_Combo_UpPath.AddString(str);
		m_Combo_UpPath.SetCurSel(nSel);
	}
	else   
		AfxMessageBox(_T("无效的目录，请重新选择"));  
}

