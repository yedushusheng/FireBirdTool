#pragma once
#include "afxcmn.h"
#include "ProcessList.h"
#include "BubleWnd.h"
#include "afxwin.h"

// DownLoad �Ի���


class DownLoad : public CDialogEx
{
	DECLARE_DYNAMIC(DownLoad)

public:
	
	DownLoad(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DownLoad();

// �Ի�������
	enum { IDD = IDD_DOWNLOAD };

protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CBubleWnd m_wndTaskbarNotifier1;
	CProcessList m_DownLoadList;
	static UINT ThreadDown(LPVOID lparam);//�������ص��߳�
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	void showlist(CString,CString,CString,CString); //����������б��л�������ļ�����Ϣ;
	CButton m_ButtonStart;
	CButton m_ButtonPause;
	CButton m_ButtonDelete;
	afx_msg void OnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton10();
	void DownIntergral();
	void SaveTempFile();
	void ReadTempFile();
};
