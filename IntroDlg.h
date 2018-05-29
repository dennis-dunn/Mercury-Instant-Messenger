#if !defined(AFX_INTRODLG_H__16A61B5B_AF51_42EC_832E_E16E86B45735__INCLUDED_)
#define AFX_INTRODLG_H__16A61B5B_AF51_42EC_832E_E16E86B45735__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IntroDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIntroDlg dialog

class CIntroDlg : public CDialog
{
// Construction
public:
	double dVer;
	void Init();
	CIntroDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIntroDlg)
	enum { IDD = IDD_INTRO };
	CString	m_strVer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIntroDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIntroDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTRODLG_H__16A61B5B_AF51_42EC_832E_E16E86B45735__INCLUDED_)
