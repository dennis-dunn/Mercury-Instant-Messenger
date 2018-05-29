#include "colorbutton.h"
#if !defined(AFX_SERVERDLG_H__4BA79C68_2BCE_4A23_8997_7A1DF3DA4E6C__INCLUDED_)
#define AFX_SERVERDLG_H__4BA79C68_2BCE_4A23_8997_7A1DF3DA4E6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

class CServerDlg : public CDialog
{
// Construction
public:
	CString GetServerIP();
	CStringArray m_strSaved;
	CStringArray m_strBanned;
	int m_iCensor;
	CServerDlg(CWnd* pParent = NULL);   // standard constructor
	COLORREF m_clrFore;		//common color to be used for all text controls
	COLORREF m_clrBG;		//common color to be used for BG of all controls AND text
	CBrush *m_brush;

// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER };
	CComboBox	m_ctlList;
	CSliderCtrl	m_ctlCensor;
	CString	m_strAdminName;
	BOOL	m_bAsk;
	int		m_iPort;
	int		m_iMax;
	CString	m_strPassword;
	CString	m_strRoomName;
	CString	m_strSMax;
	CString	m_strCompName;
	BOOL	m_bTimeStamp;
	CString	m_strBan;
	CString	m_strYourIP;
	CString	m_strModPass;
	BOOL	m_bBouncer;
	int		m_iBounceSec;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;

	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnOpentext();
	afx_msg void OnAddnew();
	afx_msg void OnRemove();
	afx_msg void OnRemoveall();
	afx_msg void OnBouncer();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bPublic;
	afx_msg void OnBnClickedOk();
	CColorButton m_ctlOpenText;
	CColorButton m_ctlOK;
	CColorButton m_ctlCancel;
	CColorButton m_ctlAddNew;
	CColorButton m_ctlRemove;
	CColorButton m_ctlRemoveAll;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__4BA79C68_2BCE_4A23_8997_7A1DF3DA4E6C__INCLUDED_)
