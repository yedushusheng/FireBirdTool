// BmpButton.cpp : 实现文件
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



// CBmpButton 消息处理程序

void CBmpButton::SetHBitmap(HBITMAP m_Down1,HBITMAP m_Ddown2)
{
	m_down1=m_Down1;
	m_down2=m_Ddown2;
}
/************************************************************************/
/* function:捕捉鼠标移动事件，改变图片                                 */
/************************************************************************/
void CBmpButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值;
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
