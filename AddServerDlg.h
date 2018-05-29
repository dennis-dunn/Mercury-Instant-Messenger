#pragma once


// CAddServerDlg dialog

class CAddServerDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddServerDlg)

public:
	CAddServerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddServerDlg();
	BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_ADDSERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strIP;
	int m_iPort;
	afx_msg void OnBnClickedOk();
};
