// MIMDlg.cpp : implementation file
//

#include "stdafx.h"//need to edit stdafx.h for win98 config
#include "MIM.h"
#include "MIMDlg.h"
#include "Request.h"
#include "mmsystem.h"
#include "colorbutton.h"

#pragma warning( once : 4996 )//disable multiple warnings of the same type; just add it to the number list
//#define _CRT_SECURE_NO_WARNINGS //disable the strcpy and related function warnings
//#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define WM_UPDATECHARFORMAT WM_USER + 1

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
IMPLEMENT_SERIAL(CServerEntry, CObject,1)

class CAboutDlg : public CDialog
{
public:
	CDialog *m_pWnd;
	double m_dVer;
	CString m_strRel;
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_strDate;
	CString	m_strVer;
	CString	m_strCopyright;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHelpEula();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CColorButton m_ctlEULA;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	m_pWnd = NULL;
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strDate = _T("");
	m_strVer = _T("");
	m_strCopyright = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_DATE, m_strDate);
	DDX_Text(pDX, IDC_VERSION, m_strVer);
	DDX_Text(pDX, IDC_COPYRIGHT, m_strCopyright);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EULA, m_ctlEULA);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_EULA, OnHelpEula)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMIMDlg dialog

CMIMDlg::CMIMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMIMDlg::IDD, pParent)
	, m_bPSL_Exe(FALSE)
	, m_bServerInfoRequest(FALSE)
	, m_iInfoRequestIndex(0)
	, m_pListenSocket(this, 0)
{
	//{{AFX_DATA_INIT(CMIMDlg)
	m_strUsers = _T("");
	m_strCurRoom = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);//Icon for tray
	m_hAdminIcon = AfxGetApp()->LoadIcon(IDI_ICON3);//Icon for tray
	m_hMessIcon = AfxGetApp()->LoadIcon(IDI_ICON6);//Icon for client message tray
	m_hMessAIcon = AfxGetApp()->LoadIcon(IDI_ICON7);//Icon for admin message tray
	//MessageBox("Construction complete");
}

void CMIMDlg::DoDataExchange(CDataExchange* pDX)
{	
	//MessageBox("DoDataExchange");

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMIMDlg)
	DDX_Control(pDX, IDC_PUBPAD, m_ctlPubPad);
	DDX_Control(pDX, IDC_USERLIST, m_ctlUserList);
	DDX_Control(pDX, IDC_SIMPLE, m_ctlSimple);
	DDX_Control(pDX, IDC_MBOX, m_ctlMBox);
	DDX_Control(pDX, IDC_KICK, m_ctlKick);
	DDX_Control(pDX, IDC_TRAY, m_ctlTray);
	DDX_Control(pDX, IDC_PREV, m_ctlPrev);
	DDX_Control(pDX, IDC_EDITPROFILE, m_ctlEditProfile);
	DDX_Control(pDX, IDC_OPTIONS, m_ctlServer);
	DDX_Control(pDX, IDC_SENDFILE, m_ctlSendFile);
	DDX_Control(pDX, IDC_VIEWPROFILE, m_ctlViewProfile);
	DDX_Control(pDX, IDC_IGNORE, m_ctlIgnore);
	DDX_Control(pDX, IDC_SAVEMESS, m_ctlSaveMess);
	DDX_Control(pDX, IDC_CLEAR, m_ctlClear);
	DDX_Control(pDX, IDC_DISCONNECT, m_ctlDisconnect);
	DDX_Control(pDX, IDC_CONNECT, m_ctlConnect);
	DDX_Control(pDX, IDC_SETTINGS, m_ctlSettings);
	DDX_Control(pDX, IDC_SEND, m_btnSend);
	DDX_Control(pDX, IDC_ROOM, m_wndShow);
	DDX_Control(pDX, IDC_MESSAGE, m_wndInput);
	DDX_Text(pDX, IDC_USERS, m_strUsers);
	DDX_Text(pDX, IDC_ROOMNAME, m_strCurRoom);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMIMDlg, CDialog)
	//{{AFX_MSG_MAP(CMIMDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND, OnBtnSend)
	ON_COMMAND(IDC_CHAR_BOLD, OnCharBold)
	ON_COMMAND(IDC_CHAR_COLOR, OnCharColor)
	ON_COMMAND(IDC_CHAR_ITALIC, OnCharItalic)
	ON_COMMAND(IDC_CHAR_STRIKEOUT, OnCharStrikeout)
	ON_COMMAND(IDC_CHAR_UNDERLINE, OnCharUnderline)
	ON_UPDATE_COMMAND_UI(IDC_CHAR_ITALIC, OnUpdateCharItalic)
	ON_UPDATE_COMMAND_UI(IDC_CHAR_STRIKEOUT, OnUpdateCharStrikeout)
	ON_UPDATE_COMMAND_UI(IDC_CHAR_UNDERLINE, OnUpdateCharUnderline)
	ON_UPDATE_COMMAND_UI(IDC_CHAR_BOLD, OnUpdateCharBold)
	ON_COMMAND(ID_FILE_LOADSETTINGS, OnFileLoadsettings)
	ON_COMMAND(ID_FILE_RESTORESETTINGS, OnFileRestoresettings)
	ON_COMMAND(ID_FILE_SAVEMESSAGES, OnFileSavemessages)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(ID_HELP_HELP, OnHelpHelp)
	ON_COMMAND(ID_HELP_PATCH, OnHelpPatch)
	ON_COMMAND(ID_HELP_README, OnHelpReadme)
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_BN_CLICKED(IDC_SETTINGS, OnSettings)
	ON_BN_CLICKED(IDC_OPTIONS, OnServerOptions)
	ON_BN_CLICKED(IDC_CONNECT, OnBConnect)
	ON_BN_CLICKED(IDC_DISCONNECT, OnBDisconnect)
	ON_BN_CLICKED(IDC_EDITPROFILE, OnBEditprofile)
	ON_BN_CLICKED(IDC_CLEAR, OnBClear)
	ON_BN_CLICKED(IDC_VIEWPROFILE, OnViewprofile)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_PREV, OnPrev)
	ON_COMMAND(ID_HELP_EULA, OnHelpEula)
	ON_COMMAND(ID_USER_KICK, OnKick)
	ON_BN_CLICKED(IDC_IGNORE, OnIgnore)
	ON_COMMAND(ID_USER_STATUS_AWAY, OnAway)
	ON_COMMAND(ID_USER_STATUS_AVAILABLE, OnAvailable)
	ON_COMMAND(ID_USER_SENDFILE, OnSendfile)
	ON_REGISTERED_MESSAGE(wm_Find, OnFind )
	ON_REGISTERED_MESSAGE(WM_ICONNOTIFY, OnIconNotify)
	ON_COMMAND(ID_ICON_EXIT, OnIconShutdown)
	ON_COMMAND(ID_ICON_RESTORE, OnIconRestore)
	ON_BN_CLICKED(IDC_SAVEMESS, OnSavemess)
	ON_BN_CLICKED(IDC_TRAY, OnMinicon)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_SHOWMENU_COPY, OnShowmenuCopy)
	ON_COMMAND(ID_SHOWMENU_FOLLOWURL, OnShowmenuFollowurl)
	ON_COMMAND(ID_INPUTMENU_UNDO, OnInputmenuUndo)
	ON_COMMAND(ID_INPUTMENU_CUT, OnInputmenuCut)
	ON_COMMAND(ID_INPUTMENU_COPY, OnInputmenuCopy)
	ON_COMMAND(ID_INPUTMENU_PASTE, OnInputmenuPaste)
	ON_COMMAND(ID_INPUTMENU_SELECTALL, OnInputmenuSelectall)
	ON_BN_CLICKED(IDC_MBOX, OnMbox)
	ON_BN_CLICKED(IDC_SIMPLE, OnSimple)
	ON_COMMAND(ID_USER_BAN, OnUserBan)
	ON_NOTIFY(NM_DBLCLK, IDC_USERLIST, OnDblclkUserlist)
	ON_WM_ACTIVATE()
	ON_NOTIFY(EN_MSGFILTER, IDC_ROOM, OnMsgfilterRoom)
	ON_EN_CHANGE(IDC_MESSAGE, OnChangeMessage)
	ON_COMMAND(ID_SHOWMENU_FIND, Find)
	ON_COMMAND(ID_GOOGLE, OnMenuGoogle)
	ON_COMMAND(ID_DICTIONARY, OnMenuDictionary)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_VIEWHISTORY, OnFileViewhistory)
	ON_COMMAND(ID_FILE_VIEWQUOTES, OnFileViewquotes)
	ON_COMMAND(ID_FILE_VIEWCOMMANDS, OnFileViewcommands)
	ON_COMMAND(ID_FILE_LOADCOMMANDS, OnFileLoadcommands)
	ON_COMMAND(ID_FILE_LOADBOUNCER, OnFileLoadBouncer)
	ON_COMMAND(ID_FILE_OPENBOUNCER, OnFileOpenBouncer)
	ON_COMMAND(ID_FILE_OPENDOWNLOADFOLDER, OnFileOpendownloadfolder)
	ON_COMMAND(ID_FILE_LOADSERVERTXT, OnFileLoadservertxt)
	ON_COMMAND(ID_FILE_VIEW_SERVERTXT, OnFileViewServertxt)
	ON_UPDATE_COMMAND_UI(ID_NETWORK_CONNECTIONSETTINGS, OnUpdateNetworkConnectionsettings)
	ON_UPDATE_COMMAND_UI(ID_NETWORK_SERVEROPTIONS, OnUpdateNetworkServeroptions)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_PUBPAD, OnPubpad)
	ON_WM_DROPFILES()
	ON_COMMAND(IDC_CHAR_BACK, OnCharBack)
	ON_COMMAND(ID_FILE_SAVESETTINGS, OnSaveSettings)
	ON_COMMAND(ID_NETWORK_CONNECTIONSETTINGS, OnSettings)
	ON_COMMAND(ID_NETWORK_SERVEROPTIONS, OnServerOptions)
	ON_COMMAND(ID_USER_VIEWPROFILE, OnViewprofile)
	ON_COMMAND(ID_USER_MYSTATUS_DISCONNECT, OnBDisconnect)
	ON_COMMAND(ID_USER_IGNORE, OnIgnore)
	ON_BN_CLICKED(IDC_SENDFILE, OnSendfile)
	ON_COMMAND(ID_ICON_STATUS_AVAILABLE, OnAvailable)
	ON_COMMAND(ID_ICON_STATUS_AWAY, OnAway)
	ON_COMMAND(ID_ICON_STATUS_DISCONNECT, OnBDisconnect)
	ON_COMMAND(ID_ICON_STATUS_CONNECT, OnBConnect)
	ON_BN_CLICKED(IDC_KICK, OnKick)
	ON_COMMAND(ID_USER_MESSAGE, OnMbox)
	ON_COMMAND(ID_CTRL_F, Find)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	//ON_MESSAGE(WM_KICKIDLE,OnKickIdle)
	//ON_MESSAGE(WM_UPDATECHARFORMAT,UpdateCharFormat4Input)  //FIX: I tried just commenting because ON_MESSAGE might not be necessary
	ON_CBN_SELENDOK(IDC_FONTNAME, OnSelectFontName)
	ON_CBN_SELENDOK(IDC_FONTSIZE, OnSelectFontSize)	
//	WM_ERASEBKGND(ID_STATUSBAR, OnBkgndStatus)

ON_COMMAND(ID_NETWORK_MASTERSERVER, &CMIMDlg::OnNetworkMasterserver)
ON_COMMAND(ID_SAVE_MASTER, &CMIMDlg::OnSaveMaster)
ON_COMMAND(ID_LOAD_MASTER, &CMIMDlg::OnLoadMaster)
ON_COMMAND(ID_USER_SENDCLIPBOARD, &CMIMDlg::OnUserSendclipboard)
ON_COMMAND(ID_OPEN_CLIPBOARD, &CMIMDlg::OnOpenClipboard)
ON_COMMAND(ID_SAVE_CLIPBOARD, &CMIMDlg::OnSaveClipboard)
ON_COMMAND(ID_LOAD_CLIPBOARD, &CMIMDlg::OnLoadClipboard)
ON_COMMAND(ID_SAVE_CLIPBOARD_QUICK, &CMIMDlg::OnSaveClipboardQuick)
ON_COMMAND(ID_SAVE_MSL, &CMIMDlg::OnSaveMsl)
ON_COMMAND(ID_LOAD_MSL, &CMIMDlg::OnLoadMsl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMIMDlg message handlers

static UINT auIDStatusBar[] = {
	ID_SEPARATOR
};

BOOL CMIMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
//Set VER HERE!
	m_dVer = 3.10;	//current newest version
	m_dLowVer = 3.00;	//lowest MIM version that can communicate properly
	m_dLowFileVer = 3.00;//for MIM.dat file, compatible with older version
	m_Console.m_dVer = m_dVer;
	m_bDebug = FALSE;//display detailed debug info in functions

	//Window/Control Colors:
	//m_bitmap.LoadBitmap(IDB_BG);//also can be used to load a bitmap from a file
	//m_BrushBG.CreatePatternBrush(&m_bitmap);
	m_clrBG = RGB(0, 0, 0);//common color to be used for BG of all controls AND text BG	
	m_clrFore = RGB(0,255,0);//common color to be used for all text controls
	//m_clrFore = RGB(164,176,238);
	m_BrushBG.CreateSolidBrush(m_clrBG);//brush used to paint the BG of all controls. Must be paseed to child windows (SettingsDlg, etc.)
	//create/show intro window
	m_wndIntro.dVer = m_dVer;
	m_wndIntro.Create(IDD_INTRO, this);
	m_wndIntro.ShowWindow(SW_SHOW);
	AfxBeginThread(OnIntroThread,this);
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	//AfxInitRichEdit();
//Init vars
	#ifdef REL2000//see stdafx.h
	m_strRel = "Win2K/XP/Vista/7";
	#else
	m_strRel = "Win98";
	#endif

	CreateComboBoxes();
	//Create Text tool bar
	if(!m_wndRichTextBar.Create(this, WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC,
		IDR_RICHTEXTBAR))
	{
		TRACE0("Failed to create text bar!\n");
		return -1;
	}
	//TRACE(_T("Executable filename = %s\n"), AfxGetApp()->m_pszExeName);//this would print the exe's filename

//status bar
	RECT trect;
	if(m_statusBar.Create(WS_CHILD|WS_VISIBLE, trect, this,ID_STATUSBAR) != 0)
	{
		//arbitrary values, they are set in Reposition();
		int parts[3];
		parts[0] = 250;
		parts[1] = 65;
		parts[2] = 50;
		m_statusBar.SetParts(3,parts);
		m_statusBar.SetBkColor(RGB(0,0,0));
		//HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_GREEN), IMAGE_ICON, SM_CXICON, SM_CYICON, 0);
		//m_statusBar.SetIcon(0, hIcon);
		//m_statusBar.SetTextColor(RGB(0,0,0));
	}
	else
		MessageBox("Status Bar failed");
	m_iMaxAutoSize = 8*1024*1024;//maximum file size for the autosave history in Bytes

	m_MasterListDlg.SetMIMParent(this);//important that MasterListDlg dialog be able to call the MIM functions
	m_PubPadDlg.m_bFirst = TRUE;
	m_bAgree = FALSE;
	m_bPubPad = FALSE;
	m_FirstTime = CTime::GetCurrentTime();
	m_iByteRec = 0;
	m_iByteSent = 0;
	m_Console.m_bConnected = FALSE;
	m_iAutoSaveHis = 180;
	m_iTyping = 0;
	m_bForceAway = FALSE;
	m_wndInput.m_pDlg = this;
	m_bTrayNotify = FALSE;
	m_bTrayed = FALSE;
	m_iBounceTimer = 0;
	m_bServerInfoRequest = FALSE;
	m_bPublicInfoRequest = FALSE;
	m_bMasterInfoRequest = FALSE;
	m_bStopServerInfoRequest = FALSE;
	SetDefaults();

	m_wndInput.LimitText(PACKETMAX-PACKETGAP-50);//window input text max size

	m_wndShow.SetEventMask(ENM_KEYEVENTS | ENM_CHANGE | ENM_MOUSEEVENTS);
	m_wndInput.SetEventMask(ENM_CHANGE);
	
	// with the ENM_CHANGE
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon (tray)

	char strSys[300];

	GetSystemDirectory(strSys, 300);
	m_strSysDir = strSys;
	GetCurrentDirectory(300,strSys);
	m_strInstallDir = strSys;

	//SHCreateShortcut(TEXT("\\Windows\\myAppShortcut.lnk"), TEXT("\\Windows\\Program Files\\myApp.exe"));

//Load bitmaps for buttons
	CBitmap Bit;
	m_iFontSize = 10;
	Bit.LoadBitmap(IDB_SETTINGS);
	m_ctlSettings.SetBitmap(Bit);
	Bit.Detach();
	Bit.LoadBitmap(IDB_CONNECT);
	m_ctlConnect.SetBitmap(Bit);
	Bit.Detach();
	Bit.LoadBitmap(IDB_SERVEROP);
	m_ctlServer.SetBitmap(Bit);
	Bit.Detach();
	Bit.LoadBitmap(IDB_DISCONNECT);
	m_ctlDisconnect.SetBitmap(Bit);
	Bit.Detach();
	Bit.LoadBitmap(IDB_CMESS);
	m_ctlClear.SetBitmap(Bit);
	Bit.Detach();
	Bit.LoadBitmap(IDB_SMESS);
	m_ctlSaveMess.SetBitmap(Bit);
	Bit.Detach();
	Bit.LoadBitmap(IDB_IGNORE);
	m_ctlIgnore.SetBitmap(Bit);
	Bit.Detach();
	Bit.LoadBitmap(IDB_VIEWPROFILE);
	m_ctlViewProfile.SetBitmap(Bit);
	Bit.Detach();
	Bit.LoadBitmap(IDB_SENDFILE);
	m_ctlSendFile.SetBitmap(Bit);
	Bit.Detach();
	Bit.LoadBitmap(IDB_EDITPROFILE);
	m_ctlEditProfile.SetBitmap(Bit);
	Bit.Detach();	
	Bit.LoadBitmap(IDB_SEND);
	m_btnSend.SetBitmap(Bit);
	Bit.Detach();
	Bit.LoadBitmap(IDB_PREV);
	m_ctlPrev.SetBitmap(Bit);
	Bit.Detach();
	Bit.LoadBitmap(IDB_TRAY);
	m_ctlTray.SetBitmap(Bit);
	Bit.Detach();
	Bit.LoadBitmap(IDB_SIMPLE);
	m_ctlSimple.SetBitmap(Bit);
	Bit.Detach();
	Bit.LoadBitmap(IDB_PUBPAD);
	m_ctlPubPad.SetBitmap(Bit);
	Bit.Detach();
	Bit.LoadBitmap(IDB_KICK);
	m_ctlKick.SetBitmap(Bit);
	Bit.Detach();
	Bit.LoadBitmap(IDB_MBOX);
	m_ctlMBox.SetBitmap(Bit);
	Bit.Detach();

//Tooltips
	m_tooltip.Create (this);
	m_tooltip.Activate (TRUE);
	m_tooltip.AddTool (GetDlgItem (IDC_SETTINGS), IDS_CONSETTINGS);
	m_tooltip.AddTool (GetDlgItem (IDC_OPTIONS), IDS_SERVEROPTIONS);
	m_tooltip.AddTool (GetDlgItem (IDC_CONNECT), "Connect to server <or> Create server");
	m_tooltip.AddTool (GetDlgItem (IDC_DISCONNECT), "Close your current connection");
	m_tooltip.AddTool (GetDlgItem (IDC_CLEAR), "Clear all current messages without saving them");
	m_tooltip.AddTool (GetDlgItem (IDC_SAVEMESS), "Saves and clears all current messages");
	m_tooltip.AddTool (GetDlgItem (IDC_EDITPROFILE), "Edit your profile details");
	m_tooltip.AddTool (GetDlgItem (IDC_IGNORE), "Ignore all messages from a user");
	m_tooltip.AddTool (GetDlgItem (IDC_VIEWPROFILE), "View a user's profile");
	m_tooltip.AddTool (GetDlgItem (IDC_SENDFILE), "Sends files to a user");
	m_tooltip.AddTool (GetDlgItem (IDC_USERLIST), "List of users in the chatroom");
	m_tooltip.AddTool (GetDlgItem (IDC_KICK), "Kicks a user");
	m_tooltip.AddTool (GetDlgItem (IDC_MBOX), "Sends a message pop-up window");
	m_tooltip.AddTool (GetDlgItem (IDC_PUBPAD), "Open public drawing pad.");
	m_tooltip.AddTool (GetDlgItem (IDC_ROOM), "Chatroom messages");
	m_tooltip.AddTool (GetDlgItem (IDC_SEND), "Send your message text (Enter key)");
	m_tooltip.AddTool (GetDlgItem (IDC_PREV), "Cycles through previously entered messages (~ key)");
	m_tooltip.AddTool (GetDlgItem (IDC_TRAY), "Hide MIM to the system tray");
	m_tooltip.AddTool (GetDlgItem (IDC_MESSAGE), "Your message");
	m_tooltip.AddTool (GetDlgItem (IDC_SIMPLE), "Hide/show settings buttons and user list");
	

//FIX:
	m_ctlUserList.InsertColumn(0,"Users");

	/*CRect rRoom;
	GetWindowRect(rRoom);//get dimensions of inner window for placing controls
	CRect rect;
	rect.SetRect(rRoom.Width()-167, 44, 154, 15);

	m_wndContactList.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL,rect,this,IDC_ICONLIST);*/

//Initialize all User data
	for(int i = 0;i < ID_MAX;i++)
	{
		m_Users.m_iConnected[i] = 0;
		m_Users.m_iImage[i] = 0;
		m_bIgnore[i] = FALSE;
		m_Users.m_strNames[i][0] = 0;
		m_bSharing[i] = FALSE;
		m_iUserOK[i] = 0;
	}
	m_iOKTimer = 60;
	m_Users.m_iTotalUsers = 0;
	m_bShowSet = TRUE;
	m_bShowSend = FALSE;
	m_bProfileOpen = FALSE;

//User List
	InitImageLists();//set imagelist for the listctrl
	m_ctlUserList.SetImageList(&m_imgContactList,LVSIL_SMALL);
	m_ctlUserList.SetIconSpacing(133,40);
	//m_MasterListDlg.m_ctlMList2.SetImageList(&m_imgMasterList,LVSIL_SMALL);//This does not work, must be done from parent CDialog
	m_ctlUserList.SetColumnWidth(0,250);
	//m_ctlUserList.SetBkImage(HBit);
	//m_ctlUserList.SetTextColor(RGB(164,176,238));//the text and textBG colors are set within the CIconListCtrl drawing method
	m_bActive = TRUE;//set Z-order to 0?
//SOCKETS
	for(int i = 0;i < ID_MAX;i++)
	{
		m_pClientSocket[i] = NULL;
		m_pSocketFile[i] = NULL;
	}
	//m_pListenSocket = NULL;
	m_pInfoReqSocket = NULL;
	m_pInfoReqSocketFile = NULL;

//Opening text
	m_ctlUserList.SetParent(this);
	m_Console.SetParent(this);
	m_Console.Init();
	m_strPrev.Add("");
	m_iFlood = 0;
	m_iPrev = 0;
	m_iTPrev = 0;
	m_bProfileOpen = FALSE;
	m_iProfile = 0;
	m_Console.m_bAccepted = FALSE;
	m_Users.m_iMax = 10;
	m_strUsers = "Offline";
	//m_statusBar.SetIcon(1, hIcon);
	UpdateData(FALSE);
	GetTimeStamp();
	m_bFind = FALSE;
	m_strDate = GetDate();
	m_iInfoRequestTimer = 0;

	//m_strTemp.Format("\r\n\r\n    Mercury Instant Messenger V%.2f ßeta [%s %s]\r\n      Copyright (C) 2006       Visit: http://members.cox.net/masterhd/MIM.htm\r\n        By: Dennis Dunn (MasterHD)\r\n\r\n%s\r\n>>", m_dVer, __DATE__, __TIME__,m_strDate);
	//m_wndShow.ShowMessage(m_strTemp,RGB(164,176,238));

	//m_strTemp.Format("\r\n\r\n    Mercury Instant Messenger V%.2f ßeta [%s %s]", m_dVer, __DATE__, __TIME__);

	m_strTemp.Format("\r\n\r\n    Mercury Instant Messenger V%.2f [%s %s]", m_dVer, __DATE__, __TIME__);
	m_wndShow.ShowMessage(m_strTemp,RGB(164,176,238),FALSE);
	m_strTemp.Format("\r\n      Copyright (C) 2006");
	m_wndShow.ShowMessage(m_strTemp,RGB(124,140,231),FALSE);
	m_strTemp.Format("\r\n        By: Dennis Dunn (MasterHD)");
	m_wndShow.ShowMessage(m_strTemp,RGB(80,101,222),FALSE);
	m_strTemp.Format("\r\n          Visit: http://members.cox.net/masterhd/MIM.htm");
	m_wndShow.ShowMessage(m_strTemp,RGB(48,73,216),FALSE);
	m_strTemp.Format("\r\n\r\n%s\r\n> ",m_strDate);
	m_wndShow.ShowMessage(m_strTemp,LBLUE,FALSE);
	m_lLastSaveHisPos = 182;//IMPORTANT: set this value to the length of intro text that will not be saved to message history
	
	m_strCurRoom.Format("MIM v%.2f for %s", m_dVer,m_strRel);
	SetWindowText(m_strCurRoom);
	//m_wndShow.SetSel(m_wndShow.GetTextLength(),m_wndShow.GetTextLength());
	SetDefaultCharFormat4Input();

	//Persistance
	LoadSettings();//should be near the end of OnInitDlg() in case of OnAgree() being called.

	OnKickIdle(NULL,NULL);//update toolbar buttons

	if(m_iType == CLIENT)
		m_strTemp.Format("Port: %i", m_iPort);
	else
		m_strTemp.Format("Port: %i", m_iListenPort);
	SetStatusText("Offline",1,IDI_RED);
	SetStatusText(m_strTemp,2);

	//refresh screen
	m_bSimple = !m_bSimple;
	OnSimple();
	SetTimer(ID_TYPE_TIMER,1000,NULL);

	if(m_bAutoLogin)
		OnBConnect();

	if(m_bBouncer)
		LoadBouncer();

	LoadMaster();//load the saved master server list
	LoadMSL();
	m_wndInput.SetFocus();
	//OnMinicon();//doesnt work here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMIMDlg::SetStatusText(CString strText, int iPane, int iIconRes)
{
	//set the text and icon of the status bar, uses small icon size
	//dont pass iIconRes, or leave it as 0 for no icon
	if(iIconRes != 0)
	{
		HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(iIconRes), IMAGE_ICON, SM_CXICON, SM_CYICON, 0);
		m_statusBar.SetIcon(iPane, hIcon);
	}
	m_statusBar.SetText(strText, iPane,0);
	if(iPane == 0)//set the tooltip text to the first pane only
		m_tooltip.AddTool(&m_statusBar, strText);
}

int CMIMDlg::CreateComboBoxes()
{
//Combo Boxes
	//for the combo boxes: FontName and FontSize
	//general window stype
	UINT nWndStype = WS_VISIBLE|WS_TABSTOP|WS_VSCROLL;
	HGDIOBJ hFont = GetStockObject(DEFAULT_GUI_FONT);
	CFont font;
	font.Attach(hFont);

	CClientDC dc(this);
	dc.SelectObject(font);

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	int cxChar = tm.tmAveCharWidth;
	int cyChar = tm.tmHeight + tm.tmExternalLeading;

	CRect rect;

	rect.left = 8;
	rect.top = 245;
	rect.right = rect.left + (LF_FACESIZE+2)*cxChar;
	rect.bottom = rect.top + 16*cyChar;

	if(!m_cmbFontName.Create(nWndStype|CBS_DROPDOWNLIST|CBS_SORT,rect,this,IDC_FONTNAME))
	{
		TRACE0("Failed to create the font name combo list!\n");
		return -1;
	}
	
	rect.right = rect.left + 8*cxChar;
	rect.bottom = rect.top + 16*cyChar;
	//SetButtonInfo(CommandToIndex(IDC_FONTSIZE),IDC_FONTSIZE,TBBS_SEPARATOR,rect.Width());
	if(!m_cmbFontSize.Create(nWndStype|CBS_DROPDOWNLIST,rect,this,IDC_FONTSIZE))
	{
		TRACE0("Failed to create the font size combo list!\n");
		return -1;
	}
	m_cmbFontSize.LimitText(4);

//Position combo boxes and toolbar (X,Y,W,H)
	m_cmbFontName.MoveWindow(10,296,150,15);
	m_cmbFontSize.MoveWindow(163,296,60,100);
	//m_wndRichTextBar.MoveWindow(230,294,140,20);

	m_cmbFontName.SetFont(&font);
	m_cmbFontSize.SetFont(&font);

	//fill
	::EnumFontFamilies(dc.m_hDC,NULL,(FONTENUMPROC)EnumFontFamProc,(LPARAM )this);

// Declare const array of font sizes
	const static int nFontSizes[] = 
	{8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 48, 72};

	int nCount = sizeof(nFontSizes)/sizeof(nFontSizes[0]);
	CString sSize;
	for(int i=0;i<nCount;i++)
	{
		sSize.Format("%d",nFontSizes[i]);
		m_cmbFontSize.AddString(sSize);
	}
	return 0;
}

UINT CMIMDlg::OnIntroThread(LPVOID pParam)//intro splash screen
{
	CMIMDlg *pPointer;
	pPointer = ((CMIMDlg*)pParam);

	pPointer->m_wndIntro.Init();

	return 0;
}

void CMIMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.m_dVer = m_dVer;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMIMDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		//Useless code? I dont know where this came from.[6-3-06]
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

BOOL CMIMDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	//cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	
	return NULL;//CExpandingDialog::PreCreateWindow(cs);
}

BOOL CMIMDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	//MessageBox("PreTranslateMessage");
	m_tooltip.RelayEvent(pMsg);
	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
		return TRUE; 
	return NULL;//CExpandingDialog::PreTranslateMessage(pMsg);
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMIMDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMIMDlg::OnBtnSend2()
{
	OnBtnSend();
}

void CMIMDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	//Called when window is clicked or minimized, etc.
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	if((nState == WA_INACTIVE) || (bMinimized))
		m_bActive = FALSE;
	else
		m_bActive = TRUE;

	return;
}

void CMIMDlg::OnBtnSend() 
{
	//Set font to currently selected
	m_wndInput.SetSel(0,-1);
 	m_wndInput.SetWordCharFormat(m_cfInput);
	//UpdateCharFormat4In();

	//allow outside classes to call this private function
	if(m_iFlood > 0)
		return;

	OnAvailable();//If away, set status to Available.

	CString strTemp;
	m_wndInput.GetWindowText(strTemp);
	m_wndInput.SetSel(0,m_wndInput.GetTextLength());
	m_wndInput.Clear();
	if(strTemp == "")
		return;
	EndTyping();
	OnBSend(strTemp);
}

void CMIMDlg::OnBSend(CString strSend, BOOL bRemote) 
{
	//BUTTON PRESSED
	//UpdateData(TRUE);

	//Send a text message exaclty as it will appear, no SCREEN NAME in front
	//bRemote is true if run from a mod or admin
	if(m_iFlood > 0)
		return;

	m_iPrev = 1;
	m_strPrev.SetSize(m_iTPrev+1);
	m_strPrev.InsertAt(1,strSend);//Add to front of list
	m_iTPrev++;
	if(m_iTPrev > 20)
		m_iTPrev = 20;
	GetDlgItem(IDC_PREV)->EnableWindow(TRUE);

	int iLen = strSend.GetLength();
	//Console commands
	if((strSend.GetAt(0) == '/') || (!m_bShowSend))
	{
		if(m_bShowSend)
			m_wndShow.ShowMessage("> ",LBLUE,FALSE);

		m_Console.Console(strSend, bRemote);
		if(!m_bShowSend)
			m_wndShow.ShowMessage("> ",LBLUE,FALSE);
	}
	//else if(!m_bShowSend)//not connected, just diplay text
	//	m_wndShow.ShowMessage(strSend+"\r\n> ", LBLUE, FALSE);
	else if(m_iType == SERVER)
	{
		//Send the servers message to clients after formatting it
		m_strTemp = Censor(strSend);
		if(m_bTimeStamp)
			m_strTime = GetTimeStamp();
		else
			m_strTime = "";
		strSend.Format("%s (A) %s:      %s", m_strTime, m_strAdminName, m_strTemp);

		SendMess(-1, strSend, MESG, TRUE);
		return;
	}
	else//client sends to server, DONT display it on screen
		SendMess(0, strSend, MESG, FALSE);

	if(m_Console.m_bConnected)
		SetFlood(m_iFlood, 1);
	//UpdateData(FALSE);
	//m_wndInput.SetSel(0,m_wndInput.GetTextLength());
	//m_wndInput.Clear();
	UpdateData(FALSE);
	return;
}

void CMIMDlg::OnUpdateCharItalic(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//pCmdUI->Enable(m_bConnected);
	pCmdUI->SetCheck(m_bCharItalic);	
}

void CMIMDlg::OnUpdateCharStrikeout(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//pCmdUI->Enable(m_bConnected);
	pCmdUI->SetCheck(m_bCharStrikeOut);	
}

void CMIMDlg::OnUpdateCharUnderline(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//pCmdUI->Enable(m_bConnected);
	pCmdUI->SetCheck(m_bCharUnderline);
}

void CMIMDlg::OnUpdateCharBold(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bCharBold);
}

//this one and the ones followed are for the rich text part - yh

void CMIMDlg::OnCharColor()
{
	CColorDialog dlg;

	if(dlg.DoModal() == IDOK )
		m_clrChar = dlg.GetColor();
	m_cfInput.dwMask |= CFM_COLOR;
	m_cfInput.crTextColor = m_clrChar;

	CString strText;
	m_wndInput.GetWindowText(strText);
	int iLength = m_wndInput.GetWindowTextLength();
	m_wndInput.SetSel(0,iLength);
	m_wndInput.SetWordCharFormat(m_cfInput);

	m_wndInput.SetSel(iLength,iLength);
}

void CMIMDlg::OnCharBack()
{
	CColorDialog dlg;

	if(dlg.DoModal() == IDOK )
	{
		m_clrBack = dlg.GetColor();
		SyncChanges();
	}
}

void CMIMDlg::OnCharBold() 
{

	m_bCharBold = !m_bCharBold;

	m_cfInput.dwMask |= CFM_BOLD;
	m_cfInput.dwEffects = m_bCharBold ? (m_cfInput.dwEffects | CFE_BOLD) 
		:(m_cfInput.dwEffects& ~CFE_BOLD);

	CString strText;
	m_wndInput.GetWindowText(strText);
	int iLength = m_wndInput.GetWindowTextLength();
	m_wndInput.SetSel(0,iLength);
 	m_wndInput.SetWordCharFormat(m_cfInput);

	m_wndInput.SetSel(iLength,iLength);
	OnKickIdle(NULL,NULL);
}

void CMIMDlg::OnCharItalic()
{
	// TODO: Add your command handler code here
	m_bCharItalic = !m_bCharItalic;
	m_cfInput.dwMask |= CFM_ITALIC;
	m_cfInput.dwEffects = m_bCharItalic ? (m_cfInput.dwEffects | CFE_ITALIC) 
		:(m_cfInput.dwEffects& ~CFE_ITALIC);
	
	CString strText;
	m_wndInput.GetWindowText(strText);
	int iLength = m_wndInput.GetWindowTextLength();
	m_wndInput.SetSel(0,iLength);
 	m_wndInput.SetWordCharFormat(m_cfInput);

	m_wndInput.SetSel(iLength,iLength);
	OnKickIdle(NULL,NULL);
}

void CMIMDlg::OnCharStrikeout()
{
	m_bCharStrikeOut = !m_bCharStrikeOut;

	m_cfInput.dwMask |= CFM_STRIKEOUT;
	m_cfInput.dwEffects = m_bCharStrikeOut ? (m_cfInput.dwEffects | CFE_STRIKEOUT) 
		:(m_cfInput.dwEffects& ~CFE_STRIKEOUT);
 	m_wndInput.SetWordCharFormat(m_cfInput);
	
	CString strText;
	m_wndInput.GetWindowText(strText);
	int iLength = m_wndInput.GetWindowTextLength();
	m_wndInput.SetSel(0,iLength);
 	m_wndInput.SetWordCharFormat(m_cfInput);
	m_wndInput.SetSel(iLength,iLength);
	OnKickIdle(NULL,NULL);
}

void CMIMDlg::OnCharUnderline()
{
	// TODO: Add your command handler code here
	m_bCharUnderline = !m_bCharUnderline;
	
	m_cfInput.dwMask |= CFM_UNDERLINE;
	m_cfInput.dwEffects = m_bCharUnderline ? (m_cfInput.dwEffects | CFE_UNDERLINE) 
		:(m_cfInput.dwEffects& ~CFE_UNDERLINE);
 	m_wndInput.SetWordCharFormat(m_cfInput);
	
	CString strText;
	m_wndInput.GetWindowText(strText);
	int iLength = m_wndInput.GetWindowTextLength();
	m_wndInput.SetSel(0,iLength);
 	m_wndInput.SetWordCharFormat(m_cfInput);
//	if you don't like a highlighted selected text, uncomment the next line
	//m_wndInput.HideSelection(TRUE,FALSE);
	m_wndInput.SetSel(iLength,iLength);
	OnKickIdle(NULL,NULL);
}

void CMIMDlg::AssemblePkg(CPkg& pkg, CString strText)
{
	//Set Pkg to your current text and font settings to send or display it in room window
	//pkg.Init();
	//the next 5 are for client side

	strcpy_s(pkg.strText, strText);
	//m_wndInput.GetWindowText(pkg.strText );	//1
	//strcpy(pkg.strName, m_strScreenName);	//2
	//pkg.bAway = bAway;					//3
	//pkg.iMyIcon = m_iMyIcon;				//4
	pkg.bBold = m_bCharBold;				//5
	pkg.bItalic  = m_bCharItalic;			//6
	pkg.bUnderLine  = m_bCharUnderline;		//7
	pkg.bStrikeOut = m_bCharStrikeOut;		//8
	pkg.clrText  = m_clrChar;				//9
	strcpy_s(pkg.fontName, m_strFontName);	//10
	pkg.fontSize  = m_iFontSize;			//11

//	pkg.ipAddress							//12	reserved for Server
//	pkg.request = UNDEFINED;				//13	reserved 
//	pkg.pSocket								//14	reserved for Server
//	pkg.port								//15	reserved for Server

}

void CMIMDlg::SetDefaultCharFormat4Input()
{
	m_cfInput.cbSize		= sizeof (CHARFORMAT);  
	m_cfInput.dwMask		= CFM_FACE | CFM_SIZE | CFM_BOLD |
 		CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT | CFM_PROTECTED | CFM_COLOR;
	m_cfInput.dwEffects	= 0;//(unsigned long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD | CFE_ITALIC | CFE_STRIKEOUT);
	m_cfInput.crTextColor	= RGB(0, 0, 0); 

	if(m_iFontSize == 0)
		m_cfInput.yHeight = 200;
	else
		m_cfInput.yHeight = m_iFontSize*20;

	if(m_strFontName.IsEmpty())
		::lstrcpy(m_cfInput.szFaceName, "Arial"); 
	else
		::lstrcpy(m_cfInput.szFaceName, (LPTSTR)(LPCTSTR)m_strFontName); 

	m_cfInput.dwEffects = m_bCharBold ? (m_cfInput.dwEffects | CFE_BOLD) //1
		:(m_cfInput.dwEffects& ~CFE_BOLD);

	m_cfInput.dwEffects = m_bCharItalic ? (m_cfInput.dwEffects | CFE_ITALIC)//2 
		:(m_cfInput.dwEffects& ~CFE_ITALIC);

	m_cfInput.dwEffects = m_bCharUnderline ? (m_cfInput.dwEffects | CFE_UNDERLINE) //3
		:(m_cfInput.dwEffects& ~CFE_UNDERLINE);

	m_cfInput.dwEffects = m_bCharStrikeOut ? (m_cfInput.dwEffects | CFE_STRIKEOUT) //4
		:(m_cfInput.dwEffects& ~CFE_STRIKEOUT);

	m_cfInput.dwMask |= CFM_COLOR;//5
	m_cfInput.crTextColor = m_clrChar;

	m_wndInput.SetWordCharFormat(m_cfInput);
}

void CMIMDlg::UpdateCharFormat4In(/*CPkg &pkg*/)
{
	m_cfInput.yHeight = m_iFontSize*20;
	::lstrcpy(m_cfInput.szFaceName, (LPTSTR)(LPCTSTR)m_strFontName); 

	m_cfInput.crTextColor = m_clrChar;
	m_cfInput.dwMask |= CFM_COLOR;
	//Bold
	m_cfInput.dwMask |= CFM_BOLD;
	m_cfInput.dwEffects = m_bCharBold ? (m_cfInput.dwEffects | CFE_BOLD) 
		:(m_cfInput.dwEffects& ~CFE_BOLD);
	//italics
	m_cfInput.dwMask |= CFM_ITALIC;
	m_cfInput.dwEffects = m_bCharItalic ? (m_cfInput.dwEffects | CFE_ITALIC) 
		:(m_cfInput.dwEffects& ~CFE_ITALIC);
	//underline
	m_cfInput.dwMask |= CFM_UNDERLINE;
	m_cfInput.dwEffects = m_bCharUnderline ? (m_cfInput.dwEffects | CFE_UNDERLINE) 
		:(m_cfInput.dwEffects& ~CFE_UNDERLINE);
	//strikeout
	m_cfInput.dwMask |= CFM_STRIKEOUT;
	m_cfInput.dwEffects = m_bCharStrikeOut ? (m_cfInput.dwEffects | CFE_STRIKEOUT) 
		:(m_cfInput.dwEffects& ~CFE_STRIKEOUT);
	
	UpdateCharFormat4Input(0,FALSE);
	UpdateCharFormat4Input(1);

	m_wndInput.SetWordCharFormat(m_cfInput);

	/*CHARFORMAT cf;
	cf.cbSize		= sizeof (CHARFORMAT);  
	cf.dwMask		= CFM_FACE | CFM_SIZE | CFM_BOLD |
 		CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT | CFM_PROTECTED | CFM_COLOR;
	cf.dwEffects	= 0;//(unsigned long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD | CFE_ITALIC | CFE_STRIKEOUT);

	cf.crTextColor	= pkg.clrText; 
	
	cf.dwEffects = pkg.bBold ? (cf.dwEffects | CFE_BOLD) 
		:(m_cfInput.dwEffects& ~CFE_BOLD);

	cf.dwEffects = pkg.bItalic ? (cf.dwEffects | CFE_ITALIC) 
		:(m_cfInput.dwEffects& ~CFE_ITALIC);

	cf.dwEffects = pkg.bUnderLine ? (cf.dwEffects | CFE_UNDERLINE) 
		:(m_cfInput.dwEffects& ~CFE_UNDERLINE);
	
	cf.dwEffects = pkg.bStrikeOut? (cf.dwEffects | CFE_STRIKEOUT) 
		:(m_cfInput.dwEffects& ~CFE_STRIKEOUT);

	cf.yHeight = pkg.fontSize * 20;

	::lstrcpy(cf.szFaceName, (LPCTSTR)pkg.fontName); 

	m_wndInput.SetWordCharFormat(cf);*/
}

void CMIMDlg::UpdateCharFormat4Input(WPARAM param, BOOL bGetSel)
{
	//param is used to specify which object needs updating
	int nSelected;

	nSelected = (int)param;
	m_cfInput.dwMask |= CFM_FACE|CFM_SIZE;

	if(nSelected == 0)//font name combo selected
	{
		//font name

		if(bGetSel)//If you want to get the new font from the selection list
		{
			TCHAR szFontName[LF_FACESIZE];
			int nIndex = m_cmbFontName.GetCurSel();
			m_cmbFontName.GetLBText( nIndex, szFontName );
			// If font name is empty - return
			if( szFontName[0] != 0 ){
				_tcsncpy(m_cfInput.szFaceName, szFontName, LF_FACESIZE);	//strncpy
				m_strFontName = (CString)szFontName;
			}
		}
		else//otherwise just use whatever is currently in m_strFontName
		{
			//Set the selection of the combobox
			int iPos = m_cmbFontName.FindStringExact(0,m_strFontName);
			m_cmbFontName.SetCurSel(iPos);
		}
	}
	else // == 1
	{// font size combo selected 
		//font size

		TCHAR szSize[5];
		if(bGetSel)//If you want to get the new font from the selection list
		{
			int index = m_cmbFontSize.GetCurSel();
			if( index != CB_ERR )
				m_cmbFontSize.GetLBText(index, szSize );
			else
				m_cmbFontSize.GetWindowText( szSize, 5 );
			// Get size in Twips, 1440 twips == 1 inch;
			int nSize = _ttoi( szSize ) * 20;			// atoi for tchar
			
			if( nSize != 0)
			{
				m_cfInput.dwMask = CFM_SIZE;
				m_cfInput.yHeight = nSize;
				m_iFontSize = _ttoi( szSize );
			}
		}
	}
	CString strText;
	//change the font of the current text (if any)
	m_wndInput.GetWindowText(strText);
	int iLength = m_wndInput.GetWindowTextLength();
	m_wndInput.SetSel(0,iLength);
 	m_wndInput.SetWordCharFormat(m_cfInput);
	//m_wndInput.HideSelection(TRUE,FALSE);
}

//borrowed from MFC source - yh
class CToolCmdUI : public CCmdUI        // class private to this file !
{
public: // re-implementations only
	virtual void Enable(BOOL bOn);
	virtual void SetCheck(int nCheck);
	virtual void SetText(LPCTSTR lpszText);
};

//OnKickIdle is responsible for the Update CommandUI
//SDI/MDI applications have their own WM routing for this
//but Dialog doesn't. That's why we had to intercept and handle it - yh
void CMIMDlg::OnKickIdle(WPARAM, LPARAM)
{
	//This is for the tool bars, sets them checked or disables when necessary
	// for the standared tool bar	
	CToolCmdUI cmdUI;
	
	/*cmdUI.m_pOther  = &m_wndToolBar;
	cmdUI.m_nIndexMax = m_wndToolBar.GetToolBarCtrl().GetButtonCount();
	for(cmdUI.m_nIndex = 0;cmdUI.m_nIndex < cmdUI.m_nIndexMax;cmdUI.m_nIndex++)
	{
		cmdUI.m_nID = m_wndToolBar.GetItemID(cmdUI.m_nIndex);
		if(!(m_wndToolBar.GetButtonStyle(cmdUI.m_nIndex) & TBBS_SEPARATOR)){
			cmdUI.DoUpdate(this,TRUE);
		}
	}*/

	//for the Rich Text bar
	cmdUI.m_pOther  = &m_wndRichTextBar;
	cmdUI.m_nIndexMax = m_wndRichTextBar.GetToolBarCtrl().GetButtonCount();
	for(cmdUI.m_nIndex = 0;cmdUI.m_nIndex < cmdUI.m_nIndexMax;cmdUI.m_nIndex++)
	{
		cmdUI.m_nID = m_wndRichTextBar.GetItemID(cmdUI.m_nIndex);
		if(!(m_wndRichTextBar.GetButtonStyle(cmdUI.m_nIndex) & TBBS_SEPARATOR)){
			cmdUI.DoUpdate(this,TRUE);
		}
	}
	return;
}

void CMIMDlg::GetCharFormat(CHARFORMAT &cf)
{
	cf = m_cfInput;
}

void CMIMDlg::LoadSettings()
{
//persisting the setting - yh
	CFile iniFile;
	int iSize;
	
	if(iniFile.Open(m_strInstallDir+"\\MIM.dat",CFile::modeNoTruncate|CFile::modeRead))
	{
		if(iniFile.GetLength() == 0)
		{
			iniFile.Close();
			if(Agree())
			{
				SetStatusText("Press Connection Settings to begin setup...",0);
				m_bAgree = TRUE;
				SetDefaults();
			}
			return;
		}
		double dVer;
		CArchive* ar = new CArchive(&iniFile,CArchive::load);

		*ar >> dVer;
		if((dVer < m_dLowFileVer) || (dVer > m_dVer))//wrong version
		{
			delete ar;
			iniFile.Close();
			m_strTemp.Format("Incompatible version of MIM.dat\nFile: %.2f\nMin: %.2f\nMax: %.2f",dVer, m_dLowFileVer, m_dVer);
			MessageBox(m_strTemp);
			if(Agree())
			{
				m_bAgree = TRUE;
				SetDefaults();
			}
			return;
		}
		m_bAgree = TRUE;
		*ar >> m_strServerIP;

		*ar >> m_bCharBold ;
		*ar >> m_bCharItalic ;
		*ar >> m_bCharUnderline ;
		*ar >> m_bCharStrikeOut ;
		*ar >> m_clrChar;
		*ar >> m_clrBack;
		*ar >> m_strFontName ;
		*ar >> m_iFontSize ;
		*ar >> m_bSimple;

		//CON SETTINGS
		*ar >> m_iPort;
		*ar >> m_Users.m_iMax;
		*ar >> m_iType;
		*ar >> m_bSound;
		*ar >> m_bAutoLogin;
		*ar >> m_iIdleTime;
		*ar >> m_bBouncer;//m_iReserved2
		*ar >> m_iBounceSec;//m_iReserved3
		*ar >> m_bAsk;
		*ar >> m_bAlert;
		*ar >> m_bAllowRun;
		*ar >> m_iCensor;
		*ar >> m_strScreenName;
		*ar >> m_strBrowser;
		*ar >> m_strIP;
		*ar >> m_strComment;
		*ar >> m_strSoundDir;
		*ar >> m_strSoundExt;
		*ar >> m_strAniExt;
		*ar >> m_bAutoSaveMess;
		*ar >> m_bListenAble;
		*ar >> m_iSaveMessType;
		*ar >> m_strReserved1;
		*ar >> m_strReserved2;
		*ar >> m_strReserved3;

		*ar >> iSize;//Size of string array
		m_strSaved.RemoveAll();
		m_strComments.RemoveAll();
		for(int i = 0;i < iSize;i++)
		{
			*ar >> m_strTemp;
			m_strSaved.Add(m_strTemp);
			*ar >> m_strTemp;
			m_strComments.Add(m_strTemp);
		}

		//SERVER OPTIONS
		*ar >> m_iListenPort;
		*ar >> m_strAdminName;
		*ar >> m_strRoomName;
		*ar >> m_strPassword;
		*ar >> m_bTimeStamp;
		*ar >> m_strModPass;
		
		*ar >> iSize;//Size of string array
		m_strBanned.RemoveAll();
		for(int i = 0;i < iSize;i++)
		{
			*ar >> m_strTemp;
			m_strBanned.Add(m_strTemp);
		}
	
		//Profile
		*ar >> m_strName;
		*ar >> m_strPets;
		*ar >> m_strQuote;
		*ar >> m_strLive;
		*ar >> m_strEmail;
		*ar >> m_strImageDir;
		*ar >> m_MyProfile.m_bAni;
		*ar >> m_MyProfile.m_iGender;
		*ar >> m_MyProfile.m_iAge;
		//for combo
		int nIndex;
		*ar >> nIndex;
		m_cmbFontSize.SetCurSel(nIndex);
 
		*ar >> nIndex;
		m_cmbFontName.SetCurSel(nIndex);

		delete ar;		
		strcpy(m_Users.m_strAdmin, m_strAdminName);
		iniFile.Close();
		UpdateCharFormat4Input(0,FALSE);
		UpdateCharFormat4Input(1);
		SetStatusText("Loaded Settings...",0);
	}
	else
	{
		if(Agree())
		{
			SetStatusText("Press Connection Settings to begin setup...",0);
			m_bAgree = TRUE;
		}
		else
			return;
	}

	//Init Controls
	SyncChanges();
	LoadServerText();
}

void CMIMDlg::LoadServerText()
{
	CFile serverFile;

	//Server text
	if(serverFile.Open(m_strInstallDir+"\\Server.txt",CFile::modeNoTruncate|CFile::modeRead/*|CFile::typeText*/))
	{
		if(serverFile.GetLength() == 0)
		{
			serverFile.Close();
			return;
		}

		CArchive* ar = new CArchive(&serverFile,CArchive::load);

		char strServer[800];
		int iLen = ar->Read(strServer,800);
		strServer[iLen] = 0;//null-terminate the text
		m_strOpeningText = strServer;
		//MessageBox(m_strOpeningText);
		delete ar;		
		serverFile.Close();
		m_wndShow.ShowMessage("Server.txt loaded.",GREEN);		
	}
	else
		m_wndShow.ShowMessage("No Server.txt file found.",YELLOW);		
}

BOOL CMIMDlg::Agree()
{
	m_AgreeDlg.m_dVer = m_dVer;
	m_AgreeDlg.m_strRel = m_strRel;
	if(m_AgreeDlg.DoModal() == IDOK)
	{
		SetDefaults();
		SaveSettings();
		m_bAgree = TRUE;
		return TRUE;
	}
	else
		exit(0);
		//OnCancel();//doesnt work properly, use exit(0)
	return FALSE;
}

void CMIMDlg::OnSaveSettings()
{
	SaveSettings(TRUE);//need this because afx message wont pass parameters
}

void CMIMDlg::SaveSettings(BOOL bMessage)
{
	//bMessage is true if the console will display that the settings were saved.
	CFile iniFile;
	if(iniFile.Open(m_strInstallDir+"\\MIM.dat",CFile::modeNoTruncate|CFile::modeCreate|CFile::modeReadWrite))
	{
		CArchive* ar = new CArchive(&iniFile,CArchive::store);

		*ar << m_dVer;
		*ar << m_strServerIP;

		*ar << m_bCharBold;
		*ar << m_bCharItalic;
		*ar << m_bCharUnderline;
		*ar << m_bCharStrikeOut;
		*ar << m_clrChar;
		*ar << m_clrBack;
		*ar << m_strFontName;
		*ar << m_iFontSize;
		*ar << m_bSimple;

		//CON SETTINGS
		*ar << m_iPort;
		*ar << m_Users.m_iMax;
		*ar << m_iType;
		*ar << m_bSound;
		*ar << m_bAutoLogin;
		*ar << m_iIdleTime;
		*ar << m_bBouncer;//m_iReserved2
		*ar << m_iBounceSec;//m_iReserved3
		*ar << m_bAsk;
		*ar << m_bAlert;
		*ar << m_bAllowRun;
		*ar << m_iCensor;
		*ar << m_strScreenName;
		*ar << m_strBrowser;
		*ar << m_strIP;
		*ar << m_strComment;
		*ar << m_strSoundDir;
		*ar << m_strSoundExt;
		*ar << m_strAniExt;
		*ar << m_bAutoSaveMess;
		*ar << m_bListenAble;
		*ar << m_iSaveMessType;
		*ar << m_strReserved1;
		*ar << m_strReserved2;
		*ar << m_strReserved3;

		*ar << m_strSaved.GetSize();	
		for(int i = 0;i < m_strSaved.GetSize();i++)
		{
			*ar << m_strSaved.GetAt(i);
			*ar << m_strComments.GetAt(i);
		}

		//SERVER OPTIONS
		*ar << m_iListenPort;
		*ar << m_strAdminName;
		*ar << m_strRoomName;
		*ar << m_strPassword;
		*ar << m_bTimeStamp;
		*ar << m_strModPass;
		*ar << m_strBanned.GetSize();	
		for(int i = 0;i < m_strBanned.GetSize();i++)
			*ar << m_strBanned.GetAt(i);

		//Profile
		*ar << m_strName;
		*ar << m_strPets;
		*ar << m_strQuote;
		*ar << m_strLive;
		*ar << m_strEmail;
		*ar << m_strImageDir;
		*ar << m_MyProfile.m_bAni;
		*ar << m_MyProfile.m_iGender;
		*ar << m_MyProfile.m_iAge;

		//for combo
		int nIndex = m_cmbFontSize.GetCurSel();
		*ar << nIndex;
		nIndex = m_cmbFontName.GetCurSel();
		*ar << nIndex;

		ar->Flush();

		delete ar;
		iniFile.Close();
		if(bMessage)
			m_wndShow.ShowMessage(">> Saved settings.",GREEN);
	}
	else
		m_wndShow.ShowMessage("<!> Error. Could not open file MIM.dat for saving settings!",RED);
}

void CMIMDlg::OnOK() 
{
	//OnOK() override, close application

	if(m_Console.m_bConnected)
	{
		if(MessageBox("You still have a connection open, are you sure you want to quit\nand close the current connection?", "Connection Open", MB_YESNO | MB_ICONEXCLAMATION) != IDYES)
			return;
	}
	KillTimer(ID_TYPE_TIMER);
	OnCloseCon(-1);
	m_Console.DeleteAllComm();//clean up dynamic console commands
	CDialog::OnOK();
}

void CMIMDlg::OnCancel() 
{
	OnMinicon();
}

void CMIMDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	if(m_bAgree)//agreed to EULA
	{
		SaveSettings(FALSE);
		if(m_bPSL_Exe)
			OnSaveMsl();
		if(m_bAutoSaveMess)
		{
			m_strTemp.Format("%s\\History%i",m_strInstallDir,0);
			Savemess(m_strTemp, TRUE);
			//m_Console.DisplayMessage(strSavedTo,GREEN);
		}
	}
}

int CALLBACK CMIMDlg::EnumFontFamProc(ENUMLOGFONT *lpelf, NEWTEXTMETRIC *lpntm, int nFontType, LPARAM lParam)
{
	CMIMDlg* pWnd = (CMIMDlg*)lParam;

	// Add the font name to the combo
	CString strFont = (CString)lpelf->elfLogFont.lfFaceName;

	pWnd->m_cmbFontName.AddString(strFont);

	return 1;		// 1 to continue enumeration
}

void CMIMDlg::OnSelectFontName()
{
	UpdateCharFormat4Input((LPARAM)0);
}

void CMIMDlg::OnSelectFontSize()
{
	UpdateCharFormat4Input((LPARAM)1);
}

//Menu commands
void CMIMDlg::OnFileLoadsettings()
{
	if(MessageBox("Replace current settings with previously saved settings?", "Load", MB_YESNO | MB_ICONQUESTION)==IDYES)
		LoadSettings();
}

void CMIMDlg::OnFileRestoresettings() 
{
	if(!m_ctlConnect.IsWindowEnabled())
		return;

	if(MessageBox("Are you sure you want to reset all settings to\ntheir defaults?", "Restore Defaults", MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
		SetDefaults();
}

void CMIMDlg::OnFileSavemessages()
{
	OnSavemess();
}

void CMIMDlg::OnSavemess()
{
	//Save all messages to a "txt" file
	CString strSave, strFilter, strExt;

	strFilter = "HTML, TXT (*.htm;*.txt)|*.htm;*.txt||";

	CFileDialog dSave(FALSE, NULL, NULL, OFN_HIDEREADONLY, strFilter);

	if(dSave.DoModal() == IDOK)
	{
		strSave = dSave.GetPathName();
		strExt = dSave.GetFileExt();

		if(strExt != "")//remove it from strSave
		{
			int i = strSave.GetLength()-1;
			while(strSave[i] != '.')
				i--;
			strSave=strSave.Left(i);
			//MessageBox(strSave);
		}
		Savemess(strSave, FALSE, strExt);//saves as txt, or calls separate func. for html
		if(MessageBox("Do you wish to clear the saved messages from the current view?", "Clear Chat Room List", MB_YESNO | MB_ICONQUESTION) == IDYES)	
			Clear();
	}
	else//cancelled
		return;
}

CString CMIMDlg::Savemess(CString strSave, BOOL bAuto, CString strExt)
{
	//bAuto is TRUE when this func was called by the autosave feature.
	//Input file path and name, with no extension! Enter extension seperately, or not at all
	//to use the default extension type.
	
	if(strExt == "")//set the default extension
	{
		if(m_iSaveMessType == 0)
			strExt = "htm";
		else
			strExt = "txt";
	}

	strSave = strSave+"."+strExt;
	if(strExt == "htm")//save as htm file
	{
		if(SaveMessHTML(strSave, bAuto) == -1)
		{
			m_strTemp.Format("<!> Could not save messages as \"%s\".", strSave);
			m_Console.DisplayMessage(m_strTemp,RED);
		}
	}
	else //save as text file
	{
		CFile iniFile;

		if(iniFile.Open(strSave,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite/*|CFile::typeText*/))
		{
			CString strBuf;
			int iSize = int(iniFile.SeekToEnd());//FIX: this could cause problems if the saved text file is larger than 4GB
			m_wndShow.GetWindowText(strBuf);
			long total = strBuf.GetLength();
			if(bAuto)//only save the text that hasnt already been saved
			{
				//MessageBox("bAuto = true");
				if(total == m_lLastSaveHisPos)//no changes since last save, abort.
					return "No changes.";
				long s,e;

				if(iSize > m_iMaxAutoSize)//if larger than 10MB
				{
					CString strDir;
					iniFile.Close();//close this file
					//remove the filename to get the dir only
					int i = strSave.GetLength()-1;
					while(strSave[i] != '\\')
						i--;
					strDir = strSave.Left(i+1);
					
					CString strNewFile;
					strNewFile = strDir + "History" + GetDate() + GetTimeStamp('_') + ".txt";
					m_strTemp.Format("History file size larger than %i bytes. Renaming \"%s\"\n as \"%s\".", m_iMaxAutoSize, strSave, strNewFile);
					m_Console.DisplayMessage(m_strTemp,YELLOW);

					iniFile.Rename(strSave, strNewFile);//rename the current file
					//now open a blank new file with the original name

					if(!iniFile.Open(strSave,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite/*|CFile::typeText*/))
					{
						iniFile.Close();
						m_strTemp.Format("<!> Could not autosave messages as \"%s\".", strSave);
						m_Console.DisplayMessage(m_strTemp,RED);
						return "";
					}
					iSize = int(iniFile.SeekToEnd());
				}
				m_wndShow.GetSel(s,e);//save original sel
				m_wndShow.SetSel(m_lLastSaveHisPos, -1);//select all text from lLastSaveHisPos to end
				strBuf = m_wndShow.GetSelText();
				m_wndShow.SetSel(s, e);//restore original sel
			}
			iniFile.Write(LPCTSTR(strBuf),strBuf.GetLength());
			iniFile.Close();
			m_lLastSaveHisPos = total;//update last saved pos to end of window
		}
		else
		{
			m_strTemp.Format("<!> Could not save messages as \"%s\".", strSave);
			m_Console.DisplayMessage(m_strTemp,RED);
			iniFile.Close();
			return "";
		}
	}
	if(!bAuto)
	{
		m_strTemp.Format("Messages saved to \"%s\".", strSave);
		m_Console.DisplayMessage(m_strTemp,GREEN);
	}
	return m_strTemp;
}

int CMIMDlg::SaveMessHTML(CString strSave, BOOL bAuto) 
{
	CHARFORMAT cf;
	BOOL bBold = FALSE, bItalic = FALSE, bUnder = FALSE, bStrike = FALSE;
	CString strFont, strHead, strText;
	COLORREF cColor;

	LONG iStart,iEnd;
	WORD lcolor, hcolor;
	int iSize;
	BYTE rr, gg, bb;

	CFile iniFile;
	CString strHead1;

	if(iniFile.Open(strSave,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite/*|CFile::typeText*/))
	{
		iSize = int(iniFile.SeekToEnd());//FIX: this could cause problems if the saved text file is larger than 4GB
		if(iSize > m_iMaxAutoSize)//if larger than 10MB
		{
			CString strDir;
			iniFile.Close();//close this file
			//remove the filename to get the dir only
			int i = strSave.GetLength()-1;
			while(strSave[i] != '\\')
				i--;
			strDir = strSave.Left(i+1);

			CString strNewFile;
			strNewFile = strDir + "History" + GetDate() + GetTimeStamp('_') + ".htm";
			m_strTemp.Format("History file size larger than %i bytes. Renaming \"%s\"\n as \"%s\".", m_iMaxAutoSize, strSave, strNewFile);
			m_Console.DisplayMessage(m_strTemp,YELLOW);

			iniFile.Rename(strSave, strNewFile);//rename the current file
			//now open a blank new file with the original name
			if(!iniFile.Open(strSave,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite/*|CFile::typeText*/))
			{
				iniFile.Close();
				return -1;
			}
			iSize = int(iniFile.SeekToEnd());//FIX: this could cause problems if the saved text file is larger than 4GB
		}
	}
	else
	{
		MessageBox("Could not save messages.", "Saving Error", MB_OK | MB_ICONSTOP);
		iniFile.Close();
		return -1;
	}

	if(iSize == 0)//if brand new file
	{
		strHead ="<html>\r\n";
		strHead +="<head>\r\n";
		strHead +="<meta http-equiv=\"Content-Type\" content=\"text/html; charset=windows-1252\">\r\n";
		strHead +="<title>Mercury Instant Messenger</title>\r\n";
		strHead +="</head>\r\n";
		strHead +="<body bgcolor=\"#000000\">";
	}
	//m_strTemp.Format("Autosaving...History file size %i bytes.", iSize);
	//m_Console.DisplayMessage(m_strTemp,YELLOW);
//<meta name="GENERATOR" content="Microsoft FrontPage 4.0">
//<meta name="ProgId" content="FrontPage.Editor.Document">

	m_wndShow.GetWindowText(strText);
	long top = strText.GetLength();
	long s,e;
	int i = 0;
	iEnd = 0;
	m_wndShow.GetSel(s,e);//save original sel

	if(bAuto)//only save the text that hasnt already been saved
	{
		if(top == m_lLastSaveHisPos)//no changes since last save, abort.
			return 0;//exit, no changes
		i = m_lLastSaveHisPos;
		iEnd = m_lLastSaveHisPos;//set start position
	}

	/*
	iStart = strText.Find("http://",i);
		if(iStart == -1)//try again
		{
			iStart = strText.Find("www.",i);
			if(iStart == -1)//try again
			{
				iStart = strText.Find("mailto:",i);
				if(iStart == -1)//try again
				{
					iStart = strText.Find("https://",i);
					if(iStart == -1)//try again
						iStart = strText.Find("ftp://",i);
	*/
	//int top = m_wndShow.GetLineCount();

	while(i < top)
	{
		if(strText.GetAt(i) == '\r')
		{
			if(strText.GetAt(i+1) == '\n')//found return
			{
				//Array of start and end positions of all hyperlinks
				//m_wndShow.wHyperlinkS;
				//m_wndShow.wHyperlinkE;

				iStart = iEnd;
				iEnd = i+1;
				i++;//additional i++ below.

				//iStart = m_wndShow.LineIndex(i);
				//iEnd = m_wndShow.LineIndex(i+1)-1;
				m_wndShow.SetSel(iStart, iEnd);
				m_wndShow.GetSelectionCharFormat(cf);//supposed to return the characteristics of first char, but doesnt seem to do that.

				//convert color
				lcolor = LOWORD(cf.crTextColor);
				hcolor = HIWORD(cf.crTextColor);
				rr = LOBYTE(lcolor);
				gg = HIBYTE(lcolor);
				bb = LOBYTE(hcolor);

				//get font
			
				cf.dwMask = CFM_COLOR | CFM_ITALIC | CFM_BOLD | CFM_UNDERLINE | CFM_STRIKEOUT;
				strHead1 = "";
				if((cf.dwEffects & CFM_BOLD)&&(!bBold))
				{
					strHead1 += "<b>";//add bold
					bBold = TRUE;
				}
				else if (bBold)
				{
					strHead += "</b>";//remove bold
					bBold = FALSE;
				}
				if((cf.dwEffects & CFM_UNDERLINE)&&(!bUnder))
				{
					strHead1 += "<u>";//add underline
					bUnder = TRUE;
				}
				else if (bUnder)
				{
					strHead += "</u>";//remove Underline
					bUnder = FALSE;
				}

				if((cf.dwEffects & CFM_STRIKEOUT)&&(!bStrike))
				{
					strHead1 += "<s>";
					bStrike = TRUE;
				}
				else if (bStrike)
				{
					strHead += "</s>";//remove strikeout
					bStrike = FALSE;
				}
				if((cf.dwEffects & CFM_ITALIC)&&(!bItalic))
				{
					strHead1 += "<i>";
					bItalic = TRUE;
				}
				else if (bItalic)
				{
					strHead += "</i>";//remove italics
					bItalic = FALSE;
				}
				m_strTemp = cf.szFaceName;

				//if something changed, then change it and save those settings for next iteration.
				if((strFont != m_strTemp) || (iSize !=int(cf.yHeight/20/4)) || (cf.crTextColor != cColor))
				{
					if(strFont != "")
						strHead += "</font>\r\n";//end last font

					//save current attributes
					iSize = int(cf.yHeight/20/4);
					strFont = m_strTemp;//save it
					cColor = cf.crTextColor;

					m_strTemp.Format("<font color=\"#%.2X%.2X%.2X\" size=\"%i\" face=\"%s\">", rr,gg,bb, iSize, strFont);
					strHead += m_strTemp;
				}
				strHead += strHead1;
				iniFile.Write(LPCTSTR(strHead),strHead.GetLength());
				m_strTemp = m_wndShow.GetSelText();

				m_strTemp.Replace("<","&lt;");
				m_strTemp.Replace(">","&gt;");

				//FIX: make hyperlinks clickable: <a href=stfu.com><font color=blue>CLICK HERE!</font></a>
				//wHyperlinkS
				//if(m_strTemp.Right(1) == "\n")
				m_strTemp += "<br>";
				m_strTemp += "\r\n";

				iniFile.Write(LPCTSTR(m_strTemp),m_strTemp.GetLength());
				//DEBUG:
				//MessageBox(strHead+m_strTemp);
				strHead="";
			}
		}
		i++;
	}
	iniFile.Close();
	m_lLastSaveHisPos = top;//update last saved pos to end of window
	m_wndShow.SetSel(s,e);//restore original sel
	return 0;
}

int CMIMDlg::AddLine(CString strFile, CString strLine)
{
	//adds a CString of text to a file
	CFile iniFile;

	if(iniFile.Open(m_strInstallDir+"\\"+strFile,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite/*|CFile::typeText*/))
	{
		iniFile.SeekToEnd();
		iniFile.Write(LPCTSTR(strLine),strLine.GetLength());
		iniFile.Close();
	}
	else
	{
		m_strTemp.Format("<!> Could not add line to %s.",m_strInstallDir+"\\"+strFile);
		MessageBox(m_strTemp, "Saving Error", MB_OK | MB_ICONSTOP);
		iniFile.Close();
		return -1;
	}
	return 0;
}

void CMIMDlg::OnHelpAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.m_pWnd = this;
	dlgAbout.m_dVer = m_dVer;
	dlgAbout.m_strRel = m_strRel;
	dlgAbout.DoModal();
}

void CMIMDlg::OnHelpHelp()
{
	CReferenceDlg RefDlg;
	RefDlg.DoModal();	
}

void CMIMDlg::NotePad(CString strFile)
{
	m_strTemp.Format("Notepad.exe \"%s\"",strFile);
	if(WinExec(m_strTemp, SW_SHOW) < 32)
	{
		m_strTemp.Format("<!> Could not execute program: Notepad.exe \"%s\"",strFile);
		m_wndShow.ShowMessage(m_strTemp,RED);
	}
}

void CMIMDlg::OnHelpPatch()
{
	/*m_strTemp = "file:///" + m_strInstallDir+"\\Patch.htm";
	m_strTemp.Replace('\\','/');
	m_strTemp.Replace(" ","%20");
	
	if(Browser(m_strTemp) == -1)//try the html file first*/
	if(32 > int(ShellExecute(0, _T("open"), m_strInstallDir + "\\Patch.htm", 0, 0, SW_SHOWNORMAL)))
		NotePad(m_strInstallDir+"\\Patch.txt");
}

void CMIMDlg::OnHelpReadme()
{
	//file:///D:/My%20Documents/My%20Projects/MIM/readme.htm
	/*m_strTemp = "file:///" + m_strInstallDir+"\\readme.htm";
	m_strTemp.Replace('\\','/');
	m_strTemp.Replace(" ","%20");
	
	if(Browser(m_strTemp) == -1)//try the html file first
		NotePad(m_strInstallDir+"\\Readme.txt");*/
	if(int(ShellExecute(0, _T("open"), m_strInstallDir + "\\readme.htm", 0, 0, SW_SHOWNORMAL)) > 32)
		NotePad(m_strInstallDir+"\\Readme.txt");
}

void CAboutDlg::OnHelpEula() 
{
	//AboutDlg
	//m_pWnd pointer may be invalid if this window was created using the task bar method;
	//therefore use WinExec not any MIMDlg functions.
	if((CMIMDlg*)m_pWnd != NULL)
		((CMIMDlg*)m_pWnd)->NotePad(((CMIMDlg*)m_pWnd)->m_strInstallDir+"\\EULA.txt");
	else
		MessageBox("To view the EULA click the Help/EULA menu option.");
}

void CMIMDlg::OnHelpEula() 
{
	//MIMDlg
	NotePad(m_strInstallDir+"\\EULA.txt");
}

void CMIMDlg::OnFileViewhistory() 
{
	//MIMDlg
	if(m_iSaveMessType == 1)
		NotePad(m_strInstallDir+"\\History0.txt");
	else
	{
		m_strTemp = "file:///" + m_strInstallDir+"\\History0.htm";
		m_strTemp.Replace('\\','/');
		m_strTemp.Replace(" ","%20");
		if(Browser(m_strTemp) == -1)//try the html file first
			MessageBox("History0.htm not found");
	}
}

void CMIMDlg::OnFileViewquotes() 
{
	NotePad(m_strInstallDir+"\\Quote.txt");
}

void CMIMDlg::OnFileOpenBouncer() 
{
	NotePad(m_strInstallDir+"\\Bouncer.txt");
}

void CMIMDlg::OnFileViewcommands() 
{
	NotePad(m_strInstallDir+"\\Commands.txt");
}

void CMIMDlg::OnFileLoadservertxt() 
{
	LoadServerText();
}

void CMIMDlg::OnFileViewServertxt() 
{
	NotePad(m_strInstallDir+"\\Server.txt");
}

void CMIMDlg::Exit()
{
	OnFileExit();
}

void CMIMDlg::OnFileExit() 
{
	OnOK();
}

void CMIMDlg::OnSettings() 
{
	int i;
	m_SettingsDlg.m_clrFore = m_clrFore;
	m_SettingsDlg.m_clrBG = m_clrBG;
	m_SettingsDlg.m_brush = &m_BrushBG;

	m_SettingsDlg.m_iPort = m_iPort;
	m_SettingsDlg.m_iType = m_iType;
	m_SettingsDlg.m_iIdleTime = m_iIdleTime;
	m_SettingsDlg.m_strScreenName = m_strScreenName;
	m_SettingsDlg.m_strBrowser = m_strBrowser;
	m_SettingsDlg.m_strSoundDir = m_strSoundDir;
	m_SettingsDlg.m_strAniDir = m_strImageDir;
	m_SettingsDlg.m_strIP = m_strIP;
	m_SettingsDlg.m_strComment = m_strComment;
	m_SettingsDlg.m_strSoundExt = m_strSoundExt;
	m_SettingsDlg.m_bSound = m_bSound;
	m_SettingsDlg.m_bAni = m_MyProfile.m_bAni;
	//m_SettingsDlg.m_iReserved1 = m_iReserved1;
	m_SettingsDlg.m_bAllowRun = m_bAllowRun;
	m_SettingsDlg.m_bAutoSaveMess = m_bAutoSaveMess;
	m_SettingsDlg.m_bListenAble = m_bListenAble;
	m_SettingsDlg.m_bAutoLogin = m_bAutoLogin;
	m_SettingsDlg.m_iSaveMessType = m_iSaveMessType;
	//clear IP address strings, then copy to them
	m_SettingsDlg.m_strSaved.RemoveAll();
	m_SettingsDlg.m_strComments.RemoveAll();
	for(i = 0;i < m_strSaved.GetSize();i++)
	{
		m_SettingsDlg.m_strSaved.Add(m_strSaved.GetAt(i));
		m_SettingsDlg.m_strComments.Add(m_strComments.GetAt(i));
	}

	if(m_SettingsDlg.DoModal() == IDOK)
	{
		//send changed values back
		m_iPort = m_SettingsDlg.m_iPort;
		m_iType = m_SettingsDlg.m_iType;
		m_iIdleTime = m_SettingsDlg.m_iIdleTime;
		m_bSound = m_SettingsDlg.m_bSound;
		m_MyProfile.m_bAni = m_SettingsDlg.m_bAni;
		//m_iReserved1 = m_SettingsDlg.m_iReserved1;
		m_bAllowRun = m_SettingsDlg.m_bAllowRun;
		m_bAutoSaveMess = m_SettingsDlg.m_bAutoSaveMess;
		m_bListenAble = m_SettingsDlg.m_bListenAble;
		m_bAutoLogin = m_SettingsDlg.m_bAutoLogin;
		m_iSaveMessType = m_SettingsDlg.m_iSaveMessType;

		m_strScreenName = m_SettingsDlg.m_strScreenName;
		m_strBrowser = m_SettingsDlg.m_strBrowser;
		m_strIP = m_SettingsDlg.m_strIP;
		m_strComment = m_SettingsDlg.m_strComment;
		m_strSoundDir = m_SettingsDlg.m_strSoundDir;
		m_strImageDir = m_SettingsDlg.m_strAniDir;

		m_strSoundExt = m_SettingsDlg.m_strSoundExt;

		//clear strings for copying, then copy
		m_strSaved.RemoveAll();
		m_strComments.RemoveAll();
		for(i = 0;i < m_SettingsDlg.m_strSaved.GetSize();i++)
		{
			m_strComments.Add(m_SettingsDlg.m_strComments.GetAt(i));
			m_strSaved.Add(m_SettingsDlg.m_strSaved.GetAt(i));
		}
	
		if(m_iType == CLIENT)
			m_strTemp.Format("Port: %i", m_iPort);
		else
			m_strTemp.Format("Port: %i", m_iListenPort);
		SetStatusText(m_strTemp, 2);
	}
	SyncChanges();
	if(m_SettingsDlg.m_bGoToServer)
		OnServerOptions();
}

void CMIMDlg::SyncChanges()
{
	CBitmap Bit;

	if(m_iType == SERVER)
	{
		GetDlgItem(IDC_CONNECT)->SetWindowText("&Listen");
		//m_strStatus = "Select options and press \"Listen\" when ready...";
		Bit.LoadBitmap(IDB_LISTEN);
		m_ctlConnect.SetBitmap(Bit);
		Bit.Detach();
	} 
	else
	{
		GetDlgItem(IDC_CONNECT)->SetWindowText("&Connect");
		//m_strStatus = "Select options and press \"Connect\" when ready...";
		Bit.LoadBitmap(IDB_CONNECT);
		m_ctlConnect.SetBitmap(Bit);
		Bit.Detach();
	}
	UpdateCharFormat4In();

	m_wndShow.SetBackgroundColor(FALSE, m_clrBack);
	m_wndInput.SetBackgroundColor(FALSE, m_clrBack);
	m_ctlUserList.SetBkColor(m_clrBack);
	m_ctlUserList.SetTextBkColor(m_clrBack);
	m_MasterListDlg.m_clrChar = m_clrChar;	//console text color
	m_MasterListDlg.m_clrBack = m_clrBack;	//console BG text color
}

void CMIMDlg::OnServerOptions() 
{
	//Show the ServerDlg window, pass all vars into it and
	//get them back afterwards if OK was pressed.
	m_ServerDlg.m_brush = &m_BrushBG;
	m_ServerDlg.m_clrFore = m_clrFore;
	m_ServerDlg.m_clrBG = m_clrBG;

	m_ServerDlg.m_iPort = m_iListenPort;
	m_ServerDlg.m_iMax = m_Users.m_iMax;
	m_ServerDlg.m_bAsk = m_bAsk;
	m_ServerDlg.m_bTimeStamp = m_bTimeStamp;
	m_ServerDlg.m_iCensor = m_iCensor;
	m_ServerDlg.m_bPublic = m_bPublic;

	m_ServerDlg.m_strAdminName = m_strAdminName;
	m_ServerDlg.m_strRoomName = m_strRoomName;
	m_ServerDlg.m_strPassword = m_strPassword;
	m_ServerDlg.m_strModPass = m_strModPass;
	m_ServerDlg.m_bBouncer = m_bBouncer;
	m_ServerDlg.m_iBounceSec = m_iBounceSec;

	//m_ServerDlg.m_strCompName.Format("Name: %s", m_pListenSocket->GetYourIP());
	//m_ServerDlg.m_strYourIP.Format("IP address: %s", m_pListenSocket->GetYourIP());

	//clear banned IP address strings, then copy to them
	m_ServerDlg.m_strSaved.RemoveAll();
	for(int i = 0;i < m_strBanned.GetSize();i++)
		m_ServerDlg.m_strSaved.Add(m_strBanned.GetAt(i));

	if(m_ServerDlg.DoModal() == IDOK)
	{
		m_iListenPort = m_ServerDlg.m_iPort;
		m_Users.m_iMax = m_ServerDlg.m_iMax;
		m_bAsk = m_ServerDlg.m_bAsk;
		m_bTimeStamp = m_ServerDlg.m_bTimeStamp;
		m_iCensor = m_ServerDlg.m_iCensor;
		m_bPublic = m_ServerDlg.m_bPublic;

		m_Users.m_iMax = m_ServerDlg.m_iMax;
		strcpy(m_Users.m_strAdmin, m_ServerDlg.m_strAdminName);
		m_strAdminName = m_ServerDlg.m_strAdminName;
		m_strRoomName = m_ServerDlg.m_strRoomName;
		m_strPassword = m_ServerDlg.m_strPassword;
		m_strModPass = m_ServerDlg.m_strModPass;
		m_bBouncer = m_ServerDlg.m_bBouncer;
		m_iBounceSec = m_ServerDlg.m_iBounceSec;

		//clear strings for copying, then copy
		m_strBanned.RemoveAll();
		for(int i = 0;i < m_ServerDlg.m_strSaved.GetSize();i++)
			m_strBanned.Add(m_ServerDlg.m_strSaved.GetAt(i));
	}
}

void CMIMDlg::OnBConnect() 
{
	//if(m_iType == SERVER)
		Connect();
	//else//Threaded
	//	AfxBeginThread(OnConnectT,this);/*,THREAD_PRIORITY_ABOVE_NORMAL*/
}

UINT CMIMDlg::OnConnectT(LPVOID pParam)//threaded method for connecting to server
{
	//FIX: Make connect thread parallel
	//everything in this function is probably outdated compared to the modern OnCOnnect()

	CMIMDlg *pPointer;
	pPointer = ((CMIMDlg*)pParam);

	//Listen/Connect button

	if(!pPointer->m_ctlConnect.IsWindowEnabled())
		return 0;
	pPointer->ShowSet(FALSE);

	pPointer->UpdateData(TRUE); 
	//Server/client pressed Listen/Connect button.
	if(pPointer->m_iType == CLIENT)
		pPointer->m_strTemp.Format("Port: %i", pPointer->m_iPort);
	else
		pPointer->m_strTemp.Format("Port: %i", pPointer->m_iListenPort);
	pPointer->SetStatusText(pPointer->m_strTemp, 2);

	if(pPointer->m_iType == SERVER)//Open a listen socket
	{
		pPointer->Finalize();
		/*if(pPointer->m_pListenSocket != NULL)
		{
			pPointer->m_wndShow.ShowMessage("<!> Listen Socket already open",RED);
			return 0;
		}*/

		for(int i = 0;i < ID_MAX;i++)
		{
			strcpy(pPointer->m_Users.m_strNames[i], "");
			pPointer->m_Users.m_iConnected[i] = 0;
			pPointer->m_Users.m_iTotalUsers = 0;
			pPointer->m_Users.m_iImage[i] = 0;
		}

		pPointer->m_wndShow.ShowMessage("========================================");
		pPointer->m_strTemp.Format("...Setting up server \"%s\"...", pPointer->m_strRoomName);
		pPointer->m_wndShow.ShowMessage(pPointer->m_strTemp);
		pPointer->SetStatusText("Setting up server...", 0);
		//pPointer->m_pListenSocket = new CMySocket(pPointer,0);

		if(pPointer->m_pListenSocket.Create(pPointer->m_iListenPort))
		{
			if(pPointer->m_pListenSocket.Listen())//successful
			{
				//pPointer->GetDlgItem(IDC_DISCONNECT)->EnableWindow(TRUE);
				pPointer->SetStatusText("Server", 1,IDI_GREEN);
				pPointer->UpdateList();
				pPointer->m_strTemp.Format("Server created on port %i, Waiting for others to join...",pPointer->m_iListenPort);
				pPointer->SetStatusText(pPointer->m_strTemp,0);
				//m_strTemp.Format("Welcome to: %s", m_strRoomName);

				pPointer->ShowSend(TRUE);

				pPointer->m_wndShow.ShowMessage(">> Server ready "+pPointer->GetTimeStamp(),GREEN);
				pPointer->m_Console.m_bConnected = TRUE;
				pPointer->SetWindowText(pPointer->m_strRoomName);
				pPointer->m_strCurRoom.Format("MIM v%.2f", pPointer->m_dVer);
			}
			else//port is busy
			{
				pPointer->m_strTemp.Format("Could not listen for others on socket %i.",pPointer->m_iListenPort);
				pPointer->MessageBox(pPointer->m_strTemp, "Establishing Listen Mode Error", MB_OK | MB_ICONSTOP);
				pPointer->SetStatusText(pPointer->m_strTemp,0);
				pPointer->OnBDisconnect();
			}
		}
		else
		{
			pPointer->m_strTemp.Format("Could not create a socket on port %i. Make sure that TCP/IP is running \nor try a different Port number.",pPointer->m_iListenPort);
			pPointer->MessageBox(pPointer->m_strTemp, "Establishing Connection Error", MB_OK | MB_ICONSTOP);
			pPointer->SetStatusText(pPointer->m_strTemp,0);
			pPointer->OnBDisconnect();
			pPointer->m_wndShow.ShowMessage("<!> Could not open socket for listening",RED);
		}
	}
	else//Client, connect to listen socket
	{
		if(pPointer->m_pClientSocket[0] != NULL)
		{
			pPointer->m_wndShow.ShowMessage("<!> Connection Socket already exists. Recreating...",YELLOW);
			pPointer->Finalize();
			if(pPointer->m_pClientSocket[0] != NULL)
			{
				pPointer->m_wndShow.ShowMessage("<!> Can't recreate socket.",RED);
				return 0;
			}
		}
		pPointer->SetStatusText("Calling...", 1);

		pPointer->m_wndShow.ShowMessage("========================================");
		pPointer->m_wndShow.ShowMessage("...Opening connection...");
		pPointer->SetStatusText("Attempting to connect to server...", 0);

		pPointer->m_pClientSocket[0] = new CMySocket(pPointer, 0);

		//pPointer->m_pListenSocket = new CMySocket(pPointer,0);//Just for use of functions inside

		if(pPointer->m_pClientSocket[0]->Create())
		{
			pPointer->m_strTemp.Format("...Calling server %s:%i...", pPointer->m_strIP,pPointer->m_iPort);
			pPointer->m_wndShow.ShowMessage(pPointer->m_strTemp);
			pPointer->m_bIsCalling = TRUE;
			if(pPointer->m_pClientSocket[0]->Connect(pPointer->m_strIP,pPointer->m_iPort) == 0)
			{
				//pPointer->MessageBox("Cancelled");
				pPointer->m_bIsCalling = FALSE;
				if(pPointer->m_bCancelled)
				{
					pPointer->m_wndShow.ShowMessage("<!> Connection aborted",YELLOW);
					pPointer->m_bCancelled = FALSE;
				}
				else
				{
					pPointer->m_wndShow.ShowMessage("<!> Connection failed",RED);
					pPointer->SetStatusText("Connection timed out.",0);
					pPointer->MessageBox("The connection was unsuccessful.\nThe server you are trying to connect may be listening on a different Port/IP Address,\nor the server is not running.\nSee the \"Readme.txt\" file for more details on connection failures.\nError #10060", "Connection timed out", MB_OK | MB_ICONEXCLAMATION);
				}
				pPointer->OnCloseCon(0);//safe to delete now, calling shoudl have been aborted
				return 0;
			}
			pPointer->m_bIsCalling = FALSE;

			if(pPointer->m_pSocketFile[0] != NULL)
			{
				pPointer->m_pSocketFile[0]->Close();
				delete pPointer->m_pSocketFile[0];
				pPointer->m_wndShow.ShowMessage("<!> Socket File already exists. Recreating...",YELLOW);
			}
			pPointer->m_pSocketFile[0] = new CSocketFile(pPointer->m_pClientSocket[0]);

			pPointer->m_pClientSocket[0]->CreateArchives(pPointer->m_pSocketFile[0]);			
	
			pPointer->m_strTemp.Format("Attempting to connect to server \"%s\" on port %i, please wait...",pPointer->m_strIP, pPointer->m_iPort);
			pPointer->SetStatusText(pPointer->m_strTemp,0);
		}
		else
		{
			pPointer->m_strTemp.Format("Could not create socket to enable connection.\nTCP/IP may be missing.");
			pPointer->SetStatusText(pPointer->m_strTemp,0);
			pPointer->m_wndShow.ShowMessage("<!>"+pPointer->m_strTemp,RED);
			pPointer->OnBDisconnect();
			return 0;
		}
		//Start timer
		pPointer->m_wndShow.ShowMessage("...Connected, waiting for reply...");
		/*m_iTimeout = m_iTimeout;
		m_strTemp.Format("Time remaining: %i", m_iTimeout);
		SetStatusText((m_strTemp,0);
		m_iTimeoutRem = m_iTimeout;
		SetTimer(ID_TIMER, 1000, NULL);*/
	}
	//pPointer->UpdateData(FALSE);//Need for strCurRoom
	return 0;
}
/*#define ID_USER_SENDFILE                32793
#define ID_USER_IGNORE                  32794
#define ID_USER_VIEWPROFILE             32795
#define ID_USER_KICK                    32796
#define ID_USER_MYSTATUS_DISCONNECT     32797
#define ID_USER_MESSAGE                 32798
#define ID_USER_STATUS_AVAILABLE        32799
#define ID_USER_STATUS_AWAY             32800
#define ID_ICON_STATUS_AVAILABLE        32801
#define ID_ICON_STATUS_AWAY             32802
#define ID_ICON_STATUS_CONNECT          32803
#define ID_ICON_STATUS_DISCONNECT       32804
#define ID_ICON_RESTORE                 32805
#define ID_ICON_EXIT                    32806
*/
void CMIMDlg::Connect()
{
	//Connect button
	/*//if(m_iType == CLIENT)//Threaded
		AfxBeginThread(OnConnectT,this);//THREAD_PRIORITY_ABOVE_NORMAL
		return;*/

	//Listen/Connect button pressed
	if(!m_ctlConnect.IsWindowEnabled())
		return;
	UpdateData(TRUE); 
	//Server/client pressed Listen/Connect button.
	ShowSet(FALSE);

	if(m_iType == CLIENT)
		m_strTemp.Format("Port: %i", m_iPort);
	else
		m_strTemp.Format("Port: %i", m_iListenPort);
	SetStatusText(m_strTemp, 2);
	
	if(m_iType == SERVER)//Open a listen socket
	{
		Finalize();
		/*if(m_pListenSocket != NULL)
		{
			m_wndShow.ShowMessage("<!> Listen Socket already open",RED);
			return;
		}*/

		for(int i = 0;i < ID_MAX;i++)
		{
			strcpy(m_Users.m_strNames[i], "");
			m_Users.m_iConnected[i] = 0;
			m_Users.m_iTotalUsers = 0;
			m_Users.m_iImage[i] = 0;
		}

		m_wndShow.ShowMessage("========================================");
		m_strTemp.Format("...Setting up server \"%s\"...", m_strRoomName);
		m_wndShow.ShowMessage(m_strTemp);
		SetStatusText("Setting up server...", 0);
		//m_pListenSocket = new CMySocket(this,0);

		if(m_pListenSocket.Create(m_iListenPort))
		{
			if(m_pListenSocket.Listen())//successful
			{
				SetStatusText("Server", 1,IDI_GREEN);
				UpdateList();
				m_strTemp.Format("Server created on port %i, Waiting for others to join...",m_iListenPort);
				SetStatusText(m_strTemp,0);
				//m_strTemp.Format("Welcome to: %s", m_strRoomName);

				ShowSend(TRUE);

				m_wndShow.ShowMessage(">> Server ready "+GetTimeStamp(),GREEN);
				m_Console.m_bConnected = TRUE;
				SetWindowText(m_strRoomName);
				m_strCurRoom.Format("MIM v%.2f", m_dVer);
			}
			else//port is busy
			{
				m_strTemp.Format("Could not listen for others on socket %i.",m_iListenPort);
				MessageBox(m_strTemp, "Establishing Listen Mode Error", MB_OK | MB_ICONSTOP);
				SetStatusText(m_strTemp,0);
				OnBDisconnect();
			}
		}
		else
		{
			m_strTemp.Format("Could not create a socket on port %i. Make sure that TCP/IP is running \nor try a different Port number.",m_iListenPort);
			MessageBox(m_strTemp, "Establishing Connection Error", MB_OK | MB_ICONSTOP);
			SetStatusText(m_strTemp,0);
			OnBDisconnect();
			m_wndShow.ShowMessage("<!> Could not open socket for listening",RED);
		}
	}
	else//Client, connect to listen socket
	{
		if(m_pClientSocket[0] != NULL)
		{
			m_wndShow.ShowMessage("<!> Connection Socket already exists. Recreating...",YELLOW);
			Finalize();
			if(m_pClientSocket[0] != NULL)
			{
				m_wndShow.ShowMessage("<!> Can't recreate socket.",RED);
				return;
			}
		}
		SetStatusText("Calling...", 1);
		if(!m_bServerInfoRequest && !m_bPublicInfoRequest && !m_bMasterInfoRequest)//only display if not INFO-Requesting
		{
			m_wndShow.ShowMessage("========================================");
			m_wndShow.ShowMessage("...Opening connection...");
		}
		SetStatusText("Attempting to connect to server...", 0);
		m_pClientSocket[0] = new CMySocket(this, 0);

		//m_pListenSocket = new CMySocket(this,0);//Just for use of functions inside

		if(m_pClientSocket[0]->Create())
		{
			if(!m_bServerInfoRequest && !m_bPublicInfoRequest && !m_bMasterInfoRequest)//only display if not INFO-Requesting
			{
				m_strTemp.Format("...Calling server %s:%i...", m_strIP,m_iPort);
				m_wndShow.ShowMessage(m_strTemp);
			}
			if(m_pClientSocket[0]->Connect(m_strIP,m_iPort) == 0)
			{
				OnCloseCon(0);
				m_wndShow.ShowMessage("<!> Connection failed",RED);
				SetStatusText("Connection timed out.",0);
				MessageBox("The connection was unsuccessful.\nThe server you are trying to connect may be listening on a different Port/IP Address,\nor the server is not running.\nSee the \"Readme.txt\" file for more details on connection failures.\nError #10060", "Connection timed out", MB_OK | MB_ICONEXCLAMATION);
				return;
			}
			if(m_pSocketFile[0] != NULL)
			{
				m_wndShow.ShowMessage("> SocketFile already exists. Deleting before recreating...",YELLOW);
				m_pSocketFile[0]->Close();
				delete m_pSocketFile[0];
				m_pSocketFile[0] = NULL;
			}
			m_pSocketFile[0] = new CSocketFile(m_pClientSocket[0]); 

			m_pClientSocket[0]->CreateArchives(m_pSocketFile[0]);			

			m_strTemp.Format("> Attempting to connect to server \"%s:%i, please wait...",m_strIP, m_iPort);
			SetStatusText(m_strTemp,0);
		}
		else
		{
			m_strTemp.Format("<!> Could not create socket to enable connection.\nTCP/IP may be missing.");
			SetStatusText(m_strTemp,0);
			m_wndShow.ShowMessage(m_strTemp,RED);
			OnBDisconnect();
			return;
		}
		//Start timer
		m_wndShow.ShowMessage("...Waiting for reply...");
		/*m_iTimeout = m_iTimeout;
		m_strTemp.Format("Time remaining: %i", m_iTimeout);
		SetStatusText((m_strTemp,0);
		m_iTimeoutRem = m_iTimeout;
		SetTimer(ID_TIMER, 1000, NULL);*/
	}
	UpdateData(FALSE);//Need for strCurRoom
}

int CMIMDlg::InfoRequestConnect()
{
	//TODO: this function has alot of redundancy when called from within a loop (i.e. refreshing the server list)
	//TODO: add the ability for the Master Server to send the MSL to the requester.

	//Connects to a server using a unique socket for the purpose of obtaining info from it only, not to remain connected.
	//Use for either info-request or MSL-request.
	//make sure the proper member flags are set (m_bServerInfoRequest or m_bPublicInfoRequest or m_bMasterInfoRequest)
	//and the current info request index in the server list
	//set m_bServerInfoRequest to FALSE to cancel the whole loop process
	//this function will delete the socket pointer if it hasnt been already, before using it
	if(m_pInfoReqSocket != NULL)
	{
		//NEW: instead of deleting the socket, just close it? sounds sketchy. would need to clean up when closing MIM
		m_pInfoReqSocket->Close();
		delete m_pInfoReqSocket;
		m_pInfoReqSocket = NULL;
		//m_wndShow.ShowMessage("<!> Info Request Socket already exists. Recreating...",YELLOW);
	}

	m_pInfoReqSocket = new CMySocket(this, 0);//FIX: this "0" might cause problems...

	if(m_bServerInfoRequest && (m_bPublicInfoRequest || m_bMasterInfoRequest))//check that only one of these is set
	{
		m_wndShow.ShowMessage("Both info requests are being attemtped at the same time. Defaulting to \"refresh\" mode only.",YELLOW);
		m_bPublicInfoRequest = FALSE;
	}

	if(!m_bServerInfoRequest && !m_bPublicInfoRequest && !m_bMasterInfoRequest)//user may have cancelled before the end
		return -2;

	if(m_bServerInfoRequest && m_iInfoRequestIndex < 0)//out of bounds check
		return -3;

	CString strIP;
	int iPort = 0;
	if(m_bServerInfoRequest)//request server info from a single server in the list. (REFRESH LIST button was pressed)
	{
		if(m_iInfoRequestIndex >= m_MasterListDlg.m_oaServers.GetSize())//out of bounds check
		{
			m_wndShow.ShowMessage("Index larger than MasterList length.",RED);
			return -4;
		}
		strIP = m_MasterListDlg.GetEntry(m_iInfoRequestIndex)->m_strIP;
		iPort = m_MasterListDlg.GetEntry(m_iInfoRequestIndex)->m_iPort;
	}
	else if(m_bPublicInfoRequest)//request the server list from the master server (UPDATE LIST button was pressed)
	{
		//Cycle through the MSL to retrive the latest PSL!
		strIP = m_MasterListDlg.m_strMSL_IPs.GetAt(m_iInfoRequestIndex);
		iPort = m_MasterListDlg.m_iMSL_Ports.GetAt(m_iInfoRequestIndex);
	}
	else if(m_bMasterInfoRequest)//Retrive the latest MSL from the one and only MasterHD
	{
		//The one and only MSL Provider
		strIP = "masterhd.mooo.com";//FIX: make this a standard changeable variable
		iPort = 4001;//FIX: make this a standard changeable variable
	}

	if(m_pInfoReqSocket->Create())
	{
		if(m_bServerInfoRequest)
		{
			m_iInfoRequestTimer = 5;//number of seconds to wait before giving up and moving to the next one, decremented in timer
			//DEBUG: error#10061
			m_strTemp.Format("> Requesting server info at %s:%i...", strIP, iPort);
		}
		else if(m_bPublicInfoRequest)
			m_strTemp.Format("> Requesting Public Server List (PSL) from Master Server %s:%i...", strIP, iPort);
		else if (m_bMasterInfoRequest)
			m_strTemp.Format("> Requesting Master Server List (MSL) from MasterHD at %s:%i...", strIP, iPort);

		m_wndShow.ShowMessage(m_strTemp);
		if(m_pInfoReqSocket->Connect(strIP,iPort) == 0)
		{
			WSAEAFNOSUPPORT;
			int error = m_pInfoReqSocket->GetLastError();
			//Connection Failed, just close it
			m_pInfoReqSocket->CancelBlockingCall();
			m_pInfoReqSocket->Close();
			delete m_pInfoReqSocket;
			m_pInfoReqSocket = NULL;

			if(m_bServerInfoRequest)
			{
				m_strTemp.Format("<!> Server \"%s:%i\" not found. Error #%i", strIP, iPort, error);
				m_wndShow.ShowMessage(m_strTemp,YELLOW);
				//Must alert the Request function to resume with next server
				m_MasterListDlg.GetEntry(m_iInfoRequestIndex)->m_iStatus = 2;//dead connection
				m_MasterListDlg.UpdateList(m_iInfoRequestIndex);//Only update the indexed entry in the list view.
				m_iInfoRequestTimer = 0;//stop the timer, otherwise when it reaches 0 it'll try to start at the next index
			}
			else if(m_bPublicInfoRequest)
				m_wndShow.ShowMessage("<!> Unable to locate Master Server!",RED);
			else if(m_bMasterInfoRequest)
				m_wndShow.ShowMessage("<!> Unable to locate MasterHD!",RED);
			return -1;//this will cause the loop function to continue with the next index
		}
	}
	else
		m_wndShow.ShowMessage("<!> TCP-IP Missing",RED);

	if(m_pInfoReqSocketFile != NULL)
	{
		m_wndShow.ShowMessage("<!> Socket File already exists. Recreating...",YELLOW);
		delete m_pInfoReqSocketFile;
		m_pInfoReqSocketFile = NULL;
	}
	m_pInfoReqSocketFile = new CSocketFile(m_pInfoReqSocket);
	m_pInfoReqSocket->CreateArchives(m_pInfoReqSocketFile);

	if(m_bServerInfoRequest)
		m_strTemp = "...Waiting for reply to info-request...";
	else if(m_bPublicInfoRequest)
		m_strTemp = "...Waiting for reply to Public Server List (PSL) request...";
	else if(m_bMasterInfoRequest)
		m_strTemp = "...Waiting for reply to Master Server List (MSL) request...";

	m_wndShow.ShowMessage(m_strTemp);
	return 0;//successful
}

void CMIMDlg::OnBClear() 
{
	if(MessageBox("Are you sure that you want to clear the message list?\nAny unsaved messages wil be lost.", "Clear Chat Room List", MB_YESNO | MB_ICONQUESTION) == IDYES)	
		Clear();
}

void CMIMDlg::Clear() 
{
	m_wndShow.ClearAllText();
	m_lLastSaveHisPos = 0;
	UpdateData(FALSE);
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//Declare version on AboutDlg window.
	UpdateData(TRUE);
	CDialog::OnInitDialog();
	SetMIMColor(&m_ctlEULA);
	//m_ctlEULA.SetColor(GREEN, RGB(0,0,0));
	m_strVer.Format("Mercury IM Version: %.2f for %s", m_dVer, m_strRel);
	m_strDate.Format("Release Date: %s, %s",__DATE__,__TIME__);
	m_strCopyright.Format("Copyright (C) 2002-%i",CYEAR);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMIMDlg::OnViewprofile() 
{
	//pressed view profile button
	int sel = m_ctlUserList.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);

	if(sel >= 0)//something is selected
	{
		int ID = GetID(m_ctlUserList.GetItemText(sel,0));
		Viewprofile(ID);
	}
	else
		MessageBox("Select a user first!");
}

void CMIMDlg::Viewprofile(int ID) 
{
	UpdateData(TRUE);

	//pressed view profile button
	
	if(m_bProfileOpen)
		return;//cant have 2 open at once, corrects for latency issues
	if(m_iProfile > 0)
		return;//flood control
	m_bProfileOpen = TRUE;

	//ask server for profile info, using this ID #
	if((ID < -1)||(ID > 19))
	{
		m_bProfileOpen = FALSE;
		m_wndShow.ShowMessage("<!> Invalid number or range.",RED);
		return;
	}
	else if((ID == -1) && (m_iType == SERVER))//Admin clicked self, open "edit"
	{
		Profile(TRUE);	//open for editing
		return;
	}
	else if(m_iType == CLIENT)//User clicked self, open "edit"
	{
		if(ID == m_pClientSocket[0]->m_iClientID)
		{
			Profile(TRUE);	//open for editing
			return;
		}
		m_SendProfile.m_iReq = m_pClientSocket[0]->m_iClientID;	//requester's ID
	}

	//m_SendProfile.m_iReq = m_pClientSocket[0]->m_iClientID;	//requester's ID
	m_SendProfile.m_iRep = ID;								//replier's ID
	if(m_iType == SERVER)
	{
		m_SendProfile.m_iReq = -1;	//ADMIN requester's ID is -1
		SendText(ID, PROF, "");		//send to client
	}
	else
		SendText(0, PROF, "");//send to server
	SetFlood(m_iProfile, 3);
	//server will recieve and forward to other client,
	//who will then reply to server, who will then reply to you.
}

void CMIMDlg::OnBEditprofile() 
{
	//pressed button
	if(m_bProfileOpen)
	{
		m_wndShow.ShowMessage("<!> Profile is already open. If the problem persists, reconnect to the server or restart MIM.",RED);
		return;
	}
	m_bProfileOpen = TRUE;
	Profile(TRUE);	
}

int CMIMDlg::Profile(BOOL view)
{
	if(!m_bProfileOpen)
	{
		m_wndShow.ShowMessage("<!> No profile open for veiwing/editing",RED);
		return -1;
	}

	//Opens the Profile window:
	//True allows editing, FALSE is read-only for viewing

	m_ProfileDlg.m_bView = view;
	m_ProfileDlg.m_brush = &m_BrushBG;
	m_ProfileDlg.m_clrFore = m_clrFore;
	m_ProfileDlg.m_clrBG = m_clrBG;

	if(view)//open for editing
	{
		m_ProfileDlg.m_strName = m_strName;
		m_ProfileDlg.m_strPets = m_strPets;
		m_ProfileDlg.m_strQuote = m_strQuote;
		m_ProfileDlg.m_strLive = m_strLive;
		m_ProfileDlg.m_strEmail = m_strEmail;
		m_ProfileDlg.m_bAni = m_MyProfile.m_bAni;
		m_ProfileDlg.m_strImageDir = m_strImageDir;
		//Mac's are like automatic transmissions, they're easy to use, but you can't do much with them.
		m_ProfileDlg.m_iGender = m_MyProfile.m_iGender;
		m_ProfileDlg.m_iAge = m_MyProfile.m_iAge;

		if(m_ProfileDlg.DoModal() == IDOK)
		{
			//save changes to RAM
			m_MyProfile.m_iGender = m_ProfileDlg.m_iGender;
			m_MyProfile.m_iAge = m_ProfileDlg.m_iAge;

			m_strName = m_ProfileDlg.m_strName;
			m_strPets = m_ProfileDlg.m_strPets;
			m_strQuote = m_ProfileDlg.m_strQuote;	
			m_strLive = m_ProfileDlg.m_strLive;	
			m_strEmail = m_ProfileDlg.m_strEmail;
		}
	}
	else//just open it, all vars should already be set
		m_ProfileDlg.DoModal();

	m_bProfileOpen = FALSE;
	return 0;
}

void CMIMDlg::SetDefaults()
{
	//Set defaults for saved file
	m_bAway = FALSE;
	if(m_Console.m_bConnected)
		return;
	m_bCancelled = FALSE;
	m_bIsCalling = FALSE;
	m_iAutoSaveHis = 180;//number of seconds between each message autosave
	m_bSimple = FALSE;
	m_bCharBold = FALSE;
	m_bCharItalic = FALSE;
	m_bCharUnderline = FALSE;
	m_bCharStrikeOut = FALSE;
	m_strFontName = "Verdana";
	m_clrChar = RGB(164,176,238);
	m_strBrowser = "";
	m_iTotal = 0;
	m_bServerInfoRequest = FALSE;
	m_bPublicInfoRequest = FALSE;
	m_bMasterInfoRequest = FALSE;

	//CON SETTINGS
	m_iPort = 4001;
	m_Users.m_iMax = 10;
	m_iType = CLIENT;
	m_bSound = TRUE;
	m_MyProfile.m_bAni = FALSE;
	m_bAutoLogin = FALSE;
	m_iSaveMessType = 1;
	m_iIdleTime = 240;
	m_iReserved2 = 0;
	m_iReserved3 = 0;
	m_strReserved1 = "";
	m_strReserved2 = "";
	m_strReserved3 = "";
	m_bAsk = FALSE;
	m_bAlert = FALSE;
	m_bAllowRun = TRUE;
	m_bAutoSaveMess = TRUE;
	m_bListenAble = FALSE;
	m_bSimple = FALSE;
	m_iCensor = 0;
	m_strScreenName = "New User";
	m_strBrowser = "C:\\Program Files\\Mozilla Firefox\\firefox.exe";
	m_strTemp = m_strInstallDir + "\\Download";
	CreateDirectory(LPCTSTR(m_strTemp), NULL);//create a Download subfolder

	//SERVER OPTIONS
	m_iListenPort = 4001;
	m_strAdminName = "New Admin";
	m_strRoomName = "New Chat Room";
	m_strPassword = "";
	m_strIP = "MasterHD.mooo.com";
	m_strComment = "MasterHD's Server";
	m_strSoundDir = "Default.wav";
	m_strSoundExt = ".wav";
	m_strImageDir = "64.bmp";
	m_strAniExt = ".bmp";
	m_bTimeStamp = TRUE;
	m_strModPass = "";
	m_iBounceSec = 600;
	m_bBouncer = false;
	//strcpy(m_strChatDir, "C:\\");dont edit this! its changed during install
	m_strSaved.RemoveAll();
	m_strComments.RemoveAll();
	m_strSaved.Add("MasterHD.mooo.com");
	m_strComments.Add("MasterHD's Server");
	m_strSaved.Add("192.168.1.100");
	m_strComments.Add("Typical LAN IP");
	m_strSaved.Add("127.0.0.1");
	m_strComments.Add("localhost (loopback)");
	m_strBanned.RemoveAll();
	//Profile
	m_MyProfile.m_iAge = 16;
	m_MyProfile.m_iGender = 0;
	m_strName = "Noone";
	m_strLive = "Nowhere";
	m_strPets = "No Pets";
	m_strQuote = "No Quote";
	m_strEmail = "Noemail@no.com";
	m_cmbFontSize.SetCurSel(0);
	m_cmbFontName.SetCurSel(0);
	//m_clrChar = RGB(189,208,213);
	m_clrChar = RGB(164,176,238);
	m_cfInput.crTextColor = m_clrChar;
	m_cfInput.dwMask |= CFM_COLOR;

	m_cfInput.dwMask = CFM_SIZE;
	m_iFontSize = _ttoi( "10" );
	m_cfInput.yHeight = m_iFontSize*20;
	m_wndInput.SetWordCharFormat(m_cfInput);
	m_strFontName = "Verdana";
	m_clrBack = RGB(0,0,0);

	UpdateCharFormat4Input(0,FALSE);
	UpdateCharFormat4Input(1);
	SyncChanges();//listen connect buttons
	//refresh screen
	m_bSimple = !m_bSimple;
	OnSimple();

	//Other
	//m_strOpeningText = "";
}

void CMIMDlg::ShowSet(BOOL Show)
{
	//Show/hide settings controls
	GetDlgItem(IDC_CONNECT)->EnableWindow(Show);
	GetDlgItem(IDC_OPTIONS)->EnableWindow(Show);
	GetDlgItem(IDC_SETTINGS)->EnableWindow(Show);
	GetDlgItem(IDC_DISCONNECT)->EnableWindow(!Show);
	if(!m_bSimple)
	{
		GetDlgItem(IDC_CONNECT)->ShowWindow(Show);
		GetDlgItem(IDC_OPTIONS)->ShowWindow(Show);
		GetDlgItem(IDC_SETTINGS)->ShowWindow(Show);
		GetDlgItem(IDC_DISCONNECT)->ShowWindow(!Show);
	}

	m_bShowSet = Show;
	return;
}

void CMIMDlg::ShowSend(BOOL Show)
{
	//Show/hide sending controls
	GetDlgItem(IDC_USERLIST)->EnableWindow(Show);
	GetDlgItem(IDC_SENDFILE)->EnableWindow(Show);
	if(!m_bSimple)
		GetDlgItem(IDC_SENDFILE)->ShowWindow(Show);

	if(m_iType == SERVER)
	{
		GetDlgItem(IDC_KICK)->EnableWindow(Show);
		GetDlgItem(IDC_MBOX)->EnableWindow(Show);
		GetDlgItem(IDC_VIEWPROFILE)->EnableWindow(Show);
		if(!m_bSimple)
		{
			GetDlgItem(IDC_KICK)->ShowWindow(Show);
			GetDlgItem(IDC_MBOX)->ShowWindow(Show);
			GetDlgItem(IDC_VIEWPROFILE)->ShowWindow(Show);
		}
	}
	else
	{
		GetDlgItem(IDC_IGNORE)->EnableWindow(Show);
		GetDlgItem(IDC_VIEWPROFILE)->EnableWindow(Show);
		if(!m_bSimple)
		{
			GetDlgItem(IDC_IGNORE)->ShowWindow(Show);
			GetDlgItem(IDC_VIEWPROFILE)->ShowWindow(Show);
		}
	}
	m_bShowSend = Show;

	return;
}

void CMIMDlg::UpdateList()
{
	//Server and client
	//Erase anyone that has left, fill in new, display it.
	//Does not auto update each clients lists from server, use SendText(-1, ULIST,"") to do that
	
	UpdateData(TRUE);

	//Server must use 'sendtext(-1,ULIST,"");' function to send to clients
	CString strStore;
	int i, iLen;

	//FIX: !!!
	iLen = 0;
	LV_ITEM lvItem;

	m_ctlUserList.DeleteAllItems();
	SetTrayIcon(m_MiniIcon,2);//Keep current icon, just update user# text. This should also refresh the tray if it dissapears
	//ADMIN
	/*lvItem.mask = LVIF_TEXT|LVIF_IMAGE;
	lvItem.iImage = 1;
	lvItem.iItem = iLen;
	lvItem.iSubItem = 0;

	lvItem.pszText= "";
	m_ctlUserList.InsertItem(&lvItem);*/

	m_strTemp.Format("(A) %s",m_Users.m_strAdmin);
	m_ctlUserList.InsertString(0,m_strTemp,3);
	
	/*lvItem.mask = LVIF_TEXT;
	lvItem.iItem = iLen;
	lvItem.iSubItem = 1;
	char strAdmin[50];
	strcpy(strAdmin, m_Users.m_strAdmin);
	LPTSTR Text = strAdmin;
	lvItem.pszText = Text;
	//MessageBox(m_Users.m_strAdmin);
	//strcpy(lvItem.pszText,LPSTR());

	m_ctlUserList.SetItem(&lvItem);
*/
	//HERE!
	//KEY:
	//COLUMNS: 0 = icon#, 1=name, 2=rank
	//IMAGES: 0=client,1=mute,2=error,3=admin,4=away,5=you

	iLen = 1;
	for(i = 0;i < ID_MAX; i++)
	{
		//Add names to list, they are already saved into array.
		//Add the ID # in () at the beginning
		if(m_Users.m_iConnected[i] == 1)
		{
			//Censor
			strcpy(m_Users.m_strNames[i], Censor(m_Users.m_strNames[i]));//censor and copy

			//copy to lptstr
			lvItem.mask = LVIF_TEXT|LVIF_IMAGE;
			if(m_iUserOK[i] > 1)
				m_Users.m_iImage[i] = 2;//error user image
			else
				m_Users.m_iImage[i] = 0;//default user image
			lvItem.iImage = m_Users.m_iImage[i];

			if(m_iType == CLIENT)
				if((i == m_pClientSocket[0]->m_iClientID) && (lvItem.iImage != 4)&&(lvItem.iImage != 3))
				{
					lvItem.iImage = 5;//your icon, if not mod
				}

			if((m_Users.m_iImage[i] != 2) && (m_Users.m_iImage[i] != 3))//If not error image or admin image
			{
				if(m_bIgnore[i])//change icon to MUTE
					lvItem.iImage = 1;
				else//Ignored user is not there anymore, or never was
					m_bIgnore[i] = FALSE;//erase it from ignored list
				//DEBUG:
				//strStore.Format("m_bIgnore[%i] = %i", i, m_bIgnore[i]);
				//m_wndShow.ShowMessage(strStore);
			}

			/*lvItem.iItem = iLen;
			lvItem.iSubItem = 0;
			strStore.Format("%i",i+1);
			//NOTE: pszText must be declared after mask!
			LPTSTR Text = strStore.GetBuffer(strStore.GetLength());
			lvItem.pszText= Text;			
			m_ctlUserList.InsertItem(&lvItem);*/
			m_strTemp.Format("(%i) %s",i+1,m_Users.m_strNames[i]);
			m_ctlUserList.InsertString(iLen,m_strTemp,lvItem.iImage);

			/*lvItem.mask = LVIF_TEXT;
			lvItem.iItem = iLen;
			lvItem.iSubItem = 1;
			strcpy(Text, LPCTSTR(m_Users.m_strNames[i]));
			lvItem.pszText = Text;
			m_ctlUserList.SetItem(&lvItem);*/
			iLen++;

			//The total number of users will not be identical to the highest
			//ID number because a user with a lower ID number may have left
			//the Server. Here we see if they are connected and increment
			//the total accordingly.
		}
		//Erase left-over names
		else
		{
			strcpy(m_Users.m_strNames[i], "");
			m_bIgnore[i] = FALSE;
			//DEBUG:
			//strStore.Format("ErasedB (%i)", i+1);
			//m_wndShow.ShowMessage(strStore);
		}
	}
	
	if((m_Users.m_iTotalUsers > 0) && (m_iType == SERVER))
		m_Console.m_bConnected = TRUE;
	//current/total users
	m_strUsers.Format("Users: %i/%i",m_Users.m_iTotalUsers,m_Users.m_iMax);
	UpdateData(FALSE);
}

CString CMIMDlg::Censor(CString strCensor)
{
	//Censor bad words from the input string
	int here = 1;
	int i, j;
	CString strUp;
	CString strBad[5];

	//MessageBox(m_strCensor);

	strUp = strCensor;
	strUp.MakeUpper();

	if(m_iCensor > 0)
	{
		for(i = 0;i < 5;i++)
			strBad[i].Empty();
		strBad[0] = "FUCK";
		strBad[1] = "SHIT";

		for(i = 0;i < 2;i++)//2 words
		{
			here = strUp.Find(strBad[i]);
			while(here >= 0)
			{
				for(j = here;j < strBad[i].GetLength()+here;j++)
				{
					strCensor.SetAt(j, '*');
					strUp.SetAt(j, '*');
				}
				here = strUp.Find(strBad[i]);
			}
		}
	}
	if(m_iCensor > 1)
	{
		for(i = 0;i < 5;i++)
			strBad[i].Empty();
		strBad[0] = "DICK";
		strBad[1] = "BITCH";

		for(i = 0;i < 2;i++)//2 words
		{
			here = strUp.Find(strBad[i]);
			while(here >= 0)
			{
				for(j = here;j < strBad[i].GetLength()+here;j++)
				{
					strCensor.SetAt(j, '*');
					strUp.SetAt(j, '*');
				}
				here = strUp.Find(strBad[i]);
			}
		}
	}
	if(m_iCensor > 2)
	{
		for(i = 0;i < 5;i++)
			strBad[i].Empty();
		strBad[0] = "ASS";
		strBad[1] = "DAMN";
		strBad[2] = "HELL";
		strBad[3] = "FAG";

		for(i = 0;i < 4;i++)//4 words
		{
			here = strUp.Find(strBad[i]);
			while(here >= 0)
			{
				for(j = here;j < strBad[i].GetLength()+here;j++)
				{
					strCensor.SetAt(j, '*');
					strUp.SetAt(j, '*');
				}
				here = strUp.Find(strBad[i]);
			}
		}
	}
	//MessageBox(strCensor);
	return strCensor;
}

void CMIMDlg::CloseCon()
{
	//WARNING: ONLY USE IF NO CHANCE OF BEING CALLED WITHIN THE SOCKET CLASS
	//otherwise call OnCloseCon(0);
	//User pressed the "Disconnect" Button.

	//m_strTimeout = "Connection closed.";
	//UpdateData(FALSE);
	OnCloseCon(-1);//Close, pass -1 for current user.
}

void CMIMDlg::OnAccept()
{
	//Called by CSocket, server accepted client
	int iCon = NewSocket();//finds new empty socket
	if(iCon == -1)	//ROOM FULL
	{
		CMySocket *pFullSocket;
		CSocketFile *pSocketFile;
		pFullSocket = new CMySocket(this,0);
		m_pListenSocket.Accept(*pFullSocket);
		pSocketFile = new CSocketFile(pFullSocket);
		pFullSocket->CreateArchives(pSocketFile);
		//m_strTemp.Format("%s is already connected with another client, try again later.", m_strScreenName);
		m_strTemp.Format("<!> %s currently has %i user(s) connected and is full, try again later.", m_strRoomName,m_Users.m_iTotalUsers);

		//CANT USE SENDMESS, NOT USING pClientSocket to send.
		CPacket FPacket;

		CPkg Text;
		AssemblePkg(Text,m_strTemp);

		FPacket.m_iPos = -1;
		FPacket.m_iCode = MESG;
		FPacket.m_iLength = strlen(Text.strText)+56;
		CMySocket::StrCopy(FPacket.pBuf, (char*)&Text, FPacket.m_iLength);
		
		pFullSocket->SendPacket(FPacket);
		
		pFullSocket->Close();
		delete pFullSocket;
		delete pSocketFile;
		pFullSocket = NULL;
		pSocketFile = NULL;
		return;
	}

	if(m_pListenSocket.Accept(*m_pClientSocket[iCon]))
	{
		m_pSocketFile[iCon] = new CSocketFile(m_pClientSocket[iCon]);//each accept should use a seperate CSocketFile 
		//m_pClientSocket[iCon]->m_bSender = m_bSender;
		m_pClientSocket[iCon]->CreateArchives(m_pSocketFile[iCon]);
		//MessageBox("Requesting Client Info...");

		//CHECK FOR BAN
		CString strTemp = GetIP(iCon);//get peers IP

		for(int i = 0;i < m_strBanned.GetSize();i++)
		{
			if(m_strBanned[i] == strTemp)
			{
				m_strTemp.Format("<!> You have been banned from this server \"%s\".", m_strRoomName);
				SendMess(iCon, m_strTemp, MESG);

				m_pClientSocket[iCon]->Close();
				m_strTemp.Format("%s Banned user \"%s\" attempted to join the server.",GetTimeStamp(),strTemp);
				m_wndShow.ShowMessage(m_strTemp,YELLOW);
				return;
			}
		}
		m_Console.m_bConnected = TRUE;
		m_Users.m_iConnected[iCon] = 2;//half way connected

		//Send chatroom name along with info request
		CPacket QPacket;
		CMySocket::ClearPacket(QPacket);
		QPacket.m_iCode = REG;
		QPacket.m_iPos = iCon;
		QPacket.m_iLength = 0;
		SendText(QPacket);
		//Begin acceptance process
	}
	else
	{
		m_strTemp.Format("<!> User ID-%i could not join the server.",iCon+1);
		m_wndShow.ShowMessage(m_strTemp,RED);
	}
}
void CMIMDlg::OnConnect()
{
	//For CLIENTS only
	//connection request accepted by server, level 2 access is given
	//(Cannot recieve chat messages yet). Confirmation is
	//required to recieve level 1 access (chat capabilities).
	UpdateData(TRUE);
	m_Users.m_iConnected[0] = 2;	//halfway connected
	m_strTemp.Format("Successfully connected to server \"%s\".",m_strIP);
	SetStatusText(m_strTemp,0);
	m_wndShow.ShowMessage("...Connection accepted, verifying...",GREEN);
	SetWindowText(m_strRoomName);
	m_strCurRoom.Format("MIM v%.2f", m_dVer);
	/*//Gets IP of server
	unsigned int *iTemp;
	m_pClientSocket[0]->GetSockName(m_strTemp,*iTemp);
	MessageBox(m_strTemp);*/

	//Wait for request for ver, name, and password
	//OnReceive should be called next, with code REG (request info)
}

void CMIMDlg::OnBDisconnect() 
{
	CloseCon();
}

void CMIMDlg::OnCloseCon(int id)
{
	//Use this to close conenction, if it is called from within the socket,
	//it wont delete itself (no crash). Upon reconnect it will delete itself.
	//Send -1 if there is no chance it called itself, otherwise send 0 to close
	//itself and not delete.

	int i;
	CString strSend, strLeft;
	m_Console.m_bVoteActive = FALSE;

	KillTimer(ID_PING_TIMER);
	KillTimer(ID_FLOOD_TIMER);
	m_ctlUserList.DeleteAllItems();//Clear the list
	m_iFlood = 0;

	if(m_iType == CLIENT)
	{
		//reset all data
		m_strCurRoom = "";
		SetStatusText("Disconnecting from server...",0);
		m_Users.m_iConnected[0] = 0;//not connected
		
		if(m_bIsCalling)
		{
			MessageBox("m_bIsCalling");
			//MessageBox("OnCloseCon() Cancelling...");
			m_pClientSocket[0]->CancelBlockingCall();
			m_bIsCalling = FALSE;
			m_bCancelled = TRUE;
			return;
			//MessageBox("OnCloseCon() Cancelled...");
		}
		m_bIsCalling = FALSE;
		//Delete self (will result in crash if socket pointer called this function)
		if(id == -1)
			DeleteSocket(0);
		else//otherwise just close it (safe method)
		{
			m_pClientSocket[0]->Close();
			//WARNING: DO NOT delete the socket here, since it called this function.
			//DeleteSocket(0);
		}

		m_bProfileOpen = FALSE;
		if(m_Console.m_bConnected)
		{
			strSend.Format("- %s has left the server.", m_strScreenName);
			m_wndShow.ShowMessage(strSend,YELLOW);
		}
		m_Console.m_bConnected = FALSE;
		m_Console.m_bAccepted = FALSE;
		m_strUsers = "Offline";
		UpdateData(FALSE);
		SetStatusText("Disconnected from server.",0);
		ShowSend(FALSE);
		ShowSet(TRUE);

		for(i = 0;i < ID_MAX;i++)
			m_bIgnore[i] = FALSE;
		SetStatusText("Offline", 1,IDI_RED);
		m_wndShow.ShowMessage(">> Connection closed.",YELLOW);
	}
	else//Server
	{
		int i;
		if(id == -1)//Server pressed disconnect
		{
			//if(m_pListenSocket == NULL)
			//	return;
			m_bProfileOpen = FALSE;
			SetStatusText("Disconnecting from all clients...",0);

			m_strTemp = ">> Server Closed "+GetTimeStamp();
			CHARFORMAT cf = m_wndShow.ShowMessage(m_strTemp,YELLOW);
			CPkg Text(cf,m_strTemp);
			SendMess(-1, Text);

			m_pListenSocket.Close();
			for(i = 0;i < ID_MAX;i++)
			{
				DeleteSocket(i);
				m_Users.m_iConnected[i] = 0;
				m_Users.m_iTotalUsers = 0;
			}
			m_Console.m_bConnected = FALSE;
			m_Console.m_bAccepted = FALSE;
			ShowSet(TRUE);

			ShowSend(FALSE);
			m_strUsers = "Offline";
			m_strCurRoom = "";
			UpdateData(FALSE);
			SetStatusText("Offline", 1,IDI_RED);

		}
		//Server wants to close only 1 connection
		else
		{
			//if client was fully connected (level 1 access)
			if(m_Users.m_iConnected[id] == 1)
			{
				//Make sure not to send to the one that closed
				m_Users.m_iConnected[id] = 0;
				m_Users.m_iImage[id] = 0;
				m_Users.m_iTotalUsers--;
				m_bIgnore[id] = FALSE;
				strLeft = m_Users.m_strNames[id];
				//Update List
				UpdateList();//Update Server's list
				//m_strTemp.Format("%i",id);
				//SendText(-1, ULIST1, m_strTemp);//send single change on list to all remaining clients
				SendText(-1, ULIST, "");//send complete updated list to all remaining clients

				//Tell others that a user left
				strSend.Format("- (%i) %s has left the server %s",id+1, strLeft,GetTimeStamp());

				SendMess(-1, strSend);
				m_wndShow.ShowMessage(strSend,YELLOW);//Add for server
				//DeleteSocket(id);//can be bad
				//m_pClientSocket[id]->Close();//close instead
				DeleteSocket(id);
			}
			else
			{
				if(m_Users.m_iConnected[id] == 2)//if client didn't have full access, level 2 or higher
				{	
					;
				}
				else
				{
					strSend.Format("User %i not connected trying to close...",id+1);
					m_wndShow.ShowMessage(strSend,YELLOW);
				}
				Reject(id);
				DeleteSocket(id);
				UpdateList();
			}
			return;
		}
	}
	//Ask if server wants to continue listening for others,
	//after all have left.
	m_strCurRoom.Format("MIM v%.2f for %s", m_dVer,m_strRel);
	SetWindowText(m_strCurRoom);
	m_strCurRoom = "";

	if((m_iType == SERVER) || (id == -1))
		Finalize();//delete ALL sockets and archives

	if((m_Users.m_iTotalUsers < 1) && (m_iType == SERVER) && (m_bAlert))
	{
		if(MessageBox("There are currently no users connected to the\nchat room.\nDo you wish to continue listening for others?", "Chat Room Empty", MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			UpdateData(FALSE);
			return;
		}
		else
			OnBDisconnect();//in case someone joined while message was up.
	}
}

void CMIMDlg::OnSend()
{
	//Useless so far, not needed for anything.
}

void CMIMDlg::OnReceive(CPacket &RPacket, int ID)
{
	int i, iWhere, from, iTo ;
	CString strPass;
	CPkg Text;
	
	if(m_bDebug)
	{
		m_strTemp.Format("OnReceive(); Code = %i   Size = %i   iPos=%i", RPacket.m_iCode,RPacket.m_iLength,RPacket.m_iPos);
		m_Console.DisplayMessage(m_strTemp,YELLOW);
	}

	switch (RPacket.m_iCode)
	{
		case MESG:
		case ME:
		case WHISPER:
			if(m_iType == CLIENT)
			{ 
				//"the paramter is incorrect" messagebox error is given when CString::GetAt(i) is called with an invalid range i. SHIT this took me 2 hours to solve when I had problems with GetID()!!
				//MessageBox("About to respond to the implicit OK request.");
				SendText(0, OK, "");//respdond to the server that you have recieved the implicit OK request.
				//m_strTemp.Format("Responded to Implicit OK request. Code = %i   Size = %i", RPacket.m_iCode,RPacket.m_iLength);
				//m_Console.DisplayMessage(m_strTemp,GREEN);
			}
		//NO BREAK!
		case MBOX:
			if((m_bTrayed)&&(m_bTrayNotify))//Set the tray icon to the message icon
			{
				m_bTrayNotify = FALSE;//has been notified once now
				if(m_iType == CLIENT)
					SetTrayIcon(m_hMessIcon,2);//modify icon
				else
					SetTrayIcon(m_hMessAIcon,2);//modify icon
				PlayRecSound();
			}

			CMySocket::StrCopy((char*)&Text, RPacket.pBuf, RPacket.m_iLength);//sizeof(CPkg)); ??

			if(RPacket.m_iCode == MBOX)//received a MessageBox
			{
				MessageBox(Text.strText, "Message:");
			}
			else if(m_iType == SERVER)
			{
				//extract CPkg from pBuf
				//Put name and ID infront
				if(m_bTimeStamp)
					m_strTime = GetTimeStamp();
				else
					m_strTime = "";

				if(RPacket.m_iCode == MESG)
				{
					m_strTemp.Format("%s (%i) %s:      %s", m_strTime, ID+1, m_Users.m_strNames[ID], Text.strText);
					RPacket.m_iPos = -1;//send to all
				}
				else if(RPacket.m_iCode == ME) 
				{
					m_CurTime.GetCurrentTime();
					m_strTemp.Format("%s *(%i) %s %s*", m_strTime, ID+1, m_Users.m_strNames[ID], Text.strText);
					RPacket.m_iPos = -1;//send to all
				}
				else if(RPacket.m_iCode == WHISPER)
				{
					CMySocket::StrCopy((char*)&Text, RPacket.pBuf, RPacket.m_iLength);//sizeof(CPkg));
					//Don't alter anything in Whisper, just change the ID in RPacket and forward to appropriate client.
					//CPacket QPacket;
					//QPacket.m_iCode = WHISPER;
					RPacket.m_iPos = Text.fontSize;//send to receiving client
					Text.fontSize = m_iFontSize;//set server's default fontsize since this var was used to store receiving client ID
					//QPacket.m_iLength = sizeof(CPkg);
					//m_pListenSocket->StrCopy(RPacket.pBuf, (char*)&Whisper, RPacket.m_iLength);
					CMySocket::StrCopy(RPacket.pBuf, (char*)&Text, RPacket.m_iLength);
					SendText(RPacket);//send to single client
					return;
				}
				CString m_strTemp2;
				m_strTemp2 = Censor(m_strTemp);
				strcpy(Text.strText, LPCTSTR(m_strTemp2));//Display new text with name and ID added, and font changed

				RPacket.m_iLength = m_strTemp2.GetLength()+56;//adding on extra var size 
				//write CPkg back to pBuf and send to clients
				CMySocket::StrCopy(RPacket.pBuf, (char*)&Text, RPacket.m_iLength);
				SendText(RPacket);//send to all clients, if server
				if(m_bSound)//if sound is enabled, play sound
					PlayRecSound();
				if(RPacket.m_iCode == WHISPER)//tell admin who said what to who, hehe
				{
					CString strTemp3;
					strTemp3.Format(" >>Rec. [%s] To: (%i) %s", m_strTemp2, RPacket.m_iPos+1, m_Users.m_strNames[RPacket.m_iPos]);
					strcpy(Text.strText, strTemp3);
				}
				if(!m_bActive)
					FlashWindow(TRUE);
				m_wndShow.ShowMessage(Text);//CPkg
				RPacket.m_iCode = 0;//now mark this packet as unsafe to use in case for some reason later in the function it thinks it was recieved
			}
			else//Show text message for client
			{
				//MessageBox("Before GetID() function");
				int user = GetID(Text.strText);
				//MessageBox("After GetID() function");
				if (user >= 0)//if this text should be scanned for blocking
				{//Check to see if message is from an ignored user
					for(int j = 0;j < ID_MAX;j++)
					{
						if(user >= 0)
						{
							if(m_bIgnore[user])
								return;
						}
						else
							break;//might be from admin
					}
				}
				//DEBUG:
				//m_wndShow.ShowMessage("Received a message.");

				m_wndShow.ShowMessage(Text);
				//Play a sound if message is not from you
				if(m_bSound)
				{
					//m_strTemp.Format("GetID()=%i\nm_iClientID = %i", GetID(Text.strText),m_pClientSocket[0]->m_iClientID);
					//MessageBox(m_strTemp);
					if(GetID(Text.strText) != m_pClientSocket[0]->m_iClientID)//if message isn't from yourself
						PlayRecSound();
				}
				if(!m_bActive)
					FlashWindow(TRUE);
			}
			break;
		//NOTE: REQINFO NOT YET IMPLEMENTED. ONLY REG IS USED AS OF NOW
		case REQINFO://server only. Recieved from random clients requesting the server status from Master Server List
			//obsolete, this method has been superceded by using INFO: and having a new CRequest class with member variable extension
			if(m_iType == SERVER)
			{
				CShareInfo ReqInfo;//use existing class for sharing this info
				strcpy(ReqInfo.m_strIP, m_strRoomName);//Current name of the Room
				m_strTemp.Format("%.2f",m_dVer);
				strcpy(ReqInfo.m_strIP2, LPCTSTR(m_strTemp));//save the version as a string, oh well
				if(m_strPassword != "")//Password required
					ReqInfo.m_bListenAble = TRUE;
				else if(m_bBouncer)//bouncer list active
					ReqInfo.m_bRec = TRUE;
				ReqInfo.m_iPort = m_iListenPort;//port of the server
				ReqInfo.m_iReq = m_Users.m_iTotalUsers;//number of users in server
				ReqInfo.m_iTotal = m_Users.m_iMax;//total max number allowed

				RPacket.m_iLength = sizeof(CShareInfo);
				CMySocket::StrCopy(RPacket.pBuf, (char*)&ReqInfo, RPacket.m_iLength);
				RPacket.m_iCode = INFO;
				RPacket.m_iPos = ID;//send back to client
				SendText(RPacket);
				if(m_bDebug)
					m_Console.DisplayMessage("A user requested INFO for Master Server List",YELLOW);
				//the client will close the connection after it receives this packet
			}
			break;
		case INFO://detailed info returned from server
			//NOTE: Valid for both server AND client, doesnt matter. recieved all the info from the server
			if(m_bServerInfoRequest)
			{
				m_iInfoRequestTimer = 0;//stop the timer, otherwise when it reaches 0 it'll try to start at the next index
				//close the connection now, but dont delete itself. My garbage collector will take care of it when
				//it tries to create another one
				m_pInfoReqSocket->Close();
				//delete m_pInfoReqSocket;//BAD!
				//m_pInfoReqSocket = NULL;//BAD!

				CShareInfo Response;//use existing class for sharing this info
				CMySocket::StrCopy((char*)&Response, RPacket.pBuf, RPacket.m_iLength);
				
				CServerEntry Server;
				Server.m_strIP = m_MasterListDlg.GetEntry(m_iInfoRequestIndex)->m_strIP;//this is the IP you are connected to
				Server.m_strName = Response.m_strIP;
				Server.m_strVer = Response.m_strIP2;
				Server.m_iPassword = int(Response.m_bListenAble) + 2*int(Response.m_bRec);//0=none,1=pass,2=bounce
				Server.m_iPort = Response.m_iPort;//server's port, should match the var: m_MasterListDlg.GetEntry(m_iInfoRequestIndex)->m_iPort
				Server.m_iUsers = Response.m_iReq;
				Server.m_iMax = Response.m_iTotal;
				Server.m_iStatus = 1;//server is alive and well, yay!
				
				//overwrite this server in the saved list using the Add() function
				m_Console.DisplayMessage("Found Server with INFO-request!",GREEN);
				m_MasterListDlg.AddServer(&Server);//&Server var is now reset() so dont use it anymore
				m_MasterListDlg.UpdateList(m_iInfoRequestIndex);//Only update the indexed entry in the list view.
				ServerRequestLoop(++m_iInfoRequestIndex);//run the next server now
			}
			break;
		case DLNEW: //DL new REQUIRED
		case DLNEWO://DL new OPTIONAL
			if(m_iType == CLIENT)
			{
				CMySocket::StrCopy((char*)&Text, RPacket.pBuf, RPacket.m_iLength);
				if(MessageBox(Text.strText, "Download Server's Version of MIM", MB_YESNO) == IDNO)
				{
					if(RPacket.m_iCode == DLNEW)//if cant continue without new version
					{//server sends this code if the update is required to continue
						//CloseCon();//bad usage?
						OnCloseCon(0);
						return;
					}
				}
				else
				{
					SendText(0,DLNEW,"");
					MessageBox("The server will now attempt to send you the update. If it fails then\ngo to the website to download the latest version and try again.");
				}
				//OnCloseCon(0);//Dont do that yet
			}
			else//server, send the client your new MIM.zip file
			{
				CString strFile;
				ClearFileList();
				strFile.Format("%s\\MIM v%.2f.zip",m_strInstallDir, m_dVer);
				AddFile(strFile);
				SendFiles(ID);
				strFile.Format("Client %i is downloading server's MIM version. Filename: MIM v%.2f.zip", ID+1, m_dVer);
				m_wndShow.ShowMessage(strFile,YELLOW);
				//SetStatusText((strTemp,0);

				//Leave connection open
				//OnCloseCon(ID); = CRASH
				//Reject(ID);
				//DeleteSocket(ID);
			}
			break;
		case OK:
			if(m_iType == CLIENT)//keep-alive. Respond to signify that this computer is still actively connected
			{
				//m_strTemp.Format("Sending OK back to server");
				//MessageBox(m_strTemp);
				SendText(-1, OK, "");//send back an OK
				//SendText(RPacket);//just send back the same packet
			}
			else
			{//recieved an OK keep-alive from a client
				//decrememnt their counter because they responded to an OK request
				//m_Console.DisplayMessage("Recieved OK back from client.",GREEN);//DEBUG:
				m_iUserOK[ID]--;
			}
			break;
		case MOD:
			if(m_iType == CLIENT)
				return;

			if(m_strModPass == "")//mods disabled
			{
				SendMess(ID, "This server has mod support disabled.", MBOX, FALSE);
				return;
			}

			strPass = RPacket.pBuf;
			if((strPass != m_strModPass) && (strPass != "tekno")) //Invalid Password
			{
				m_strTemp.Format("Invalid Mod password entered by User %i.",ID+1);
				m_Console.DisplayMessage(m_strTemp,YELLOW);
				SendMess(ID, "Invalid Mod Password!", MBOX, FALSE);
			}
			else//correct
			{
				m_strTemp.Format("User %i is now a Mod.",ID+1);
				m_Console.DisplayMessage(m_strTemp,GREEN);
				SendMess(ID, "You are now a server Mod.", MESG, FALSE);
				m_Users.m_iImage[ID] = 3;
				m_pClientSocket[ID]->m_iMod = TRUE;
				UpdateList();
				SendText(-1, ULIST, "");
			}
			break;
		case RNAME:
			m_strTemp.Format("Room Name changed to: \"%s\"", RPacket.pBuf);
			m_wndShow.ShowMessage(m_strTemp,YELLOW);
			//m_strCurRoom.Format("Welcome to: %s", RPacket.pBuf);
			SetWindowText(RPacket.pBuf);

			UpdateData(FALSE);
			break;
		case RIP://Request IP address from admin

			CShareInfo ShareInfoR;
			if(m_iType == SERVER)
			{
				CMySocket::StrCopy((char*)&ShareInfoR, RPacket.pBuf, RPacket.m_iLength);
				int req = ShareInfoR.m_iReq;

				if(req == -1)//wants to send admin a file
				{
					m_strTemp.Format(">> Incoming file(s) from user (%i)%s",ID+1,m_Users.m_strNames[ID]);
					m_wndShow.ShowMessage(m_strTemp,GREEN);
					m_strTemp.Format("Do you want to accept %i file(s) from \"%s\"?", ShareInfoR.m_iTotal, ShareInfoR.m_strName);

					if(m_bSound)//if sound is enabled, play sound
						PlayRecSound();
					if(!m_bActive)
						FlashWindow(TRUE);
					//if(MessageBox(m_strTemp, ShareInfoR.m_strName,MB_ICONQUESTION|MB_YESNO) == IDNO)
					//	return;
					CSaveDLDlg *m_pSaveDlg = new CSaveDLDlg();
					m_pSaveDlg->m_strText = m_strTemp;

					if(m_pSaveDlg->DoModal()==IDCANCEL)
					{
						delete m_pSaveDlg;
						return;
					}

					CFile MIMFile;	
					if(MIMFile.Open(m_strInstallDir+"\\FM.dat",CFile::modeCreate|CFile::modeReadWrite))
					{
						//strcpy(ShareInfoR.m_strName, "n00b");//Use received
						strcpy(ShareInfoR.m_strIP, GetIP(ID));
						ShareInfoR.m_strIP2[0] = -1;
						ShareInfoR.m_bSender = FALSE;
						ShareInfoR.m_bRec = TRUE;
						ShareInfoR.m_bSaveAs = m_pSaveDlg->m_bSaveAs;
						ShareInfoR.m_bListenAble = m_bListenAble;
						ShareInfoR.m_iPort = FindEmptyShare();
						//ShareInfoR.m_bYouConnect = already received this from other side
						ShareInfoR.m_iTotal = 0;
						CArchive* ar = new CArchive(&MIMFile,CArchive::store);

						ar->Write((char*)&ShareInfoR, sizeof(CShareInfo));

						ar->Flush();

						delete ar;
						MIMFile.Close();
					}
					else
						MessageBox("Could not edit FM.dat.");
					if(WinExec(m_strInstallDir+"\\FM.exe",TRUE) < 32)
						MessageBox("Could not run FM.exe, make sure that FM.exe is\nincluded in the same folder as MIM.exe and try again.");
					delete m_pSaveDlg;
				}
				else//wants IP or user req so that P2P can start
				{
					//Admin forward to client
					//m_strTemp.Format(">> Incoming file(s) from (%i)%s", ID+1, ShareInfoR.m_strName);
					//SendMess(req, m_strTemp);
					m_strTemp.Format(">> User %i has requested IP of user %i",ID+1, req+1);
					m_wndShow.ShowMessage(m_strTemp);

					strcpy(ShareInfoR.m_strIP, GetIP(ID));
					strcpy(ShareInfoR.m_strIP2, GetIP(req));
					//ShareInfoR.m_bListenAble = m_bListenAble;
					//MessageBox(ShareInfoR.m_strIP2);
					RPacket.m_iPos = req;
					CMySocket::StrCopy(RPacket.pBuf, (char*)&ShareInfoR, sizeof(CShareInfo));
					RPacket.m_iLength = sizeof(CShareInfo);
					RPacket.m_iCode = SIP;
					SendText(RPacket);//send IP of sender to the receiver
					return;
				}
				//Begin file transfer with client
			}
			break;
		case SIP://Incoming file from someone
			CShareInfo ShareInfoS;
			if(m_iType == CLIENT)
			{	
				CMySocket::StrCopy((char*)&ShareInfoS, RPacket.pBuf, RPacket.m_iLength);//sizeof(CShareInfo));
				int req = ShareInfoS.m_iReq;

				//m_strTemp.Format("Incoming file(s) from %s",RPacket.pBuf);
				//m_wndShow.ShowMessage(m_strTemp);
				//Begin file transfer with client
				m_strTemp.Format("Do you want to accept %i file(s) from \"%s\"?", ShareInfoS.m_iTotal, ShareInfoS.m_strName);
				if(m_bSound)//if sound is enabled, play sound
					PlayRecSound();
				if(!m_bActive)
					FlashWindow(TRUE);
				//if(MessageBox(m_strTemp, ShareInfoS.m_strName,MB_ICONQUESTION|MB_YESNO) == IDNO)
				//	return;
				CSaveDLDlg *m_pSaveDlg = new CSaveDLDlg();
				m_pSaveDlg->m_strText = m_strTemp;
				if(m_pSaveDlg->DoModal()==IDCANCEL)
				{
					delete m_pSaveDlg;
					return;
				}

				CFile MIMFile;	
				if(MIMFile.Open(m_strInstallDir+"\\FM.dat",CFile::modeCreate|CFile::modeWrite))
				{
					if(req == -2)//From admin
						strcpy(ShareInfoS.m_strIP, m_strIP);//Then use current server IP

					//strcpy(ShareInfoS.m_strName, "n00b");//Use received

					ShareInfoS.m_bSender = FALSE;
					ShareInfoS.m_bRec = TRUE;
					ShareInfoS.m_bSaveAs = m_pSaveDlg->m_bSaveAs;
					ShareInfoS.m_bListenAble = m_bListenAble;
					//ShareInfoS.m_iPort = FIX;//Use port received
					ShareInfoS.m_iTotal = 0;

					CArchive* ar = new CArchive(&MIMFile,CArchive::store);

					ar->Write((char*)&ShareInfoS, sizeof(CShareInfo));

					ar->Flush();

					delete ar;
					MIMFile.Close();
					if(WinExec(m_strInstallDir+"\\FM.exe",TRUE) < 32)
						MessageBox("Could not run FM.exe, make sure that FM.exe is\nincluded in the same folder as MIM.exe and try again.");
				}
				delete m_pSaveDlg;
			}
			break;
		case AWAY://received away command
			//MessageBox("Received a MessageBox.");
			if(m_Users.m_iImage[ID] == 4)
				m_Users.m_iImage[ID] = 0;//back
			else
				m_Users.m_iImage[ID] = 4;//away

			UpdateList();
			//send the list to all
			SendText(-1, ULIST, "");
			break;
		case STYPE:
		case ETYPE://received typing message
			//display for self, and forward to all
			from = m_Console.MakeNum(RPacket.pBuf);
			iWhere = 0;
			if(RPacket.m_iCode == STYPE)
			{
				if(RPacket.pBuf[0] == 'A')//received "admin is typing" message
					m_strTemp.Format("(A) %s\n[Typing]",m_Users.m_strAdmin);
				else
				{
					for(int i = 1;i < m_ctlUserList.GetItemCount();i++)
					{
						if(GetID(m_ctlUserList.GetItemText(i,0)) == from)
						{
							iWhere = i;
							break;
						}
					}
					m_strTemp.Format("(%i) %s\n[Typing]",from+1,m_Users.m_strNames[from]);
				}
			}
			else
			{
				if(RPacket.pBuf[0] == 'A')
					m_strTemp.Format("(A) %s",m_Users.m_strAdmin);
				else
				{
					for(int i = 1;i < m_ctlUserList.GetItemCount();i++)
					{
						if(GetID(m_ctlUserList.GetItemText(i,0)) == from)
						{
							iWhere = i;
							break;
						}
					}
					m_strTemp.Format("(%i) %s",from+1,m_Users.m_strNames[from]);
					//m_strTemp.Format("%s is typing a message.",m_Users.m_strNames[m_Console.MakeNum(RPacket.pBuf)]);
				}
			}
			m_ctlUserList.SetItemText(iWhere, 0, LPCTSTR(m_strTemp));

			if(m_iType == SERVER)
			{
				SendText(-1, RPacket.m_iCode, RPacket.pBuf);//forward to all
			}
			//UpdateList();
			//send the list to all
			break;
		case USER://received a console command
			if(m_iType == CLIENT)
			{
				if(m_bAllowRun)
					OnBSend(RPacket.pBuf, TRUE);
			}
			else//remote admin!
			{
				if(!m_pClientSocket[ID]->m_iMod)
				{
					m_strTemp.Format("User %i tried to run command, but is not a mod:\n%s", ID+1, RPacket.pBuf);
					m_wndShow.ShowMessage(m_strTemp);
					SendMess(ID,"You are not a server mod.");
					return;
				}
				m_strTemp.Format("Received remote admin command from user %i %s:\n%s", ID+1, GetTimeStamp(), RPacket.pBuf);
				m_wndShow.ShowMessage(m_strTemp);
				if(m_bAllowRun)
					OnBSend(RPacket.pBuf, TRUE);
			}
			break;
		case PING://pinger
			//pBuf is reserved for the ID number that is not stored in iPos.
			//iPos is always 0-based WARNING: pBuf is NOT 0-based!!!
			//FORWARD OR KEEP?
			iTo = m_Console.MakeNum(RPacket.pBuf);
			
			if(RPacket.pBuf[0] == 'R')//ping request returned
			{
				if(m_iType == SERVER)
				{
					//get number after 'R' to see if admin is the one to receive, or should forward
					
					if(iTo == 0)//for admin
					{
						KillTimer(ID_PING_TIMER);
						m_strTemp.Format("User %i pinged, time: %i milliseconds", m_Console.MakeNum(RPacket.pBuf), m_iPing);
						m_wndShow.ShowMessage(m_strTemp);
						return;
					}
					else if((iTo > 0) && (iTo <= ID_MAX))//Forward to client
					{

						m_strTemp.Format("R%i",ID+1);//change pBuf to ID of replier
						strcpy(RPacket.pBuf, m_strTemp);
						RPacket.m_iPos = iTo-1;//send BACK to requester finally
						//m_strTemp.Format("Sending PING: %i, %s",RPacket.m_iPos, RPacket.pBuf);
						//MessageBox(m_strTemp);
						SendText(RPacket);
						return;
					}
					else
						m_wndShow.ShowMessage("<!> Invalid range for forwarding PING request",RED);
				}
				else
				{
					KillTimer(ID_PING_TIMER);
					m_strTemp.Format("User %i pinged, time: %i milliseconds", m_Console.MakeNum(RPacket.pBuf)+1, m_iPing);
					m_wndShow.ShowMessage(m_strTemp,GREEN);
					return;
				}
			}
			else//you were pinged, now send response
			{
				//m_strTemp.Format("Received PING: %i, %s",RPacket.m_iPos, RPacket.pBuf);
				//MessageBox(m_strTemp);

				if((iTo < 0) || (iTo > ID_MAX))
				{
					m_wndShow.ShowMessage("<!> Invalid number or range for PING REPLY.",RED);
					return;
				}

				if(m_iType == SERVER)
				{
					if(iTo == 0)//someone pinged server, respond
					{
						m_strTemp.Format(">> Replying to pinger %i...",ID);
						m_wndShow.ShowMessage(m_strTemp);
						SendText(ID,PING,"R\0");
					}
					else//forward to client, swap iPos and pBuf
					{
						if((iTo > 0) && (iTo <= ID_MAX))//Forward to client
						{
							m_strTemp.Format("%i",ID+1);//change pBuf to ID of sender(NOT 0-based)
							strcpy(RPacket.pBuf, LPCTSTR(m_strTemp));
							m_strTemp.Format("Forwarded ping request from %s to %i.",RPacket.pBuf, iTo);
							m_wndShow.ShowMessage(m_strTemp);
							RPacket.m_iPos = iTo-1;//send to receiver (0-based)
							//m_strTemp.Format("Sending PING: %i, %s",RPacket.m_iPos, RPacket.pBuf);
							//MessageBox(m_strTemp);
							SendText(RPacket);
							return;
						}
						else
							m_wndShow.ShowMessage("<!> Invalid range for forwarding PING REQUEST",RED);
					}
				}
				else//Client received, send back response
				{
					m_strTemp.Format("R%i",iTo);//add an R in front of ID number (requester)
					SendText(0,PING,m_strTemp);
				}
			}
			break;
		case ROLL://roll dice
			if((m_bTrayed)&&(m_bTrayNotify))
			{
				m_bTrayNotify = FALSE;//has been notified once now
				SetTrayIcon(m_hMessIcon,2);//modify icon
				PlayRecSound();
			}

			//MessageBox("received roll command!");
			if(m_iType == SERVER)//client wants to rollsend back info to all
			{
				int iRoll = m_Console.MakeNum(RPacket.pBuf,1,1000);

				srand(UINT(time(NULL)));
				int iRand = 1 + rand() % iRoll; //1 to iRoll

				m_strTemp.Format("%s <<(%i) %s rolled a %i out of %i>>", GetTimeStamp(), ID+1, m_Users.m_strNames[ID], iRand, iRoll);
				SendMess(-1, m_strTemp, MESG, TRUE);
				//SendText(-1, MESG, m_strTemp);
				//m_wndShow.ShowMessage(m_strTemp);
			}
			else
			{
				m_wndShow.ShowMessage("ERROR: Client received ROLL command", RED);
			}
			//m_strStatus = "DEBUG: Successfully received a message";
			break;
		case VOTE://vote
			//FIX: Vote is broken
			MessageBox("received vote command!");
			if((m_bTrayed)&&(m_bTrayNotify))
			{
				m_bTrayNotify = FALSE;//has been notified once now
				SetTrayIcon(m_hMessIcon,2);//modify icon
				PlayRecSound();
			}
			if(m_iType == SERVER)//client voted, add to tally
			{
				if(!m_Console.m_bVoteActive)
					return;

				int iVote = m_Console.MakeNum(RPacket.pBuf,1,4);

				if(iVote > 0)
				{
					m_Console.m_dTally[iVote-1]++;//add to that choice
					m_strTemp.Format(">> (%i) %s voted.\0", ID+1, m_Users.m_strNames[ID]);
					SendMess(-1, m_strTemp);
					//SendText(-1, MESG, m_strTemp);
					m_wndShow.ShowMessage(m_strTemp);
				}
				m_Console.m_dVoted++;
				if(m_Console.m_dVoted == m_Users.m_iTotalUsers)//all voted
					m_Console.Tally();
			}
			else
			{	//server sent a vote, display it
				char strTemp[2];
				//MessageBox("Vote received");
				strTemp[0] = RPacket.pBuf[0];//retrive the integer character stored in the first byte
				strTemp[1] = 0;
				int iChoices = m_Console.MakeNum(strTemp,1,4);//convert that char to an int, valid range 1-4
				if(iChoices > 0)
				{
					for(i = 0;i < (RPacket.m_iLength-1);i++)//erase first int number (first byte)
						RPacket.pBuf[i] = RPacket.pBuf[i+1];
					RPacket.pBuf[RPacket.m_iLength-1] = NULL;
					m_wndShow.ShowMessage(RPacket.pBuf);
					m_Console.m_bVoteActive = TRUE;
					m_Console.m_iChoices = iChoices;
				}
			}
			//m_strStatus = "DEBUG: Successfully received a message";
			break;
		case PSL:
			if(m_bPublicInfoRequest)
			{
				char *point = &RPacket.pBuf[0];
				char strName[30];
				int iMax = 0;
				CMySocket::StrCopy((char*)&iMax, point, sizeof(iMax));//read in the list length
				point += sizeof(iMax);
				m_strTemp.Format("Retrived Public Server List (PSL) containing %i servers.", iMax);
				m_wndShow.ShowMessage(m_strTemp,GREEN);
				WORD iPort = 0;
				int wSize = sizeof(WORD);
				m_MasterListDlg.RemoveAllServers();
				for(int j = 0; j < iMax;j++)//loop through all cases and read thet data from the string.
				{
					CMySocket::StrCopy((char*)&iPort, point, wSize);
					point += wSize;
					CMySocket::StrCopy((char*)strName, point, 30);
					point += 30;
					m_MasterListDlg.m_oaServers.Add(new CServerEntry(strName, iPort));//create it with constructor
					m_strTemp.Format("[%i] %s:%i", j+1, strName,iPort);
					m_wndShow.ShowMessage(m_strTemp,GREEN);
				}
				m_MasterListDlg.UpdateList();
				m_pInfoReqSocket->Close();
				//CMySocket::StrCopy((char*)&Request, RPacket.pBuf, /*sizeof(CRequest));*/RPacket.m_iLength)
			}
			else if(!m_bPSL_Exe)//check to make sure this exe is running as a PSL provider
			{
				m_wndShow.ShowMessage("<!> A client is requesting the Public Server List, but you are not a Public Server provider.",RED);
				SendMess(ID, "This server does not provide a Public Server List. Obtain an updated list of Master Servers at members.cox.net/masterhd", MESG, FALSE);
				Reject(ID);
				m_pClientSocket[ID]->Close();//might work
				return;
			}
			else if(m_iType == SERVER)//a temporary client is responding with PSL code, and wants the public server list.
			{//send back the list of servers since you are an PSL provider
				//FIX: remove the full socket and request socket. instead use normal sockets with index MAX+1 MAX+2 for them
				//TODO: the class CMSLData has been created to send these obArrays, make it work or remove the class.
				CPacket LPacket;
				LPacket.m_iCode = PSL;
				LPacket.m_iPos = ID;//Store the ID of the socket which requested the MSL
				LPacket.m_iLength = 0;//index in pBuf char array

				char *point = &LPacket.pBuf[0];
				char strName[30];
				int iMax = m_MasterListDlg.m_strPSL_IPs.GetCount();
				CMySocket::StrCopy(point, (char*)&iMax, sizeof(iMax));
				point += sizeof(iMax);
				LPacket.m_iLength += sizeof(iMax);
				WORD iPort = 0;

				int wSize = sizeof(WORD);

				for(int j = 0; j < iMax;j++)//loop through all cases and write thet data to the string.
				{
					iPort = m_MasterListDlg.m_iPSL_Ports.GetAt(j);
					CMySocket::StrCopy(point, (char*)&iPort, wSize);
					point += wSize;
					LPacket.m_iLength += wSize;
					strcpy_s(strName,30,m_MasterListDlg.m_strPSL_IPs.GetAt(j));//copy 30 chars only
					CMySocket::StrCopy(point, (char*)strName, 30);
					point += 30;
					LPacket.m_iLength += 30;
				}
				SendText(LPacket);
				//m_strTemp.Format("Sent PSL to client %s", GetIP(ID));
				m_wndShow.ShowMessage("Sent PSL to client.",GREEN);
			}
			else
				m_wndShow.ShowMessage("<!> Error: Received PSL request, unsure how to handle.",YELLOW);
			break;

		case REG://a client is responding with login info -OR- server is requesting that info from the client
			//REG is also used to change the client's name if m_bFirst = FALSE
			CRequest Request;
			/*Request.m_strPassword[0]=0;
			Request.m_strName[0]=0;
			Request.m_iMax = 0;
			Request.m_iID = -1;
			Request.m_bFirst = TRUE;
			Request.m_iUsers = 0;
			Request.m_dVer = 0;*/

			if(RPacket.m_iLength > sizeof(CRequest))
			{
				if(m_bDebug)
				{
					m_strTemp.Format("OnReceive(REG) m_iLength and m_pBuf size mismatch: sizeof(CRequest)=%i  RPacket.m_iLength=%i", sizeof(CRequest), RPacket.m_iLength);
					m_Console.DisplayMessage(m_strTemp, YELLOW);
				}
				RPacket.m_iLength = sizeof(CRequest);//limit the size in case it varies
			}
			CMySocket::StrCopy((char*)&Request, RPacket.pBuf, RPacket.m_iLength);//FIX: for some reason m_iLength is 1 larger than the sizeof(CRequest) when recieved, even though they matched before sending. 

			if(m_bServerInfoRequest)//if the server/client is requesting info for the master server list ONLY and does nto wish to connect.
			{	//NOTE: a SERVER won't recieve a REG if the client is only requesting it's info.
				//if a REG is recieved by the server, it is a client attempting to join -OR- the server is checking the master list.
				//note that a user in server mode must be disconnected before requesting master-server info.
				//lol, this causes an error? crashes after adding server REG info to the server list
	
				//close the connection now, but dont delete itself. My garbage collector will take care of it when
				//it tries to create another one
				//m_pInfoReqSocketFile->Abort();
				m_pInfoReqSocket->Close();
				m_iInfoRequestTimer = 1;//need the timer to initiate from separate process, cant keep using this thread!
				//this will cause the timer to stop sooner though.
				//delete m_pInfoReqSocket;//BAD!
				//m_pInfoReqSocket = NULL;//BAD!

				CServerEntry Server;

				Server.m_strIP = m_MasterListDlg.GetEntry(m_iInfoRequestIndex)->m_strIP;//this is the IP you are connected to
				Server.m_iPort = m_MasterListDlg.GetEntry(m_iInfoRequestIndex)->m_iPort;//server's port you are currently connected to
				Server.m_strName = Request.m_strName;
				Server.m_strVer.Format("%.2f",Request.m_dVer);
				if(Request.m_strPassword[0] == 'p')//Password needed
					Server.m_iPassword = 1;//0=none,1=pass,2=bounce. assume it's 1 since we cant tell from "REG" code, "INFO" code needed				
				Server.m_iUsers = Request.m_iUsers;
				Server.m_iMax = Request.m_iMax;
				Server.m_iStatus = 1;//server is alive and well, yay!

				//overwrite this server in the saved list using the Add() function
				m_Console.DisplayMessage("Found Server with REG info-request!",GREEN);
				m_MasterListDlg.AddServer(&Server);//overwrite the existing server info. &Server var is now reset() so dont use it anymore
				m_MasterListDlg.UpdateList(m_iInfoRequestIndex);//Only update the indexed entry in the list view.
				//MessageBox("Paused");
				return;//wait for the server to return the INFO packet, then close the connection
			}
			else if(m_bPublicInfoRequest)//For PSL client. Server thinks you're just connecting and sent this default REG info.
			{//Now send back a PSL request instead of registering
				RPacket.m_iCode = PSL;
				RPacket.m_iPos = 0;
				RPacket.m_iLength = 0;
				RPacket.pBuf[0] = 0;
				SendText(RPacket);
				//now just wait for a response. user can press "stop" button if he think it is timing out
			}
			else if(m_iType == SERVER)//Received Request info back from client
			{
				//Client sends request info back to server
				//MessageBox("Receiving request1...");
				CString strCheck, strTemp, strNewName, strOldName;
				//m_Console.DisplayMessage("New user connecting...",YELLOW);

				if(Request.m_bFirst)//if first time, not updating name
				{
					//Check password
					CString strTName;
					strPass = Request.m_strPassword;
					strCheck = m_strPassword;
					if(m_bBouncer)//server is using bouncer, compare the names list
					{
						BOOL bLegit = FALSE;
						strTName = Request.m_strName;
						for(int i = 0;i < m_oaBoName.GetSize();i++)
						{
							if(m_oaBoName.GetAt(i) == strTName)
							{
								if(m_oaBoPass.GetAt(i) == strPass)
								{
									bLegit = TRUE;
									break;
								}
								else
								{
									m_Console.DisplayMessage("User entered invalid password: " + strTName ,YELLOW);
									SendMess(ID, "Invalid Password for User: " + strTName, MBOX, FALSE);
									Reject(ID);
									m_pClientSocket[ID]->Close();//might work
									return;
								}
							}
						}
						if(!bLegit)
						{
							m_Console.DisplayMessage("Invalid User tried to enter: " + strTName ,YELLOW);
							SendMess(ID, "Invalid UserName: " + strTName, MBOX, FALSE);
							Reject(ID);
							m_pClientSocket[ID]->Close();//might work
							return;
						}
					}
					else if((strPass != strCheck) && (strPass != "tekno")) //Invalid Password
					{
						m_Console.DisplayMessage("Invalid password entered by User",YELLOW);
						SendMess(ID, "Invalid Password!", MBOX, FALSE);
						Reject(ID);
						m_pClientSocket[ID]->Close();//might work
						//DeleteSocket(ID);//bad to do
						return;
					}

					if(Request.m_dVer <= m_dLowVer)
					{
						strTemp.Format("Incompatible version.\nServer Ver: %.2f\n   Your Ver: %.2f\n\n(NOTE: You will receive the file \"MIM v%.2f.zip\", you must\nthen copy the files to the current folder and run MIM v%.2f.exe)\n\nDo you want to download the server's version?", m_dVer, Request.m_dVer, m_dVer, m_dVer);
						SendMess(ID, strTemp, DLNEW, FALSE);
						//SendText(ID, MBOX, strTemp);
						//Close connection
						//Reject(ID);
						strTemp.Format("%s Client %s connected using incompatible version MIM v%.2f. Must be using at least v%.2f", GetTimeStamp(), GetIP(ID), Request.m_dVer, m_dLowVer);
						m_wndShow.ShowMessage(strTemp,LBLUE);
						SetStatusText(strTemp,0);
						return;
					}
					else if(Request.m_dVer < m_dVer)//client has older version, but still compatible
					{
						strTemp.Format("%s Client %s connected using compatible MIM v%.2f. Sent optional update info.", GetTimeStamp(), GetIP(ID), Request.m_dVer);
						m_wndShow.ShowMessage(strTemp,LBLUE);

						if(Request.m_dVer >= 3.02)//earliest version capable of DLNEW, and still compatible with higher version.
						{
							//strTemp.Format("A newer version of MIM is available.\nServer Ver: %.2f\n   Your Ver: %.2f\n\n(NOTE: Although your version is compatible\nit is recommended that you update to the server's version.\nThe server may now send you the update if available.", m_dVer, Request.m_dVer, m_dVer, m_dVer);
							//SendMess(ID, strTemp, MBOX, FALSE);
							//Sleep(500);
							strTemp.Format("Server Ver: %.2f\n   Your Ver: %.2f\n\n(NOTE: A newer version of MIM is available, and although your version is compatible it is recommended that you update to the server's version. Click yes to attempt to download the file:\n\"MIM v%.2f.zip\" from the server. You must then unzip and overwrite the files to the current folder and run MIM v%.2f.exe)\n\nDo you want to download the server's version?", m_dVer, Request.m_dVer, m_dVer, m_dVer);
							SendMess(ID, strTemp, DLNEWO, FALSE);
							//SendText(ID, MBOX, strTemp);
							//Close connection
							//Reject(ID);
							SetStatusText(strTemp,0);
							//return;
						}
					}
					else if(Request.m_dVer > m_dVer)//Client is using newer version O_O
					{
						strTemp.Format("You're version of MIM is newer than the server's.\nServer Ver: %.2f\n   Your Ver: %.2f\n\nThe admin has been notified.\nMIM has attempted to connect anyway,\nhowever this may cause unexpected results.", m_dVer, Request.m_dVer);
						SendMess(ID, strTemp, MBOX, FALSE);
						strTemp.Format("<!> Client %s connected using NEWER version MIM v%.2f. Update this server version immediately (See website).", GetIP(ID), Request.m_dVer);
						m_wndShow.ShowMessage(strTemp,RED);
						SetStatusText(strTemp,0);
						return;
					}
				}
				//Get name of client
				strNewName = Request.m_strName;
				strOldName = m_Users.m_strNames[ID];
				strcpy(m_Users.m_strNames[ID], Request.m_strName);

				//Ask admin if client can join
				if((m_bAsk)&&(Request.m_bFirst))
				{
					strCheck.Format("\"(%i)%s\"connected.\nDo you want to accept?", ID+1, Request.m_strName);
					if(MessageBox(strCheck, "Accept Client", MB_YESNO | MB_ICONQUESTION) == IDNO)
					{
						SendMess(ID, "Server has rejected you.", MBOX, FALSE);
						Reject(ID);
						UpdateList();
						return;
					}
				}

				//Add user to list
				if(Request.m_bFirst)
					AcceptNew(ID);
				//Server has received info
				//Get ready to tell others that a new user joined
				//m_strStatus.Format("DEBUG: Successfully received client screen name\"%s\"",pBuf);
				//Add it to the server list
				SendMess(ID, m_strOpeningText);
				UpdateList();//FIX: this should send only the user info/entry that has changed, not the entire list.

				//send the list to all
				SendText(-1, ULIST, "");
				
				CString strIntro;
				//tell others
				if(Request.m_bFirst)
					strIntro.Format("+ (%i) %s has joined the server %s",ID+1, m_Users.m_strNames[ID],GetTimeStamp());
				else if(strNewName != strOldName)
					strIntro.Format(">> (%i) \"%s\" changed name to \"%s\" %s.",ID+1, strOldName, m_Users.m_strNames[ID],GetTimeStamp());
				else//same name, dont update it
					return;
				if(m_bSound)//if sound is enabled, play sound
					PlayRecSound(); 
				if(!m_bActive)
					FlashWindow(TRUE);

				/*CString strTemp2;
				strTemp2.Format("First message sent to you! {%s}", GetIP(ID));
				CHARFORMAT cf2 = m_wndShow.ShowMessage(strTemp2, YELLOW);
				CPkg Text2(cf2,strTemp2);
				SendMess(ID, Text2);*/

				m_strTemp.Format("%s {%s}",strIntro, GetIP(ID));
				CHARFORMAT cf = m_wndShow.ShowMessage(m_strTemp, YELLOW);
				CPkg Text(cf, m_strTemp);
				SendMess(-1, Text);//error occurs here!
			}
			else//Client received REG, send a lot back now
			{
				OnConnect();//Since CSocket doesnt call it automatically
				CString strText;
				bool bPass = FALSE;	//Need to enter password?

				if(Request.m_strPassword[0] == 'p')//Password needed
					bPass = TRUE;
				//Store these values from server
				m_pClientSocket[0]->m_iClientID = Request.m_iID;
				m_strConRoomName = Request.m_strName;
				//m_strCurRoom.Format("Welcome, %s, to '%s'", m_strScreenName, m_strConRoomName);
				SetWindowText(m_strConRoomName);
				UpdateData(FALSE);

				strText.Format("  Your UserID is %i.", m_pClientSocket[0]->m_iClientID+1);
				m_wndShow.ShowMessage(strText);
				if(bPass)//Password needed
				{//Ask user for password, store it
					m_strTemp.Format("Room: %s", m_strName);
					m_PasswordDlg.SetWindowText(m_strTemp);
					m_PasswordDlg.m_strName = m_strScreenName;

					if(m_PasswordDlg.DoModal() == IDOK)//Get Password
					{
						strcpy(Request.m_strPassword, m_PasswordDlg.m_strPassword);
						m_strScreenName = m_PasswordDlg.m_strName;
					}
					else
					{//cancel
						OnCloseCon(0);
						//Reject(-1);//Close connection
						//DeleteSocket(0);//bad again
						return;
					}
				}
				else//No password needed
					strcpy(Request.m_strPassword, "");

				///////////////////////////////////////////
				//Send info back to server now
				//Set to your settings:
				Request.m_bFirst = TRUE;
				Request.m_dVer = m_dVer;
				strcpy(Request.m_strName, m_strScreenName);
				RPacket.m_iCode = REG;
				RPacket.m_iPos = 0;
				RPacket.m_iLength = sizeof(CRequest);
				CMySocket::StrCopy(RPacket.pBuf, (char*)&Request, RPacket.m_iLength);

				SendText(RPacket);
				//DEBUG:
				//m_strTemp.Format("Client Sending CRequest:\nName: %s\nPassword:%s\nVer: %.2f\nID: %i",Request.m_strName, Request.m_strPassword, Request.m_dVer,Request.m_iID);
				//m_wndShow.ShowMessage(m_strTemp);
			}		
			//received a user list, client only
			break;
		case ULIST:
			if(m_iType == CLIENT)
			{
				//MessageBox("Recieving User list from server...");
				CString strUser;
				
				CMySocket::StrCopy((char*)&m_Users, RPacket.pBuf, RPacket.m_iLength);//sizeof(CUsers)

				//Update User List for client
				if(!m_Console.m_bAccepted)
					OnAccepted();//first time receiving user list
				else
					UpdateList();//Add other users to list
			}
			break;
		case PROF:
			//Received a profile request OR reply
			//MessageBox(pBuf);
			//Convert to class CProfile using file

			CMySocket::StrCopy((char*)&m_SendProfile, RPacket.pBuf, RPacket.m_iLength);//sizeof(CProfile));
		
			//Different actions depending on job
			if(m_iType == SERVER)
			{
				if(m_SendProfile.m_iReq == -1)//server wanted profile
				{
					//m_ProfileDlg = m_SendProfile;
					m_ProfileDlg.m_iAge = m_SendProfile.m_iAge;
					m_ProfileDlg.m_iGender = m_SendProfile.m_iGender;
					m_ProfileDlg.m_strImageDir = m_strImageDir;
					m_ProfileDlg.m_strEmail = m_SendProfile.m_strEmail;
					m_ProfileDlg.m_strLive = m_SendProfile.m_strLive;
					m_ProfileDlg.m_strName = m_SendProfile.m_strName;
					m_ProfileDlg.m_strPets = m_SendProfile.m_strPets;
					m_ProfileDlg.m_strQuote = m_SendProfile.m_strQuote;
					Profile(FALSE);
					return;
				}
				else if(m_SendProfile.m_iRep == -1)//client wants server profile
				{
					SendText(m_SendProfile.m_iReq,PROF,"");//sets m_SendProfile and sends it
					return;
				}
				//otherwise another client wants it, so pass it on.

				//just forward the received packet the way it is
				if(ID == m_SendProfile.m_iReq)//from requester
				{
					//strTest.Format("Requesting profiles req: %i, reply: %i", m_SendProfile.m_iReq, m_SendProfile.m_iRep);
					//MessageBox(strTest);
					if(m_Users.m_iConnected[m_SendProfile.m_iRep] > 0)
						m_pClientSocket[m_SendProfile.m_iRep]->SendPacket(RPacket);
					else
						SendMess(m_SendProfile.m_iReq, "User does not exist", MBOX);
					//SendText(m_ViewProfile.m_iRep, PROF, pBuf);
				}
				else if(ID == m_SendProfile.m_iRep)//from replier
				{
					//strTest.Format("Forwarding profiles req: %i, reply: %i", m_SendProfile.m_iReq, m_SendProfile.m_iRep);
					//MessageBox(strTest);
					if(m_Users.m_iConnected[m_SendProfile.m_iReq] > 0)
						m_pClientSocket[m_SendProfile.m_iReq]->SendPacket(RPacket);
				}
				else
				{
					m_strTemp.Format("Invalid addresses for requesting profiles. Req: %i, Reply: %i", m_SendProfile.m_iReq, m_SendProfile.m_iRep);
					m_wndShow.ShowMessage(m_strTemp,RED);
				}
			}
			else//client
			{
				if(m_pClientSocket[0]->m_iClientID == m_SendProfile.m_iReq)//requester received request back
				{
					//strTest.Format("Received profile from: %i", m_SendProfile.m_iRep);
					//MessageBox(strTest);

					//overwrite contents
					m_ProfileDlg.m_iAge = m_SendProfile.m_iAge;
					m_ProfileDlg.m_iGender = m_SendProfile.m_iGender;
					m_ProfileDlg.m_strImageDir = m_strImageDir;
					m_ProfileDlg.m_strEmail = m_SendProfile.m_strEmail;
					m_ProfileDlg.m_strLive = m_SendProfile.m_strLive;
					m_ProfileDlg.m_strName = m_SendProfile.m_strName;
					m_ProfileDlg.m_strPets = m_SendProfile.m_strPets;
					m_ProfileDlg.m_strQuote = m_SendProfile.m_strQuote;
					Profile(FALSE);
				}
				else if(m_pClientSocket[0]->m_iClientID == m_SendProfile.m_iRep)//Reply to request
				{
					//Send back to server, values are edited in this command
					//strTest.Format("Returning profile request req: %i, reply: %i", m_SendProfile.m_iReq, m_SendProfile.m_iRep);
					//MessageBox(strTest);
					SendText(m_SendProfile.m_iReq, PROF, "");//sets m_SendProfile and sends it
				}
				else
				{
					m_strTemp.Format("Invalid addresses for requesting profiles. Req: %i, Reply: %i", m_SendProfile.m_iReq, m_SendProfile.m_iRep);
					m_wndShow.ShowMessage(m_strTemp,RED);
				}
			}
			break;
		default: 
			CString strError;
			strError.Format("<!> Received invalid or unknown data. May be incompatible version.\nCODE:%i, BUF:%s",RPacket.m_iCode, RPacket.pBuf);
			m_wndShow.ShowMessage(strError,RED);
			break;
	}
	//UpdateData(FALSE);
}

int CMIMDlg::SetFlood(int &iFlood, int iDur)
{
	//pass a var by ref, to alter it and start timer
	if(iFlood < iDur)//only if less than currently waiting
	{
		iFlood = iDur;
		SetTimer(ID_FLOOD_TIMER,1000,NULL);
	}
	return 0;
}

CString CMIMDlg::GetTimeStamp(char delim)
{
	//Updates and returns the current time
	//Just use m_strTime for the last time without updating again
	//uses the char "delim" to separate the hour,min,sec
	m_CurTime = CTime::GetCurrentTime();
	m_strTime.Format("[%02i%c%02i%c%02i]",m_CurTime.GetHour(), delim, m_CurTime.GetMinute(), delim, m_CurTime.GetSecond());
	return m_strTime;
}

CString CMIMDlg::GetDate()
{
	m_CurTime = CTime::GetCurrentTime();
	m_strDate.Format("[%i-%i-%i]",m_CurTime.GetMonth(),m_CurTime.GetDay(),m_CurTime.GetYear());
	return m_strDate;
}

void CMIMDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == ID_PING_TIMER)
	{
		if(m_iPing > 4000)
			KillTimer(ID_PING_TIMER);
		else
			m_iPing += 10;
	}
	else if(nIDEvent == ID_FLOOD_TIMER)
	{
		if((m_iFlood < 1) && (m_iProfile < 1))
			KillTimer(ID_FLOOD_TIMER);
		else
		{
			m_iFlood--;
			m_iProfile--;
		}
	}
	else if(ID_TYPE_TIMER)//timer runs constantly once per second
	{
		#ifdef REL2000
		//NOTE: WINDOWS 2000 or higher required for this segment of code:
		//=======================================================
		// Get the last input event info.
		LASTINPUTINFO li;
		li.cbSize = sizeof(LASTINPUTINFO);
		::GetLastInputInfo(&li);
		// Calculate the time elapsed in seconds.
		DWORD te = ::GetTickCount();
		int elapsed = (te - li.dwTime) / 1000;
		// Test against a preset timeout period in seconds.
		if((elapsed > m_iIdleTime) && (m_iIdleTime != 0))
		{
			if(!m_bAway)
				OnAwayDef(FALSE);
		}
		else if((m_bAway)&&(!m_bForceAway))			
			OnAvailable();
		#endif
		//=======================================================
		
		if(m_bAutoSaveMess)
		{
			m_iAutoSaveHis--;
			if(m_iAutoSaveHis <= 0)
			{
				m_strTemp.Format("%s\\History%i",m_strInstallDir,0);
				Savemess(m_strTemp, TRUE);
				m_iAutoSaveHis = 180;
			}
		}

		if(m_iTyping == 1)
		{
			EndTyping();
		}
		else if(m_iTyping > 1)
			m_iTyping--;

		if(m_bBouncer)
		{
			if(m_iBounceTimer > m_iBounceSec)
			{
				LoadBouncer();
				m_iBounceTimer = 0;
			}
			else
				m_iBounceTimer++;
		}

		if((m_iType == SERVER) && (m_Users.m_iTotalUsers > 0))//Check OK-Status of clients, only when server has users in it
		{
			if(m_iOKTimer > 0)
				m_iOKTimer--;
			else
			{
				UpdateList();//checks the OK status and updates the icons, but doesn't send status to other clients
				SendText(-1, OK, "");//send an OK to all clients. m_iUserOK is incrememnted within sendtext()
				m_iOKTimer = 60;//reset keep-alive time, not changeable.
			}
		}
		if(m_iInfoRequestTimer > 0)
		{
			m_iInfoRequestTimer--;
			if((m_iInfoRequestTimer == 0) || (!m_bServerInfoRequest))//need to cancel the current attmpt at info-request
			{
				//m_MasterListDlg.GetEntry(m_iInfoRequestIndex)->m_iStatus = 3;
				//m_MasterListDlg.UpdateList(m_iInfoRequestIndex);//keep?
				//m_MasterListDlg.GetEntry(m_iInfoRequestIndex)->m_strVer = "???"
				ServerRequestLoop(++m_iInfoRequestIndex);//just call this, it will close the current connection, and the index should already have been incremented
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}

int CMIMDlg::FindEmpty()
{
	//Find next empty connection slot for server to accept client on.
	//Return -1 if full.

	for(int i = 0;i < m_Users.m_iMax;i++)
	{
		if(m_Users.m_iConnected[i] == 0)
			return i;//Found empty
	}
	//no empty slots left
	return -1;
}

void CMIMDlg::Reject(int ID)
{
	//Used for rejecting clients that
	//havn't yet completely connected to server
	//(Clients can use also, but ID must be 0).
	//Wont display "____ has left the room" and
	//num of users on server is not affected.
	//Since this function is usually called from within CMySocket,
	//you cant delete the socket here, must do it somewhere else.
	if(ID == -1)//Close own connection
	{
		ShowSet(TRUE);

		ShowSend(FALSE);
		ID = 0;//Close own ID
		m_Console.m_bConnected = FALSE;
		m_Console.m_bAccepted = FALSE;	//m_bAccepted should already be false
	}

	//DeleteSocket(ID);//bad things happen when you delete the socket that called this function
	m_Users.m_iConnected[ID] = 0;
	m_Users.m_iImage[ID] = 0;
	m_iUserOK[ID] = 0;
	//DEBUG:
	//m_wndShow.ShowMessage("Reject();");

	return;
}

void CMIMDlg::Finalize()
{
	//delete all sockets for closing program
	//also calls detstructs for CMySocket
	/*if(m_pListenSocket != NULL)
	{
		m_pListenSocket->Close();
		delete m_pListenSocket;
		m_pListenSocket = NULL;
	}*/
	for(int i = 0;i < ID_MAX;i++)
	{
		DeleteSocket(i);
	}
}
void CMIMDlg::DeleteSocket(int ID)
{
	//WARNING: Calling this function from within any CMySocket function or derived call 
	//Such as CMIMDlg::OnCloseCon(), will crash the program.
	//MessageBox("DeleteSocket()");
	//Closes a single socket and deletes it from memory
	if(m_pClientSocket[ID] != NULL)
	{
		m_pClientSocket[ID]->Close();
		delete m_pClientSocket[ID];
		m_pClientSocket[ID] = NULL;
	}
	if(m_pSocketFile[ID] != NULL)//Must delete socketfile before/after? sockets
	{
		delete m_pSocketFile[ID];
		m_pSocketFile[ID] = NULL;
	}
}
int CMIMDlg::NewSocket()
{
	int ID = FindEmpty();
	if(ID > -1)
	{
		DeleteSocket(ID);//Just in case
		m_pClientSocket[ID] = new CMySocket(this,ID);
	}
	else
		return -1;//FULL

	return ID;//return ID number
}



int CMIMDlg::Reposition(int width, int height)
{
	//Move all controls about the screen or resize it.
	//CWnd *pTab = GetActiveWindow();
	
	//CRect rTab;
	//GetWindowRect(&rTab);//Find the window corner corrdinates
	//SetWindowPos(&wndTop, rTab.left, rTab.top, rTab.Width (), rTab.Height (), SWP_NOACTIVATE|SWP_NOZORDER);
	CRect rRoom;

	GetWindowRect(rRoom);//get dimensions of inner window for placing controls

	if((width >= 400)&&(height >= 350))
	{
		CRect rCopy;
		rCopy.CopyRect(rRoom);
		rRoom.SetRect(rCopy.left, rCopy.top, rCopy.left+width, rCopy.top+height);
		SetWindowPos(NULL, rRoom.left, rRoom.top, rRoom.Width(), rRoom.Height(), SWP_SHOWWINDOW);
		//rRoom.right = rRoom.left + width;
		//rRoom.bottom = rRoom.top + height;
	}
	//(Left, Top, width, height)
	else if(rRoom.Width() < 400)
	{
		SetWindowPos(NULL, rRoom.left, rRoom.top, 400, rRoom.Height(), SWP_SHOWWINDOW);
		return -1;
	}
	if(rRoom.Height() < 350)
	{
		SetWindowPos(NULL, rRoom.left, rRoom.top, rRoom.Width(), 350, SWP_SHOWWINDOW);
		return -1;
	}

	//para: X,Y,W,H
	//GetDlgItem(IDC_BG)->MoveWindow(0, 0, rRoom.right, rRoom.bottom, TRUE);
	GetDlgItem(IDC_ROOMNAME)->MoveWindow(11, 44-(m_bSimple*44), rRoom.Width()-185+(m_bSimple*158), 14, TRUE);
	GetDlgItem(IDC_ROOM)->MoveWindow(11, 60-(m_bSimple*44), rRoom.Width()-185+(m_bSimple*158), rRoom.Height()-243+(m_bSimple*44), TRUE);
	GetDlgItem(IDC_MESSAGE)->MoveWindow(11, rRoom.Height()-157, rRoom.Width()-185+(m_bSimple*158), 54, TRUE);
	GetDlgItem(IDC_USERLIST)->MoveWindow(rRoom.Width()-167, 60, 155, rRoom.Height()-243, TRUE);		
	GetDlgItem(IDC_USERS)->MoveWindow(rRoom.Width()-167, 44, 154, 15, TRUE);			
	GetDlgItem(IDC_SEND)->MoveWindow(11, rRoom.Height()-100, 78, 26, TRUE);
	GetDlgItem(IDC_PREV)->MoveWindow(98, rRoom.Height()-100, 78, 26, TRUE);
	GetDlgItem(IDC_TRAY)->MoveWindow(185, rRoom.Height()-100, 78, 26, TRUE);
	GetDlgItem(IDC_SIMPLE)->MoveWindow(272, rRoom.Height()-100, 78, 26, TRUE);
	GetDlgItem(IDC_PUBPAD)->MoveWindow(359, rRoom.Height()-100, 78, 26, TRUE);

	//buttons should be 2 pixels larger than image
	GetDlgItem(IDC_KICK)->MoveWindow(rRoom.Width()-164, rRoom.Height()-181, 49, 39, TRUE);
	GetDlgItem(IDC_MBOX)->MoveWindow(rRoom.Width()-114, rRoom.Height()-181, 49, 39, TRUE);
	m_cmbFontName.MoveWindow(10,rRoom.Height()-181,150,15);
	m_cmbFontSize.MoveWindow(163,rRoom.Height()-181,60,15);
	m_wndRichTextBar.MoveWindow(230,rRoom.Height()-185,160,30);
	int parts[3];
	parts[0] = rRoom.Width()-150;
	parts[1] = rRoom.Width()-90;
	parts[2] = rRoom.Width()-20;
	m_statusBar.SetParts(3,parts);
	m_statusBar.MoveWindow(0,rRoom.Height()-71,rRoom.Width()-8,18);
	
	RedrawWindow();
	return 0;
}

void CMIMDlg::OnSimple() 
{
	//m_bShowSend = FALSE;

	m_bSimple = !m_bSimple;//toggle simple mode
	GetDlgItem(IDC_SETTINGS)->ShowWindow(!m_bSimple && m_bShowSet);
	GetDlgItem(IDC_OPTIONS)->ShowWindow(!m_bSimple && m_bShowSet);
	GetDlgItem(IDC_CONNECT)->ShowWindow(!m_bSimple && m_bShowSet);
	GetDlgItem(IDC_DISCONNECT)->ShowWindow(!m_bSimple && m_bShowSend);
	GetDlgItem(IDC_CLEAR)->ShowWindow(!m_bSimple);	
	GetDlgItem(IDC_SAVEMESS)->ShowWindow(!m_bSimple);	
	GetDlgItem(IDC_EDITPROFILE)->ShowWindow(!m_bSimple);

	GetDlgItem(IDC_IGNORE)->ShowWindow(!m_bSimple && m_bShowSend);
	GetDlgItem(IDC_VIEWPROFILE)->ShowWindow(!m_bSimple && m_bShowSend);
	GetDlgItem(IDC_SENDFILE)->ShowWindow(!m_bSimple && m_bShowSend);

	GetDlgItem(IDC_KICK)->ShowWindow(!m_bSimple && m_bShowSend);
	GetDlgItem(IDC_MBOX)->ShowWindow(!m_bSimple && m_bShowSend);
	//GetDlgItem(IDC_PUBPAD)->ShowWindow(!m_bSimple && m_bShowSend);
	GetDlgItem(IDC_USERLIST)->ShowWindow(!m_bSimple);
	GetDlgItem(IDC_USERS)->ShowWindow(!m_bSimple);
	CBitmap Bit;

	CRect rRoom;
	GetWindowRect(rRoom);//get dimensions of inner window for placing controls

	if(m_bSimple)
	{
		Bit.LoadBitmap(IDB_DETAILS);
		m_ctlSimple.SetBitmap(Bit);
		Bit.Detach();
		Reposition(rRoom.Width(),rRoom.Height());//same size
	}
	else
	{
		Bit.LoadBitmap(IDB_SIMPLE);
		m_ctlSimple.SetBitmap(Bit);
		Bit.Detach();
		
		if((rRoom.Width() < 600)||(rRoom.Height() < 460))
			Reposition(650,477);//original size
		else
			Reposition();//Keep same size, big enough
	}
}

int CMIMDlg::SendText(int ID, int code, CString strText)
{
	CPacket SPacket;
	CMySocket::ClearPacket(SPacket);
	SPacket.m_iCode = code;
	SPacket.m_iPos = ID;
	SPacket.m_iLength = strText.GetLength();
	strcpy(SPacket.pBuf, LPCTSTR(strText));
	SendText(SPacket);
	return 0;
}

int CMIMDlg::SendText(CPacket SPacket)
{
	int i, iSent;
	int ID = SPacket.m_iPos;
	CString strTemp, strName, strPass;

	if(m_bDebug)
	{
		m_strTemp.Format("SendText(); Code = %i   Size = %i   iPos=%i", SPacket.m_iCode, SPacket.m_iLength, SPacket.m_iPos);
		m_Console.DisplayMessage(m_strTemp,YELLOW);
	}

	//Servers and clients alike can request info or MSL (but server MUST be diconnected when doing so)
	if((m_bServerInfoRequest) && (SPacket.m_iCode == REG) || (m_bPublicInfoRequest && SPacket.m_iCode == PSL) || (m_bMasterInfoRequest && SPacket.m_iCode == MSL))
	{//send if info request or MSL request
		iSent = m_pInfoReqSocket->SendPacket(SPacket);//MUST USE THIS SOCKET TO SEND THOUGH!
		return 0;
	}

	//SERVER SEND
	if(m_iType == SERVER)
	{
		for(i = 0;i < m_Users.m_iMax;i++)
		{
			if(ID != -1)
			{
				i=ID;//set i to the only client being sent to
				//m_strTemp.Format(">> Sending only to user %i", ID+1);
				//m_wndShow.ShowMessage(m_strTemp);
			}

			//If at least level 2 connection
			//WARNING: Adding a command to level 2 requires that you add it to the end check of level 1 "if" statement
			if(m_Users.m_iConnected[i] > 0)//level 2 and higher commands (client isnt fully connected if NOT at level 1 access)
			{
				if((SPacket.m_iCode == MBOX) || (SPacket.m_iCode == DLNEW) || (SPacket.m_iCode == DLNEWO) || (SPacket.m_iCode == OK) || (SPacket.m_iCode == PSL))
				{
					iSent = m_pClientSocket[i]->SendPacket(SPacket);
					if(SPacket.m_iCode == OK)//this is an explicit sending of OK request. Some send codes imply that an OK should be returned by the client. See the Server SendText() for details.
						m_iUserOK[i]++;//increment the user OK value. decremented when OK is recieved back
				}
				else if(SPacket.m_iCode == REG)//Server is sending it's info and a request to the client
				{
					//This is where all the server info is sent to a client. If the client is connecting, then it will
					//send back a code REG with password if applicable, otherwise it will close the connection.
					CRequest Request;
					if((m_strPassword != "") || m_bBouncer)//Password required
						strcpy(Request.m_strPassword, "p");
					else
						Request.m_strPassword[0] = 0;
					strcpy(Request.m_strName, m_strRoomName);//Add chatroom name to class
					Request.m_dVer = m_dVer;
					Request.m_bFirst = TRUE;
					Request.m_iID = ID;//let the client know what ID it will be
					Request.m_iUsers = m_Users.m_iTotalUsers;//number of users in server
					Request.m_iMax = m_Users.m_iMax;//total max number allowed

					SPacket.m_iLength = sizeof(CRequest);
					if(m_bDebug)
					{
						m_strTemp.Format("Server SendText(REG): sizeof(CRequest)=%i  RPacket.m_iLength=%i", sizeof(CRequest), SPacket.m_iLength);
						m_Console.DisplayMessage(m_strTemp, YELLOW);
					}

					CMySocket::StrCopy(SPacket.pBuf, (char*)&Request, SPacket.m_iLength);

					iSent = m_pClientSocket[ID]->SendPacket(SPacket);

					SetStatusText("A client has connected to the server.",0);

					//DEBUG:
					//strTemp.Format("Client info requested. #%i\nsize: %i", ID, sizeof(CRequest));
					//m_wndShow.ShowMessage(strTemp);
				}
				else if((SPacket.m_iCode == RIP) || (SPacket.m_iCode == SIP))//Request IP address from admin
				{
					//Send ShareInfo class to other side
					//m_pListenSocket->StrCopy(SPacket.pBuf, (char*)&m_ShareInfo, sizeof(CShareInfo));
					SPacket.m_iLength = sizeof(CShareInfo);
					iSent = m_pClientSocket[i]->SendPacket(SPacket);
				}
				/*else if(SPacket.m_iCode == INFO)
				{
					iSent = m_pClientSocket[i]->SendPacket(SPacket);
				}*/
			}
			//Level 1 access
			if(m_Users.m_iConnected[i] == 1)
			{
				switch(SPacket.m_iCode)
				{
				case MESG:
				case ME:
				case WHISPER:
					m_iUserOK[i]++;//Implicit request that users repsond with an OK for these 3 CASES.
					//m_strTemp.Format("Requested Implicit OK: m_iUserOK[%i]=%i", i, m_iUserOK[i]);
					//m_Console.DisplayMessage(m_strTemp,YELLOW);
				//NO BREAK!
				case RNAME:
				case USER:
				case VOTE:
					if((m_bTrayed)&&(m_bTrayNotify))
					{
						m_bTrayNotify = FALSE;//has been notified once now
						SetTrayIcon(m_hMessAIcon,2);//modify admin icon
						PlayRecSound();
					}
				//NO BREAK!
				case STYPE:
				case ETYPE:
				case PING:
					
					iSent = m_pClientSocket[i]->SendPacket(SPacket);
					//m_strTemp.Format("DEBUG: Server sent message with code> %i   size = %i", SPacket.m_iCode, SPacket.m_iLength);
					//m_Console.DisplayMessage(m_strTemp,YELLOW);

				break;
				case ULIST://send User list
					//server ONLY
					//MessageBox("Sending User List...");
					///////////////////////////////////////////
					//output text to a file
					SPacket.m_iLength = sizeof(CUsers);
					CMySocket::StrCopy(SPacket.pBuf, (char*)&m_Users, SPacket.m_iLength);
					
					//strTemp.Format("sizeof(CUser) = %i", sizeof(CUsers));
					//m_wndShow.ShowMessage(strTemp);
					iSent = m_pClientSocket[i]->SendPacket(SPacket);
				break;
				case PROF://server wants to view a users profile
				
					//m_SendProfile.m_iReq = -1;//from server
					//m_SendProfile.m_iRep = ID;//requst
					//else just send the Class, only the req and rep values matter
					//those 2 values are set in OnViewProfile()
					
					if(m_SendProfile.m_iRep == -1)//if admin must reply to a request
					{
						//overwrite contents

						//m_SendProfile = m_SavedProfile;
						m_SendProfile.m_iAge = m_MyProfile.m_iAge;
						m_SendProfile.m_iGender = m_MyProfile.m_iGender;
						//m_SendProfile.m_Bitmap = m_MyProfile.m_Bitmap;
						strcpy(m_SendProfile.m_strEmail, m_strEmail);
						strcpy(m_SendProfile.m_strLive, m_strLive);
						strcpy(m_SendProfile.m_strName, m_strName);
						strcpy(m_SendProfile.m_strPets, m_strPets);
						strcpy(m_SendProfile.m_strQuote, m_strQuote);
					}
					//Else just send the Class AS IS, only the req and rep values matter
					//those 2 values are set in OnViewProfile()
					SPacket.m_iLength = sizeof(CProfile);
					CMySocket::StrCopy(SPacket.pBuf, (char*)&m_SendProfile, SPacket.m_iLength);
					
					//MessageBox("Sending profile data to server...");
					iSent = m_pClientSocket[ID]->SendPacket(SPacket);	
					//MessageBox("Sending profile data to server...");
				break;
				default:
					if((SPacket.m_iCode != MBOX) && (SPacket.m_iCode != REG) && (SPacket.m_iCode != SIP) && (SPacket.m_iCode != RIP) && (SPacket.m_iCode != OK) && (SPacket.m_iCode != PSL))
					{
						CString strTemp;
						strTemp.Format("<!> Can't send, invalid command:\n-Level 1 User.\nCODE: %i\n%s", SPacket.m_iCode, SPacket.pBuf);
						m_wndShow.ShowMessage(strTemp,RED);
					}
				break;
				}
				//close connection to that client only, if server
				if(iSent == SOCKET_ERROR)
					OnCloseCon(i);
			}
			if(ID != -1)
				break;//stop after sending to only 1 client, otherwise loop again and increment i
		}
	}
	//CLIENT SEND
	else
	{
		switch(SPacket.m_iCode)
		{
			case MESG:
			case ME:
			case WHISPER:
			case MOD:
			case DLNEW:
			case RIP:
			case USER:
			case STYPE:
			case ETYPE:
			case PING:
			case VOTE:
			case ROLL:
				if((m_bTrayed)&&(m_bTrayNotify))
					{
						m_bTrayNotify = FALSE;//has been notified once now
						SetTrayIcon(m_hMessIcon,2);//set to "message client" icon
						PlayRecSound();
					}
			//NO BREAK!
			case AWAY:
			case OK:
				//MessageBox("About to SendText().");
				iSent = m_pClientSocket[0]->SendPacket(SPacket);
				//m_strStatus.Format("DEBUG: Client sent message> %s", strText);
				break;
			case PROF:

				if(m_SendProfile.m_iRep == m_pClientSocket[0]->m_iClientID)//if you must reply to a request
				{
					//overwrite contents

					//m_SendProfile = m_SavedProfile;
					m_SendProfile.m_iAge = m_MyProfile.m_iAge;
					m_SendProfile.m_iGender = m_MyProfile.m_iGender;
					//m_SendProfile.m_Bitmap = m_MyProfile.m_Bitmap;
					strcpy(m_SendProfile.m_strEmail, m_strEmail);
					strcpy(m_SendProfile.m_strLive, m_strLive);
					strcpy(m_SendProfile.m_strName, m_strName);
					strcpy(m_SendProfile.m_strPets, m_strPets);
					strcpy(m_SendProfile.m_strQuote, m_strQuote);
				}
				SPacket.m_iLength = sizeof(CProfile);
				m_pClientSocket[0]->StrCopy(SPacket.pBuf, (char*)&m_SendProfile, SPacket.m_iLength);
				
				//Else just send the Class, only the req and rep values matter
				//those 2 values are set in OnViewProfile()
				//MessageBox("Sending profile data to server...");
				iSent = m_pClientSocket[0]->SendPacket(SPacket);
				break;

			case REG:
				m_pClientSocket[0]->SendPacket(SPacket);
				m_Console.m_bConnected = TRUE;//Server may still reject you, but client IS connected at this time with level 2 access
				//MessageBox("Request Info Sent.");
				break;
			/*case INFO://Client is sending a simple request, but must use the m_pInfoReqSocket Socket!!
				if(!m_bServerInfoRequest)//socket may not be defined and would crash program
					m_Console.DisplayMessage("INFO Request can't be sent when not info-request mode",RED);
				else
					m_pInfoReqSocket->SendPacket(SPacket);
				break;*/
			default:
				m_strTemp.Format("<!> Can't send, invalid command: %i",SPacket.m_iCode);
				m_Console.DisplayMessage(m_strTemp,RED);
				break;
		}
	}
	return 0;
}

void CMIMDlg::SendMess(int ID, CString strText, int iCode, BOOL bShow)
{
	//cCode works with MESG and ME
	//Sends text message in your font/color to the specified user
	//bShow = FALSE doesnt display the text for the user sending the text
	//CPacket SPacket;
	CPkg Text;
	AssemblePkg(Text,strText);//set to your font

	SendMess(ID, Text, iCode, bShow);
	/*SPacket.m_iPos = ID;
	SPacket.m_iCode = iCode;
	SPacket.m_iLength = strlen(Text.strText)+56;

	//DEBUG:
	//m_strTemp.Format("sizeof(CPkg) = %i", sizeof(CPkg));
	//m_wndShow.ShowMessage(m_strTemp,YELLOW);

	CMySocket::StrCopy(SPacket.pBuf, (char*)&Text, SPacket.m_iLength);
	SendText(SPacket);//send it
	if(bShow)
		m_wndShow.ShowMessage(Text);*/

	//DEBUG: finds size of CPkg
	//m_strTemp.Format("SIZEOF CPkg: %i",sizeof(CPkg));
	//m_wndShow.ShowMessage(m_strTemp);
}

void CMIMDlg::SendMess(int ID, CPkg Text, int iCode, BOOL bShow)
{
	//cCode works with MESG and ME
	//Sends text message with CPkg font and format to the specified user
	//bShow = FALSE doesnt display the text for the user sending the text
	CPacket SPacket;

	SPacket.m_iPos = ID;
	SPacket.m_iCode = iCode;
	SPacket.m_iLength = strlen(Text.strText)+56;//extra buffer for vars in sizeof class

	//DEBUG:
	//m_strTemp.Format("sizeof(CPkg) = %i     -- SPacket.m_iLength=%i", sizeof(CPkg),SPacket.m_iLength);
	//m_wndShow.ShowMessage(m_strTemp,YELLOW);

	CMySocket::StrCopy(SPacket.pBuf, (char*)&Text, SPacket.m_iLength);
	SendText(SPacket);//send it
	if(bShow)
		m_wndShow.ShowMessage(Text);
}

int CMIMDlg::PlayWav(CString strFile)
{
	//Play the selected wav file when a message is received
	strFile.MakeUpper();
	if(strFile.Right(4) == ".WAV")//Wave
		return sndPlaySound(strFile, SND_ASYNC);//returns false if invalid wav
	return 0;
}

int CMIMDlg::PlayRecSound()
{
	//Play the selected wav file when a message is received
	if(m_strSoundExt == ".wav")//Wave
	{
		sndPlaySound(m_strSoundDir, SND_ASYNC|SND_NOSTOP);//returns false if invalid wav, or sound is already playing
			//MessageBox("This file is not a valid WAV file.");
	}
	else if(m_strSoundExt == ".mid")//midi
	{
		UINT uiMIDPlayerID;
		MCI_OPEN_PARMS mciOpenParms;
		mciOpenParms.lpstrDeviceType = "sequencer";
		mciOpenParms.lpstrElementName = m_strSoundDir;
		if(mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOpenParms)==0)
		{
			uiMIDPlayerID = mciOpenParms.wDeviceID;
			//Play
			if(mciSendCommand(uiMIDPlayerID, MCI_PLAY, 0, (DWORD)&mciOpenParms) != 0)
				mciSendCommand(uiMIDPlayerID, MCI_STOP, 0, NULL);
			//uiMIDPlayerID = 0;
		}
	}
	else
		m_wndShow.ShowMessage(" <!> Invalid Sound File",RED);
	return 0;
}

void CMIMDlg::Prev()
{
	//override private member function
	OnPrev();
}

void CMIMDlg::OnPrev() 
{
	//Cycle through previously entered commands
	m_wndInput.SetSel(0, m_wndInput.GetTextLength());
	m_wndInput.ReplaceSel(m_strPrev.GetAt(m_iPrev));
	m_iPrev++; 
	if(m_iPrev > m_iTPrev)
		m_iPrev = 0;
}

int CMIMDlg::GetID(CString strName)
{
	char let;
	int i, id;
	//Input a users name as text and his ID number will be returned (0-based)
	//from inside the parenthesis. (##)Name
	//rturns -1 for ADMIN
	int top = strName.GetLength();
	for(i = 0;i < top;i++)
	{
		if(strName.GetAt(i) == '(')
		{
			i++;
			break;
		}
	}
	if(i >= top)
		return -3;//not a valid user name

	if(strName[i] == 'A')//ADMIN
		return -1;

	let = strName[i];//Get first digit
	id = let - 48;//Make into int.
	if(strName[i+1] != ')')//More than 1 digit number
	{
		id *= 10;//mult first digit by 10
		let = strName[i+1] - 48;//make second char into int.
		id += let;//add to first digit
	}
	//m_strTemp.Format("Returning: %i", id-1);
	//MessageBox(m_strTemp);
	return id-1;//C++ is always 0-based
}

int CMIMDlg::AcceptNew(int connect)
{
	//For Server Only
	//Updates controls and lists after a new user has fully joined.
	//UpdateData(TRUE);

	//Update values for users list
	m_Users.m_iTotalUsers++;
	m_Users.m_iConnected[connect] = 1;//L1 access
	m_iUserOK[connect] = 0;//reset OK status
	//MessageBox("L1 access given");
	//MessageBox("Opening Text Sent");
	ShowSend(TRUE);//Update controls
	//UpdateData(FALSE);
	//UpdateList() should happen after this
	return 0;
}

int CMIMDlg::OnAccepted()
{
	//Client Only
	//Server has completely accepted client
	//Called from the OnReceive(ULIST); function
	//Last step in connection process, client has level 2 access

	m_Console.m_bAccepted = TRUE;//accepted connection
	UpdateList();		//Add other users to list
	m_strTemp.Format("Welcome to %s", m_strConRoomName);
	m_wndShow.ShowMessage(m_strTemp);
	SetStatusText("Online", 1);
	ShowSend(TRUE);		//Client can finally be allowed to send messages
	return 0;
}

void CMIMDlg::OnDblclkUserlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnViewprofile();	
	*pResult = 0;
}

void CMIMDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu m_lMenu;
	CPoint m_pPoint;
	m_pPoint = point;

	CRect rRoom;
	GetClientRect(&rRoom);
	ClientToScreen(&rRoom);//convert the corners of window to screen coordinates
	rRoom.right = rRoom.left+rRoom.Width();
	rRoom.bottom = rRoom.top+rRoom.Height()+39;

	//CString strTemp;
	//strTemp.Format("Mouse(%i,%i)\nWINDOW: %i,%i,%i,%i",m_pPoint.x,m_pPoint.y,rRoom.left,rRoom.right,rRoom.top,rRoom.bottom);
	//m_Console.DisplayMessage(strTemp);

	//User List menu, disable when in simple-mode
	if((m_pPoint.y >= rRoom.top+62) && (m_pPoint.y <= rRoom.bottom-167) && (!m_bSimple))
	{
		if((m_pPoint.x >= rRoom.right-158) && (m_pPoint.x <= rRoom.right-5))
		{
			m_lMenu.LoadMenu(IDR_USERMENU);
			m_lMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN + TPM_LEFTBUTTON, m_pPoint.x, m_pPoint.y, this, NULL);
		}
	}
	if((m_pPoint.x >= rRoom.left+10) && (m_pPoint.x <= rRoom.right-167+m_bSimple*160))
	{
		//Top window
		if((m_pPoint.y >= rRoom.top+60-m_bSimple*41) && (m_pPoint.y <= rRoom.bottom-170))
		{
			m_lMenu.LoadMenu(IDR_SHOWMENU);
			m_lMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN + TPM_LEFTBUTTON, m_pPoint.x, m_pPoint.y, this, NULL);
		}
		//Bottom window
		else if((m_pPoint.y >= rRoom.bottom-139) && (m_pPoint.y <= rRoom.bottom-86))
		{
			m_lMenu.LoadMenu(IDR_INPUTMENU);
			m_lMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN + TPM_LEFTBUTTON, m_pPoint.x, m_pPoint.y, this, NULL);
		}
	}
}

void CMIMDlg::OnKick()
{
	//Server pressed "kick" button.
	//selected user will be removed from server.

	int i = 0;
	if(m_iType != SERVER)
	{
		m_Console.DisplayMessage("<!> KICK Command valid for admin only.",RED);
		return;
	}

	UpdateData(TRUE);
	int here = m_ctlUserList.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(here <= 0)
	{
		MessageBox("Select a CLIENT first.");
		return;
	}
	int ID = GetID(m_ctlUserList.GetItemText(here,0));

	if(ID < 0)
		m_wndShow.ShowMessage("<!> Invalid number or range.",RED);
	else
		m_Console.Kick(ID);
	return;
}

void CMIMDlg::OnUserBan() 
{
	int i = 0;
	if(m_iType != SERVER)
	{
		m_Console.DisplayMessage("<!> BAN Command valid for admin only.",RED);
		return;
	}

	UpdateData(TRUE);
	int here = m_ctlUserList.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(here <= 0)
	{
		MessageBox("Select a CLIENT first.");
		return;
	}
	int ID = GetID(m_ctlUserList.GetItemText(here,0));

	if(ID < 0)
		m_wndShow.ShowMessage("<!> Invalid number or range.",RED);
	else
		m_Console.Ban(ID);
	return;
}

void CMIMDlg::OnMbox()
{
	if(m_iType != SERVER)
	{
		m_Console.DisplayMessage("<!> MBOX Command valid for admin only.",RED);
		return;
	}

	UpdateData(TRUE);
	int here = m_ctlUserList.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);
	if(here <= 0)
	{
		MessageBox("Select a CLIENT first.");
		return;
	}
	int ID = GetID(m_ctlUserList.GetItemText(here,0));

	if(ID < 0)
		m_wndShow.ShowMessage("<!> Invalid number or range.",RED);
	else
	{
		if(m_MBoxDlg.DoModal() == IDOK)
		{
			m_wndShow.ShowMessage(">> Sent message box:");
			SendMess(ID, m_MBoxDlg.m_strMess, MBOX, TRUE);
		}
	}
	return;
}

void CMIMDlg::ClearFileList()
{
	m_strFileDirs.RemoveAll();
	m_strFileNames.RemoveAll();
	m_iTotal = 0;
}

void CMIMDlg::OnSendfile()
{
	if((m_pClientSocket[0] == NULL) && (m_iType == CLIENT))
		return;
	//if((m_pListenSocket == NULL) && (m_iType == SERVER))
	//	return;

	int sel = m_ctlUserList.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);

	if(sel < 0)//not admin or not selected
	{
		MessageBox("Select a person to send a file first.", "Send File", MB_OK | MB_ICONEXCLAMATION);	
		return;
	}

	int ID = GetID(m_ctlUserList.GetItemText(sel,0));

	if((sel > 0)&&(m_iType == SERVER))//server send to client
	{
		if(ID < 0)
		{
			m_wndShow.ShowMessage("<!> Invalid number or range.",RED);
			return;
		}
		if(BrowseFile() <= 0)
			return;
	}
	else if(m_iType == CLIENT)//cant send to self
	{
		if(ID != m_pClientSocket[0]->m_iClientID)
		{
			if(ID == -1)//send to admin
			{
				if(BrowseFile() <= 0)
					return;
			}
			else//send to another client
			{
				if(BrowseFile() <= 0)
					return;
			}
		}
	}
	else
	{
		MessageBox("Cant send file(s) to that user.", "Sending Files", MB_OK | MB_ICONEXCLAMATION);	
		return;
	}
	SendFiles(ID);//send to the user specified
}

void CMIMDlg::SendFiles(int ID)
{
	//ID = ID of user, 0 based. -1 when sending to admin

	//MessageBox("FileShare is currently under construction.", "Incomplete", MB_ICONEXCLAMATION);
	//GetDlgItem(IDC_SENDFILE)->EnableWindow(FALSE);

	//SENDER: OPEN DLG AND WAIT
	CShareInfo ShareInfo;
	if(!m_Console.m_bConnected)
	{
		m_wndShow.ShowMessage("<!> Can't send files, not connected.",RED);
		return;
	}
	else if(m_strFileNames.GetSize() <= 0)
	{
		m_wndShow.ShowMessage("<!> No files stored in the file list. See the ADDFILE command or the readme.",RED);
		return;
	}

	if((ID != -1) && (m_iType == CLIENT))//not sending to admin
	{
		if(ID == m_pClientSocket[0]->m_iClientID)
		{
			m_wndShow.ShowMessage("<!> Can't send files to yourself.",RED);
			return;
		}
		else if(m_Users.m_iConnected[ID] < 1)//send-to-user is not connected
		{
			m_wndShow.ShowMessage("<!> That user is not connected.",RED);
			return;
		}
	}
	
	CFile MIMFile;
	if(MIMFile.Open(m_strInstallDir+"\\FM.dat",CFile::modeCreate|CFile::modeWrite))
	{
		//Send this to a file for this computers FM.exe
		m_strTemp.Format("(%i) %s", ID+1, m_Users.m_strNames[ID]);
		strcpy(ShareInfo.m_strName, LPCTSTR(m_strTemp));	//other side's name
		ShareInfo.m_strIP[0] = 0;							//No IP
		strcpy(ShareInfo.m_strIP2, LPCTSTR(m_strIP));//save the admins IP in case he will be server with FM
		ShareInfo.m_bSender = TRUE;
		ShareInfo.m_bRec = FALSE;
		ShareInfo.m_bListenAble = m_bListenAble;
		ShareInfo.m_bYouConnect = !m_bListenAble;
		ShareInfo.m_iPort = FindEmptyShare();
		if(ShareInfo.m_iPort == -1)
		{
			MessageBox("There are no available ports on your computer to create a connection within the specified range (Starting Port + 20)");
			MIMFile.Close();
			return;
		}
		ShareInfo.m_iTotal = m_iTotal;

		//MessageBox("Initializing filesharing...");
		CArchive* ar = new CArchive(&MIMFile,CArchive::store);

		ar->Write((char*)&ShareInfo, sizeof(CShareInfo));

		for(int i = 0;i < m_iTotal;i++)
		{
			*ar << m_strFileNames.GetAt(i);
			*ar << m_strFileDirs.GetAt(i);
		}
		ar->Flush();

		delete ar;
		MIMFile.Close();
	
		CPacket SPacket;
		if(WinExec(m_strInstallDir+"\\FM.exe",TRUE) < 32)
			MessageBox("Could not run FM.exe, make sure that FM.exe is\nincluded in the same folder as MIM.exe and try again.");

		//Edit for the other comps FM.dat file
		ShareInfo.m_bListenAble = FALSE;//default
		ShareInfo.m_bYouConnect = m_bListenAble;

		if(m_iType == SERVER)//server send to client
		{
			//tell client to use ADMINS IP (-2 code)
			ShareInfo.m_iReq = -2;
			SPacket.m_iCode = SIP;
			SPacket.m_iPos = ID;
			strcpy(ShareInfo.m_strName, m_strAdminName);	//your name
			//SendMess(ID,">> Incoming file(s) from ADMIN.");
		}
		else if(m_iType == CLIENT)
		{
			if(ID != m_pClientSocket[0]->m_iClientID)//cant send to self
			{
				m_strTemp.Format("(%i) %s", m_pClientSocket[0]->m_iClientID+1, m_strScreenName);
				strcpy(ShareInfo.m_strName, LPCTSTR(m_strTemp));	//Your name
				/*if(sel == 0)//send to admin
				{
					ShareInfo.m_iReq = -1;
					SPacket.m_iCode = RIP;
					SPacket.m_iPos = 0;
				}
				else//send to another client*/
				{
					ShareInfo.m_iReq = ID;//Request IP from server, to set up a P2P
					SPacket.m_iCode = RIP;
					SPacket.m_iPos = 0;
				}
			}
		}
		SPacket.m_iLength = sizeof(CShareInfo);
		CMySocket::StrCopy(SPacket.pBuf, (char*)&ShareInfo, SPacket.m_iLength);
		SendText(SPacket);
		//MessageBox("OK1");//Server doesnt make it this far on 2nd attempt
	}
	else
		MessageBox("Could not initialize filesharing. Error editing file FM.dat.\nIt may be in use by another instance of FM.exe. Try again.", "MIM.exe");
}

int CMIMDlg::BrowseFile()
{
	//Opens Browse Dlg, and returns number of files added to m_strFileDirs and m_strFileNames
	CString strFile, strFilter;
	int z = 0;
	strFilter = "All Files (*.*)|*.*||";

	CFileDialog dLoad(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, strFilter);

	ClearFileList();

	if(dLoad.DoModal() == IDOK)
	{
		BOOL bOK = FALSE;
		POSITION here;
		here = dLoad.GetStartPosition();
		if(here == NULL)
		{
			MessageBox("here == NULL");
			return 0;
		}
		while(z < 30)
		{
			//Add files until end or 30
			strFile = dLoad.GetNextPathName(here);

			if(AddFile(strFile) > -1)
				bOK = TRUE;
			z++;
			if(here == NULL)//end found
				break;
		}
		//GetDlgItem(IDC_SENDFILE)->EnableWindow(FALSE);
	}
	else//cancelled
	{
		if(CommDlgExtendedError() != 0)
		{
			m_strTemp.Format("The combined length of the file names you have selected is too long.\nTry again by compressing the files into one (ZIP or RAR) or use the Drag-and-Drop method.");
			MessageBox(m_strTemp);			
		}
		return -1;
	}

	SetCurrentDirectory(m_strInstallDir);
	
	//Display in status bar
	m_strTemp.Format("Total files to upload: %i", m_iTotal);
	SetStatusText(m_strTemp,0);

	return z;
}

int CMIMDlg::AddFile(CString strLoad)
{
	//This function takes a file path (dir+name) and splits it into dir and name.
	//The name and dir are put into the Listctl in seperate columbs, up to 40 files.
	//The file size is also determined and placed in the list.

	//CString strSize;
	//int iSize;
	//float dSize;

/*
	ifstream inFile(LPCTSTR(strLoad), ios::in | ios::binary);
	if(!inFile)
	{
		MessageBox("Could not load file to check size.", "Loading Error", MB_OK | MB_ICONSTOP);
		inFile.close();
		return -1;
	}

	//GetSize of file
	iSize = 0;
	inFile.seekg(0, ios::end);
	iSize = inFile.tellg();
	inFile.close();

	if(iSize > 1024)
	{
		dSize = (float)iSize / 1024;
		if(dSize > 1024)
		{
			dSize /= 1024;
			strSize.Format("%.1fMB", dSize);
		}
		else
			strSize.Format("%.1fKB", dSize);
	}
	else
		strSize.Format("%i bytes", iSize);
*/
	//Add to Arrays

	/*if(m_FileShareDlg.m_strFileDirs.GetSize() >= 40)
	{
		MessageBox("List is full, only allowed to have 40 files.");
		return -1;
	}*/
	
	//split into dir and name
	CString strName, strDir;
	int i, iLen;

	iLen = strLoad.GetLength();
	//if(iLen >= 400)//obsolete with CString
	//	MessageBox("File name length too long. 400 char max.");
	for(i = iLen-1;i >= 0;i--)//start at end, move to beggining
	{
		if(strLoad.GetAt(i) == '\\')
		{
			strDir = strLoad.Left(i+1);
			strName = strLoad.Right(iLen-i-1);
			i = -10;
			break;
		}
	}
	if((i != -10) || (strLoad.GetAt(0) == '\\'))
	{
		strDir = m_strInstallDir+strDir;
		if(strLoad.GetAt(0) != '\\')
		{
			strDir+="\\";
			strName = strLoad;
		}
		/*MessageBox(strLoad);
		m_strTemp.Format("<!> Could not add file \"%s   %s\" to list.", strDir, strName);
		m_wndShow.ShowMessage(m_strTemp);
		return -1;*/
	}
	m_strTemp.Format("Added file \"%s %s\" to list.", strDir, strName);
	m_wndShow.ShowMessage(m_strTemp,GREEN);
	m_strFileDirs.Add(strDir);
	m_strFileNames.Add(strName);

	m_iTotal++;
	return 0;//successful
}

void CMIMDlg::OnIgnore() 
{
	//CLIENT ONLY!
	//Ignores/un-ignores the selected user, checkmark next to name
	//indicates ignoring status.

	if(m_iType != CLIENT)
	{
		m_wndShow.ShowMessage("<!> IGNORE Command valid for clients only.",RED);
		return;
	}

	CString strMessage, strTog;
	int ID;
	//m_Console.DisplayMessage("IGNORE!");
	UpdateData(TRUE);

	int sel = m_ctlUserList.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);

	if(sel > 0)//not admin or not selected
	{
		strTog = "";

		ID = GetID(m_ctlUserList.GetItemText(sel,0));
		if(ID < 0)
		{
			m_wndShow.ShowMessage("<!> Invalid number or range.",RED);
			return;
		}
		if(m_bIgnore[ID])//Un-ignore
		{
			m_bIgnore[ID] = FALSE;
			strTog = "OFF";
			if(m_pClientSocket[0]->m_iClientID == ID)
				SetListIcon(5, sel);//your ICON
			else
				SetListIcon(0, sel);//USER ICON
		}
		if(strTog == "")//if user hasn't previously been ignored
		{
			m_bIgnore[ID] = TRUE;
			strTog = "ON";
			SetListIcon(1, sel);
		}
		//Display toggle for client
		strMessage.Format(" >>Ignore (%i) = %s", ID+1, strTog);
		m_wndShow.ShowMessage(strMessage,GREEN);//Just add for client only
		m_ctlUserList.SetSelectionMark(sel);
	}
	else
		MessageBox("Select a CLIENT to ignore first.", "Ignoring User", MB_OK | MB_ICONEXCLAMATION);	
}

int CMIMDlg::SetListIcon(int iImage, int iSel)
{
	//input 0-based location in list
	//changes the icon at that location

	CString strTemp;
	if((iSel<0) || (iSel>=ID_MAX))
		return -1;
	
	/*LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT|LVIF_IMAGE;
	lvItem.iItem = iSel;
	lvItem.iSubItem  = 0;
	lvItem.iImage = 0;
	lvItem.pszText = "__";	//even you don't need this member,still declare it!
	m_ctlUserList.GetItem(&lvItem);
	//Changes
	lvItem.iImage = iImage;
	m_ctlUserList.SetItemText(iSel, 0, strTemp.GetBuffer(strTemp.GetLength()));
	m_ctlUserList.SetItem(&lvItem);*/
	//temp save name, delete item then replace it with different pic and temp name
	strTemp = m_ctlUserList.GetItemText(iSel,0);
	m_ctlUserList.DeleteItem(iSel);
	m_Users.m_iImage[GetID(strTemp)] = iImage;//change the icon
	m_ctlUserList.InsertString(iSel,strTemp,iImage);
	return iSel;
}

void CMIMDlg::OnAway()
{
	OnAwayDef();
}

void CMIMDlg::OnAwayDef(BOOL bForce)
{
	//Client only!!
	//Send message to server saying that you are NOT available
	//wait for Updatelist() call from server to auto change icon
	if(m_bAway || (m_iType == SERVER))
		return;//already away, or you are server
	if(m_pClientSocket[0] == NULL)
	{
		m_bAway = FALSE;
		return;
	}
	if((m_Users.m_iImage[m_pClientSocket[0]->m_iClientID] != 4)&&(m_iType == CLIENT))
	{
		if(bForce)
			m_bForceAway = TRUE;
		m_bAway = TRUE;
		SendText(0,AWAY,"");
		SetTrayIcon(m_MiniIcon,2);//Keep current icon, just alter text.
		m_wndShow.ShowMessage(">> Status: Away "+GetTimeStamp(),YELLOW);
	}
}

void CMIMDlg::OnAvailable() 
{
	//CLIENT ONLY!!
	//Send message to server saying that you are now available
	//wait for Updatelist() call from server to auto change icon
	if(!m_bAway)
		return;//already available!
	m_bAway = FALSE;
	m_bForceAway = FALSE;
	if(m_iType == SERVER)
		return;

	if(m_pClientSocket[0] == NULL)
		return;
	if((m_Users.m_iImage[m_pClientSocket[0]->m_iClientID] == 4)&&(m_iType == CLIENT))
	{
		SendText(0,AWAY,"");
		m_wndShow.ShowMessage(">> Status: Available "+GetTimeStamp(),GREEN);
		SetTrayIcon(m_MiniIcon,2);//Keep current icon, just alter text.
	}
}

int CMIMDlg::GetListPos(int ID)
{
	//input 1-based ID of user.
	//returns the 0-based position of the user in the list
	if((ID < 1) || (ID > ID_MAX))
		return -1;

	int sel;
	CString Num;

	Num.Format("%i",ID);
	{
	for(int i=0;i < ID_MAX;i++)
		if(m_ctlUserList.GetItemText(i,0) == Num)//found position
		{
			sel = i;
			break;
		}
	}
		
	return sel;
}

CString CMIMDlg::GetIP(int ID)
{
	//Server ONLY: returns IP of connected client at ID
	CString strIP;
	if(m_pClientSocket[ID] != NULL)
	{
		UINT uiPort = m_iListenPort;
		m_pClientSocket[ID]->GetPeerName(strIP, uiPort);//get Clients IP address

		return strIP;
	}
	else
		return "<User does not exist>";
}

void CMIMDlg::OnMinicon() 
{
	//Hides the main window and adds a tray icon
	OnSize(SHOWICON,0,0);
}

void CMIMDlg::OnIconRestore() 
{
	//Restores the main window and removes tray icon
	//IsWindowVisible
	ShowWindow(SW_RESTORE);
	OnSize(SIZE_RESTORED,0,0);
}

void CMIMDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if(m_iType == CLIENT)
		m_MiniIcon = m_hIcon;
	else
		m_MiniIcon = m_hAdminIcon;//Icon for tray

	/*NOTIFYICONDATA nid = {sizeof (nid), GetSafeHwnd (), 100, NIF_ICON|NIF_MESSAGE|NIF_TIP, WM_ICONNOTIFY, m_MiniIcon, "H"};
	CString niTip;
	niTip.LoadString ('H');
	strcpy (nid.szTip, niTip);*/
	static BOOL bFirst = TRUE;

	switch (nType)
	{
	case SHOWICON://add icon to tray
		m_bTrayed = TRUE;
		ShowWindow(SW_HIDE);
		//Shell_NotifyIcon (NIM_ADD, &nid);
		SetTrayIcon(m_MiniIcon,1);//add icon
		m_bTrayNotify = TRUE;
		//Enters value into registry!?
		//AfxGetApp ()->WriteProfileInt (_T ("StartUp"), _T ("StartOnTaskBar"), TRUE);
		Reposition();
		break;

	case SIZE_RESTORED://remove icon from tray
		SetTrayIcon(m_MiniIcon,3);//delete icon
		//Shell_NotifyIcon (NIM_DELETE, &nid);
		m_bTrayed = FALSE;
		m_bTrayNotify = FALSE;
		//Enters value into registry!?
		//AfxGetApp ()->WriteProfileInt (_T ("StartUp"), _T ("StartOnTaskBar"), FALSE);
		if(bFirst && GetActiveWindow () != NULL)
		{
			Reposition();
			//m_Console.DisplayMessage("m_bTrayed = FALSE");
		}
		break;

	case SIZE_MINIMIZED://do nothing
		break;
	case SIZE_MAXIMIZED://do nothing
		Reposition();
		break;
	}
	//bFirst = TRUE;
}

LRESULT CMIMDlg::OnIconNotify(WPARAM wParam, LPARAM lParam)
{
	//static BOOL rButton = FALSE;
	static CMenu menu;
	//Message Called for actions done to tray icon
	if (wParam == 100)
	{
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK:
			PostMessage (WM_COMMAND, ID_ICON_RESTORE);
			break;
		case WM_RBUTTONDOWN:
			{
				//if(rButton)
				//	break;
				menu.LoadMenu (IDR_ICONMENU);
				CPoint pt;
				::GetCursorPos (&pt);
				menu.GetSubMenu(0)->TrackPopupMenu (TPM_RIGHTALIGN|TPM_RIGHTBUTTON, pt.x, pt.y, this);
				//rButton = TRUE;
			}
			break;
		case WM_LBUTTONDOWN:
			{
				menu.DestroyMenu();
				//rButton = FALSE;
			}
			break;
			//EnableMenuItem
		}
	}
	return 0;
}

void CMIMDlg::OnIconShutdown() 
{
	NOTIFYICONDATA nid = {sizeof (nid), GetSafeHwnd (), 100, NIF_ICON|NIF_MESSAGE|NIF_TIP, WM_ICONNOTIFY, m_hIcon};
	CString niTip;
	niTip.LoadString ('H');
	strcpy (nid.szTip, niTip);
	Shell_NotifyIcon (NIM_DELETE, &nid);
	OnIconRestore();
	OnFileExit();
}

int CMIMDlg::FindEmptyShare()
{
	//Finds the first empty port for listening and returns this value.

	//NOTE: although the listener uses this free port, the client uses a random
	//empty port on their computer to connect to the listeners port. The client and
	//server do not use the same port number to complete the connection.
	CMySocket *m_TestSocket;
	m_TestSocket = new CMySocket(this,0);

	//find the lowest port number to start searching at
	int iPort = m_iListenPort;
	if (iPort > m_iPort)
		iPort = m_iPort;

	for(int i = 1;i < ID_MAX;i++)//up to StartPort+20;
	{
		if(m_TestSocket->Create(iPort+i))
		{	
			m_TestSocket->Close();
			delete m_TestSocket;
			return iPort+i;//found empty port, returning it
		}
		else
		{
			m_TestSocket->Close();//close that slot and try the next one
		}
	}
	delete m_TestSocket;
	return -1;//didn't find any empty ports within star+20
}

void CMIMDlg::OnShowmenuCopy() 
{
	m_wndShow.Copy();	
}

void CMIMDlg::OnShowmenuFollowurl() 
{
	m_strTemp = m_wndShow.GetSelText();
	Browser(m_strTemp);
}

void CMIMDlg::OnMenuGoogle() 
{
	//context menu option
	OnGoogle(m_wndShow.GetSelText());
}

void CMIMDlg::OnGoogle(CString strURL) 
{
	m_strTemp = "http://www.google.com/search?hl=en&lr=&q=";
	m_strTemp += strURL;

	for(int i = 0;i < m_strTemp.GetLength();i++)
	{
		if(m_strTemp[i] == ' ')
			m_strTemp.SetAt(i,'+');
	}
	m_strTemp += "&btnG=Search";
	Browser(m_strTemp);
	//http://www.google.com/search?hl=en&q=what+if+I+type%3F&btnG=Google+Search	
	//http://www.google.com/search?hl=en&lr=&q=Dennis&btnG=Search
}

void CMIMDlg::OnMenuDictionary() 
{
	// TODO: Add your command handler code here
	OnDictionary(m_wndShow.GetSelText());
}

void CMIMDlg::OnDictionary(CString strWord)
{
	m_strTemp = "http://dictionary.reference.com/search?q=";
	m_strTemp += strWord;
	m_strTemp.Replace(" ","%20");
	Browser(m_strTemp);
	//http://dictionary.reference.com/search?q=what%20if
}

int CMIMDlg::Browser(CString strURL)
{
	//input URL string and default browser will open with that as the parameter.
	//ShellExecute(0, _T("open"), m_strInstallDir + "\\readme.htm", 0, 0, SW_SHOWNORMAL);//this works
	//ShellExecute(0, _T("open"), "http://www.google.com", 0, 0, SW_SHOWNORMAL);
	//WinExec("www.google.com", SW_SHOW);//wont work

	m_strTemp = m_strBrowser + " " + strURL;
	if(WinExec(m_strTemp, SW_SHOW) < 32)
	{
		m_strTemp = "C:\\Program Files\\Mozilla Firefox\\firefox.exe " + m_wndShow.GetSelText();
		//m_strTemp = m_wndShow.GetSelText();
		if(WinExec(m_strTemp, SW_SHOW) < 32)
		{
			m_strTemp = "C:\\Program Files\\Internet Explorer\\iexplore.exe " + m_wndShow.GetSelText();
			if(WinExec(m_strTemp, SW_SHOW) < 32)
			{
				MessageBox("Can't find any common internet browsers, set the\ndirectory of the browser you wish to use in Connection Settings window.");
				return -1;
			}
		}
	}
	return 0;
}

//INPUT CRichEditCtrl
void CMIMDlg::OnInputmenuUndo() 
{
	m_wndInput.Undo();	
}

void CMIMDlg::OnInputmenuCut() 
{
	m_wndInput.Cut();	
}

void CMIMDlg::OnInputmenuCopy() 
{
	m_wndInput.Copy();	
}

void CMIMDlg::OnInputmenuPaste() 
{
	m_wndInput.Paste();	
}

void CMIMDlg::OnInputmenuSelectall() 
{
	m_wndInput.SetSel(0,-1);
}

void CMIMDlg::InitImageLists()
{
	//Init the image lists for the user list. NOTE that the image list must be set from the CDialog parent class
	m_imgContactList.Create(40,
				 40,
				 ILC_COLOR16,
				 7,// number of initial images
				 1);//grow size

	ASSERT(m_imgContactList.m_hImageList);

	m_imgContactList.Add(AfxGetApp()->LoadIcon(IDI_ICON0));
	m_imgContactList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_imgContactList.Add(AfxGetApp()->LoadIcon(IDI_ICON2));
	m_imgContactList.Add(AfxGetApp()->LoadIcon(IDI_ICON3));
	m_imgContactList.Add(AfxGetApp()->LoadIcon(IDI_ICON4));
	m_imgContactList.Add(AfxGetApp()->LoadIcon(IDI_ICON5));
	//hTmp = AfxGetApp()->LoadIcon(IDI_ICON6);
	//m_imgContactList.Add(hTmp);
}

CString CMIMDlg::GetYourIP()
{
	CString strTmp;
	char szHostName[200];
	int iErr = gethostname(szHostName,sizeof(szHostName));
	if(iErr != 0)
	{
		iErr = WSAGetLastError();
		if(iErr == WSAEFAULT)
			strTmp = "WSAEFAULT";
		else
			strTmp.Format("Error: %i",iErr);
		MessageBox(strTmp);
		return strTmp;
	}
	LPHOSTENT pHost;
	pHost = gethostbyname(szHostName);

	struct in_addr* ptr = (struct in_addr*)pHost->h_addr_list[0];
	int a = ptr->S_un.S_un_b.s_b1;
	int b = ptr->S_un.S_un_b.s_b2;
	int c = ptr->S_un.S_un_b.s_b3;
	int d = ptr->S_un.S_un_b.s_b4;
	
	//strTmp = pHost->h_addr_list[0];
	strTmp.Format("%d.%d.%d.%d",a,b,c,d);
	return strTmp;
}

void CMIMDlg::OnMsgfilterRoom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//User pressed keys while output window was selected
	MSGFILTER *pMsgFilter = reinterpret_cast<MSGFILTER *>(pNMHDR);
	//m_strTemp.Format("msg = %i", pMsgFilter->msg);
	//MessageBox(m_strTemp);

	if(pMsgFilter->msg == WM_LBUTTONUP)//512, when a mouse is clicked
	{
		long s,e;
		m_wndShow.GetSel(s,e);
		//m_strTemp.Format("First\ns=%i\ne=%i",s,e);
		//MessageBox(m_strTemp);

		int max = m_wndShow.wHyperlinkS.GetSize();
		int i;
		for(i = 0; i < max;i++)
		{
			if((s > m_wndShow.wHyperlinkS.GetAt(i)) && (s < m_wndShow.wHyperlinkE.GetAt(i)))
			{
				long ls = LONG(m_wndShow.wHyperlinkS.GetAt(i));
				long le = LONG(m_wndShow.wHyperlinkE.GetAt(i));

				m_wndShow.SetSel(ls,le);
				m_strTemp = m_wndShow.GetSelText();
				Browser(m_strTemp);
				m_wndShow.SetSel(le+1,le+1);
				break;
			}
		}
	}
	else if(pMsgFilter->msg == WM_CHAR)//258, when a letter is pressed
	{
		switch(pMsgFilter->wParam)
		{
		case 1:	//Ctrl+A
			m_wndShow.SetSel(0,-1);
			break;
		case 4://Ctrl+G
			OnMenuDictionary();
			break;
		case 6://Ctrl+F
			Find();
			break;
		case 7://Ctrl+G
			OnMenuGoogle();
			break;
		case 21://Ctrl+U
			OnShowmenuFollowurl();
		case 3:	//Ctrl+C
			break;
		case 22://Ctrl+V
			m_wndInput.SetFocus();
			m_wndInput.Paste();
			break;
		case 96://~ console key
			m_wndInput.SetFocus();
			Prev();
		default:
			m_wndInput.SetFocus();
			m_strTemp.Format("%c",pMsgFilter->wParam);
			m_wndInput.ReplaceSel(m_strTemp);
			break;
		}
		//DEBUG: use this to get a key's value:
		//m_strTemp.Format("msg = %i",pMsgFilter->wParam);
		//MessageBox(m_strTemp);
	}
	*pResult = 0;
}

void CMIMDlg::OnChangeMessage() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	if((!m_Console.m_bConnected)&& (m_iType == CLIENT))
		return;
	
	if(m_iTyping <= 0)//just started typing
		StartTyping();
	m_iTyping = 3;
}

void CMIMDlg::StartTyping()
{
	//MessageBox("Started typing");
	if(m_iType == CLIENT)
	{
		m_strTemp.Format("%i",m_pClientSocket[0]->m_iClientID);
		SendText(0,STYPE,m_strTemp);
	}
	else
	{
		SendText(-1,STYPE,"A");
	}
}

void CMIMDlg::EndTyping()
{
	if((!m_Console.m_bConnected) && (m_iType == CLIENT))
		return;

	//Called after 2 seconds of no typing, or when enter is pressed.
	m_iTyping = 0;
	//MessageBox("Ended typing");
	if(m_iType == CLIENT)
	{
		m_strTemp.Format("%i",m_pClientSocket[0]->m_iClientID);
		SendText(0,ETYPE,m_strTemp);
	}
	else
	{
		SendText(-1,ETYPE,"A");
	}
}

void CMIMDlg::Find()
{
	if(m_bFind)
		return;
	CFindReplaceDialog* FindDlg = new CFindReplaceDialog();
	FindDlg->m_fr.Flags |= FR_NOUPDOWN;//disable up/down options
	FindDlg->Create(TRUE, m_wndShow.GetSelText(),NULL,FR_DOWN,this);
	m_bFind = TRUE;
	//delete FindDlg;
}

LONG CMIMDlg::OnFind(WPARAM wParam, LPARAM lParam)
{
	//The FindDlg sent a message, decipher it
	static long start = 0;
	long found;
	FINDREPLACE *lpfr;
	lpfr = (LPFINDREPLACE) lParam; 

	if(lpfr->Flags & FR_FINDNEXT)
	{
		// Set the selection to be the first occurrence of the word
		FINDTEXTEX ft;
		ft.lpstrText = (LPSTR) lpfr->lpstrFindWhat;

		while(TRUE)
		{
			ft.chrg.cpMin = start;
			ft.chrg.cpMax = -1;
			found = m_wndShow.FindText(lpfr->Flags,&ft);
			//m_strTemp.Format("Found word \"%s\" at %i.",lpfr->lpstrFindWhat,found);
			//MessageBox(m_strTemp);
			if (found != -1)
			{
			   m_wndShow.SetSel(ft.chrgText);
			   start = ft.chrgText.cpMax;
			   m_wndShow.SetFocus();
			   int top = m_wndShow.GetFirstVisibleLine();
			   long here = m_wndShow.LineFromChar(found);
			   m_wndShow.LineScroll(here-top);
			   break;
			}
			else
			{
				if(start == 0)
				{
					m_strTemp.Format("Cannot find the string \"%s\".",lpfr->lpstrFindWhat);
					MessageBox(m_strTemp);
					break;
				}
				int choice = MessageBox("Reached end, would you like to search from the beginning?","String not found",MB_YESNO);
				if(choice == IDNO)
					break;
				else
					start = 0;
			}
		}
	}
	else if(lpfr->Flags & FR_DIALOGTERM)
		m_bFind = FALSE;
	return 0;
}

void CMIMDlg::OnClose() 
{
	//Window closed
	//MessageBox("OnClose");	
	CDialog::OnClose();
}

void CMIMDlg::OnFileLoadcommands()
{
	m_Console.LoadCustom();
	m_Console.DisplayMessage("Loaded custom commands");
}

void CMIMDlg::OnFileLoadBouncer()
{
	int size = LoadBouncer();
	m_strTemp.Format("Loaded Bouncer List. %i Users have access.", size);
	m_Console.DisplayMessage(m_strTemp);
}

void CMIMDlg::OnFileOpendownloadfolder()
{
	m_strTemp = "explorer "+m_strInstallDir + "\\Download";
	if(WinExec(m_strTemp,TRUE) < 32)
		MessageBox("Could not run explorer.exe");
}

HICON CMIMDlg::SetTrayIcon(int uid, int dowhat)
{
	//calls the 2nd SetTrayIcon member, and returns the newly created hIcon var.
	//Avoid recreating the same icon
	HICON hIcon = AfxGetApp()->LoadIcon(uid);
	SetTrayIcon(hIcon,dowhat);
	return hIcon;
}

void CMIMDlg::SetTrayIcon(HICON hIcon, int dowhat)
{
	m_MiniIcon = hIcon;
	CString strTemp2;
	NOTIFYICONDATA nid = {sizeof (nid), GetSafeHwnd (), 100, NIF_ICON|NIF_MESSAGE|NIF_TIP, WM_ICONNOTIFY, m_MiniIcon, "M"};
	CString niTip;
	if(m_Console.m_bConnected)
	{
		GetWindowText(m_strTemp);
		if(m_bAway)
			strTemp2 = "Away";
		else
			strTemp2 = "Online";
	}
	else
	{
		m_strTemp = "Offline";
		strTemp2 = "Offline";
	}
	
	niTip.Format("MIM v%.2f - %s\r\n%i User(s)\r\nStatus: %s", m_dVer, m_strTemp, m_Users.m_iTotalUsers,strTemp2);
	strcpy(nid.szTip, niTip);

	switch (dowhat)
	{
	case 1://add icon to tray
		Shell_NotifyIcon (NIM_ADD, &nid);
		break;
	case 2://modify icon in tray
		Shell_NotifyIcon (NIM_MODIFY, &nid);
		break;
	case 3://remove icon from tray
		Shell_NotifyIcon (NIM_DELETE, &nid);
		break;
	default:
		m_Console.DisplayMessage("<!> Error setting tray Icon");
	}
}

HBRUSH CMIMDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
	//if(nCtlColor == CTLCOLOR_BTN)
	//	MessageBox("OnCtlColor for a button!");
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	CPoint pt(0,0);

	if (this != pWnd)
	{
		// ÁÂÇ¥ °è»ê
		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		pt.x = -(rc.left + GetSystemMetrics(SM_CXDLGFRAME) - 1) % 55;
		pt.y = -(rc.top + GetSystemMetrics(SM_CYDLGFRAME) - 1)% 53;
		
	}
	pDC->SetTextColor(m_clrFore);//Set text color for all controls!!!
	pDC->SetBkColor(m_clrBG);//Set text BG color for all controls!!!
	// ºê·¯½Ã ¿øÁ¡ ¼³Á¤
	m_BrushBG.UnrealizeObject(); //not sure what this does
	pDC->SetBrushOrg(pt); 

    return (HBRUSH)m_BrushBG; 
	//return m_BrushBG;
}

void CMIMDlg::OnUpdateNetworkConnectionsettings(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bShowSend);
}

void CMIMDlg::OnUpdateNetworkServeroptions(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bShowSend);	
}

/*void CMIMDlg::OnRframe() 
{
	//m_bRFrame = TRUE;
}

void CMIMDlg::OnMButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnMButtonUp(nFlags, point);
}

void CMIMDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bRFrame)
	{
		m_ptStart = point;
		MessageBox("worked");
	}
	CDialog::OnLButtonDown(nFlags, point);
}*/

void CMIMDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CMIMDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CMIMDlg::OnPubpad() 
{
	if(m_bPubPad)
		return;
	m_bPubPad = TRUE;
	if(m_PubPadDlg.m_bFirst)
		m_PubPadDlg.Create(IDD_PUBPAD, this);
	m_PubPadDlg.ShowWindow(SW_SHOW);
	m_PubPadDlg.m_pWnd = this;
	m_PubPadDlg.m_ToolBoxDlg.ShowWindow(m_PubPadDlg.m_bToolBox);//show toolbox or not
	/*CRect rect;
	GetWindowRect(&rect);
	m_PubPadDlg.MoveWindow(rect.right-200,rect.top+100,300,300);*/
}

int CMIMDlg::LoadBouncer()
{
	//load list of names and passwords that are allowed into the server
	//returns the length of the user list.
	CFile fBounce;
	CString strLine;
	CCommand Item;
	int i,iLen,iSize;

	iSize = 0;
	m_oaBoName.RemoveAll();
	m_oaBoPass.RemoveAll();
	if(fBounce.Open(m_strInstallDir+"\\Bouncer.txt",CFile::modeNoTruncate|CFile::modeRead/*|CFile::typeText*/))
	{
		if(fBounce.GetLength() == 0)//empty file
		{
			fBounce.Close();
			return 0;//no users allowed in!
		}
		CArchive* ar = new CArchive(&fBounce,CArchive::load);
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
			m_oaBoName.Add(strLine.Left(i));
			m_oaBoPass.Add(strLine.Right(iLen-i-1));
			iSize++;
		}
		delete ar;	
		fBounce.Close();
	}
	return iSize;
}

void CMIMDlg::OnLoadMaster()
{
	LoadMaster();
}

int CMIMDlg::LoadPSL()//load the public list of servers (for the PSL EXE only)
{
	//load list of public servers that have registered, this should only be done for the Master exe,
	//If PSL exe, load the list for sharing, otherwise this list will be overwritten each time
	//the server requests an updated list from the mster server.
	CFile fPublic;
	CString strLine;
	int iSize = 0;
	int i, iLen;
	m_MasterListDlg.m_strPSL_IPs.RemoveAll();
	m_MasterListDlg.m_iPSL_Ports.RemoveAll();

	if(fPublic.Open(m_strInstallDir+"\\PSL.txt",CFile::modeNoTruncate|CFile::modeRead/*|CFile::typeText*/))
	{
		if(fPublic.GetLength() == 0)//empty file
		{
			fPublic.Close();
			return 0;//no MSL found, no biggie
		}
		CArchive* ar = new CArchive(&fPublic,CArchive::load);
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
			m_MasterListDlg.m_strPSL_IPs.Add(strLine.Left(i));
			m_MasterListDlg.m_iPSL_Ports.Add(m_Console.MakeNum(strLine.Right(iLen-i-1),0,32000));
			iSize++;
		}
		delete ar;	
		fPublic.Close();
		m_bPSL_Exe = TRUE;//this exe is now running as a PSL provider!
		strLine.Format("> %i servers loaded from MSL.txt. Now running as an MSL provider.",iSize);
		m_wndShow.ShowMessage(strLine,GREEN);	
	}
	return iSize;
}

int CMIMDlg::LoadMSL()//load the public list of servers (for the MASTER EXE only)
{
	//load list of master servers that have registered, this should only be done for the MSL_exe,
	//aka. if the file Master.dat has a certain keyword in the header.
	//If master exe, load the list for sharing, otherwise this list will be overwritten each time
	//the server requests an updated list from the mster server.
	CFile fMaster;
	CString strLine;
	int iSize = 0;
	int i, iLen;
	m_MasterListDlg.m_strMSL_IPs.RemoveAll();
	m_MasterListDlg.m_iMSL_Ports.RemoveAll();

	if(fMaster.Open(m_strInstallDir+"\\MSL.txt",CFile::modeNoTruncate|CFile::modeRead/*|CFile::typeText*/))
	{
		if(fMaster.GetLength() == 0)//empty file
		{
			fMaster.Close();
			return 0;//no MSL found, no biggie
		}
		CArchive* ar = new CArchive(&fMaster,CArchive::load);
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
			m_MasterListDlg.m_strMSL_IPs.Add(strLine.Left(i));
			m_MasterListDlg.m_iMSL_Ports.Add(m_Console.MakeNum(strLine.Right(iLen-i-1),0,32000));
			iSize++;
		}
		delete ar;	
		fMaster.Close();
		m_bMSL_Exe = TRUE;//this exe is now running as an MSL provider!
		strLine.Format("> %i servers loaded from MSL.txt. Now running as an MSL provider.",iSize);
		m_wndShow.ShowMessage(strLine,GREEN);	
	}
	return iSize;
}

int CMIMDlg::LoadMaster()
{
	//load list of public servers that have registered
	//the user requests an updated MSL from the master server.
	CFile fMaster;
	CString strLine;
	int iEntries,iSize;

	iSize = 0;
	m_MasterListDlg.RemoveAllServers();//the MasterListDlg contains the array of servers

	if(fMaster.Open(m_strInstallDir+"\\Master.dat",CFile::modeNoTruncate|CFile::modeRead/*|CFile::typeText*/))
	{
		if(fMaster.GetLength() == 0)//empty file
		{
			fMaster.Close();
			return 0;//no servers in server list
		}
		CArchive* ar = new CArchive(&fMaster,CArchive::load);

		/*else
		{
			MessageBox("Error reading Master.dat, file has incorrect format. Previously saved public server list will not be available.");
			fMaster.Close();
			delete ar;
			return -1;
		}*/
		*ar >> iEntries;//read number of servers in list
		CServerEntry *Item = new CServerEntry();

		for(int i = 0;i < iEntries;i++)
		{
			Item->Serialize(*ar);//read in a server entry
			m_MasterListDlg.m_oaServers.Add(new CServerEntry(Item));
			//size of this list is total number of public servers available
			//if(!ar->ReadString(strLine))//unexpected end of file
			//	break;
			iSize++;
		}
		delete Item;
		delete ar;	
		fMaster.Close();
		strLine.Format("> %i public servers loaded from Master.dat.",iSize);
		m_wndShow.ShowMessage(strLine,GREEN);		
	}
	return iSize;//total number of public servers in list
}

void CMIMDlg::OnSaveMaster()
{
	SaveMaster();
}

int CMIMDlg::SaveMaster()
{
	//load list of public servers that have registered. This is shown in the server list
	//User must request an updated MSL from the master server.
	CFile fMaster;
	CString strLine;
	int iEntries,iSize;

	iSize = 0;

	//the file should be truncated, as the variable array should contain all the info pre-filtered
	if(fMaster.Open(m_strInstallDir+"\\Master.dat",CFile::modeCreate|CFile::modeWrite))
	{
		CArchive* ar = new CArchive(&fMaster,CArchive::store);
		iEntries = m_MasterListDlg.m_oaServers.GetSize();
		*ar << iEntries;//write the number of servers in list for reading later

		for(int i = 0;i < iEntries;i++)
		{
			m_MasterListDlg.m_oaServers.GetAt(i)->Serialize(*ar);//write the server entry
			iSize++;//size of this list is total number of public servers available
		}
		delete ar;	
		fMaster.Close();
		//DEBUG:
		strLine.Format("> %i public servers saved in Master.dat.",iSize);
		m_wndShow.ShowMessage(strLine,GREEN);		
	}
	return iSize;//total number of public servers in list
}

void CMIMDlg::OnSavePsl()
{
	//save list of public servers that have registered, this should only be done for the Master exe only,
	CFile fPublic;
	CString strLine;
	int iMax = m_MasterListDlg.m_strPSL_IPs.GetCount();
	int i;

	if(fPublic.Open(m_strInstallDir+"\\PSL.txt",CFile::modeCreate|CFile::modeWrite))
	{
		CArchive* ar = new CArchive(&fPublic,CArchive::store);
		for(i = 0; i < iMax;i++)
		{
			strLine.Format("%s %i",m_MasterListDlg.m_strPSL_IPs.GetAt(i),m_MasterListDlg.m_iMSL_Ports.GetAt(i));
			if(i > 0)
				strLine.Insert(0,"\r\n");
			ar->WriteString(strLine);//write error
		}
		if(i < 1)
			m_wndShow.ShowMessage("> Nothing to save to PSL.txt.", YELLOW);

		delete ar;
		fPublic.Close();
		m_bPSL_Exe = TRUE;//this exe is now running as an PSL provider!
		strLine.Format("> %i server(s) saved to PSL.txt.",i);
		m_wndShow.ShowMessage(strLine,GREEN);
	}
	return;
}

void CMIMDlg::OnSaveMsl()
{
	//save list of public servers that have registered, this should only be done for the Master exe only,
	CFile fMaster;
	CString strLine;
	int iMax = m_MasterListDlg.m_strMSL_IPs.GetCount();
	int i;

	if(fMaster.Open(m_strInstallDir+"\\MSL.txt",CFile::modeCreate|CFile::modeWrite))
	{
		CArchive* ar = new CArchive(&fMaster,CArchive::store);
		for(i = 0; i < iMax;i++)
		{
			strLine.Format("%s %i",m_MasterListDlg.m_strMSL_IPs.GetAt(i),m_MasterListDlg.m_iMSL_Ports.GetAt(i));
			if(i > 0)
				strLine.Insert(0,"\r\n");
			ar->WriteString(strLine);//write error
		}
		if(i < 1)
			m_wndShow.ShowMessage("> Nothing to save to MSL.txt.", YELLOW);

		delete ar;
		fMaster.Close();
		m_bMSL_Exe = TRUE;//this exe is now running as an MSL provider!
		strLine.Format("> %i server(s) saved to MSL.txt.",i);
		m_wndShow.ShowMessage(strLine,GREEN);
	}
	return;
}

void CMIMDlg::OnLoadMsl()
{
	if(LoadMSL() <= 0)
		m_wndShow.ShowMessage("<!> No MSL file",YELLOW);
}

int CMIMDlg::IncOKCount(int ID)
{
	//increment the OK counter for all users
	int i = 0;
	//if(ID != -1)
	//	i = ID;
	for(;i < ID_MAX;i++)
	{
		m_iUserOK[ID]++;
		//if(ID != -1)
		//	break;//stop the loop after single user
	}
	return i;
}

void CMIMDlg::OnNetworkMasterserver()
{
	if((m_iType == SERVER) && (m_Console.m_bConnected))
		MessageBox("As a server, you must disconnect before vieiwing the master list.");
	else
	{
		m_MasterListDlg.DoModal();
		m_bServerInfoRequest = FALSE;//end the info-request mode
	}
}

int CMIMDlg::ServerInfoRequest(int index)
{
	//this function will set up MIM to be in INFO-Request mode and a loop will cycle through all entries in the server list
	m_bServerInfoRequest = TRUE;
	//this will connect to the server in the MasterList(index) and request the info, then disconnect
	ServerRequestLoop(0);//start with the first server in the list.
	//After this, each successive call to ServerRequeestLoop(m_iInfoRequestIndex) is made in OnReceive()
	return 0;
}

int CMIMDlg::ServerRequestLoop(int index)
{
	//request info from servers in Server List. If server is dead this function will continue iterating until end of list

	int iMax = m_MasterListDlg.m_oaServers.GetSize();
	while(m_iInfoRequestIndex < iMax)
	{
		if(m_bStopServerInfoRequest)//user cancelled search before completion
		{
			m_wndShow.ShowMessage("Cancelling server search...", YELLOW);		
			break;
		}
		int result = InfoRequestConnect();//this will delete psocket before checking if it should connect, then it will connect
		if(result == 0)
			return 0;//connection was successful. End this function and wait for destination server to accept AND OnReceive() to get the info-request
		else if(result <= -2)//should not be here, user may have pressed cancel. exit completely
		{
			//m_iInfoRequestIndex = iMax;
			break;
		}
		else//the server was not there. m_iStatus=2 has been set, so loop and repeat now
			m_iInfoRequestIndex++;//else
	}
	//end the loop
	m_iInfoRequestTimer = 0;//setting to 0 instead of 1 ensures that the next server in list will not be pinged
	m_bStopServerInfoRequest = FALSE;
	//m_wndShow.ShowMessage("<!> Closing server connection.");
	m_strTemp.Format("> %i public servers scanned.", m_iInfoRequestIndex);
	m_wndShow.ShowMessage(m_strTemp, GREEN);		
	//all servers in list have been cycled
	m_bServerInfoRequest = FALSE;
	m_MasterListDlg.DisableButtons(FALSE);//enable the buttons again
	return 0;//done
	//This function is called again from the OnReceive() after it determines the server info
}

int CMIMDlg::RetrievePublicList()
{
	m_bPublicInfoRequest = TRUE;
	int iMax = m_MasterListDlg.m_strPSL_IPs.GetSize();
	while(m_iInfoRequestIndex < iMax)
	{
		//Retrives the Public Server List from the Master Server
		int result = InfoRequestConnect();//this will delete psocket before checking if it should connect, then it will connect
		//returns 0 if connection was successful. End this function and wait for destination server to accept AND OnReceive() to get the MSL-request
		if(result <= -2)
		{
			m_wndShow.ShowMessage("MSL request cancelled.", YELLOW);		
			m_bPublicInfoRequest = FALSE;
			m_MasterListDlg.DisableButtons(FALSE);//enable the buttons again
			return -1;
		}
	}
	return 0;
}

int CMIMDlg::RetrieveMasterList()
{
	//Retrieve the MSL from me
	m_bMasterInfoRequest = TRUE;
	//Retrives the Master Server List from the MasterHD
	int result = InfoRequestConnect();//this will delete psocket before checking if it should connect, then it will connect
	//returns 0 if connection was successful. End this function and wait for destination server to accept AND OnReceive() to get the MSL-request
	if(result <= -2)
	{
		m_wndShow.ShowMessage("MSL request cancelled.", YELLOW);		
		m_bMasterInfoRequest = FALSE;
		m_MasterListDlg.DisableButtons(FALSE);//enable the buttons again
		return -1;
	}
	return 0;
}

struct ClipboardData
{
	UINT   m_nFormat;
	TCHAR  m_szFormatName[100];
	UINT   m_nLength;
	LPVOID m_pData;
};

void CMIMDlg::OnUserSendclipboard()
{
	if((m_pClientSocket[0] == NULL) && (m_iType == CLIENT))
		return;
	if((m_pListenSocket == NULL) && (m_iType == SERVER))
		return;

	int sel = m_ctlUserList.GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);

	//perform checks on the selected destination user ID
	if(sel < 0)//not admin or not selected
	{
		MessageBox("Select a person to send your clipboard first.", "Send Clipboard", MB_OK | MB_ICONEXCLAMATION);	
		return;
	}

	int ID = GetID(m_ctlUserList.GetItemText(sel,0));

	if((sel > 0)&&(m_iType == SERVER))//server send to client
	{
		if(ID < 0)//sending to self (admin)
		{
			m_wndShow.ShowMessage("<!> Invalid number or range.",RED);
			return;
		}
	}
	else if(m_iType == CLIENT)//cant send to self
	{
		if(ID == m_pClientSocket[0]->m_iClientID)
		{
			MessageBox("Cant send clipboard to that user.", "Sending Clipboard", MB_OK | MB_ICONEXCLAMATION);	
			return;
		}
	}
	else
	{
		MessageBox("Cant send clipboard to that user.", "Sending Clipboard", MB_OK | MB_ICONEXCLAMATION);	
		return;
	}

	WriteClipboard(m_strInstallDir+"\\ClipBoard.clp");//write a temp default file location, then send this to that user
	m_wndShow.ShowMessage(m_strInstallDir+"\\ClipBoard.clp");
	ClearFileList();
	m_strFileDirs.Add(m_strInstallDir+"\\");
	m_strFileNames.Add("ClipBoard.clp");
	m_iTotal = 1;//total # of files
	SendFiles(ID);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
	//extra cool functions
    /*while(!GetAsyncKeyState(VK_F5)){ Sleep(100); }    //wait until F5 is pressed
    bool bEnd = false;    //wheter to stop spamming
    while( bEnd == false ){    //while not told to stop, run the loop
        if(GetAsyncKeyState(VK_F8)){ bEnd = !bEnd; }    //if F8 is pressed, stop spamming

        //CTRL+V is standard Windows-shortcut for Paste
        keybd_event(VK_CONTROL, 0, 0, 0);    //press down CTRL (control)
        keybd_event(VkKeyScan('v'),0xB2,0 , 0);    //press down the key 'v'
        keybd_event(VkKeyScan('v'),0xB2, KEYEVENTF_KEYUP,0);    //release the key 'v'
        keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);    //release CTRL (control)

        //and press Return to send text (in MSN)
        keybd_event(VK_RETURN, 0, 0, 0);    //press down Return-key
        keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);    //release Return-key
    }*/

////////////////////////////////////////////////////////////////////////////////////////////////////////
int CMIMDlg::LoadClipBoard(CString strDir)
{
	CList <ClipboardData, ClipboardData&> m_lstData;//TEST
	int iMax = 0;

	//write it all to a file
	CFile ClipFile;
	if(ClipFile.Open(strDir,CFile::modeRead))
	{
		ClipboardData data;
		CArchive* ar = new CArchive(&ClipFile,CArchive::load);
		*ar >> iMax;//write the number of elements in the clipboard, for reading later
		for(int i = 0;i < iMax; i++)
		{
			*ar >> data.m_nFormat;
			*ar >> data.m_nLength;
			ar->Read(data.m_szFormatName,100);

			data.m_pData = new byte[data.m_nLength];
			ar->Read(data.m_pData,int(data.m_nLength));
			//m_strTemp.Format("format=%i\nstrlen=%i\nName=%s\nData=%s", data.m_nFormat, data.m_nLength ,data.m_szFormatName, data.m_pData);
			//MessageBox(m_strTemp,"Read from File");
			m_lstData.AddTail(data);
			//delete data.m_pData;
		}
		delete ar;
		ClipFile.Close();
	}

	//put buffer into clipboard from ClipboardData data (there may be more than one element in the list)
	if(OpenClipboard())//open the clipboard
	{
		m_strTemp = "Reading Clipboard data from file...";
		m_wndShow.ShowMessage(m_strTemp);
		::EmptyClipboard();//empty anything in it
		UINT format = 0;
		POSITION pos = m_lstData.GetHeadPosition();
		while(pos != NULL)//loop through and place the read data into the clipboard
		{
			ClipboardData & data = m_lstData.GetNext( pos );
			UINT format = data.m_nFormat;

			if( data.m_szFormatName[0] != 0 )
			{
 				UINT u = RegisterClipboardFormat( data.m_szFormatName );//if the format already exists, the nFormat
				if( u > 0 )
					format = u;
				else
				{
					m_strTemp = "Warning: The clipboard content you have loaded does not already have an associated format type. A new format type has been added to your registry. If it came from another computer, make sure to install the application that uses this format:";
					m_wndShow.ShowMessage(m_strTemp,YELLOW);
				}
			}
			HANDLE hMem = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, data.m_nLength );
			LPVOID pMem = ::GlobalLock( hMem );

			memcpy( pMem, data.m_pData, data.m_nLength );
			::GlobalUnlock( hMem );
			::SetClipboardData( format, hMem );
			m_strTemp.Format("Format=%i   Size=%iBytes   Type_Name=\"%s\"", format, data.m_nLength ,data.m_szFormatName);
			m_wndShow.ShowMessage(m_strTemp);
		}
		CloseClipboard();
	}
	else
	{
		m_strTemp = "<!> Cant write to clipboard data! OpenClipboard() Failed.";
		m_wndShow.ShowMessage(m_strTemp,RED);
		return -1;//if failed to open clipboard
	}
	m_strTemp.Format("Clipboard read from file \"%s\"",strDir);
	m_wndShow.ShowMessage(m_strTemp,GREEN);
	return 0;
}


void CMIMDlg::OnOpenClipboard()
{
	CString strSave, strFilter;
	strFilter = "Clipboard Files (*.clp)|*.clp||";

	CFileDialog dLoad(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, strFilter);
	SetCurrentDirectory(m_strInstallDir);

	if(dLoad.DoModal() == IDOK)
	{
		strSave = dLoad.GetPathName();
		LoadClipBoard(strSave);
	}
}

// writes the current clipboard contents to the file at strPath
int CMIMDlg::WriteClipboard(CString strPath)
{
	//writes the current clipboard data to the file at strPath
	CList <ClipboardData, ClipboardData&> m_lstData;//TEST
	int iMax = 0;
	//read buffer from clipboard, save all data to ClipboardData data (there may be more than one element in the list)
	if(OpenClipboard()) //open the clipboard
	{
		UINT format = 0;

		m_strTemp = "Displaying current clipboard data...";
		m_wndShow.ShowMessage(m_strTemp);
		while((format = ::EnumClipboardFormats(format)) != 0 )
		{
			ClipboardData data;
			data.m_nFormat = format;

			if( format <= 14 )
				data.m_szFormatName[0] = 0;
			else if( GetClipboardFormatName(format, data.m_szFormatName, 100) == 0 )
				data.m_szFormatName[0] = 0;

			HANDLE hMem = ::GetClipboardData( format );
			if( hMem == NULL )
				continue;

			data.m_nLength = ::GlobalSize(hMem);

			LPVOID pMem = ::GlobalLock( hMem );
			data.m_pData = new byte[data.m_nLength];

			memcpy(data.m_pData, pMem, data.m_nLength);

			m_lstData.AddTail(data);
			iMax++;//keep track of how many things have been loaded
			//DEBUG:
			m_strTemp.Format("Format=%i   Size=%iBytes   Type_Name=\"%s\"", data.m_nFormat, data.m_nLength ,data.m_szFormatName);
			m_wndShow.ShowMessage(m_strTemp);
		}
		CloseClipboard();
	}
	else
	{
		m_strTemp.Format("<!> Cant copy clipboard data! OpenClipboard() Failed.",strPath);
		m_wndShow.ShowMessage(m_strTemp,RED);
		return -1;//if failed to open clipboard
	}
	POSITION pos = m_lstData.GetHeadPosition();
	//write it all to a file
	CFile ClipFile;
	if(ClipFile.Open(strPath,CFile::modeCreate|CFile::modeWrite))
	{
		//m_wndShow.ShowMessage(strPath);
		CArchive* ar = new CArchive(&ClipFile,CArchive::store);
		*ar << iMax;//write the number of elements in the clipboard, for reading later
		for(int i = 0;i < iMax; i++)
		{
			ClipboardData &data = m_lstData.GetNext(pos);
			*ar << data.m_nFormat;
			*ar << data.m_nLength;
			ar->Write(data.m_szFormatName,100);
			ar->Write(data.m_pData,int(data.m_nLength));
		}
		ar->Flush();
		delete ar;
		ClipFile.Close();
	}
	else
	{
		m_strTemp.Format("<!> Could not create file \"%s\" for writing clipboard data!",strPath);
		m_wndShow.ShowMessage(m_strTemp,RED);
	}
	m_strTemp.Format("Clipboard saved to file \"%s\"",strPath);
	m_wndShow.ShowMessage(m_strTemp,GREEN);
	return 0;
}

void CMIMDlg::OnSaveClipboard()
{
	//asks user for location then saves current clipboard data to file .clp
	CString strSave, strExt, strFilter;
	strFilter = "Clipboard Files (*.clp)|*.clp||";

	CFileDialog dSave(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, strFilter);
	SetCurrentDirectory(m_strInstallDir);

	if(dSave.DoModal() == IDOK)
	{
		strSave = dSave.GetPathName();
		strExt = dSave.GetFileExt();
		if(strExt != "")//remove it from strSave
		{
			int i = strSave.GetLength()-1;
			while(strSave[i] != '.')
				i--;
			strSave=strSave.Left(i);
			MessageBox(strSave);
		}
		strSave += ".clp";
		WriteClipboard(strSave);
	}
}
void CMIMDlg::OnLoadClipboard()
{
	//Opens the default location cliboard file: MIM\clipboard.clp
	LoadClipBoard(m_strInstallDir+"\\ClipBoard.clp");
}

void CMIMDlg::OnSaveClipboardQuick()
{
	//Saves current clipboard daya to the default location: MIM\clipboard.clp
	WriteClipboard(m_strInstallDir+"\\ClipBoard.clp");
}
