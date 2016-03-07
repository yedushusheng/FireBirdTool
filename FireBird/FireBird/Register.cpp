// Register.cpp : 实现文件
//

#include "stdafx.h"
#include "FireBird.h"
#include "Register.h"
#include "afxdialogex.h"
#include "FireBirdDlg.h"



// CRegister 对话框

IMPLEMENT_DYNAMIC(CRegister, CDialogEx)

CRegister::CRegister(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegister::IDD, pParent)
	, m_username(_T(""))
	, m_password1(_T(""))
	, m_password2(_T(""))
	, m_email(_T(""))
{

}

CRegister::~CRegister()
{
}

void CRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_affirm,m_affirm);
	DDX_Control(pDX,IDC_cancel,m_cancel);
	DDX_Text(pDX, IDC_EDIT1, m_username);
	DDX_Text(pDX, IDC_EDIT9, m_password1);
	DDX_Text(pDX, IDC_EDIT3, m_password2);
	DDX_Text(pDX, IDC_EDIT7, m_email);
	DDX_Control(pDX, IDC_COMBOBOXEX1, m_ImageComboBox);
}


BEGIN_MESSAGE_MAP(CRegister, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_cancel, &CRegister::OnBnClickedcancel)
	ON_BN_CLICKED(IDC_affirm, &CRegister::OnBnClickedaffirm)
	
END_MESSAGE_MAP()


// CRegister 消息处理程序

/************************************************************************/
/* function:注册对话框背景贴图                                           */
/************************************************************************/
void CRegister::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpBackground;
	bmpBackground.LoadBitmap(IDB_RegisterBG);
	BITMAP bitmap;
	bmpBackground.GetBitmap(&bitmap);
	CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground);
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}
/************************************************************************/
/* function:注册对话框初始化                                            */
/************************************************************************/
BOOL CRegister::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	 CRect   temprect(0,0,564,521);
	 CWnd::SetWindowPos(NULL,0,0,temprect.Width(),temprect.Height(),SWP_NOZORDER|SWP_NOMOVE);

	 m_affirm.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Register)));
	 m_affirm.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_RegisterHot)),
		 LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Register)));
	 m_cancel.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Cancel)));
	 m_cancel.SetHBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_CancelHot)),
		 LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Cancel)));
	 //头像列表;
	 CImageList* pImageList; 
	 pImageList = new CImageList(); 
	 pImageList->Create(42,41, ILC_COLOR32, 0, 8);
	 CBitmap bmp; 
	 bmp.LoadBitmap(IDB_Register001); 
	 pImageList->Add(&bmp, RGB(0, 0, 0)); 
	 bmp.DeleteObject();
	 bmp.LoadBitmap(IDB_Register002); 
	 pImageList->Add(&bmp, RGB(0, 0, 0)); 
	 bmp.DeleteObject();
	 bmp.LoadBitmap(IDB_Register003); 
	 pImageList->Add(&bmp, RGB(0, 0, 0)); 
	 bmp.DeleteObject();
	 bmp.LoadBitmap(IDB_Register004); 
	 pImageList->Add(&bmp, RGB(0, 0, 0)); 
	 bmp.DeleteObject();
	 bmp.LoadBitmap(IDB_Register005); 
	 pImageList->Add(&bmp, RGB(0, 0, 0)); 
	 bmp.DeleteObject();
	 bmp.LoadBitmap(IDB_Register006); 
	 pImageList->Add(&bmp, RGB(0, 0, 0)); 
	 bmp.DeleteObject();
	 bmp.LoadBitmap(IDB_Register007); 
	 pImageList->Add(&bmp, RGB(0, 0, 0)); 
	 bmp.DeleteObject();
	 bmp.LoadBitmap(IDB_Register008); 
	 pImageList->Add(&bmp, RGB(0, 0, 0)); 
	 bmp.DeleteObject();
	 m_ImageComboBox.SetImageList(pImageList);
	 COMBOBOXEXITEM comboItem;
	 comboItem.mask=CBEIF_IMAGE|CBEIF_SELECTEDIMAGE;
	 for (int i=0;i<8;i++)
	 {
		 comboItem.iItem=i;
		 comboItem.iImage=i;
		 comboItem.iSelectedImage=i;
		 m_ImageComboBox.InsertItem(&comboItem);
	 }
	 m_ImageComboBox.SetCurSel(5);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
/************************************************************************/
/* function:关闭注册对话框                                             */
/************************************************************************/
void CRegister::OnBnClickedcancel()
{
	CRegister::OnCancel();
}
/************************************************************************/
/* function:确定注册                                                  */
/************************************************************************/
void CRegister::OnBnClickedaffirm()
{
	CFireBirdDlg *m_dlg=(CFireBirdDlg *) AfxGetApp()->m_pMainWnd;
	UpdateData();
	if (m_username==""||m_password1==""||m_password2==""||m_email=="")
	{
		AfxMessageBox(_T("请输入完整注册信息！"));
		return;
	}
	if (m_password1!=m_password2)
	{
		AfxMessageBox(_T("两次输入密码不一致！"));
		return;
	}
	if (m_username.GetLength()<4)
	{
		AfxMessageBox(_T("用户名最少4位！"));
		return;
	}
	if (m_password1.GetLength()<6)
	{
		AfxMessageBox(_T("密码最少为六位！"));
		return;
	}
	if (m_email==_T(""))
	{
		AfxMessageBox(_T("请输入邮箱"));
		return;
	}
	if (m_email.ReverseFind('@')==-1)
	{
		AfxMessageBox(_T("请输入正确的邮箱格式"));
		return;
	}
	if (m_email.Find(_T(".com"))==-1)
	{
		AfxMessageBox(_T("请输入正确的邮箱格式"));
		return;
	}
	int nSel=m_ImageComboBox.GetCurSel();
	CString temp;
	temp.Format(_T("R%s*%s*%s*%d"),m_username,m_password2,m_email,nSel);
	DWORD n=temp.GetLength()*sizeof(TCHAR);
	m_dlg->ClientSocket.Send(temp,n);
}
void CRegister::ReturnError1()
{
	AfxMessageBox(_T("该用户名已存在"));
}
void CRegister::ReturnError2()
{
	AfxMessageBox(_T("该邮箱已注册"));
}
void CRegister::ReturnSuccess()
{
	MessageBox(_T("注册成功！"),_T("注册"),MB_OK);
}
void CRegister::OnCancel()
{
	CDialogEx::OnCancel();
}