#pragma once


// CToolBoxDlg dialog

class CToolBoxDlg : public CDialog
{
	DECLARE_DYNAMIC(CToolBoxDlg)

public:
	CDialog *m_pWnd;
	CToolBoxDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CToolBoxDlg();

// Dialog Data
	enum { IDD = IDD_TOOLBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bFirst;// if this is the first time opening the window (being created)
};
