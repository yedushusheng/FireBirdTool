// sharedfile.h : Csharedfile ���ʵ��



// Csharedfile ʵ��

// ���������� 2013��11��26�� ���ڶ�, ���� 11:03

#include "stdafx.h"
#include "sharedfile.h"
IMPLEMENT_DYNAMIC(Csharedfile, CRecordset)

Csharedfile::Csharedfile(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_file_id = 0;
	m_filename = L"";
	m_username = L"";
	m_filetype = L"";
	m_filesize = L"";
	m_nFields = 5;
	m_nDefaultType = dynaset;
}

// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString Csharedfile::GetDefaultConnect()
{
	return _T("DSN=FireBird;UID=sa;PWD=666666;APP=Microsoft\x00ae Visual Studio\x00ae 2010;WSID=PAITOKNGYDQ6OO0;DATABASE=FireBird");
}

CString Csharedfile::GetDefaultSQL()
{
	return _T("[dbo].[sharedfile]");
}

void Csharedfile::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[file_id]"), m_file_id);
	RFX_Text(pFX, _T("[filename]"), m_filename);
	RFX_Text(pFX, _T("[username]"), m_username);
	RFX_Text(pFX, _T("[filetype]"), m_filetype);
	RFX_Text(pFX, _T("[filesize]"), m_filesize);

}
/////////////////////////////////////////////////////////////////////////////
// Csharedfile ���

#ifdef _DEBUG
void Csharedfile::AssertValid() const
{
	CRecordset::AssertValid();
}

void Csharedfile::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


