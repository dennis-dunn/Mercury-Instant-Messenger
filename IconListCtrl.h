#if !defined(AFX_ICONLISTCTRL_H__04FE292C_E2C1_45DE_8D02_5487C9D7FAB9__INCLUDED_)
#define AFX_ICONLISTCTRL_H__04FE292C_E2C1_45DE_8D02_5487C9D7FAB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IconListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIconListCtrl window

//NOTE: this list is only useful for 1 column with images...
class CIconListCtrl : public CListCtrl
{
// Construction
public:
	CIconListCtrl();
// Attributes
public:

// Attributes
protected:
	CImageList* m_pImageList;
	BOOL bGrayText;
// Operations
public:
	CImageList* SetImageList(CImageList* pImageList, int nImageList);
	int AddString(LPCTSTR lpszItem);
	int AddString(LPCTSTR lpszItem, int iImg);
	int AddString(LPCTSTR lpszItem, int iImg,BOOL bAway);
	int InsertString(int iIndex, LPCTSTR lpszItem);
	int InsertString(int iIndex, LPCTSTR lpszItem, int iImg);
	void SetItemImage(int iIndex, int iImg);
	void SetParent(CDialog *pWnd);
	CDialog *m_pWnd;
	/*inline void SetImageList(CImageList* pImgList = NULL)
	{ m_pImageList = pImgList; }*/
private:
	CString ExpandShortcut(CString& csFilename) const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconListCtrl)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIconListCtrl();
	BOOL ValidatePathname(const CString& csPathname, UINT& iPathType) const;

	// Generated message map functions
protected:
	//{{AFX_MSG(CIconListCtrl)
	afx_msg void OnDropFiles(HDROP dropInfo);
	//}}AFX_MSG
	//{{AFX_MSG(CFileDropListCtrl)

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONLISTCTRL_H__04FE292C_E2C1_45DE_8D02_5487C9D7FAB9__INCLUDED_)
