// AgreeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MIM.h"
#include "AgreeDlg.h"
#include "MIMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAgreeDlg dialog


CAgreeDlg::CAgreeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAgreeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAgreeDlg)
	m_strText = _T("");
	m_strDate = _T("");
	m_strVer = _T("");
	m_strCopyright = _T("");
	//}}AFX_DATA_INIT
}


void CAgreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAgreeDlg)
	DDX_Text(pDX, IDC_TEXT, m_strText);
	DDX_Text(pDX, IDC_DATE, m_strDate);
	DDX_Text(pDX, IDC_VERSION, m_strVer);
	DDX_Text(pDX, IDC_COPYRIGHT, m_strCopyright);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAgreeDlg, CDialog)
	//{{AFX_MSG_MAP(CAgreeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAgreeDlg message handlers

BOOL CAgreeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_strVer.Format("Welcome to MIM Version: %.2f for %s", m_dVer, m_strRel);
	m_strDate.Format("Release Date: %s, %s",__DATE__,__TIME__);
	m_strCopyright.Format("Copyright (C) 2002-%i",CYEAR);

	CFile iniFile;	
	if(iniFile.Open("EULA.txt",CFile::modeNoTruncate|CFile::modeRead|CFile::typeText))
	{
		char ch;
		int iLen = (int)iniFile.GetLength();
		if(iLen == 0) return TRUE;

		CArchive* ar = new CArchive(&iniFile,CArchive::load);

		for(int i = 0;i < iLen;i++)
		{
			*ar >> ch;
			m_strText += ch;
		}
		delete ar;		
		iniFile.Close();
	}
	else
	{
		
		m_strText = "EULA.txt file is missing, to read the User Licencse agreement email me\nor run MIM with all of the files included in the same folder.";
		MessageBox(m_strText);
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
