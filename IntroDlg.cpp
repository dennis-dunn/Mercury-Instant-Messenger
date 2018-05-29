// IntroDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MIM.h"
#include "IntroDlg.h"
#include "mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIntroDlg dialog


CIntroDlg::CIntroDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIntroDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIntroDlg)
	m_strVer = _T("");
	//}}AFX_DATA_INIT
}


void CIntroDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIntroDlg)
	DDX_Text(pDX, IDC_VERSION, m_strVer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIntroDlg, CDialog)
	//{{AFX_MSG_MAP(CIntroDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIntroDlg message handlers

BOOL CIntroDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_strVer.Format("MIM v%.2f", dVer);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIntroDlg::Init()
{
	sndPlaySound("MIMintro.wav", SND_ASYNC);//SND_SYNC will not return until sound finishes
	Sleep(2000);
	OnCancel();
}
