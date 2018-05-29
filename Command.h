// Command.h: interface for the CCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMAND_H__FF928615_A7D3_4F9B_88AD_CA0665A6BA7D__INCLUDED_)
#define AFX_COMMAND_H__FF928615_A7D3_4F9B_88AD_CA0665A6BA7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCommand : public CObject
{
public:
	CCommand();
	CCommand(CCommand *Item);
	virtual ~CCommand();

	void Reset();
	CString strCommand;	//name of command
	CString strHelp;	//help text (up to 150 chars?)
	BOOL bPara;		//needs parameters? FALSE for vars that can be set by user with 1 parameter
					//Only TRUE for commands that need parameters
	BOOL bEdit;		//allow edit/use while connected?
	int dType;		//var type: -1=NONE, 0=str, 1=int/BOOL, 2=double, 3=custom(not supported yet)
	int iUpper;		//Upper Bound allowable value
					//Use NONE if its a command, use other for vars
	int *iVal;		//address of int value (for int/bool vars only)
	CString* strVal;//address of string value (for string vars only)

	//m_Commands[MAXCONSOLE];//predefined commands
};

#endif // !defined(AFX_COMMAND_H__FF928615_A7D3_4F9B_88AD_CA0665A6BA7D__INCLUDED_)
