// userinfo.h : Cuserinfo 类的实现



// Cuserinfo 实现

// 代码生成在 2013年11月26日 星期二, 下午 11:04

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

// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
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
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Text(pFX, _T("[username]"), m_username);
	RFX_Text(pFX, _T("[password]"), m_password);
	RFX_Text(pFX, _T("[email]"), m_email);
	RFX_Long(pFX, _T("[integral]"), m_integral);
	RFX_Long(pFX, _T("[image_id]"), m_image_id);

}
/////////////////////////////////////////////////////////////////////////////
// Cuserinfo 诊断

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


