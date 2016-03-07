#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// UpFile 对话框

class UpFile : public CDialogEx
{
	DECLARE_DYNAMIC(UpFile)

public:
	UpFile(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~UpFile();

// 对话框数据
	enum { IDD = IDD_UPFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	CString CalculateFileSize(ULONGLONG uLong);   //计算文件各种表示大小;KB,MB,GB
	UINT CopyThreadProc(LPVOID lparam);//负责移动文件线程;
	CButton m_ButtonSelect;
	CButton m_ButtonUpFile;
	CButton m_ButtonDelFile;
	afx_msg void OnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton10();
};
