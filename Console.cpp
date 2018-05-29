// Console.cpp: implementation of the CConsole class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIM.h"
#include "Console.h"
#include "Request.h"
#include "MIMDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//WARNING: Must call SetParent() and Init() before using CConsole

CConsole::CConsole()
{

}

CConsole::~CConsole()
{

}

int CConsole::Init()
{
	//INIT all command variables. Set names, addresses of actual vars,
	//var types, para requirements, allow edit during connection, etc.
	int i = 0;

	m_bHelp = FALSE;
	m_bVoteActive = FALSE;
	m_bConnected = FALSE;

	i = 0;
	//Commands alphabetical!
	CCommand Item;
	Item.strCommand = "ADDBAN";
	Item.strHelp = "Usage: /addban \"address\"\nDescription: Adds the IP address to the ban list. See also BAN.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "ADDFILE";
	Item.strHelp = "Usage: /addfile \"filepath\"\nDescription: Adds the file to the file list.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "ADDIP";
	Item.strHelp = "Usage: /ip \"address\" \"comment\"\nDescription: Adds an IP address to the saved list. Optional comment.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "ADMINNAME";
	Item.strHelp = "Description: Sets the admin's name.";
	Item.strVal = &((CMIMDlg*)m_pWnd)->m_strAdminName;
	Item.bEdit = TRUE;
	AddCommand(&Item);//1

	Item.strCommand = "ALLOWRUN";
	Item.strHelp = "Usage: /allowrun [bool]\nDescription: Toggle whether admin is allowed to run commands on the client's computer.";
	Item.iVal = (int*)&((CMIMDlg*)m_pWnd)->m_bAllowRun;	
	Item.dType = 1;//integer
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "AUTOLOGIN";
	Item.strHelp = "Usage: /autologin [BOOL]\nDescription: Set auto login mode.\n[1 = ON, 0 = OFF]";
	Item.iVal = (int*)&((CMIMDlg*)m_pWnd)->m_bAutoLogin;
	Item.dType = 1;//integer
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "AUTOSAVE";
	Item.strHelp = "Usage: /autosave [BOOL]\nDescription: Set auto save message history.\n[1 = ON, 0 = OFF]";
	Item.iVal = (int*)&((CMIMDlg*)m_pWnd)->m_bAutoSaveMess;
	Item.dType = 1;//integer
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "BAN";
	Item.strHelp = "Usage: /ban [ID]\nDescription: Adds the IP address of the user (ID) to the ban list. See also ADDBAN.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "BOUNCER";
	Item.strHelp = "Usage: /bouncer [bool]\nDescription: Sets whether the server should use the bouncer.txt list to filter incoming users.";
	Item.iVal = (int*)&((CMIMDlg*)m_pWnd)->m_bBouncer;
	Item.dType = 1;//integer
	Item.iUpper = 1;//BOOL 0-1
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "BOUNCERINT";
	Item.strHelp = "Usage: /bouncerint [bool]\nDescription: Interval in seconds between bouncer.txt update.";
	Item.iVal = (int*)&((CMIMDlg*)m_pWnd)->m_iBounceSec;
	Item.dType = 1;//integer
	Item.iUpper = 10000;//int
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "BROWSER";
	Item.strHelp = "Description: Sets the internet browser used for opening hyperlinks (exe file).";
	Item.strVal = &((CMIMDlg*)m_pWnd)->m_strBrowser;
	Item.bEdit = TRUE;
	AddCommand(&Item);//2

	Item.strCommand = "CENSOR";
	Item.strHelp = "Usage: /censor [int]\nDescription: Enables or disables sound (0 = OFF, 1 = ON)";
	Item.iVal = (int*)&((CMIMDlg*)m_pWnd)->m_iCensor;	
	Item.dType = 1;//int
	Item.iUpper = 3;//max
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "CLEARBAN";
	Item.strHelp = "Usage: /clearban \nDescription: Clears the ban list.";
	Item.dType = -1;//Not a var, command
	Item.bPara = FALSE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "CLEARFILE";
	Item.strHelp = "Usage: /clearfile \nDescription: Clears the file list.";
	Item.dType = -1;//Not a var, command
	Item.bPara = FALSE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "CLOSE";
	Item.strHelp = "Usage: /CLOSE\n/DISCONNECT\nDescription: Closes the current connection.";
	Item.dType = -1;//Not a var, command
	Item.bPara = FALSE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "CMD";
	Item.strHelp = "Usage: /cmd \"system command\"\nDescription: Runs a system command.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "DATE";
	Item.strHelp = "Description: Displays the current date [MM/DD/YYYY].";
	Item.strVal = &((CMIMDlg*)m_pWnd)->m_strDate;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "DEBUG";
	Item.strHelp = "Description: Set to true to display detailed info in functions.";
	Item.iVal = (int*)&((CMIMDlg*)m_pWnd)->m_bDebug;
	Item.dType = 1;//integer
	Item.iUpper = 1;//BOOL 0-1
	Item.bEdit = TRUE;

	AddCommand(&Item);


	Item.strCommand = "DICTIONARY";
	Item.strHelp = "Usage: /DICTIONARY \"PHRASE\"\nDescription: Looks up PHRASE at dictionary.com.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "FOLLOWURL";
	Item.strHelp = "Usage: /FOLLOWURL \"URL\"\nDescription: Opens URL in internet browser.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "GETIP";
	Item.strHelp = "Usage: /GETIP [ID]\nDescription: (Admin only) Returns the IP address of the user indicated.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "GOOGLE";
	Item.strHelp = "Usage: /GOOGLE \"PHRASE\"\nDescription: Googles the entered phrase.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "INSTALL";
	Item.strHelp = "Description: Sets the current installed directory (Must be set to directory of MIM.exe and FM.exe).";
	Item.strVal = &((CMIMDlg*)m_pWnd)->m_strInstallDir;
	AddCommand(&Item);

	Item.strCommand = "IP";
	Item.strHelp = "Usage: /ip \"address\"\nDescription: Sets the address of server to connect to.";
	Item.strVal = &((CMIMDlg*)m_pWnd)->m_strIP;
	AddCommand(&Item);

	Item.strCommand = "KICK";
	Item.strHelp = "Usage: /kick [ID]\nDescription: Kicks the user with ID from server (admin only).";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "LISTEN";
	Item.strHelp = "Usage: /LISTEN\nDescription: Create chatroom (Server mode only).";
	Item.dType = -1;//Not a var, command
	Item.bPara = FALSE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "LISTENABLE";
	Item.strHelp = "Usage: /listenable [bool]\nDescription: Sets whether this computer can act as a server and accept calls.";
	Item.iVal = (int*)&((CMIMDlg*)m_pWnd)->m_bListenAble;
	Item.dType = 1;//integer
	Item.iUpper = 1;//BOOL 0-1
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "LISTENPORT";
	Item.strHelp = "Description: Server port to listen on for other clients.";
	Item.iVal = (int*)&((CMIMDlg*)m_pWnd)->m_iListenPort;
	Item.dType = 1;//integer
	Item.iUpper = 10000;//upper bound port
	AddCommand(&Item);

	Item.strCommand = "LISTFILE";
	Item.strHelp = "Usage: /listfile \nDescription: Shows the file list.";
	Item.dType = -1;
	Item.bPara = FALSE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "MAX";
	Item.strHelp = "Usage: /max [int]\nDescription: Sets the max users allowed to connect to server.";
	Item.iVal = (int*)&((CMIMDlg*)m_pWnd)->m_Users.m_iMax;
	Item.iUpper = 20;//max
	Item.dType = 1;//integer
	AddCommand(&Item);

	Item.strCommand = "MBOX";
	Item.strHelp = "Usage: /mbox [ID] \"text\"\nDescription: Pop-up message box on client's screen (admin only).";
	Item.dType = -1;
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "ME";
	Item.strHelp = "Usage: /me \"message\"\nDescription: Places text after name between asterisks.";
	Item.dType = -1;
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "MOD";
	Item.strHelp = "Usage: /MOD \"PASSWORD\"\nDescription: Request to be a server moderator.";
	Item.dType = -1;
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "MODPASS";
	Item.strHelp = "Description: Sets the Mod password, set to \"\" to disable moderators.";
	Item.strVal = &((CMIMDlg*)m_pWnd)->m_strModPass;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "MYIP";
	Item.strHelp = "Usage: /myIP\nDescription: Your computer's LOCAL IP address.";
	Item.dType = -1;
	Item.bPara = FALSE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "NAME";
	Item.strHelp = "Description: Sets your screen name.";
	Item.strVal = &((CMIMDlg*)m_pWnd)->m_strScreenName;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "NET";
	Item.strHelp = "Usage: /NET \nDescription: Displays bandwidth usage information.";
	Item.dType = -1;
	Item.bPara = FALSE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "NEW";
	Item.strHelp = "Usage: /NEW [COMMAND] \"SHORTCUT %PARAM\"\nDescription: Creates a new custom command. See \"Custom Commands\" in the readme.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);//#47

	Item.strCommand = "PASSWORD";
	Item.strHelp = "Description: Sets the password, set to \"\" to erase it.";
	Item.strVal = &((CMIMDlg*)m_pWnd)->m_strPassword;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "PING";
	Item.strHelp = "Usage: /ping [ID] \nDescription: Pings a user through the server, returns time in milliseconds.\nUse ID 0 to ping server.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "PLAY";
	Item.strHelp = "Usage: /PLAY [Filename]\nDescription: Plays a WAVE file.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "PLAYSOUND";
	Item.strHelp = "Usage: /playsound [bool]\nDescription: Changes censor level (0 = OFF, 4 = MAX)";
	Item.iVal = (int*)&((CMIMDlg*)m_pWnd)->m_bSound;	
	Item.dType = 1;//int
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "PORT";
	Item.strHelp = "Description: Used by client, port that server is listening on.";
	Item.iVal = (int*)&((CMIMDlg*)m_pWnd)->m_iPort;
	Item.iUpper = 60000;//max
	Item.dType = 1;//integer
	AddCommand(&Item);

	Item.strCommand = "QUOTE";
	Item.strHelp = "Usage: /QUOTE \"QUOTE\" \"NAME\"\nDescription: Places the current date, quote, then author name into the quotes.txt file.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "ROOMNAME";
	Item.strHelp = "Description: Sets the room name, must be disconnected";
	Item.strVal = &((CMIMDlg*)m_pWnd)->m_strRoomName;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "REFRESH";
	Item.strHelp = "Usage: /REFRESH \nDescription: Reloads all of the custom commands from commands.txt.";
	Item.dType = -1;//Not a var, command
	Item.bPara = FALSE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "REJOIN";
	Item.strHelp = "Usage: /REJOIN\nDescription: Closes the current connection and re-connects.";
	Item.dType = -1;//Not a var, command
	Item.bPara = FALSE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "ROLL";
	Item.strHelp = "Usage: /ROLL [integer]\nDescription: Randomly chooses a number from 1 to the entered integer.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "RUN";
	Item.strHelp = "Usage: /run \"[dir] [parameters]\"\nDescription: Same function as the Windows RUN command, executes a program.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "SCREEN";
	Item.strHelp = "Usage: /screen [BOOL]\nDescription: Asks admin permission for client to join.\n[1 = ON, 0 = OFF]";
	Item.iVal = (int*)&((CMIMDlg*)m_pWnd)->m_bAsk;
	Item.dType = 1;//integer
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "SENDFILE";
	Item.strHelp = "Usage: /sendfile [ID]\nDescription: Sends all files in the current file list to user ID.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "SOUND";
	Item.strHelp = "Usage: /sound \"Sound.wav\"\nDescription: Sets the sound file to be used.";
	Item.strVal = &((CMIMDlg*)m_pWnd)->m_strSoundDir;
	Item.dType = 0;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "TIME";
	Item.strHelp = "Description: Displays the current time [HH:MM:SS].";
	Item.strVal = &((CMIMDlg*)m_pWnd)->m_strTime;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "TIMESTAMP";
	Item.strHelp = "Usage: /timestamp [BOOL]\nDescription: Show time stamp before each message.\n[1 = ON, 0 = OFF]";
	Item.iVal = (int*)&((CMIMDlg*)m_pWnd)->m_bTimeStamp;
	Item.dType = 1;//integer
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "UNBAN";
	Item.strHelp = "Usage: /UNBAN \"IP\"\nDescription: Unbans a user IP from ban list.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "USER";
	Item.strHelp = "Usage: /user [ID] \"command\"\nDescription: Runs a console command on client ID (admin only).";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "VIEWPROFILE";
	Item.strHelp = "Usage: /viewprofile [ID]\nDescription: Requests and displays profile from specified user. Use ID=0 for admin.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "VOTE";
	Item.strHelp = "Usage1: /vote \"question\" [choice1] [c2] [c3] [c4]\nUsage2: /vote [choice]\nDescription: Polls clients, and shows \"tally\"\nDefault choices are Y/N.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	Item.strCommand = "WHISPER";
	Item.strHelp = "Usage: /WHISPER [ID] \"Text\"\nDescription: Use \"/w\" for shortcut.\nSends \"Text\" only to the user specified.";
	Item.dType = -1;//Not a var, command
	Item.bPara = TRUE;
	Item.bEdit = TRUE;
	AddCommand(&Item);

	m_iTotalCom = m_oaCommands.GetSize();//number of built-in commands, not including custom ones
	LoadCustom();
	return 0;
}

void CConsole::LoadCustom()
{
	//load custom commands
	CFile fcustom;
	CString strLine;
	CCommand Item;
	int i,iLen;

	//remove any custom commands appended to end of regular commands
	for(i = m_oaCommands.GetSize()-1;i >= m_iTotalCom;i--)	
	{
		delete m_oaCommands[i];
		m_oaCommands.RemoveAt(i);
	}
	if(fcustom.Open(((CMIMDlg*)m_pWnd)->m_strInstallDir+"\\Commands.txt",CFile::modeNoTruncate|CFile::modeRead/*|CFile::typeText*/))
	{
		if(fcustom.GetLength() == 0)//empty file
		{
			fcustom.Close();
			return;
		}
		CArchive* ar = new CArchive(&fcustom,CArchive::load);
		while (true)
		{
			if(!ar->ReadString(strLine))//end of file
				break;
			iLen = strLine.GetLength();
			for(i = 0;i < iLen;i++)
			{
				if(strLine[i] == ' ')
					break;
			}
			Item.strCommand = strLine.Left(i);
			Item.strHelp = strLine.Right(iLen-i-1);
			Item.dType = 3;//Custom command
			//Item.bPara = TRUE;
			//Item.bEdit = TRUE;
			//((CMIMDlg*)m_pWnd)->MessageBox(Item.strCommand+"+"+Item.strHelp);
			AddCommand(&Item);
		}
		delete ar;	
		fcustom.Close();
	}
}

void CConsole::SetParent(CDialog *pWnd)
{
	//Set m_Pwnd to point to the address of the CMIMDlg class
	m_pWnd = pWnd;
	//m_pWnd = ((CMIMDlg*)m_pWnd);
}

int CConsole::Console(CString strLine, BOOL bRemote)
{
	int i, j, max, para, error;
	char line[400];
	char tmpCommand[150];

	CString strMess, m_strTemp;
		
	//((CMIMDlg*)m_pWnd)->MessageBox("Console Started");
	strcpy_s(line, LPCTSTR(strLine));

	//clear strings
	strCommand.RemoveAll();

	//remove first char '/'
	if(line[0] == '/')
	{
		for(i = 0;i < 399;i++)
			line[i] = line[i+1];
		line[399] = 0;
	}

	if(!m_bHelp)//Show console command after pressing enter
	{
		strLine = line;
		//DisplayMessage(">> " + strLine);//Old way
		DisplayMessage(strLine);
	}

	j = 0;
	max = strlen(line);
	para = 0;

	for(i = 0;i < max;i++)
	{
		if((line[i] == '\"') && (j == 0))//quotation starting a word
		{
			i++;//skip " mark
			int max2 = max-i;
			while(j < max2)//max length of 1 word
			{
				if(line[i] == '\"')//end quote
				{
					tmpCommand[j] = '\0';//end of word
					strCommand.Add(tmpCommand);//add command to array

					j = 0;//reset to start of word
					para++;//add new parameter
					i++;
					tmpCommand[0] = 0;//erase string for next parameter
					break;
				}
				tmpCommand[j] = line[i];//copy letters until the end of the quote is found
				j++;
				i++;
			}

			if(line[i] != ' ')//if no space after quote
			{
				//put a space there
				i--;
				line[i] = ' ';
			}
		}
		else if(line[i] == ' ')//space
		{
			tmpCommand[j] = '\0';
			strCommand.Add(tmpCommand);
			tmpCommand[0] = 0;//erase string for next parameter
			j = 0;
			para++;
		}
		else if(i == max-1)//end of commands
		{
			tmpCommand[j] = line[i];
			tmpCommand[j+1] = '\0';//NOTE: second letter is \0 if no parameter
			strCommand.Add(tmpCommand);
			tmpCommand[0] = 0;//erase string for next parameter
			para++;
			break;
		}
		else
		{
			tmpCommand[j] = line[i];
			j++;
		}
	}
	//for(int m = 2;m < strCommand.GetSize();m++)
	//	DisplayMessage(strCommand.GetAt(m));

	if(para <= 0)
	{
		strCommand.Add("HELP");//If user only types a '/', then display help command
		para = 1;
	}
	strMess = strCommand.GetAt(0);
	strMess.MakeUpper();
	strCommand.SetAt(0, strMess);//make first command uppercase
	//DisplayMessage(strCommand.GetAt(0)+"-"+strCommand.GetAt(1)+"-"+strCommand.GetAt(2));
	
//SHORTCUTS, RENAME
	if(strCommand.GetAt(0) == "W")
		strCommand.SetAt(0,"WHISPER");
	else if(strCommand.GetAt(0) == "TIME")
		((CMIMDlg*)m_pWnd)->GetTimeStamp();//do this before getting the pointer string value
	//else if(strCommand.GetAt(0) == "DATE")
	//	((CMIMDlg*)m_pWnd)->GetDate();//meh this isnt really necessary, update the date

//START COMMAND LIST
	for(i = 0;i < m_oaCommands.GetSize();i++)
	{
		//Find a match for name
		if(strCommand.GetAt(0) == GetComm(i)->strCommand)
		{
			//DisplayMessage(GetComm(i)->strCommand);

			if(m_bHelp)//Show help text
			{
				DisplayMessage(GetComm(i)->strHelp+"\r\n");
				return 0;
			}
			else if(GetComm(i)->dType == 3)//Custom Command
			{
				//((CMIMDlg*)m_pWnd)->MessageBox(GetComm(i)->strHelp);
				CString strTemp;
				strTemp = GetComm(i)->strHelp;
				int i = 1;
				int iPos1;
				while(i < para)
				{
					iPos1 = strTemp.Find("%s");
					//iPos2 = strTemp.Find("%i");
					if(iPos1 == -1)
					{
						/*iPos = strTemp.Find("%i");
						if(iPos == -1)
							break;
						strTemp.Delete(iPos,2);
						iUser = MakeNum(strCommand.GetAt(i),1);
						if(iUser > 0)
						{
							if(((CMIMDlg*)m_pWnd)->m_Users.m_iConnected[iUser-1] == 1)
								strTemp.Insert(iPos,((CMIMDlg*)m_pWnd)->m_Users.m_strNames[iUser-1]);
						}
						strTemp.Format("iUser=%i - Connected = %i - Name = %s",iUser,((CMIMDlg*)m_pWnd)->m_Users.m_iConnected[iUser-1],((CMIMDlg*)m_pWnd)->m_Users.m_strNames[iUser-1]);
						DisplayMessage(strTemp);*/
						break;
					}
					else
					{
						strTemp.Delete(iPos1,2);
						strTemp.Insert(iPos1,strCommand.GetAt(i));
					}

					i++;
				}
				((CMIMDlg*)m_pWnd)->OnBSend(strTemp);
			}
			else if(strCommand.GetSize() < 2)//no params
			{
				//DisplayMessage("No Params.");
				if(GetComm(i)->dType == -1)//show usage for command
				{
					if(GetComm(i)->bPara)//needs parameters, for command/action
					{
						DisplayMessage(GetComm(i)->strHelp);//show usage
						return 0;
					}
					else
						break;//go to special cases to perform action
				}
				else if(GetComm(i)->bPara)//needs parameters, for command/action
				{
					DisplayMessage("Missing parameter(s)",RED);
					return -1;
				}
				else if(GetComm(i)->dType == 0)//show string val
				{
					strMess.Format("\"%s\" [STRING] = \"%s\"", GetComm(i)->strCommand, *GetComm(i)->strVal);
					DisplayMessage(strMess);
					return 0;
				}
				else if(GetComm(i)->dType == 1)//show int val
				{
					if(GetComm(i)->iUpper == 1)
						m_strTemp = "[BOOL]";
					else
						m_strTemp = "[INT]";
					strMess.Format("\"%s\" %s = %i", GetComm(i)->strCommand, m_strTemp, *GetComm(i)->iVal);
					DisplayMessage(strMess);
					return 0;
				}
				else
					DisplayMessage("Bad commands setup, please report this error.",RED);
			}
			else if((!GetComm(i)->bEdit) && (m_bConnected))
			{
				DisplayMessage("<!> Cant change while connected",RED);
				return 0;
			}
			else if(!GetComm(i)->bPara)//not supposed to ba any params
			{
				if(GetComm(i)->dType == 0)//edit string val
				{
					//combines all parameters into 1 string
					//CString m_strTemp;
					m_strTemp = "";
					for(int j = 1;j < strCommand.GetSize();j++)
						m_strTemp.Format("%s%s ",m_strTemp, strCommand.GetAt(j));
					m_strTemp.Delete(m_strTemp.GetLength()-1,1);//remove last space

					//if(*GetComm(i)->strVal == strCommand.GetAt(1))//check for the SAME value
					if(*GetComm(i)->strVal == m_strTemp)//check for the SAME value
					{
						strMess.Format("\"%s\" is already set to: \"%s\"", GetComm(i)->strCommand, *GetComm(i)->strVal);
						DisplayMessage(strMess,YELLOW);
						return -1;
					}

					//*GetComm(i)->strVal = strCommand.GetAt(1);
					*GetComm(i)->strVal = m_strTemp;
					strMess.Format("Changed \"%s\" to: \"%s\"", GetComm(i)->strCommand, *GetComm(i)->strVal);
					DisplayMessage(strMess,GREEN);

					//odd ball
					if((strCommand.GetAt(0) == "NAME") && (((CMIMDlg*)m_pWnd)->m_iType == CLIENT))//only if screen name was edited
					{
						DisplayMessage("NAME",GREEN);
						if(m_bConnected)
						{
							CRequest Request;
							StrCpyN(Request.m_strName, CombinePara(1),30);
							Request.m_bFirst = FALSE;
							Request.m_dVer = m_dVer;
							CPacket QPacket;
							QPacket.m_iCode = REG;
							QPacket.m_iPos = 0;
							QPacket.m_iLength = sizeof(CRequest);
							CMySocket::StrCopy(QPacket.pBuf, (char*)&Request, QPacket.m_iLength);
							((CMIMDlg*)m_pWnd)->SendText(QPacket);
						}
					}
					else if((strCommand.GetAt(0) == "ADMINNAME") && (((CMIMDlg*)m_pWnd)->m_iType == SERVER))//only if adminname was edited
					{
						StrCpyN(((CMIMDlg*)m_pWnd)->m_Users.m_strAdmin, ((CMIMDlg*)m_pWnd)->m_strAdminName,30);
						((CMIMDlg*)m_pWnd)->m_Users.m_strAdmin[29] = 0;
						((CMIMDlg*)m_pWnd)->UpdateList();
						((CMIMDlg*)m_pWnd)->SendText(-1,ULIST,"");
					}
					else if(strCommand.GetAt(0) == "ROOMNAME")
					{
						if(((CMIMDlg*)m_pWnd)->m_iType == SERVER)
						{
							((CMIMDlg*)m_pWnd)->SendText(0, RNAME, ((CMIMDlg*)m_pWnd)->m_strRoomName);
							((CMIMDlg*)m_pWnd)->SetWindowText(((CMIMDlg*)m_pWnd)->m_strRoomName);
							((CMIMDlg*)m_pWnd)->UpdateData(FALSE);
						}
					}

					return 0;
				}
				else if(GetComm(i)->dType == 1)//edit as an int
				{
					error = MakeNum(strCommand.GetAt(1),0,GetComm(i)->iUpper);
					if(error == -1)
					{
						DisplayMessage("Invalid integer.",RED);
						return -1;
					}
					else if(error == -2)
					{
						DisplayMessage("Invalid Range.",RED);
						return -1;
					}
 
					if(*GetComm(i)->iVal == error)//check for the SAME value
					{
						strMess.Format("\"%s\" is already set to: \"%i\"", GetComm(i)->strCommand, *GetComm(i)->iVal);
						DisplayMessage(strMess,YELLOW);
						return -1;
					}

					*GetComm(i)->iVal = error;
					strMess.Format("Changed \"%s\" to: %i", GetComm(i)->strCommand, *GetComm(i)->iVal);
					DisplayMessage(strMess,GREEN);
					//return 0;
				}
				else//edit var
					DisplayMessage("<!> Invalid variable type, please report this error.",RED);
			}
			else
				break;//go to special cases
			return 0;
		}
	}
	
//////////////Special Cases//////////////////
	//Each if statement (command) should return a value

	if((strCommand.GetAt(0) == "EXIT") || (strCommand.GetAt(0) == "QUIT"))
	{
		if(bRemote)//no remotely closing server
			return -1;

		((CMIMDlg*)m_pWnd)->Exit();
		return 0;
	}
	else if(strCommand.GetAt(0) == "KICK")
	{
		error = Kick(MakeNum(strCommand.GetAt(1), 1, 20)-1);
		if(error == -1)
			DisplayMessage("<!> You do not have access to this command.",RED);
		else if(error == -2)
		{
			strMess.Format("<!> Invalid Number: %i", MakeNum(strCommand.GetAt(1)));
			DisplayMessage(strMess,RED);
		}
		else if(error == -3)
		{
			strMess.Format("<!> User %i not connected", MakeNum(strCommand.GetAt(1)));
			DisplayMessage(strMess,RED);
		}
		return 0;
	}
	else if(strCommand.GetAt(0) == "ADDBAN")
	{
		((CMIMDlg*)m_pWnd)->m_strBanned.Add(strCommand.GetAt(1));
		int here = ((CMIMDlg*)m_pWnd)->m_strBanned.GetSize()-1;
		strMess.Format("Added banned IP address (#%i): \"%s\".", here+1, ((CMIMDlg*)m_pWnd)->m_strBanned.GetAt(here));
		DisplayMessage(strMess,GREEN);
		return i;//added
	}
	else if(strCommand.GetAt(0) == "BAN")
	{
		error = Ban(MakeNum(strCommand.GetAt(1), 1, 20)-1);
		if(error == -1)
			DisplayMessage("<!> You do not have access to this command.",RED);
		else if(error == -2)
		{
			strMess.Format("<!> Invalid Number: %i", MakeNum(strCommand.GetAt(1)));
			DisplayMessage(strMess,RED);
		}
		else if(error == -3)
		{
			strMess.Format("<!> User %i not connected", MakeNum(strCommand.GetAt(1)));
			DisplayMessage(strMess,RED);
		}
		return 0;
	}
	else if(strCommand.GetAt(0) == "UNBAN")
	{
		int top = ((CMIMDlg*)m_pWnd)->m_strBanned.GetSize();
		int i = 0;
		for(; i < top;i++)
		{
			if(	((CMIMDlg*)m_pWnd)->m_strBanned.GetAt(i) == strCommand.GetAt(1))
			{
				((CMIMDlg*)m_pWnd)->m_strBanned.RemoveAt(i);
				i = top+1;
				break;
			}
		}
		if(i == top+1)//removed
		{
			strMess.Format("%s <!> \"%s\" removed from ban list.", ((CMIMDlg*)m_pWnd)->GetTimeStamp(), strCommand.GetAt(1));
			DisplayMessage(strMess,GREEN);
		}
		else//reached end of list
		{
			strMess.Format("%s <!> \"%s\" not found in ban list.", ((CMIMDlg*)m_pWnd)->GetTimeStamp(), strCommand.GetAt(1));
			DisplayMessage(strMess,YELLOW);
		}
		return 0;
	}
	else if(strCommand.GetAt(0) == "HELP")
	{
		if(para <= 1)
		{
			DisplayMessage("\nUsage: /command  [parameter1]  [p2]  [p3]  [p4]  [p5]");
			DisplayMessage("Type \"/help [command]\" to see details for that command. Type \"/commands\" to see a list of available commands\n(See the readme for more extensive help,\r\nType: /run \"notepad readme.txt\")\r\n");
			return 0;
		}
		DisplayMessage("");//blank line
		strMess.Format("/%s", strCommand.GetAt(1));
		m_bHelp = TRUE;
		Console(strMess);//call, searching for help
		m_bHelp = FALSE;
		return 0;
	}
	else if(strCommand.GetAt(0) == "CONNECT")
	{
		if(((CMIMDlg*)m_pWnd)->m_iType == CLIENT)
		{
			if(!((CMIMDlg*)m_pWnd)->m_bShowSend)//not connected
				((CMIMDlg*)m_pWnd)->OnConnect();
				//((CMIMDlg*)m_pWnd)->Connect();
			else
				DisplayMessage("<!> Already connected",YELLOW);
		}
		else
			DisplayMessage("<!> Can't connect as server. Try /LISTEN",YELLOW);
		return 0;
	}
	else if(strCommand.GetAt(0) == "LISTEN")
	{
		if(((CMIMDlg*)m_pWnd)->m_iType == SERVER)
		{
			if(!((CMIMDlg*)m_pWnd)->m_bShowSend)//not connected
				((CMIMDlg*)m_pWnd)->Connect();
			else
				DisplayMessage("<!> Already connected",YELLOW);
		}
		else
			DisplayMessage("<!> Can't create server, you are a client.",RED);
		return 0;
	}
	else if(strCommand.GetAt(0) == "PLAY")
	{
		((CMIMDlg*)m_pWnd)->PlayWav(strCommand.GetAt(1));
		return 0;
	}
	else if(strCommand.GetAt(0) == "REJOIN")
	{
		if(((CMIMDlg*)m_pWnd)->m_iType == SERVER)
			((CMIMDlg*)m_pWnd)->CloseCon();//Close, pass -1 for current user. This will crash the client if used with /user command
		else
			((CMIMDlg*)m_pWnd)->OnCloseCon(0);//do this to avoid crash
		Sleep(500);
		if(!((CMIMDlg*)m_pWnd)->m_bShowSend)//not connected
			((CMIMDlg*)m_pWnd)->Connect();
		else
			DisplayMessage("<!> Already connected",YELLOW);

		return 0;
	}
	else if(strCommand.GetAt(0) == "ROLL")
	{
		int iRoll = MakeNum(strCommand.GetAt(1),1,100);
		if(iRoll < 1)
		{
			DisplayMessage("Invalid number (1-1000)",RED);
			return -1;
		}
		srand(time(NULL)*100);
		int iRand = 1 + rand() % iRoll; //1 to iRoll

		if(!m_bConnected)//do it yourself
		{
			m_strTemp.Format("%s <<Rolled a %i out of %i>>", ((CMIMDlg*)m_pWnd)->GetTimeStamp(), iRand, iRoll);
			DisplayMessage(m_strTemp,GREEN);
		}
		else//online need to get server generated rand
		{
			if(((CMIMDlg*)m_pWnd)->m_iType == SERVER)
			{
				m_strTemp.Format("%s <<(A) %s rolled a %i out of %i>>", ((CMIMDlg*)m_pWnd)->GetTimeStamp(), ((CMIMDlg*)m_pWnd)->m_strAdminName, iRand, iRoll);
				((CMIMDlg*)m_pWnd)->SendMess(-1, m_strTemp, MESG, TRUE);
			}
			else//client
				((CMIMDlg*)m_pWnd)->SendText(0, ROLL, strCommand.GetAt(1));//send to server
		}
		return 0;
	}
	else if(strCommand.GetAt(0) == "VOTE")//FIX: didnt work even when I was online
	{
		if(!m_bConnected)
		{
			DisplayMessage("<!> Must be online to vote.",RED);
			return -1;
		}
		if(((CMIMDlg*)m_pWnd)->m_iType == CLIENT)
		{
			if(!m_bVoteActive)
			{
				DisplayMessage("<!> No vote is currently in progress, or you already voted.",RED);
				return -2;
			}
			int iChoice = MakeNum(strCommand.GetAt(1),1,m_iChoices);
			if(iChoice > 0)//valid vote
			{
				((CMIMDlg*)m_pWnd)->SendText(0, VOTE, strCommand.GetAt(1));//send to server
				m_bVoteActive = FALSE;
			}
			else
			{
				strMess.Format("<!> Invalid vote (1-%i)", m_iChoices);
				DisplayMessage(strMess,RED);
			}
		}
		else  //Server
		{
			if(m_bVoteActive)
			{
				DisplayMessage("<!> A vote is currently in progress. Type /TALLY to end it.",YELLOW);
				return -2;
			}

			m_strReturn.Format("VOTE:  \"%s\"\n", strCommand.GetAt(1));
			if(para >= 3)//choice 1
			{
				strMess.Format("1. %s", strCommand.GetAt(2));
				m_strReturn = m_strReturn+strMess;
				m_iChoices = 1;
			}
			else
			{	//default answers
				m_strReturn += "1. Yes";	
				m_strReturn += "\n2. No";
				m_iChoices = 2;
			}
			if(para >= 4)//choice 2
			{
				strMess.Format("\n2. %s", strCommand.GetAt(3));
				m_strReturn += strMess;
				m_iChoices = 2;
			}
			if(para >= 5)//choice 3
			{
				strMess.Format("\n3. %s", strCommand.GetAt(4));
				m_strReturn += strMess;
				m_iChoices = 3;
			}
			if(para >= 6)//choice 4
			{
				strMess.Format("\n4. %s", strCommand.GetAt(5));
				m_strReturn += strMess;
				m_iChoices = 4;
			}
			Vote(m_strReturn, m_iChoices);
			return 0;
		}
	}
	else if(strCommand.GetAt(0) == "TALLY")
	{
		if(m_bHelp)
		{
			DisplayMessage("Description: Shows results of vote even if some users have not voted yet.");
			return 0;
		}
		if(((CMIMDlg*)m_pWnd)->m_iType != SERVER)//client
		{
			DisplayMessage("<!> You do not have access to this command.",RED);
			return -1;
		}
		if(Tally() == -1)
			DisplayMessage("<!> No vote in progress.",RED);
		return 0;
	}
	else if((strCommand.GetAt(0) == "DISCONNECT") || (strCommand.GetAt(0) == "CLOSE"))
	{
		/*if(m_bHelp)
		{
			DisplayMessage();
			return 0;
		}*/
		if(bRemote)//no remotely closing server
			return -1;

		if(((CMIMDlg*)m_pWnd)->m_iType == SERVER)
			((CMIMDlg*)m_pWnd)->CloseCon();//Close, pass -1 for current user. This will crash the client if used with /user command
		else
			((CMIMDlg*)m_pWnd)->OnCloseCon(0);//do this to avoid crash

		return 0;
	}
	/*else if(strCommand.GetAt(0) == "AWAY")
	{
		if((m_Users.m_iImage[m_pClientSocket[0]->m_iClientID] == 4)&&(m_iType == CLIENT))
		{
			SendText(0,AWAY,"");
			m_wndShow.ShowMessage(">> Status: Available",GREEN);
		}
	}*/
	else if(strCommand.GetAt(0) == "ADDIP")
	{
		CString strTemp;
		((CMIMDlg*)m_pWnd)->m_strSaved.Add(strCommand.GetAt(1));
		if(para >= 3)
			strTemp = strCommand.GetAt(2);
		else
			strTemp = "";
		((CMIMDlg*)m_pWnd)->m_strComments.Add(strTemp);
		int here = ((CMIMDlg*)m_pWnd)->m_strSaved.GetSize()-1;
		strMess.Format("Added IP address (#%i): \"%s\" [Comment: %s].", here+1, ((CMIMDlg*)m_pWnd)->m_strSaved.GetAt(here), strTemp);
		
		DisplayMessage(strMess,GREEN);
		return i;//added
	}
	else if(strCommand.GetAt(0) == "SENDFILE")
	{
		int id = MakeNum(strCommand.GetAt(1))-1;
		if(id < -1)//error
			DisplayMessage("<!> Invalid number or range.",RED);
		else
			((CMIMDlg*)m_pWnd)->SendFiles(id);
		return 0;
	}
	else if(strCommand.GetAt(0) == "ADDFILE")
	{
		((CMIMDlg*)m_pWnd)->AddFile(strCommand.GetAt(1));
		int here = ((CMIMDlg*)m_pWnd)->m_strFileNames.GetSize()-1;
		//strMess.Format("Added file (#%i): \"%s%s\".", here+1, ((CMIMDlg*)m_pWnd)->m_strFileDirs.GetAt(here), ((CMIMDlg*)m_pWnd)->m_strFileNames.GetAt(here));
		//DisplayMessage(strMess);
		return i;//added
	}
	else if(strCommand.GetAt(0) == "CLEARFILE")
	{
		((CMIMDlg*)m_pWnd)->m_strFileDirs.RemoveAll();
		((CMIMDlg*)m_pWnd)->m_strFileNames.RemoveAll();
		DisplayMessage("File list cleared.",GREEN);
		return 0;//added
	}
	else if(strCommand.GetAt(0) == "CLEARBAN")
	{
		((CMIMDlg*)m_pWnd)->m_strBanned.RemoveAll();
		DisplayMessage("Ban list cleared.",GREEN);
		return 0;//added
	}
	else if(strCommand.GetAt(0) == "LISTFILE")
	{
		CString strTemp;
		for(int i=0;i < ((CMIMDlg*)m_pWnd)->m_strFileDirs.GetSize();i++)
		{
			strTemp.Format("%i. %s %s",i+1,((CMIMDlg*)m_pWnd)->m_strFileDirs.GetAt(i), ((CMIMDlg*)m_pWnd)->m_strFileNames.GetAt(i));
			DisplayMessage(strTemp);
		}
		return 0;
	}
	else if(strCommand.GetAt(0) == "NET")
	{
		long sec;
		CString strTemp;
		((CMIMDlg*)m_pWnd)->m_CurTime = CTime::GetCurrentTime();
		CTimeSpan tDuration;
		tDuration =  ((CMIMDlg*)m_pWnd)->m_CurTime - ((CMIMDlg*)m_pWnd)->m_FirstTime;
		sec = tDuration.GetTotalSeconds();
		ASSERT(sec != 0);
		strTemp.Format("Sent:      %i Bytes\nReceived:  %i Bytes\nUpload: %d Bytes/s\nDownload: %d Bytes/s",((CMIMDlg*)m_pWnd)->m_iByteSent, ((CMIMDlg*)m_pWnd)->m_iByteRec, ((CMIMDlg*)m_pWnd)->m_iByteSent/sec,((CMIMDlg*)m_pWnd)->m_iByteRec/sec);
		DisplayMessage(strTemp);
		return 0;
	}
	else if(strCommand.GetAt(0) == "CLEAR")
	{
		if(m_bHelp)
		{
			DisplayMessage("Description: Clears all text in console window.");
			return 0;
		}
		else
		{
			//((CMIMDlg*)m_pWnd)->m_strRoom.Empty();//dont need with richedit
			if(((CMIMDlg*)m_pWnd)->MessageBox("Are you sure that you want to clear the message list?", "Clear Chat Room List", MB_YESNO | MB_ICONQUESTION) == IDYES)	
				((CMIMDlg*)m_pWnd)->Clear();
		}
		return 0;
	}
	else if(strCommand.GetAt(0) == "CMD")
	{
		if(bRemote)//safety feature
			return -1;
		
		m_strTemp = "";
		for(int i = 1;i < strCommand.GetSize();i++)
			m_strTemp.Format("%s%s ",m_strTemp, strCommand.GetAt(i));
		m_strTemp.Delete(m_strTemp.GetLength()-1,1);//remove last space

		error = system(m_strTemp);
		/*if(error == 0)
			DisplayMessage("No error occured.",RED);*/
		return 0;
	}
	else if(strCommand.GetAt(0) == "GOOGLE")
	{
		((CMIMDlg*)m_pWnd)->OnGoogle(CombinePara(1));
		return 0;
	}
	else if(strCommand.GetAt(0) == "DICTIONARY")
	{
		((CMIMDlg*)m_pWnd)->OnDictionary(CombinePara(1));
		return 0;
	}
	else if(strCommand.GetAt(0) == "FOLLOWURL")
	{
		((CMIMDlg*)m_pWnd)->Browser(CombinePara(1));
		return 0;
	}
	else if(strCommand.GetAt(0) == "GETIP")
	{
		int id = MakeNum(strCommand.GetAt(1))-1;
		if(id < 0)//error
			DisplayMessage("<!> Invalid number or range.",RED);
		else
			DisplayMessage("IP = "+((CMIMDlg*)m_pWnd)->GetIP(id));
		return 0;
	}
	else if(strCommand.GetAt(0) == "ME")
	{
		if(!m_bConnected)
			return -1;

		CString m_strTemp;
		m_strTemp = "";
		for(int i = 1;i < strCommand.GetSize();i++)
			m_strTemp.Format("%s%s ",m_strTemp, strCommand.GetAt(i));
		m_strTemp.Delete(m_strTemp.GetLength()-1,1);//remove last space
		m_strTemp = ((CMIMDlg*)m_pWnd)->Censor(m_strTemp);
		if(((CMIMDlg*)m_pWnd)->m_iType == SERVER)
		{
			strMess.Format("%s *(A) %s %s*", ((CMIMDlg*)m_pWnd)->GetTimeStamp(),((CMIMDlg*)m_pWnd)->m_strAdminName, m_strTemp);
			((CMIMDlg*)m_pWnd)->SendMess(-1, strMess, ME,TRUE);
		}
		else
			((CMIMDlg*)m_pWnd)->SendMess(0, m_strTemp, ME);
		return 0;
	}
	else if(strCommand.GetAt(0) == "MOD")
	{
		if(((CMIMDlg*)m_pWnd)->m_iType == SERVER)
		{
			DisplayMessage("<!> Only clients can be mods.",RED);
			return -1;
		}
		else if(para <= 1)
			DisplayMessage("<!> Missing parameter: [password]",RED);
		else//OK
		{	
			DisplayMessage(">> Sent Mod request.");
			((CMIMDlg*)m_pWnd)->SendText(0, MOD, strCommand.GetAt(1));//Send request to admin
		}
		return 0;
	}
	else if(strCommand.GetAt(0) == "MBOX")
	{
		error = MakeNum(strCommand.GetAt(1), 1);
		
		if(error == -1)
		{
			strMess.Format("<!> Invalid Number: %i", MakeNum(strCommand.GetAt(1)));
			DisplayMessage(strMess,RED);
		}
		else if(error == -2)
			DisplayMessage("<!> Invalid range.",RED);
		else if(para <= 2)
			DisplayMessage("<!> Missing parameter: [text]",RED);
		else//OK
		{	
			error--;
			DisplayMessage(">> Sent message box");
			((CMIMDlg*)m_pWnd)->SendMess(error, strCommand.GetAt(2), MBOX, FALSE);//Send command to user
		}
		return 0;
	}
	else if(strCommand.GetAt(0) == "MYIP")
	{
		//DisplayMessage("Out of Order");
		DisplayMessage(((CMIMDlg*)m_pWnd)->GetYourIP());
		return 0;
	}
	else if(strCommand.GetAt(0) == "TESTSOUND")
	{
		if(m_bHelp)
		{
			DisplayMessage("Description: Attempts to play the sound file specified in Connection Settings.");
			return 0;
		}
		else
		{
			strMess.Format("  Playing Sound: \"%s\"...", ((CMIMDlg*)m_pWnd)->m_strSoundDir);
			DisplayMessage(strMess);

			((CMIMDlg*)m_pWnd)->PlayRecSound();
			return 0;
		}
	}
	else if(strCommand.GetAt(0) == "RUN")
	{
		m_strTemp = "";
		for(int i = 1;i < strCommand.GetSize();i++)
			m_strTemp.Format("%s%s ",m_strTemp, strCommand.GetAt(i));
		m_strTemp.Delete(m_strTemp.GetLength()-1,1);//remove last space

		error = WinExec(m_strTemp, SW_SHOW);
		if(error == 0)
			DisplayMessage("The system is out of memory or resources.",RED);
		else if(error == ERROR_BAD_FORMAT)
			DisplayMessage("<!> The .EXE file is invalid (non-Win32 .EXE or error in .EXE image)",RED);

		else if(error == ERROR_FILE_NOT_FOUND)
			DisplayMessage("<!> The specified file was not found.",RED);

		else if(error == ERROR_PATH_NOT_FOUND)
			DisplayMessage("<!> The specified path was not found.",RED);
		return 0;
	}
	else if(strCommand.GetAt(0) == "SYSTEM")
	{
		char strSys[101];

		GetSystemDirectory(strSys, 100);
		strMess.Format("System dir: \"%s\"", strSys);
		DisplayMessage(strMess,GREEN);
		GetWindowsDirectory(strSys, 100);
		strMess.Format("Windows dir: \"%s\"", strSys);
		DisplayMessage(strMess,GREEN);
		return 0;
	}
	else if(strCommand.GetAt(0) == "SAVE")
	{
		((CMIMDlg*)m_pWnd)->SaveSettings();
		return 0;
	}
	else if(strCommand.GetAt(0) == "LOAD")
	{
		if(m_bConnected)
		{
			DisplayMessage("<!> Can't change settings while connected.",RED);
			return -1;
		}
		((CMIMDlg*)m_pWnd)->LoadSettings();
		DisplayMessage("Files: \"MIM.dat\" and \"Server.txt\" loaded.",GREEN);
		return 0;
	}
	else if(strCommand.GetAt(0) == "WHISPER")
	{
		//DisplayMessage("<!> WHISPER command is under currently out of order.",YELLOW);
		//return 0;

		if(!m_bConnected)
			return -1;

		error = MakeNum(strCommand.GetAt(1))-1;
		if(error < 0)
		{
			DisplayMessage("<!> Invalid number or range.",RED);
			return -1;
		}

		if(((CMIMDlg*)m_pWnd)->m_iType == SERVER)//server whispers to only 1 client (ID = error)
		{
			strMess.Format("WHISPER (A) %s:  ", ((CMIMDlg*)m_pWnd)->m_strAdminName);
			for(int i = 2;i < para;i++)
				strMess.Format("%s%s ",strMess,strCommand.GetAt(i));
			((CMIMDlg*)m_pWnd)->SendMess(error, strMess);
		}
		else
		{
			strMess.Format("(%i) [WHISPER] %s:  ", ((CMIMDlg*)m_pWnd)->m_pClientSocket[0]->m_iClientID+1, ((CMIMDlg*)m_pWnd)->m_strScreenName);
			for(int i = 2;i < para;i++)
				strMess.Format("%s%s ",strMess,strCommand.GetAt(i));

			CPkg Whisper;
			((CMIMDlg*)m_pWnd)->AssemblePkg(Whisper,strMess);//set to your font

			Whisper.fontSize = error;//receiving client

			CPacket QPacket;
			QPacket.m_iCode = WHISPER;
			QPacket.m_iPos = 0;//send to server
			QPacket.m_iLength = strMess.GetLength()+56;//adding on extra vars size in CPkg
			CMySocket::StrCopy(QPacket.pBuf, (char*)&Whisper, QPacket.m_iLength);
			((CMIMDlg*)m_pWnd)->SendText(QPacket);//send to server where server will redirect it
		}
		strMess.Format("WHISPER TO (%i) %s: %s",error+1, ((CMIMDlg*)m_pWnd)->m_Users.m_strNames[error], strMess);
		DisplayMessage(((CMIMDlg*)m_pWnd)->Censor(strMess));

		return 0;
	}
	else if(strCommand.GetAt(0) == "QUOTE")
	{
		CString strTemp;
		strTemp.Format("%s \"%s\" -%s\r\n",((CMIMDlg*)m_pWnd)->GetDate(),strCommand.GetAt(1),strCommand.GetAt(2));
		if(((CMIMDlg*)m_pWnd)->AddLine("Quote.txt",strTemp)==0)
		{
			m_strTemp.Format("Quote saved to \"%s\".", ((CMIMDlg*)m_pWnd)->m_strInstallDir+"\\Quote.txt");
			DisplayMessage(m_strTemp,GREEN);
		}
		return 0;
	}
	else if(strCommand.GetAt(0) == "REFRESH")
	{
		LoadCustom();
		DisplayMessage("Loaded custom commands");
		return 0;
	}
	else if(strCommand.GetAt(0) == "NEW")
	{
		CString strTemp,strTemp2;
		strTemp2 = strCommand.GetAt(1);
		strTemp2.MakeUpper();
		strTemp.Format("%s %s\r\n",strTemp2,CombinePara(2));
		CCommand Item;
		Item.strCommand = strTemp2;
		Item.strHelp = CombinePara(2);
		Item.dType = 3;//Custom command
		//Item.bPara = TRUE;
		//Item.bEdit = TRUE;
		//((CMIMDlg*)m_pWnd)->MessageBox(Item.strCommand+"+"+Item.strHelp);
		AddCommand(&Item);

		if(((CMIMDlg*)m_pWnd)->AddLine("Commands.txt",strTemp) == 0)
		{
			m_strTemp.Format("New command saved to \"%s\".", ((CMIMDlg*)m_pWnd)->m_strInstallDir+"\\Commands.txt");
			DisplayMessage(m_strTemp,GREEN);
		}
		return 0;
	}
	else if(strCommand.GetAt(0) == "COMMANDS")
	{
		int j;
		m_strTemp="\nAvaliable commands (alphabetical):\n";
		int max = m_oaCommands.GetSize();
		m_iTotalCom; 
		for(i = 0;i < m_iTotalCom;i++)//cycles through built-in commands
		{
			j=0;
			while (j < 4)//number of columns
			{
				if(i >= max)
					break;
				m_strTemp.Format("%s%15s",m_strTemp,GetComm(i)->strCommand);
				j++;
				i++;
			}
			i--;
			DisplayMessage(m_strTemp);
			m_strTemp="";
		}
			//m_strTemp += GetComm(i)->strCommand+"\t ";
		DisplayMessage(m_strTemp);

		//DisplayMessage("/addban, addfile, addip, adminname, allowrun, autologin, autosave, ban, browser, censor, clear, clearban, clearfile, close, connect, date, dictionary, disconnect, editprofile, exit, followurl, google, help, install, ip, kick, listen, listfile, listenable, listenport, load, max, mbox, me, modpass, name, new, password, ping, playsound, port, quit, roomname, run, save, sendfile, sound, system, tally, test, testsound, time, timestamp, user, ver, viewprofile, vote, whisper");
		m_strTemp = "\nCustom Commands:\n";
		for(i = m_iTotalCom;i < m_oaCommands.GetSize();i++)
			m_strTemp += GetComm(i)->strCommand+", ";
		DisplayMessage(m_strTemp);

		return 0;
	}
	else if(strCommand.GetAt(0) == "VIEWPROFILE")
	{
		i = MakeNum(strCommand.GetAt(1));
		if((i >= 0) && (i <= ID_MAX))
		{ 
			i--;
			((CMIMDlg*)m_pWnd)->Viewprofile(i);
		}
		else
			DisplayMessage("<!> Invalid number or range.",RED);

		return 0;
	}
	else if(strCommand.GetAt(0) == "EDITPROFILE")
	{
		if(((CMIMDlg*)m_pWnd)->m_bProfileOpen)
			return -1;
		((CMIMDlg*)m_pWnd)->m_bProfileOpen = TRUE;
		((CMIMDlg*)m_pWnd)->Profile(TRUE);	
		return 0;
	}
	else if(strCommand.GetAt(0) == "VER")
	{
		strMess.Format("\nMercury IM V%.2f for %s\n(C) 2002-%i\nRelease Date: %s %s\nMasterHD@cox.net", ((CMIMDlg*)m_pWnd)->m_dVer, ((CMIMDlg*)m_pWnd)->m_strRel, CYEAR, __DATE__, __TIME__);
		DisplayMessage(strMess);
		return 0;
	}
	else if(strCommand.GetAt(0) == "TEST")//test parameter entry
	{
		if((m_bHelp) || (para < 2))
		{
			DisplayMessage("Usage: /test [text1] [text2] [text3] [text4]");
			DisplayMessage("Description: Displays parameters (for debug purposes)");
			return 0;
		}
		strMess.Format("  TEST: %s", strCommand.GetAt(1));
		DisplayMessage(strMess);
		if(para >= 3)
			DisplayMessage(strCommand.GetAt(2));
		if(para >= 4)
			DisplayMessage(strCommand.GetAt(3));
		if(para >= 5)
			DisplayMessage(strCommand.GetAt(4));
		if(para >= 6)
			DisplayMessage(strCommand.GetAt(5));
		return 0;
	}
	else if(strCommand.GetAt(0) == "PING")
	{
		error = Ping(MakeNum(strCommand.GetAt(1), 0, 20));
		
		if(error == -2)
		{
			strMess.Format("<!> Invalid Range: %i", MakeNum(strCommand.GetAt(1)));
			DisplayMessage(strMess,RED);
		}
		else if(error == -3)
		{
			strMess.Format("<!> User %i not connected", MakeNum(strCommand.GetAt(1)));
			DisplayMessage(strMess,RED);
		}
		else if(error == -4)
			DisplayMessage("<!> Not online.",RED);
		else//OK
		{	
			//error is NOT 0-based

			if(((CMIMDlg*)m_pWnd)->m_iType == SERVER)
			{
				if(error == 0)//Server pinged self, dumb
				{
					DisplayMessage("<!> Pinged server: loopback.");
					return -1;
				}
				((CMIMDlg*)m_pWnd)->m_iPing = 0;
				((CMIMDlg*)m_pWnd)->SetTimer(ID_PING_TIMER, 10, NULL);
				strMess.Format("Pinging user %i...",error);
				DisplayMessage(strMess);				
				//Send pBuf with admin ID (-1) and PING code
				((CMIMDlg*)m_pWnd)->SendText(error-1, PING, "0");//Send ping to client
			}
			else//Client
			{
				if(((CMIMDlg*)m_pWnd)->m_pClientSocket[0]->m_iClientID == (error-1))//Pinging self
				{
					DisplayMessage("<!> Pinged self: loopback.");
					return -1;
				}

				((CMIMDlg*)m_pWnd)->m_iPing = 0;
				((CMIMDlg*)m_pWnd)->SetTimer(ID_PING_TIMER, 1, NULL);
				strMess.Format("Pinging user %i...",error);
				DisplayMessage(strMess);
				//Send pBuf with receiver ID and PING code
				strMess.Format("%i",error);
				((CMIMDlg*)m_pWnd)->SendText(0, PING, strMess);//Send ping to server
			}
		}
		return 0;
	}
	else if(strCommand.GetAt(0) == "USER")
	{
		//returns ID (not 0-based)
		error = User(MakeNum(strCommand.GetAt(1), 0, 20));

		if(error == -2)
			DisplayMessage("<!> You do not have access to this command.",RED);
		else if(para <= 2)
			DisplayMessage("<!> Missing parameter: [command]",RED);
		else if(error == -3)
		{
			strMess.Format("<!> Invalid Range: %i", MakeNum(strCommand.GetAt(1)));
			DisplayMessage(strMess,RED);
		}
		else if(error == -4)
		{
			strMess.Format("<!> User %i not connected", MakeNum(strCommand.GetAt(1)));
			DisplayMessage(strMess,RED);
		}
		else//OK
		{
			error--;//make 0-based, server = -1
			//m_strTemp.Format("error=%i",error);
			//((CMIMDlg*)m_pWnd)->MessageBox(m_strTemp);

			if(((CMIMDlg*)m_pWnd)->m_iType == CLIENT)
			{
				if(error != -1)
				{
					strCommand.InsertAt(2,"/user");
					m_strTemp.Format("%i",error+1);
					strCommand.InsertAt(3, m_strTemp);
				}
				error = 0;
			}
			//BEST METHOD for remoteness. Multiple params stay intact without quotes around entire thing.
			//EX: /user 0 /vote "This will work?" yes no maybe
			m_strTemp.Format("%s ", strCommand.GetAt(2));//put command and space after it
			//for(int i = 3;i < strCommand.GetSize();i++)//put quotes around each remaining param
			//	m_strTemp.Format("%s\"%s\" ",m_strTemp, strCommand.GetAt(i));
			//m_strTemp.Delete(m_strTemp.GetLength()-1,1);//remove last space
			m_strTemp += CombinePara(3);
			((CMIMDlg*)m_pWnd)->SendText(error, USER, m_strTemp);//Send command to user
		}
		return 0;
	}
	else//invalid command
	{
		strMess.Format("<!> Unknown command \"%s\"", strCommand.GetAt(0));
		DisplayMessage(strMess,RED);
		return -1;//failed
	}

	return 0;//ok
}

CHARFORMAT CConsole::DisplayMessage(CString strMess, COLORREF clrText, BOOL bRet)//true by default
{
	//Receives a string and displays it in the console. String is 
	//parsed into lines of 'ILINELEN' char each.
	//All lines after first are indented 4 iSpaces if "indent" is true.
	//((CMIMDlg*)m_pWnd)->UpdateData(TRUE);
	
/*	char strPara[1000];
	int iTotal = strMess.GetLength();
	CString m_strMessBox;
	
	strcpy(strPara, LPCTSTR(strMess));

	//add returns where necessary
	for(int i = (iTotal-1);i >= 0;i--)
	{
		if((strPara[i-1] != 13) && (strPara[i] == '\n'))
		{
			for(int j = (iTotal-1);j > (i-1);j--)//move all right 1
				strPara[j+1] = strPara[j];
			strPara[i] = 13;
			//if(iTotal < 960)//too many letters
				iTotal++;
		}
	}
	strPara[iTotal] = 13;
	strPara[iTotal+1] = 10;
	strPara[iTotal+2] = '\0';
*/
	//strMess=+"\n\r";
	CHARFORMAT cf;
	cf = ((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage(strMess, clrText,bRet);

	//((CMIMDlg*)m_pWnd)->m_ctlMessage.SetFocus();
	return cf;
}

int CConsole::MakeNum(CString string, int min, int max, int iStart)
{
	char *strNew = string.GetBuffer(string.GetLength());

	return MakeNum(strNew, min, max, iStart);
}

int CConsole::MakeNum(char *string, int min, int max, int iStart)
{
	char let;
	int i, top, num, start;
	//Input a string as text and number will be returned (int only)
	//up to 7 digit number.
	//returns exact number, not always 0-based.
	i = iStart;
	num = 0;
	top = strlen(string);
	
	start = -1;
	for(i = 0;i < top;i++)//5 char max
	{
		if((string[i] >= 48)&&(string[i] <= 58))
		{
			start = i;//found first digit of number
			break;
		}
	}
	if(start == -1)
		return -2;//never found any digits, exit

	if(top > 7)
		top = start+7;//up to 7 digits in number
	
	for(i = start;i < top;i++)//7 char max
	{
		if((string[i] >= 48)&&(string[i] <= 58))
		{
			num *= 10;
			let = string[i];//Get digit
			num = num + (let - 48);//Make into int.
		}
		else
			break;//end of digits
	}

	if((num < min) || (num > max))
		return -2;//invalid range
	//DEBUG:
	/*CString strTemp;
	strTemp.Format("MakeNum(\"%s\") = %i", string, num);
	DisplayMessage(strTemp);*/
	return num;
}

int CConsole::Kick(int ID, BOOL bShow) 
{
	//Input: 0-based ID
	//Output: Kicks user, returns 0 if successful 

	CString strTemp;
	if(((CMIMDlg*)m_pWnd)->m_iType != SERVER)//Not admin
		return -1;
	if((ID < 0) || (ID >= ID_MAX))//invalid range
		return -2;
	if(((CMIMDlg*)m_pWnd)->m_Users.m_iConnected[ID] != 1)//not fully connected
	{
		((CMIMDlg*)m_pWnd)->OnCloseCon(ID);//close connection
		return -3;
	}
	if(bShow)
	{
		strTemp.Format("(%i) %s has been kicked by admin.",ID+1, ((CMIMDlg*)m_pWnd)->m_Users.m_strNames[ID]);	
		DisplayMessage(strTemp, YELLOW);
	}
	//CHARFORMAT cf = m_wndShow.ShowMessage(m_strTemp,YELLOW);
	//CPkg Text(cf,m_strTemp);
	//SendMess(-1, Text);

	((CMIMDlg*)m_pWnd)->SendMess(-1, strTemp);	
	((CMIMDlg*)m_pWnd)->OnCloseCon(ID);//close connection

	return 0;
}

int CConsole::Ban(int ID, BOOL bShow) 
{
	//Input: 0-based ID
	//Output: Kicks user, returns 0 if successful 

	CString strTemp;
	if(((CMIMDlg*)m_pWnd)->m_iType != SERVER)//Not admin
		return -1;
	if((ID < 0) || (ID >= ID_MAX))//invalid range
		return -2;
	if(((CMIMDlg*)m_pWnd)->m_Users.m_iConnected[ID] != 1)//not fully connected
	{
		((CMIMDlg*)m_pWnd)->OnCloseCon(ID);//close connection
		return -3;
	}
	if(bShow)
	{
		strTemp.Format("(%i) %s has been banned by admin.",ID+1, ((CMIMDlg*)m_pWnd)->m_Users.m_strNames[ID]);	
		DisplayMessage(strTemp,YELLOW);
		((CMIMDlg*)m_pWnd)->SendMess(-1, strTemp);
	}

	((CMIMDlg*)m_pWnd)->m_strBanned.Add(((CMIMDlg*)m_pWnd)->GetIP(ID));
	Kick(ID, FALSE);
	int here = ((CMIMDlg*)m_pWnd)->m_strBanned.GetSize()-1;
	strTemp.Format("Added IP address \"%s\" to ban list.", ((CMIMDlg*)m_pWnd)->m_strBanned.GetAt(here));
	DisplayMessage(strTemp);
	return ID;//added
}

int CConsole::User(int ID)
{
	//Input: ID (NOT 0-based)
	//Output: Runs a command on User's console.

	CString strTemp;
	if((((CMIMDlg*)m_pWnd)->m_iType == CLIENT)&&(m_bConnected))//Not admin (could be mod though)
		return ID;//success
	else if((ID < 0) || (ID >= ID_MAX))//invalid range
		return -3;
	else if(((CMIMDlg*)m_pWnd)->m_Users.m_iConnected[ID-1] != 1)//not fully connected
		return -4;

	return ID;//Success
}

int CConsole::Ping(int ID)
{
	//Input: NOT 0-based ID!
	//Output: if user is valid or not
	if(!m_bConnected)
		return -4;

	CString strTemp;
	if(ID == 0)//for server
		return 0;//Success
	if((ID < 1) || (ID > ID_MAX))//invalid range
		return -2;
	if(((CMIMDlg*)m_pWnd)->m_Users.m_iConnected[ID-1] != 1)//not fully connected
		return -3;

	return ID;//Success
}

int CConsole::Vote(CString strDisp, int iChoices)
{
	CString strTemp;
	if(m_bVoteActive)
		return -1;

	//Admin only!
	for(int i = 0;i < 4;i++)
		m_dTally[i] = 0;

	m_bVoteActive = TRUE;
	m_dVoted = 0;

	DisplayMessage(strDisp);
	strTemp.Format("%i%s",iChoices, strDisp);//add # of choices to front of string
	((CMIMDlg*)m_pWnd)->SendText(-1, VOTE, strTemp);
	return 0;
}

int CConsole::Tally()
{
	CString strTemp, strFinal;
	double dPer1, dPer2;

	if(!m_bVoteActive)
		return -1;
	if(m_dVoted < 1)
		m_dVoted = 1;

	dPer1 = m_dTally[0]/m_dVoted*100;
	dPer2 = m_dTally[1]/m_dVoted*100;
	strFinal.Format("VOTING RESULTS:\n(1)  %.1f%% [%.0f votes]\n(2)  %.1f%% [%.0f votes]\n", dPer1, m_dTally[0], dPer2, m_dTally[1]);
	
	if(m_iChoices > 2)
	{
		dPer1 = m_dTally[2]/m_dVoted*100;
		strTemp.Format("(3)  %.1f%% [%.0f votes]\n", dPer1, m_dTally[2]);
		strFinal += strTemp;
	}
	if(m_iChoices > 3)
	{
		dPer1 = m_dTally[3]/m_dVoted*100;
		strTemp.Format("(4)  %.1f%% [%.0f votes]\n", dPer1, m_dTally[3]);
		strFinal += strTemp;
	}
	DisplayMessage(strFinal);
	((CMIMDlg*)m_pWnd)->SendMess(-1, strFinal);
	m_bVoteActive = FALSE;
	return 0;
}

CString CConsole::CombinePara(int iStart)
{
	CString strTemp;
	strTemp = "";
	for(int i = iStart;i < strCommand.GetSize();i++)
		strTemp.Format("%s%s ",strTemp, strCommand.GetAt(i));
	strTemp.Delete(strTemp.GetLength()-1,1);//remove last space
	strTemp = ((CMIMDlg*)m_pWnd)->Censor(strTemp);
	return strTemp;
}

int CConsole::AddCommand(CCommand *oItem, BOOL clear)
{
	try
	{
		//create it
		m_oaCommands.Add(new CCommand(oItem));
		//m_oaCommands.InsertAt(0,new CCommand(oItem));
	}
	catch (CMemoryException* perr)
	{
		//check it
		AfxMessageBox("Out of memory", MB_ICONSTOP|MB_OK);
		perr->Delete();
	}
	//clear the pointer after it has been copied into the arry
	if(clear)
		oItem->Reset();
	//Show it
	/*int iSize = m_oaItem.GetSize()-1;
	m_ctlList.InsertItem(iSize, GetItem(iSize)->m_strName);
	m_ctlList.SetItemText(iSize, 1, GetItem(iSize)->m_strVal2);
	m_ctlList.SetItemText(iSize, 2, GetItem(iSize)->m_strVal3);
	m_iTotal++;
	m_bChanges = TRUE;*/
	return 0;
}

CCommand * CConsole::GetComm(int index)
{
	//return item at index, not pointer
	return (CCommand*)m_oaCommands[index];
}

int CConsole::DeleteComm(int nItem)
{
	delete m_oaCommands[nItem];
	m_oaCommands.RemoveAt(nItem);
	return 0;
}

int CConsole::DeleteAllComm()
{
	int iSize = m_oaCommands.GetSize();	

	for(int i = 0;i < iSize;i++)
		delete m_oaCommands[i];
	m_oaCommands.RemoveAll();
	return 0;
}

