
// FireBirdDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "DownLoad.h"
#include "UpFile.h"
#include "SearchDlg.h"
#include "LoginDlg.h"
#include "Config.h"
#include "ChatRoom.h"
#include "afxwin.h"
#include "BmpButton.h"
#include "MySocket.h"
#include "RECVPARAM.h"



// CFireBirdDlg 对话框
class CFireBirdDlg : public CDialogEx
{
	// 构造
public:
	CFireBirdDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_FIREBIRD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	// 实现

protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	LRESULT OnHotKey(WPARAM     wParam,LPARAM     lParam);
	DECLARE_MESSAGE_MAP()

public:
	MySocket ClientSocket; //负责与服务器通信的套接字;
	CSocket *serverSocket;//负责p2p侦听的套接字;

	CString username;   //界面右上角当前用户名;
	int m_intergral;  //界面右上角当前积分;
	CString m_SendWords; //搜索关键词;
	CEdit m_SearchKeys;  //搜索编辑控件;

	BOOL IsSearch; //搜索列表初始化标志;
	BOOL LoginOff;//登录注销标志;

	//主界面Table控件类对象;
	CTabCtrl m_tab;	    //界面TABLE控件;
	DownLoad TabPage1;  //下载子页面;
	UpFile TabPage2;	//上传子页面;
	SearchDlg TabPage3; //搜索子页面;
	CConfig TabPage4;   //设置子页面;
	TCITEM tc1,tc2,tc3,tc4; //四个子页面;
	CRect rec; //Table控件范围;
	CString tabsel[4]; //用来记录子页面出现的先后顺序;
	int tabcount;//用来记录已出现子页面的数目;
	//主界面Button控件类对象;
	CBmpButton m_down; //下载Button图片;
	CBmpButton m_upFile; //上传Button图片;
	CBmpButton m_chat; //聊天室Button图片;
	CBmpButton m_conf; //设置Button图片;
	CBmpButton m_userlogin; //用户头像Button图片;
	CBmpButton m_search; //搜索Button图片;
	CBmpButton m_min; //最小化Button图片;
	
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);  //负责Table控件各页面切换;
	afx_msg LRESULT OnSystemTray(WPARAM wParam, LPARAM lParam);	  //托盘消息处理;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);//界面右上角用户名和积分STATIC控件重绘;
	afx_msg void OnBnClickedLogin();  //登录对话框显示;
	afx_msg void OnBnClickedUpfile(); //上传对话框显示;
	afx_msg void OnBnClickedChat(); //聊天室对话框显示;
	afx_msg void OnBnClickedSearch(); //搜索对话框显示;
	afx_msg void OnBnClickedShezhi(); //设置对话框显示;
	afx_msg void OnBnClickedButtonmin(); //负责界面最小化按钮功能;
	afx_msg void OnDestroy();  ///负责托盘退出后销毁托盘;
	afx_msg void OnExit(); //负责托盘退出;
	void _fnToTray();	//托盘;
	void GUI_Init();  //负责界面初始化;
	void ConnectServer(); //负责连接服务器;
	void Config_Init();  //设置初始化;
	void AutoLogin();//负责自动登录功能;
	void ServerListen(); //负责监听p2p客户端的下载连接请求;
	void UpdateImage(int image_id);//负责更新用户头像;
    void ServerRecv();//负责接收服务器发来的消息;
	void BeforeExitDo();//退出前处理;
	static UINT SendRecvProc(LPVOID lparam);//负责与客户端通讯的线程;
	static UINT ListenThreadProc(LPVOID lparam);//负责侦听的线程;
	afx_msg void OnClose();  //关闭主程序，向服务器发送消息;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);//主窗口最小化快捷键设置;
};
