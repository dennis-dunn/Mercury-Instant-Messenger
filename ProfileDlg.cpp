// ProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MIM.h"
#include "ProfileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProfileDlg dialog


CProfileDlg::CProfileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProfileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProfileDlg)
	m_strName = _T("");
	m_strPets = _T("");
	m_strQuote = _T("");
	m_iGender = -1;
	m_strEmail = _T("");
	m_iAge = 0;
	m_strLive = _T("");
	//}}AFX_DATA_INIT
}

void CProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfileDlg)
	DDX_Control(pDX, IDC_QUOTE, m_ctlQuote);
	DDX_Control(pDX, IDC_EMAIL, m_ctlEmail);
	DDX_Control(pDX, IDC_PETS, m_ctlPets);
	DDX_Control(pDX, IDC_LIVE, m_ctlLive);
	DDX_Control(pDX, IDC_NAME, m_ctlName);
	DDX_Control(pDX, IDC_AGE, m_ctlAge);
	DDX_Control(pDX, IDC_BIT, m_ctlBit);
	DDX_Control(pDX, IDC_ANIMATE, m_ctlAnim);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 39);
	DDX_Text(pDX, IDC_PETS, m_strPets);
	DDV_MaxChars(pDX, m_strPets, 39);
	DDX_Text(pDX, IDC_QUOTE, m_strQuote);
	DDV_MaxChars(pDX, m_strQuote, 399);
	DDX_Radio(pDX, IDC_GENDER, m_iGender);
	DDX_Text(pDX, IDC_EMAIL, m_strEmail);
	DDV_MaxChars(pDX, m_strEmail, 39);
	DDX_Text(pDX, IDC_AGE, m_iAge);
	DDV_MinMaxInt(pDX, m_iAge, 1, 130);
	DDX_Text(pDX, IDC_LIVE, m_strLive);
	DDV_MaxChars(pDX, m_strLive, 39);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_ctlOK);
	DDX_Control(pDX, IDCANCEL, m_ctlCancel);
}


BEGIN_MESSAGE_MAP(CProfileDlg, CDialog)
	//{{AFX_MSG_MAP(CProfileDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProfileDlg message handlers

BOOL CProfileDlg::OnInitDialog() 
{
	SetMIMColor(&m_ctlOK);
	SetMIMColor(&m_ctlCancel);
	CDialog::OnInitDialog();
	
	CBitmap Bit;
	CString strImageDir = m_strImageDir;

	if(m_bAni)
	{
		if(strImageDir.Right(4) == ".bmp")
		{
			m_ctlBit.ShowWindow(TRUE);
			m_ctlAnim.ShowWindow(FALSE);
			HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_strImageDir, IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			Bit.Attach(hBitmap);
			m_ctlBit.SetBitmap(Bit);
			Bit.Detach();
		}
		else if(strImageDir.Right(4) == ".avi")
		{
			m_ctlAnim.ShowWindow(TRUE);
			m_ctlBit.ShowWindow(FALSE);
				
			m_ctlAnim.Open(m_strImageDir);
			m_ctlAnim.Play(0, -1, -1);
		}
	}

	if(m_bView)//Allow editing
	{
		SetWindowText("Edit Profile");
		GetDlgItem(IDOK)->ShowWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDCANCEL)->SetWindowText("Cancel");
	}
	else//view another user's profile
	{
		SetWindowText("View Profile");
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->ShowWindow(FALSE);
		GetDlgItem(IDCANCEL)->SetWindowText("Done");
	}

	m_ctlAge.SetReadOnly(!m_bView);
	m_ctlName.SetReadOnly(!m_bView);
	m_ctlLive.SetReadOnly(!m_bView);
	m_ctlPets.SetReadOnly(!m_bView);
	m_ctlEmail.SetReadOnly(!m_bView);
	m_ctlQuote.SetReadOnly(!m_bView);
	GetDlgItem(IDC_GENDER)->EnableWindow(m_bView);
	GetDlgItem(IDC_GENDER2)->EnableWindow(m_bView);
	GetDlgItem(IDC_INFO)->ShowWindow(m_bView);

	UpdateData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CProfileDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	CPoint pt(0,0);

	if (this != pWnd)
	{
		// 좌표 계산
		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		pt.x = -(rc.left + GetSystemMetrics(SM_CXDLGFRAME) - 1) % 55;
		pt.y = -(rc.top + GetSystemMetrics(SM_CYDLGFRAME) - 1)% 53;
	}
	pDC->SetTextColor(m_clrFore);//Set text color for all controls!!!
	pDC->SetBkColor(m_clrBG);//Set text BG color for all controls!!!
	// 브러시 원점 설정
	//m_brush->UnrealizeObject(); //not sure what this does
	pDC->SetBrushOrg(pt); 

    return (HBRUSH)*m_brush;
}
