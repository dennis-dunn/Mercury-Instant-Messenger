// ShareInfo.h: interface for the CShareInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAREINFO_H__8E8BCB2A_C86E_42D2_8FF6_527BC42EBDE6__INCLUDED_)
#define AFX_SHAREINFO_H__8E8BCB2A_C86E_42D2_8FF6_527BC42EBDE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CShareInfo
{
public:
	//Info to send to other side
	char m_strName[30];	//name
	char m_strIP[60];	//IP of sender
	char m_strIP2[60];	//IP of receiver
	BOOL m_bSender;		//m_bSender
	BOOL m_bRec;		//m_bRec
	BOOL m_bSaveAs;		//Whether or not to display the save as dialog in FM.exe
	BOOL m_bListenAble;	//this computer is capable of being the server
	BOOL m_bYouConnect;	//whether you should connect or not
	int m_iPort;		//Port of sender
	int m_iTotal;		//total # files
	int m_iReq;			//ID of requested connection
};

#endif // !defined(AFX_SHAREINFO_H__8E8BCB2A_C86E_42D2_8FF6_527BC42EBDE6__INCLUDED_)
