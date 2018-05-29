// FlatComboBox.cpp: implementation of the CFlatComboBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIM.h"
#include "FlatComboBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFlatComboBox::CFlatComboBox()
{
	m_bLBtnDown	= FALSE;
	m_bPainted	= FALSE;
	m_bHasFocus	= FALSE;

	m_clrBtnHilite  = ::GetSysColor(COLOR_BTNHILIGHT);
	m_clrBtnShadow  = ::GetSysColor(COLOR_BTNSHADOW);
	m_clrBtnFace    = ::GetSysColor(COLOR_BTNFACE);
	m_nOffset		= ::GetSystemMetrics(SM_CXHTHUMB);
}

CFlatComboBox::~CFlatComboBox()
{
}


BEGIN_MESSAGE_MAP(CFlatComboBox, CComboBox)
	//{{AFX_MSG_MAP(CFlatComboBox)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(CBN_SETFOCUS,   OnSetFocus)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS,  OnKillFocus)
	ON_WM_SYSCOLORCHANGE()
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlatComboBox message handlers

void CFlatComboBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	SetTimer(1,100,NULL);
	OnTimer(1);
	CComboBox::OnMouseMove(nFlags, point);
}

void CFlatComboBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = TRUE;
	CComboBox::OnLButtonDown(nFlags, point);
}

void CFlatComboBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = FALSE;
	Invalidate();
	CComboBox::OnLButtonUp(nFlags, point);
}

void CFlatComboBox::OnTimer(UINT nIDEvent) 
{
	POINT pt;
	GetCursorPos(&pt);
	CRect rcItem;
	GetWindowRect(&rcItem);

	// OnLButtonDown, show pressed.
	if (m_bLBtnDown==TRUE) {
		KillTimer (1);
		if (m_bPainted == TRUE) {
			DrawCombo( pressed, m_clrBtnShadow, m_clrBtnHilite );
			m_bPainted = FALSE;
		}
		return;
	}

	// If mouse leaves, show flat.
	if (!rcItem.PtInRect(pt) && !m_bHasFocus) {
		KillTimer (1);
		if (m_bPainted == TRUE) {
			DrawCombo( normal, m_clrBtnFace, m_clrBtnFace );
			m_bPainted = FALSE;
		}
		return;
	}

	// On mouse over, show raised.
	else {
		if (m_bPainted == TRUE)
			return;
		else {
			m_bPainted = TRUE;
			DrawCombo( raised, m_clrBtnShadow, m_clrBtnHilite );
		}
	}
	
	CComboBox::OnTimer(nIDEvent);
}

void CFlatComboBox::OnPaint() 
{
	ModifyStyleEx (WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE,
		0, SWP_FRAMECHANGED);
	
	Default();

	CPoint pt;
	GetCursorPos(&pt);

	CRect rcItem;
	GetWindowRect(&rcItem);

	if ((rcItem.PtInRect(pt)) || m_bHasFocus)
		DrawCombo( raised, m_clrBtnShadow, m_clrBtnHilite );
	else
		DrawCombo( normal, m_clrBtnFace, m_clrBtnFace );
}

void CFlatComboBox::DrawCombo(STATE eState, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	CRect rcItem;
	GetClientRect(&rcItem);
	CDC* pDC = GetDC();
	
	// Cover up dark 3D shadow.
	pDC->Draw3dRect(rcItem, clrTopLeft, clrBottomRight);
	rcItem.DeflateRect(1,1);
	
	if (!IsWindowEnabled()) {
		pDC->Draw3dRect( rcItem, m_clrBtnHilite, m_clrBtnHilite );
	}
	
	else {
		pDC->Draw3dRect( rcItem, m_clrBtnFace, m_clrBtnFace );
	}

	// Cover up dark 3D shadow on drop arrow.
	rcItem.DeflateRect(1,1);
	rcItem.left = rcItem.right-m_nOffset;
	pDC->Draw3dRect( rcItem, m_clrBtnFace, m_clrBtnFace );
	
	// Cover up normal 3D shadow on drop arrow.
	rcItem.DeflateRect(1,1);
	pDC->Draw3dRect( rcItem, m_clrBtnFace, m_clrBtnFace );
	
	if (!IsWindowEnabled()) {
		return;
	}

	switch (eState)
	{
	case normal:
		rcItem.top -= 1;
		rcItem.bottom += 1;
		pDC->Draw3dRect( rcItem, m_clrBtnHilite, m_clrBtnHilite );
		rcItem.left -= 1;
		pDC->Draw3dRect( rcItem, m_clrBtnHilite, m_clrBtnHilite );
		break;

	case raised:
		rcItem.top -= 1;
		rcItem.bottom += 1;
		pDC->Draw3dRect( rcItem, m_clrBtnHilite, m_clrBtnShadow);
		break;

	case pressed:
		rcItem.top -= 1;
		rcItem.bottom += 1;
		rcItem.OffsetRect(1,1);
		pDC->Draw3dRect( rcItem, m_clrBtnShadow, m_clrBtnHilite );
		break;
	}

	ReleaseDC(pDC);
}

void CFlatComboBox::OnSetFocus()
{
	m_bHasFocus = TRUE;
	DrawCombo( raised, m_clrBtnShadow, m_clrBtnHilite );
}

void CFlatComboBox::OnKillFocus() 
{
	m_bHasFocus = FALSE;
	DrawCombo( readOnly, m_clrBtnFace, m_clrBtnFace );
}

BOOL CFlatComboBox::PreTranslateMessage(MSG* pMsg) 
{
	// Catch the Alt key so we don't choke if focus 
	// is going to an owner drawn button
	if (pMsg->message == WM_SYSCHAR)
		return TRUE;
		
	return CComboBox::PreTranslateMessage(pMsg);
}


void CFlatComboBox::OnSysColorChange() 
{
	CComboBox::OnSysColorChange();

	// re-initialize system colors.
	m_clrBtnHilite  = ::GetSysColor(COLOR_BTNHILIGHT);
	m_clrBtnShadow  = ::GetSysColor(COLOR_BTNSHADOW);
	m_clrBtnFace    = ::GetSysColor(COLOR_BTNFACE);
}
