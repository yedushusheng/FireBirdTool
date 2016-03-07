#pragma once
#include "afxwin.h"


// CConfig 对话框

class CConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CConfig)

public:
	CConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConfig();

	// 对话框数据
	enum { IDD = IDD_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonApplication();
	afx_msg void OnBnClickedButtonDefault();
	void SetInitial();
	virtual BOOL OnInitDialog();
	CButton m_Check_AutoRun;
	CButton m_Check_BossKey;
	CButton m_Check_ChatSendKey;
	CButton m_Check_MinToClose;
	CComboBox m_BossKey1;
	CEdit m_BossKey2;
	CComboBox m_ChatSendKey1;
	CEdit m_ChatSendKey2;
	int m_RADIO_RunToOpen;
	CComboBox m_Combo_DownPath;
	CComboBox m_Combo_UpPath;
	CButton m_Check_Sound;
	CString m_EDIT_SoundPath;
	CButton m_Check_Window;
	CButton m_Check_AutoLogin;
	CString m_EDIT_UserName;
	CString m_EDIT_PassWord;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton10();
	void AutoRun_RegKey(CString AutoRun);
};
