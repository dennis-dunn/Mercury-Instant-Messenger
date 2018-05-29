// MyEdit.h: interface for the CMyEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYEDIT_H__C9ADD245_6D95_4AAA_94A0_888959653C14__INCLUDED_)
#define AFX_MYEDIT_H__C9ADD245_6D95_4AAA_94A0_888959653C14__INCLUDED_

#include "Pkg.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMIMDlg;


class CMyEdit : public CRichEditCtrl
{
// Construction
public:
	CMyEdit();

// Attributes
public:
	CBrush Brush;
	CFont Font;
	//CMIMDlg* m_pDlg;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyEdit)
	//}}AFX_VIRTUAL
private:
	void Message(CHARFORMAT& cf);

// Implementation
public:
	int ClearAllText();
	CWordArray wHyperlinkS;
	CWordArray wHyperlinkE;
	void Hyperlinks(CHARFORMAT& cf);
	long m_nStart, m_nEnd;	//memorize user selection
	int m_iNewLine, m_iOldLine,	m_iTotalLength,	m_iStartPos, m_iEndPos;

	void ShowMessage(CPkg& pkg, BOOL bRet = TRUE);
	CHARFORMAT ShowMessage(CString strText, COLORREF clrText = RGB(164,176,238), BOOL bRet = TRUE);

	virtual ~CMyEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShowWin)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg HBRUSH CtlColor(CDC *pDC,UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_MYEDIT_H__C9ADD245_6D95_4AAA_94A0_888959653C14__INCLUDED_)
