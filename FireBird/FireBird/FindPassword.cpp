// FindPassword.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FireBird.h"
#include "FindPassword.h"
#include "afxdialogex.h"
#include "FireBirdDlg.h"


// CFindPassword �Ի���

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


// CFindPassword ��Ϣ�������


void CFindPassword::OnBnClickedButton1()
{
	UpdateData(TRUE);
	if (m_email==_T(""))
	{
		AfxMessageBox(_T("����������"));
		return;
	}
	if (m_email.ReverseFind('@')==-1)
	{
		AfxMessageBox(_T("��������ȷ�������ʽ"));
		return;
	}
	if (m_email.Find(_T(".com"))==-1)
	{
		AfxMessageBox(_T("��������ȷ�������ʽ"));
		return;
	}
	CFireBirdDlg *m_dlg=(CFireBirdDlg *) AfxGetApp()->m_pMainWnd;
	CString temp;
	temp.Format(_T("Y%s"),m_email);
	m_dlg->ClientSocket.Send(temp,temp.GetLength()*sizeof(TCHAR));
}

void CFindPassword::ReturnError()
{
	AfxMessageBox(_T("�����䲻���ڣ�"));
}

void CFindPassword::ReturnSuccess()
{
	MessageBox(_T("�һ�����ɹ����ѽ������뷢����ע�����䣡"),_T("�һ�����"),MB_OK);
	CDialog::OnCancel();
}