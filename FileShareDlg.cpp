// FileShareDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MIM.h"
#include "FileShareDlg.h"
#include "math.h"
#include "MIMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileShareDlg dialog


CFileShareDlg::CFileShareDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileShareDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileShareDlg)
	m_strPercent = _T("");
	m_strDisplayPath = _T("");
	m_strCurFile = _T("");
	m_strPercent2 = _T("");
	m_strElapsed = _T("");
	m_strStatus = _T("");
	m_strRate = _T("");
	m_strOtherName = _T("");
	//}}AFX_DATA_INIT
}


void CFileShareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileShareDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_ctlProg);
	DDX_Control(pDX, IDC_PROGRESS2, m_ctlProg2);
	DDX_Text(pDX, IDC_FILE, m_strPercent);
	DDX_Text(pDX, IDC_FILENAME, m_strDisplayPath);
	DDX_Text(pDX, IDC_FILE2, m_strCurFile);
	DDX_Text(pDX, IDC_PERCENT2, m_strPercent2);
	DDX_Text(pDX, IDC_ELAPSED, m_strElapsed);
	DDX_Text(pDX, IDC_STATUS, m_strStatus);
	DDX_Text(pDX, IDC_RATE, m_strRate);
	DDX_Text(pDX, IDC_OTHERNAME, m_strOtherName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileShareDlg, CDialog)
	//{{AFX_MSG_MAP(CFileShareDlg)
	ON_BN_CLICKED(IDC_CANCTRANS, OnCancelTrans)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileShareDlg message handlers

BOOL CFileShareDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_iCurFile = 0;
	m_strCurFile.Format("File %i/%.0f",m_iCurFile+1,m_dTotal);
	m_pFClientSocket = NULL;
	m_pFListenSocket = NULL;
	m_pFSocketFile = NULL;
	m_bConOpen = FALSE;
	m_bConnected = FALSE;
	m_bReady = FALSE;		//open
	m_Elapsed = 0;
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

	if(m_bRec)
	{
		if(MessageBox("Do you want to accept files from this user?", "Incoming File(s)",MB_ICONQUESTION|MB_YESNO) == IDNO)
		{
			OnCancel();
			return TRUE;
		}
		UpdateData(TRUE);
		//Save all messages to a "txt" file
		CString strFile, strFilter, strTemp;
		strFilter = "Folders (*.000)|*.000||";

		CFileDialog dLoad(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, strFilter);
		strcpy(dLoad.m_ofn.lpstrFile, "folder.exe");
		if(dLoad.DoModal() == IDOK)
		{
			strTemp = dLoad.GetFileName();
			m_strDLto = dLoad.GetPathName();
			m_strDLto.TrimRight(strTemp);//cut off ending
		}
		else//cancelled
		{
			OnCancel();
			return TRUE;
		}
		UpdateData(FALSE);
		OnBCall();
	}
	else
	{
		OnListen();
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CFileShareDlg::SendFile()
{
	int z = 0;

	m_iCurFile++;
	StartTimer();
	UpdateData(FALSE);

	while(z == 0)//error loop
	{
		if((m_iCurFile >= m_dTotal) || (m_iCurFile > 39))
		{
			OnCancelTrans();;
			//m_strStatus = "File transfer complete.\n";
			//if(m_bLog)
			//	fprintf(m_fLog.GetFile(), LPCTSTR(m_strStatus));
			StepRate();
			m_strStatus = "Upload complete.";
			SetWindowText("File Done");
			m_strCurFile.Format("Current File: %i/%.0f", m_iCurFile, m_dTotal);//fixes 5/4 error
			UpdateData(FALSE);
			return -1;//Done
		}

		if(!m_bConnected)
			return -2;//error

		if(m_pFClientSocket == NULL)//no socket open for receiving
			return -4;

		m_strFileName = m_strFileNames.GetAt(m_iCurFile);
		m_strPath = m_strFileDirs.GetAt(m_iCurFile);
		m_strFinalPath = m_strPath + m_strFileName;
		//Open a file and send name to other client
		//MessageBox("About to send file...");

		double dSize;
		//returns file size here, ONLY CHANCE! or returns error.
		//this function tells socket class the file name, location, and size.
		dSize = m_pFClientSocket->SendFileInfo(m_strPath, m_strFileName);
		//NEWFILE was sent with the name, the transfer process will start when 'O' is returned.
		
		if(dSize == -1)//error
		{
			m_strMess.Format("Could not open file \"%s\"! It will be skipped.", m_strFileName);
			MessageBox(m_strMess, "Load file", MB_ICONSTOP);
			m_iCurFile++;
			//loop back again
		}
		else if(dSize == -2)//error
			MessageBox("Cant send file, not connected", "Load file", MB_ICONSTOP);
		else if(dSize > 0)//now just wait for receiver to say OK
		{
			m_pFClientSocket->m_dFileSize = dSize;
			m_iRange = 1;
			while(m_pFClientSocket->m_dFileSize/pow(1024,m_iRange) > 100000)
				m_iRange++;//lower range size for progress2 bar to display large files too

			m_ctlProg2.SetRange(0, (int)(m_pFClientSocket->m_dFileSize/pow(1024,m_iRange)));
			m_strDisplayPath = m_strFinalPath;
			//FIX:
			//m_strStatus.Format("SendFile(): \"%s\"\n", m_strFileName);
			//if(m_bLog)
			//	fprintf(m_fLog.GetFile(), LPCTSTR(m_strStatus));
			z = 1;//exit loop, waits for file transfer to finish, then FileDone() calls this function again.
		}
		else
		{
			MessageBox("Unknown error sending FILEINFO");
			return -3;
		}
	}
	//MessageBox("SendFileInfo() Successful");
	UpDisplay();

	return m_iCurFile;
}

void CFileShareDlg::OnCancelTrans() 
{
	GetDlgItem(IDC_CANCTRANS)->EnableWindow(FALSE);
	m_strStatus = "File transferring cancelled.";
	if(m_pFClientSocket != NULL)
	{
		m_pFClientSocket->SendPacket(STOP);//tell other side too
		m_pFClientSocket->StopStreams();//close streams
	}
	Cancel();
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
}

void CFileShareDlg::Cancel()
{
	//Other clients should already be cancelled by this time
	//if(m_bLog)
	//	m_fLog.LineOut("File transferring cancelled.");

	m_bConnected = FALSE;	//no sharing, but still connected, m_bConOpen = TRUE still

	//disables controls etc.
	//m_bAllowRec = FALSE;
	//m_bAllowSend = FALSE;

	KillTimer(ID_TIMER);

	//FIX:
	/*GetDlgItem(IDC_CANCEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAUSE)->EnableWindow(FALSE);
	if(!m_bSender)
		GetDlgItem(IDC_RECEIVE)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_RECEIVE)->EnableWindow(FALSE);*/

	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	//m_ctlAni.Stop();
	//m_ctlAni.ShowWindow(FALSE);

	//m_iCurFile--;//start where transfer left off
	if(m_pFClientSocket != NULL)
	{
		m_pFClientSocket->m_dFilePos = 0;
		m_pFClientSocket->m_dFileSize = 0;
	}
	m_ctlProg.SetRange(0, int(m_dTotal));
	//m_strTotal.Format("%.0f File(s)", m_dTotal);
	m_iTriggerStep = 20;
	m_iStep = 0;
	UpDisplay();
}

void CFileShareDlg::StepRate()
{
	//m_iStep should be 10
	//called every 10 packet sends or receives, edits progress bar pos
	m_iCurRate = m_iStep;
	UpDisplay();
	//m_strStep.Format("%i",m_iStep);
	
	m_iStep = 0;
	m_dRateCount += m_iCurRate;
	return;
}

void CFileShareDlg::UpDisplay()
{	
	ASSERT(m_dTotal != 0);
	m_ctlProg.SetPos(m_iCurFile);
	m_strCurFile.Format("Current File: %i/%.0f", m_iCurFile+1, m_dTotal);
	m_strPercent.Format("%.0f%% Total", float(m_iCurFile/m_dTotal*100));
	if(m_pFClientSocket != NULL)
	{
		m_ctlProg2.SetPos((int)(m_pFClientSocket->m_dFilePos/pow(1024,m_iRange)));
		m_strMess.Format("%.0f%%", float(m_pFClientSocket->m_dFilePos/m_pFClientSocket->m_dFileSize*100));
		m_strPercent2.Format("%.0f / %.0f KB        %s", m_pFClientSocket->m_dFilePos/1024, m_pFClientSocket->m_dFileSize/1024, m_strMess);
		m_strMess += " Complete";
		SetWindowText(m_strMess);
	}
	m_strElapsed.Format("Elapsed: %02i:%02i:%02i", m_Elapsed.GetHour()-17, m_Elapsed.GetMinute(), m_Elapsed.GetSecond());	
	UpdateData(FALSE);
}

void CFileShareDlg::OnListen()
{
	UpdateData(TRUE);

	if(m_pFListenSocket != NULL)
		return;

	m_bReady = TRUE;
	//ClearLabels();

	m_iType = SERVER;//server
	/*GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_CANCEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAUSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_RECEIVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CLOSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_LISTEN)->EnableWindow(FALSE);
	GetDlgItem(IDC_CALL)->EnableWindow(FALSE);
	GetDlgItem(IDC_IP)->EnableWindow(FALSE);
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_SCREENNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_AUTO)->EnableWindow(FALSE);*/

	m_pFListenSocket = new CMyFileSocket(this,-1);

	if(m_pFListenSocket->Create(m_iPort))
	{
		if(m_pFListenSocket->Listen())//successful
			m_strStatus.Format("Server created on port %i, Waiting for others to join...\n", m_iPort);
		else//port is busy
		{
			m_strMess.Format("Could not listen for others on socket %i.", m_iPort);
			MessageBox(m_strMess, "Establishing Listen Mode Error", MB_OK | MB_ICONSTOP);
			m_strStatus.Format("Failed to listen on socket %i.", m_iPort);

			OnClose(-1);
		}
	}
	else
	{
		m_strMess.Format("Could not create a socket on port %i. Make sure that TCP/IP is running \nor try a different Port number.", m_iPort);
		MessageBox(m_strMess, "Establishing Connection Error", MB_OK | MB_ICONSTOP);
		m_strStatus.Format("Failed to create socket %i. TCP-IP or different Port number needed.", m_iPort);

		OnClose(-1);
	}
	Reset();
	UpdateData(FALSE);
}

void CFileShareDlg::OnBCall()
{
	if(m_strIP == "")
	{
		MessageBox("Enter the IP address of the host/server.");
		return;
	}

	//ClearLabels();
	m_bReady = TRUE;
	m_iType = CLIENT;
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	/*GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_AUTO)->EnableWindow(FALSE);
	GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_CANCEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAUSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_LISTEN)->EnableWindow(FALSE);
	GetDlgItem(IDC_CALL)->EnableWindow(FALSE);
	GetDlgItem(IDC_IP)->EnableWindow(FALSE);
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_SCREENNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_CLOSE)->EnableWindow(TRUE);*/

	//Client, connect to listen socket
	m_pFClientSocket = new CMyFileSocket(this,-1);

	if(m_pFClientSocket->Create())
	{
		m_pFSocketFile = new CSocketFile(m_pFClientSocket);
		m_pFClientSocket->m_bSender = m_bSender;
		m_pFClientSocket->CreateArchives(m_pFSocketFile);
		m_strStatus.Format("Attempting to connect to server \"%s\" on port %i, please wait...\n",m_strIP, m_iPort);

		m_pFClientSocket->Connect(m_strIP, m_iPort);
		m_pFClientSocket->m_iPort = m_iPort;
	}
	else
	{
		//m_strStatus.Format("Could not create socket to enable connection.\nTCP/IP may be missing.");
		OnClose(-1);//cleans up
		UpdateData(FALSE);
		return;
	}
	Reset();
	UpdateData(FALSE);
}

void CFileShareDlg::OnClose(int id)
{
	Finalize();

	if(m_iType == CLIENT)//client
	{
		if(id != -1)//client pressed close
			m_strStatus = "OnClose(): Disconnected from server.\n";
		else
			m_strStatus = "OnClose(-1): Server closed connection.\n";
		
		//reset all data
		KillTimer(ID_TIMER);
	}
	else//Server
	{
		if(id != -1)//server pressed close
			m_strStatus = "OnClose(): Client disconnected from server.\n";
		else
			m_strStatus = "OnClose(-1): Server closed.\n";
	}
	//if(m_bLog)
	//	fprintf(m_fLog.GetFile(), LPCTSTR(m_strStatus));

	m_bConnected = FALSE;
	m_bAllowSend = FALSE;
	m_bAllowRec = FALSE;
	m_bConOpen = FALSE;
	m_bReady = FALSE;

	GetDlgItem(IDC_CANCTRANS)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	
	UpdateData(FALSE);
}

void CFileShareDlg::Finalize()
{
	//Closes all sockets and Cleans them ALL up
	//NOTE: Each sockets' delete function for archives is called in destructor

	if(m_pFClientSocket != NULL)
	{
		m_pFClientSocket->Close();
		delete m_pFClientSocket;
		m_pFClientSocket = NULL;
	}
	if(m_pFListenSocket != NULL)
	{
		m_pFListenSocket->Close();
		delete m_pFListenSocket;
		m_pFListenSocket = NULL;
	}
	if(m_pFSocketFile != NULL)//Must delete socketfile before/after? sockets
	{
		delete m_pFSocketFile;
		m_pFSocketFile = NULL;
	}
}

void CFileShareDlg::Reset()
{
	//Only resets basic file settings, in case the transfer is re-initiated after a cancel
	//m_iCurFile = 0;

	if(m_pFClientSocket != NULL)
	{
		m_pFClientSocket->m_dFilePos = 0;
		m_pFClientSocket->m_dFileSize = 0;
	}
	m_ctlProg.SetRange(0, int(m_dTotal));
	m_ctlProg2.SetRange(0, 0);
	m_iTriggerStep = 20;
	m_iStep = 0;
}

void CFileShareDlg::OnTimer(UINT nIDEvent) 
{
	//Change m_iTriggerStep depending on DL speed
	if((m_dRateCount < 1) && (m_iTriggerStep > 1))
		m_iTriggerStep -= 1;
	else if((m_dRateCount/m_iTriggerStep > 5) && (m_iTriggerStep < 100))
		m_iTriggerStep += 1;

	//Display bitrate
	m_dRate = m_dRateCount*1010;
	if(m_dRate > 1024)
		m_strRate.Format("%.1f KB/Sec", m_dRate/1024);
	else
		m_strRate.Format("%.1f Bytes/Sec", m_dRate);

	//DEBUG: m_strRate.Format("%.1f/%i = %.1f KB/Sec", m_dRateCount, m_iTimerCount, m_dRate);
	UpdateData(FALSE);
	m_dRateCount = 0;	//reset StepRate() vars:
	m_dRate = 0;
	m_Elapsed += 1;

	//IDLE COUNTER AND HANDLING:
	/*if(m_bAllowRec)
		m_iIdleCount++;//only for receiver
	
	if(m_iIdleCount > 20)
	{
		KillTimer(ID_TIMER);
		m_strMess.Format("The transfer has been idle for more than %i seconds, would you\nlike to request the server to re-send the next stream?\nPress \"No\" if the transfer has started again or to continue waiting,\n press \"Cancel\" to stop the transfer.", 20);
		int iRes = MessageBox(m_strMess, "Idle Timeout", MB_YESNOCANCEL);
		if(iRes == IDYES)
		{
			m_iIdleCount = 0;
			//if(m_pFClientSocket != NULL)//no socket open for receiving
			//	m_pFClientSocket->SendPkg('E');
			SetTimer(ID_TIMER, 1000, NULL);//set timer for rate
		}
		else if(iRes == IDCANCEL)
			OnCancelTrans();
	}*/
	CDialog::OnTimer(nIDEvent);
}

void CFileShareDlg::StartTimer()
{
	m_dRateCount = 0;
	m_dRate = 0;
	m_iIdleCount = 0;
	SetTimer(ID_TIMER, 1000, NULL);//set timer for rate
}

void CFileShareDlg::OnCancel() 
{
	if(GetDlgItem(IDCANCEL)->IsWindowEnabled())
	{
		OnClose(-1);
		GetParent()->GetDlgItem(IDC_SENDFILE)->EnableWindow(TRUE);

		CDialog::OnCancel();
	}
}

void CFileShareDlg::OnAccept()
{
	CString strFull;

	//MessageBox("Entered OnAccept(); function.");

	m_pFClientSocket = new CMyFileSocket(this,-1);

	if(m_bConOpen)
	{
		CMyFileSocket *pFullSocket;
		CSocketFile *pSocketFile;
		pFullSocket = new CMyFileSocket(this,-1);
		m_pFListenSocket->Accept(*pFullSocket);
		pSocketFile = new CSocketFile(pFullSocket);
		pFullSocket->CreateArchives(pSocketFile);
		//strFull.Format("%s is already connected with another client, try again later.", m_strScreenName);
		CPacket FPacket;
		FPacket.m_iPos = -1;
		FPacket.m_iCode = MESG;
		FPacket.m_iLength = strFull.GetLength();
		strcpy(FPacket.pBuf, LPCTSTR(strFull));
		pFullSocket->SendPacket(FPacket);
		
		pFullSocket->Close();
		delete pFullSocket;
		delete pSocketFile;
		pFullSocket = NULL;
		pSocketFile = NULL;
		return;
	}

//Accept the connection request

	if(m_pFListenSocket->Accept(*m_pFClientSocket))
	{
		m_pFSocketFile = new CSocketFile(m_pFClientSocket);//each accept should use a seperate CSocketFile 
		m_pFClientSocket->m_bSender = m_bSender;
		m_pFClientSocket->CreateArchives(m_pFSocketFile);
		m_pFClientSocket->m_iPort = m_iPort;
		//MessageBox("Requesting Client Info...");
		m_bConOpen = TRUE;
		m_bConnected = TRUE;

		//Send screen name along with info request
		CPacket FPacket;
		FPacket.m_iCode = FREG;
		FPacket.m_iPos = 0;
		FPacket.pBuf[0] = int(m_dTotal);
		FPacket.m_iLength = 1;
		m_pFClientSocket->SendPacket(FPacket);

		m_strStatus = "Client successfully connected to server, receiving user info...\n";
	}
	else//error
		MessageBox("A client attempted to join the server, but failed");
	UpdateData(FALSE);
}

void CFileShareDlg::OnConnect()
{
	UpdateData(TRUE);
	//For CLIENTS only
	//connection request accepted by server, level 2 access is given
	//(Cannot recieve chat messages yet). Confirmation is
	//required to recieve level 1 access (chat capabilities).
	//KillTimer(ID_TIMER);
	//m_Console.DisplayMessage(">> Connection accepted, verifying...");
	//m_strTimeout = "Connected to server";
	m_bConOpen = TRUE;
	m_strStatus.Format("Successfully connected to server \"%s\" on port %i, press Receive if you are going to receive files. Otherwise, wait for server.\n", m_strIP, m_iPort);

	m_bConnected = TRUE;
	UpdateData(FALSE);
	OnBReceive();//auto
	//Wait for request for ver, name, and password
	//OnRecieve should be called next, with code REG (request info)
}

void CFileShareDlg::OnBReceive()
{
	GetDlgItem(IDC_CANCTRANS)->EnableWindow(TRUE);
	/*GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_RECEIVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CLOSE)->EnableWindow(FALSE);*/

	//MessageBox("Pressed Receive\nSending RWAITING");	
	m_pFClientSocket->SendPacket(RWAITING);//waiting to receive files
	if(m_dTotal <= 0)
	{
		MessageBox("No files to receive");
		return;
	}

	m_Elapsed = 0;
	m_bSender = FALSE;
	m_pFClientSocket->m_bSender = m_bSender;
	m_bConnected = TRUE;
	m_bAllowRec = TRUE;
	m_bAllowSend = FALSE;

	m_ctlProg.SetRange(0, int(m_dTotal));
	UpDisplay();
}

double CFileShareDlg::RecFile(CString strFile, double dSize)//FileShare
{
	//receiver only, called from socket class when code NEWFILE is received from sender
	if(!m_bConnected)
	{
		MessageBox("Connection not open!");
		return -1;
	}

	//#3 of process
	//client is ready to receive a new file
	//receive a file from sender (name and size first packet)
	m_strStatus.Format("Received File Info: \"%s\" - Total Size: %.0f Bytes.\n", strFile, dSize);

	//MessageBox(m_strStatus);
	//m_iFilePos = 0;

	m_iRange = 1;
	while(dSize/pow(1024,m_iRange) > 10000)
		m_iRange++;

	m_ctlProg2.SetRange(0, (int)(dSize/pow(1024,m_iRange)));
	StartTimer();
	UpDisplay();

	//save to list for end report
	m_strFileName = strFile;
	m_strPath = m_strDLto;
	m_strFileNames.Add(strFile);
	m_strFileDirs.Add(m_strDLto);
	//m_ctlAni.Play(0, -1, -1);
	UpdateData(FALSE);

	return 0;
}

void CFileShareDlg::OnBSend()
{
	//begin sending files now, peer is waiting
	GetDlgItem(IDC_CANCTRANS)->EnableWindow(TRUE);

	if(!m_bSender)//But you are not the sender?
	{
		//SendText(MESG, "Niether peer has selected files to transfer.");
		MessageBox("Niether peer has selected files to transfer.");
		OnCancelTrans();
		return;
	}

	if(!m_bSender)
	{
		MessageBox("You dont have any files to send.");
		return;
	}
	//m_bSender = TRUE;//dont do it
	m_bAllowSend = TRUE;
	m_bAllowRec = FALSE;
	m_bConnected = TRUE;

	//pressed the Send button, begin transfer process:
	//m_ctlAni.ShowWindow(TRUE);
	//m_ctlAni.Play(0, -1, -1);
	GetDlgItem(IDC_CANCTRANS)->EnableWindow(TRUE);
	//GetDlgItem(IDC_PAUSE)->EnableWindow(TRUE);
	//GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	//GetDlgItem(IDC_CLOSE)->EnableWindow(FALSE);

	m_strStatus = "Sending files...\n";
	m_Elapsed = 0;

	m_iCurFile--;//balance reconnecting, in case its a resume after cancel

	UpDisplay();

	if(SendFile() == -2)//start the process
		MessageBox("Connection not open!");
}

int CFileShareDlg::FileDone()
{
	//Called for both sender and receiver
	//WARNING: DO NOT call this function, it willed be auto called from CMyFileSocket.
	//Called whenever the current file is complete or stopped.
	//All file streams are already closed. A new file will begin with a call to SendFile()
	//Steps #7 AND #8 continued...

	double dHere;

	KillTimer(ID_TIMER);
	m_dRateCount = 0;
	m_dRate = 0;

	UpDisplay();

	if(m_pFClientSocket == NULL)//no socket open for receiving
		return 1;

	dHere = m_pFClientSocket->FileInf.dSize;

	if(m_pFClientSocket->m_dFilePos == -10)//receiver cancelled
		;
	else if((m_pFClientSocket->m_dFilePos != dHere) && (!m_bSender))
	{
		m_strMess.Format("File sizes differ!\nSender: %.0f bytes\nReceiver: %.0f bytes", dHere, m_pFClientSocket->m_dFilePos);
		MessageBox(m_strMess);
	}
	m_pFClientSocket->m_dFilePos = 0;
	m_pFClientSocket->m_iPos = 0;

	if(m_bAllowSend)//sender only
	{
		//MessageBox("File done!");
		//Sender only, tell client to move to next file
		if(SendFile() == -1)//finished uploading
			return 1;
			//OnOK();//Dont call these from INSIDE CMyFileSocket, because CMyFileSocket is destroyed in OnClose()
			//OnClose(-1);
	}
	else //receiver only
	{
		m_iCurFile++;
		UpDisplay();
		if(m_iCurFile >= m_dTotal)//Done
		{
			m_strStatus.Format("File transfer complete: Last file #%i.\n", m_iCurFile);

			OnCancelTrans();
			StepRate();
			m_strStatus = "Download complete";
			SetWindowText("File Done");
			m_strCurFile.Format("Current File: %i/%.0f", m_iCurFile, m_dTotal);
			UpdateData(FALSE);
			//OnOK();
			//OnClose(-1);//BAD!
		}
		//Now just wait for sender to send NEWFILE with a new filename.
	}
	return 0;
}
