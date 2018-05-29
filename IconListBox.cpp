// IconListBox.cpp : implementation file
//

#include "stdafx.h"
#include "MIM.h"
#include "IconListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIconListBox


CIconListBox::CIconListBox()
{
	// Set the Image List Pointer to NULL - Initialization
	m_pImageList = NULL;
	bGrayText = FALSE;
}

CIconListBox::~CIconListBox()
{
	m_pImageList = NULL;
	bGrayText = FALSE;
}


BEGIN_MESSAGE_MAP(CIconListBox, CListCtrl)
	//{{AFX_MSG_MAP(CIconListBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconListBox message handlers

void CIconListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);

	CDC* pDC    = CDC::FromHandle(lpDrawItemStruct->hDC);

	if ((int)lpDrawItemStruct->itemID < 0)
	{
		// If there are no elements in the List Box 
		// based on whether the list box has Focus or not 
		// draw the Focus Rect or Erase it,
		if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && 
			(lpDrawItemStruct->itemState & ODS_FOCUS))
		{
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
		}
		else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&	
			!(lpDrawItemStruct->itemState & ODS_FOCUS)) 
		{
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem); 
		}
		return;
	}

    CRect  rcItem(lpDrawItemStruct->rcItem); // To draw the focus rect.
    CRect  rClient(rcItem); // Rect to highlight the Item
    CRect  rText(rcItem); // Rect To display the Text
    CPoint Pt( rcItem.left , rcItem.top ); // Point To draw the Image

	// if the Image list exists for the list box
	// adjust the Rect sizes to accomodate the Image for each item.
	if(m_pImageList)
	{
		//Since I had not intension to add the icon so far,
		// I commented out the following line, if you want 
		// to add icon by yourself, uncomment it to right shift the text.
		//rText.left += 34;
		rText.top += 2;
	}
	else
	{
		rText.top += 2;
	}


	COLORREF crText;
	CString strText;

	// Image information in the item data.
	int iImg = (int)lpDrawItemStruct->itemData;

	// If item selected, draw the highlight rectangle.
	// Or if item deselected, draw the rectangle using the window color.
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
		 (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		CBrush br(::GetSysColor(COLOR_HIGHLIGHT));
		pDC->FillRect(&rClient, &br);
	}
	else if (!(lpDrawItemStruct->itemState & ODS_SELECTED) && 
		(lpDrawItemStruct->itemAction & ODA_SELECT)) 
	{
		CBrush br(::GetSysColor(COLOR_WINDOW));
		pDC->FillRect(&rClient, &br);
	}

	// If the item has focus, draw the focus rect.
	// If the item does not have focus, erase the focus rect.
	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && 
		(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		pDC->DrawFocusRect(&rcItem); 
	}
	else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&	
		!(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		pDC->DrawFocusRect(&rcItem); 
	}

	// To draw the Text set the background mode to Transparent.
	int iBkMode = pDC->SetBkMode(TRANSPARENT);

//	if (bGrayText) lpDrawItemStruct->itemState | ODS_DISABLED;

	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		crText = pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	else if (lpDrawItemStruct->itemState & ODS_DISABLED || bGrayText)
		crText = pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
	else
		crText = pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));

	// Get the item text.
	strText = GetItemText(lpDrawItemStruct->itemID,0);

	// Setup the text format.
	UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	if (GetStyle() & LBS_USETABSTOPS)
		nFormat |= DT_EXPANDTABS;
	

	// if the ImageList is Existing and there is an associated Image
	// for the Item, draw the Image.
	if(m_pImageList && (iImg != -1 ) )
		m_pImageList->Draw(pDC,iImg,Pt,ILD_NORMAL);
	
	//Draw the Text
	pDC->DrawText(strText, -1, &rText, nFormat | DT_CALCRECT);
	pDC->DrawText(strText, -1, &rText, nFormat);

	pDC->SetTextColor(crText); 
	pDC->SetBkMode(iBkMode);

}

void CIconListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your code to determine the size of specified item
		lpMeasureItemStruct->itemHeight = 36;
//		lpMeasureItemStruct->itemHeight = 16;//::GetSystemMetrics(SM_CYICON) + 4;
}

int CIconListBox::AddString(LPCTSTR lpszItem)
{
	int iRet = CListCtrl::InsertItem(GetItemCount(),lpszItem);
	if (iRet >= 0)
		SetItemData(iRet, -1);
	return iRet;
}

int CIconListBox::AddString(LPCTSTR lpszItem, int iImg)
{
	int iRet = CListCtrl::InsertItem(GetItemCount(),lpszItem);
	if (iRet >= 0)
		SetItemData(iRet, iImg);
	return iRet;
}

int CIconListBox::AddString(LPCTSTR lpszItem, int iImg,BOOL bAway)
{
	bGrayText = bAway;
	return AddString(lpszItem,iImg);
}

int CIconListBox::InsertString(int iIndex, LPCTSTR lpszItem)
{
	int iRet = CListCtrl::InsertItem(iIndex,lpszItem);
	if (iRet >= 0)
		SetItemData(iRet, -1);
	return iRet;
}

int CIconListBox::InsertString(int iIndex, LPCTSTR lpszItem, int iImg)
{
	int iRet = CListCtrl::InsertItem(iIndex,lpszItem,iImg);
	/*if (iRet >= 0)
		SetItemData(iRet, iImg);*/
	return iRet;
}

void CIconListBox::SetItemImage(int iIndex, int iImg)
{
	SetItemData(iIndex, iImg);
	RedrawWindow();
}

