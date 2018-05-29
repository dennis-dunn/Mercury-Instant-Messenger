#if !defined(AFX_PUBPADDLG_H__888EAF15_7EF3_49AD_9AEC_185EE9C1B820__INCLUDED_)
#define AFX_PUBPADDLG_H__888EAF15_7EF3_49AD_9AEC_185EE9C1B820__INCLUDED_

#include "Line.h"	// Added by ClassView
#include "toolboxdlg.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PubPadDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPubPadDlg dialog

class CPubPadDlg : public CDialog
{
// Construction
public:
	int m_iSkip;
	CPoint m_ptOffset;
	HICON m_hIcon;
	int m_iCursor;
	int m_iBar;//Height, in pixels, of a single-line menu bar.
	int m_iFrame;//Width and height, in pixels, of a window border.
	void SetResCur(unsigned short ResID);
	CString m_strTemp;
	int m_iLines;
	CLine* GetLine(int index);
	CObArray m_Lines;
	CPen m_pen;
	CPen* m_pOldPen;
	CDC *m_pDC;
	int m_iThick;
	COLORREF m_clr;
	BOOL m_bClicked;
	BOOL m_bFirst;
	CPoint m_pLast;
	CDialog *m_pWnd;
	CPubPadDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CPubPadDlg)
	enum { IDD = IDD_PUBPAD };
	CComboBox	m_ctlThick;
	BOOL m_bToolBox;
	int m_iTool;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPubPadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPubPadDlg)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnColor();
	afx_msg void OnSelchangeThickness();
	afx_msg void OnPaint();
	afx_msg void OnUndo();
	afx_msg void OnRedo();
	afx_msg void OnTool();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTool2();
	afx_msg void OnTool3();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedToolwin();
	// instance of the toolbox dialog that is used for pubpad
	CToolBoxDlg m_ToolBoxDlg;
	// show/hide toolbox for pubpad
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PUBPADDLG_H__888EAF15_7EF3_49AD_9AEC_185EE9C1B820__INCLUDED_)
