// Packet.h: interface for the CPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKET_H__0D7F8D98_CFE3_4B29_9418_667D47FDBBF1__INCLUDED_)
#define AFX_PACKET_H__0D7F8D98_CFE3_4B29_9418_667D47FDBBF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPacket  
{
public:
	//CPacket();
	//virtual ~CPacket();
	void Clear();
	void Serialize(CArchive &ar);

	int m_iPos;
	int m_iLength;	//Length of pBuf!! not entire packet. Important that you use this size with StrCpy()!!
	int m_iCode;
	char pBuf[PACKETMAX];//use (PACKETMAX-1) for message, and last space for null terminator
};

#endif // !defined(AFX_PACKET_H__0D7F8D98_CFE3_4B29_9418_667D47FDBBF1__INCLUDED_)
