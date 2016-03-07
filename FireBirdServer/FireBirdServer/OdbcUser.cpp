// OdbcUser.h : COdbcUser ���ʵ��



// COdbcUser ʵ��

// ���������� 2013��11��11�� ����һ, ���� 7:36

#include "stdafx.h"
#include "OdbcUser.h"
IMPLEMENT_DYNAMIC(COdbcUser, CRecordset)

COdbcUser::COdbcUser(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_username = "";
	m_password = "";
	m_email = "";
	m_integral = 0;
	m_image_id = 0;
	m_nFields = 5;
	m_nDefaultType = dynaset;
}

// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString COdbcUser::GetDefaultConnect()
{
	return _T("DSN=FireBird;UID=sa;PWD=666666;APP=Microsoft\x00ae Visual Studio\x00ae 2010;WSID=PAITOKNGYDQ6OO0;DATABASE=FireBird");
}

CString COdbcUser::GetDefaultSQL()
{
	return _T("[dbo].[userinfo]");
}

void COdbcUser::DoFieldExchange(CFieldExchange* pFX)
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
// COdbcUser ���

#ifdef _DEBUG
void COdbcUser::AssertValid() const
{
	CRecordset::AssertValid();
}

void COdbcUser::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


