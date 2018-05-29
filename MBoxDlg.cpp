// MBoxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MIM.h"
#include "MBoxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMBoxDlg dialog


CMBoxDlg::CMBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMBoxDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMBoxDlg)
	m_strMess = _T("");
	//}}AFX_DATA_INIT
}


void CMBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMBoxDlg)
	DDX_Text(pDX, IDC_MESS, m_strMess);
	DDV_MaxChars(pDX, m_strMess, 300);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMBoxDlg, CDialog)
	//{{AFX_MSG_MAP(CMBoxDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMBoxDlg message handlers

void CMBoxDlg::OnOK() 
{
	UpdateData(TRUE);
	if(m_strMess != "")
		CDialog::OnOK();
	else
		CDialog::OnCancel();
}
