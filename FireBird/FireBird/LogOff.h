#pragma once


// CLogOff 对话框

class CLogOff : public CDialog
{
	DECLARE_DYNAMIC(CLogOff)

public:
	CLogOff(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogOff();

// 对话框数据
	enum { IDD = IDD_LOGOFF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
