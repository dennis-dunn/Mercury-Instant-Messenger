// Request.h: interface for the CRequest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REQUEST_H__8E913BFC_490F_4B69_BE01_C038E99F92A8__INCLUDED_)
#define AFX_REQUEST_H__8E913BFC_490F_4B69_BE01_C038E99F92A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//This class may be used for other information such as whispering, or any command that requires
//more than just a string and int in pBuf.

class CRequest  
{
public:
	BOOL m_bFirst;			//True if first time connecting
	double m_dVer;			//Version of client/Server
	char m_strPassword[16];	//Password from client
	char m_strName[30];		//Name of Client OR Server Room
	int m_iID;				//Assigned ID for client
	//all vars after this will work in version 3.10, but older versions will not be able to recieve them
	int m_iUsers;	//Number of users in server
	int m_iMax;		//Max number of users possible within server
};

#endif // !defined(AFX_REQUEST_H__8E913BFC_490F_4B69_BE01_C038E99F92A8__INCLUDED_)
