// Command.cpp: implementation of the CCommand class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIM.h"
#include "Command.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommand::CCommand()
{
	Reset();
}

void CCommand::Reset()
{
	strCommand.Empty();	//clear name
	strHelp.Empty();	//clear description
	bEdit = FALSE;		//default, cannot edit while connected
	dType = 0;			//default to string type
	strVal = NULL;		//pointer to a string variable
	iVal = NULL;		//pointer to an int/BOOL variable
	iUpper = 1;			//max int value for single paremeter (default of 1 indicates BOOLEAN)
	bPara = FALSE;		//Doesn't need parameters (for variables)
}

CCommand::CCommand(CCommand *Item)
{
	strCommand = Item->strCommand;
	strHelp = Item->strHelp;
	bEdit = Item->bEdit;
	dType = Item->dType;
	strVal = Item->strVal;
	iVal = Item->iVal;
	iUpper = Item->iUpper;
	bPara = Item->bPara;
}

CCommand::~CCommand()
{

}
