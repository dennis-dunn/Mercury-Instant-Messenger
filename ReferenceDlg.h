#if !defined(AFX_REFERENCEDLG_H__EC9B3EEF_5936_4FA8_BE51_99E998332ABA__INCLUDED_)
#define AFX_REFERENCEDLG_H__EC9B3EEF_5936_4FA8_BE51_99E998332ABA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReferenceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReferenceDlg dialog

class CReferenceDlg : public CDialog
{
// Construction
public:
	CReferenceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReferenceDlg)
	enum { IDD = IDD_HELP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReferenceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReferenceDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REFERENCEDLG_H__EC9B3EEF_5936_4FA8_BE51_99E998332ABA__INCLUDED_)
