#pragma once


// CFindPassword 对话框

class CFindPassword : public CDialog
{
	DECLARE_DYNAMIC(CFindPassword)

public:
	CFindPassword(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFindPassword();

// 对话框数据
	enum { IDD = IDD_FINDPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_email;
	void ReturnError();
	void ReturnSuccess();
};
