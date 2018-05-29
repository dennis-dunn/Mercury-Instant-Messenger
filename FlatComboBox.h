// FlatComboBox.h: interface for the CFlatComboBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLATCOMBOBOX_H__97B0B217_7C61_4931_94F9_1B0695FBD734__INCLUDED_)
#define AFX_FLATCOMBOBOX_H__97B0B217_7C61_4931_94F9_1B0695FBD734__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFlatComboBox : public CComboBox
{
// Construction
public:
	CFlatComboBox();

// Attributes
public:

// Operations
public:

protected:

	int			m_nOffset;	
	BOOL		m_bLBtnDown;
	BOOL		m_bPainted;
	BOOL		m_bHasFocus;
	COLORREF	m_clrBtnHilite;
	COLORREF	m_clrBtnShadow;
	COLORREF	m_clrBtnFace;

	enum STATE { normal = 1, raised = 2, pressed = 3, readOnly = 4 };

	void DrawCombo(STATE eState, COLORREF clrTopLeft, COLORREF clrBottomRight);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlatComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFlatComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFlatComboBox)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus();
	afx_msg void OnKillFocus();
	afx_msg void OnSysColorChange();
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_FLATCOMBOBOX_H__97B0B217_7C61_4931_94F9_1B0695FBD734__INCLUDED_)
