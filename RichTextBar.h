// RichTextBar.h: interface for the CRichTextBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHTEXTBAR_H__AFE23C75_B54A_45A5_AFEF_E7BFD6F5FE6D__INCLUDED_)
#define AFX_RICHTEXTBAR_H__AFE23C75_B54A_45A5_AFEF_E7BFD6F5FE6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FlatComboBox.h"

class CRichTextBar : public CToolBar
{
// Construction
public:
	CRichTextBar();

// Attributes
public:
// Operations
public:
static int CALLBACK EnumFontFamProc(ENUMLOGFONT *lpelf, NEWTEXTMETRIC *lpntm,
					int nFontType, LPARAM lParam);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichTextBar)
	//}}AFX_VIRTUAL
	virtual BOOL PreTranslateMessage(MSG* pMsg);
// Implementation
public:
	virtual ~CRichTextBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRichTextBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHTEXTBAR_H__AFE23C75_B54A_45A5_AFEF_E7BFD6F5FE6D__INCLUDED_)
