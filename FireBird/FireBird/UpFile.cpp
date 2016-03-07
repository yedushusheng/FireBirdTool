// UpFile.cpp : 实现文件
//

#include "stdafx.h"
#include "FireBird.h"
#include "UpFile.h"
#include "afxdialogex.h"
#include "FireBirdDlg.h"
#include "GlobalVariable.h"

// UpFile 对话框

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


// UpFile 消息处理程序

/************************************************************************/
/* function:上传对话框初始化                                            */
/************************************************************************/
BOOL UpFile::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	LONG lStyle; 
	lStyle = GetWindowLong(m_UpFileList.m_hWnd, GWL_STYLE);//获取当前窗口style 
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位 
	lStyle |= LVS_REPORT; //设置style 
	SetWindowLong(m_UpFileList.m_hWnd, GWL_STYLE, lStyle);//设置style 
	DWORD dwStyle = m_UpFileList.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl） 
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl） 
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件 
	m_UpFileList.SetExtendedStyle(dwStyle); //设置扩展风格 

	m_UpFileList.InsertColumn( 0, _T("文件名"), LVCFMT_LEFT, 220 );//插入列 
	m_UpFileList.InsertColumn( 1, _T("类型"), LVCFMT_LEFT, 70 ); 
	m_UpFileList.InsertColumn( 2, _T("大小"), LVCFMT_LEFT, 100 ); 
	m_UpFileList.InsertColumn( 3, _T("状态"), LVCFMT_LEFT, 100 ); 

	m_ButtonUpFile.EnableWindow(FALSE);
	m_ButtonDelFile.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
/************************************************************************/
/* function:选择文件显示在上传列表，等待选择上传                        */
/************************************************************************/
void UpFile::OnBnClickedButtonUp()	
{
	//创建选择文件对话框，选择文件;
	CString strFilter=_T("所有文件（*.*)|*.*|word文档(*.doc)|*.doc|压缩文件(*.rar)|*.rar|avi视频(*.avi)|*.avi|mp3音乐(*.mp3)|*.mp3|jpg图像(*.jpg)|*.jpg|bmp图像(*.bmp)|*.bmp||");   //选择文件下拉框类型;
	CFileDialog fileDlg(TRUE,NULL,_T(""),OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT|OFN_ENABLESIZING,strFilter,NULL);
	if (fileDlg.DoModal()==IDOK)      
	{
		int i=0;
		POSITION pos=fileDlg.GetStartPosition();
		while(pos)
		{
			CString filepath=fileDlg.GetNextPathName(pos);			   //获取文件全路径（..\..\*.*）;
			CString filennametype=filepath.Mid(filepath.ReverseFind( '\\')+1);  //获取文件名(既含名称，又含类型);
			CString filename=filennametype.Left(filennametype.ReverseFind('.'));  //获取文件名(只含名称);
			CString filetype=filennametype.Mid(filennametype.ReverseFind('.'));  //获得文件类型;
			CFileStatus rStatus;                      
			CFile::GetStatus(filepath,rStatus);      
			ULONGLONG ulong = rStatus.m_size; 
			CString filesize=CalculateFileSize(ulong);      //获取文件大小;
			m_FileInFo[i].filename=filename;    //将选择的文件的文件名依次存放进结构体;
			m_FileInFo[i].filetype=filetype;	 //将选择的文件的文件类型依次存放进结构体;
			m_FileInFo[i].filesize=filesize;	 //将选择的文件的文件大小依次存放进结构体;
			m_FileInFo[i].filepath=filepath;   //将选择的文件的文件路径依次存放进结构体;
			i++;                              //文件信息结构体数组元素项数+1;
		}
		m_count=i;
		//更新上传列表信息;
		int nItem=m_UpFileList.GetItemCount();      //获得当前列表项数;
		for (int i=0;i<m_count;i++)
		{
			m_UpFileList.InsertItem(nItem,m_FileInFo[i].filename);       //插入文件名;
			m_UpFileList.SetItemText(nItem,1,m_FileInFo[i].filetype);    //插入文件类型;
			m_UpFileList.SetItemText(nItem,2,m_FileInFo[i].filesize);    //插入文件大小;
			m_UpFileList.SetItemText(nItem,3,_T("待上传"));              //设置文件状态;
			m_filepath[nItem]=m_FileInFo[i].filepath;                   //将当前项的文件路径保存在一个字符串数组;
			nItem++;
		}
		m_ButtonDelFile.EnableWindow(TRUE);
	}
}
/*************************************************************************/
/* function:负责将文件索引信息发给服务器，并将文件在本机上移动共享文件夹 */
/*************************************************************************/
void UpFile::OnBnClickedButtonStart()
{
	CString filename,filetype,filesize,filepath;
	int nSel=m_UpFileList.GetNextItem(-1,LVNI_SELECTED);  //获得当前列表选择索引;
	if (nSel==-1)										  //若没有选择，则提示用户信息，并返回;
	{
		AfxMessageBox(_T("请选择上传的文件！"));
		return;								 
	}
	filename=m_UpFileList.GetItemText(nSel,0);   //从列表当前项选择项获得文件名;
	filetype=m_UpFileList.GetItemText(nSel,1);	 //从列表当前项选择项获得文件类型;
	filesize=m_UpFileList.GetItemText(nSel,2);	 //从列表当前项选择项获得文件大小;
	filepath=m_filepath[nSel];					 //从列表当前项选择项获得文件路径;

	CFireBirdDlg *m_dlg=(CFireBirdDlg *) AfxGetApp()->m_pMainWnd;    //获得主对话框指针;
	CString FileMsg;            
	//将文件名、文件归属的用户的用户名、文件类型、文件大小存进待发文件消息变量;
	FileMsg.Format(_T("U%s*%s*%s*%s"),filename,m_dlg->username,filetype,filesize);   //上传文件消息头标志——'U';
	
	//【注】此处上传文件消息回馈机制后期需要完善;
	if (m_dlg->ClientSocket.Send(FileMsg,FileMsg.GetLength()*sizeof(TCHAR))!=SOCKET_ERROR)  //向服务器发送上传文件消息;
	{
		CString strCopyFile=GlobalVariable::strDir_up+filename+filetype;  //获得得制后的文件路径名;
		CopyFile(filepath,strCopyFile,TRUE);							 //复制文件到FireBird共享文件夹;
		m_UpFileList.SetItemText(nSel,3,_T("已上传"));                   //设置文件状态;
		AfxMessageBox(_T("上传文件成功！"));  
		m_ButtonUpFile.EnableWindow(FALSE);
	}
	else
	{
		AfxMessageBox(_T("上传文件失败,请检查相关操作是否正确！"));
	}
}
/************************************************************************/
/* function:文件大小进制换算（GB,MB,KB,B）				
参数：uLong表示文件原始大小，原始单位B，使用ULONGLONG型可表示4G以上的文件*/
/************************************************************************/
CString UpFile::CalculateFileSize(ULONGLONG uLong)    
{
	//获取文件大小单位表示;
	CString filesize;
	double f= (double)uLong;
	if (f/1024>1)
	{
		if (f/1024/1024>1)
		{
			if (f/1024/1024/1024>1)
			{
				filesize.Format(_T("%.2fGB"),f/1024/1024/1024);   //文件大小GB显示;
				return filesize;
			}
			filesize.Format(_T("%.2fMB"),f/1024/1024);     //文件大小MB显示;
			return filesize;
		}
		filesize.Format(_T("%.2fKB"),f/1024);		 //文件大小KB显示;
	}
	else
	{
		filesize.Format(_T("%I64uB"),uLong);        //文件大小B显示;
	}
	return filesize;
}
/************************************************************************/
/* function:根据上传任务状态，改变操控按钮状态				             /
/************************************************************************/
void UpFile::OnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nSel=m_UpFileList.GetNextItem(-1,LVNI_SELECTED);  //获得当前列表选择索引;
	if (nSel!=-1)
	{
		CString temp;
		temp=m_UpFileList.GetItemText(nSel,3);
		if (temp==_T("待上传"))
		{
			m_ButtonUpFile.EnableWindow(TRUE);
		}
		if (temp==_T("已上传"))
		{
			m_ButtonUpFile.EnableWindow(FALSE);
		}
	}
	*pResult = 0;
}
/************************************************************************/
/* function:删除上传任务功能		                                    /
/************************************************************************/
void UpFile::OnBnClickedButton10()
{
	int nSel=m_UpFileList.GetNextItem(-1,LVNI_SELECTED);  //获得当前列表选择索引;
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
