// MyFileSocket.cpp: implementation of the CMyFileSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIM.h"
#include "MyFileSocket.h"
#include "FileShareDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//Use this socket class for sending files with the CFileShareDlg

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMyFileSocket::CMyFileSocket(CDialog *pWnd, int ID)
{
	m_pWnd = pWnd;
	m_bSender = FALSE;

	m_iID = ID;
	m_pArchiveIn = NULL;
	m_pArchiveOut = NULL;
	m_bArchive = FALSE;

	m_dFileSize = 0;
	m_dFilePos = 0;
	m_bPaused = FALSE;
}

void CMyFileSocket::CreateArchives(CSocketFile *m_pSocketFile)
{
	if(m_pSocketFile != NULL)
	{
		m_pArchiveIn = new CArchive(m_pSocketFile,CArchive::load);
		m_pArchiveOut = new CArchive(m_pSocketFile,CArchive::store);
		m_bArchive = TRUE;
	}
	else
	{
		m_pArchiveIn = NULL;
		m_pArchiveOut = NULL;
		m_bArchive = FALSE;
	}
}

CMyFileSocket::~CMyFileSocket()
{
	//Finalize();
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CMyFileSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CMyFileSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CMyFileSocket member functions

void CMyFileSocket::OnAccept(int nErrorCode)
{
	//Server Only, function is called when a client wants to be Accepted
	CString strError;

	if(nErrorCode == 0)
	{
		((CFileShareDlg*)m_pWnd)->OnAccept();//Override:
	}
	else
	{
		//FIX:
		strError.Format("<!> Could not accept a client connection.\nClient will be dropped.\n Error #%i", nErrorCode);
		//((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage(strError);
	}
}

void CMyFileSocket::OnConnect(int nErrorCode)
{
	//Client only, called when server has accepted
	CString strError;
	if(nErrorCode == 0)//found server
	{
		((CFileShareDlg*)m_pWnd)->OnConnect();//Override:
	}
	else
	{
		strError.Format("Server does not exist. Check the IP address, and Port number.\n Error #%i", nErrorCode);
		((CFileShareDlg*)m_pWnd)->OnClose(-1);//Override:
		((CFileShareDlg*)m_pWnd)->m_strStatus.Format("Server does not exist. Check the IP address, and Port number.\n Error #%i", nErrorCode);
	}
}

void CMyFileSocket::OnClose(int nErrorCode)
{

	//Close connection
	CString strError;

	((CFileShareDlg*)m_pWnd)->m_bConnected = FALSE;
	if(nErrorCode == 0)
	{
		((CFileShareDlg*)m_pWnd)->OnClose(m_iID);
	}
	else
	{
		if(nErrorCode == WSAECONNABORTED)
		{
			//FIX:
			/*if(m_iID >= 0)
			{
				((CMIMDlg*)m_pWnd)->SetListIcon(2, ((CMIMDlg*)m_pWnd)->GetListPos(m_iID+1));
				strError.Format("<!> WARNING: Connection problem on socket %i.\nError #%i", m_iID+1, nErrorCode);
				((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage(strError);
				((CMIMDlg*)m_pWnd)->OnClose(m_iID);
			}*/
		}
	}
	//WARNING: DONT ADD CODE AFTER HERE BECAUSE CMyFileSocket WILL ALREADY HAVE
	//BEEN DETROYED AT THIS POINT "->OnClose();"
}

void CMyFileSocket::OnReceive(int nErrorCode)
{
	//DEBUG:
	//((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage("OnReceive()");

	if(nErrorCode != 0)
	{
		CString strError;
		strError.Format("Connection Problem: Could not receive data from server.\nError #", nErrorCode);

		//FIX:
		//if(m_iID >= 0)
		//	((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage(strError);
	}
	else
	{	//Receive stuff
		//ASSERT(((CMIMDlg*)m_pWnd));
		ParsePacket();//call here to use archive and retrieve it
	}
}

void CMyFileSocket::OnSend(int nErrorCode)
{
	//Any Errors?
	CString strError;

	/*if(nErrorCode == 0)
		((CMIMDlg*)m_pWnd)->OnSend();//useless so far
	else
	{
		strError.Format("Connection Problem: Cannot send data, you may have to close and re-open the connection.\nError #", nErrorCode);
		((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage(strError);
	}*/
}

/////////NEW FILE SUPPORT//////
void CMyFileSocket::StrCopy(char *strTo, char *strFrom, int iSize)
{
	//copy "ALL" ASCII characters to another string
	char *pBuf;
	pBuf = &strFrom[0];

	for(int i = 0;i < iSize;i++)
	{
		*pBuf = strFrom[i];//copy char position
		strTo[i] = *pBuf;//add 1 char
		pBuf++;//increment
	}
	//((CDialog*)((CMIMDlg*)m_pWnd))->MessageBox("StrCopy Complete");
}

double CMyFileSocket::SendFileInfo(CString strDir, CString strFile)
{
	//#1, almost first process in file transfer.
	//The socket class is given the file name and location here (only chance)
	//Sender only, sends the specified filename (code NEWFILE) to the client and waits for OK
	//(code OK) return from client.
	//After this the file names will be stored in m_fFile[UL].

	CString strMess;

	if((m_fFile[UL].Open(strDir+strFile, RB)) < 0)//error
	{
		//NOTE: DONT CLOSE FILE IF IT NEVER OPENED CORRECTLY!
		strMess.Format("Could not open file \"%s\"! It will be skipped.", strFile);
		//((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage(strMess);
		return -1;
	}
	else//send file name now
	{
		//Send code n, file name, and size to signal a new file
		///////////////////////////////////////////
		//output text to a file
		CPacket Packet;
		FileInf.dSize = m_fFile[UL].GetSize();		//give receiver the size of the file
		strcpy(FileInf.strName, LPCTSTR(strFile));	//give receiver name of file, not dir

		StrCopy(Packet.pBuf, (char*)&FileInf, sizeof(CFileInf));
		Packet.m_iLength = sizeof(CFileInf);
		Packet.m_iCode = FILEINF;
		Packet.m_iPos = -1;
		if(!((CFileShareDlg*)m_pWnd)->m_bConnected)
			return -2;

		if(SendPacket(Packet) != 0)//Send FILEINF, check for error sending
		{
			((CDialog*)m_pWnd)->MessageBox("Could not tell client file name and size.");
			return -1;
		}
		//((CDialog*)((CMIMDlg*)m_pWnd))->MessageBox("Sent file name and size.");
	}
	m_iPos = 0;
	((CFileShareDlg*)m_pWnd)->m_bAllowRec = FALSE;
	((CFileShareDlg*)m_pWnd)->m_bAllowSend = TRUE;
	return FileInf.dSize;//successful, return file size to the parent class
}

double CMyFileSocket::RecFile(CPacket &RPacket)
{
	//STEP #2
	//Sender has sent FILEINF (size+name), receiver sends back start position now (bytes/PACKETMAX-1)
	StrCopy((char*)&FileInf, RPacket.pBuf, sizeof(CFileInf));//convert pBuf to FileInf
	//((CDialog*)((CMIMDlg*)m_pWnd))->MessageBox("RecFile");

	CString strMess;	
	CString strName = FileInf.strName;
	m_dFileSize = FileInf.dSize;

	CString strPath = ((CFileShareDlg*)m_pWnd)->m_strDLto + strName;
	int choice = 0;
	int err = m_fFile[DL].Check(strPath);

	((CFileShareDlg*)m_pWnd)->RecFile(strName, m_dFileSize);//tell main window file attributes,
	//for progress bar range etc.
	//This is the only place where these vars should be set to 0
	m_dFilePos = 0;
	m_iPos = 0;

	if(err == 1)//file exists
	{
		//Overwrite prompt:
		m_fFile[DL].Close();//close test file
		strMess.Format("The file \"%s\" already exists. Do you want to replace it?", strName);
		choice = ((CDialog*)m_pWnd)->MessageBox(strMess, "Overwrite file", MB_YESNOCANCEL | MB_ICONEXCLAMATION);
		if(choice == IDNO)
		{
			m_dFilePos = -10;//let user know the file was cancelled
			if(((CFileShareDlg*)m_pWnd)->m_bConnected)
				SendPacket(FILEDONE);	
			FileDone();//skip to next file
			m_dFilePos = -10;//Repeat because it would be reset to 0 after filedone()
			return -1;
		}
		else if(choice == IDCANCEL)
		{
			SendPacket(STOP);//tell other side too
			((CFileShareDlg*)m_pWnd)->Cancel();
			StopStreams();		//Close open streams
			m_dFilePos = -10;//let user know the file was cancelled
			return -3;
		}

		//otherwise search for position to begin at
		if(((CDialog*)m_pWnd)->MessageBox("If the file is exactly the same but incomplete,\nyou may choose to continue append data to the end. Do you wish to do this?","Finish Incomplete File",MB_ICONQUESTION | MB_YESNO)==IDYES)
		{
			//find pos to start at
			m_dFilePos = m_fFile->GetSize(strPath);
			if(m_dFileSize <= m_dFilePos)//file is already done
			{
				m_dFilePos = -10;
				((CFileShareDlg*)m_pWnd)->m_strStatus = "File already complete, skipping to next file...";
				((CFileShareDlg*)m_pWnd)->UpdateData(FALSE);

				if(((CFileShareDlg*)m_pWnd)->m_bConnected)
					SendPacket(FILEDONE);
				FileDone();//skip to next file
				return -1;
			}
			m_dFilePos -= ((PACKETMAX-1)*100);//Move 100 packets backward for safety
			if(m_dFilePos < 0)
				m_dFilePos = 0;
			m_iPos = (int)m_dFilePos/(PACKETMAX-1);

			strMess.Format("Starting from position %.0fbyte (packet %i)", m_dFilePos, m_iPos);
			((CDialog*)m_pWnd)->MessageBox(strMess);
		}
	}
	else if(err == -2)
	{
		strMess.Format("Bad flags: \"%s\".", m_fFile[DL].GetFlags());
		((CDialog*)m_pWnd)->MessageBox(strMess, "Load file", MB_ICONSTOP);
	}

	//Open file for writing, send 'O' and wait for first stream packet.
	if((m_fFile[DL].Open(strPath, WB)) < 0)
	{
		strMess.Format("Could not create file \"%s\" (%s) for writing! It will be skipped.", strPath, m_fFile[DL].GetFlags());
		((CDialog*)m_pWnd)->MessageBox(strMess, "Load file", MB_ICONSTOP);
		SendPacket(FILEDONE);
		FileDone();//skip to next file
		return -1;
	}
	else
	{
		//Start process
		CPacket SPacket;
		SPacket.Clear();
		SPacket.m_iCode = OK;
		SPacket.m_iPos = m_iPos;//Beginning position (might be appending)

		SendPacket(SPacket);//'o'
	}
	return 0;
}

int CMyFileSocket::StopStreams()
{
	//Cancel share, close streams
	((CFileShareDlg*)m_pWnd)->m_bAllowSend = FALSE;
	((CFileShareDlg*)m_pWnd)->m_bAllowRec = FALSE;
	m_fFile[UL].Close();//UL
	m_fFile[DL].Close();//DL

	return 0;
}

int CMyFileSocket::SendPacket(int iCode)
{
	if(!m_bArchive)
		return -1;

	CPacket NPacket;
	NPacket.Clear();
	NPacket.m_iCode = iCode;
	SendPacket(NPacket);
	return 0;
}

int CMyFileSocket::SendPacket(CPacket &SPacket)
{	
	if(!m_bArchive)
		return -1;

	//DEBUG:
	//m_strTemp.Format("SendPacket() | CODE: %i | TEXT: %s | Length: %i",SPacket.m_iCode,SPacket.pBuf,SPacket.m_iLength+1);
	//((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage(m_strTemp);

	//AssemblePkg(pkg);
	try
	{
		SPacket.Serialize(*m_pArchiveOut);
		m_pArchiveOut->Flush();
	}
	catch(CFileException* e)
	{
		TCHAR lpszError[255];
		e->GetErrorMessage(lpszError,255);
		((CDialog*)m_pWnd)->MessageBox(lpszError, "File Streaming Error", MB_ICONSTOP);
		//exit(0);
		return -1;
	}
	return 0;

}

int CMyFileSocket::ParsePacket()
{

	//Convert received data to CPacket then call appropriate functions
	if(!m_bArchive)
	{
		//FIX:
		//((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage("<!> ParsePacket(): Archive not open.");
		return -1;
	}

	CPacket RPacket;
	try
	{
		do
		{
			RPacket.Serialize(*m_pArchiveIn);
			//DEBUG:
			//m_strTemp.Format("ParsePkg() | CODE: %i | TEXT: %s | Length: %i",RPacket.m_iCode,RPacket.pBuf,RPacket.m_iLength);
			//((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage(m_strTemp);
			
			//Functions for file sending
			if(RPacket.m_iCode == FREG)
			{
				if(m_iID < 0)//fileshare
					((CFileShareDlg*)m_pWnd)->m_dTotal = int(RPacket.pBuf[0]);//tell receiver total number of files to receive
				OnConnect(0);//Init file sharing stuff, or MIM stuff
			}
			else if(RPacket.m_iCode == FILEINF)
				RecFile(RPacket);
			else if(RPacket.m_iCode == SFILE)
				WritePacket(RPacket);
			else if(RPacket.m_iCode == OK)
				StreamFile(RPacket);
			else if(RPacket.m_iCode == FILEDONE)
				FileDone();
			else if(RPacket.m_iCode == PAUSE)
				Pause(!m_bPaused);
			else if(RPacket.m_iCode == RWAITING)//receiver is ready
				((CFileShareDlg*)m_pWnd)->OnBSend();
		}
		while(!m_pArchiveIn->IsBufferEmpty());
	}
	catch(CFileException* e)
	{ 
		m_pArchiveIn->Abort();
	}
	return 0;
}

int CMyFileSocket::StreamFile(CPacket &RPacket)
{
	//STEP #3
	//FOR SENDER ONLY
	//called when receiver has name and file size, and returned starting position to sender here
	BOOL end = FALSE;
	CPacket FPacket;
	int iSize = 0;
	CString strMess;

	FPacket.Clear();
	//((CDialog*)m_pWnd)->MessageBox("Sending File packets now...");
	FPacket.m_iPos = RPacket.m_iPos;//This Position is the beginning of the cluster	

	for(;;)//Stream Loop
	{
		if(end)//if FINAL packet was sent last time
		{
			//((CMIMDlg*)m_pWnd)->MessageBox("<<File Done>>");
			break;//exit endless loop
		}
		else
		{
			//DEBUG:
			//strMess.Format("Pos: %i", FPacket.m_iPos);
			//((CDialog*)((CMIMDlg*)m_pWnd))->MessageBox(strMess);
			//Receiver tells sender where to start, might be appending
									
			//Add cluster position 'c' when reading to send packet
			fseek(m_fFile[UL].GetFile(), (FPacket.m_iPos*(PACKETMAX-1)), SEEK_SET);

			for(int i = 0;i < (PACKETMAX-1);i++)
			{
				if(feof(m_fFile[UL].GetFile()) != 0)
				{
					//strMess.Format("EOF found at %i(T: %i).", i, ftell(m_fFile[UL].GetFile()));
					//((CDialog*)((CMIMDlg*)m_pWnd))->MessageBox(strMess);

					end = TRUE;//Static bool will call CheckSend("D") on next receive of 'O'
					i--;
					break;//exit i loop
				}
				else
					FPacket.pBuf[i] = getc(m_fFile[UL].GetFile());
			}
			iSize = i;
			FPacket.m_iCode = SFILE;
			FPacket.m_iLength = iSize;
			//DEBUG:
			//strMess.Format("Sending packet: %i\nSize: %i", FPacket.m_iPos, iSize);
			//((CDialog*)((CMIMDlg*)m_pWnd))->MessageBox(strMess);
			if(!((CFileShareDlg*)m_pWnd)->m_bConnected)//stop
			{
				((CDialog*)m_pWnd)->MessageBox("Not Connected");
				return 0;
			}

			if(SendPacket(FPacket) == -1)//send packet to receiver
			{
				((CDialog*)m_pWnd)->MessageBox("Streaming cancelled");
				break;//stream closed or canceled;
			}
			//m_strStatus.Format("%.0f/%.0f bytes   (%.0fKB)", m_dFilePos, FileInf.dSize, m_dFilePos/1024);
			((CFileShareDlg*)m_pWnd)->m_iStep++;
			FPacket.m_iPos++;
			m_dFilePos += i;
			if(((CFileShareDlg*)m_pWnd)->m_iStep >= ((CFileShareDlg*)m_pWnd)->m_iTriggerStep)
				((CFileShareDlg*)m_pWnd)->StepRate();

			//MessageBox("Packet Sent");
		}//repeat loop until entire cluster is sent, then wait for 'o' again, or 'e' to resend all
	}
	m_fFile[UL].Close();//UL

	SendPacket(FILEDONE);//tell client file is done now
	FileDone();

	return 0;
}

int CMyFileSocket::WritePacket(CPacket &RPacket)
{
	//Receiver writes the received packet to the open file

	if(m_dFilePos > m_dFileSize)
	{
		((CDialog*)m_pWnd)->MessageBox("Writing overflow.");
		return -1;
	}

	//((CFileShareDlg*)m_pWnd)->m_iIdleCount = 0;
	if(!m_fFile[DL].IsOpen())
	{
		//((CDialog*)((CMIMDlg*)m_pWnd))->MessageBox("No file open for WritePacket();");
		SendPacket(STOP);
		StopStreams();
		return -1;
	}
	fseek(m_fFile[DL].GetFile(), RPacket.m_iPos*(PACKETMAX-1), SEEK_SET);

	for(int i = 0;i < RPacket.m_iLength-1;i++)
		putc(RPacket.pBuf[i], m_fFile[DL].GetFile());
	
	m_dFilePos += i;
	((CFileShareDlg*)m_pWnd)->m_iStep++;
	if(((CFileShareDlg*)m_pWnd)->m_iStep >= ((CFileShareDlg*)m_pWnd)->m_iTriggerStep)
		((CFileShareDlg*)m_pWnd)->StepRate();
	
	//DEBUG:	
	//m_strTemp.Format("Wrote iPos: %i\nSize: %i", RPacket.m_iPos, RPacket.m_iLength);
	//((CFileShareDlg*)m_pWnd)->MessageBox(m_strTemp);

	return 0;
}

int CMyFileSocket::FileDone()
{
	//Steps #7 AND #8 continued
	//((CDialog*)((CMIMDlg*)m_pWnd))->MessageBox("File done!");
	//Closes File streams and calls MainDlg FileDone()
	//File has finished downloading, client only
	m_fFile[DL].Close();
	m_fFile[UL].Close();//UL
	//DEBUG:
	//((CDialog*)((CMIMDlg*)m_pWnd))->MessageBox("A file done");
	m_iPos = 0;
	//Go back to parent class:
	((CFileShareDlg*)m_pWnd)->FileDone();
	return 0;
}

void CMyFileSocket::Pause(BOOL bState)
{
	m_bPaused = bState;
	if(m_bSender && !m_bPaused)//resume loop
		StreamFile(m_PausedPacket);
}

void CMyFileSocket::ClearPacket(CPacket &EPacket)
{
	for(int i = 0;i < PACKETMAX;i++)
		EPacket.pBuf[i] = 0;
}

void CMyFileSocket::Finalize()
{
	//clear all objects
	//m_bConOpen = FALSE;
	m_fFile[DL].Close();//close any open file
	m_fFile[UL].Close();//close any open file

	if(m_pArchiveIn != NULL)
	{
		delete m_pArchiveIn;
		m_pArchiveIn = NULL;
	}
	if(m_pArchiveOut != NULL)
	{
		delete m_pArchiveOut;
		m_pArchiveOut = NULL;
	}
}

///////ERROR MESSAGES/////////
int CMyFileSocket::GetError()
{
	//Only call this function if the returned value from a command is SOCKET_ERROR.
	//Calls the GetLastError() WinSock function and displays the error in a messagebox if found.
	//ID is returned in case you want to add more error handling.

	int iErr = CMyFileSocket::GetLastError();
	CString strErr;
	switch(iErr)
	{
	case WSANOTINITIALISED:
		strErr = "A successful AfxSocketInit must occur before using this API.";
		break;

	case WSAENETDOWN:
	   strErr = "The Windows Sockets implementation detected that the network subsystem failed.";
		break;

	case WSAEACCES:
	   strErr = "The requested address is a broadcast address, but the appropriate flag was not set.";
		break;

	case WSAEINPROGRESS:
	   strErr = "A blocking Windows Sockets operation is in progress.";
		break;

	case WSAEFAULT:
	   strErr = "The lpBuf argument is not in a valid part of the user address space.";
		break;

	case WSAENETRESET:
	   strErr = "The connection must be reset because the Windows Sockets implementation dropped it.";
		break;

	case WSAENOBUFS:
	   strErr = "The Windows Sockets implementation reports a buffer deadlock.";
		break;

	case WSAENOTCONN:
	   strErr = "The socket is not connected.";
		break;

	case WSAENOTSOCK:
	   strErr = "The descriptor is not a socket.";
		break;

	case WSAEOPNOTSUPP:
	   strErr = "MSG_OOB was specified, but the socket is not of type SOCK_STREAM.";
		break;

	case WSAESHUTDOWN:
	   strErr = "The socket has been shut down; it is not possible to call Send on a socket after ShutDown has been invoked with nHow set to 1 or 2.";
		break;

	case WSAEWOULDBLOCK:
	   strErr = "The socket is marked as nonblocking and the requested operation would block.";
		break;

	case WSAEMSGSIZE:
	   strErr = "The socket is of type SOCK_DGRAM, and the datagram is larger than the maximum supported by the Windows Sockets implementation.";
		break;

	case WSAEINVAL:
	   strErr = "The socket has not been bound with Bind.";
		break;

	case WSAECONNABORTED:
	   strErr = "The virtual circuit was aborted due to timeout or other failure.";
		break;

	case WSAECONNRESET:
	   strErr = "The virtual circuit was reset by the remote side.";
	   break;

	default:
	   strErr = "Unkown error occured.";
	   break;
	}
	//if(((CMIMDlg*)m_pWnd)->m_bLog)
	//	fprintf(((CMIMDlg*)m_pWnd)->m_fLog.GetFile(), LPCTSTR(strErr));

	((CDialog*)m_pWnd)->MessageBox(strErr, "WinSock error", MB_OK | MB_ICONSTOP);

	return iErr;
}
