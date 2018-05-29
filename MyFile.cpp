// CMyFile.cpp: implementation of the CCMyFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIM.h"
#include "MyFile.h"
#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//DESCRIPTION:
//  This class was made because the newer File classes (fstream and CFile)
//  Dont allow files to remain open after a function closes. The old C-style
//  File class uses pointers to streams, allowing the streams to remain open
//  after the function closes. The problem is that the old C-Style class
//  does not check to see if files are open or closed before trying to do
//  operations. Use the functions provided in this class to handle those problems
//	EX: use "CCMyFile::Close()" instead of "fclose(File)" so that if the file
//  is already closed, the program won't crash. Same with CCMyFile::Open() and fopen(*)

//Note: Some functions dont require a file to be open for their use. Ex: Copy(),
//	Remove(), Move(), Check() just need a file name to work. Leaving the parameter
//	empty will use the current open file it there is one.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyFile::CMyFile()
{
	m_bOpen = FALSE;
	fFile = NULL;
}

CMyFile::~CMyFile()
{

}

char CMyFile::Read(char *strIn)
{
	//USELESS FUNCTION SO FAR
	//save string to class member
	int i = 0;
	for(;i < MAX-2;i++)
	{
		if(strIn[i] != NULL)
			strRead[i] = strIn[i];
		else
			break;
	}
	strRead[i] = NULL;//last char
	return 0;
}

int CMyFile::Open(CString strDir, int flags)
{
	//Opens a file using the pointer "fFile".
	//Only one file open per CMyFile instance, use an array for multiple files.

	//Flags:
	//W (103) = "w": write text file
	//R (102) = "r": read text file
	//WB (101) = "w+b": write binary file
	//RB (100) = "r+b": read binary file

	char flag[4];
	if(m_bOpen)
		return -1;//stream is already open

	if(flags == WB)
		strcpy(flag, "w+b");
	else if(flags == RB)
		strcpy(flag, "r+b");
	else if(flags == W)
		strcpy(flag, "w");
	else if(flags == R)
		strcpy(flag, "r");
	else
		return -2;//bad flag param

	//set global vars
	m_strPath = strDir;
	m_strFlags = flag;
	if((fFile = fopen(LPCTSTR(m_strPath), flag)) == NULL)//error
	{
		//NOTE: DONT CLOSE FILE IF IT NEVER OPENED CORRECTLY!
		fFile = NULL;
		m_bOpen = FALSE;
		return -3;//cant open file, may already be in use by another program
	}
	else//opened OK
	{
		m_bOpen = TRUE;//File is open now, ready for streaming. Use fFile for operations.
		return 0;
	}
}

int CMyFile::Check(CString strDir)
{
	//Checks if a file with path "strDir" exists. File Stream must be closed.
	//returns:
	// 1 = YES
	// 0 = NO
	//-1 = stream is already open
	//-2 = bad flag for Open()
	//-3 = cant open file, may already be open
	//??check for read only??

	if(m_bOpen)
		return -1;//stream is already open

	int err = Open(strDir, RB);
	if(err == 0)//no error
	{
		//Overwrite prompt:
		Close();//close test file
		return 1;//file exists
	}
	else if(err == -3)
		return 0;//Doesnt exist, or error
	else
		return err;	//NOTE: DONT CLOSE FILE IF IT NEVER OPENED CORRECTLY!
}

BOOL CMyFile::Close()
{
	//Closes file if one is open
	m_strPath.Empty();
	m_strFlags.Empty();

	//closes file
	if(m_bOpen)
	{
		fclose(fFile);
		fFile = NULL;
		m_bOpen = FALSE;
	}
	else
		return FALSE;//no file open
	return TRUE;//successfully closed file
}

int CMyFile::PutS(char *strIn)
{
	//Add a string (stream) to the end of the open file
	if(m_bOpen)
	{
		fputs(strIn, fFile);
		if(feof(fFile) != 0)
			return -2;//EOF
		else
			return 0;//normal
	}
	return -1;//no file open
}

int CMyFile::LineOut(char *strText)
{
	//NOTE: return chars only work if the file is opened in "W" flag mode. Binary wont work.
	//Add a string of text to the end of an open file, usually for LOG files
	if((m_bOpen)/* && (m_strFlags == "W")*/)
	{
		fprintf(fFile, "%s\n", strText);
		if(feof(fFile) != 0)
			return -2;//EOF
		else
			return 0;//normal
	}
	return -1;//no file open, or wrong flags for writing text
}

int CMyFile::GetS(char *strOut)
{
	//Retreive a string of size MAX-2 from the current position in file
	if(m_bOpen)
	{
		fgets(strOut, MAX-2, fFile);
		if(feof(fFile) != 0)
			return -2;//EOF
		else
			return 0;//normal
	}
	return -1;//no file open
}

double CMyFile::GetSize(CString strDir)//Overloaded function
{
	if(strDir != "")//if no file is open, but user wants to open one and check its size
	{
		if(m_bOpen)//Tried to check size of a file while stream was already open
			return -2;
		Open(strDir, RB);//If it didnt open correctly, it will return at next if statement
		//NOTE: closes file at end 
	}

	//returns the size (in Bytes) of the file that is currently open
	if(!m_bOpen)
		return -1;

	ifstream inFile(LPCTSTR(m_strPath), ios::in | ios::binary);
	if(!inFile)
	{
		inFile.close();
		return -1;
	}

	//GetSize of file
	double Size = 0;
	inFile.seekg(0, ios::end);
	Size = inFile.tellg();
	inFile.close();
	
	if(strDir != "")//close the file now, since it wasnt open before this function
	{
		Close();
	}

	dSize = Size;
	return Size;
}

int CMyFile::Delete(CString strDir)
{
	if((strDir == "") && (!m_bOpen))
		return -2;//no stream open, and no file name specified
	else if(m_bOpen)
	{
		strDir = m_strPath;//stream is open, but we have to close it now, and save path before it's erased by Close()
		Close();
	}
	if(remove(LPCTSTR(strDir)) == -1)
		return -1;//error

	return 0;
}

int CMyFile::Copy(CString strTo, CString strFrom, BOOL bOverwrite, BOOL bMove)
{
	char letter;

	if((strFrom == "") && (!m_bOpen))
		return -7;//no stream open, and no file name specified
	else if(m_bOpen)
	{
		strFrom = m_strPath;//stream is open, but we have to close it now, and save path before it's erased by Close()
		Close();
	}

///////
	//Check to see if source files exist
	if(Check(strFrom) < 0)//error
	{
		//CString strTemp;
		//strTemp.Format("Source copy of file \"%s\"\ndoes not exist at \"%s\".\n", strFile, strSource);
		//MessageBox(strTemp);
		//m_iError = 2;

		return -1;//No Source file, or other check error
	}
//Check to see if dest file exists "OVERWRITE"
	int err = Check(strTo);
	if(err == 1)//exists
	{
		//strTemp.Format("\nOlder version of file \"%s\"\n already exists at\"%s\".\n", strFile, strDest);
		if(!bOverwrite)//Keep older file
			return -2;
	}
	else if(err != 0)//0 means file does not exist, which is good
		return -3;//already in use error

//source stream
	ifstream in(strFrom, ios::in | ios::binary);
	if(!in)
	{
		//CString strTemp;
		//strTemp.Format("Unable to read from file \"%s\".\n", strSourcePath);
		//MessageBox(strTemp);

		//m_iError = 2;
		in.close();
		return -4;//already in use error
	}
//destination stream
	ofstream out(strTo, ios::out | ios::binary);
	if(!out)
	{
		//CString strTemp;
		//strTemp.Format("Unable to write to file \"%s\".\n", strDestPath);
		//MessageBox(strTemp);
		//m_iError = 3;

		out.close();
		return -5;//already in use error
	}

	//m_strSource.Format("%s", strSourcePath);
	//m_strDest.Format("%s", strDestPath);

	//UpdateData(FALSE);

	//copy from one to the other
	while(in.get(letter))
	{
		if(!out.put(letter))
		{
			out.close();
			in.close();
		
			//CString strTemp;
			//strTemp.Format("Cannot write to destination file \"%s\".\n", strDestPath);
			//MessageBox(strTemp);
			return -6;
		}
	}
	//Close up streams
	out.close();
	in.close();
	/////////
	if(bMove)//delete source file now
		Delete(strFrom);
	return 0;
}

int CMyFile::Move(CString strTo, CString strFrom, BOOL bOverwrite)
{
	//Moves a file to another location
	return Copy(strTo, strFrom, bOverwrite, TRUE);
}
