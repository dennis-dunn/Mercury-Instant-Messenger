// Console.h: interface for the CConsole class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONSOLE_H__229E0CD9_7F28_4C1D_8521_1AD6252EFFD0__INCLUDED_)
#define AFX_CONSOLE_H__229E0CD9_7F28_4C1D_8521_1AD6252EFFD0__INCLUDED_

#include "Command.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CConsole  
{
public:
	void LoadCustom();
	int m_iTotalCom;
	int DeleteAllComm();
	int DeleteComm(int nItem);
	CObArray m_oaCommands;
	CStringArray strCommand;
	CString CombinePara(int iStart);
	double m_dVer;
	int Tally();
	int Vote(CString strDisp, int iChoices);
	BOOL m_bAccepted;	//Server accepted connection
	BOOL m_bConnected;	//Online connection, (if server only true when m_Users.iTotalUsers > 0)
	int Init();			//predefine m_Commands[MAXCONSOLE]
	BOOL m_bHelp;		//Show help for command
	BOOL m_bVoteActive;	//Vote in progress
	double m_dVoted;	//Total votes from users
	double m_dTally[4];	//total for each choice
	int m_iChoices;		//total choices

	CConsole();
	virtual ~CConsole();

	CString m_strReturn;

	void SetParent(CDialog *pWind);
	int Console(CString strLine, BOOL bRemote = FALSE);	//Run command through console
	CHARFORMAT DisplayMessage(CString strMess, COLORREF clrText = RGB(164,176,238), BOOL bRet = TRUE);//Display text on screen
	int MakeNum(char *string, int min = 0, int max = ID_MAX, int iStart = 0);//convert string to int
	int MakeNum(CString string, int min = 0, int max = ID_MAX, int iStart = 0);//convert string to int
	int Kick(int ID, BOOL bShow = TRUE);	//Admin: kick client
	int Ban(int ID, BOOL bShow = TRUE);	//Admin: ban client
	int User(int ID);	//Admin: run command on client
	int Ping(int ID);

private:

	int AddCommand(CCommand *oItem, BOOL clear = TRUE);
	CCommand * GetComm(int index);

	//CMIMDlg *m_pWnd;
	CDialog *m_pWnd;
	//CSaved *m_SavedData;
};

#endif // !defined(AFX_CONSOLE_H__229E0CD9_7F28_4C1D_8521_1AD6252EFFD0__INCLUDED_)
