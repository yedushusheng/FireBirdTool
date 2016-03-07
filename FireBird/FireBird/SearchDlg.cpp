// SearchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FireBird.h"
#include "SearchDlg.h"
#include "afxdialogex.h"
#include "FireBirdDlg.h"

// SearchDlg �Ի���

IMPLEMENT_DYNAMIC(SearchDlg, CDialogEx)

	SearchDlg::SearchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SearchDlg::IDD, pParent)
{

}

SearchDlg::~SearchDlg()
{
}

void SearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_SearchList);
}


BEGIN_MESSAGE_MAP(SearchDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &SearchDlg::OnNMDblclkList1)
END_MESSAGE_MAP()


// SearchDlg ��Ϣ�������

/************************************************************************/
/* function:�����Ի����ʼ��                                            */
/************************************************************************/
BOOL SearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	LONG lStyle; 
	lStyle = GetWindowLong(m_SearchList.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style 
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ 
	lStyle |= LVS_REPORT; //����style 
	SetWindowLong(m_SearchList.m_hWnd, GWL_STYLE, lStyle);//����style 
	DWORD dwStyle = m_SearchList.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl�� 
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl�� 
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ� 
	m_SearchList.SetExtendedStyle(dwStyle); //������չ��� 

	m_SearchList.InsertColumn( 0, _T("�ļ���"), LVCFMT_LEFT, 220 );//������ 
	m_SearchList.InsertColumn( 1, _T("����"), LVCFMT_LEFT, 70 ); 
	m_SearchList.InsertColumn( 2, _T("��С"), LVCFMT_LEFT, 100 ); 
	m_SearchList.InsertColumn( 3, _T("��Դ"), LVCFMT_LEFT, 120 ); 

	m_ListFlag=FALSE;		  //��־�����������ز���δ��ʼ��;

	//����������Ϣ��������
	CFont *m_Font = new CFont;
	m_Font->CreateFont(-13,0,0,0,100,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,_T("����"));
	CStatic *m_Static=(CStatic *)GetDlgItem(IDC_STATIC);
	m_Static->SetFont(m_Font,FALSE);

	CFont *m_Font2 = new CFont;
	m_Font2->CreateFont(-13,0,0,0,700,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,_T("����"));
	CStatic *m_Static2=(CStatic *)GetDlgItem(IDC_STATIC_SearchWords);
	m_Static2->SetFont(m_Font2,FALSE);
	CStatic *m_Static3=(CStatic *)GetDlgItem(IDC_STATIC_nCount);
	m_Static3->SetFont(m_Font2,FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* function:���ݷ�����������Ϣ�������ص������������������б�
   ������TCHAR sTemp[200]������Ϣͷ��һ��ֻ����һ���ļ����             */
/************************************************************************/
void SearchDlg::doSearch(TCHAR sTemp[200])
{
	if (sTemp[1]=='F')      //F����Ϣ��ʼ��־;
	{
		if (!m_ListFlag)
		{
			fileip_array.RemoveAll();			//ÿ��һ������������������б����Ӧ��IP;
			m_ListFlag=TRUE;                    //��־�����������ز����ѳ�ʼ��;
		}
		int i=2;
		CString filename=_T("");
		CString filetype=_T("");
		CString filesize=_T("");
		CString fileuser=_T("");
		CString fileip=_T("");
		do 
		{
			filename+=sTemp[i];			  //��ȡ�ļ���;
			i++;
		} while (sTemp[i]!='*');
		i++;
		do 
		{
			filetype+=sTemp[i];			  //��ȡ�ļ�����;
			i++;
		} while (sTemp[i]!='*');
		i++;
		do 
		{
			filesize+=sTemp[i];			  //��ȡ�ļ���С;
			i++;
		} while (sTemp[i]!='*');		  
		i++;
		do 
		{
			fileuser+=sTemp[i];			  //��ȡ�ļ���ӵ���ߵ��û���;
			i++;
		} while (sTemp[i]!='*');
		i++;
		do 
		{
			fileip+=sTemp[i];			  //��ȡ�ļ���ӵ���ߵ�ǰIP;
			i++;
		} while (sTemp[i]!='\0');

		int nItem=m_SearchList.InsertItem(m_SearchList.GetItemCount(),filename);    //���ļ������������б�;
		m_SearchList.SetItemText(nItem,1,filetype);		//���ļ����Ͳ��������б�;
		m_SearchList.SetItemText(nItem,2,filesize);		//���ļ���С���������б�;
		m_SearchList.SetItemText(nItem,3,fileuser);		//���ļ�ӵ�����û������������б�;
		fileip_array.Add(fileip);						//���ļ�ӵ���ߵ�ǰIP�����ڸ������飬���ļ��ڸ������λ�ú�nItemһ��;

	}
	if (sTemp[1]=='E')
	{
		m_ListFlag=FALSE;
		int nCount=m_SearchList.GetItemCount();
		CString tempCount;
		tempCount.Format(_T("%d"),nCount);
		GetDlgItem(IDC_STATIC_nCount)->SetWindowText(tempCount);
	}
}
/************************************************************************/
/* function:�����б�˫���ļ��¼�                                        */
/************************************************************************/
void SearchDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nsel=m_SearchList.GetNextItem(-1,LVNI_SELECTED);    
	if(nsel!=-1)  
	{
		CFireBirdDlg *m_dlg=(CFireBirdDlg *) AfxGetApp()->m_pMainWnd;      //������Ի�����ָ��;
		CString filename,filetype,filesize,fileip;

		filename=m_SearchList.GetItemText(nsel,0);
		filetype=m_SearchList.GetItemText(nsel,1);
		filesize=m_SearchList.GetItemText(nsel,2);
		fileip=fileip_array.GetAt(nsel);

		m_dlg->m_tab.SetCurSel(0);
		m_dlg->TabPage1.ShowWindow(SW_SHOW);
		m_dlg->TabPage2.ShowWindow(SW_HIDE);
		m_dlg->TabPage3.ShowWindow(SW_HIDE);
		m_dlg->TabPage4.ShowWindow(SW_HIDE);

		m_dlg->TabPage1.showlist(filename,filetype,filesize,fileip);
	} 
	*pResult = 0;
}
