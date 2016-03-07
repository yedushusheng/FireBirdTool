#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// UpFile �Ի���

class UpFile : public CDialogEx
{
	DECLARE_DYNAMIC(UpFile)

public:
	UpFile(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~UpFile();

// �Ի�������
	enum { IDD = IDD_UPFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_UpFileList;
	typedef struct{
		CString filename;
		CString filetype;
		CString filesize;
		CString filepath;
	}FileInfo;
	FileInfo m_FileInFo[30];     
	int m_count; 
	CString m_filepath[30];        
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonStart();
	CString CalculateFileSize(ULONGLONG uLong);   //�����ļ����ֱ�ʾ��С;KB,MB,GB
	UINT CopyThreadProc(LPVOID lparam);//�����ƶ��ļ��߳�;
	CButton m_ButtonSelect;
	CButton m_ButtonUpFile;
	CButton m_ButtonDelFile;
	afx_msg void OnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton10();
};
