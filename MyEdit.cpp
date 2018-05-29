// MyEdit.cpp: implementation of the CMyEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIM.h"
#include "MyEdit.h"
#include "MIMDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyEdit::CMyEdit()
{
	Brush.CreateSolidBrush(RGB(255, 255, 255));
	Font.CreateFont(15, 15, 0, 0, FW_BOLD, FALSE, FALSE,0,0,0,0,0,0, "Lucida Console");
	m_iNewLine = 0;
	m_iOldLine = 0;
}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CRichEditCtrl)
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowWin message handlers

int CMyEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	SetReadOnly();	
	return 0;
}

HBRUSH CMyEdit::CtlColor(CDC* pDC,UINT nCtlColor)
{
	pDC->SetBkColor(RGB(255,255,255));
	return (HBRUSH)Brush;
}

void CMyEdit::ShowMessage(CPkg& pkg, BOOL bRet)
{
	CString message;
	message = pkg.strText;

	if(bRet)
		message += "\r\n";
	
	GetSel(m_nStart,m_nEnd);

	m_iTotalLength = GetWindowTextLength();
	SetSel(m_iTotalLength, m_iTotalLength);
	ReplaceSel(message);
	m_iStartPos = m_iTotalLength;
	CHARFORMAT cf;

	cf.cbSize		= sizeof (CHARFORMAT);  
	cf.dwMask		= CFM_FACE | CFM_SIZE | CFM_BOLD |
 		CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT | CFM_PROTECTED | CFM_COLOR ;
	cf.dwEffects	= 0;//(unsigned long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD | CFE_ITALIC | CFE_STRIKEOUT);

	cf.crTextColor	= pkg.clrText; 
	
	cf.dwEffects = pkg.bBold ? (cf.dwEffects | CFE_BOLD) 
		:(cf.dwEffects& ~CFE_BOLD);

	cf.dwEffects = pkg.bItalic ? (cf.dwEffects | CFE_ITALIC) 
		:(cf.dwEffects& ~CFE_ITALIC);

	cf.dwEffects = pkg.bUnderLine ? (cf.dwEffects | CFE_UNDERLINE) 
		:(cf.dwEffects& ~CFE_UNDERLINE);
	
	cf.dwEffects = pkg.bStrikeOut? (cf.dwEffects | CFE_STRIKEOUT) 
		:(cf.dwEffects& ~CFE_STRIKEOUT);

	cf.dwMask |= CFM_SIZE;
	//NOTE:  Some guy deliberately fixed the fontsize in order to prevent the 
	// possible maliceious actions,like very large font size.
	//so if the size set to be larger than 16, restrict it to 16
	if(pkg.fontSize > 18)
		pkg.fontSize = 18;
	cf.yHeight = pkg.fontSize * 20;
	::lstrcpy(cf.szFaceName, (LPCTSTR)pkg.fontName);
	Message(cf);
}

CHARFORMAT CMyEdit::ShowMessage(CString strText, COLORREF clrText, BOOL bRet)
{
	//This displays any text using the console default font with optional color specified.
	//send bRet as false to NOT put return carraige at end.

	CString message;
	message = strText;

	GetSel(m_nStart,m_nEnd);

	if(bRet)//add return at end
		message += "\r\n";

	m_iTotalLength = GetWindowTextLength();
	SetSel(m_iTotalLength, m_iTotalLength);
	ReplaceSel(message);
	m_iStartPos = m_iTotalLength;
	CHARFORMAT cf;

	cf.cbSize		= sizeof (CHARFORMAT);  
	cf.dwMask		= CFM_FACE | CFM_SIZE | CFM_BOLD |
 		CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT | CFM_PROTECTED | CFM_COLOR ;
	cf.dwEffects	= 0;//(unsigned long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD | CFE_ITALIC | CFE_STRIKEOUT);

	//Set default font for console
	cf.crTextColor	= clrText;

	/*cf.dwEffects = (cf.dwEffects& ~CFE_BOLD);

	cf.dwEffects = (cf.dwEffects& ~CFE_ITALIC);

	cf.dwEffects = (cf.dwEffects& ~CFE_UNDERLINE);
	
	cf.dwEffects = (cf.dwEffects& ~CFE_STRIKEOUT);*/

	cf.dwMask |= CFM_SIZE;

	cf.yHeight = 10 * 20;//default console font

	::lstrcpy(cf.szFaceName, "Lucida Console");
	
	Message(cf);
	return cf;
}

void CMyEdit::Message(CHARFORMAT& cf)
{
	//Edits the selected text and scrolls down, also return user selection back
	m_iEndPos = GetWindowTextLength();
	SetSel(m_iStartPos, m_iEndPos);

	SetSelectionCharFormat(cf);
	//SetSel(-1, -1);
	Hyperlinks(cf);

	m_iOldLine = m_iNewLine;	//Memorize last line before adding text
	m_iNewLine = GetLineCount();//Find new last line
	int diff = m_iNewLine - m_iOldLine;
	/*if(diff > 8)
		diff*=.8;	
	if(diff > 3)
		diff*=.6;*/
	LineScroll(diff);//);//Scroll the difference
	SetSel(m_nStart,m_nEnd);//set user selection back
}

void CMyEdit::Hyperlinks(CHARFORMAT &cf)
{
	CString strText, strTemp;
	int iStart = 0;
	int i = 0;
	strText = GetSelText();//get the newly added text only

	while(TRUE)
	{
		iStart = strText.Find("http://",i);
		if(iStart == -1)//try again
		{
			iStart = strText.Find("https://",i);
			if(iStart == -1)//try again
			{
				iStart = strText.Find("www.",i);
				if(iStart == -1)//try again
				{
					iStart = strText.Find("ftp://",i);
					if(iStart == -1)//try again
						iStart = strText.Find("mailto:",i);
				}
			}
		}

		if(iStart != -1)//found one
		{
			for(i = iStart;i < strText.GetLength();i++)
			{
				if(strText.GetAt(i) == ' ')
					break;
				else if(strText.GetAt(i) == 10)
					break;
			}
			//DEBUG:
			//strTemp.Format("Hyperlink:\niStart: %i\ni: %i\nStart: %i\nEnd: %i",iStart,i,m_iStartPos+iStart,m_iStartPos+i);
			//MessageBox(strTemp);
		
			//Set font for hyperlink
			cf.dwMask |= CFM_UNDERLINE;
			//cf.cbSize		= sizeof (CHARFORMAT);  
			//cf.dwMask		= CFM_FACE | CFM_SIZE | CFM_BOLD |
 			//	CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT | CFM_PROTECTED | CFM_COLOR ;
			cf.dwEffects	= 0;//(unsigned long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD | CFE_ITALIC | CFE_STRIKEOUT);
				cf.dwEffects = (cf.dwEffects | CFE_UNDERLINE);
			cf.crTextColor	= BLUE;
			
			SetSel(m_iStartPos+iStart,m_iStartPos+i);
 			SetSelectionCharFormat(cf);
			wHyperlinkS.Add(m_iStartPos+iStart);
			wHyperlinkE.Add(m_iStartPos+i);

		}
		else
			break;
	}
}

int CMyEdit::ClearAllText()
{
	//Clear all messages in the chatroom list.
	SetSel(0,GetTextLength());
	ReplaceSel("\r\n");
	//clears hyperlinks
	wHyperlinkS.RemoveAll();
	wHyperlinkE.RemoveAll();
	return 0;
}
