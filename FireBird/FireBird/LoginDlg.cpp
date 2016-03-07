// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FireBird.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "Register.h"
#include "FireBirdDlg.h"
#include "FindPassword.h"
#include "AlterPassword.h"

// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
	, m_username(_T(""))
	, m_passwords(_T(""))
{
	
}

CLoginDlg::~CLoginDlg()
{
}


void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_UserName, m_username);
	DDX_Text(pDX, IDC_PassWord, m_passwords);
	DDX_Control(pDX, IDC_Login,m_ButtonLogin);
	DDX_Control(pDX,IDC_BUTTON2,m_ButtonRegister);
	DDX_Control(pDX,IDC_AlterPw,m_ButtonAlter);
	DDX_Control(pDX,IDC_ForgetPw,m_ButtonFind);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON2, &CLoginDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_Login, &CLoginDlg::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_ForgetPw, &CLoginDlg::OnBnClickedForgetpw)
	ON_BN_CLICKED(IDC_AlterPw, &CLoginDlg::OnBnClickedAlterpw)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CBitmap bmp;
	bmp.LoadBitmap(IDB_LoginNew001);
	m_brBk.CreatePatternBrush(&bmp);
	bmp.DeleteObject();

	m_ButtonLogin.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_LoginNew002)));
	m_ButtonLogin.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_LoginNew003)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_LoginNew002)));
	m_ButtonRegister.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_LoginNew004)));
	m_ButtonRegister.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_LoginNew005)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_LoginNew004)));
	m_ButtonAlter.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_LoginNew006)));
	m_ButtonAlter.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_LoginNew007)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_LoginNew006)));
	m_ButtonFind.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_LoginNew008)));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  在此更改 DC 的任何特性
	if (pWnd == this)
	{
		return m_brBk;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

/************************************************************************/
/* function:负责打开注册对话框                                          */
/************************************************************************/
void CLoginDlg::OnBnClickedButton2()
{
	CRegister dlg;
	dlg.DoModal();
}
/************************************************************************/
/* function:负责登录服务器                                              */
/************************************************************************/
void CLoginDlg::OnBnClickedLogin()
{
	CFireBirdDlg *m_dlg=(CFireBirdDlg *) AfxGetApp()->m_pMainWnd;
	GetDlgItem(IDC_Login)->EnableWindow(FALSE); //登录按钮关闭;
	/*********获取本机IP地址*********/
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
	UpdateData();
	CString temp;
	temp.Format(_T("L%s*%s*%s"),m_username,m_passwords,sIP);
	m_dlg->ClientSocket.Send(temp,temp.GetLength()*sizeof(TCHAR));
}
/************************************************************************/
/* function:关闭登录窗口                                               */
/************************************************************************/
void CLoginDlg::OnCancel()
{
	CDialogEx::OnCancel();
}
/************************************************************************/
/* function:打开找回密码对话框                                         */
/************************************************************************/
void CLoginDlg::OnBnClickedForgetpw()
{
	CFindPassword m_FindPasswordDlg;
	m_FindPasswordDlg.DoModal();
}
/************************************************************************/
/* function:打开修改密码对话框                                         */
/************************************************************************/
void CLoginDlg::OnBnClickedAlterpw()
{
	CAlterPassword m_AlterPasswordDlg;
	m_AlterPasswordDlg.DoModal();
}
