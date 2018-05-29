// MyEditIn.h: interface for the CMyEditIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYEDITIN_H__3ADBE36E_CC23_4D67_B7CC_7CC8C7F1FCA5__INCLUDED_)
#define AFX_MYEDITIN_H__3ADBE36E_CC23_4D67_B7CC_7CC8C7F1FCA5__INCLUDED_

#include "MIMDlg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyEditIn : public CRichEditCtrl
{
// Construction
public:
	CMyEditIn(CMIMDlg* pDlg);
	CMyEditIn();

// Attributes
public:

// Operations
public:
	CBrush Brush;
	CFont Font;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputWin)
	//}}AFX_VIRTUAL

// Implementation
public:
	void DefaultCharFormat();
	CMIMDlg* m_pDlg;
	virtual ~CMyEditIn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInputWin)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_MYEDITIN_H__3ADBE36E_CC23_4D67_B7CC_7CC8C7F1FCA5__INCLUDED_)
