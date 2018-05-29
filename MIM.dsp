# Microsoft Developer Studio Project File - Name="MIM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MIM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MIM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MIM.mak" CFG="MIM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MIM - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MIM - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MIM - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "MIM - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MIM - Win32 Release"
# Name "MIM - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AgreeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Command.cpp
# End Source File
# Begin Source File

SOURCE=.\Console.cpp
# End Source File
# Begin Source File

SOURCE=.\FlatComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\IconListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\IntroDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Line.cpp
# End Source File
# Begin Source File

SOURCE=.\MBoxDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MIM.cpp
# End Source File
# Begin Source File

SOURCE=.\MIM.idl
# ADD MTL /h "MIM_i.h" /iid "MIM_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\MIM.rc
# End Source File
# Begin Source File

SOURCE=.\MIMDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MyEditIn.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\MySocket.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet.cpp
# End Source File
# Begin Source File

SOURCE=.\PasswordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Pkg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PubPadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ReferenceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Request.cpp
# End Source File
# Begin Source File

SOURCE=.\RichTextBar.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveDLDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SettingsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ShareInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AgreeDlg.h
# End Source File
# Begin Source File

SOURCE=.\Command.h
# End Source File
# Begin Source File

SOURCE=.\Console.h
# End Source File
# Begin Source File

SOURCE=.\FlatComboBox.h
# End Source File
# Begin Source File

SOURCE=.\IconListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\IntroDlg.h
# End Source File
# Begin Source File

SOURCE=.\Line.h
# End Source File
# Begin Source File

SOURCE=.\MBoxDlg.h
# End Source File
# Begin Source File

SOURCE=.\MIM.h
# End Source File
# Begin Source File

SOURCE=.\MIMDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyEdit.h
# End Source File
# Begin Source File

SOURCE=.\MyEditIn.h
# End Source File
# Begin Source File

SOURCE=.\MyFile.h
# End Source File
# Begin Source File

SOURCE=.\MyPoint.h
# End Source File
# Begin Source File

SOURCE=.\MySocket.h
# End Source File
# Begin Source File

SOURCE=.\Packet.h
# End Source File
# Begin Source File

SOURCE=.\PasswordDlg.h
# End Source File
# Begin Source File

SOURCE=.\Pkg.h
# End Source File
# Begin Source File

SOURCE=.\ProfileDlg.h
# End Source File
# Begin Source File

SOURCE=.\PubPadDlg.h
# End Source File
# Begin Source File

SOURCE=.\ReferenceDlg.h
# End Source File
# Begin Source File

SOURCE=.\Request.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RichTextBar.h
# End Source File
# Begin Source File

SOURCE=.\SaveDLDlg.h
# End Source File
# Begin Source File

SOURCE=.\ServerDlg.h
# End Source File
# Begin Source File

SOURCE=.\SettingsDlg.h
# End Source File
# Begin Source File

SOURCE=.\ShareInfo.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\64.bmp
# End Source File
# Begin Source File

SOURCE=.\res\admin.ico
# End Source File
# Begin Source File

SOURCE=.\res\away.ico
# End Source File
# Begin Source File

SOURCE=.\res\BG.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp65.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Chatroom.ico
# End Source File
# Begin Source File

SOURCE=.\res\Client.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CMess.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Connect.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\Details.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Disconnect.bmp
# End Source File
# Begin Source File

SOURCE=.\res\EditProfile.bmp
# End Source File
# Begin Source File

SOURCE=.\res\error.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon0.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon5.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon6.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\Ignore.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ip.ico
# End Source File
# Begin Source File

SOURCE=.\res\Kick.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Listen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MBox.bmp
# End Source File
# Begin Source File

SOURCE=.\MIM.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mim.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MIM.ico
# End Source File
# Begin Source File

SOURCE=.\res\MIM.rc2
# End Source File
# Begin Source File

SOURCE=.\MIM.rgs
# End Source File
# Begin Source File

SOURCE=.\res\mim_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mute.ico
# End Source File
# Begin Source File

SOURCE=.\res\pass.ico
# End Source File
# Begin Source File

SOURCE=.\res\pencil1.cur
# End Source File
# Begin Source File

SOURCE=.\res\phone.ico
# End Source File
# Begin Source File

SOURCE=.\res\Prev.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pubpad.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Send.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SendFile.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Server.bmp
# End Source File
# Begin Source File

SOURCE=.\res\server.ico
# End Source File
# Begin Source File

SOURCE=.\res\Server2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Settings.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Simple.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SMess.bmp
# End Source File
# Begin Source File

SOURCE=.\res\text.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Tray.bmp
# End Source File
# Begin Source File

SOURCE=.\res\user.ico
# End Source File
# Begin Source File

SOURCE=.\res\ViewProfile.bmp
# End Source File
# Begin Source File

SOURCE=.\res\you.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\EULA.txt
# End Source File
# Begin Source File

SOURCE=.\patch.htm
# End Source File
# Begin Source File

SOURCE=.\Patch.txt
# End Source File
# Begin Source File

SOURCE=.\readme.htm
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\Server.txt
# End Source File
# End Target
# End Project
# Section MIM : {00520045-0052-004F-5200-340030003300}
# 	1:7:IDR_MIM:138
# End Section
