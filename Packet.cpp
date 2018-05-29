// Packet.cpp: implementation of the CPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIM.h"
#include "Packet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CPacket::Clear()
{
	m_iPos = -1;
	m_iLength = 1;
	m_iCode = 0;
	for(int i =0;i < PACKETMAX;i++)
		pBuf[i] = 0;
}

void CPacket::Serialize(CArchive &ar)
{
	//The null terminator is added here
	m_iLength++;//NO!!

	if(ar.IsStoring())
	{//save
		ASSERT((m_iLength > 0) && (m_iLength < PACKETMAX));
		pBuf[m_iLength-1] = 0;//Null terminate
		ar << m_iPos << m_iLength << m_iCode;
		ar.Write(pBuf,m_iLength);

		//for(int i = 0;i < m_iLength;i++)
		//	ar << pBuf[i];
	}
	else
	{//load
		ar >> m_iPos >> m_iLength >> m_iCode;

		ASSERT((m_iLength > 0)&&(m_iLength < PACKETMAX));
		ar.Read(pBuf,m_iLength);
		//for(int i = 0;i < m_iLength;i++)
		//	ar >> pBuf[i];
		ASSERT(pBuf[m_iLength-1] == 0);//NULL terminate
		m_iLength--;//Decrease length back to original
	}
}
