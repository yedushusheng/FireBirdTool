// userinfo.h : Cuserinfo ���ʵ��



// Cuserinfo ʵ��

// ���������� 2013��11��26�� ���ڶ�, ���� 11:04

#include "stdafx.h"
#include "userinfo.h"
IMPLEMENT_DYNAMIC(Cuserinfo, CRecordset)

Cuserinfo::Cuserinfo(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_username = L"";
	m_password = L"";
	m_email = L"";
	m_integral = 0;
	m_image_id = 0;
	m_nFields = 5;
	m_nDefaultType = dynaset;
}

// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString Cuserinfo::GetDefaultConnect()
{
	return _T("DSN=FireBird;UID=sa;PWD=666666;APP=Microsoft\x00ae Visual Studio\x00ae 2010;WSID=PAITOKNGYDQ6OO0;DATABASE=FireBird");
}

CString Cuserinfo::GetDefaultSQL()
{
	return _T("[dbo].[userinfo]");
}

void Cuserinfo::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[username]"), m_username);
	RFX_Text(pFX, _T("[password]"), m_password);
	RFX_Text(pFX, _T("[email]"), m_email);
	RFX_Long(pFX, _T("[integral]"), m_integral);
	RFX_Long(pFX, _T("[image_id]"), m_image_id);

}
/////////////////////////////////////////////////////////////////////////////
// Cuserinfo ���

#ifdef _DEBUG
void Cuserinfo::AssertValid() const
{
	CRecordset::AssertValid();
}

void Cuserinfo::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


