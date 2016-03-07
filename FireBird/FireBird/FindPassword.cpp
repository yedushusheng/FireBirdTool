// FindPassword.cpp : 实现文件
//

#include "stdafx.h"
#include "FireBird.h"
#include "FindPassword.h"
#include "afxdialogex.h"
#include "FireBirdDlg.h"


// CFindPassword 对话框

IMPLEMENT_DYNAMIC(CFindPassword, CDialog)

CFindPassword::CFindPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CFindPassword::IDD, pParent)
	, m_email(_T(""))
{

}

CFindPassword::~CFindPassword()
{
}

void CFindPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_email);
}


BEGIN_MESSAGE_MAP(CFindPassword, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CFindPassword::OnBnClickedButton1)
END_MESSAGE_MAP()


// CFindPassword 消息处理程序


void CFindPassword::OnBnClickedButton1()
{
	UpdateData(TRUE);
	if (m_email==_T(""))
	{
		AfxMessageBox(_T("请输入邮箱"));
		return;
	}
	if (m_email.ReverseFind('@')==-1)
	{
		AfxMessageBox(_T("请输入正确的邮箱格式"));
		return;
	}
	if (m_email.Find(_T(".com"))==-1)
	{
		AfxMessageBox(_T("请输入正确的邮箱格式"));
		return;
	}
	CFireBirdDlg *m_dlg=(CFireBirdDlg *) AfxGetApp()->m_pMainWnd;
	CString temp;
	temp.Format(_T("Y%s"),m_email);
	m_dlg->ClientSocket.Send(temp,temp.GetLength()*sizeof(TCHAR));
}

void CFindPassword::ReturnError()
{
	AfxMessageBox(_T("该邮箱不存在！"));
}

void CFindPassword::ReturnSuccess()
{
	MessageBox(_T("找回密码成功，已将新密码发至该注册邮箱！"),_T("找回密码"),MB_OK);
	CDialog::OnCancel();
}