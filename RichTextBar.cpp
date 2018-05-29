// RichTextBar.cpp: implementation of the CRichTextBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIM.h"
#include "MIMDlg.h"
#include "RichTextBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define WM_UPDATECHARFORMAT WM_USER + 1

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRichTextBar::CRichTextBar()
{
}

CRichTextBar::~CRichTextBar()
{
}


BEGIN_MESSAGE_MAP(CRichTextBar, CToolBar)
	//{{AFX_MSG_MAP(CRichTextBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichTextBar message handlers

int CRichTextBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	
	// TODO: Add your specialized creation code here
	LoadToolBar(IDR_TEXTBAR);
	GetToolBarCtrl().ModifyStyle(0,TBSTYLE_FLAT);

	return 0;
}

BOOL CRichTextBar::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		NMHDR nm;
		nm.hwndFrom = m_hWnd;
		nm.idFrom = GetDlgCtrlID();
		nm.code = NM_RETURN;
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			// Send change notification
			/*if( m_cmbFontName.IsChild(GetFocus()) )
				OnSelectFontName();
			else if( m_cmbFontSize.IsChild(GetFocus()) )
				OnSelectFontSize();*/
			//Fall through
			break;
		case VK_ESCAPE:
			GetOwner()->SendMessage(WM_NOTIFY, nm.idFrom, (LPARAM)&nm);
			return TRUE;
		}
	}
	
	return CToolBar::PreTranslateMessage(pMsg);
}

/*
//Requires:
afx_msg BOOL OnEraseBkgnd(CDC* pDC);
ON_WM_ERASEBKGND()

BOOL CRichTextBar::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//AfxMessageBox("OnEraseBkgnd");
	
	// Set brush to desired background color
	CBrush backBrush(RGB(0, 0, 0));

	// Save old brush
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);

	CRect rect;
	pDC->GetClipBox(&rect);     // Erase the area needed

	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	return CToolBar::OnEraseBkgnd(pDC);
}
*/