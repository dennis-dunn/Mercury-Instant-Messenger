// Line.h: interface for the CLine class.
//
//////////////////////////////////////////////////////////////////////
#include "MyPoint.h"	// Added by ClassView

#if !defined(AFX_LINE_H__9624B9CA_E5A7_4D98_B0B0_C3C6FEA86F5C__INCLUDED_)
#define AFX_LINE_H__9624B9CA_E5A7_4D98_B0B0_C3C6FEA86F5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLine : public CObject
{
public:
	CMyPoint* GetPoint(int index);
	CObArray m_Points;
	COLORREF m_clr;
	int m_iThick;

	CLine();
	CLine(int iThick, COLORREF clr);
	virtual ~CLine();
};

#endif // !defined(AFX_LINE_H__9624B9CA_E5A7_4D98_B0B0_C3C6FEA86F5C__INCLUDED_)
