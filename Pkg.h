// Pkg.h: interface for the CPkg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PKG_H__1A459735_62A5_480E_87D5_6CE679BC96EC__INCLUDED_)
#define AFX_PKG_H__1A459735_62A5_480E_87D5_6CE679BC96EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPkg
{
protected:
	//DECLARE_DYNCREATE(CPkg)
public:	
	CPkg();
	CPkg(CHARFORMAT &cf, CString strText1 = "");
	//SIEZOF = 960
	BOOL bBold;			//5
	BOOL bItalic;		//6
	BOOL bUnderLine;	//7	
	BOOL bStrikeOut;	//8	

	COLORREF clrText;	//9

	int fontSize;		//11
	char fontName[30];	//10

	char strText[PACKETMAX-PACKETGAP];	//could be any length, must be last variable in class!!

// Attributes
public:
	//CPkg();     
public:

// Implementation
/*public:
	void Init();

	virtual ~CPkg();
*/
	// Generated message map functions
};


#endif // !defined(AFX_PKG_H__1A459735_62A5_480E_87D5_6CE679BC96EC__INCLUDED_)
