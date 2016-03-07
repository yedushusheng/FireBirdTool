// LogOff.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FireBird.h"
#include "LogOff.h"
#include "afxdialogex.h"
#include "FireBirdDlg.h"


// CLogOff �Ի���

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


// CLogOff ��Ϣ�������


void CLogOff::OnBnClickedButton1()
{
	CFireBirdDlg *m_dlg=(CFireBirdDlg*) AfxGetApp()->m_pMainWnd;
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
	ExitMessage.Format(_T("O%s"),sIP);
	m_dlg->ClientSocket.Send(ExitMessage,200);
	m_dlg->LoginOff=FALSE;//��ע��;
	m_dlg->TabPage1.SaveTempFile();//��δ����������񱣴�;
	m_dlg->m_userlogin.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP2)));
	m_dlg->m_userlogin.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_UserHot)),
		LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP2)));
	m_dlg->GetDlgItem(IDC_Static_UserName)->SetWindowText(_T(""));
	m_dlg->GetDlgItem(IDC_Static_Intergal)->SetWindowText(_T(""));
	CRect rtlbl,rtlbl1; 
	m_dlg->GetDlgItem(IDC_Static_Intergal)->GetWindowRect(&rtlbl);
	m_dlg->ScreenToClient(&rtlbl); //ת���ͻ��˽���
	m_dlg->InvalidateRect(&rtlbl);//���ˢ�¶Ի��򱳾� 
	m_dlg->UpdateWindow();
	m_dlg->GetDlgItem(IDC_Static_UserName)->GetWindowRect(&rtlbl1); 
	m_dlg->ScreenToClient(&rtlbl1); //ת���ͻ��˽���
	m_dlg->InvalidateRect(&rtlbl1);//���ˢ�¶Ի��򱳾� 
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
