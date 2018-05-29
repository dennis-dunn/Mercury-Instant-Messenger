// AddServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MIM.h"
#include "AddServerDlg.h"


// CAddServerDlg dialog

IMPLEMENT_DYNAMIC(CAddServerDlg, CDialog)

BOOL CAddServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_iPort = 4001;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

CAddServerDlg::CAddServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddServerDlg::IDD, pParent)
	, m_strIP(_T(""))
	, m_iPort(0)
{

}

CAddServerDlg::~CAddServerDlg()
{
}

void CAddServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IP, m_strIP);
	DDX_Text(pDX, IDC_PORT, m_iPort);
	DDV_MinMaxInt(pDX, m_iPort, 0, INT_MAX);
}


BEGIN_MESSAGE_MAP(CAddServerDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAddServerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddServerDlg message handlers

void CAddServerDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	OnOK();
}
