// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MIM.h"
#include "ServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog


CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
	, m_bPublic(FALSE)
{
	//{{AFX_DATA_INIT(CServerDlg)
	m_strAdminName = _T("");
	m_bAsk = FALSE;
	m_iPort = 0;
	m_iMax = 0;
	m_strPassword = _T("");
	m_strRoomName = _T("");
	m_strSMax = _T("");
	m_strCompName = _T("");
	m_bTimeStamp = FALSE;
	m_strBan = _T("");
	m_strYourIP = _T("");
	m_strModPass = _T("");
	m_bBouncer = FALSE;
	m_iBounceSec = 0;
	//}}AFX_DATA_INIT
}


void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
	DDX_Control(pDX, IDC_BANNED, m_ctlList);
	DDX_Control(pDX, IDC_CENSOR, m_ctlCensor);
	DDX_Text(pDX, IDC_ADMINNAME, m_strAdminName);
	DDV_MaxChars(pDX, m_strAdminName, 29);
	DDX_Check(pDX, IDC_ASK, m_bAsk);
	DDX_Text(pDX, IDC_LISTENPORT, m_iPort);
	DDV_MinMaxInt(pDX, m_iPort, 0, 60000);
	DDX_Text(pDX, IDC_MAX, m_iMax);
	DDV_MinMaxInt(pDX, m_iMax, 1, 20);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 15);
	DDX_Text(pDX, IDC_ROOMNAME, m_strRoomName);
	DDV_MaxChars(pDX, m_strRoomName, 30);
	DDX_Text(pDX, IDC_SMAX, m_strSMax);
	DDX_Text(pDX, IDC_COMPNAME, m_strCompName);
	DDX_Check(pDX, IDC_TIMESTAMP, m_bTimeStamp);
	DDX_CBString(pDX, IDC_BANNED, m_strBan);
	DDX_Text(pDX, IDC_YOURIP, m_strYourIP);
	DDX_Text(pDX, IDC_MODPASS, m_strModPass);
	DDV_MaxChars(pDX, m_strModPass, 15);
	DDX_Check(pDX, IDC_BOUNCER, m_bBouncer);
	DDX_Text(pDX, IDC_BSECONDS, m_iBounceSec);
	DDV_MinMaxInt(pDX, m_iBounceSec, 20, 10000);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_PUBLIC, m_bPublic);
	DDX_Control(pDX, IDC_OPENTEXT, m_ctlOpenText);
	DDX_Control(pDX, IDOK, m_ctlOK);
	DDX_Control(pDX, IDCANCEL, m_ctlCancel);
	DDX_Control(pDX, IDC_ADDNEW, m_ctlAddNew);
	DDX_Control(pDX, IDC_REMOVE, m_ctlRemove);
	DDX_Control(pDX, IDC_REMOVEALL, m_ctlRemoveAll);
}


BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	//{{AFX_MSG_MAP(CServerDlg)
	ON_BN_CLICKED(IDC_OPENTEXT, OnOpentext)
	ON_BN_CLICKED(IDC_ADDNEW, OnAddnew)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_REMOVEALL, OnRemoveall)
	ON_BN_CLICKED(IDC_BOUNCER, OnBouncer)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CServerDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog() 
{
	SetMIMColor(&m_ctlOpenText);
	SetMIMColor(&m_ctlOK);
	SetMIMColor(&m_ctlCancel);
	SetMIMColor(&m_ctlAddNew);
	SetMIMColor(&m_ctlRemove);
	SetMIMColor(&m_ctlRemoveAll);

	CDialog::OnInitDialog();
	m_tooltip.Create (this);
	m_tooltip.Activate (TRUE);
	m_tooltip.AddTool (GetDlgItem (IDC_ROOMNAME), "Name of the chatroom /ROOMNAME");
	m_tooltip.AddTool (GetDlgItem (IDC_ADMINNAME), "Name shown for admin /ADMINNAME");
	m_tooltip.AddTool (GetDlgItem (IDC_YOURIP), "Your computer's IP address /MYIP");
	m_tooltip.AddTool (GetDlgItem (IDC_LISTENPORT), "Port that server will listen on /LISTENPORT");
	m_tooltip.AddTool (GetDlgItem (IDC_PASSWORD), "Required password to enter server /PASSWORD");
	m_tooltip.AddTool (GetDlgItem (IDC_MODPASS), "Password for users to become moderators /MODPASS");
	m_tooltip.AddTool (GetDlgItem (IDC_ASK), "Admin approval required before client can join /SCREEN");
	m_tooltip.AddTool (GetDlgItem (IDC_TIMESTAMP), "Show/hide timestamps on messages /TIMESTAMP");
	m_tooltip.AddTool (GetDlgItem (IDC_OPENTEXT), "Edit custom text displayed to all users entering chatroom");
	m_tooltip.AddTool (GetDlgItem (IDC_BANNED), "List of banned IP's. These clients cannot join the chatroom /ADDBAN, CLEARBAN");
	m_tooltip.AddTool (GetDlgItem (IDC_MAX), "Max number of users allowed in the chatroom (not including admin) /MAX");

	//Show max users next to edit box
	m_strSMax.Format("(1-%i)", ID_MAX);

	m_strYourIP = GetServerIP();
	m_ctlCensor.SetRange(0,3);
	m_ctlCensor.SetPos(m_iCensor);
	//Populate banned IP list
	m_ctlList.ResetContent();
	for(int i = 0;i < m_strSaved.GetSize();i++)
		m_ctlList.AddString(m_strSaved.GetAt(i));

	UpdateData(FALSE);
	OnBouncer();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CServerDlg::OnOK() 
{
	m_iCensor = m_ctlCensor.GetPos();

	int top = m_ctlList.GetCount();//# of saved addresses
	m_strSaved.RemoveAll();
	CString strTemp;
	//Copy from list to saved string array.
	for(int i = 0;i < top;i++)
	{
		m_ctlList.GetLBText(i, strTemp);
		m_strSaved.Add(strTemp);
	}
	CDialog::OnOK();
}

void CServerDlg::OnOpentext() 
{
	if(WinExec("Notepad.exe \"Server.txt\"", SW_SHOW) < 32)
		MessageBox("<!> Could not execute program: Notepad.exe \"Server.txt\"");		
}

void CServerDlg::OnAddnew() 
{
	//Adds the IP address in the edit box to the list.

	CString strSearch;
	UpdateData(TRUE);
	if(m_strBan != "")//make sure an address is entered
	{
		strSearch = m_strBan;
		int temp = m_ctlList.FindStringExact(0, strSearch);
		if((temp >= 0) && (temp < m_ctlList.GetCount()))
		{
			MessageBox("This IP address already exists.", "Duplicate IP address found", MB_OK | MB_ICONINFORMATION);
			return;
		}
		/*if(m_ctlList.GetCount() < 30)
		{*/
			m_ctlList.AddString(m_strBan);
			//m_strIP = "";
		/*}
		else
		{
			MessageBox("You can't have more than 30 saved addresses.");
			GetDlgItem(IDC_ADDNEW)->EnableWindow(FALSE);
		}*/
	}
	else
		MessageBox("You must first enter an IP address to ban.", "Add IP address", MB_OK | MB_ICONINFORMATION);

	UpdateData(FALSE);
}

void CServerDlg::OnRemove() 
{
	//Removes the selected IP address from the list.
	CString strRemove;
	int i;
	UpdateData(TRUE);
	if(m_strBan != "")
	{
		i = m_ctlList.GetCurSel();
		m_ctlList.DeleteString(i);
		m_strBan = "";
		GetDlgItem(IDC_ADDNEW)->EnableWindow(TRUE);
	}
	else
		MessageBox("You must select a banned IP address first.", "Remove IP address", MB_OK | MB_ICONINFORMATION);
	UpdateData(FALSE);
}

void CServerDlg::OnRemoveall() 
{
	//Removes all IP addresses from the list
	if(MessageBox("Are you sure you want to remove all banned IP addresses?", "Remove all addresses?", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
	{
		m_ctlList.ResetContent();
		UpdateData(FALSE);
	}
}

CString CServerDlg::GetServerIP()
{
	CString strTmp;
	char szHostName[200];
	int iErr = gethostname(szHostName,sizeof(szHostName));
	if(iErr != 0)
	{
		iErr = WSAGetLastError();
		if(iErr == WSAEFAULT)
			strTmp = "WSAEFAULT";
		else
			strTmp.Format("Error: %i",iErr);
		MessageBox(strTmp);
		return strTmp;
	}
	m_strCompName.Format("Comp. Name:  %s", szHostName);
	LPHOSTENT pHost;
	pHost = gethostbyname(szHostName);

	struct in_addr* ptr = (struct in_addr*)pHost->h_addr_list[0];
	int a = ptr->S_un.S_un_b.s_b1;
	int b = ptr->S_un.S_un_b.s_b2;
	int c = ptr->S_un.S_un_b.s_b3;
	int d = ptr->S_un.S_un_b.s_b4;
	
	//strTmp = pHost->h_addr_list[0];
	strTmp.Format("%d.%d.%d.%d",a,b,c,d);
	return strTmp;
}

BOOL CServerDlg::PreTranslateMessage(MSG* pMsg) 
{
	m_tooltip.RelayEvent(pMsg);
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CServerDlg::OnBouncer() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_BSTATIC)->EnableWindow(m_bBouncer);
	GetDlgItem(IDC_BSTATIC2)->EnableWindow(m_bBouncer);
	GetDlgItem(IDC_BSECONDS)->EnableWindow(m_bBouncer);
}

HBRUSH CServerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

    return (HBRUSH)*m_brush; 	// TODO: Return a different brush if the default is not desired
}

void CServerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
