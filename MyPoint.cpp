// MyPoint.cpp: implementation of the CMyPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIM.h"
#include "MyPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMyPoint::CMyPoint(CPoint P)
{
	x = short(P.x);
	y = short(P.y);
}

/*CMyPoint::CMyPoint()
{
	x = 0;
	y = 0;
}

CMyPoint::~CMyPoint()
{

}
*/