// DownLoad.cpp : 实现文件
//

#include "stdafx.h"
#include "FireBird.h"
#include "DownLoad.h"
#include "afxdialogex.h"
#include "FireBirdDlg.h"
#include "GlobalVariable.h"
#include "mmsystem.h"//导入声音头文件
#pragma comment(lib,"winmm.lib")//导入声音头文件库

// DownLoad 对话框
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


// DownLoad 消息处理程序

/************************************************************************/
/* function:下载对话框初始化                                            /
/************************************************************************/
BOOL DownLoad::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	LONG lStyle; 
	lStyle = GetWindowLong(m_DownLoadList.m_hWnd, GWL_STYLE);//获取当前窗口style 
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位 
	lStyle |= LVS_REPORT; //设置style 
	SetWindowLong(m_DownLoadList.m_hWnd, GWL_STYLE, lStyle);//设置style 
	DWORD dwStyle = m_DownLoadList.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl） 
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl） 
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件 
	m_DownLoadList.SetExtendedStyle(dwStyle); //设置扩展风格 

	m_DownLoadList.InsertColumn( 0, _T("文件名"), LVCFMT_LEFT, 180 );//插入列 
	m_DownLoadList.InsertColumn( 1, _T("类型"), LVCFMT_LEFT, 70 ); 
	m_DownLoadList.InsertColumn( 2, _T("大小"), LVCFMT_LEFT, 70 ); 
	m_DownLoadList.InsertColumn( 3, _T("状态"), LVCFMT_LEFT, 70 ); 
	m_DownLoadList.InsertColumn( 4, _T("进度"), LVCFMT_LEFT, 130 ); 
	m_DownLoadList.InsertColumn( 5, _T("IP"), LVCFMT_LEFT, 0 ); 
	// TODO:  在此添加额外的初始化

	m_wndTaskbarNotifier1.Create(this);
	m_wndTaskbarNotifier1.SetSkin(IDB_MaoPao);
	m_wndTaskbarNotifier1.SetTextFont(_T("黑体"),110,TN_TEXT_NORMAL,TN_TEXT_UNDERLINE);
	m_wndTaskbarNotifier1.SetTextColor(RGB(0,0,0),RGB(0,0,200));
	m_wndTaskbarNotifier1.SetTextRect(CRect(45,60,m_wndTaskbarNotifier1.m_nSkinWidth-10,m_wndTaskbarNotifier1.m_nSkinHeight-25));

	m_ButtonStart.EnableWindow(FALSE);
	m_ButtonPause.EnableWindow(FALSE);
	m_ButtonDelete.EnableWindow(FALSE);

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
/************************************************************************/
/* function:接收下载文件线程	                                         /
/************************************************************************/
UINT DownLoad::ThreadDown(LPVOID lparam)
{
	RECVPARAM *pRecvData=(RECVPARAM *)lparam;
	CSocket *down=pRecvData->serverSocket; 

	SOCKET sock=pRecvData->pSOCKET;
	down->Attach(sock);//将CSocket与SOCKET绑定

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
		AfxMessageBox(_T("连接下载文件对方客户端失败！"));
		return 0;
	}

	down->Send(filename,filename.GetLength()*sizeof(TCHAR));
	TCHAR szFileInfo[1024] = {0}; 
	down->Receive((void*)szFileInfo,1024);  //接收到下载文件长度;
	tagFILE  *szFile_Recv=(tagFILE*) szFileInfo;  
	if (szFile_Recv->len==-1)
	{
		AfxMessageBox(_T("该文件不存在，可能对方已删除该文件！"));
		return 0;
	}
	ULONGLONG FILESIZE=szFile_Recv->len;

	if (m_dlg->m_intergral<2)
	{
		AfxMessageBox(_T("你积分不够，请上传文件赚取积分！"));
		return 0;
	}
	/***************************************************/
	CString str_filepath=_T("");
	str_filepath+=GlobalVariable::strDir_down+filename;
	CString m_temp=str_filepath+_T(".down");  //临时文件的名字为文件名加后缀.down

	int progress;
	tagFILE szFILE;
	CStdioFile myFile_ini;  //初始下载建立的临时文件;
	FILE *myFile_alr=NULL;   //已存在的临时文件;

	if ((myFile_alr=_tfopen(m_temp,_T("rb")))==NULL)     //初始下载，没有临时文件;
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
	else			//中断继续下载，有临时文件;
	{
		// (关键片段);
		// Step 1: 先读取 0xfffe (你也可以跳过 2 bytes);
		char unicodeheader[2];
		fread(unicodeheader,2,1,myFile_alr);

		// Step 2: 真正读取 Unicode 内容;
		TCHAR str_len[25],str_seek[25],str_pro[25];
		_ftscanf(myFile_alr,_T("%s\n"),&str_len);
		_ftscanf(myFile_alr,_T("%s\n"),&str_seek);
		_ftscanf(myFile_alr,_T("%s\n"),&str_pro);
		fclose(myFile_alr);

		//Step 3: 转换为ULONGLONG类型;
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

	//给对方发送下载请求消息结构体;
	down->Send(&szFILE,sizeof(szFILE));   //发送文件下载进度;

	//界面交互优化;
	m_dlg->TabPage1.m_DownLoadList.SetItemText(nsel,3,_T("正在下载.."));

	ULONGLONG len=szFILE.len;
	char* m_buf=new char[SIZE];
	ULONGLONG re=len;//文件剩余大小;
	long len1,len2;//len1实际接收的大小;len2每次传输的字节数;
	int round=FILESIZE/SIZE;
	int step;	

	//进度条增量初始化;
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
		//断点续传核心代码;
		len2=re>SIZE?SIZE:re; //若剩余文件的大小还大于设定发送的大小，则还是将缓冲区大小len2设为设定大小SIZE;
		len1=down->Receive(m_buf,len2);//接收一块数据;
		if (len1<0)//如果接收到的大小为0，则连接断开或者接收出错，退出接收线程;
		{
			break;
		}
		destFile.Write(m_buf,len1);//将接收到的数据写入文件;
		szFILE.seek+=len1;//修改文件进度;
		szFILE.len-=len1;//修改文件剩余大小

		myFile.Seek(0,CFile::begin);//将当前进度存进下载文件;
		CString t1,t2,t3;
		t1.Format(_T("%I64u"),szFILE.len);
		t2.Format(_T("%I64u"),szFILE.seek);
		t3.Format(_T("%d"),j);
		myFile.Write("\xff\xfe",2);
		myFile.WriteString(t1);//更临临时文件数据;
		myFile.WriteString(_T("\n"));
		myFile.WriteString(t2);//更临临时文件数据;
		myFile.WriteString(_T("\n"));
		myFile.WriteString(t3);//更临临时文件数据;

		re=re-len1;	//将接收到的数据写入文个;

		i++;
		//进度条显示优化;
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
	if (re<=0) //文件下载完成;
	{
		//任务下载完成，界面交互性优化;
		m_dlg->TabPage1.m_DownLoadList.SetItemText(nsel,3,_T("已完成"));
		//进度条优化;
		m_dlg->TabPage1.m_DownLoadList.SetItemData(nsel,100);
		//清除临时文件;
		CFile TempFile; 
		TempFile.Remove(m_temp);
		//显示冒泡窗;
		if (m_dlg->TabPage4.m_Check_Window.GetCheck()==1)
		{
			CString str_WndShow;
			str_WndShow=filename+_T("已下载完毕！");
			m_dlg->TabPage1.m_wndTaskbarNotifier1.Show(str_WndShow);
		}
		//播放下载完成声音;
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
/* function:负责开始下载按钮	                                         /
/************************************************************************/
void DownLoad::OnBnClickedButton2()    
{
	CSocket *DownSocket=new CSocket;
	DownSocket->Create();
	SOCKET hSOCKET=DownSocket->Detach();
	RECVPARAM *pRecvData=new RECVPARAM;
	pRecvData->serverSocket=DownSocket;
	pRecvData->currentSocket=NULL;
	pRecvData->hwnd=m_hWnd;			//当前窗口的句柄;
	pRecvData->pSOCKET=hSOCKET;
	AfxBeginThread(ThreadDown,pRecvData);//启动新的线程来负责下载;
	m_ButtonStart.EnableWindow(FALSE);
	m_ButtonPause.EnableWindow(TRUE);
}
/************************************************************************/
/* function:负责暂停按钮	                                            /
/************************************************************************/
void DownLoad::OnBnClickedButton1()
{
	int nSel=m_DownLoadList.GetNextItem(-1,LVNI_SELECTED);  //获得当前列表选择索引;				 
	g_StopFlag[nSel]=FALSE;
	m_DownLoadList.SetItemText(nSel,3,_T("暂停.."));
	m_ButtonStart.EnableWindow(TRUE);
	m_ButtonPause.EnableWindow(FALSE);
}
/************************************************************************/
/* function:负责从搜索文件列表对话框获得下载文件信息		             /
/************************************************************************/
void DownLoad::showlist(CString filename,CString filetype,CString filesize,CString fileip)
{
	int nItem=m_DownLoadList.InsertItem(m_DownLoadList.GetItemCount(),filename);
	m_DownLoadList.SetItemText(nItem,1,filetype);
	m_DownLoadList.SetItemText(nItem,2,filesize);
	m_DownLoadList.SetItemText(nItem,3,_T("待下载"));
	m_DownLoadList.SetItemText(nItem,5,fileip);
}
/************************************************************************/
/* function:根据下载任务状态，改变操控按钮状态				             /
/************************************************************************/
void DownLoad::OnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nSel=m_DownLoadList.GetNextItem(-1,LVNI_SELECTED);  //获得当前列表选择索引;
	if (nSel!=-1)
	{
		CString temp;
		temp=m_DownLoadList.GetItemText(nSel,3);
		if (temp==_T("待下载"))
		{
			m_ButtonStart.EnableWindow(TRUE);
			m_ButtonPause.EnableWindow(FALSE);
			m_ButtonDelete.EnableWindow(TRUE);
		}
		if (temp==_T("正在下载.."))
		{
			m_ButtonStart.EnableWindow(FALSE);
			m_ButtonPause.EnableWindow(TRUE);
			m_ButtonDelete.EnableWindow(TRUE);
		}
		if (temp==_T("暂停.."))
		{
			m_ButtonStart.EnableWindow(TRUE);
			m_ButtonPause.EnableWindow(FALSE);
			m_ButtonDelete.EnableWindow(TRUE);
		}
		if (temp==_T("已完成"))
		{
			m_ButtonStart.EnableWindow(FALSE);
			m_ButtonPause.EnableWindow(FALSE);
			m_ButtonDelete.EnableWindow(TRUE);
		}
	}
	*pResult = 0;
}
/************************************************************************/
/* function:删除下载任务功能		                                    /
/************************************************************************/
void DownLoad::OnBnClickedButton10()
{
	int nSel=m_DownLoadList.GetNextItem(-1,LVNI_SELECTED);  //获得当前列表选择索引;
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
/* function:下载积分处理		                                         /
/************************************************************************/
void DownLoad::DownIntergral()
{
	CFireBirdDlg *m_dlg=(CFireBirdDlg *) AfxGetApp()->m_pMainWnd;
	CString temp;
	temp.Format(_T("D%s"),m_dlg->username);
	m_dlg->ClientSocket.Send(temp,temp.GetLength()*sizeof(TCHAR));
}
/************************************************************************/
/* function:程序退出前，将未下载的任务保存下来			                 /
/************************************************************************/
void DownLoad::SaveTempFile()
{
	CFireBirdDlg *m_dlg=(CFireBirdDlg *) AfxGetApp()->m_pMainWnd;
	CString path=GlobalVariable::strDir_userdata+m_dlg->username+_T("\\");
	CreateDirectory(path,NULL);//创建用户文件夹;
	CStdioFile myDownFile;
	CString m_temp;
	m_temp=path+GlobalVariable::DownTempLog;
	myDownFile.Open(m_temp,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	int nCount=0;
	for (int i=0;i<m_DownLoadList.GetItemCount();i++)
	{
		CString temp;
		temp=m_DownLoadList.GetItemText(i,3);
		if (temp!=_T("已完成"))
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
		if (temp!=_T("已完成"))
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
/* function:用户登录后将未下载任务更新到列表			                 /
/************************************************************************/
void DownLoad::ReadTempFile()
{
	CFireBirdDlg *m_dlg=(CFireBirdDlg *) AfxGetApp()->m_pMainWnd;
	CString path=GlobalVariable::strDir_userdata+m_dlg->username+_T("\\")+GlobalVariable::DownTempLog;
	FILE *myFile_alr=NULL;   //已存在的临时文件;
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
			m_DownLoadList.SetItemText(nItem,3,_T("待下载"));
			m_DownLoadList.SetItemText(nItem,5,str5);
			m_DownLoadList.SetItemData(nItem,progress);
		}
		fclose(myFile_alr);
		CFile TempFile; 
		TempFile.Remove(path);
	}
	
}