// SettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MIM.h"
#include "SettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg dialog


CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsDlg)
	m_iPort = 0;
	m_strScreenName = _T("");
	m_bSound = FALSE;
	m_strSoundDir = _T("");
	m_iType = -1;
	m_bAni = FALSE;
	m_strAniDir = _T("");
	m_bAllowRun = FALSE;
	m_strBrowser = _T("");
	m_bAutoSaveMess = FALSE;
	m_bListenAble = FALSE;
	m_bAutoLogin = FALSE;
	m_iSaveMessType = -1;
	m_strIP = _T("");
	m_strComment = _T("");
	m_iIdleTime = 0;
	//}}AFX_DATA_INIT
}


void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsDlg)
	DDX_Control(pDX, IDC_IPLIST, m_ctlList);
	DDX_Control(pDX, IDC_SPORT, m_ctlSport);
	DDX_Text(pDX, IDC_PORT, m_iPort);
	DDX_Text(pDX, IDC_SCREENNAME, m_strScreenName);
	DDV_MaxChars(pDX, m_strScreenName, 29);
	DDX_Check(pDX, IDC_SOUND, m_bSound);
	DDX_Text(pDX, IDC_SOUNDDIR, m_strSoundDir);
	DDX_Radio(pDX, IDC_TYPE0, m_iType);
	DDX_Check(pDX, IDC_ANI, m_bAni);
	DDX_Text(pDX, IDC_ANIDIR, m_strAniDir);
	DDX_Check(pDX, IDC_ALLOWRUN, m_bAllowRun);
	DDX_Text(pDX, IDC_BROWSER, m_strBrowser);
	DDX_Check(pDX, IDC_AUTOSAVEMESS, m_bAutoSaveMess);
	DDX_Check(pDX, IDC_LISTENABLE, m_bListenAble);
	DDX_Check(pDX, IDC_AUTOLOGIN, m_bAutoLogin);
	DDX_Radio(pDX, IDC_AUTO1, m_iSaveMessType);
	DDX_Text(pDX, IDC_IP, m_strIP);
	DDV_MaxChars(pDX, m_strIP, 30);
	DDX_Text(pDX, IDC_COMMENT, m_strComment);
	DDV_MaxChars(pDX, m_strComment, 20);
	DDX_Text(pDX, IDC_IDLE, m_iIdleTime);
	DDV_MinMaxInt(pDX, m_iIdleTime, 0, 2000);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ADDNEW, m_ctlAddNew);
	DDX_Control(pDX, IDC_REMOVE, m_ctlRemove);
	DDX_Control(pDX, IDC_REMOVEALL, m_ctlRemoveAll);
	DDX_Control(pDX, IDOK, m_ctlOK);
	DDX_Control(pDX, IDCANCEL, m_ctlCancel);
	DDX_Control(pDX, IDC_SBROWSE, m_ctlBrowse);
	DDX_Control(pDX, IDC_ABROWSE, m_ctlAbrowse);
	DDX_Control(pDX, IDC_BBROWSE, m_ctlBBrowse);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CSettingsDlg)
	ON_BN_CLICKED(IDC_SOUND, OnSound)
	ON_BN_CLICKED(IDC_ANI, OnAni)
	ON_BN_CLICKED(IDC_SBROWSE, OnSbrowse)
	ON_BN_CLICKED(IDC_ABROWSE, OnAbrowse)
	ON_BN_CLICKED(IDC_ADDNEW, OnAddnew)
	ON_BN_CLICKED(IDC_REMOVEALL, OnRemoveall)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_TYPE0, OnType0)
	ON_BN_CLICKED(IDC_TYPE1, OnType1)
	ON_BN_CLICKED(IDC_BBROWSE, OnBbrowse)
	ON_NOTIFY(NM_CLICK, IDC_IPLIST, OnClickIplist)
	ON_EN_CHANGE(IDC_COMMENT, OnChangeComment)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg message handlers

BOOL CSettingsDlg::OnInitDialog() 
{
	//Set Button colors using my custom function
	SetMIMColor(&m_ctlAddNew);
	SetMIMColor(&m_ctlRemove);
	SetMIMColor(&m_ctlRemoveAll);
	SetMIMColor(&m_ctlOK);
	SetMIMColor(&m_ctlCancel);
	SetMIMColor(&m_ctlBrowse);
	SetMIMColor(&m_ctlAbrowse);
	SetMIMColor(&m_ctlBBrowse);

	CDialog::OnInitDialog();

	m_bGoToServer = FALSE;
	int i;
	bool show;

	m_ctlList.InsertColumn(0,"IP's");
	m_ctlList.InsertColumn(1,"Comments");
	m_ctlList.SetColumnWidth(0,100);
	m_ctlList.SetColumnWidth(1,145);
	m_ctlList.SetTextColor(m_clrFore);
	m_ctlList.SetBkColor(m_clrBG);
	m_ctlList.SetTextBkColor(m_clrBG);

	m_tooltip.Create (this);
	m_tooltip.Activate (TRUE);
	m_tooltip.AddTool (GetDlgItem (IDC_PORT), "Port of server you're connecting to /PORT");
	m_tooltip.AddTool (GetDlgItem (IDC_TYPE0), "Server Mode: Create a chatroom for others to join");
	m_tooltip.AddTool (GetDlgItem (IDC_TYPE1), "Client Mode: Join a chatroom server");
	m_tooltip.AddTool (GetDlgItem (IDC_SCREENNAME), "Client's screen name others see /NAME");
	m_tooltip.AddTool (GetDlgItem (IDC_IP), "IP address of the server /IP");
	m_tooltip.AddTool (GetDlgItem (IDC_ADDNEW), "Add entered IP to the saved list /ADDIP");
	m_tooltip.AddTool (GetDlgItem (IDC_REMOVE), "Remove entered IP from the saved list");
	m_tooltip.AddTool (GetDlgItem (IDC_REMOVEALL), "Remove all IP's from the saved list");
	m_tooltip.AddTool (GetDlgItem (IDC_IPLIST), "List of saved IP's");
	m_tooltip.AddTool (GetDlgItem (IDC_COMMENT), "Comment for IP address");
	m_tooltip.AddTool (GetDlgItem (IDC_ALLOWRUN), "Allow/block admin from running text on your computer");
	m_tooltip.AddTool (GetDlgItem (IDC_LISTENABLE), "Check this box if your computer has no firewall or router blocking a direct connection");
	m_tooltip.AddTool (GetDlgItem (IDC_AUTOLOGIN), "Enable/disable auto login at program startup");
	m_tooltip.AddTool (GetDlgItem (IDC_AUTOSAVEMESS), "Auto save your message history at program exit");
	m_tooltip.AddTool (GetDlgItem (IDC_AUTO1), "Save message history as HTML (saves text fonts, colors, and sizes)");
	m_tooltip.AddTool (GetDlgItem (IDC_AUTO2), "Save message history as plain text");
	
	//Populate IP list
	for(i = 0;i < m_strSaved.GetSize();i++)
	{
		m_ctlList.InsertItem(i, LPCTSTR(m_strSaved.GetAt(i)));
		m_ctlList.SetItemText(i, 1, LPCTSTR(m_strComments.GetAt(i)));
	}

	if(m_iType == 0)//Server
		show = FALSE;
	else
		show = TRUE;

	if(!m_bSound)
	{
		GetDlgItem(IDC_SBROWSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SOUNDDIR)->EnableWindow(FALSE);
	}
	if(!m_bAni)
	{
		GetDlgItem(IDC_ABROWSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_ANIDIR)->EnableWindow(FALSE);
	}

	GetDlgItem(IDC_BITSERVER)->ShowWindow(!show);
	GetDlgItem(IDC_BITCLIENT)->ShowWindow(show);
	GetDlgItem(IDC_PORT)->EnableWindow(show);
	GetDlgItem(IDC_SPORT)->EnableWindow(show);
	
	UpdateData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingsDlg::OnOK() 
{
	//Pressed OK,
	//return values for ChatroomDlg class and let it handle saving
	//the new info to the file.

	UpdateData(TRUE);
	int i, top;
	if(m_strIP == "")
	{
		MessageBox("Select an IP address from the list or type one in.");
		return;
	}
	top = m_ctlList.GetItemCount();//# of saved addresses
	
	//clear old strings
	m_strSaved.RemoveAll();
	m_strComments.RemoveAll();
	CString strTemp;
	//Copy from list to cleared strings.
	for(i = 0;i < top;i++)
	{
		strTemp = m_ctlList.GetItemText(i,0);
		m_strSaved.Add(strTemp);
		strTemp = m_ctlList.GetItemText(i,1);
		m_strComments.Add(strTemp);
	}

	if(m_bSound)
	{
		m_strSoundDir.MakeLower();
		m_strSoundExt = m_strSoundDir.Right(4);
		if((m_strSoundExt != ".wav") && (m_strSoundExt != ".mid"))//Lower case
		{
			MessageBox("Invalid sound file!");
			return;
		}
	}

	if(m_bAni)
	{
		m_strAniDir.MakeLower();
		m_strAniExt = m_strAniDir.Right(4);
		if((m_strAniExt != ".bmp") && (m_strAniExt != ".avi"))
		{
			MessageBox("Invalid image file!");
			return;
		}
	}
	m_strBrowser.MakeLower();
	strTemp = m_strBrowser.Right(4);
	if(strTemp != ".exe")
	{
		MessageBox("Invalid Internet Browser file!\n(Path name must end with \".exe\")");
		return;
	}

	CDialog::OnOK();
}

void CSettingsDlg::OnSound() 
{
	UpdateData(TRUE);
	if(m_bSound)//Enable sound
	{
		GetDlgItem(IDC_SBROWSE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SOUNDDIR)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_SBROWSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SOUNDDIR)->EnableWindow(FALSE);
	}
}

void CSettingsDlg::OnSbrowse() 
{
	//Load Dialog window. Allow ".wav" files
	CFileDialog Open(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"Sound Files (*.wav;*.mid)|*.wav;*.mid||");
	if(Open.DoModal() == IDOK)
	{
		m_strSoundDir = Open.GetPathName();
		m_strSoundDir.MakeLower();
		UpdateData(FALSE);
	}
}

void CSettingsDlg::OnBbrowse() 
{
	//Load Dialog window. Allow ".wav" files
	CFileDialog Open(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"Executable Files (*.exe)|*.exe||");
	if(Open.DoModal() == IDOK)
	{
		m_strBrowser = Open.GetPathName();
		m_strBrowser.MakeLower();
		UpdateData(FALSE);
	}
}

void CSettingsDlg::OnAni() 
{
	UpdateData(TRUE);
	if(m_bAni)//Enable Ani/bmp
	{
		GetDlgItem(IDC_ABROWSE)->EnableWindow(TRUE);
		GetDlgItem(IDC_ANIDIR)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ABROWSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_ANIDIR)->EnableWindow(FALSE);
	}
}

void CSettingsDlg::OnAbrowse() 
{
	//Load Dialog window. Allow ".bmp" and ".avi" files
	CFileDialog Open(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"Image Files (*.bmp;*.avi)|*.bmp;*.avi||");
	if(Open.DoModal() == IDOK)
	{
		m_strAniDir = Open.GetPathName();
		m_strAniDir.MakeLower();

		UpdateData(FALSE);
	}
}

void CSettingsDlg::OnAddnew() 
{
	//Adds the IP address in the edit box to the list.

	UpdateData(TRUE);
	int iTop = m_ctlList.GetItemCount();
	if(m_strIP != "")//make sure an address is entered
	{
		LVFINDINFO fInfo;
		fInfo.psz = LPCTSTR(m_strIP);
		//int temp = m_ctlList.FindStringExact(0, strSearch);
		int temp = m_ctlList.FindItem(&fInfo);
		if((temp >= 0) && (temp < iTop))
		{
			MessageBox("This IP address already exists.", "Duplicate IP address found", MB_OK | MB_ICONINFORMATION);
			return;
		}

		/*if(m_ctlList.GetCount() < 30)
		{*/
			m_ctlList.InsertItem(iTop, LPCTSTR(m_strIP));
			m_ctlList.SetItemText(iTop, 1, m_strComment);
			//m_strIP = "";
		/*}
		else
		{
			MessageBox("You can't have more than 30 saved addresses.");
			GetDlgItem(IDC_ADDNEW)->EnableWindow(FALSE);
		}*/
	}
	else
		MessageBox("You must enter a new IP address first.", "Add IP address", MB_OK | MB_ICONINFORMATION);

	UpdateData(FALSE);
}

void CSettingsDlg::OnRemove() 
{
	//Removes the selected IP address from the list.
	CString strRemove;
	UpdateData(TRUE);
	if(m_strIP != "")
	{
		int sel = m_ctlList.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);

		if(sel >= 0)//something is selected
		{
			m_ctlList.DeleteItem(sel);
			m_strIP = "";
			m_strComment = "";
			GetDlgItem(IDC_ADDNEW)->EnableWindow(TRUE);
		}
		else
			MessageBox("Select an IP first.");
	}
	else
		MessageBox("You must select an IP address first.", "Remove IP address", MB_OK | MB_ICONINFORMATION);
	UpdateData(FALSE);
}

void CSettingsDlg::OnRemoveall() 
{
	//Removes all IP addresses from the list
	if(MessageBox("Are you sure you want to remove all saved/nIP addresses?", "Remove all addresses?", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
	{
		m_ctlList.DeleteAllItems();
		UpdateData(FALSE);
	}
}

void CSettingsDlg::OnType0() 
{
	//Pressed the Server radio button,
	//Enable/Disable bmp's and edit boxes
	UpdateData(TRUE);
	if(m_iType == 0)
	{
		m_iType = 0;
		GetDlgItem(IDC_BITSERVER)->ShowWindow(TRUE);
		GetDlgItem(IDC_BITCLIENT)->ShowWindow(FALSE);
		GetDlgItem(IDC_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPORT)->EnableWindow(FALSE);
		m_bGoToServer = TRUE;
		OnOK();
	}
	UpdateData(FALSE);
}

void CSettingsDlg::OnType1() 
{
	//Pressed the Client radio button,
	//Enable/Disable bmp's and edit boxes

	UpdateData(TRUE);
	if(m_iType == 1)
	{
		GetDlgItem(IDC_BITCLIENT)->ShowWindow(TRUE);
		GetDlgItem(IDC_BITSERVER)->ShowWindow(FALSE);
		GetDlgItem(IDC_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPORT)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

void CSettingsDlg::OnClickIplist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//UpdateData(FALSE);
	int sel = m_ctlList.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);

	if(sel >= 0)//something is selected
	{
		m_strIP = m_ctlList.GetItemText(sel,0);
		m_strComment = m_ctlList.GetItemText(sel,1);
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CSettingsDlg::OnChangeComment() 
{
	//Changes the comments in the saved list to match that of the edit box when the corresponding IP is selected
	int sel = m_ctlList.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(sel >= 0)//something is selected
	{
		UpdateData(TRUE);
		LVFINDINFO fInfo;
		fInfo.psz = LPCTSTR(m_strIP);
		//int temp = m_ctlList.FindStringExact(0, strSearch);
		int temp = m_ctlList.FindItem(&fInfo);
		if((temp >= 0) && (temp < m_ctlList.GetItemCount()))
			m_ctlList.SetItemText(temp, 1, LPCTSTR(m_strComment));
	}
}

BOOL CSettingsDlg::PreTranslateMessage(MSG* pMsg) 
{
	m_tooltip.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CSettingsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
	m_brush->UnrealizeObject(); 
	pDC->SetBrushOrg(pt); 

    return (HBRUSH)*m_brush; 	// TODO: Return a different brush if the default is not desired
}
