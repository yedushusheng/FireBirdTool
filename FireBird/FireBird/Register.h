#pragma once

#include "BmpButton.h"
#include "afxcmn.h"
// CRegister 对话框

class CRegister : public CDialogEx
{
	DECLARE_DYNAMIC(CRegister)

public:
	CRegister(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegister();

	// 对话框数据
	enum { IDD = IDD_REGISTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	CBmpButton m_affirm;
	CBmpButton m_cancel;
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedcancel();
	CString m_username;
	CString m_password1;
	CString m_password2;
	CString m_email;
	afx_msg void OnBnClickedaffirm();
	CComboBoxEx m_ImageComboBox;
	void ReturnError1();
	void ReturnError2();
	void ReturnSuccess();
	void OnCancel();
};
