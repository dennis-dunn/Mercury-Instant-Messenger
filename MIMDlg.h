// MIMDlg.h : header file
//

#if !defined(AFX_MIMDLG_H__F7C7CFA3_63E6_4FC3_940A_0298C6EFF7D9__INCLUDED_)
#define AFX_MIMDLG_H__F7C7CFA3_63E6_4FC3_940A_0298C6EFF7D9__INCLUDED_

#include "IntroDlg.h"	// Added by ClassView
#include "FlatComboBox.h"	// Added by ClassView
#include "MyEdit.h"	// Added by ClassView
#include "Pkg.h"	// Added by ClassView
#include "MyEditIn.h"	// Added by ClassView
#include "RichTextBar.h"	// Added by ClassView
#include "ReferenceDlg.h"	// Added by ClassView
#include "ServerDlg.h"	// Added by ClassView
#include "SettingsDlg.h"	// Added by ClassView
#include "ProfileDlg.h"	// Added by ClassView
#include "MySocket.h"	// Added by ClassView
#include "Console.h"	// Added by ClassView
#include "PasswordDlg.h"	// Added by ClassView
#include "AgreeDlg.h"	// Added by ClassView
#include "ShareInfo.h"	// Added by ClassView
#include "MBoxDlg.h"	// Added by ClassView
#include "IconListCtrl.h"	// Added by ClassView
#include "SaveDLDlg.h"	// Added by ClassView
#include "PubPadDlg.h"	// Added by ClassView
#include "masterlistdlg.h"
#include "ServerEntry.h"
#include "ColorButton.h"

//#include "FileShareDlg.h"	// Added by ClassView

#define CYEAR 2007//copyright year
#define GREEN RGB(0,255,0)
#define RED RGB(255,0,0)
#define BLUE RGB(60,60,255)
#define LBLUE RGB(164,176,238)
#define YELLOW RGB(255,255,0)
#define BLACK RGB(0,0,0);

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMIMDlg dialog
#define WM_UPDATECHARFORMAT WM_USER + 1
//Declare tray Icon message
const UINT WM_ICONNOTIFY = ::RegisterWindowMessage (_T ("WM_ICONNOTIFY"));
//declare ctrl+F hotkey
const UINT wm_Find = RegisterWindowMessage( FINDMSGSTRING );

class CMIMDlg : public CDialog
{
// Construction
public:
	void SetStatusText(CString strText, int iPane, int iIconRes = 0);
	int m_iMaxAutoSize;
	int PlayWav(CString strFile);
	int m_iBounceTimer;
	CStringArray m_oaBoName;
	CStringArray m_oaBoPass;
	int LoadBouncer();//load the bouncer.txt file
	int LoadMaster();//load the master.dat file
	int LoadPSL();//Load the PSL.dat file
	int LoadMSL();//Load the MSL.dat file
	int SaveMaster();//save the current list varaible into the master.dat file
	BOOL m_bPubPad;
	CPubPadDlg m_PubPadDlg;
	unsigned int m_iByteRec;
	unsigned int m_iByteSent;
	BOOL m_bCancelled;	//CLIENT ONLY pressed disconnect button while connecting, is set back to false after CSocket->Connect() fails
	BOOL m_bIsCalling;	//CLIENT ONLY pressed connect and is waiting.
	static UINT OnIntroThread(LPVOID pParam);
	static UINT OnConnectT(LPVOID pParam);
	CIntroDlg m_wndIntro;
	long m_lLastSaveHisPos;	//character index of the last saved text (for autosave without removing text from message box)
	int m_iAutoSaveHis;
	void LoadServerText();
	BOOL m_bTrayNotify;		//indicates if tray is on and user was notified once (new icon and beep).
	void SetTrayIcon(HICON hIcon, int dowhat = 1);
	HICON SetTrayIcon(int uid, int dowhat = 1);
	HICON m_MiniIcon;
	BOOL m_bTrayed;		//whether window is in hidden and in tray
	void OnAwayDef(BOOL bForce=TRUE);
	bool m_bAway;
	bool m_bForceAway;
	void ClearFileList();
	int SaveMessHTML(CString strSave, BOOL bAuto = FALSE);
	void NotePad(CString strFile);
	void OnDictionary(CString strWord);
	void OnGoogle(CString strURL);
	BOOL m_bFind;
	void StartTyping();
	void EndTyping();
	int m_iTyping;				//how long ago you stopped typing
	int AddLine(CString strFile, CString strLine);
	int Browser(CString strURL);
	CString GetYourIP();
	CString m_strDate;
	CString GetDate();
	BOOL m_bListenAble;			//flag used to determine who should lsiten when establishing a connection for filesharing
	void SendFiles(int ID);
	//CSaveDLDlg m_SaveDLDlg;
	BOOL m_bActive;				//Whether or not window is 0 z-order etc
	CString Savemess(CString strSave, BOOL bAuto = FALSE, CString strExt = "");
	void SyncChanges();
	CImageList m_imgContactList;
	CImageList m_imgMasterList;
	void InitImageLists();
	CMBoxDlg m_MBoxDlg;
	CString GetTimeStamp(char delim = ':');
	CString m_strTime;
	BOOL m_bTimeStamp;
	BOOL m_bAutoSaveMess;
	CTime m_CurTime;
	CTime m_FirstTime;
	BOOL m_bSimple;
	CBitmap m_bitmap;

	BOOL m_bAgree;
	CString m_strRel;
	void Viewprofile(int ID);
	CString m_strConRoomName;
	CString m_strOpeningText;
	CString m_strBrowser;
	//CShareInfo m_ShareInfo;
	int FindEmptyShare();
	BOOL m_bSharing[ID_MAX];
	int m_iTotal;		//total # of files ready for transfer (length of both string arrays)
	CStringArray m_strFileNames;
	CStringArray m_strFileDirs;
	CString m_strInstallDir;
	CString m_strSaveDL;
	CMySocket m_pListenSocket;
	CMySocket *m_pClientSocket[ID_MAX];
	CSocketFile *m_pSocketFile[ID_MAX];
	CMySocket *m_pInfoReqSocket;		//used for info-request only
	CSocketFile *m_pInfoReqSocketFile;	//used for info-request only, this HAS to be a pointer in order to attach a CArchive object to it :(

	void Exit();
	CString GetIP(int ID);
	CAgreeDlg m_AgreeDlg;
	//CFileShareDlg m_FileShareDlg;
	CPasswordDlg m_PasswordDlg;

	int BrowseFile();
	int AddFile(CString strLoad);
	int GetListPos(int ID);
	int SetListIcon(int iImage, int iSel);
	int NewSocket();
	BOOL Agree();
	void SendMess(int ID, CString strText, int iCode = MESG, BOOL bShow = FALSE);
	void SendMess(int ID, CPkg Text, int iCode = MESG, BOOL bShow = FALSE);
	void OnBtnSend2();
	int OnAccepted();
	int AcceptNew(int connect);
	int GetID(CString strName);
	void Prev();
	int m_iTPrev;	//total previous commands
	int m_iPrev;
	int PlayRecSound();
	int SendText(int ID, int code, CString strText);
	int SendText(CPacket SPacket);
	int Reposition(int width = 0, int height = 0);
	void DeleteSocket(int ID);
	void Finalize();
	void Reject(int ID);
	int FindEmpty();
	CString m_strSysDir;
	int SetFlood(int &iFlood, int iDur);
	CStringArray m_strPrev;
	int m_iPing;
	void Clear();
	void CloseCon();
	CConsole m_Console;
	int IncOKCount(int ID);
	int InfoRequestConnect();

	//FileSharing
	CString m_strDLto;

	//CON SETTINGS
	int m_iPort;
	int m_iType;//bool
	int m_iCensor;
	int m_iIdleTime;//reserved1
	int m_iReserved2;
	int m_iReserved3;
	int m_iSaveMessType;
	BOOL m_bAutoLogin;
	BOOL m_bAllowRun;
	BOOL m_bSound;
	BOOL m_bAsk;
	BOOL m_bAlert;
	BOOL m_bDebug;	//3.10, whether or not to show advanced update messages
	BOOL m_bPublic;	//3.10, whether or not to be shown in the master server list
	CString m_strScreenName;
	CString m_strIP;
	CString m_strComment;
	CString m_strSoundDir;
	CString m_strSoundExt;
	CString m_strAniExt;
	CString m_strReserved1;
	CString m_strReserved2;
	CString m_strReserved3;
	//strcpy(m_SavedData.m_strChatDir, "C:\\");dont edit this! its changed during install
	CStringArray m_strSaved;	//saved IP's
	CStringArray m_strComments; //Saved IP comments
	//SERVER OPTIONS
	int m_iListenPort;
	BOOL m_bBouncer;
	int m_iBounceSec;			//internval in seconds for updating user list
	CString m_strAdminName;
	CString m_strRoomName;
	CString m_strPassword;
	CStringArray m_strBanned;	//saved banned IP's
	CString m_strModPass;		//mod password

	//PROFILE outside of class
	CString m_strName;
	CString m_strPets;
	CString m_strQuote;
	CString m_strLive;
	CString m_strEmail;
	CString m_strImageDir;

	class CProfile
	{
	public:
		char m_strName[40];
		char m_strPets[40];
		char m_strQuote[400];
		char m_strLive[40];
		char m_strEmail[40];
		//CBitmap m_Bitmap;
		BOOL m_bAni;
		int m_iGender;
		int m_iAge;
		int m_iReq;		//client requesting data (For server)
		int m_iRep;		//clietn replying to request (For server)
	}m_MyProfile, m_SendProfile;
	//Mac's are like automatic transmissions, they're easy to use, but you can't do much with them.

	class CUsers  
	{
	public:
		int m_iTotalUsers;			//total current users in room
		int m_iMax;					//Max users allowed in room
		int m_iImage[ID_MAX];		//image_list ID for each user's icon
		int m_iConnected[ID_MAX];	//0=discon, 1=connected, 2=registering, 3=INFO request only.
		char m_strAdmin[30];			//Admin Name
		char m_strNames[ID_MAX][30];	//List of all screenNames
		//char m_strComment[IDMAX][30];	//List of all comments. SUGGEST: why not just make the name longer, and have comment be after carraige return
	}m_Users;

	int m_iFlood;			//time left to send another message
	int m_iProfile;			//time left to view another profile
	CProfileDlg m_ProfileDlg;
	BOOL m_bProfileOpen;
	int Profile(BOOL view);
	CSettingsDlg m_SettingsDlg;
	CServerDlg m_ServerDlg;
	CImageList m_imgUsers;
	double m_dLowFileVer;	//Oldest compatible MIM.dat version for saved settings
	double m_dVer;			//Current MIM.dat and MIM version
	double m_dLowVer;		//Oldest compatible version with current version

	CString m_strTemp;
	BOOL m_bIgnore[ID_MAX];
	BOOL m_bShowSend;
	BOOL m_bShowSet;
	
	static int CALLBACK EnumFontFamProc(ENUMLOGFONT *lpelf, NEWTEXTMETRIC *lpntm,int nFontType, LPARAM lParam);
	CString Censor(CString strCensor);
	void UpdateList();
	void ShowSend(BOOL Show);
	void ShowSet(BOOL Show);
	void Connect();
	void SetDefaults();
	void OnCancel();
	void OnOK();
	CString m_strServerIP;
	void SaveSettings(BOOL bMessage = TRUE);
	void OnSaveSettings();
	void LoadSettings();
	void UpdateCharFormat4In(/*CPkg &pkg*/);
	void GetCharFormat(CHARFORMAT &cf);

	CFlatComboBox m_cmbFontName;
	CFlatComboBox m_cmbFontSize;

	void SetDefaultCharFormat4Input();
	void AssemblePkg(CPkg& pkg, CString strText);
	int m_iFontSize;
	CString m_strFontName;
	BOOL m_bCharUnderline;
	BOOL m_bCharStrikeOut;
	BOOL m_bCharItalic;
	COLORREF m_clrChar;		//console text color
	COLORREF m_clrBack;		//console BG text color
	CHARFORMAT m_cfInput;
	COLORREF m_clrFore;		//common color to be used for all text controls
	COLORREF m_clrBG;		//common color to be used for BG of all controls AND text BG
	CBrush m_BrushBG;

	/*void OnCharUnderline();
	void OnCharStrikeout();
	void OnCharColor();
	void OnCharItalic();
	void OnCharBold();*/
	void OnCharBack();
	BOOL m_bCharBold;
	CStatusBarCtrl m_statusBar;
	void OnBSend(CString strSend, BOOL bRemote = FALSE);
	//BOOL m_bConnected;
	CRichTextBar m_wndRichTextBar;
	CMIMDlg(CWnd* pParent = NULL);	// standard constructor

	void OnAccept();
	void OnConnect();
	void OnCloseCon(int id = 0);
	void OnSend();
	void OnReceive(CPacket &RPacket,int ID = 0);

// Dialog Data
	//{{AFX_DATA(CMIMDlg)
	enum { IDD = IDD_MIM_DIALOG };
	CColorButton	m_ctlPubPad;
	CIconListCtrl	m_ctlUserList;
	CColorButton	m_ctlSimple;
	CColorButton	m_ctlMBox;
	CColorButton	m_ctlKick;
	CColorButton	m_ctlTray;
	CColorButton	m_ctlPrev;
	CColorButton	m_ctlEditProfile;
	CColorButton	m_ctlServer;
	CColorButton	m_ctlSendFile;
	CColorButton	m_ctlViewProfile;
	CColorButton	m_ctlIgnore;
	CColorButton	m_ctlSaveMess;
	CColorButton	m_ctlClear;
	CColorButton	m_ctlDisconnect;
	CColorButton	m_ctlConnect;
	CColorButton	m_ctlSettings;
	CColorButton	m_btnSend;
	CMyEdit	m_wndShow;
	CMyEditIn m_wndInput;
	CString	m_strUsers;
	CString	m_strCurRoom;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMIMDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CToolTipCtrl m_tooltip;
	HICON m_hIcon;
	HICON m_hAdminIcon;
	HICON m_hMessIcon;
	HICON m_hMessAIcon;
	// Generated message map functions
	//{{AFX_MSG(CMIMDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnSend();
	afx_msg void OnCharBold();
	afx_msg void OnCharColor();
	afx_msg void OnCharItalic();
	afx_msg void OnCharStrikeout();
	afx_msg void OnCharUnderline();
	afx_msg void OnUpdateCharItalic(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCharStrikeout(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCharUnderline(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCharBold(CCmdUI* pCmdUI);
	afx_msg void OnFileLoadsettings();
	afx_msg void OnFileRestoresettings();
	afx_msg void OnFileSavemessages();
	afx_msg void OnHelpAbout();
	afx_msg void OnHelpHelp();
	afx_msg void OnHelpPatch();
	afx_msg void OnHelpReadme();
	afx_msg void OnFileExit();
	afx_msg void OnSettings();
	afx_msg void OnServerOptions();
	afx_msg void OnBConnect();
	afx_msg void OnBDisconnect();
	afx_msg void OnBEditprofile();
	afx_msg void OnBClear();
	afx_msg void OnViewprofile();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPrev();
	afx_msg void OnHelpEula();
	afx_msg void OnKick();
	afx_msg void OnIgnore();
	afx_msg void OnAway();
	afx_msg void OnAvailable();
	afx_msg void OnSendfile();
	afx_msg LONG OnFind(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIconNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnIconShutdown();
	afx_msg void OnIconRestore();
	afx_msg void OnSavemess();
	afx_msg void OnMinicon();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnShowmenuCopy();
	afx_msg void OnShowmenuFollowurl();
	afx_msg void OnInputmenuUndo();
	afx_msg void OnInputmenuCut();
	afx_msg void OnInputmenuCopy();
	afx_msg void OnInputmenuPaste();
	afx_msg void OnInputmenuSelectall();
	afx_msg void OnMbox();
	afx_msg void OnSimple();
	afx_msg void OnUserBan();
	afx_msg void OnDblclkUserlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnMsgfilterRoom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeMessage();
	afx_msg void Find();
	afx_msg void OnMenuGoogle();
	afx_msg void OnMenuDictionary();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnFileViewhistory();
	afx_msg void OnFileViewquotes();
	afx_msg void OnFileViewcommands();
	afx_msg void OnFileLoadcommands();
	afx_msg void OnFileLoadBouncer();
	afx_msg void OnFileOpenBouncer();
	afx_msg void OnFileOpendownloadfolder();
	afx_msg void OnFileLoadservertxt();
	afx_msg void OnFileViewServertxt();
	afx_msg void OnUpdateNetworkConnectionsettings(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNetworkServeroptions(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPubpad();
	//}}AFX_MSG
	afx_msg void OnKickIdle(WPARAM, LPARAM);
	afx_msg void UpdateCharFormat4Input(WPARAM param = NULL, BOOL bGetSel = TRUE);
	afx_msg void OnSelectFontName();
	afx_msg void OnSelectFontSize();

	DECLARE_MESSAGE_MAP()
public:
	// number of seconds remaining until the server will send an OK request to all clients to make sure their still connected. Re-init this value each time the server sends out a similar request.
	int m_iOKTimer;//keep-alive timer
	// //Array that tracks which users are in good standing with connection. Value greater than 0 indicates how many times the client did not respond to an OK request
	int m_iUserOK[ID_MAX];
	CMasterListDlg m_MasterListDlg;
	afx_msg void OnNetworkMasterserver();
	//BOOL m_bMasterRequest;
	// true if this instance of the exe is the Master server and needs to behave accordingly
	BOOL m_bPSL_Exe;
	BOOL m_bMSL_Exe;//TODO: use this for the MSL providers
	afx_msg void OnSaveMaster();
	afx_msg void OnLoadMaster();
	int ServerInfoRequest(int index = -1);//request the server info, can't be called while a chatroom connection is open
	int ServerRequestLoop(int index);//Connect to the server specifid, calls itself if fails to connect

	BOOL m_bServerInfoRequest;// =TRUE if m_pInfoReqSocket is being used to request server info, and NOT for a normal chatroom. Therefore don't display the same console messages when connecting, etc.
	BOOL m_bPublicInfoRequest;// =TRUE if m_pInfoReqSocket is being used to request Public server list info, and NOT for a normal chatroom. Therefore don't display the same console messages when connecting, etc.
	BOOL m_bMasterInfoRequest;// =TRUE if m_pInfoReqSocket is being used to request Master server list info, and NOT for a normal chatroom. Therefore don't display the same console messages when connecting, etc.
	BOOL m_bStopServerInfoRequest;//Call to stop the loop ASAP
	// the index of the ObArray of Server Entries for info-requesting
	int m_iInfoRequestIndex;//server obArray index of the current attempt info-request
	int m_iInfoRequestTimer;//number of seconds remaining until the attempt will close the connection if no response
	int CreateComboBoxes(void);
	afx_msg void OnUserSendclipboard();
	int LoadClipBoard(CString strDir);
	int RetrieveMasterList();//retrieve the MSL from me
	int RetrievePublicList();//retrive the PSL from first available Master Server
	afx_msg void OnOpenClipboard();
	// writes the current clipboard contents to the file at strPath
	int WriteClipboard(CString strPath);
	afx_msg void OnSaveClipboard();
	afx_msg void OnLoadClipboard();
	afx_msg void OnSaveClipboardQuick();
	afx_msg void OnSaveMsl();
	afx_msg void OnLoadMsl();
	afx_msg void OnSavePsl();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIMDLG_H__F7C7CFA3_63E6_4FC3_940A_0298C6EFF7D9__INCLUDED_)
