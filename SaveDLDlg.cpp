// SaveDLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MIM.h"
#include "SaveDLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveDLDlg dialog


CSaveDLDlg::CSaveDLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveDLDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveDLDlg)
	m_strText = _T("");
	//}}AFX_DATA_INIT
}


void CSaveDLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveDLDlg)
	DDX_Text(pDX, IDC_TEXT, m_strText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveDLDlg, CDialog)
	//{{AFX_MSG_MAP(CSaveDLDlg)
	ON_BN_CLICKED(IDYES, OnYes)
	ON_BN_CLICKED(IDSAVEAS, OnSaveas)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveDLDlg message handlers

void CSaveDLDlg::OnYes() 
{
	OnOK();
}

BOOL CSaveDLDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_bSaveAs = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSaveDLDlg::OnSaveas() 
{
	m_bSaveAs = TRUE;
	OnOK();
}

void CSaveDLDlg::OnCancel() 
{	
	CDialog::OnCancel();
}
