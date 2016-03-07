// AlterPassword.cpp : 实现文件
//

#include "stdafx.h"
#include "FireBird.h"
#include "AlterPassword.h"
#include "afxdialogex.h"
#include "FireBirdDlg.h"


// CAlterPassword 对话框

IMPLEMENT_DYNAMIC(CAlterPassword, CDialog)

CAlterPassword::CAlterPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CAlterPassword::IDD, pParent)
	, m_UserName(_T(""))
	, m_OldPassWord(_T(""))
	, m_NewPassWord1(_T(""))
	, m_NewPassWord2(_T(""))
{

}

CAlterPassword::~CAlterPassword()
{
}

void CAlterPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_UserName);
	DDX_Text(pDX, IDC_EDIT3, m_OldPassWord);
	DDX_Text(pDX, IDC_EDIT4, m_NewPassWord1);
	DDX_Text(pDX, IDC_EDIT9, m_NewPassWord2);
}


BEGIN_MESSAGE_MAP(CAlterPassword, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CAlterPassword::OnBnClickedButton1)
END_MESSAGE_MAP()


// CAlterPassword 消息处理程序


void CAlterPassword::OnBnClickedButton1()
{
	UpdateData(TRUE);
	if (m_UserName==""||m_OldPassWord==""||m_NewPassWord1==""||m_NewPassWord2=="")
	{
		AfxMessageBox(_T("请输入相应的信息"));
	}
	if (m_NewPassWord1!=m_NewPassWord2)
	{
		AfxMessageBox(_T("两次新密码输入不一致"));
	}
	CFireBirdDlg *m_dlg=(CFireBirdDlg *) AfxGetApp()->m_pMainWnd;
	CString temp;
	temp.Format(_T("A%s*%s*%s"),m_UserName,m_OldPassWord,m_NewPassWord1);
	m_dlg->ClientSocket.Send(temp,temp.GetLength()*sizeof(TCHAR));
}

void CAlterPassword::ReturnError()
{
	AfxMessageBox(_T("该用户名不存在或旧密码错误！"));
}

void CAlterPassword::ReturnSuccess()
{
	MessageBox(_T("修改密码成功"),_T("修改密码"),MB_OK);
	CDialog::OnCancel();
}