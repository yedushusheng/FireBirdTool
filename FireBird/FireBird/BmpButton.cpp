// BmpButton.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FireBird.h"
#include "BmpButton.h"


// CBmpButton

IMPLEMENT_DYNAMIC(CBmpButton, CButton)

CBmpButton::CBmpButton()
{

}

CBmpButton::~CBmpButton()
{
}


BEGIN_MESSAGE_MAP(CBmpButton, CButton)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CBmpButton ��Ϣ�������

void CBmpButton::SetHBitmap(HBITMAP m_Down1,HBITMAP m_Ddown2)
{
	m_down1=m_Down1;
	m_down2=m_Ddown2;
}
/************************************************************************/
/* function:��׽����ƶ��¼����ı�ͼƬ                                 */
/************************************************************************/
void CBmpButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ;
	CButton::OnMouseMove(nFlags,point);
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point))
	{
		SetCapture();
		SetBitmap(m_down1);
	} 
	else
	{
		ReleaseCapture();
		SetBitmap(m_down2);
	}
}
