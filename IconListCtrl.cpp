// IconListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MIM.h"
#include "MIMDlg.h"
#include "IconListCtrl.h"
#include <sys/stat.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIconListCtrl::CIconListCtrl()
{
	// Set the Image List Pointer to NULL - Initialization
	m_pImageList = NULL;
	bGrayText = FALSE;
}

CIconListCtrl::~CIconListCtrl()
{
	m_pImageList = NULL;
	bGrayText = FALSE;
}


BEGIN_MESSAGE_MAP(CIconListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CIconListCtrl)
	ON_WM_DROPFILES()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconListCtrl message handlers

void CIconListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	//MessageBox("IconList DrawItem()");
	// TODO: Add your code to draw the specified item
	//ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

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
		//Since I had not intended to add the icon so far,
		// I commented out the following line, if you want 
		// to add icon by yourself, uncomment it to right shift the text.
		rText.left += 42;
	}
	rText.top += 2;

	COLORREF crText;
	CString strText;

	// Image information in the item data.
	int iImg = (int)lpDrawItemStruct->itemData;

	// If item selected, draw the highlight rectangle.
	// Or if item deselected, draw the rectangle using the window color.
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
		 (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		CBrush br(RGB(0,255,0));
		//CBrush br(::GetSysColor(COLOR_HIGHLIGHT));
		pDC->FillRect(&rClient, &br);
	}
	else
	{
		CBrush br(RGB(0,0,0));
		//CBrush br(::GetSysColor(COLOR_WINDOW));
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
		crText = pDC->SetTextColor(RGB(0,0,0));
		//crText = pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	else if (lpDrawItemStruct->itemState & ODS_DISABLED || bGrayText)
		crText = pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
	else
		crText = pDC->SetTextColor(RGB(0,255,0));
		//crText = pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));

	// Get the item text.
	strText = GetItemText(lpDrawItemStruct->itemID,0);
	//MessageBox(strText);

	// Setup the text format.
	UINT nFormat = DT_LEFT;
	//| DT_WORDBREAK| DT_SINGLELINE | DT_VCENTER;
	if (GetStyle() & LBS_USETABSTOPS)
		nFormat |= DT_EXPANDTABS;

	// if the ImageList is Existing and there is an associated Image
	// for the Item, draw the Image.
	if(!m_pImageList)
		MessageBox("No m_pImageList");
	if(m_pImageList && (iImg != -1 ) )
		m_pImageList->Draw(pDC,iImg,Pt,ILD_NORMAL);
	
	//Draw the Text
	//pDC->DrawText(strText, -1, &rText, nFormat | DT_CALCRECT);
	pDC->DrawText(strText, -1, &rText, nFormat);
	pDC->SetTextColor(crText); 
	pDC->SetBkMode(iBkMode);
}

void CIconListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your code to determine the size of specified item
		lpMeasureItemStruct->itemHeight = 36;
//		lpMeasureItemStruct->itemHeight = 16;//::GetSystemMetrics(SM_CYICON) + 4;
}

int CIconListCtrl::AddString(LPCTSTR lpszItem)
{
	int iRet = CListCtrl::InsertItem(GetItemCount(),lpszItem);
	if (iRet >= 0)
		SetItemData(iRet, -1);
	return iRet;
}

int CIconListCtrl::AddString(LPCTSTR lpszItem, int iImg)
{
	int iRet = CListCtrl::InsertItem(GetItemCount(),lpszItem,iImg);
	if (iRet >= 0)
		SetItemData(iRet, iImg);
	return iRet;
}

int CIconListCtrl::AddString(LPCTSTR lpszItem, int iImg,BOOL bAway)
{
	bGrayText = bAway;
	return AddString(lpszItem,iImg);
}

int CIconListCtrl::InsertString(int iIndex, LPCTSTR lpszItem)
{
	int iRet = CListCtrl::InsertItem(iIndex,lpszItem);
	if (iRet >= 0)
		SetItemData(iRet, -1);
	return iRet;
}

int CIconListCtrl::InsertString(int iIndex, LPCTSTR lpszItem, int iImg)
{
	int iRet = CListCtrl::InsertItem(iIndex,lpszItem,iImg);
	if (iRet >= 0)
		SetItemData(iRet, iImg);
	return iRet;
}

void CIconListCtrl::SetItemImage(int iIndex, int iImg)
{
	SetItemData(iIndex, iImg);
	RedrawWindow();
}

CString CIconListCtrl::ExpandShortcut(CString& csFilename) const
{
	USES_CONVERSION;		// For T2COLE() below
	CString csExpandedFile;

	//
    // Make sure we have a path
	//
	if(csFilename.IsEmpty())
	{
		ASSERT(FALSE);
		return csExpandedFile;
	}

	//
    // Get a pointer to the IShellLink interface
	//
    HRESULT hr;
    IShellLink* pIShellLink;

    hr = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
							IID_IShellLink, (LPVOID*) &pIShellLink);

    if (SUCCEEDED(hr))
    {
        // Get a pointer to the persist file interface
		IPersistFile* pIPersistFile;
        hr = pIShellLink->QueryInterface(IID_IPersistFile, (LPVOID*) &pIPersistFile);

        if (SUCCEEDED(hr))
        {
            // Load the shortcut and resolve the path
			//
            // IPersistFile::Load() expects a UNICODE string
			// so we're using the T2COLE macro for the conversion
			//
			// For more info, check out MFC Technical note TN059
			// (these macros are also supported in ATL and are
			// so much better than the ::MultiByteToWideChar() family)
            hr = pIPersistFile->Load(T2COLE(csFilename), STGM_READ);
			
			if (SUCCEEDED(hr))
			{
				WIN32_FIND_DATA wfd;
				hr = pIShellLink->GetPath(csExpandedFile.GetBuffer(MAX_PATH),
										  MAX_PATH,
										  &wfd,
										  SLGP_UNCPRIORITY);

				csExpandedFile.ReleaseBuffer(-1);
            }
            pIPersistFile->Release();
        }
        pIShellLink->Release();
    }

    return csExpandedFile;
}

void CIconListCtrl::OnDropFiles(HDROP dropInfo)
{
	if((((CMIMDlg*)m_pWnd)->m_pClientSocket[0] == NULL) && (((CMIMDlg*)m_pWnd)->m_iType == CLIENT))
		return;
	if((((CMIMDlg*)m_pWnd)->m_pListenSocket == NULL) && (((CMIMDlg*)m_pWnd)->m_iType == SERVER))
		return;

	int sel = GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);

	if(sel < 0)//not admin or not selected
	{
		MessageBox("Select a person to send a file first.", "File Sending", MB_OK | MB_ICONEXCLAMATION);	
		return;
	}

	int ID = ((CMIMDlg*)m_pWnd)->GetID(GetItemText(sel,0));
	((CMIMDlg*)m_pWnd)->m_iTotal = 0;
	((CMIMDlg*)m_pWnd)->m_strFileDirs.RemoveAll();
	((CMIMDlg*)m_pWnd)->m_strFileNames.RemoveAll();

	if((sel > 0)&&(((CMIMDlg*)m_pWnd)->m_iType == SERVER))//server send to client
	{
		if(ID < 0)
		{
			((CMIMDlg*)m_pWnd)->m_wndShow.ShowMessage("<!> Invalid number or range.");
			return;
		}
	}
	else if((((CMIMDlg*)m_pWnd)->m_iType == CLIENT)&&(ID != ((CMIMDlg*)m_pWnd)->m_pClientSocket[0]->m_iClientID))//cant send to self
	{
		;//ok
	}
	else
	{
		MessageBox("Cant send file(s) to that user.", "Sending Files", MB_OK | MB_ICONEXCLAMATION);	
		return;
	}

	// Get the number of pathnames (files or folders) dropped
	UINT nNumFilesDropped = DragQueryFile(dropInfo, 0xFFFFFFFF, NULL, 0);

	// Iterate through the pathnames and process each one
	TCHAR szFilename[MAX_PATH + 1];
	CString csPathname, csExpandedFilename, strName, strDir;

	for (UINT nFile = 0 ; nFile < nNumFilesDropped; nFile++)
	{
		// Get the pathname
		DragQueryFile(dropInfo, nFile, szFilename, MAX_PATH + 1);

		// It might be shortcut, so try and expand it
		csPathname = szFilename;
		csExpandedFilename = ExpandShortcut(csPathname);
		if(!csExpandedFilename.IsEmpty())
		{
			csPathname = csExpandedFilename;
		}

		// Now see if its something we allow to be dropped
		UINT iPathType = 0;
		if(ValidatePathname(csPathname, iPathType))
		{
			// By default, we insert the filename into the list
			// ourselves, but if our parent wants to do something flashy
			// with it (maybe get the filesize and insert that as an extra
			// column), they will have installed a callback for each
			// droppped item
			/*if(m_dropMode.iMask & DL_USE_CALLBACK)
			{
				// Let them know about this list control and the item
				// droppped onto it
				if(m_dropMode.pfnCallback)
					m_dropMode.pfnCallback(this, csPathname, iPathType);
			}
			else*/
			((CMIMDlg*)m_pWnd)->AddFile(csPathname);
		}
	}

	// Free the dropped-file info that was allocated by windows
	DragFinish(dropInfo);
	((CMIMDlg*)m_pWnd)->SendFiles(ID);
}

BOOL CIconListCtrl::ValidatePathname(const CString& csPathname, UINT& iPathType) const
{
	// Get some info about that path so we can filter out dirs
	// and files if need be
	//
	BOOL bValid = FALSE;

	struct _stat buf;
	int result = _tstat( csPathname, &buf );
	if( result == 0 )
	{
		// Do we have a directory? (if we want dirs)
		if ((buf.st_mode & _S_IFDIR) == _S_IFDIR)
	    {
			//ITS A FOLDER!
			bValid = FALSE;
			//iPathType = DL_FOLDER_TYPE;
		} 
	    else if ((buf.st_mode & _S_IFREG) == _S_IFREG)
	    {
			// We've got a file and files are allowed.
			//iPathType = DL_FILE_TYPE;
			bValid = TRUE;
		}
	}
	return bValid;
}

void CIconListCtrl::SetParent(CDialog *pWnd)
{
	//Set m_Pwnd to point to the address of the CMIMDlg class
	m_pWnd = pWnd;
}

CImageList* CIconListCtrl::SetImageList(CImageList* pImageList, int nImageList)
{
	m_pImageList = pImageList;
	return CListCtrl::SetImageList(pImageList, nImageList);
}
