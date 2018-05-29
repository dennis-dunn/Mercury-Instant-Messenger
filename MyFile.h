// MyFile.h: interface for the CMyFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYFILE_H__A65127AC_3C76_4337_890B_8BCAA44F0D3E__INCLUDED_)
#define AFX_MYFILE_H__A65127AC_3C76_4337_890B_8BCAA44F0D3E__INCLUDED_

#define RB 100
#define WB 101
#define R 102
#define W 103

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//Started:2-6-04
//Last: 7-19-04
//Dennis Dunn
//My File V1.0

//This class is used to stream multiple files outside of the function that they are opened in 
//by using a pointer to the file (old C-style). Use the commands here to ensure that no errors
//occur, C-style has no error checking, so I added it. Ex: Closing a file that is not open
//causes an error. Use my Open() and Close() functions to prevent that from happening.
//You can still use the other C functions, but don't crash anything.

class CMyFile
{
public:
	int Delete();
	CMyFile();
	virtual ~CMyFile();

	//access private members
	BOOL IsOpen() {return m_bOpen;}
	BOOL Close();
	CString GetFlags() {return m_strFlags;}
	CString GetPath() {return m_strPath;}
	FILE *GetFile() {return fFile;}		//return handle to current file

	int Open(CString strDir, int flags);//Flags are RB, WB, R, or W
	int Copy(CString strTo, CString strFrom = "", BOOL bOverwrite = FALSE, BOOL bMove = FALSE);//
	int Move(CString strTo, CString strFrom = "", BOOL bOverwrite = FALSE);//
	int Delete(CString strDir = "");	//Delete a file
	int Check(CString strDir);			//Check if file already exists
	int PutS(char *strIn);				//Add string to file
	int GetS(char *strOut);				//Read string from file
	int LineOut(char *strText);			//Adds a text string and return, only for Text Files, not binary
	double GetSize(CString strDir = "");//returns size of file, or current open file, WARNING see comments!

private:
	FILE *fFile;					//The file pointer
	char Read(char*);		//??
	CString m_strPath;				//Path of writing or reading
	CString m_strFlags;				//flags for current file

	double dSize;			//Size of current file
	BOOL m_bOpen;			//is file open?
	char strRead[MAX];		//??
	
};

#endif // !defined(AFX_MYFILE_H__A65127AC_3C76_4337_890B_8BCAA44F0D3E__INCLUDED_)
