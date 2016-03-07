#pragma once


// CBmpButton

class CBmpButton : public CButton
{
	DECLARE_DYNAMIC(CBmpButton)

public:
	HBITMAP m_down1;
	HBITMAP m_down2;
	HBITMAP m_upFile1;
	HBITMAP m_upFile2;
	HBITMAP m_chat1;
	HBITMAP m_chat2;
	HBITMAP m_config1;
	HBITMAP m_config2;
	HBITMAP m_userlogin1;
	HBITMAP m_userlogin2;
	CBmpButton();
	virtual ~CBmpButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void SetHBitmap(HBITMAP m_down1,HBITMAP m_down2);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


