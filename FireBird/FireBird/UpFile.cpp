// UpFile.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FireBird.h"
#include "UpFile.h"
#include "afxdialogex.h"
#include "FireBirdDlg.h"
#include "GlobalVariable.h"

// UpFile �Ի���

IMPLEMENT_DYNAMIC(UpFile, CDialogEx)

	UpFile::UpFile(CWnd* pParent /*=NULL*/)
	: CDialogEx(UpFile::IDD, pParent)
{

}

UpFile::~UpFile()
{
}

void UpFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_UpFileList);
	DDX_Control(pDX, IDC_BUTTON_UP, m_ButtonSelect);
	DDX_Control(pDX, IDC_BUTTON_START, m_ButtonUpFile);
	DDX_Control(pDX, IDC_BUTTON10, m_ButtonDelFile);
}


BEGIN_MESSAGE_MAP(UpFile, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_UP, &UpFile::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_START, &UpFile::OnBnClickedButtonStart)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &UpFile::OnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON10, &UpFile::OnBnClickedButton10)
END_MESSAGE_MAP()


// UpFile ��Ϣ�������

/************************************************************************/
/* function:�ϴ��Ի����ʼ��                                            */
/************************************************************************/
BOOL UpFile::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	LONG lStyle; 
	lStyle = GetWindowLong(m_UpFileList.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style 
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ 
	lStyle |= LVS_REPORT; //����style 
	SetWindowLong(m_UpFileList.m_hWnd, GWL_STYLE, lStyle);//����style 
	DWORD dwStyle = m_UpFileList.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl�� 
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl�� 
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ� 
	m_UpFileList.SetExtendedStyle(dwStyle); //������չ��� 

	m_UpFileList.InsertColumn( 0, _T("�ļ���"), LVCFMT_LEFT, 220 );//������ 
	m_UpFileList.InsertColumn( 1, _T("����"), LVCFMT_LEFT, 70 ); 
	m_UpFileList.InsertColumn( 2, _T("��С"), LVCFMT_LEFT, 100 ); 
	m_UpFileList.InsertColumn( 3, _T("״̬"), LVCFMT_LEFT, 100 ); 

	m_ButtonUpFile.EnableWindow(FALSE);
	m_ButtonDelFile.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* function:ѡ���ļ���ʾ���ϴ��б��ȴ�ѡ���ϴ�                        */
/************************************************************************/
void UpFile::OnBnClickedButtonUp()	
{
	//����ѡ���ļ��Ի���ѡ���ļ�;
	CString strFilter=_T("�����ļ���*.*)|*.*|word�ĵ�(*.doc)|*.doc|ѹ���ļ�(*.rar)|*.rar|avi��Ƶ(*.avi)|*.avi|mp3����(*.mp3)|*.mp3|jpgͼ��(*.jpg)|*.jpg|bmpͼ��(*.bmp)|*.bmp||");   //ѡ���ļ�����������;
	CFileDialog fileDlg(TRUE,NULL,_T(""),OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT|OFN_ENABLESIZING,strFilter,NULL);
	if (fileDlg.DoModal()==IDOK)      
	{
		int i=0;
		POSITION pos=fileDlg.GetStartPosition();
		while(pos)
		{
			CString filepath=fileDlg.GetNextPathName(pos);			   //��ȡ�ļ�ȫ·����..\..\*.*��;
			CString filennametype=filepath.Mid(filepath.ReverseFind( '\\')+1);  //��ȡ�ļ���(�Ⱥ����ƣ��ֺ�����);
			CString filename=filennametype.Left(filennametype.ReverseFind('.'));  //��ȡ�ļ���(ֻ������);
			CString filetype=filennametype.Mid(filennametype.ReverseFind('.'));  //����ļ�����;
			CFileStatus rStatus;                      
			CFile::GetStatus(filepath,rStatus);      
			ULONGLONG ulong = rStatus.m_size; 
			CString filesize=CalculateFileSize(ulong);      //��ȡ�ļ���С;
			m_FileInFo[i].filename=filename;    //��ѡ����ļ����ļ������δ�Ž��ṹ��;
			m_FileInFo[i].filetype=filetype;	 //��ѡ����ļ����ļ��������δ�Ž��ṹ��;
			m_FileInFo[i].filesize=filesize;	 //��ѡ����ļ����ļ���С���δ�Ž��ṹ��;
			m_FileInFo[i].filepath=filepath;   //��ѡ����ļ����ļ�·�����δ�Ž��ṹ��;
			i++;                              //�ļ���Ϣ�ṹ������Ԫ������+1;
		}
		m_count=i;
		//�����ϴ��б���Ϣ;
		int nItem=m_UpFileList.GetItemCount();      //��õ�ǰ�б�����;
		for (int i=0;i<m_count;i++)
		{
			m_UpFileList.InsertItem(nItem,m_FileInFo[i].filename);       //�����ļ���;
			m_UpFileList.SetItemText(nItem,1,m_FileInFo[i].filetype);    //�����ļ�����;
			m_UpFileList.SetItemText(nItem,2,m_FileInFo[i].filesize);    //�����ļ���С;
			m_UpFileList.SetItemText(nItem,3,_T("���ϴ�"));              //�����ļ�״̬;
			m_filepath[nItem]=m_FileInFo[i].filepath;                   //����ǰ����ļ�·��������һ���ַ�������;
			nItem++;
		}
		m_ButtonDelFile.EnableWindow(TRUE);
	}
}
/*************************************************************************/
/* function:�����ļ�������Ϣ�����������������ļ��ڱ������ƶ������ļ��� */
/*************************************************************************/
void UpFile::OnBnClickedButtonStart()
{
	CString filename,filetype,filesize,filepath;
	int nSel=m_UpFileList.GetNextItem(-1,LVNI_SELECTED);  //��õ�ǰ�б�ѡ������;
	if (nSel==-1)										  //��û��ѡ������ʾ�û���Ϣ��������;
	{
		AfxMessageBox(_T("��ѡ���ϴ����ļ���"));
		return;								 
	}
	filename=m_UpFileList.GetItemText(nSel,0);   //���б�ǰ��ѡ�������ļ���;
	filetype=m_UpFileList.GetItemText(nSel,1);	 //���б�ǰ��ѡ�������ļ�����;
	filesize=m_UpFileList.GetItemText(nSel,2);	 //���б�ǰ��ѡ�������ļ���С;
	filepath=m_filepath[nSel];					 //���б�ǰ��ѡ�������ļ�·��;

	CFireBirdDlg *m_dlg=(CFireBirdDlg *) AfxGetApp()->m_pMainWnd;    //������Ի���ָ��;
	CString FileMsg;            
	//���ļ������ļ��������û����û������ļ����͡��ļ���С��������ļ���Ϣ����;
	FileMsg.Format(_T("U%s*%s*%s*%s"),filename,m_dlg->username,filetype,filesize);   //�ϴ��ļ���Ϣͷ��־����'U';
	
	//��ע���˴��ϴ��ļ���Ϣ�������ƺ�����Ҫ����;
	if (m_dlg->ClientSocket.Send(FileMsg,FileMsg.GetLength()*sizeof(TCHAR))!=SOCKET_ERROR)  //������������ϴ��ļ���Ϣ;
	{
		CString strCopyFile=GlobalVariable::strDir_up+filename+filetype;  //��õ��ƺ���ļ�·����;
		CopyFile(filepath,strCopyFile,TRUE);							 //�����ļ���FireBird�����ļ���;
		m_UpFileList.SetItemText(nSel,3,_T("���ϴ�"));                   //�����ļ�״̬;
		AfxMessageBox(_T("�ϴ��ļ��ɹ���"));  
		m_ButtonUpFile.EnableWindow(FALSE);
	}
	else
	{
		AfxMessageBox(_T("�ϴ��ļ�ʧ��,������ز����Ƿ���ȷ��"));
	}
}
/************************************************************************/
/* function:�ļ���С���ƻ��㣨GB,MB,KB,B��				
������uLong��ʾ�ļ�ԭʼ��С��ԭʼ��λB��ʹ��ULONGLONG�Ϳɱ�ʾ4G���ϵ��ļ�*/
/************************************************************************/
CString UpFile::CalculateFileSize(ULONGLONG uLong)    
{
	//��ȡ�ļ���С��λ��ʾ;
	CString filesize;
	double f= (double)uLong;
	if (f/1024>1)
	{
		if (f/1024/1024>1)
		{
			if (f/1024/1024/1024>1)
			{
				filesize.Format(_T("%.2fGB"),f/1024/1024/1024);   //�ļ���СGB��ʾ;
				return filesize;
			}
			filesize.Format(_T("%.2fMB"),f/1024/1024);     //�ļ���СMB��ʾ;
			return filesize;
		}
		filesize.Format(_T("%.2fKB"),f/1024);		 //�ļ���СKB��ʾ;
	}
	else
	{
		filesize.Format(_T("%I64uB"),uLong);        //�ļ���СB��ʾ;
	}
	return filesize;
}
/************************************************************************/
/* function:�����ϴ�����״̬���ı�ٿذ�ť״̬				             /
/************************************************************************/
void UpFile::OnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSel=m_UpFileList.GetNextItem(-1,LVNI_SELECTED);  //��õ�ǰ�б�ѡ������;
	if (nSel!=-1)
	{
		CString temp;
		temp=m_UpFileList.GetItemText(nSel,3);
		if (temp==_T("���ϴ�"))
		{
			m_ButtonUpFile.EnableWindow(TRUE);
		}
		if (temp==_T("���ϴ�"))
		{
			m_ButtonUpFile.EnableWindow(FALSE);
		}
	}
	*pResult = 0;
}
/************************************************************************/
/* function:ɾ���ϴ�������		                                    /
/************************************************************************/
void UpFile::OnBnClickedButton10()
{
	int nSel=m_UpFileList.GetNextItem(-1,LVNI_SELECTED);  //��õ�ǰ�б�ѡ������;
	if (nSel!=-1)
	{
		m_UpFileList.DeleteItem(nSel);
		if (m_UpFileList.GetItemCount()==0)
		{
			m_ButtonDelFile.EnableWindow(FALSE);
			m_ButtonUpFile.EnableWindow(FALSE);
		}
	}
}
