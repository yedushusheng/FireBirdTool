#pragma once
#include "BmpButton.h"


// CLoginDlg 对话框
class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginDlg();
// 对话框数据
	enum { IDD = IDD_LOGINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBrush m_brBk;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedLogin();
	CString m_username;
	CString m_passwords;
	virtual void OnCancel();
	CBmpButton m_ButtonLogin;
	CBmpButton m_ButtonRegister;
	CBmpButton m_ButtonAlter;
	CBmpButton m_ButtonFind;
	afx_msg void OnBnClickedForgetpw();
	afx_msg void OnBnClickedAlterpw();
};
