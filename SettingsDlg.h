#include "colorbutton.h"
#if !defined(AFX_SETTINGSDLG_H__8C40A4F0_E6BB_49BA_BBD1_F77682929FAD__INCLUDED_)
#define AFX_SETTINGSDLG_H__8C40A4F0_E6BB_49BA_BBD1_F77682929FAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg dialog

class CSettingsDlg : public CDialog
{
// Construction
public:
	BOOL m_bGoToServer;
	CSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_strAniExt;	//Lower case
	CString m_strSoundExt;	//Lower case
	CStringArray m_strSaved;
	CStringArray m_strComments;
	COLORREF m_clrFore;		//common color to be used for all text controls
	COLORREF m_clrBG;		//common color to be used for BG of all controls AND text
	CBrush *m_brush;

// Dialog Data
	//{{AFX_DATA(CSettingsDlg)
	enum { IDD = IDD_SETTINGS };
	CListCtrl	m_ctlList;
	CStatic	m_ctlSport;
	int		m_iPort;
	CString	m_strScreenName;
	BOOL	m_bSound;
	CString	m_strSoundDir;
	int		m_iType;
	BOOL	m_bAni;
	CString	m_strAniDir;
	BOOL	m_bAllowRun;
	CString	m_strBrowser;
	BOOL	m_bAutoSaveMess;
	BOOL	m_bListenAble;
	BOOL	m_bAutoLogin;
	int		m_iSaveMessType;
	CString	m_strIP;
	CString	m_strComment;
	int		m_iIdleTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingsDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;

	// Generated message map functions
	//{{AFX_MSG(CSettingsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSound();
	afx_msg void OnAni();
	afx_msg void OnSbrowse();
	afx_msg void OnAbrowse();
	afx_msg void OnAddnew();
	afx_msg void OnRemoveall();
	afx_msg void OnRemove();
	afx_msg void OnType0();
	afx_msg void OnType1();
	afx_msg void OnBbrowse();
	afx_msg void OnClickIplist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeComment();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CColorButton m_ctlAddNew;
	CColorButton m_ctlRemove;
	CColorButton m_ctlRemoveAll;
	CColorButton m_ctlOK;
	CColorButton m_ctlCancel;
	CColorButton m_ctlBrowse;
	CColorButton m_ctlAbrowse;
	CColorButton m_ctlBBrowse;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGSDLG_H__8C40A4F0_E6BB_49BA_BBD1_F77682929FAD__INCLUDED_)
