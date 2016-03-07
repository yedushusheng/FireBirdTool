// DownLoad.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FireBird.h"
#include "DownLoad.h"
#include "afxdialogex.h"
#include "FireBirdDlg.h"
#include "GlobalVariable.h"
#include "mmsystem.h"//��������ͷ�ļ�
#pragma comment(lib,"winmm.lib")//��������ͷ�ļ���

// DownLoad �Ի���
BOOL g_StopFlag[20];

IMPLEMENT_DYNAMIC(DownLoad, CDialogEx)

	DownLoad::DownLoad(CWnd* pParent /*=NULL*/)
	: CDialogEx(DownLoad::IDD, pParent)
{

}


DownLoad::~DownLoad()
{
}

void DownLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_DownLoadList);
	DDX_Control(pDX, IDC_BUTTON2, m_ButtonStart);
	DDX_Control(pDX, IDC_BUTTON1, m_ButtonPause);
	DDX_Control(pDX, IDC_BUTTON10, m_ButtonDelete);
}


BEGIN_MESSAGE_MAP(DownLoad, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &DownLoad::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &DownLoad::OnBnClickedButton1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &DownLoad::OnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON10, &DownLoad::OnBnClickedButton10)
END_MESSAGE_MAP()


// DownLoad ��Ϣ�������

/************************************************************************/
/* function:���ضԻ����ʼ��                                            /
/************************************************************************/
BOOL DownLoad::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	LONG lStyle; 
	lStyle = GetWindowLong(m_DownLoadList.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style 
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ 
	lStyle |= LVS_REPORT; //����style 
	SetWindowLong(m_DownLoadList.m_hWnd, GWL_STYLE, lStyle);//����style 
	DWORD dwStyle = m_DownLoadList.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl�� 
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl�� 
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ� 
	m_DownLoadList.SetExtendedStyle(dwStyle); //������չ��� 

	m_DownLoadList.InsertColumn( 0, _T("�ļ���"), LVCFMT_LEFT, 180 );//������ 
	m_DownLoadList.InsertColumn( 1, _T("����"), LVCFMT_LEFT, 70 ); 
	m_DownLoadList.InsertColumn( 2, _T("��С"), LVCFMT_LEFT, 70 ); 
	m_DownLoadList.InsertColumn( 3, _T("״̬"), LVCFMT_LEFT, 70 ); 
	m_DownLoadList.InsertColumn( 4, _T("����"), LVCFMT_LEFT, 130 ); 
	m_DownLoadList.InsertColumn( 5, _T("IP"), LVCFMT_LEFT, 0 ); 
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_wndTaskbarNotifier1.Create(this);
	m_wndTaskbarNotifier1.SetSkin(IDB_MaoPao);
	m_wndTaskbarNotifier1.SetTextFont(_T("����"),110,TN_TEXT_NORMAL,TN_TEXT_UNDERLINE);
	m_wndTaskbarNotifier1.SetTextColor(RGB(0,0,0),RGB(0,0,200));
	m_wndTaskbarNotifier1.SetTextRect(CRect(45,60,m_wndTaskbarNotifier1.m_nSkinWidth-10,m_wndTaskbarNotifier1.m_nSkinHeight-25));

	m_ButtonStart.EnableWindow(FALSE);
	m_ButtonPause.EnableWindow(FALSE);
	m_ButtonDelete.EnableWindow(FALSE);

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* function:���������ļ��߳�	                                         /
/************************************************************************/
UINT DownLoad::ThreadDown(LPVOID lparam)
{
	RECVPARAM *pRecvData=(RECVPARAM *)lparam;
	CSocket *down=pRecvData->serverSocket; 

	SOCKET sock=pRecvData->pSOCKET;
	down->Attach(sock);//��CSocket��SOCKET��

	CFireBirdDlg *m_dlg=(CFireBirdDlg *) AfxGetApp()->m_pMainWnd;

	CString filentitle,filetype,filename,fileip;

	int nsel=m_dlg->TabPage1.m_DownLoadList.GetNextItem(-1,LVNI_SELECTED);   
	filentitle=m_dlg->TabPage1.m_DownLoadList.GetItemText(nsel,0);
	filetype=m_dlg->TabPage1.m_DownLoadList.GetItemText(nsel,1);
	fileip=m_dlg->TabPage1.m_DownLoadList.GetItemText(nsel,5);
	filename=filentitle+filetype;
	g_StopFlag[nsel]=TRUE;

	if (!down->Connect(fileip,GlobalVariable::peer_port))
	{
		AfxMessageBox(_T("���������ļ��Է��ͻ���ʧ�ܣ�"));
		return 0;
	}

	down->Send(filename,filename.GetLength()*sizeof(TCHAR));
	TCHAR szFileInfo[1024] = {0}; 
	down->Receive((void*)szFileInfo,1024);  //���յ������ļ�����;
	tagFILE  *szFile_Recv=(tagFILE*) szFileInfo;  
	if (szFile_Recv->len==-1)
	{
		AfxMessageBox(_T("���ļ������ڣ����ܶԷ���ɾ�����ļ���"));
		return 0;
	}
	ULONGLONG FILESIZE=szFile_Recv->len;

	if (m_dlg->m_intergral<2)
	{
		AfxMessageBox(_T("����ֲ��������ϴ��ļ�׬ȡ���֣�"));
		return 0;
	}
	/***************************************************/
	CString str_filepath=_T("");
	str_filepath+=GlobalVariable::strDir_down+filename;
	CString m_temp=str_filepath+_T(".down");  //��ʱ�ļ�������Ϊ�ļ����Ӻ�׺.down

	int progress;
	tagFILE szFILE;
	CStdioFile myFile_ini;  //��ʼ���ؽ�������ʱ�ļ�;
	FILE *myFile_alr=NULL;   //�Ѵ��ڵ���ʱ�ļ�;

	if ((myFile_alr=_tfopen(m_temp,_T("rb")))==NULL)     //��ʼ���أ�û����ʱ�ļ�;
	{
		szFILE.len=FILESIZE;  
		szFILE.seek=0;
		progress=0;
		myFile_ini.Open(m_temp,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

		CString t1,t2,t3;
		t1.Format(_T("%I64u"),szFILE.len);
		t2.Format(_T("%I64u"),szFILE.seek);
		t3.Format(_T("%d"),progress);
		myFile_ini.Write("\xff\xfe",2);
		myFile_ini.WriteString(t1);
		myFile_ini.WriteString(_T("\n"));
		myFile_ini.WriteString(t2);
		myFile_ini.WriteString(_T("\n"));
		myFile_ini.WriteString(t3);

		myFile_ini.Close();
	}
	else			//�жϼ������أ�����ʱ�ļ�;
	{
		// (�ؼ�Ƭ��);
		// Step 1: �ȶ�ȡ 0xfffe (��Ҳ�������� 2 bytes);
		char unicodeheader[2];
		fread(unicodeheader,2,1,myFile_alr);

		// Step 2: ������ȡ Unicode ����;
		TCHAR str_len[25],str_seek[25],str_pro[25];
		_ftscanf(myFile_alr,_T("%s\n"),&str_len);
		_ftscanf(myFile_alr,_T("%s\n"),&str_seek);
		_ftscanf(myFile_alr,_T("%s\n"),&str_pro);
		fclose(myFile_alr);

		//Step 3: ת��ΪULONGLONG����;
		CString str1,str2,str3;
		str1.Format(L"%s",str_len);
		str2.Format(L"%s",str_seek);
		str3.Format(L"%s",str_pro);
		swscanf((LPCTSTR)str1,L"%I64u ", &szFILE.len);
		swscanf((LPCTSTR)str2,L"%I64u ", &szFILE.seek);
		progress=_ttoi(str3);

	}	
	if (szFILE.seek==0)
	{
		m_dlg->TabPage1.DownIntergral();
	}
	/***************************************************/

	CFile destFile;
	if (!PathFileExists(str_filepath))
	{
		destFile.Open(str_filepath,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary|CFile::shareDenyNone);
	}
	else
	{
		destFile.Open(str_filepath,CFile::modeWrite|CFile::typeBinary|CFile::shareDenyNone);
		destFile.Seek(szFILE.seek,CFile::begin);
	}
	CStdioFile myFile;
	myFile.Open(m_temp,CFile::modeWrite|CFile::typeBinary|CFile::shareDenyNone);

	//���Է���������������Ϣ�ṹ��;
	down->Send(&szFILE,sizeof(szFILE));   //�����ļ����ؽ���;

	//���潻���Ż�;
	m_dlg->TabPage1.m_DownLoadList.SetItemText(nsel,3,_T("��������.."));

	ULONGLONG len=szFILE.len;
	char* m_buf=new char[SIZE];
	ULONGLONG re=len;//�ļ�ʣ���С;
	long len1,len2;//len1ʵ�ʽ��յĴ�С;len2ÿ�δ�����ֽ���;
	int round=FILESIZE/SIZE;
	int step;	

	//������������ʼ��;
	if (round<=100)
	{
		step=100/round;
	}
	else
	{
		step=round/100;
	}
	int i=0,j=progress;

	while(re>0 && g_StopFlag[nsel])
	{
		//�ϵ��������Ĵ���;
		len2=re>SIZE?SIZE:re; //��ʣ���ļ��Ĵ�С�������趨���͵Ĵ�С�����ǽ���������Сlen2��Ϊ�趨��СSIZE;
		len1=down->Receive(m_buf,len2);//����һ������;
		if (len1<0)//������յ��Ĵ�СΪ0�������ӶϿ����߽��ճ����˳������߳�;
		{
			break;
		}
		destFile.Write(m_buf,len1);//�����յ�������д���ļ�;
		szFILE.seek+=len1;//�޸��ļ�����;
		szFILE.len-=len1;//�޸��ļ�ʣ���С

		myFile.Seek(0,CFile::begin);//����ǰ���ȴ�������ļ�;
		CString t1,t2,t3;
		t1.Format(_T("%I64u"),szFILE.len);
		t2.Format(_T("%I64u"),szFILE.seek);
		t3.Format(_T("%d"),j);
		myFile.Write("\xff\xfe",2);
		myFile.WriteString(t1);//������ʱ�ļ�����;
		myFile.WriteString(_T("\n"));
		myFile.WriteString(t2);//������ʱ�ļ�����;
		myFile.WriteString(_T("\n"));
		myFile.WriteString(t3);//������ʱ�ļ�����;

		re=re-len1;	//�����յ�������д���ĸ�;

		i++;
		//��������ʾ�Ż�;
		if (round<=100)
		{
			j+=step;
			m_dlg->TabPage1.m_DownLoadList.SetItemData(nsel,j);
		}
		else
		{
			if (i%step==0)
			{
				j++;
				m_dlg->TabPage1.m_DownLoadList.SetItemData(nsel,j);
			}
		}
	}

	destFile.Close();
	myFile.Close();
	down->Close();
	delete down;
	down=NULL;

	/***************************************************/
	if (re<=0) //�ļ��������;
	{
		//����������ɣ����潻�����Ż�;
		m_dlg->TabPage1.m_DownLoadList.SetItemText(nsel,3,_T("�����"));
		//�������Ż�;
		m_dlg->TabPage1.m_DownLoadList.SetItemData(nsel,100);
		//�����ʱ�ļ�;
		CFile TempFile; 
		TempFile.Remove(m_temp);
		//��ʾð�ݴ�;
		if (m_dlg->TabPage4.m_Check_Window.GetCheck()==1)
		{
			CString str_WndShow;
			str_WndShow=filename+_T("��������ϣ�");
			m_dlg->TabPage1.m_wndTaskbarNotifier1.Show(str_WndShow);
		}
		//���������������;
		if (m_dlg->TabPage4.m_Check_Sound.GetCheck()==1)
		{
			if (m_dlg->TabPage4.m_EDIT_SoundPath==_T(""))
			{
				PlaySound(MAKEINTRESOURCE(IDR_WAVE_Complete),AfxGetResourceHandle(),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT); 
			}
			else
			{
				sndPlaySound (m_dlg->TabPage4.m_EDIT_SoundPath,SND_ASYNC);
			}	
		}
	}
	/***************************************************/
	return 0;
}
/************************************************************************/
/* function:����ʼ���ذ�ť	                                         /
/************************************************************************/
void DownLoad::OnBnClickedButton2()    
{
	CSocket *DownSocket=new CSocket;
	DownSocket->Create();
	SOCKET hSOCKET=DownSocket->Detach();
	RECVPARAM *pRecvData=new RECVPARAM;
	pRecvData->serverSocket=DownSocket;
	pRecvData->currentSocket=NULL;
	pRecvData->hwnd=m_hWnd;			//��ǰ���ڵľ��;
	pRecvData->pSOCKET=hSOCKET;
	AfxBeginThread(ThreadDown,pRecvData);//�����µ��߳�����������;
	m_ButtonStart.EnableWindow(FALSE);
	m_ButtonPause.EnableWindow(TRUE);
}
/************************************************************************/
/* function:������ͣ��ť	                                            /
/************************************************************************/
void DownLoad::OnBnClickedButton1()
{
	int nSel=m_DownLoadList.GetNextItem(-1,LVNI_SELECTED);  //��õ�ǰ�б�ѡ������;				 
	g_StopFlag[nSel]=FALSE;
	m_DownLoadList.SetItemText(nSel,3,_T("��ͣ.."));
	m_ButtonStart.EnableWindow(TRUE);
	m_ButtonPause.EnableWindow(FALSE);
}
/************************************************************************/
/* function:����������ļ��б�Ի����������ļ���Ϣ		             /
/************************************************************************/
void DownLoad::showlist(CString filename,CString filetype,CString filesize,CString fileip)
{
	int nItem=m_DownLoadList.InsertItem(m_DownLoadList.GetItemCount(),filename);
	m_DownLoadList.SetItemText(nItem,1,filetype);
	m_DownLoadList.SetItemText(nItem,2,filesize);
	m_DownLoadList.SetItemText(nItem,3,_T("������"));
	m_DownLoadList.SetItemText(nItem,5,fileip);
}
/************************************************************************/
/* function:������������״̬���ı�ٿذ�ť״̬				             /
/************************************************************************/
void DownLoad::OnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nSel=m_DownLoadList.GetNextItem(-1,LVNI_SELECTED);  //��õ�ǰ�б�ѡ������;
	if (nSel!=-1)
	{
		CString temp;
		temp=m_DownLoadList.GetItemText(nSel,3);
		if (temp==_T("������"))
		{
			m_ButtonStart.EnableWindow(TRUE);
			m_ButtonPause.EnableWindow(FALSE);
			m_ButtonDelete.EnableWindow(TRUE);
		}
		if (temp==_T("��������.."))
		{
			m_ButtonStart.EnableWindow(FALSE);
			m_ButtonPause.EnableWindow(TRUE);
			m_ButtonDelete.EnableWindow(TRUE);
		}
		if (temp==_T("��ͣ.."))
		{
			m_ButtonStart.EnableWindow(TRUE);
			m_ButtonPause.EnableWindow(FALSE);
			m_ButtonDelete.EnableWindow(TRUE);
		}
		if (temp==_T("�����"))
		{
			m_ButtonStart.EnableWindow(FALSE);
			m_ButtonPause.EnableWindow(FALSE);
			m_ButtonDelete.EnableWindow(TRUE);
		}
	}
	*pResult = 0;
}
/************************************************************************/
/* function:ɾ������������		                                    /
/************************************************************************/
void DownLoad::OnBnClickedButton10()
{
	int nSel=m_DownLoadList.GetNextItem(-1,LVNI_SELECTED);  //��õ�ǰ�б�ѡ������;
	if (nSel!=-1)
	{
		m_DownLoadList.DeleteItem(nSel);
		if (m_DownLoadList.GetItemCount()==0)
		{
			m_ButtonStart.EnableWindow(FALSE);
			m_ButtonPause.EnableWindow(FALSE);
			m_ButtonDelete.EnableWindow(FALSE);
		}
	}
}
/************************************************************************/
/* function:���ػ��ִ���		                                         /
/************************************************************************/
void DownLoad::DownIntergral()
{
	CFireBirdDlg *m_dlg=(CFireBirdDlg *) AfxGetApp()->m_pMainWnd;
	CString temp;
	temp.Format(_T("D%s"),m_dlg->username);
	m_dlg->ClientSocket.Send(temp,temp.GetLength()*sizeof(TCHAR));
}
/************************************************************************/
/* function:�����˳�ǰ����δ���ص����񱣴�����			                 /
/************************************************************************/
void DownLoad::SaveTempFile()
{
	CFireBirdDlg *m_dlg=(CFireBirdDlg *) AfxGetApp()->m_pMainWnd;
	CString path=GlobalVariable::strDir_userdata+m_dlg->username+_T("\\");
	CreateDirectory(path,NULL);//�����û��ļ���;
	CStdioFile myDownFile;
	CString m_temp;
	m_temp=path+GlobalVariable::DownTempLog;
	myDownFile.Open(m_temp,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	int nCount=0;
	for (int i=0;i<m_DownLoadList.GetItemCount();i++)
	{
		CString temp;
		temp=m_DownLoadList.GetItemText(i,3);
		if (temp!=_T("�����"))
		{
			nCount++;
		}
	}
	if (nCount==0)
	{
		myDownFile.Close();
		return;
	}
	CString strCount;
	strCount.Format(_T("%d"),nCount);
	myDownFile.Write("\xff\xfe",2);
	myDownFile.WriteString(strCount);
	myDownFile.WriteString(_T("\n"));

	for (int i=0;i<m_DownLoadList.GetItemCount();i++)
	{
		CString temp;
		temp=m_DownLoadList.GetItemText(i,3);
		if (temp!=_T("�����"))
		{
			int progress;
			progress=m_DownLoadList.GetItemData(i);
			CString strProgress;
			strProgress.Format(_T("%d"),progress);
			myDownFile.WriteString(m_DownLoadList.GetItemText(i,0));
			myDownFile.WriteString(_T("\n"));
			myDownFile.WriteString(m_DownLoadList.GetItemText(i,1));
			myDownFile.WriteString(_T("\n"));
			myDownFile.WriteString(m_DownLoadList.GetItemText(i,2));
			myDownFile.WriteString(_T("\n"));
			myDownFile.WriteString(strProgress);
			myDownFile.WriteString(_T("\n"));
			myDownFile.WriteString(m_DownLoadList.GetItemText(i,5));
			myDownFile.WriteString(_T("\n"));
		}
	}
	myDownFile.Close();
}
/************************************************************************/
/* function:�û���¼��δ����������µ��б�			                 /
/************************************************************************/
void DownLoad::ReadTempFile()
{
	CFireBirdDlg *m_dlg=(CFireBirdDlg *) AfxGetApp()->m_pMainWnd;
	CString path=GlobalVariable::strDir_userdata+m_dlg->username+_T("\\")+GlobalVariable::DownTempLog;
	FILE *myFile_alr=NULL;   //�Ѵ��ڵ���ʱ�ļ�;
	if ((myFile_alr=_tfopen(path,_T("rb")))!=NULL)  
	{
		char unicodeheader[2];
		fread(unicodeheader,2,1,myFile_alr);

		TCHAR str_count[20],str_filename[50],str_filetype[20],str_filsize[20],str_progress[20],str_fileip[20];
		_ftscanf(myFile_alr,_T("%s\n"),&str_count);
		int nCount;
		nCount=_ttoi(str_count);

		for (int i=1;i<=nCount;i++)
		{
			_ftscanf(myFile_alr,_T("%s\n"),&str_filename);
			_ftscanf(myFile_alr,_T("%s\n"),&str_filetype);
			_ftscanf(myFile_alr,_T("%s\n"),&str_filsize);
			_ftscanf(myFile_alr,_T("%s\n"),&str_progress);
			_ftscanf(myFile_alr,_T("%s\n"),&str_fileip);


			CString str1,str2,str3,str5;
			int progress;
			str1.Format(L"%s",str_filename);
			str2.Format(L"%s",str_filetype);
			str3.Format(L"%s",str_filsize);
			str5.Format(L"%s",str_fileip);
			progress=_ttoi(str_progress);

			int nItem=m_DownLoadList.InsertItem(m_DownLoadList.GetItemCount(),str1);
			m_DownLoadList.SetItemText(nItem,1,str2);
			m_DownLoadList.SetItemText(nItem,2,str3);
			m_DownLoadList.SetItemText(nItem,3,_T("������"));
			m_DownLoadList.SetItemText(nItem,5,str5);
			m_DownLoadList.SetItemData(nItem,progress);
		}
		fclose(myFile_alr);
		CFile TempFile; 
		TempFile.Remove(path);
	}
	
}