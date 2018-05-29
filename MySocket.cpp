// MySocket.cpp: implementation of the CMySocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIM.h"
#include "MySocket.h"
#include "MIMDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//Use this socket class for sending packets, (for file sharing use CMyFileSocket)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMySocket::CMySocket(CDialog *pWnd, int ID)
{
	m_pWnd = pWnd;
	m_bSender = FALSE;

	m_iID = ID;
	m_iMod = FALSE;
	m_pArchiveIn = NULL;
	m_pArchiveOut = NULL;
	m_bArchive = FALSE;
	m_dFileSize = 0;
	m_dFilePos = 0;
}

void CMySocket::CreateArchives(CSocketFile *m_pSocketFile)
{
	if(m_pSocketFile != NULL)
	{
		Finalize();//erase previous archives if they have not been erased
		m_pArchiveIn = new CArchive(m_pSocketFile,CArchive::load);//create new archives
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

CMySocket::~CMySocket()
{
	Finalize();
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CMySocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CMySocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CMySocket member functions

void CMySocket::OnAccept(int nErrorCode)
{
	//Server Only, function is called when a client wants to be Accepted
	CString strError;

	if(nErrorCode == 0)
	{
		((CMIMDlg*)m_pWnd)->OnAccept();//Override:
	}
	else
	{
		strError.Format("<!> Could not accept a client connection.\nClient will be dropped.\n Error #%i", nErrorCode);
		((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage(strError);
	}
}

void CMySocket::OnConnect(int nErrorCode)
{
	//Client only, called when server has accepted
	CString strError;
	if(nErrorCode == 0)//found server
	{
		((CMIMDlg*)m_pWnd)->OnConnect();//Override:
	}
	else
	{
		strError.Format("Server does not exist. Check the IP address, and Port number.\n Error #%i", nErrorCode);
		((CMIMDlg*)m_pWnd)->OnCloseCon(-1);//Disconnect and stop Timer
		((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage(strError);
	}
}

void CMySocket::OnClose(int nErrorCode)
{
	//Close connection
	CString strError;
	((CMIMDlg*)m_pWnd)->m_Console.m_bConnected = FALSE;

	if(nErrorCode == 0)//normal close
		((CMIMDlg*)m_pWnd)->OnCloseCon(m_iID);
	else if(((CMIMDlg*)m_pWnd)->m_iType == CLIENT)//client only
	{
		strError.Format("<!> WARNING: Connection problem.\nError #%i", nErrorCode);
		((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage(strError,RED);
		((CMIMDlg*)m_pWnd)->OnCloseCon(0);
	}
	else//for server
	{
		if(nErrorCode == WSAECONNABORTED)//client closed connection somehow that sockets whine about
		{
			strError.Format("<!> Connection lost on socket %i.\nError #%i", m_iID+1, nErrorCode);
			((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage(strError,YELLOW);
			((CMIMDlg*)m_pWnd)->OnCloseCon(m_iID);
		}
		else
		{
			strError.Format("<!> WARNING: Connection problem on socket %i.\nError #%i", m_iID+1, nErrorCode);
			((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage(strError,RED);
			((CMIMDlg*)m_pWnd)->OnCloseCon(m_iID);
		}
	}
	//WARNING: DONT ADD CODE AFTER HERE BECAUSE CMySocket WILL ALREADY HAVE
	//BEEN DETROYED AT THIS POINT "->OnCloseCon();"
}

void CMySocket::OnReceive(int nErrorCode)
{
	//DEBUG:
	//((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage("OnReceive()");

	if(nErrorCode != 0)
	{
		CString strError;
		strError.Format("Connection Problem: Could not receive data from server.\nError #", nErrorCode);
		if(m_iID >= 0)
			((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage(strError);
	}
	else
	{	//Receive stuff
		ASSERT(((CMIMDlg*)m_pWnd));
		
		DWORD dwBytes;
		VERIFY(IOCtl(FIONREAD, &dwBytes));
		if (0 == dwBytes) // if nothing to read, return
			return;

		((CMIMDlg*)m_pWnd)->m_iByteRec += int(dwBytes);
		ParsePacket();//call here to use archive and retrieve it
	}
}

void CMySocket::OnSend(int nErrorCode)
{
	//Any Errors?
	CString strError;

	if(nErrorCode == 0)
		((CMIMDlg*)m_pWnd)->OnSend();//useless so far
	else
	{
		strError.Format("Connection Problem: Cannot send data, you may have to close and re-open the connection.\nError #", nErrorCode);
		((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage(strError);
	}
}

/////////NEW FILE SUPPORT//////
void CMySocket::StrCopy(char *strTo, char *strFrom, int iSize)
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
	//((CDialog*)m_pWnd)->MessageBox("StrCopy Complete");
}

int CMySocket::SendPacket(int iCode)
{
	if(!m_bArchive)
		return -1;

	CPacket NPacket;
	NPacket.Clear();
	NPacket.m_iCode = iCode;
	SendPacket(NPacket);
	return 0;
}

int CMySocket::SendPacket(CPacket &SPacket)
{	
	if(!m_bArchive)
		return -1;

	//DEBUG:
	//m_strTemp.Format("SendPacket() | CODE: %i | TEXT: %s | Length: %i",SPacket.m_iCode,SPacket.pBuf,SPacket.m_iLength+1);
	//((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage(m_strTemp);

	try
	{
		SPacket.Serialize(*m_pArchiveOut);
		m_pArchiveOut->Flush();
	}
	catch(CFileException* e)
	{
		TCHAR lpszError[255];
		e->GetErrorMessage(lpszError,255);
		CString strTemp;
		strTemp.Format("CFileException on Socket: %i\n%s", m_iID, lpszError);
		AfxMessageBox(LPCTSTR(strTemp));
		//exit(0);
		return -1;
	}
	((CMIMDlg*)m_pWnd)->m_iByteSent += (SPacket.m_iLength + sizeof(int)*3);
	return 0;
}

int CMySocket::ParsePacket()
{
	//Convert received data to CPacket then call appropriate functions
	if(!m_bArchive)
	{
		((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage("<!> ParsePacket(): Archive not open.");
		return -1;
	}

	CPacket RPacket;
	try
	{
		DWORD dwBytes;
		VERIFY(IOCtl(FIONREAD, &dwBytes));
		if (0 == dwBytes) // if nothing to read, return
			return -1;
		do
		{
			RPacket.Serialize(*m_pArchiveIn);
			((CMIMDlg*)m_pWnd)->m_iByteRec += (RPacket.m_iLength + sizeof(int)*3);//bandwidth tracker
			//DEBUG:
			//m_strTemp.Format("ParsePkg() | CODE: %i | TEXT: %s | Length: %i",RPacket.m_iCode,RPacket.pBuf,RPacket.m_iLength);
			//((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage(m_strTemp);

			//Forward to CMIMDlg function
			((CMIMDlg*)m_pWnd)->OnReceive(RPacket, m_iID);
		}
		while(!m_pArchiveIn->IsBufferEmpty());
	}
	catch(CFileException* e)
	{
		((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage("<!> ParsePacket(): FileException. Aborting...");
		m_pArchiveIn->Abort();
	}
	return 0;
}

void CMySocket::ClearPacket(CPacket &EPacket)
{
	for(int i = 0;i < PACKETMAX;i++)
		EPacket.pBuf[i] = 0;
}

void CMySocket::Finalize()
{
	//clear all objects
	//m_bConOpen = FALSE;

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
int CMySocket::GetError()
{
	//Only call this function if the returned value from a command is SOCKET_ERROR.
	//Calls the GetLastError() WinSock function and displays the error in a messagebox if found.
	//ID is returned in case you want to add more error handling.

	int iErr = CMySocket::GetLastError();
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
