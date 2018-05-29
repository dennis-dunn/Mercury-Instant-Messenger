// MyEditIn.cpp: implementation of the CMyEditIn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIM.h"
#include "MIMDlg.h"
#include "MyEditIn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CInputWin

CMyEditIn::CMyEditIn(CMIMDlg* pDlg)
{	
	m_pDlg = pDlg;
	Brush.CreateSolidBrush(RGB(255, 255, 255));
//	Font.CreateFont(15, 15, 0, 0, FW_BOLD, FALSE, FALSE,0,0,0,0,0,0, "Verdana");
}

CMyEditIn::CMyEditIn()
{	
	m_pDlg = NULL;
	Brush.CreateSolidBrush(RGB(255, 255, 255));
//	Font.CreateFont(15, 15, 0, 0, FW_BOLD, FALSE, FALSE,0,0,0,0,0,0, "Verdana");
}

CMyEditIn::~CMyEditIn()
{
}


BEGIN_MESSAGE_MAP(CMyEditIn, CRichEditCtrl)
	//{{AFX_MSG_MAP(CInputWin)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputWin message handlers

void CMyEditIn::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//Check for keys being pressed, if return send message
	if(nChar == '`')
		m_pDlg->Prev();//press previous button
	else if(nChar != VK_RETURN)
	{
		CRichEditCtrl::OnChar(nChar,nRepCnt,nFlags);
	}
	else
		m_pDlg->OnBtnSend2();
}

void CMyEditIn::DefaultCharFormat()
{
	CHARFORMAT cfm;
	cfm.cbSize = sizeof(CHARFORMAT);
	cfm.dwMask = CFM_FACE | CFM_SIZE | CFM_BOLD |
		CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT | CFM_PROTECTED;
	cfm.dwEffects = 0;
	cfm.yHeight = 480;
	::lstrcpy(cfm.szFaceName, "Times New Roman"); 
	SetWordCharFormat(cfm);
}
