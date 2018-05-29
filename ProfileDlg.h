#include "colorbutton.h"
#if !defined(AFX_PROFILEDLG_H__247C994B_E088_4892_98C3_5ED27B84E427__INCLUDED_)
#define AFX_PROFILEDLG_H__247C994B_E088_4892_98C3_5ED27B84E427__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProfileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProfileDlg dialog

class CProfileDlg : public CDialog
{
// Construction
public:
	CString m_strImageDir;
	BOOL m_bAni;
	BOOL m_bView;
	CProfileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProfileDlg)
	enum { IDD = IDD_PROFILE };
	CEdit	m_ctlQuote;
	CEdit	m_ctlEmail;
	CEdit	m_ctlPets;
	CEdit	m_ctlLive;
	CEdit	m_ctlName;
	CEdit	m_ctlAge;
	CStatic	m_ctlBit;
	CAnimateCtrl	m_ctlAnim;
	CString	m_strName;
	CString	m_strPets;
	CString	m_strQuote;
	int		m_iGender;
	CString	m_strEmail;
	int		m_iAge;
	CString	m_strLive;
	CBrush *m_brush;
	COLORREF m_clrFore;		//common color to be used for all text controls
	COLORREF m_clrBG;		//common color to be used for BG of all controls AND text BG
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProfileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProfileDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CColorButton m_ctlOK;
	CColorButton m_ctlCancel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFILEDLG_H__247C994B_E088_4892_98C3_5ED27B84E427__INCLUDED_)
