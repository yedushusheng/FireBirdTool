// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FireBird.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "Register.h"
#include "FireBirdDlg.h"
#include "FindPassword.h"
#include "AlterPassword.h"

// CLoginDlg �Ի���

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


// CLoginDlg ��Ϣ�������


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
	// �쳣: OCX ����ҳӦ���� FALSE
}


HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd == this)
	{
		return m_brBk;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

/************************************************************************/
/* function:�����ע��Ի���                                          */
/************************************************************************/
void CLoginDlg::OnBnClickedButton2()
{
	CRegister dlg;
	dlg.DoModal();
}
/************************************************************************/
/* function:�����¼������                                              */
/************************************************************************/
void CLoginDlg::OnBnClickedLogin()
{
	CFireBirdDlg *m_dlg=(CFireBirdDlg *) AfxGetApp()->m_pMainWnd;
	GetDlgItem(IDC_Login)->EnableWindow(FALSE); //��¼��ť�ر�;
	/*********��ȡ����IP��ַ*********/
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
	UpdateData();
	CString temp;
	temp.Format(_T("L%s*%s*%s"),m_username,m_passwords,sIP);
	m_dlg->ClientSocket.Send(temp,temp.GetLength()*sizeof(TCHAR));
}
/************************************************************************/
/* function:�رյ�¼����                                               */
/************************************************************************/
void CLoginDlg::OnCancel()
{
	CDialogEx::OnCancel();
}
/************************************************************************/
/* function:���һ�����Ի���                                         */
/************************************************************************/
void CLoginDlg::OnBnClickedForgetpw()
{
	CFindPassword m_FindPasswordDlg;
	m_FindPasswordDlg.DoModal();
}
/************************************************************************/
/* function:���޸�����Ի���                                         */
/************************************************************************/
void CLoginDlg::OnBnClickedAlterpw()
{
	CAlterPassword m_AlterPasswordDlg;
	m_AlterPasswordDlg.DoModal();
}
