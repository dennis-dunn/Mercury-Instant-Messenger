// MyPoint.h: interface for the CMyPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPOINT_H__788EDD32_F029_4341_A476_C465C9508DAD__INCLUDED_)
#define AFX_MYPOINT_H__788EDD32_F029_4341_A476_C465C9508DAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyPoint : public CObject
{
public:
	CMyPoint(CPoint P);
	short x;
	short y;
};

#endif // !defined(AFX_MYPOINT_H__788EDD32_F029_4341_A476_C465C9508DAD__INCLUDED_)
