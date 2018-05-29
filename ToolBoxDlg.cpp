// ToolBoxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MIM.h"
#include "ToolBoxDlg.h"


// CToolBoxDlg dialog

IMPLEMENT_DYNAMIC(CToolBoxDlg, CDialog)

CToolBoxDlg::CToolBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolBoxDlg::IDD, pParent)
	, m_bFirst(TRUE)
{

}

CToolBoxDlg::~CToolBoxDlg()
{
}

void CToolBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CToolBoxDlg, CDialog)
END_MESSAGE_MAP()


// CToolBoxDlg message handlers
