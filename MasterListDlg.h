#pragma once
#include "iconlistctrl.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "ServerEntry.h"
#include "addserverdlg.h"

// CMasterListDlg dialog

class CMasterListDlg : public CDialog
{
	DECLARE_DYNAMIC(CMasterListDlg)

public:
	CMasterListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMasterListDlg();
// Dialog Data
	enum { IDD = IDD_MASTERLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLvnItemchangedMasterlist(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog(void);
	COLORREF m_clrBack;// color of BG
	COLORREF m_clrChar;// color of text
	void SetMIMParent(CDialog *pWnd);
	// ClistCtrl version of the list
	void RemoveAllServers();
	void RemoveAllMasterServers();//clears the MSL data (doesnt edit the file)
	CListCtrl m_ctlMList2;
	CImageList m_imgMasterList;
	CImageList m_imgMasterState;
	CObArray m_oaServers;			//list of all servers in server list
	CStringArray m_strMSL_IPs;	//list of all IP's the master list contains
	CWordArray m_iMSL_Ports;		//list of all corresponding ports that master list contains
	CStringArray m_strPSL_IPs;		//list of all IP's the public list contains
	CWordArray m_iPSL_Ports;		//list of all corresponding ports that master list contains
	CServerEntry * GetEntry(int index);
	int m_iViewMode;
	//int AddServer(CString strName, int iPass = 0, double dVer = 0, BOOL clear = TRUE, BOOL bAbort = FALSE);
	int AddServer(CServerEntry *oItem, BOOL clear = TRUE, BOOL bAbort = FALSE);//note the pointer is reset() after copied into the obArray of clear = TRUE
	void UpdateList(int index = -1);
	afx_msg void OnBnClickedAdd();
	CAddServerDlg m_AddServerDlg;
	CDialog *m_pWnd;//Parent Window CMIMDlg
	afx_msg void OnBnClickedRefresh();
	// first time opening the dialog
	BOOL m_bFirst;
	void DisableButtons(BOOL state = TRUE);
	afx_msg void OnBnClickedRemove();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedView();
};
