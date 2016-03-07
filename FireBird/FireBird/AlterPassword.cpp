// AlterPassword.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FireBird.h"
#include "AlterPassword.h"
#include "afxdialogex.h"
#include "FireBirdDlg.h"


// CAlterPassword �Ի���

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


// CAlterPassword ��Ϣ�������


void CAlterPassword::OnBnClickedButton1()
{
	UpdateData(TRUE);
	if (m_UserName==""||m_OldPassWord==""||m_NewPassWord1==""||m_NewPassWord2=="")
	{
		AfxMessageBox(_T("��������Ӧ����Ϣ"));
	}
	if (m_NewPassWord1!=m_NewPassWord2)
	{
		AfxMessageBox(_T("�������������벻һ��"));
	}
	CFireBirdDlg *m_dlg=(CFireBirdDlg *) AfxGetApp()->m_pMainWnd;
	CString temp;
	temp.Format(_T("A%s*%s*%s"),m_UserName,m_OldPassWord,m_NewPassWord1);
	m_dlg->ClientSocket.Send(temp,temp.GetLength()*sizeof(TCHAR));
}

void CAlterPassword::ReturnError()
{
	AfxMessageBox(_T("���û��������ڻ���������"));
}

void CAlterPassword::ReturnSuccess()
{
	MessageBox(_T("�޸�����ɹ�"),_T("�޸�����"),MB_OK);
	CDialog::OnCancel();
}