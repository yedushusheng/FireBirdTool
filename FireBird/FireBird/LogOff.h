#pragma once


// CLogOff �Ի���

class CLogOff : public CDialog
{
	DECLARE_DYNAMIC(CLogOff)

public:
	CLogOff(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogOff();

// �Ի�������
	enum { IDD = IDD_LOGOFF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
