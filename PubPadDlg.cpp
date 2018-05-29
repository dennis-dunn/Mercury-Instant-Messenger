// PubPadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MIM.h"
#include "PubPadDlg.h"
#include "MIMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPubPadDlg dialog


CPubPadDlg::CPubPadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPubPadDlg::IDD, pParent)
	, m_bToolBox(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);//Icon for tray
	//{{AFX_DATA_INIT(CPubPadDlg)
	m_bToolBox = TRUE;
	//}}AFX_DATA_INIT
}


void CPubPadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPubPadDlg)
	DDX_Control(pDX, IDC_THICKNESS, m_ctlThick);
	DDX_Radio(pDX, IDC_TOOL, m_bToolBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPubPadDlg, CDialog)
	//{{AFX_MSG_MAP(CPubPadDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	ON_CBN_SELCHANGE(IDC_THICKNESS, OnSelchangeThickness)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_UNDO, OnUndo)
	ON_BN_CLICKED(IDC_REDO, OnRedo)
	ON_BN_CLICKED(IDC_TOOL, OnTool)
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_TOOL2, OnTool2)
	ON_BN_CLICKED(IDC_TOOL3, OnTool3)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_TOOLWIN, &CPubPadDlg::OnBnClickedToolwin)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPubPadDlg message handlers

BOOL CPubPadDlg::OnInitDialog() 
{
	m_bClicked = FALSE;
	CDialog::OnInitDialog();
	
	if(m_bFirst)
	{
		// Set the icon for this dialog.  The framework does this automatically
		//  when the application's main window is not a dialog
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon (tray)
		m_iCursor = IDC_PENCIL;
		m_iLines = 0;
		m_ptOffset = CPoint(0,0);
		SetScrollRange(SB_HORZ,0,4000);
		SetScrollRange(SB_VERT,0,4000);
		m_iBar = GetSystemMetrics(SM_CYMENU);//Height, in pixels, of a single-line menu bar.
		m_iFrame = GetSystemMetrics(SM_CXBORDER)+GetSystemMetrics(SM_CXFIXEDFRAME);//Width and height, in pixels, of a window border.
		m_ctlThick.AddString("1 pt");
		m_ctlThick.AddString("2 pt");
		m_ctlThick.AddString("3 pt");
		m_ctlThick.AddString("4 pt");
		m_ctlThick.AddString("5 pt");
		m_ctlThick.SetCurSel(0);
		m_iThick = 1;
		m_clr = RGB(0,0,0);
		m_bFirst = FALSE;
		m_iTool = 0;
		m_bToolBox = TRUE;
		//create the toolbox
		if(m_ToolBoxDlg.m_bFirst)
			m_ToolBoxDlg.Create(IDD_TOOLBOX, this);
		m_ToolBoxDlg.m_pWnd = this;
		m_ToolBoxDlg.ShowWindow(true);//show toolbox
		UpdateData(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPubPadDlg::OnClose() 
{
	((CMIMDlg*)m_pWnd)->m_bPubPad = FALSE;//tell MIM window that pubpad closed.
	m_ToolBoxDlg.ShowWindow(false);
	CDialog::OnClose();
}

void CPubPadDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_iSkip = 1;
	//convert relative to drawing surface 
	point.y += (m_iBar+m_iFrame);
	point.x += m_iFrame;

	//FIX: Look in GetSystemMetrics() for title bar height
	if(m_bClicked)
		return;
	m_bClicked = TRUE;
	m_pLast = point;

	if(m_iTool <= 1)//pencil/line
	{
		if(m_iLines < m_Lines.GetSize())//erase undo data!
		{
			for(int i = m_iLines; i < m_Lines.GetSize();i++)
			{
				delete GetLine(i);
				m_Lines.RemoveAt(m_iLines);//keep deleting this same index as they shift left
			}
			m_Lines.SetSize(m_iLines);
		}

		if((m_iLines <= 0) || (GetLine(m_iLines-1)->m_Points.GetSize() > 0))//if no previous line, or previous line isn't empty
			m_Lines.Add(new CLine(m_iThick, m_clr));
		else//overwrite last line (empty already?)
		{
			GetLine(m_iLines-1)->m_clr = m_clr;
			GetLine(m_iLines-1)->m_iThick = m_iThick;
		}
		m_iLines = m_Lines.GetSize();

		m_pDC = GetWindowDC();
		m_pen.CreatePen(PS_SOLID,m_iThick,m_clr);
		m_pOldPen = m_pDC->SelectObject(&m_pen);
		CPoint logical(point.x+m_ptOffset.x, point.y+m_ptOffset.y);
		GetLine(m_iLines-1)->m_Points.Add(new CMyPoint(logical));
		m_pDC->MoveTo(point);
		m_pDC->LineTo(point);
		//keep DC ready for OnMouseMove()
	}
	else if(m_iTool == 2)//eraser
	{
		//figure out which line (index) wants to be erased.
		//then move that line to the end of the array and decrememnt m_iLines
		//m_iLines--;
		//Invalidate();
	}
	GetDlgItem(IDC_REDO)->EnableWindow(FALSE);
	GetDlgItem(IDC_REDO)->ShowWindow(FALSE);
	/*CString strTemp;
	strTemp.Format("m_iLines = %i",m_iLines);
	MessageBox(strTemp);*/

	CDialog::OnLButtonDown(nFlags, point);
}

void CPubPadDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_iSkip++;
	//convert relative to drawing surface 
	point.y += (m_iBar+m_iFrame);
	point.x += m_iFrame;

	if(m_bClicked)
	{
		//m_pDC->MoveTo(m_pLast);
		if(m_iTool == 0)//pencil
		{
			if(m_iSkip >= 6)
			{
				m_pLast = point;
				m_pDC->LineTo(point);
				CPoint logical(point.x+m_ptOffset.x, point.y+m_ptOffset.y);

				GetLine(m_iLines-1)->m_Points.Add(new CMyPoint(logical));
				m_iSkip = 0;
			}
		}
		else if(m_iTool == 1)//line
		{
			m_pDC->MoveTo(m_pLast);
			//Invalidate();
			m_pDC->LineTo(point);
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CPubPadDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(!m_bClicked)
		return;
	m_bClicked = FALSE;
	if(m_iTool <= 1)//pencil
	{
		m_pDC->SelectObject(&m_pOldPen);
		m_pen.DeleteObject();
		ReleaseDC(m_pDC);
		GetDlgItem(IDC_UNDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_UNDO)->ShowWindow(TRUE);
		if(m_iTool <= 1)//line/pencil tool
		{
			//convert relative to drawing surface 
			point.y += (m_iBar+m_iFrame);
			point.x += m_iFrame;

			m_pDC->LineTo(point);
			m_pLast = point;
			CPoint logical(point.x+m_ptOffset.x, point.y+m_ptOffset.y);
			GetLine(m_iLines-1)->m_Points.Add(new CMyPoint(logical));
			Invalidate();
		}
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CPubPadDlg::OnColor()
{
	CColorDialog dlg;

	if(dlg.DoModal() == IDOK )
		m_clr = dlg.GetColor();
}

void CPubPadDlg::OnSelchangeThickness() 
{
	m_iThick = m_ctlThick.GetCurSel()+1;	
}

void CPubPadDlg::OnPaint() 
{
	int x,y;
	CSize szLimits;
	szLimits.cy = m_iBar+m_iFrame+20;
	szLimits.cx = m_iFrame;

	int sum = 0;
	for(int k = 0;k < m_iLines;k++)
	{
		sum += GetLine(k)->m_Points.GetSize()*sizeof(CMyPoint);
	}
	m_strTemp.Format("PuPad (%i lines, %i bytes, %i CPoint)",m_iLines, sum, sizeof(CMyPoint));
	SetWindowText(m_strTemp);

	CPaintDC dc(this); // device context for painting
	if((m_iLines <= 0) || (m_bClicked))
		return;
	for(int j = 0;j < m_iLines;j++)//cycle through lines
	{
		int iSize = GetLine(j)->m_Points.GetSize();
		
		if(iSize <= 0)
			break;
		//CString strTemp;
		/*strTemp.Format("Line (%i)\nPoints = %i",j,iSize);
		MessageBox(strTemp);*/
		m_pDC = GetWindowDC();
		m_pen.CreatePen(PS_SOLID,GetLine(j)->m_iThick,GetLine(j)->m_clr);
		m_pOldPen = m_pDC->SelectObject(&m_pen);
		m_pDC->MoveTo(int(GetLine(j)->GetPoint(0)->x)-m_ptOffset.x,int(GetLine(j)->GetPoint(0)->y)-m_ptOffset.y);

		for (int i = 0;i < iSize;i++)//cycle through points
		{
			x = int(GetLine(j)->GetPoint(i)->x)-m_ptOffset.x;
			y = int(GetLine(j)->GetPoint(i)->y)-m_ptOffset.y;
			if(x > szLimits.cx)
			{
				if(y > szLimits.cy)
					m_pDC->LineTo(x,y);
				else
					m_pDC->MoveTo(x,szLimits.cy);
			}
			else if(y > szLimits.cy)
				m_pDC->MoveTo(szLimits.cx,y);
		}
		m_pDC->SelectObject(&m_pOldPen);
		m_pen.DeleteObject();
		ReleaseDC(m_pDC);
	}
	// Do not call CDialog::OnPaint() for painting messages
}

CLine* CPubPadDlg::GetLine(int index)
{
	//return item at index, not pointer
	return (CLine*)m_Lines[index];
}

void CPubPadDlg::OnUndo() 
{
	//m_Lines.RemoveAt(m_iLines-1);
	m_iLines--;//decrease total number, but dont actually delete them until they are overwritten
	if(m_iLines == 0)
	{
		GetDlgItem(IDC_UNDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_UNDO)->ShowWindow(FALSE);
	}
	Invalidate();
	GetDlgItem(IDC_REDO)->EnableWindow(TRUE);
	GetDlgItem(IDC_REDO)->ShowWindow(TRUE);
}

void CPubPadDlg::OnRedo() 
{
	m_iLines++;
	if(m_iLines == m_Lines.GetSize())
	{
		GetDlgItem(IDC_REDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_REDO)->ShowWindow(FALSE);
	}
	Invalidate();
	GetDlgItem(IDC_UNDO)->EnableWindow(TRUE);
	GetDlgItem(IDC_UNDO)->ShowWindow(TRUE);
}

void CPubPadDlg::OnTool() 
{
	UpdateData(TRUE);
	m_iCursor = IDC_PENCIL;
	SetResCur(IDC_PENCIL);
}

void CPubPadDlg::OnTool2() 
{
	UpdateData(TRUE);
	m_iCursor = IDC_LINE;
	SetResCur(IDC_LINE);
}

void CPubPadDlg::OnTool3() 
{
	UpdateData(TRUE);
	m_iCursor = IDC_ERASER;
	SetResCur(IDC_ERASER);
}

void CPubPadDlg::SetResCur(unsigned short ResID)
{
	HCURSOR lhCursor;
	lhCursor = AfxGetApp()->LoadCursor(ResID);
	SetCursor(lhCursor);
}

BOOL CPubPadDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	SetResCur(m_iCursor);
	return FALSE;//keep custom cursor showing	
	//return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CPubPadDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	//short x = nPos;
	//ScrollWindow(m_ptOffset.x-nPos,0);
	/*CString strTemp;
	strTemp.Format("nPos=%i\ncode=%i",nPos,nSBCode);
	MessageBox(strTemp);*/
	//SetScrollRange(SB_HORZ,0,4000);
	if(nSBCode == SB_PAGERIGHT)
		m_ptOffset.x += 100;
	else if(nSBCode == SB_PAGELEFT)
		m_ptOffset.x -= 100;
	else if(nSBCode == SB_LINERIGHT)
		m_ptOffset.x += 10;
	else if(nSBCode == SB_LINELEFT)
		m_ptOffset.x -= 10;
	else if(nSBCode == SB_THUMBPOSITION)
		m_ptOffset.x = nPos;
	else
		return;

	if(m_ptOffset.x < 0)
		m_ptOffset.x = 0;
	else if(m_ptOffset.x > 4000)
		m_ptOffset.x = 4000;

	SetScrollPos(SB_HORZ, m_ptOffset.x);
	Invalidate();
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPubPadDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(nSBCode == SB_PAGEUP)
		m_ptOffset.y -= 100;
	else if(nSBCode == SB_PAGEDOWN)
		m_ptOffset.y += 100;
	else if(nSBCode == SB_LINEUP)
		m_ptOffset.y -= 10;
	else if(nSBCode == SB_LINEDOWN)
		m_ptOffset.y += 10;
	else if(nSBCode == SB_THUMBPOSITION)
		m_ptOffset.y = nPos;
	else
		return;

	if(m_ptOffset.y < 0)
		m_ptOffset.y = 0;
	else if(m_ptOffset.y > 4000)
		m_ptOffset.y = 4000;

	SetScrollPos(SB_VERT, m_ptOffset.y);
	Invalidate();
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPubPadDlg::OnBnClickedToolwin()
{
	m_bToolBox = !m_bToolBox;
	m_ToolBoxDlg.ShowWindow(m_bToolBox);
	//UpdateData(FALSE);
}
