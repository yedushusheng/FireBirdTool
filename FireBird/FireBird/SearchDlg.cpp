// SearchDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FireBird.h"
#include "SearchDlg.h"
#include "afxdialogex.h"
#include "FireBirdDlg.h"

// SearchDlg 对话框

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


// SearchDlg 消息处理程序

/************************************************************************/
/* function:搜索对话框初始化                                            */
/************************************************************************/
BOOL SearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LONG lStyle; 
	lStyle = GetWindowLong(m_SearchList.m_hWnd, GWL_STYLE);//获取当前窗口style 
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位 
	lStyle |= LVS_REPORT; //设置style 
	SetWindowLong(m_SearchList.m_hWnd, GWL_STYLE, lStyle);//设置style 
	DWORD dwStyle = m_SearchList.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl） 
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl） 
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件 
	m_SearchList.SetExtendedStyle(dwStyle); //设置扩展风格 

	m_SearchList.InsertColumn( 0, _T("文件名"), LVCFMT_LEFT, 220 );//插入列 
	m_SearchList.InsertColumn( 1, _T("类型"), LVCFMT_LEFT, 70 ); 
	m_SearchList.InsertColumn( 2, _T("大小"), LVCFMT_LEFT, 100 ); 
	m_SearchList.InsertColumn( 3, _T("来源"), LVCFMT_LEFT, 120 ); 

	m_ListFlag=FALSE;		  //标志这次搜索，相关操作未初始化;

	//搜索交互消息字体设置
	CFont *m_Font = new CFont;
	m_Font->CreateFont(-13,0,0,0,100,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,_T("宋体"));
	CStatic *m_Static=(CStatic *)GetDlgItem(IDC_STATIC);
	m_Static->SetFont(m_Font,FALSE);

	CFont *m_Font2 = new CFont;
	m_Font2->CreateFont(-13,0,0,0,700,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,_T("黑体"));
	CStatic *m_Static2=(CStatic *)GetDlgItem(IDC_STATIC_SearchWords);
	m_Static2->SetFont(m_Font2,FALSE);
	CStatic *m_Static3=(CStatic *)GetDlgItem(IDC_STATIC_nCount);
	m_Static3->SetFont(m_Font2,FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
/************************************************************************/
/* function:根据服务器返回消息，将返回的搜索结果插入进搜索列表
   参数：TCHAR sTemp[200]，含消息头，一次只返回一个文件结果             */
/************************************************************************/
void SearchDlg::doSearch(TCHAR sTemp[200])
{
	if (sTemp[1]=='F')      //F是消息开始标志;
	{
		if (!m_ListFlag)
		{
			fileip_array.RemoveAll();			//每新一次搜索，就清空搜索列表项对应的IP;
			m_ListFlag=TRUE;                    //标志这次搜索，相关操作已初始化;
		}
		int i=2;
		CString filename=_T("");
		CString filetype=_T("");
		CString filesize=_T("");
		CString fileuser=_T("");
		CString fileip=_T("");
		do 
		{
			filename+=sTemp[i];			  //获取文件名;
			i++;
		} while (sTemp[i]!='*');
		i++;
		do 
		{
			filetype+=sTemp[i];			  //获取文件类型;
			i++;
		} while (sTemp[i]!='*');
		i++;
		do 
		{
			filesize+=sTemp[i];			  //获取文件大小;
			i++;
		} while (sTemp[i]!='*');		  
		i++;
		do 
		{
			fileuser+=sTemp[i];			  //获取文件的拥有者的用户名;
			i++;
		} while (sTemp[i]!='*');
		i++;
		do 
		{
			fileip+=sTemp[i];			  //获取文件的拥有者当前IP;
			i++;
		} while (sTemp[i]!='\0');

		int nItem=m_SearchList.InsertItem(m_SearchList.GetItemCount(),filename);    //将文件名插入搜索列表;
		m_SearchList.SetItemText(nItem,1,filetype);		//将文件类型插入搜索列表;
		m_SearchList.SetItemText(nItem,2,filesize);		//将文件大小插入搜索列表;
		m_SearchList.SetItemText(nItem,3,fileuser);		//将文件拥有者用户名插入搜索列表;
		fileip_array.Add(fileip);						//将文件拥有者当前IP保存在辅助数组，该文件在该数组的位置和nItem一致;

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
/* function:搜索列表双击文件事件                                        */
/************************************************************************/
void SearchDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nsel=m_SearchList.GetNextItem(-1,LVNI_SELECTED);    
	if(nsel!=-1)  
	{
		CFireBirdDlg *m_dlg=(CFireBirdDlg *) AfxGetApp()->m_pMainWnd;      //获得主对话框句柄指针;
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
