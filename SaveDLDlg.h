#if !defined(AFX_SAVEDLDLG_H__5954C574_9A8F_461D_B162_CBBB5BCC30D1__INCLUDED_)
#define AFX_SAVEDLDLG_H__5954C574_9A8F_461D_B162_CBBB5BCC30D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaveDLDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSaveDLDlg dialog

class CSaveDLDlg : public CDialog
{
// Construction
public:
	BOOL m_bSaveAs;
	CSaveDLDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaveDLDlg)
	enum { IDD = IDD_SAVEDL };
	CString	m_strText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveDLDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSaveDLDlg)
	afx_msg void OnYes();
	virtual BOOL OnInitDialog();
	afx_msg void OnSaveas();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVEDLDLG_H__5954C574_9A8F_461D_B162_CBBB5BCC30D1__INCLUDED_)
