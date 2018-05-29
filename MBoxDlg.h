#if !defined(AFX_MBOXDLG_H__B06EC2CF_C42C_40A9_AE19_52A139F2E825__INCLUDED_)
#define AFX_MBOXDLG_H__B06EC2CF_C42C_40A9_AE19_52A139F2E825__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MBoxDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMBoxDlg dialog

class CMBoxDlg : public CDialog
{
// Construction
public:
	CMBoxDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMBoxDlg)
	enum { IDD = IDD_MBOX };
	CString	m_strMess;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMBoxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMBoxDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MBOXDLG_H__B06EC2CF_C42C_40A9_AE19_52A139F2E825__INCLUDED_)
