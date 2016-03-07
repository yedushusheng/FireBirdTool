#pragma once


// CAlterPassword 对话框

class CAlterPassword : public CDialog
{
	DECLARE_DYNAMIC(CAlterPassword)

public:
	CAlterPassword(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAlterPassword();

// 对话框数据
	enum { IDD = IDD_ALTERPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_UserName;
	CString m_OldPassWord;
	CString m_NewPassWord1;
	CString m_NewPassWord2;
	void ReturnError();
	void ReturnSuccess();
};
