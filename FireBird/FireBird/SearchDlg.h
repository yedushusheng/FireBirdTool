#pragma once
#include "afxcmn.h"


// SearchDlg 对话框

class SearchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SearchDlg)

public:
	SearchDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SearchDlg();

// 对话框数据
	enum { IDD = IDD_SEARCHDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_SearchList;
	CStringArray fileip_array;
	BOOL m_ListFlag;
	void doSearch(TCHAR sTemp[200]);
	virtual BOOL OnInitDialog();
	
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
