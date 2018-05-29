#if !defined(AFX_AGREEDLG_H__2A3D757A_B16D_4770_8267_BE26987FAC6A__INCLUDED_)
#define AFX_AGREEDLG_H__2A3D757A_B16D_4770_8267_BE26987FAC6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AgreeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAgreeDlg dialog

class CAgreeDlg : public CDialog
{
// Construction
public:
	CString m_strRel;
	double m_dVer;
	CAgreeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAgreeDlg)
	enum { IDD = IDD_AGREE };
	CString	m_strText;
	CString	m_strDate;
	CString	m_strVer;
	CString	m_strCopyright;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAgreeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAgreeDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AGREEDLG_H__2A3D757A_B16D_4770_8267_BE26987FAC6A__INCLUDED_)
