#pragma once

// CServerEntry command target

class CServerEntry : public CObject
{
public:
	CServerEntry();
	CServerEntry(CServerEntry *Item);
	CServerEntry(CString strIP, int iPort);
	virtual ~CServerEntry();

	void Serialize(CArchive &ar);
	void Reset();
	CString m_strName;//name of command
	CString m_strVer;	//needs parameters? FALSE for vars that can be set by user with 1 parameter
	CString m_strIP;//IP address of server
	int m_iPort;	//port of server
	int m_iPassword;//0=none,1=pass,2=bouncer
	int m_iUsers;	//Number of users in server
	int m_iMax;		//Max number of users in server
	int m_iStatus;	//Info about connection status, etc. 0 =unknown, 1 = good, 2 = dead, 3 = connected but wont respond
	bool m_bCustom;	//TRUE if this server was added by the user and should not be deleted when refreshing fro mthe master list

protected:
   DECLARE_SERIAL(CServerEntry)
};
