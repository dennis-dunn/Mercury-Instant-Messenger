#if !defined(AFX_FILESHAREDLG_H__294CB0A0_8569_4165_85B1_693477713DCC__INCLUDED_)
#define AFX_FILESHAREDLG_H__294CB0A0_8569_4165_85B1_693477713DCC__INCLUDED_

#include "MyFileSocket.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileShareDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileShareDlg dialog

class CFileShareDlg : public CDialog
{
// Construction
public:
	int FileDone();
	void OnBSend();
	double RecFile(CString strFile, double dSize);
	CString m_strDLto;
	void OnBReceive();
	void OnConnect();
	void OnAccept();
	void OnListen();
	void Cancel();
	void StartTimer();
	int m_iIdleCount;
	double m_dRate;
	int m_iTriggerStep;
	BOOL m_bConOpen;
	BOOL m_bAllowRec;
	BOOL m_bAllowSend;
	void Reset();
	void Finalize();
	void OnClose(int ID);
	int m_iPort;
	BOOL m_bSender;
	BOOL m_bReady;
	int m_iType;
	CString m_strIP;
	void OnBCall();
	BOOL m_bRec;
	CTime m_Elapsed;
	void UpDisplay();
	CString m_strFileName;
	int m_iRange;
	CString m_strMess;
	CString m_strFinalPath;
	CString m_strPath;
	CStringArray m_strFileDirs;
	CStringArray m_strFileNames;
	BOOL m_bConnected;
	int m_iStep;
	int m_iCurRate;
	double m_dRateCount;
	void StepRate();
	int SendFile();
	int m_iCurFile;
	double m_dTotal;
	CFileShareDlg(CWnd* pParent = NULL);   // standard constructor

	CMyFileSocket *m_pFClientSocket;
	CMyFileSocket *m_pFListenSocket;
	CSocketFile *m_pFSocketFile;

// Dialog Data
	//{{AFX_DATA(CFileShareDlg)
	enum { IDD = IDD_SENDFILE };
	CProgressCtrl	m_ctlProg;
	CProgressCtrl	m_ctlProg2;
	CString	m_strPercent;
	CString	m_strDisplayPath;
	CString	m_strCurFile;
	CString	m_strPercent2;
	CString	m_strElapsed;
	CString	m_strStatus;
	CString	m_strRate;
	CString	m_strOtherName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileShareDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileShareDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCancelTrans();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILESHAREDLG_H__294CB0A0_8569_4165_85B1_693477713DCC__INCLUDED_)
