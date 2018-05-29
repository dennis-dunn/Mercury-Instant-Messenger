// ReferenceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MIM.h"
#include "ReferenceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReferenceDlg dialog


CReferenceDlg::CReferenceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReferenceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReferenceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CReferenceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReferenceDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReferenceDlg, CDialog)
	//{{AFX_MSG_MAP(CReferenceDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReferenceDlg message handlers
