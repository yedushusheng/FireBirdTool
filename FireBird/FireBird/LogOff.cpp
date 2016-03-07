// LogOff.cpp : 实现文件
//

#include "stdafx.h"
#include "FireBird.h"
#include "LogOff.h"
#include "afxdialogex.h"
#include "FireBirdDlg.h"


// CLogOff 对话框

IMPLEMENT_DYNAMIC(CLogOff, CDialog)

CLogOff::CLogOff(CWnd* pParent /*=NULL*/)
	: CDialog(CLogOff::IDD, pParent)
{

}

CLogOff::~CLogOff()
{
}

void CLogOff::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLogOff, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CLogOff::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLogOff::OnBnClickedButton2)
END_MESSAGE_MAP()


// CLogOff 消息处理程序


void CLogOff::OnBnClickedButton1()
{
	CFireBirdDlg *m_dlg=(CFireBirdDlg*) AfxGetApp()->m_pMainWnd;
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
	ExitMessage.Format(_T("O%s"),sIP);
	m_dlg->ClientSocket.Send(ExitMessage,200);
	m_dlg->LoginOff=FALSE;//已注销;
	m_dlg->TabPage1.SaveTempFile();//将未下载完的任务保存;
	m_dlg->m_userlogin.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP2)));
	m_dlg->m_userlogin.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_UserHot)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP2)));
	m_dlg->GetDlgItem(IDC_Static_UserName)->SetWindowText(_T(""));
	m_dlg->GetDlgItem(IDC_Static_Intergal)->SetWindowText(_T(""));
	CRect rtlbl,rtlbl1; 
	m_dlg->GetDlgItem(IDC_Static_Intergal)->GetWindowRect(&rtlbl);
	m_dlg->ScreenToClient(&rtlbl); //转到客户端界面
	m_dlg->InvalidateRect(&rtlbl);//最后刷新对话框背景 
	m_dlg->UpdateWindow();
	m_dlg->GetDlgItem(IDC_Static_UserName)->GetWindowRect(&rtlbl1); 
	m_dlg->ScreenToClient(&rtlbl1); //转到客户端界面
	m_dlg->InvalidateRect(&rtlbl1);//最后刷新对话框背景 
	m_dlg->UpdateWindow();
	m_dlg->TabPage1.m_DownLoadList.DeleteAllItems();
	m_dlg->TabPage2.m_UpFileList.DeleteAllItems();
	m_dlg->TabPage3.m_SearchList.DeleteAllItems();
	CLogOff::OnCancel();
}


void CLogOff::OnBnClickedButton2()
{
	CLogOff::OnCancel();
}
