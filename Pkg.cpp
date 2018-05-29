// Pkg.cpp: implementation of the CPkg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIM.h"
#include "Pkg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//IMPLEMENT_DYNCREATE(CPkg, CObject)

CPkg::CPkg()
{
}

CPkg::CPkg(CHARFORMAT &cf, CString strText1)
{
	//strcpy(pkg.strText, strText);
	//m_wndInput.GetWindowText(pkg.strText );//1
	//strcpy(pkg.strName, m_strScreenName);	//2
	//pkg.bAway = bAway;					//3
	//pkg.iMyIcon = m_iMyIcon;				//4
	//pkg.bBold = m_bCharBold;				//5
	//pkg.bItalic  = m_bCharItalic;			//6
	//pkg.bUnderLine  = m_bCharUnderline;	//7
	//pkg.bStrikeOut = m_bCharStrikeOut;	//8
	clrText  = cf.crTextColor;			//9
	strcpy(fontName, cf.szFaceName);		//10
	
	//FIX:
	//pkg.fontSize  = m_iFontSize;			//11}

	cf.dwMask = CFM_COLOR | CFM_ITALIC | CFM_BOLD | CFM_UNDERLINE | CFM_STRIKEOUT | CFM_SIZE;

	if (!(cf.dwEffects & CFM_BOLD))
		bBold = FALSE;
	if (!(cf.dwEffects & CFM_UNDERLINE))
		bUnderLine = FALSE;
	if (!(cf.dwEffects & CFM_STRIKEOUT))
		bStrikeOut = FALSE;
	if (!(cf.dwEffects & CFM_ITALIC))
		bItalic = FALSE;
	
	fontSize = cf.yHeight / 20;
	strcpy(strText, LPCTSTR(strText1));
	//::lstrcpy(cf.szFaceName, "Lucida Console");
}

/*CPkg::~CPkg()
{

}*/


/////////////////////////////////////////////////////////////////////////////
// CPkg diagnostics
/*
#ifdef _DEBUG
void CPkg::AssertValid() const
{
	CObject::AssertValid();
}

void CPkg::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPkg serialization

void CPkg::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar << strText ;
		ar << strName ;
		ar << bAway ;
		ar << iMyIcon ;
		ar << bBold ;
		ar << bItalic ;
		ar << bUnderLine ;
		ar << bStrikeOut ;
		ar << clrText ;
		ar << fontName ;
		ar << fontSize ;

		ar << ipAddress;
		ar << port;
		ar << request;
		ar << pSocket;

	}
	else
	{
		// TODO: add loading code here
		ar >> strText ;
		ar >> strName ;
		ar >> bAway ;
		ar >> iMyIcon ;
		ar >> bBold ;
		ar >> bItalic ;
		ar >> bUnderLine ;
		ar >> bStrikeOut ;
		ar >> clrText ;
		ar >> fontName ;
		ar >> fontSize ;

		ar >> ipAddress;
		ar >> port;
		ar >> request;
		ar >> pSocket;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPkg commands

void CPkg::Init()
{

	strText[0] = 0;
	//bAway = FALSE;
	//iMyIcon = 0;
	bBold = FALSE;
	bItalic = FALSE;
	bUnderLine = FALSE;
	bStrikeOut = FALSE;
	clrText = RGB(0,0,0);
	strcpy(fontName, "Times New Roman");
	fontSize = 10;

	ipAddress[0] = 0;
	//port = 0;
	//request = 0;//UNDEFINED
	//pSocket = NULL;
}
*/