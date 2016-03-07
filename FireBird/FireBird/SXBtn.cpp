// SXBtn.cpp : 实现文件
//

#include "stdafx.h"
#include "FireBird.h"
#include "SXBtn.h"


// CSXBtn

IMPLEMENT_DYNAMIC(CSXBtn, CButton)

CSXBtn::CSXBtn()
{

}

CSXBtn::~CSXBtn()
{
}


BEGIN_MESSAGE_MAP(CSXBtn, CButton)
END_MESSAGE_MAP()



// CSXBtn 消息处理程序




//void CSXBtn::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
//{
//
//	/*UINT uStyle = BS_DEFPUSHBUTTON ;//DFCS_BUTTONPUSH;
//
//	// This code only works with buttons.
//	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);
//
//	// If drawing selected, add the pushed style to DrawFrameControl.
//	if (lpDrawItemStruct->itemState & ODS_SELECTED)
//		uStyle |= DFCS_PUSHED;
//
//	// Draw the button frame.
//	::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem,
//		DFC_BUTTON, uStyle);
//
//	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
//
//	// Get the button's text.
//	CString strText;
//	GetWindowText(strText);
//
//	// Draw the button text using the text color red.
//	CBrush B;
//	CRect rect;
//	CRect focusRect;
//	focusRect.CopyRect(&lpDrawItemStruct->rcItem);
//	DrawFocusRect(lpDrawItemStruct->hDC, (LPRECT)&focusRect);
//	focusRect.left += 4;
//	focusRect.right -= 4;
//	focusRect.top += 4;
//	focusRect.bottom -= 4;
//
//	rect.CopyRect(&lpDrawItemStruct->rcItem);
//	pDC->Draw3dRect(rect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
//	B.CreateSolidBrush(RGB(200,230,190));
//	::FillRect(lpDrawItemStruct->hDC,&rect, (HBRUSH)B.m_hObject);
//	::SetBkMode(lpDrawItemStruct->hDC,TRANSPARENT);
//	COLORREF crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, RGB(0,0,0));
//	::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(),
//		&lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
//	::SetTextColor(lpDrawItemStruct->hDC, crOldColor);*/
//
//}
