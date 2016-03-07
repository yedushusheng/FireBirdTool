#pragma once
#include "afxcmn.h"
#include "ProcessList.h"
#include "BubleWnd.h"
#include "afxwin.h"

// DownLoad 对话框


class DownLoad : public CDialogEx
{
	DECLARE_DYNAMIC(DownLoad)

public:
	
	DownLoad(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DownLoad();

// 对话框数据
	enum { IDD = IDD_DOWNLOAD };

protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CBubleWnd m_wndTaskbarNotifier1;
	CProcessList m_DownLoadList;
	static UINT ThreadDown(LPVOID lparam);//负责下载的线程
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	void showlist(CString,CString,CString,CString); //负责从搜索列表中获得下载文件的信息;
	CButton m_ButtonStart;
	CButton m_ButtonPause;
	CButton m_ButtonDelete;
	afx_msg void OnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton10();
	void DownIntergral();
	void SaveTempFile();
	void ReadTempFile();
};
