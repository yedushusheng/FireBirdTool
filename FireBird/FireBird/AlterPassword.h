#pragma once


// CAlterPassword �Ի���

class CAlterPassword : public CDialog
{
	DECLARE_DYNAMIC(CAlterPassword)

public:
	CAlterPassword(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAlterPassword();

// �Ի�������
	enum { IDD = IDD_ALTERPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
