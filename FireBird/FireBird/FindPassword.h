#pragma once


// CFindPassword �Ի���

class CFindPassword : public CDialog
{
	DECLARE_DYNAMIC(CFindPassword)

public:
	CFindPassword(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFindPassword();

// �Ի�������
	enum { IDD = IDD_FINDPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_email;
	void ReturnError();
	void ReturnSuccess();
};
