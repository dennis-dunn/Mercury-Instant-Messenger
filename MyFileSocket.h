// MyFileSocket.h: interface for the CMyFileSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYFILESOCKET_H__4CBFE12F_7A18_49E7_A31C_29C07BE78619__INCLUDED_)
#define AFX_MYFILESOCKET_H__4CBFE12F_7A18_49E7_A31C_29C07BE78619__INCLUDED_

#include "Packet.h"	// Added by ClassView
#include "MyFile.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyFileSocket : public CSocket
{
public:
	CMyFileSocket(CDialog* pWnd, int ID);
	virtual ~CMyFileSocket();

// Attributes
	BOOL m_bArchive;					//Are archives allowed to send for sharing?
	int StreamFile(CPacket &RPacket);	//Continous loop streaming file packets (threads may be necessary)
	int WritePacket(CPacket &RPacket);	//Writes 1 packet of streamed file data 
	int ParsePacket();						//Convert received archive into a CPacket then calls functions
	int SendPacket(int iCode);			//Overloaded to accept just code
	int SendPacket(CPacket &SPacket);		//Send CPacket using open archive
	int m_iClientID;					//ID number of your socket, server assigned
	double m_dFilePos;					//Current packet position of streaming file
	double m_dFileSize;					//Size of file in bytes
	BOOL m_bUnknown;					//The command received was unknown
	BOOL m_bSender;						//Are you the sender? FALSE = client
	int m_iPort;						//Current IP port
	int m_iPos;							//Current packet position in file
	int m_iID;							//Current index in array

	CPacket m_Packet;
	CArchive* m_pArchiveIn;				//Archive attached to CSocketFile for receiving streams
	CArchive* m_pArchiveOut;			//Archive attached to CSocketFile for sending streams

// Operations
	BOOL IsPaused(){return m_bPaused;}	//Returns whether the transfer is paused or not
	void Pause(BOOL bState);			//Sets pause value, if true resumes StreamFile()
	void CreateArchives(CSocketFile *m_pSocketFile);
										//Creates send and rec archives for sending data, needs socketfile pointer
	void ClearPacket(CPacket &EPacket);	//Resets content of packet
	void StrCopy(char *strTo, char *strFrom, int iSize);
										//copies binary text to a char*
	void SetJob(BOOL sender){m_bSender = sender;}
										//pass true for sender, false for receiver
	int StopStreams();					//Closes file streams
	int FileDone();						//Called when current file is done transferring
	int GetError();						//Displays message box with last error
	double SendFileInfo(CString strDir, CString strFile);//tells other side the filename and size (code=FILEINF)
	double RecFile(CPacket &RPacket);	//Called for client when ready to receive a file
	
	//This class stores the name and size of the file that is currently DL'ing
	class CFileInf
	{
		public:
			char strName[500];
			double dSize;
	}FileInf;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFileSocket)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMyFileSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	virtual void OnAccept(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);

private:
	CPacket m_PausedPacket;
	BOOL m_bPaused;						//Has the transfer been paused?
	CString m_strTemp;	//Temp string for inside functions
	void Finalize();	//delete archives and socketfile (called in destructor)
	CMyFile m_fFile[2];	//(ul or dl)
	CDialog *m_pWnd;	//Parent Window
};
#endif // !defined(AFX_MYFILESOCKET_H__4CBFE12F_7A18_49E7_A31C_29C07BE78619__INCLUDED_)
