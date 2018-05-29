// MySocket.h: interface for the CMySocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSOCKET_H__9295DAB2_E141_4C45_AFB0_CBE16A6B3694__INCLUDED_)
#define AFX_MYSOCKET_H__9295DAB2_E141_4C45_AFB0_CBE16A6B3694__INCLUDED_

//#include "MIMDlg.h"	// Added by ClassView
#include "Packet.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMySocket : public CSocket
{
public:
	CMySocket(CDialog* pWnd, int ID);
	virtual ~CMySocket();

// Attributes
	BOOL m_bArchive;					//Are archives allowed to send/rec packets?
	int ParsePacket();					//Convert received archive into a CPacket then calls functions
	int SendPacket(int iCode);			//Overloaded to accept just code
	int SendPacket(CPacket &SPacket);	//Send CPacket using open archive
	int m_iClientID;					//ID number of your socket, server assigned
	double m_dFilePos;					//Current packet position of streaming file
	double m_dFileSize;					//Size of file in bytes
	BOOL m_bUnknown;					//The command received was unknown
	BOOL m_bSender;						//Are you the sender? FALSE = client
	int m_iPort;						//Current IP port
	int m_iPos;							//Current packet position in file
	int m_iID;							//Current index in array
	int m_iMod;							//FOR SERVER: is the client a moderator?

	CPacket m_Packet;
	CArchive* m_pArchiveIn;				//Archive attached to CSocketFile for receiving streams
	CArchive* m_pArchiveOut;			//Archive attached to CSocketFile for sending streams

// Operations
	void CreateArchives(CSocketFile *m_pSocketFile);
										//Creates send and rec archives for sending data, needs socketfile pointer
	static void ClearPacket(CPacket &EPacket);	//Resets content of packet
	static void StrCopy(char *strTo, char *strFrom, int iSize);
										//copies binary text to a char*
	void SetJob(BOOL sender){m_bSender = sender;}
										//pass true for sender, false for receiver
	int GetError();						//Displays message box with last error
	

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySocket)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMySocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	virtual void OnAccept(int nErrorCode);//this means the function bodies dont even have to exist. Good for re-using the base class in multiple derived classes.
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);

private:
	CString m_strTemp;	//Temp string for inside functions
	void Finalize();	//delete archives and socketfile (called in destructor)
	CDialog *m_pWnd;	//Parent Window
};

#endif // !defined(AFX_MYSOCKET_H__9295DAB2_E141_4C45_AFB0_CBE16A6B3694__INCLUDED_)
