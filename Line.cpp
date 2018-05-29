// Line.cpp: implementation of the CLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIM.h"
#include "Line.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLine::CLine()
{

}

CLine::CLine(int iThick, COLORREF clr)
{
	m_iThick = iThick;
	m_clr = clr;
}

/*CLine::CLine(int p1x,int p1y,int p2x,int p2y)
{
	P1.x = p1x;
	P1.y = p1y;
	P2.x = p2x;
	P2.y = p2y;
}*/

CLine::~CLine()
{
	m_Points.RemoveAll();
}

CMyPoint* CLine::GetPoint(int index)
{
	//return item at index, pointer
	return (CMyPoint*)m_Points[index];
}
