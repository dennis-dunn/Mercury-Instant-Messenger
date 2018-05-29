// MIM.h : main header file for the MIM application
//

#if !defined(AFX_MIM_H__9D3741CF_0A96_4D3A_8730_62CED3CE4894__INCLUDED_)
#define AFX_MIM_H__9D3741CF_0A96_4D3A_8730_62CED3CE4894__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "MIM_i.h"

/////////////////////////////////////////////////////////////////////////////
// CMIMApp:
// See MIM.cpp for the implementation of this class
//

class CMIMApp : public CWinApp
{
public:
	CMIMApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMIMApp)
	public:
	virtual BOOL InitInstance();
		virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMIMApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bATLInited;
private:
	BOOL InitATL();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIM_H__9D3741CF_0A96_4D3A_8730_62CED3CE4894__INCLUDED_)
