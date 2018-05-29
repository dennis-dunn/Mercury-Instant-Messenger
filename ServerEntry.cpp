// ServerEntry.cpp : implementation file
//

#include "stdafx.h"
#include "MIM.h"
#include "ServerEntry.h"


// CServerEntry

CServerEntry::CServerEntry()
{
	Reset();
}

CServerEntry::CServerEntry(CServerEntry *Item)
{
	m_strName = Item->m_strName;//name of Server
	m_strVer = Item->m_strVer;	//version
	m_strIP = Item->m_strIP;	//IP
	m_iPassword = Item->m_iPassword;//0=none,1=pass,2=bouncer
	m_iUsers = Item->m_iUsers;	//Number of users currently in server
	m_iMax = Item->m_iMax;
	m_iPort = Item->m_iPort;
	m_iStatus = Item->m_iStatus;
}

CServerEntry::CServerEntry(CString strIP, int iPort)
{
	Reset();
	m_strIP = strIP;	//IP
	m_iPort = iPort;
}

CServerEntry::~CServerEntry()
{
}


// CServerEntry member functions

void CServerEntry::Reset()
{
	m_strName.Empty();//name of command
	m_strVer.Empty();//needs parameters? FALSE for vars that can be set by user with 1 parameter
	m_strIP.Empty();//IP address of server
	m_iPassword = 0;//0=none,1=pass,2=bouncer
	m_iUsers = 0;;	//Number of users in server
	m_iMax = 0;		//Max number of users in server
	m_iPort = 0;
	m_iStatus = 0;
}

void CServerEntry::Serialize(CArchive &ar)
{
	//read/write all member variables to archive	
	if(ar.IsStoring())
	{//save
		ar << m_strName << m_strVer << m_strIP << m_iPassword << m_iUsers << m_iMax << m_iPort << m_iStatus;
	}
	else
	{//load
		ar >> m_strName >> m_strVer >> m_strIP >> m_iPassword >> m_iUsers >> m_iMax >> m_iPort >> m_iStatus;
	}
}