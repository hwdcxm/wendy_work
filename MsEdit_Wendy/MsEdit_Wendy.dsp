# Microsoft Developer Studio Project File - Name="MsEdit_Wendy" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MsEdit_Wendy - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MsEdit_Wendy.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MsEdit_Wendy.mak" CFG="MsEdit_Wendy - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MsEdit_Wendy - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MsEdit_Wendy - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MsEdit_Wendy - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "MsEdit_Wendy - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MsEdit_Wendy - Win32 Release"
# Name "MsEdit_Wendy - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DataSrc_TTFrameFile.cpp
# End Source File
# Begin Source File

SOURCE=.\EINFO.cpp
# End Source File
# Begin Source File

SOURCE=.\Master.cpp
# End Source File
# Begin Source File

SOURCE=.\MsEdit_Wendy.cpp
# End Source File
# Begin Source File

SOURCE=.\MsEdit_Wendy.rc
# End Source File
# Begin Source File

SOURCE=.\MsEdit_WendyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MyProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\PathAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\QServer.cpp
# End Source File
# Begin Source File

SOURCE=.\QServer_Native.cpp

!IF  "$(CFG)" == "MsEdit_Wendy - Win32 Release"

!ELSEIF  "$(CFG)" == "MsEdit_Wendy - Win32 Debug"

# SUBTRACT CPP /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QServer_Remote.cpp
# End Source File
# Begin Source File

SOURCE=.\QSNative_Option.cpp
# End Source File
# Begin Source File

SOURCE=.\Set.cpp
# End Source File
# Begin Source File

SOURCE=.\ShowData.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TTOptionsDataTable.cpp
# End Source File
# Begin Source File

SOURCE=.\Unpacker_TTFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkThread.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Datadef.h
# End Source File
# Begin Source File

SOURCE=.\DataSrc_TTFrameFile.h
# End Source File
# Begin Source File

SOURCE=.\EINFO.h
# End Source File
# Begin Source File

SOURCE=.\lock.h
# End Source File
# Begin Source File

SOURCE=.\Log.cpp
# End Source File
# Begin Source File

SOURCE=.\Log.h
# End Source File
# Begin Source File

SOURCE=.\Master.h
# End Source File
# Begin Source File

SOURCE=.\MsEdit_Wendy.h
# End Source File
# Begin Source File

SOURCE=.\MsEdit_WendyDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyListBox.h
# End Source File
# Begin Source File

SOURCE=.\MyProgress.h
# End Source File
# Begin Source File

SOURCE=.\PathAPI.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\Protocol_SRV.h
# End Source File
# Begin Source File

SOURCE=.\QServer.h
# End Source File
# Begin Source File

SOURCE=.\QServer_Native.h
# End Source File
# Begin Source File

SOURCE=.\QServer_Remote.h
# End Source File
# Begin Source File

SOURCE=.\QSNative_Option.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Set.h
# End Source File
# Begin Source File

SOURCE=.\ShowData.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TTOptionsDataTable.h
# End Source File
# Begin Source File

SOURCE=.\Unpacker_TTFrame.h
# End Source File
# Begin Source File

SOURCE=.\WorkThread.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\IGPEditor_hwd.ico
# End Source File
# Begin Source File

SOURCE=.\res\IGPEditor_hwdDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\MsEdit_Wendy.ico
# End Source File
# Begin Source File

SOURCE=.\res\MsEdit_Wendy.rc2
# End Source File
# Begin Source File

SOURCE=.\res\set1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\set2.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
