// MasterListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MIM.h"
#include "MIMDlg.h"
#include "MasterListDlg.h"

// CMasterListDlg dialog

IMPLEMENT_DYNAMIC(CMasterListDlg, CDialog)

CMasterListDlg::CMasterListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMasterListDlg::IDD, pParent)
	, m_bFirst(TRUE)
{

}

CMasterListDlg::~CMasterListDlg()
{
}

void CMasterListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MLIST, m_ctlMList2);
}


BEGIN_MESSAGE_MAP(CMasterListDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMasterListDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ADD, &CMasterListDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REFRESH, &CMasterListDlg::OnBnClickedRefresh)
	ON_BN_CLICKED(IDCANCEL, &CMasterListDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_REMOVE, &CMasterListDlg::OnBnClickedRemove)
	ON_BN_CLICKED(IDC_STOP, &CMasterListDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_UPDATE, &CMasterListDlg::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_VIEW, &CMasterListDlg::OnBnClickedView)
END_MESSAGE_MAP()


// CMasterListDlg message handlers

void CMasterListDlg::SetMIMParent(CDialog *pWnd)
{
	m_pWnd = pWnd;
}

BOOL CMasterListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//MessageBox("OnInitDialog()");
	
	if(m_bFirst)//only attach the images once
	{
		m_bFirst = FALSE;
		m_iViewMode = 0;
		m_imgMasterList.Create(16, 16, ILC_COLOR16, 3, 1);
		m_imgMasterList.Add(AfxGetApp()->LoadIcon(IDI_USER));//server is empty
		m_imgMasterList.Add(AfxGetApp()->LoadIcon(IDI_NET));//server has users in it
		m_imgMasterList.Add(AfxGetApp()->LoadIcon(IDI_RED));//server is not responding

		m_imgMasterState.Create(16, 16, ILC_COLOR16, 3, 1);
		m_imgMasterState.Add(AfxGetApp()->LoadIcon(IDI_GREEN));//server is ready for you to connect
		m_imgMasterState.Add(AfxGetApp()->LoadIcon(IDI_PASSWORD));//server requires password
		m_imgMasterState.Add(AfxGetApp()->LoadIcon(IDI_NET));//nothing useful yet
	}
	m_ctlMList2.SetImageList(&m_imgMasterList,LVSIL_SMALL);//set image list for master list
	m_ctlMList2.SetImageList(&m_imgMasterState,LVSIL_STATE);//set state-image list for master list

	m_ctlMList2.SetBkColor(m_clrBack);
	m_ctlMList2.SetTextBkColor(m_clrBack);
	m_ctlMList2.SetTextColor(RGB(164,176,238));//the text and textBG colors are set within the CIconListCtrl drawing method

	//m_ctlMList2.SetIconSpacing(16,16);
	m_ctlMList2.InsertColumn(0,"Server",NULL,150,0);
	m_ctlMList2.InsertColumn(1,"Users",NULL,40,0);
	m_ctlMList2.InsertColumn(2,"Version",NULL,50,0);
	m_ctlMList2.InsertColumn(3,"Password",NULL,60,0);
	m_ctlMList2.InsertColumn(4,"IP",NULL,150,0);

	UpdateList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMasterListDlg::UpdateList(int index)
{
	//displays the server entries in the list. Updates all by default index = -1, or you can choose to update only a single entry by specifying index
	//m_ctlMList2.DeleteAllItems();

	/*for(int i = 0;i < 5;i++)
	{
		m_ctlMList2.InsertItem(i, "New Server", 2);
		m_ctlMList2.SetItemText(i, 1, "2/20");
		m_ctlMList2.SetItemText(i, 2, "3.02");
		m_ctlMList2.SetItemText(i, 3, "No");
		m_ctlMList2.SetItemText(i, 4, "192.168.1.100");
        m_ctlMList2.SetItemState(i, INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK);//Using SetItemState, WORKS! set state-image
	}*/
	int iMax,i;
	if(m_iViewMode == 0)
		iMax = m_oaServers.GetSize();//list of Public servers retrieved
	else
		iMax = m_strMSL_IPs.GetSize();//list of master servers that can provide a PSL to the client

	int icon = 0;
	CString strTemp;
	if(index != -1)//bounds check
		i = (index < iMax)?index:iMax-1;
	else i = 0;

	if(m_iViewMode == 0)//show list of Public Servers (PSL)
	{
		if(index == -1)
			m_ctlMList2.DeleteAllItems();

		for(;i < iMax;i++)
		{
			if(GetEntry(i)->m_iStatus == 2)//dead server
				icon = 2;//red icon
			m_ctlMList2.DeleteItem(i);
			m_ctlMList2.InsertItem(i, GetEntry(i)->m_strName, icon);
			//m_ctlMList2.SetItemText(i, 0, GetEntry(i)->m_strName);//server name
			m_ctlMList2.SetItemText(i, 2, GetEntry(i)->m_strVer);
			if(GetEntry(i)->m_iStatus == 1)//server is alive/responding
			{
				strTemp.Format("%i/%i",GetEntry(i)->m_iUsers,GetEntry(i)->m_iMax);
				m_ctlMList2.SetItemText(i, 1, strTemp);//user numbers, ex: "2/20"
				if(GetEntry(i)->m_iPassword == 1)
				{
					m_ctlMList2.SetItemText(i, 3, "Password");
					m_ctlMList2.SetItemState(i, INDEXTOSTATEIMAGEMASK(2), LVIS_STATEIMAGEMASK);//Using SetItemState, WORKS! set state-image
				}
				else if(GetEntry(i)->m_iPassword == 2)
				{
					m_ctlMList2.SetItemState(i, INDEXTOSTATEIMAGEMASK(2), LVIS_STATEIMAGEMASK);//Using SetItemState, WORKS! set state-image
					m_ctlMList2.SetItemText(i, 3, "Bouncer");
				}
				else//green for public
				{
					m_ctlMList2.SetItemState(i, INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK);//Using SetItemState, WORKS! set state-image
					m_ctlMList2.SetItemText(i, 3, "Public");
				}
			}

			strTemp.Format("%s:%i",GetEntry(i)->m_strIP,GetEntry(i)->m_iPort);
			m_ctlMList2.SetItemText(i, 4, strTemp);
			if(index != -1)
				break;//stop after updating one server
		}
	}
	else//View list of Master Servers (MSL)
	{
		if(index == -1)
			m_ctlMList2.DeleteAllItems();
		for(;i < iMax;i++)
		{
			m_ctlMList2.InsertItem(i, m_strMSL_IPs.GetAt(i), icon);
			strTemp.Format("%s:%i",m_strMSL_IPs.GetAt(i),m_iMSL_Ports.GetAt(i));
			m_ctlMList2.SetItemText(i, 4, strTemp);
			if(index != -1)
				break;//stop after updating one server
		}
	}
	//else
	//m_ctlMList2.SetItemState(i, INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK);//Using SetItemState, WORKS! set state-image
}

int CMasterListDlg::AddServer(CServerEntry *oItem, BOOL clear, BOOL bAbort)
{
	//This adds a server based on the IP:port. if a matching IP:port is found, the entry attributes are overwritten not added.
	//if bAbort = TRUE, the entry is not replaced/added if it already exists
	
	//search for matches
	int iMax = m_oaServers.GetCount();
	for(int i = 0;i < iMax;i++)
	{
		//if a match is found, just update the other attributes
		if((GetEntry(i)->m_strIP == oItem->m_strIP) && (GetEntry(i)->m_iPort == oItem->m_iPort))
		{
			if(bAbort)//flag is set, so do not change if it matches
				return -1;
			CObject *prev = m_oaServers.GetAt(i);
			m_oaServers.SetAt(i, new CServerEntry(oItem));//overwrite it
			delete prev;//delete the old pointer
			return 1;
		}
	}

	try//otherwise just adding a new server to the obArray list
	{
		m_oaServers.Add(new CServerEntry(oItem));//create it
	}
	catch (CMemoryException* perr)
	{
		//check it
		AfxMessageBox("Out of memory, Master Server List too large.", MB_ICONSTOP|MB_OK);
		perr->Delete();
	}
	//NOTE: the input pointer oItem is reset() after copied into the obArray of clear = TRUE (by default)
	if(clear)
		oItem->Reset();
	return 0;
}

CServerEntry * CMasterListDlg::GetEntry(int index)
{
	//return item at index, not pointer
	return (CServerEntry*)m_oaServers[index];
}

void CMasterListDlg::OnBnClickedAdd()
{
	//add a server to the list with custom IP:port
	if(m_AddServerDlg.DoModal() == IDOK)
	{
		if(m_iViewMode == 0)
		{
			//NOTE: if user is a PSL provider, then this only edits his public list, not the PSL he sends to others
			CServerEntry oItem;
			//add only the items given
			oItem.m_strIP = m_AddServerDlg.m_strIP;
			oItem.m_iPort = m_AddServerDlg.m_iPort;
			oItem.m_bCustom = true;
			if(AddServer(&oItem, TRUE, TRUE) == -1)//do not add if it's already in the list
				MessageBox("This server is already in the list.");
			else
				UpdateList();
		}
		else
		{
			//TODO: add to the MSL obArrays and check for redundancies
			int iMax = m_strMSL_IPs.GetCount();
			for(int j = 0;j < iMax;j++)
			{
				if(m_strMSL_IPs.GetAt(j) == m_AddServerDlg.m_strIP && m_iMSL_Ports.GetAt(j) == m_AddServerDlg.m_iPort)
				{
					MessageBox("This server is already in the list.");
					return;
				}
			}
			//otherwise add it
			m_strMSL_IPs.Add(m_AddServerDlg.m_strIP);
			m_iMSL_Ports.Add(m_AddServerDlg.m_iPort);
			UpdateList(iMax);
		}
	}
}

void CMasterListDlg::OnBnClickedRemove()
{
	int here = m_ctlMList2.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	//Remove the server. List index is the same as the obArray index
	if(m_iViewMode == 0)
	{
		if (here < 0 || here >= m_oaServers.GetCount())
			return;
		m_oaServers.RemoveAt(here);
		m_ctlMList2.DeleteItem(here);
	}
	else if(m_iViewMode == 1)
	{
		if (here < 0 || here >= m_strMSL_IPs.GetCount())
			return;
		m_strMSL_IPs.RemoveAt(here);
		m_iMSL_Ports.RemoveAt(here);
		m_ctlMList2.DeleteItem(here);
	}
}

void CMasterListDlg::OnBnClickedOk()
{
	m_iViewMode = 0;//FIX: instead place this somewhere in MIMDlg?
	OnOK();
}

void CMasterListDlg::OnBnClickedCancel()
{
	m_iViewMode = 0;//FIX: instead place this somewhere in MIMDlg?
	OnCancel();
}
void CMasterListDlg::DisableButtons(BOOL bHide)
{
	GetDlgItem(IDC_STOP)->EnableWindow(bHide);
	GetDlgItem(IDC_REFRESH)->EnableWindow(!bHide);
	GetDlgItem(IDC_VIEW)->EnableWindow(!bHide);
	GetDlgItem(IDC_UPDATE)->EnableWindow(!bHide);
	GetDlgItem(IDC_ADD)->EnableWindow(!bHide);
	GetDlgItem(IDC_REMOVE)->EnableWindow(!bHide);
	GetDlgItem(IDOK)->EnableWindow(!bHide);
	GetDlgItem(IDCANCEL)->EnableWindow(!bHide);
}

void CMasterListDlg::OnBnClickedStop()
{
	((CMIMDlg*)m_pWnd)->m_bStopServerInfoRequest = TRUE;//this should stop the loop when it gets the first chance
}

void CMasterListDlg::OnBnClickedRefresh()
{
	if(m_iViewMode == 0)//refresh all servers in list
	{
		DisableButtons(TRUE);
		((CMIMDlg*)m_pWnd)->m_iInfoRequestIndex = 0;
		((CMIMDlg*)m_pWnd)->ServerInfoRequest(-1);//refresh all servers in list
	}
	/*else if(m_iViewMode == 1)//clear current server list and copy over the MSL.
	{
		//sync this current MSL with the one in the server list
		//First, delete the list of public servers
		RemoveAllServers();
		
		for(int i=0; i<m_strMSL_IPs.GetCount(); i++)
		{
			//populate the list
			m_oaServers.Add(new CServerEntry(m_strMSL_IPs.GetAt(i), m_iMSL_Ports.GetAt(i)));//create it with constructor
		}
		MessageBox("MSL copied to the server list.");
	}*/
}

void CMasterListDlg::OnBnClickedUpdate()
{
	if(m_iViewMode == 0)//Get the latest PSL from a first available Master Server
	{
		//Retrieve the Public Server List from the first available Master Server
		((CMIMDlg*)m_pWnd)->m_iInfoRequestIndex = 0;
		((CMIMDlg*)m_pWnd)->RetrievePublicList();
	}
	else if(m_iViewMode == 1)//Get the latest MSL from MasterHD
	{
		((CMIMDlg*)m_pWnd)->RetrieveMasterList();
		//TODO: request the MSL from me. This is only needed if all the master servers are dead.
		//This does not edit the PSL, it edits the MSL
		RemoveAllMasterServers();//First, delete the current MSL because it will be overwritten.
		
		//now request the MSL data from MasterHD
		/*for(int i=0; i<m_strMSL_IPs.GetCount(); i++)
		{
			//populate the list
			m_oaServers.Add(new CServerEntry(m_strMSL_IPs.GetAt(i), m_iMSL_Ports.GetAt(i)));//create it with constructor
		}*/
		MessageBox("MSL retrieved from MasterHD, showing in list.");
	}
}

void CMasterListDlg::OnBnClickedView()
{
	//toggle modes
	if(m_iViewMode == 0)
	{
		if(m_strMSL_IPs.GetCount() == 0)//no MSL created yet
		{
			if (MessageBox("MSL (Master Server List) is a list of all Master servers that have registered and provide the Public Server List (PSL) to anyone who requests it. If you want to act as an MSL provider contact me to be added to the list of dedicated MSL providers, otherwise editing this list here will serve no purpose.\nDo you want to continue?", "MSL Provider", MB_YESNO | MB_ICONQUESTION) == IDNO)
			{
				//((CMIMDlg*)m_pWnd)->m_bPSL_Exe = FALSE;
				return;
			}
		}
		GetDlgItem(IDC_REFRESH)->EnableWindow(FALSE);
		//GetDlgItem(IDC_REFRESH)->SetWindowText("&Sync MSL");
		GetDlgItem(IDC_UPDATE)->SetWindowText("&Update MSL");
		((CMIMDlg*)m_pWnd)->m_bPSL_Exe = TRUE;
		m_iViewMode = 1;
		SetWindowText("Master Server List");
		GetDlgItem(IDC_VIEW)->SetWindowText("&View PSL");
		//m_ctlMList2.SetColumnWidth(2);
	}
	else
	{
		GetDlgItem(IDC_REFRESH)->EnableWindow(TRUE);
		GetDlgItem(IDC_UPDATE)->SetWindowText("&Update List");

		m_iViewMode = 0;
		SetWindowText("Public Server List");
		GetDlgItem(IDC_VIEW)->SetWindowText("&View MSL");
	}
	UpdateList();
}

void CMasterListDlg::RemoveAllServers()
{
	//delets all servers in the server list
	int iMax = m_oaServers.GetCount();
	for(int i=0; i<iMax; i++)
		delete m_oaServers[i];
	m_oaServers.RemoveAll();
}

void CMasterListDlg::RemoveAllMasterServers()
{
	//delets all servers in the Master server list
	m_strMSL_IPs.RemoveAll();
	m_iMSL_Ports.RemoveAll();
}