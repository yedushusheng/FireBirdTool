
// FireBirdDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "DownLoad.h"
#include "UpFile.h"
#include "SearchDlg.h"
#include "LoginDlg.h"
#include "Config.h"
#include "ChatRoom.h"
#include "afxwin.h"
#include "BmpButton.h"
#include "MySocket.h"
#include "RECVPARAM.h"



// CFireBirdDlg �Ի���
class CFireBirdDlg : public CDialogEx
{
	// ����
public:
	CFireBirdDlg(CWnd* pParent = NULL);	// ��׼���캯��

	// �Ի�������
	enum { IDD = IDD_FIREBIRD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	// ʵ��

protected:
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	LRESULT OnHotKey(WPARAM     wParam,LPARAM     lParam);
	DECLARE_MESSAGE_MAP()

public:
	MySocket ClientSocket; //�����������ͨ�ŵ��׽���;
	CSocket *serverSocket;//����p2p�������׽���;

	CString username;   //�������Ͻǵ�ǰ�û���;
	int m_intergral;  //�������Ͻǵ�ǰ����;
	CString m_SendWords; //�����ؼ���;
	CEdit m_SearchKeys;  //�����༭�ؼ�;

	BOOL IsSearch; //�����б��ʼ����־;
	BOOL LoginOff;//��¼ע����־;

	//������Table�ؼ������;
	CTabCtrl m_tab;	    //����TABLE�ؼ�;
	DownLoad TabPage1;  //������ҳ��;
	UpFile TabPage2;	//�ϴ���ҳ��;
	SearchDlg TabPage3; //������ҳ��;
	CConfig TabPage4;   //������ҳ��;
	TCITEM tc1,tc2,tc3,tc4; //�ĸ���ҳ��;
	CRect rec; //Table�ؼ���Χ;
	CString tabsel[4]; //������¼��ҳ����ֵ��Ⱥ�˳��;
	int tabcount;//������¼�ѳ�����ҳ�����Ŀ;
	//������Button�ؼ������;
	CBmpButton m_down; //����ButtonͼƬ;
	CBmpButton m_upFile; //�ϴ�ButtonͼƬ;
	CBmpButton m_chat; //������ButtonͼƬ;
	CBmpButton m_conf; //����ButtonͼƬ;
	CBmpButton m_userlogin; //�û�ͷ��ButtonͼƬ;
	CBmpButton m_search; //����ButtonͼƬ;
	CBmpButton m_min; //��С��ButtonͼƬ;
	
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);  //����Table�ؼ���ҳ���л�;
	afx_msg LRESULT OnSystemTray(WPARAM wParam, LPARAM lParam);	  //������Ϣ����;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);//�������Ͻ��û����ͻ���STATIC�ؼ��ػ�;
	afx_msg void OnBnClickedLogin();  //��¼�Ի�����ʾ;
	afx_msg void OnBnClickedUpfile(); //�ϴ��Ի�����ʾ;
	afx_msg void OnBnClickedChat(); //�����ҶԻ�����ʾ;
	afx_msg void OnBnClickedSearch(); //�����Ի�����ʾ;
	afx_msg void OnBnClickedShezhi(); //���öԻ�����ʾ;
	afx_msg void OnBnClickedButtonmin(); //���������С����ť����;
	afx_msg void OnDestroy();  ///���������˳�����������;
	afx_msg void OnExit(); //���������˳�;
	void _fnToTray();	//����;
	void GUI_Init();  //��������ʼ��;
	void ConnectServer(); //�������ӷ�����;
	void Config_Init();  //���ó�ʼ��;
	void AutoLogin();//�����Զ���¼����;
	void ServerListen(); //�������p2p�ͻ��˵�������������;
	void UpdateImage(int image_id);//��������û�ͷ��;
    void ServerRecv();//������շ�������������Ϣ;
	void BeforeExitDo();//�˳�ǰ����;
	static UINT SendRecvProc(LPVOID lparam);//������ͻ���ͨѶ���߳�;
	static UINT ListenThreadProc(LPVOID lparam);//�����������߳�;
	afx_msg void OnClose();  //�ر��������������������Ϣ;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);//��������С����ݼ�����;
};
